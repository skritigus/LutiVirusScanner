#include "FileManager.h"
#include <QDirIterator>

FileManager* FileManager::instance = nullptr;

FileManager::FileManager()
{
    filterManager = FilterManager::getInstance();
}

FileManager::~FileManager()
{
    delete filterManager;
}

FileManager* FileManager::getInstance()
{
    if (!instance)
    {
        instance = new FileManager();
    }

    return instance;
}

QList<QString> FileManager::selectFiles(const QString& path, const bool& isFiltersOn)
{
    QList<QString> files;
    QFileInfo info(path);
    if (info.isFile())
    {
        files.append(info.absoluteFilePath());
        if(isFiltersOn)
        {
            files = filterManager->applyFilters(files, info.fileName());
        }

        return files;
    }

    QDirIterator iterator(path, QDir::Files, QDirIterator::Subdirectories);
    while (iterator.hasNext())
    {
        files.append(iterator.next());
    }

    if(files.empty())
    {
        return files;
    }

    if(isFiltersOn)
    {
        files = filterManager->applyFilters(files, path);
    }
    return files;
}

QString FileManager::encryptApiKey(const QString& key, const QByteArray& secret)
{
    QByteArray data = key.toUtf8();
    QByteArray encrypted;

    for (int i = 0; i < data.size(); ++i)
        encrypted.append(data[i] ^ secret[i % secret.size()]);

    return QString(encrypted.toBase64());
}

QString FileManager::decryptApiKey(const QString& encoded, const QByteArray& secret)
{
    QByteArray data = QByteArray::fromBase64(encoded.toUtf8());
    QByteArray decrypted;

    for (int i = 0; i < data.size(); ++i)
        decrypted.append(data[i] ^ secret[i % secret.size()]);

    return QString::fromUtf8(decrypted);
}


QString FileManager::getAPIKey()
{
    QFile file("key.dat");
    QString apiKey;

    if (file.open(QIODevice::ReadOnly))
    {
        QString encrypted = QString::fromUtf8(file.readAll());
        file.close();
        apiKey = decryptApiKey(encrypted, SECRET_WORD);
    }

    return apiKey;
}

bool FileManager::saveAPIKey(const QString& apiKey)
{
    QFile file("key.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(encryptApiKey(apiKey, SECRET_WORD).toUtf8());
        file.close();
        return true;
    }

    return false;
}
