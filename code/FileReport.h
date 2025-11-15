#ifndef FILEREPORT_H
#define FILEREPORT_H

#include <QMap>
#include <QString>

class FileReport
{
    QString filePath;
    QString status;
    int detectedEngines;
    int totalEngines;
    QMap<QString, QString> detailedData;

public:
    FileReport();
    FileReport(QString status, int detectedEngines, int totalEngines,
               QMap<QString, QString> detailedData, QString filePath);

    QString getStatus() const;
    QString getFilePath() const;
    int getDetectedEngines() const;
    int getTotalEngines() const;
    QMap<QString, QString> getDetailedData() const;

    void setStatus(const QString& status);
    void setFilePath(const QString& filePath);
    void setDetectedEngines(int detectedEngines);
    void setTotalEngines(int totalEngines);
    void setDetailedData(const QMap<QString, QString>& detailedData);
};

#endif // FILEREPORT_H
