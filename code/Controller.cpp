#include "Controller.h"
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QTimer>
#include "JsonParser.h"

Controller* Controller::instance = nullptr;

Controller::Controller()
{
    fileManager = FileManager::getInstance();
    apiKey = fileManager->getAPIKey();
}

Controller::~Controller()
{
    delete fileManager;
}

Controller* Controller::getInstance()
{
    if (!instance)
    {
        instance = new Controller();
    }

    return instance;
}

bool Controller::checkAPIKey()
{
    apiKey = fileManager->getAPIKey();
    return !apiKey.isEmpty();
}

void Controller::postFiles(const QString& path, const bool& isFiltersOn)
{
    if (!checkAPIKey())
    {
        return;
    }

    QList<QString> filePaths = fileManager->selectFiles(path, isFiltersOn);
    if(filePaths.isEmpty())
    {
        emit emptyFolder("Folder is empty, or current filters removed all files for uploading");
    }
    filesCounter = 0;
    expectedFilesCounter = filePaths.count();
    isErrorOccured = false;

    for (const QString& filePath : std::as_const(filePaths))
    {
        QFileInfo info(filePath);

        if (info.size() > 650 * 1024 * 1024)
        {
            emit fileScanned(FileReport("skipped", 0, 0, QMap<QString, QString>(), filePath));
            checkEndOfTransmission();
            continue;
        }
        if (info.size() > 32 * 1024 * 1024)
        {
            uploadLargeFile(filePath);
            continue;
        }

        QFile* file = new QFile(filePath);
        if (!file->open(QIODevice::ReadOnly))
        {
            delete file;
            checkEndOfTransmission();
            continue;
        }

        QNetworkRequest request(QUrl("https://www.virustotal.com/api/v3/files"));
        request.setRawHeader("x-apikey", apiKey.toUtf8());

        QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(*file).fileName() + "\""));
        filePart.setBodyDevice(file);
        file->setParent(multiPart);
        multiPart->append(filePart);

        QNetworkReply* reply = net.post(request, multiPart);
        multiPart->setParent(reply);

        connect(reply, &QNetworkReply::finished, this, [this, filePath](){ onFileUploaded(filePath); });
        connect(reply, &QNetworkReply::errorOccurred, this, &Controller::onNetworkError);
    }
}

void Controller::onFileUploaded(const QString& path)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray data = reply->readAll();
    reply->deleteLater();

    QString analysisId = JsonParser::getAnalysisId(data);

    if (analysisId.isEmpty())
    {
        emit statusChanged(JsonParser::getErrorMessage(data));
        checkEndOfTransmission();
        return;
    }

    getReport(analysisId, path);
}

void Controller::getReport(const QString& analysisId, const QString& path)
{
    QNetworkRequest reportRequest(QUrl("https://www.virustotal.com/api/v3/analyses/" + analysisId));
    reportRequest.setRawHeader("x-apikey", apiKey.toUtf8());

    QNetworkReply* reply = net.get(reportRequest);

    connect(reply, &QNetworkReply::finished, this, [this, path, analysisId](){ onReportReceived(analysisId, path); });
    connect(reply, &QNetworkReply::errorOccurred, this, &Controller::onNetworkError);
}

void Controller::onReportReceived(const QString& analysisId, const QString& path)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray data = reply->readAll();
    reply->deleteLater();

    if (JsonParser::isContainsError(data))
    {
        emit statusChanged(JsonParser::getErrorMessage(data));
        checkEndOfTransmission();
        return;
    }

    QString status = JsonParser::getAnalysisStatus(data);

    if (status == "queued" || status == "in-progress")
    {
        QTimer::singleShot(2000, this, [this, analysisId, path]() {
            getReport(analysisId, path);
        });
        return;
    }

    FileReport report = JsonParser::parseReport(data, path);
    emit fileScanned(report);
    checkEndOfTransmission();
}

void Controller::testAPIKey(const QString& key)
{
    QNetworkRequest request(QUrl("https://www.virustotal.com/api/v3/users/me"));
    request.setRawHeader("x-apikey", key.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = net.get(request);
    connect(reply, &QNetworkReply::finished, this, &Controller::onAPIKeyTestFinished);
}

void Controller::onAPIKeyTestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    reply->deleteLater();

    if (status == 200)
    {
        emit apiKeyTestResult(true, "API Key is correct");
    }
    else
    {
        QString message;
        if (status == 401)
        {
            message = "Error: API Key is incorrect";
        }
        else
        {
            if (status == 403)
            {
                message = "Error: Access denied";
            }
            else
            {
                message = QString("Error: %1").arg(status);
            }
        }

        emit apiKeyTestResult(false, message);
    }
}

void Controller::checkEndOfTransmission()
{
    ++filesCounter;

    if(filesCounter == expectedFilesCounter)
    {
        emit endAnalysing();
    }
}

void Controller::onNetworkError(QNetworkReply::NetworkError code)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    if(!isErrorOccured)
    {
        isErrorOccured = true;
        if (code >= 1 && code <= 8)
        {
            emit errorOccured("Unable to send request. Please check your internet connection and try again.");
        }
        else
        {
            emit errorOccured(reply->errorString());
        }
    }
}

void Controller::uploadLargeFile(const QString& filePath)
{
    QNetworkRequest request(QUrl("https://www.virustotal.com/api/v3/files/upload_url"));
    request.setRawHeader("x-apikey", apiKey.toUtf8());

    QNetworkReply* reply = net.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, filePath]() { onLargeUploadUrlReceived(filePath); });
    connect(reply, &QNetworkReply::errorOccurred, this, &Controller::onNetworkError);
}

void Controller::onLargeUploadUrlReceived(const QString& filePath)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray data = reply->readAll();
    reply->deleteLater();

    QString uploadUrl = JsonParser::getLargeFileUploadUrl(data);
    if (uploadUrl.isEmpty())
    {
        emit errorOccured("Failed to get upload URL for large file");
        checkEndOfTransmission();
        return;
    }

    QFile* file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly))
    {
        delete file;
        checkEndOfTransmission();
        return;
    }

    QNetworkRequest uploadRequest(uploadUrl);
    uploadRequest.setRawHeader("x-apikey", apiKey.toUtf8());

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(*file).fileName() + "\""));

    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(filePart);

    QNetworkReply* uploadReply = net.post(uploadRequest, multiPart);
    multiPart->setParent(uploadReply);

    connect(uploadReply, &QNetworkReply::finished, this, [this, filePath]() { onFileUploaded(filePath); });
    connect(uploadReply, &QNetworkReply::errorOccurred, this, &Controller::onNetworkError);
}
