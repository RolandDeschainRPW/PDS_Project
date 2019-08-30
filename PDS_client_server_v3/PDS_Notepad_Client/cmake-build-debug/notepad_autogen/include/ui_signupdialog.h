/********************************************************************************
** Form generated from reading UI file 'signupdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUPDIALOG_H
#define UI_SIGNUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SignUpDialog
{
public:
    QLineEdit *newUsernameLineEdit;
    QLineEdit *newPasswordLineEdit;
    QLabel *newPasswordLabel;
    QLabel *newUsernameLabel;
    QLabel *statusLabel;
    QLabel *repeatUsernameLabel;
    QLabel *repeatPasswordLabel;
    QLineEdit *repeatUsernameLineEdit;
    QLineEdit *repeatPasswordLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SignUpDialog)
    {
        if (SignUpDialog->objectName().isEmpty())
            SignUpDialog->setObjectName(QString::fromUtf8("SignUpDialog"));
        SignUpDialog->resize(289, 243);
        SignUpDialog->setModal(false);
        newUsernameLineEdit = new QLineEdit(SignUpDialog);
        newUsernameLineEdit->setObjectName(QString::fromUtf8("newUsernameLineEdit"));
        newUsernameLineEdit->setGeometry(QRect(110, 40, 171, 20));
        newPasswordLineEdit = new QLineEdit(SignUpDialog);
        newPasswordLineEdit->setObjectName(QString::fromUtf8("newPasswordLineEdit"));
        newPasswordLineEdit->setGeometry(QRect(110, 80, 171, 20));
        newPasswordLineEdit->setEchoMode(QLineEdit::Password);
        newPasswordLabel = new QLabel(SignUpDialog);
        newPasswordLabel->setObjectName(QString::fromUtf8("newPasswordLabel"));
        newPasswordLabel->setGeometry(QRect(10, 80, 101, 21));
        newUsernameLabel = new QLabel(SignUpDialog);
        newUsernameLabel->setObjectName(QString::fromUtf8("newUsernameLabel"));
        newUsernameLabel->setGeometry(QRect(10, 40, 101, 21));
        statusLabel = new QLabel(SignUpDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(36, 10, 221, 20));
        repeatUsernameLabel = new QLabel(SignUpDialog);
        repeatUsernameLabel->setObjectName(QString::fromUtf8("repeatUsernameLabel"));
        repeatUsernameLabel->setGeometry(QRect(10, 120, 101, 20));
        repeatPasswordLabel = new QLabel(SignUpDialog);
        repeatPasswordLabel->setObjectName(QString::fromUtf8("repeatPasswordLabel"));
        repeatPasswordLabel->setGeometry(QRect(10, 160, 101, 21));
        repeatUsernameLineEdit = new QLineEdit(SignUpDialog);
        repeatUsernameLineEdit->setObjectName(QString::fromUtf8("repeatUsernameLineEdit"));
        repeatUsernameLineEdit->setGeometry(QRect(110, 120, 171, 20));
        repeatPasswordLineEdit = new QLineEdit(SignUpDialog);
        repeatPasswordLineEdit->setObjectName(QString::fromUtf8("repeatPasswordLineEdit"));
        repeatPasswordLineEdit->setGeometry(QRect(110, 160, 171, 20));
        repeatPasswordLineEdit->setEchoMode(QLineEdit::Password);
        okButton = new QPushButton(SignUpDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setEnabled(false);
        okButton->setGeometry(QRect(30, 200, 81, 23));
        cancelButton = new QPushButton(SignUpDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(180, 200, 81, 23));
#if QT_CONFIG(shortcut)
        newPasswordLabel->setBuddy(newPasswordLineEdit);
        newUsernameLabel->setBuddy(newUsernameLineEdit);
        repeatUsernameLabel->setBuddy(repeatUsernameLineEdit);
        repeatPasswordLabel->setBuddy(repeatPasswordLineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(SignUpDialog);

        QMetaObject::connectSlotsByName(SignUpDialog);
    } // setupUi

    void retranslateUi(QDialog *SignUpDialog)
    {
        SignUpDialog->setWindowTitle(QCoreApplication::translate("SignUpDialog", "Sign up", nullptr));
        newPasswordLabel->setText(QCoreApplication::translate("SignUpDialog", "New Password:", nullptr));
        newUsernameLabel->setText(QCoreApplication::translate("SignUpDialog", "New Username:", nullptr));
        statusLabel->setText(QCoreApplication::translate("SignUpDialog", "Insert your new credentials for Signing Up!", nullptr));
        repeatUsernameLabel->setText(QCoreApplication::translate("SignUpDialog", "Repeat Username:", nullptr));
        repeatPasswordLabel->setText(QCoreApplication::translate("SignUpDialog", "Repeat Password:", nullptr));
        okButton->setText(QCoreApplication::translate("SignUpDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("SignUpDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignUpDialog: public Ui_SignUpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUPDIALOG_H
