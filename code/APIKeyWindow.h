#ifndef APIKEYWINDOW_H
#define APIKEYWINDOW_H

#include "Controller.h"
#include <QWidget>

namespace Ui
{
    class APIKeyWindow;
}

class APIKeyWindow : public QWidget
{
    Q_OBJECT

public:
    explicit APIKeyWindow(QWidget *parent = nullptr);
    ~APIKeyWindow();

signals:
    void showMainWindow();
    void closeMainWindow();

public slots:
    void onApiKeyTestResult(bool success, const QString& message);

private slots:
    void saveAPIKey();

private:
    Ui::APIKeyWindow *ui;
    Controller* controller;
    FileManager* fileManager;

    void closeEvent(QCloseEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif // APIKEYWINDOW_H
