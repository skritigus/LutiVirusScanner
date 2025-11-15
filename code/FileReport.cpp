#include "FileReport.h"

FileReport::FileReport() {}

FileReport::FileReport(QString status, int detectedEngines, int totalEngines,
           QMap<QString, QString> detailedData, QString filePath) :
    filePath(filePath),
    status(status),
    detectedEngines(detectedEngines),
    totalEngines(totalEngines),
    detailedData(detailedData){}

QString FileReport::getStatus() const
{
    return status;
}

QString FileReport::getFilePath() const
{
    return filePath;
}

int FileReport::getDetectedEngines() const
{
    return detectedEngines;
}

int FileReport::getTotalEngines() const
{
    return totalEngines;
}

QMap<QString, QString> FileReport::getDetailedData() const
{
    return detailedData;
}

void FileReport::setStatus(const QString& newStatus)
{
    status = newStatus;
}

void FileReport::setFilePath(const QString& newFilePath)
{
    filePath = newFilePath;
}

void FileReport::setDetectedEngines(int newDetectedEngines)
{
    detectedEngines = newDetectedEngines;
}

void FileReport::setTotalEngines(int newTotalEngines)
{
    totalEngines = newTotalEngines;
}

void FileReport::setDetailedData(const QMap<QString, QString>& newDetailedData)
{
    detailedData = newDetailedData;
}
