#include "JsonParser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMap>
#include <QString>

FileReport JsonParser::parseReport(const QByteArray& jsonData, const QString& filePath)
{
    FileReport report;
    auto doc = QJsonDocument::fromJson(jsonData);
    if (!doc.isObject())
    {
        return report;
    }

    report.setFilePath(filePath);

    auto obj = doc.object();
    auto stats = obj["data"].toObject()["attributes"].toObject()["stats"].toObject();
    report.setDetectedEngines(stats["malicious"].toInt());
    if(report.getDetectedEngines() > 0)
    {
        report.setStatus("malicious");
    }
    else
    {
        report.setStatus("undetected");
    }
    report.setTotalEngines(stats["undetected"].toInt() + stats["malicious"].toInt());

    QMap<QString, QString> details;
    auto scans = obj["data"].toObject()["attributes"].toObject()["results"].toObject();
    for (auto it = scans.begin(); it != scans.end(); ++it)
    {
        details[it.key()] = it->toObject()["category"].toString() + "/" + it->toObject()["result"].toString();
    }
    report.setDetailedData(details);
    return report;
}

QByteArray JsonParser::createPostFilesRequest(const QList<QString>& filePaths)
{
    QJsonArray arr;
    for (auto &f : filePaths)
    {
        arr.append(f);
    }
    QJsonDocument doc(arr);
    return doc.toJson();
}

bool JsonParser::saveToFile(const FileReport& fileReport, const QString& filePath)
{
    QJsonObject root;

    root["file_path"] = fileReport.getFilePath();
    root["status"] = fileReport.getStatus();
    root["detected"] = fileReport.getDetectedEngines();
    root["total"] = fileReport.getTotalEngines();

    QJsonObject det;
    QMap<QString, QString> detailedData = fileReport.getDetailedData();
    for (auto it = detailedData.begin(); it != detailedData.end(); ++it)
    {
        det[it.key()] = it.value();
    }

    root["detailed"] = det;

    QJsonDocument doc(root);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

QString JsonParser::getAnalysisId(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc["data"]["id"].toString();
}

QString JsonParser::getErrorMessage(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc["error"]["message"].toString();
}

QString JsonParser::getAnalysisStatus(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc["data"]["attributes"]["status"].toString();
}

bool JsonParser::isContainsError(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object().contains("error");
}

QString JsonParser::getLargeFileUploadUrl(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    return obj["data"].toString();
}
