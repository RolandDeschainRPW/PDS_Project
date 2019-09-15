/********************************************************************************
** Form generated from reading UI file 'modifyprofiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODIFYPROFILEDIALOG_H
#define UI_MODIFYPROFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ModifyProfileDialog
{
public:
    QLineEdit *newPasswordLineEdit;
    QLabel *newPasswordLabel;
    QLabel *statusLabel;
    QLabel *repeatPasswordLabel;
    QLineEdit *repeatPasswordLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *imageStatusLabel;
    QLabel *imageLabel;
    QPushButton *newImageButton;

    void setupUi(QDialog *ModifyProfileDialog)
    {
        if (ModifyProfileDialog->objectName().isEmpty())
            ModifyProfileDialog->setObjectName(QString::fromUtf8("ModifyProfileDialog"));
        ModifyProfileDialog->resize(331, 341);
        ModifyProfileDialog->setModal(false);
        newPasswordLineEdit = new QLineEdit(ModifyProfileDialog);
        newPasswordLineEdit->setObjectName(QString::fromUtf8("newPasswordLineEdit"));
        newPasswordLineEdit->setGeometry(QRect(130, 220, 171, 20));
        newPasswordLineEdit->setEchoMode(QLineEdit::Password);
        newPasswordLabel = new QLabel(ModifyProfileDialog);
        newPasswordLabel->setObjectName(QString::fromUtf8("newPasswordLabel"));
        newPasswordLabel->setGeometry(QRect(30, 220, 101, 21));
        statusLabel = new QLabel(ModifyProfileDialog);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(10, 10, 311, 20));
        repeatPasswordLabel = new QLabel(ModifyProfileDialog);
        repeatPasswordLabel->setObjectName(QString::fromUtf8("repeatPasswordLabel"));
        repeatPasswordLabel->setGeometry(QRect(30, 260, 101, 21));
        repeatPasswordLineEdit = new QLineEdit(ModifyProfileDialog);
        repeatPasswordLineEdit->setObjectName(QString::fromUtf8("repeatPasswordLineEdit"));
        repeatPasswordLineEdit->setGeometry(QRect(130, 260, 171, 20));
        repeatPasswordLineEdit->setEchoMode(QLineEdit::Password);
        okButton = new QPushButton(ModifyProfileDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setEnabled(false);
        okButton->setGeometry(QRect(10, 310, 81, 23));
        cancelButton = new QPushButton(ModifyProfileDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(100, 310, 81, 23));
        imageStatusLabel = new QLabel(ModifyProfileDialog);
        imageStatusLabel->setObjectName(QString::fromUtf8("imageStatusLabel"));
        imageStatusLabel->setGeometry(QRect(130, 40, 71, 21));
        imageLabel = new QLabel(ModifyProfileDialog);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(100, 70, 121, 121));
        imageLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/default_profile_pic.png")));
        imageLabel->setScaledContents(true);
        newImageButton = new QPushButton(ModifyProfileDialog);
        newImageButton->setObjectName(QString::fromUtf8("newImageButton"));
        newImageButton->setGeometry(QRect(190, 310, 131, 23));
#if QT_CONFIG(shortcut)
        newPasswordLabel->setBuddy(newPasswordLineEdit);
        repeatPasswordLabel->setBuddy(repeatPasswordLineEdit);
        imageStatusLabel->setBuddy(imageLabel);
#endif // QT_CONFIG(shortcut)

        retranslateUi(ModifyProfileDialog);

        QMetaObject::connectSlotsByName(ModifyProfileDialog);
    } // setupUi

    void retranslateUi(QDialog *ModifyProfileDialog)
    {
        ModifyProfileDialog->setWindowTitle(QCoreApplication::translate("ModifyProfileDialog", "Modify your profile", nullptr));
        newPasswordLabel->setText(QCoreApplication::translate("ModifyProfileDialog", "New Password:", nullptr));
        statusLabel->setText(QCoreApplication::translate("ModifyProfileDialog", "Here you can modify your profile picture and/or your password.", nullptr));
        repeatPasswordLabel->setText(QCoreApplication::translate("ModifyProfileDialog", "Repeat Password:", nullptr));
        okButton->setText(QCoreApplication::translate("ModifyProfileDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("ModifyProfileDialog", "Cancel", nullptr));
        imageStatusLabel->setText(QCoreApplication::translate("ModifyProfileDialog", "Image profile:", nullptr));
        imageLabel->setText(QString());
        newImageButton->setText(QCoreApplication::translate("ModifyProfileDialog", "Choose new image...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModifyProfileDialog: public Ui_ModifyProfileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODIFYPROFILEDIALOG_H
