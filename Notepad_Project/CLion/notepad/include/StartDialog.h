//
// Created by raffa on 25/08/2019.
//

#ifndef NOTEPAD_STARTDIALOG_H
#define NOTEPAD_STARTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class StartDialog;
}
QT_END_NAMESPACE

class StartDialog : public QDialog {
    Q_OBJECT

public:
    explicit StartDialog(QWidget* parent = nullptr);
    ~StartDialog();

private:
    Ui::StartDialog* ui;
};


#endif //NOTEPAD_STARTDIALOG_H
