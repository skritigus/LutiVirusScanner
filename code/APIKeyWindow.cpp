#include "APIKeyWindow.h"
#include "ui_APIKeyWindow.h"
#include <QMessageBox>
#include <QFile>

APIKeyWindow::APIKeyWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::APIKeyWindow)
{
    ui->setupUi(this);

    controller = Controller::getInstance();
    fileManager = FileManager::getInstance();

    ui->descriptionLabel->setText(ui->descriptionLabel->text()
                                  + "<a href='https://www.virustotal.com/gui/my-apikey'>"
                                  + "on official site</a>");
    ui->descriptionLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->descriptionLabel->setOpenExternalLinks(true);

    connect(ui->saveButton, &QPushButton::clicked, this, &APIKeyWindow::saveAPIKey);
    connect(controller, &Controller::apiKeyTestResult, this, &APIKeyWindow::onApiKeyTestResult);
}

APIKeyWindow::~APIKeyWindow()
{
    delete ui;
}

void APIKeyWindow::saveAPIKey()
{
    QString key = ui->APIKeyLineEdit->text().trimmed();
    if (key.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "API key can't be empty");
        return;
    }

    controller->testAPIKey(key);

    ui->APIKeyLineEdit->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->saveButton->setText("Checking...");
}

void APIKeyWindow::onApiKeyTestResult(bool success, const QString& message)
{
    if (success)
    {
        if (fileManager->saveAPIKey(ui->APIKeyLineEdit->text()))
        {
            emit showMainWindow();
            hide();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Couldn't save key");
        }
    }
    else
    {
        QMessageBox::critical(this, "Error", message);
    }

    ui->APIKeyLineEdit->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->saveButton->setText("Save and continue");
}

void APIKeyWindow::closeEvent(QCloseEvent* event)
{
    emit closeMainWindow();

    QWidget::closeEvent(event);
}

void APIKeyWindow::showEvent(QShowEvent* event)
{
    ui->APIKeyLineEdit->setText(fileManager->getAPIKey());

    QWidget::showEvent(event);
}
