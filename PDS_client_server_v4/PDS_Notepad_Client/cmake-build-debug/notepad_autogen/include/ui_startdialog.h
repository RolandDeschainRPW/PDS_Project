/********************************************************************************
** Form generated from reading UI file 'startdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARTDIALOG_H
#define UI_STARTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_StartDialog
{
public:
    QComboBox *hostCombo;
    QPushButton *connectToServerButton;
    QPushButton *quitButton;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QLabel *hostLabel;
    QLabel *portLabel;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLabel *usernameLabel;
    QLabel *passwordLabel;
    QPushButton *signUpButton;

    void setupUi(QDialog *StartDialog)
    {
        if (StartDialog->objectName().isEmpty())
            StartDialog->setObjectName(QString::fromUtf8("StartDialog"));
        StartDialog->resize(372, 193);
        hostCombo = new QComboBox(StartDialog);
        hostCombo->setObjectName(QString::fromUtf8("hostCombo"));
        hostCombo->setGeometry(QRect(100, 10, 261, 22));
        hostCombo->setEditable(true);
        connectToServerButton = new QPushButton(StartDialog);
        connectToServerButton->setObjectName(QString::fromUtf8("connectToServerButton"));
        connectToServerButton->setEnabled(false);
        connectToServerButton->setGeometry(QRect(140, 160, 121, 23));
        quitButton = new QPushButton(StartDialog);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(270, 160, 81, 23));
        portLineEdit = new QLineEdit(StartDialog);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));
        portLineEdit->setGeometry(QRect(100, 40, 261, 20));
        statusLabel = new QLabel(StartDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(10, 130, 291, 20));
        hostLabel = new QLabel(StartDialog);
        hostLabel->setObjectName(QString::fromUtf8("hostLabel"));
        hostLabel->setGeometry(QRect(10, 10, 91, 21));
        portLabel = new QLabel(StartDialog);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setGeometry(QRect(10, 40, 91, 20));
        usernameLineEdit = new QLineEdit(StartDialog);
        usernameLineEdit->setObjectName(QString::fromUtf8("usernameLineEdit"));
        usernameLineEdit->setGeometry(QRect(100, 70, 261, 20));
        passwordLineEdit = new QLineEdit(StartDialog);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(100, 100, 261, 20));
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        usernameLabel = new QLabel(StartDialog);
        usernameLabel->setObjectName(QString::fromUtf8("usernameLabel"));
        usernameLabel->setGeometry(QRect(10, 70, 91, 21));
        passwordLabel = new QLabel(StartDialog);
        passwordLabel->setObjectName(QString::fromUtf8("passwordLabel"));
        passwordLabel->setGeometry(QRect(10, 100, 91, 21));
        signUpButton = new QPushButton(StartDialog);
        signUpButton->setObjectName(QString::fromUtf8("signUpButton"));
        signUpButton->setEnabled(false);
        signUpButton->setGeometry(QRect(40, 160, 91, 23));
#if QT_CONFIG(shortcut)
        hostLabel->setBuddy(hostCombo);
        portLabel->setBuddy(portLineEdit);
#endif // QT_CONFIG(shortcut)

        retranslateUi(StartDialog);

        connectToServerButton->setDefault(true);


        QMetaObject::connectSlotsByName(StartDialog);
    } // setupUi

    void retranslateUi(QDialog *StartDialog)
    {
        StartDialog->setWindowTitle(QCoreApplication::translate("StartDialog", "Shared Text Editor", nullptr));
        connectToServerButton->setText(QCoreApplication::translate("StartDialog", "Connect to Server", nullptr));
        quitButton->setText(QCoreApplication::translate("StartDialog", "Quit", nullptr));
        statusLabel->setText(QCoreApplication::translate("StartDialog", "Insert the requested data to be able to connect to Server.", nullptr));
        hostLabel->setText(QCoreApplication::translate("StartDialog", "Server name:", nullptr));
        portLabel->setText(QCoreApplication::translate("StartDialog", "Server port:", nullptr));
        usernameLabel->setText(QCoreApplication::translate("StartDialog", "Username:", nullptr));
        passwordLabel->setText(QCoreApplication::translate("StartDialog", "Password", nullptr));
        signUpButton->setText(QCoreApplication::translate("StartDialog", "Sign Up", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartDialog: public Ui_StartDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTDIALOG_H
