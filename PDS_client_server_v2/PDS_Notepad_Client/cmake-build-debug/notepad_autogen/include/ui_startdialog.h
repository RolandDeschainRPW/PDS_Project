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

    void setupUi(QDialog *StartDialog)
    {
        if (StartDialog->objectName().isEmpty())
            StartDialog->setObjectName(QString::fromUtf8("StartDialog"));
        StartDialog->resize(402, 162);
        hostCombo = new QComboBox(StartDialog);
        hostCombo->setObjectName(QString::fromUtf8("hostCombo"));
        hostCombo->setGeometry(QRect(110, 20, 261, 22));
        hostCombo->setEditable(true);
        connectToServerButton = new QPushButton(StartDialog);
        connectToServerButton->setObjectName(QString::fromUtf8("connectToServerButton"));
        connectToServerButton->setGeometry(QRect(170, 120, 111, 23));
        quitButton = new QPushButton(StartDialog);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(300, 120, 75, 23));
        portLineEdit = new QLineEdit(StartDialog);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));
        portLineEdit->setGeometry(QRect(110, 60, 261, 20));
        statusLabel = new QLabel(StartDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(20, 90, 341, 20));
        hostLabel = new QLabel(StartDialog);
        hostLabel->setObjectName(QString::fromUtf8("hostLabel"));
        hostLabel->setGeometry(QRect(30, 20, 71, 21));
        portLabel = new QLabel(StartDialog);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setGeometry(QRect(30, 60, 71, 20));
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
        StartDialog->setWindowTitle(QCoreApplication::translate("StartDialog", "SharedEditor", nullptr));
        connectToServerButton->setText(QCoreApplication::translate("StartDialog", "Connect to Server", nullptr));
        quitButton->setText(QCoreApplication::translate("StartDialog", "Quit", nullptr));
        statusLabel->setText(QCoreApplication::translate("StartDialog", "Insert Server name and Server port to be able to connect to Server.", nullptr));
        hostLabel->setText(QCoreApplication::translate("StartDialog", "Server name:", nullptr));
        portLabel->setText(QCoreApplication::translate("StartDialog", "Server port:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StartDialog: public Ui_StartDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARTDIALOG_H
