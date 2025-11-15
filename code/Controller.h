#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "FileReport.h"
#include "FileManager.h"
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class Controller : public QObject
{
    Q_OBJECT

public:
    ~Controller();

    static Controller* getInstance();
    bool checkAPIKey();
    void postFiles(const QString& path, const bool& isFiltersOn);
    QList<FileReport> getReports();
    void testAPIKey(const QString& key);

signals:
    void fileScanned(const FileReport& report);
    void apiKeyTestResult(bool success, const QString& message);
    void statusChanged(QString message);
    void emptyFolder(QString message);
    void endAnalysing();
    void errorOccured(QString message);

private slots:
    void onFileUploaded(const QString& path);
    void onReportReceived(const QString& analysisId, const QString& path);
    void onAPIKeyTestFinished();
    void onNetworkError(QNetworkReply::NetworkError code);

private:
    explicit Controller();
    void getReport(const QString& analysisId, const QString& path);
    void scheduleNextReportRequest(const QString& analysisId);
    void checkEndOfTransmission();
    void uploadLargeFile(const QString& filePath);
    void onLargeUploadUrlReceived(const QString& filePath);

private:
    static Controller* instance;
    QString apiKey;
    QNetworkAccessManager net;
    FileManager* fileManager;
    int filesCounter = 0;
    int expectedFilesCounter = 0;
    bool isErrorOccured = false;
};

#endif // CONTROLLER_H
