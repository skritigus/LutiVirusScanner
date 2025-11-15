#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QProgressDialog>

class ProgressDialog : public QProgressDialog
{
public:
    ProgressDialog(QWidget* parent = nullptr);
    ProgressDialog(const QString& labelText, const QString& cancelButtonText,
                   int minimum, int maximum, QWidget* parent = nullptr,
                   Qt::WindowFlags f = Qt::WindowFlags());

private:
    void keyPressEvent(QKeyEvent* event) override;
};

#endif // PROGRESSDIALOG_H
