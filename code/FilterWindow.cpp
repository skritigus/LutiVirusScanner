#include "FilterWindow.h"
#include "ui_FilterWindow.h"
#include <QFile>
#include <QMessageBox>

FilterWindow::FilterWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FilterWindow)
{
    ui->setupUi(this);

    helpWindow = new HelpWindow();

    QFile file(FILE_NAME);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        ui->filtersTextEdit->setText(in.readAll());

        file.close();
    }

    connect(ui->confirmButton, &QPushButton::clicked, this, &FilterWindow::saveNewFilters);
    connect(ui->helpButton, &QPushButton::clicked, this, &FilterWindow::showHelp);
}

FilterWindow::~FilterWindow()
{
    delete helpWindow;
    delete ui;
}

void FilterWindow::showHelp()
{
    if(helpWindow->loadHelpInfo("Filters help.txt", "Filters Help"))
    {
        helpWindow->show();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Failed to load filters help");
    }
}

void FilterWindow::saveNewFilters()
{
    QFile file(FILE_NAME);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Failed to save new filters to file");
        return;
    }

    QTextStream out(&file);

    out << ui->filtersTextEdit->toPlainText();

    file.close();

    hide();
}
