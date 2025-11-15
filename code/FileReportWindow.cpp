#include "FileReportWindow.h"
#include "JsonParser.h"
#include "ui_FileReportWindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

FileReportWindow::FileReportWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileReportWindow)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &FileReportWindow::saveReport);
    connect(ui->okButton, &QPushButton::clicked, this, &QWidget::close);
}

FileReportWindow::~FileReportWindow()
{
    delete ui;
}

void FileReportWindow::setReport(const FileReport& newFileReport)
{
    fileReport = newFileReport;

    QFileInfo fileInfo(fileReport.getFilePath());
    ui->fileNameLabel->setText("File: " + fileInfo.fileName());

    ui->detectionsLabel->setText(QString("Detections: %1/%2").arg(fileReport.getDetectedEngines()).arg(fileReport.getTotalEngines()));

    auto details = fileReport.getDetailedData();

    ui->vendorTableWidget->setRowCount(details.size());

    int row = 0;
    for (auto it = details.begin(); it != details.end(); ++it)
    {
        QString vendor = it.key();
        QString value  = it.value();

        QString category, result;

        int slashIndex = value.indexOf('/');

        category = value.left(slashIndex);
        result = value.mid(slashIndex + 1);

        QString displayResult;

        if (category == "undetected")
        {
            displayResult = "undetected";
        }
        else if (!result.isEmpty())
        {
            displayResult = result;
        }
        else
        {
            displayResult = category;
        }

        QTableWidgetItem* vendorItem = new QTableWidgetItem(vendor);
        QTableWidgetItem* resultItem = new QTableWidgetItem(displayResult);

        if (category == "malicious")
            resultItem->setForeground(Qt::red);
        else if (category == "undetected")
            resultItem->setForeground(Qt::darkGreen);
        else
            resultItem->setForeground(Qt::gray);

        ui->vendorTableWidget->setItem(row, 0, vendorItem);
        ui->vendorTableWidget->setItem(row, 1, resultItem);

        ++row;
    }

    ui->vendorTableWidget->resizeColumnsToContents();
}

void FileReportWindow::saveReport()
{
    QFileInfo fi(fileReport.getFilePath());
    QString suggestedName = fi.completeBaseName() + "Report.json";
    QString savePath = QFileDialog::getSaveFileName(this, "Save File Report",
        suggestedName, "JSON Report (*.json)");

    if (savePath.isEmpty())
    {
        return;
    }

    if (!savePath.endsWith(".json", Qt::CaseInsensitive))
    {
        savePath += ".json";
    }

    if(!JsonParser::saveToFile(fileReport, savePath))
    {
        QMessageBox::critical(this, "Error", "Could not save report.");
    }
    else
    {
        QMessageBox::information(this, "Success", "Report saved successfully.");
    }
}
