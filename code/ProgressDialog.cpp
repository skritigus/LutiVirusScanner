#include "ProgressDialog.h"
#include <QKeyEvent>

ProgressDialog::ProgressDialog(QWidget* parent) : QProgressDialog(parent) {}

ProgressDialog::ProgressDialog(const QString& labelText, const QString& cancelButtonText,
                               int minimum, int maximum, QWidget* parent,
                               Qt::WindowFlags f) : QProgressDialog(labelText, cancelButtonText,
                                                                    minimum, maximum, parent, f)
{
    setWindowModality(Qt::WindowModal);
    setCancelButton(nullptr);
    setAutoClose(false);
    setAutoReset(false);
}

void ProgressDialog::keyPressEvent(QKeyEvent* event)
{
    if(event->key() != Qt::Key_Escape)
    {
        QProgressDialog::keyPressEvent(event);
    }
}
