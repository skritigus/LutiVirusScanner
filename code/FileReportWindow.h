#ifndef FILEREPORTWINDOW_H
#define FILEREPORTWINDOW_H

#include "FileReport.h"
#include <QDialog>

namespace Ui {
class FileReportWindow;
}

class FileReportWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileReportWindow(QWidget *parent = nullptr);
    ~FileReportWindow();

    void setReport(const FileReport& newFileReport);

private:
    Ui::FileReportWindow* ui;
    FileReport fileReport;

    void saveReport();
};

#endif // FILEREPORTWINDOW_H
