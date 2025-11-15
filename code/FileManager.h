#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "FilterManager.h"
#include <QList>
#include <QString>

#define SECRET_WORD "LutiVirusScanner"

class FileManager
{
    static FileManager* instance;
    FilterManager* filterManager;

    FileManager();

public:
    ~FileManager();
    static FileManager* getInstance();
    QList<QString> selectFiles(const QString& path, const bool& isFiltersOn);
    QString getAPIKey();
    bool saveAPIKey(const QString& apiKey);
    QString encryptApiKey(const QString& key, const QByteArray& secret);
    QString decryptApiKey(const QString& encoded, const QByteArray& secret);
};

#endif // FILEMANAGER_H
