#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "APIKeyWindow.h"
#include "Controller.h"
#include "FileReportWindow.h"
#include "FilterWindow.h"
#include "ProgressDialog.h"
#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void showWarning(QString message);
    void showError(QString message);
    void showWindow();

private slots:
    void onChooseFileClicked();
    void onChooseFolderClicked();
    void onFileItemDoubleClicked(QTreeWidgetItem* item, int);
    void changeApiKey();
    void showHelp();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    QMap<QString, QTreeWidgetItem*> treeItems;
    QString baseDir;

    Controller* controller;
    APIKeyWindow* apiKeyWindow;
    FilterWindow* filterWindow;
    FileReportWindow* fileReportWindow;
    HelpWindow* helpWindow;
    ProgressDialog* progressDialog;
    QProgressBar* progressBar;

    void clearReports();
    void addFile(const FileReport& report);

    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
