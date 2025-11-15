#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "FileReport.h"

class JsonParser
{
public:
    static FileReport parseReport(const QByteArray& jsonData, const QString& filePath);
    static QByteArray createPostFilesRequest(const QList<QString>& filePaths);
    static bool saveToFile(const FileReport& fileReport, const QString& filePath);

    static QString getAnalysisId(const QByteArray& data);
    static QString getErrorMessage(const QByteArray& data);
    static QString getAnalysisStatus(const QByteArray& data);
    static QString getLargeFileUploadUrl(const QByteArray& data);
    static bool isContainsError(const QByteArray& data);
};

#endif // JSONPARSER_H
