#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include "HelpWindow.h"
#include <QDialog>

#define FILE_NAME "filters.txt"

namespace Ui
{
    class FilterWindow;
}

class FilterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget* parent = nullptr);
    ~FilterWindow();

private:
    Ui::FilterWindow* ui;
    HelpWindow* helpWindow;

    void saveNewFilters();
    void showHelp();
};

#endif // FILTERWINDOW_H
