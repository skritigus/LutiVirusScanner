#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include <QList>
#include <QString>

enum FilterType { Extension, Directory, FileName };

struct FilterRule
{
    FilterType type;
    QString value;
};

class FilterManager
{
    QList<FilterRule> filters;
    static FilterManager* instance;

    FilterManager() = default;

public:
    static FilterManager* getInstance();

    bool uploadFiltersFromFile();
    QList<QString> applyFilters(const QList<QString>& files, const QString& baseDir);
};

#endif // FILTERMANAGER_H
