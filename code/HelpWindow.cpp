#include "HelpWindow.h"
#include "ui_HelpWindow.h"
#include <QFile>

HelpWindow::HelpWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

bool HelpWindow::loadHelpInfo(const QString& fileName, const QString& windowTitle)
{
    setWindowTitle(windowTitle);

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    else
    {
        QTextStream in(&file);
        ui->helpInfo->setText(in.readAll());

        file.close();
        return true;
    }
}
