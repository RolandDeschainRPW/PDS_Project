/********************************************************************************
** Form generated from reading UI file 'shareddocumentsexplorer.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREDDOCUMENTSEXPLORER_H
#define UI_SHAREDDOCUMENTSEXPLORER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_SharedDocumentsExplorer
{
public:
    QPushButton *newDocumentButton;
    QPushButton *openDocumentButton;
    QPushButton *cancelButton;
    QTableView *documentsView;
    QLabel *statusLabel;
    QPushButton *addCollaboratorButton;
    QPushButton *modifyProfileButton;

    void setupUi(QDialog *SharedDocumentsExplorer)
    {
        if (SharedDocumentsExplorer->objectName().isEmpty())
            SharedDocumentsExplorer->setObjectName(QString::fromUtf8("SharedDocumentsExplorer"));
        SharedDocumentsExplorer->resize(581, 286);
        newDocumentButton = new QPushButton(SharedDocumentsExplorer);
        newDocumentButton->setObjectName(QString::fromUtf8("newDocumentButton"));
        newDocumentButton->setGeometry(QRect(10, 250, 121, 23));
        openDocumentButton = new QPushButton(SharedDocumentsExplorer);
        openDocumentButton->setObjectName(QString::fromUtf8("openDocumentButton"));
        openDocumentButton->setGeometry(QRect(140, 250, 101, 23));
        cancelButton = new QPushButton(SharedDocumentsExplorer);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(500, 250, 71, 23));
        documentsView = new QTableView(SharedDocumentsExplorer);
        documentsView->setObjectName(QString::fromUtf8("documentsView"));
        documentsView->setGeometry(QRect(10, 40, 561, 192));
        statusLabel = new QLabel(SharedDocumentsExplorer);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(10, 10, 231, 21));
        addCollaboratorButton = new QPushButton(SharedDocumentsExplorer);
        addCollaboratorButton->setObjectName(QString::fromUtf8("addCollaboratorButton"));
        addCollaboratorButton->setGeometry(QRect(250, 250, 101, 23));
        modifyProfileButton = new QPushButton(SharedDocumentsExplorer);
        modifyProfileButton->setObjectName(QString::fromUtf8("modifyProfileButton"));
        modifyProfileButton->setGeometry(QRect(360, 250, 131, 23));

        retranslateUi(SharedDocumentsExplorer);

        QMetaObject::connectSlotsByName(SharedDocumentsExplorer);
    } // setupUi

    void retranslateUi(QDialog *SharedDocumentsExplorer)
    {
        SharedDocumentsExplorer->setWindowTitle(QCoreApplication::translate("SharedDocumentsExplorer", "Shared Documents Explorer", nullptr));
        newDocumentButton->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Create new document", nullptr));
        openDocumentButton->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Open document", nullptr));
        cancelButton->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Cancel", nullptr));
        statusLabel->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Select the document you want to open.", nullptr));
        addCollaboratorButton->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Add collaborator", nullptr));
        modifyProfileButton->setText(QCoreApplication::translate("SharedDocumentsExplorer", "Modify your profile...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SharedDocumentsExplorer: public Ui_SharedDocumentsExplorer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREDDOCUMENTSEXPLORER_H
