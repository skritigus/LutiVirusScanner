#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    controller = Controller::getInstance();
    apiKeyWindow = new APIKeyWindow();
    filterWindow = new FilterWindow(this);
    fileReportWindow = new FileReportWindow();
    helpWindow = new HelpWindow();

    progressDialog = new ProgressDialog(this);
    progressBar = progressDialog->findChild<QProgressBar*>();

    progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    progressDialog->setParent(this);
    progressDialog->setModal(false);
    progressDialog->setMinimum(0);
    progressDialog->setMaximum(0);
    progressDialog->setCancelButton(nullptr);
    progressDialog->setSizeGripEnabled(false);
    progressDialog->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint
                                   | Qt::MSWindowsFixedSizeDialogHint);
    progressDialog->setWindowTitle("Loading");
    progressDialog->setLabelText("Files are being transmitted to server");
    progressBar->setMinimumWidth(progressDialog->width() + 50);
    progressDialog->close();

    connect(apiKeyWindow, &APIKeyWindow::showMainWindow, this, &MainWindow::show);
    connect(apiKeyWindow, &APIKeyWindow::closeMainWindow, this, &MainWindow::close);

    ui->fileWidget->header()->setSectionsClickable(true);
    ui->fileWidget->header()->setSortIndicatorShown(true);

    if(!controller->checkAPIKey())
    {
        changeApiKey();
    }
    else
    {
        show();
    }

    connect(ui->fileChooseButton, &QPushButton::clicked, this, &MainWindow::onChooseFileClicked);
    connect(ui->chooseFileAction, &QAction::triggered, this, &MainWindow::onChooseFileClicked);
    connect(ui->folderChooseButton, &QPushButton::clicked, this, &MainWindow::onChooseFolderClicked);
    connect(ui->chooseFolderAction, &QAction::triggered, this, &MainWindow::onChooseFolderClicked);

    connect(ui->filterButton, &QPushButton::clicked, filterWindow, &FilterWindow::show);
    connect(ui->changeFiltersAction, &QAction::triggered, filterWindow, &FilterWindow::show);

    connect(ui->changeApiKeyAction, &QAction::triggered, this, &MainWindow::changeApiKey);
    connect(ui->helpAction, &QAction::triggered, this, &MainWindow::showHelp);
    connect(controller, &Controller::fileScanned, this, &MainWindow::addFile);

    connect(ui->fileWidget, &QTreeWidget::itemDoubleClicked, this, &MainWindow::onFileItemDoubleClicked);
    connect(controller, &Controller::emptyFolder, this, &MainWindow::showWarning);
    connect(controller, &Controller::errorOccured, this, &MainWindow::showError);
    connect(controller, &Controller::endAnalysing, progressDialog, &QProgressDialog::close);
}

MainWindow::~MainWindow()
{
    delete controller;
    delete fileReportWindow;
    delete filterWindow;
    delete apiKeyWindow;
    delete helpWindow;
    delete progressBar;
    delete progressDialog;
    delete ui;
}

void MainWindow::changeApiKey()
{
    apiKeyWindow->show();
    hide();
}

void MainWindow::showHelp()
{
    if(helpWindow->loadHelpInfo("Help.txt", "Help"))
    {
        helpWindow->show();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to load help information");
    }
}

void MainWindow::onChooseFileClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Choose file");
    if (!file.isEmpty())
    {
        clearReports();
        baseDir.clear();

        progressDialog->show();
        controller->postFiles(file, ui->activateFiltersAction->isChecked());
    }
}

void MainWindow::onChooseFolderClicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Choose folder");
    if (!dir.isEmpty())
    {
        clearReports();
        baseDir = QDir::toNativeSeparators(dir);

        progressDialog->show();
        controller->postFiles(dir, ui->activateFiltersAction->isChecked());   
    }
}

void MainWindow::clearReports()
{
    ui->fileWidget->clear();
    treeItems.clear();
}

void MainWindow::onFileItemDoubleClicked(QTreeWidgetItem* item, int)
{
    FileReport fileReport;
    if(item->data(0, Qt::UserRole).canConvert<FileReport>())
    {
        fileReport = item->data(0, Qt::UserRole).value<FileReport>();
    }
    else
    {
        return;
    }

    fileReportWindow->setReport(fileReport);
    fileReportWindow->show();
}

void MainWindow::addFile(const FileReport& report)
{
    QFileInfo fileInfo(report.getFilePath());
    QString relPath;
    QStringList parts;

    if(!baseDir.isEmpty())
    {
        QString absPath = QDir::fromNativeSeparators(fileInfo.absoluteFilePath());

        relPath = QDir(baseDir).relativeFilePath(absPath);
        relPath = QDir::fromNativeSeparators(relPath);

        parts = relPath.split('/', Qt::SkipEmptyParts);
    }
    else
    {
        parts.append(fileInfo.fileName());
    }

    QTreeWidgetItem* rootItem = nullptr;

    if (!treeItems.contains(baseDir) && !baseDir.isEmpty())
    {
        rootItem = new QTreeWidgetItem();
        rootItem->setText(0, QFileInfo(baseDir).fileName());
        rootItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));

        ui->fileWidget->addTopLevelItem(rootItem);
        treeItems.insert(baseDir, rootItem);
    }
    else
    {
        rootItem = treeItems.value(baseDir, nullptr);
    }

    QTreeWidgetItem* parentItem = rootItem;
    QString currentPath = baseDir;

    for (const QString& name : std::as_const(parts))
    {
        currentPath += "/" + name;

        QTreeWidgetItem* item = nullptr;

        if(!treeItems.contains(currentPath))
        {
            item = new QTreeWidgetItem();
            item->setText(0, name);
            treeItems.insert(currentPath, item);
        }
        else
        {
            item = treeItems.value(currentPath);
        }

        QFileInfo info(currentPath);

        if (info.isDir())
        {
            item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
            item->setExpanded(true);
        }
        else
        {
            item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));
            item->setText(1, report.getStatus());
            item->setText(2, QString("%1/%2")
                                 .arg(report.getDetectedEngines())
                                 .arg(report.getTotalEngines()));

            item->setData(0, Qt::UserRole, QVariant::fromValue(report));

            if (report.getStatus() == "malicious")
            {
                item->setForeground(1, QBrush(Qt::red));
            }
            else
            {
                if (report.getStatus() == "undetected")
                {
                    item->setForeground(1, QBrush(Qt::darkGreen));
                }
                else
                {
                    item->setForeground(1, QBrush(Qt::gray));
                    item->setText(2, QString(""));
                }
            }
        }

        if (parentItem)
        {
            parentItem->addChild(item);
        }
        else
        {
            ui->fileWidget->addTopLevelItem(item);
        }

        parentItem = item;
    }

    if (parentItem)
    {
        parentItem->setExpanded(true);
    }
    ui->fileWidget->expandAll();
}

void MainWindow::showWindow()
{
    setWindowFlags(Qt::Window);
    setWindowOpacity(1.0);
    show();
}

void MainWindow::showWarning(QString message)
{
    QMessageBox::warning(this, "Warning", message);
    progressDialog->close();
}

void MainWindow::showError(QString message)
{
    QMessageBox::critical(this, "Error", message);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QMainWindow::closeEvent(event);
   // QApplication::quit();
}
