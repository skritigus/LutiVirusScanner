#include "FilterManager.h"
#include <QFileInfo>
#include <QDir>

FilterManager* FilterManager::instance = nullptr;

FilterManager* FilterManager::getInstance()
{
    if (!instance)
    {
        instance = new FilterManager();
    }

    return instance;
}

QList<QString> FilterManager::applyFilters(const QList<QString>& files, const QString& baseDir)
{
    QList<QString> result;

    if(!uploadFiltersFromFile())
    {
        return files;
    }

    for (const QString& file : files)
    {
        QFileInfo info(file);
        QString name = info.completeBaseName().toLower();
        QString ext = info.suffix().toLower();
        QString relPath = QDir(baseDir).relativeFilePath(info.absoluteFilePath());
        relPath = QDir::fromNativeSeparators(relPath).toLower();

        bool excluded = false;

        for (const FilterRule& rule : std::as_const(filters))
        {
            switch (rule.type)
            {
            case FileName:
                if (name == rule.value)
                {
                    excluded = true;
                }
                break;

            case Directory:
                if (relPath.contains("/" + rule.value + "/"))
                {
                    excluded = true;
                }
                break;

            case Extension:
                if (ext == rule.value)
                {
                    excluded = true;
                }
                break;
            }

            if (excluded)
            {
                break;
            }
        }

        if (!excluded)
        {
            result.append(file);
        }
    }

    return result;
}

bool FilterManager::uploadFiltersFromFile()
{
    QFile file("filters.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    filters.clear();

    QTextStream inputStream(&file);

    while (!inputStream.atEnd())
    {
        QString line = inputStream.readLine().trimmed().toLower();

        if (line.isEmpty() || line.startsWith('#'))
        {
            continue;
        }

        if (line.startsWith("*."))
        {
            filters.append({Extension, line.mid(2)});
        }
        else
        {
            if (line.endsWith('/'))
            {
                filters.append({Directory, line.left(line.length() - 1)});
            }
            else
            {
                filters.append({FileName, line});
            }
        }
    }

    file.close();
    return true;
}
