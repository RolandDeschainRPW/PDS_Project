/********************************************************************************
** Form generated from reading UI file 'notepad.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTEPAD_H
#define UI_NOTEPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyTextEdit.h"

QT_BEGIN_NAMESPACE

class Ui_Notepad
{
public:
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QAction *actionUndo;
    QAction *actionRedo;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    MyTextEdit *textEdit;
    QWidget *collaboratorsWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *collaboratorLayout1;
    QLabel *collaboratorPicLabel1;
    QLabel *nicknameLabel1;
    QHBoxLayout *collaboratorLayout2;
    QLabel *collaboratorPicLabel2;
    QLabel *nicknameLabel2;
    QHBoxLayout *collaboratorLayout3;
    QLabel *collaboratorPicLabel3;
    QLabel *nicknameLabel3;
    QHBoxLayout *collaboratorLayout4;
    QLabel *collaboratorPicLabel4;
    QLabel *nicknameLabel4;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Notepad)
    {
        if (Notepad->objectName().isEmpty())
            Notepad->setObjectName(QString::fromUtf8("Notepad"));
        Notepad->resize(1052, 737);
        actionPrint = new QAction(Notepad);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon);
        actionExit = new QAction(Notepad);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon1;
        QString iconThemeName = QString::fromUtf8("exit.png");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8(":/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionExit->setIcon(icon1);
        actionCopy = new QAction(Notepad);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon2);
        actionCut = new QAction(Notepad);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon3);
        actionPaste = new QAction(Notepad);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon4);
        actionUndo = new QAction(Notepad);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/edit_undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon5);
        actionRedo = new QAction(Notepad);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/edit_redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon6);
        centralWidget = new QWidget(Notepad);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textEdit = new MyTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        horizontalLayout->addWidget(textEdit);

        collaboratorsWidget = new QWidget(centralWidget);
        collaboratorsWidget->setObjectName(QString::fromUtf8("collaboratorsWidget"));
        verticalLayout = new QVBoxLayout(collaboratorsWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        collaboratorLayout1 = new QHBoxLayout();
        collaboratorLayout1->setSpacing(6);
        collaboratorLayout1->setObjectName(QString::fromUtf8("collaboratorLayout1"));
        collaboratorPicLabel1 = new QLabel(collaboratorsWidget);
        collaboratorPicLabel1->setObjectName(QString::fromUtf8("collaboratorPicLabel1"));
        collaboratorPicLabel1->setMaximumSize(QSize(100, 100));
        collaboratorPicLabel1->setPixmap(QPixmap(QString::fromUtf8(":/images/default_profile_pic.png")));
        collaboratorPicLabel1->setScaledContents(true);

        collaboratorLayout1->addWidget(collaboratorPicLabel1);

        nicknameLabel1 = new QLabel(collaboratorsWidget);
        nicknameLabel1->setObjectName(QString::fromUtf8("nicknameLabel1"));
        nicknameLabel1->setMaximumSize(QSize(16777215, 35));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        QBrush brush2(QColor(120, 120, 120, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        nicknameLabel1->setPalette(palette);
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        nicknameLabel1->setFont(font);
        nicknameLabel1->setAutoFillBackground(true);

        collaboratorLayout1->addWidget(nicknameLabel1);


        verticalLayout->addLayout(collaboratorLayout1);

        collaboratorLayout2 = new QHBoxLayout();
        collaboratorLayout2->setSpacing(6);
        collaboratorLayout2->setObjectName(QString::fromUtf8("collaboratorLayout2"));
        collaboratorPicLabel2 = new QLabel(collaboratorsWidget);
        collaboratorPicLabel2->setObjectName(QString::fromUtf8("collaboratorPicLabel2"));
        collaboratorPicLabel2->setMaximumSize(QSize(100, 100));
        collaboratorPicLabel2->setPixmap(QPixmap(QString::fromUtf8(":/images/default_profile_pic.png")));
        collaboratorPicLabel2->setScaledContents(true);

        collaboratorLayout2->addWidget(collaboratorPicLabel2);

        nicknameLabel2 = new QLabel(collaboratorsWidget);
        nicknameLabel2->setObjectName(QString::fromUtf8("nicknameLabel2"));
        nicknameLabel2->setMaximumSize(QSize(16777215, 35));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush3(QColor(0, 255, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        nicknameLabel2->setPalette(palette1);
        nicknameLabel2->setFont(font);
        nicknameLabel2->setAutoFillBackground(true);

        collaboratorLayout2->addWidget(nicknameLabel2);


        verticalLayout->addLayout(collaboratorLayout2);

        collaboratorLayout3 = new QHBoxLayout();
        collaboratorLayout3->setSpacing(6);
        collaboratorLayout3->setObjectName(QString::fromUtf8("collaboratorLayout3"));
        collaboratorPicLabel3 = new QLabel(collaboratorsWidget);
        collaboratorPicLabel3->setObjectName(QString::fromUtf8("collaboratorPicLabel3"));
        collaboratorPicLabel3->setMaximumSize(QSize(100, 100));
        collaboratorPicLabel3->setPixmap(QPixmap(QString::fromUtf8(":/images/default_profile_pic.png")));
        collaboratorPicLabel3->setScaledContents(true);

        collaboratorLayout3->addWidget(collaboratorPicLabel3);

        nicknameLabel3 = new QLabel(collaboratorsWidget);
        nicknameLabel3->setObjectName(QString::fromUtf8("nicknameLabel3"));
        nicknameLabel3->setMaximumSize(QSize(16777215, 35));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush4(QColor(0, 0, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush4);
        nicknameLabel3->setPalette(palette2);
        nicknameLabel3->setFont(font);
        nicknameLabel3->setAutoFillBackground(true);

        collaboratorLayout3->addWidget(nicknameLabel3);


        verticalLayout->addLayout(collaboratorLayout3);

        collaboratorLayout4 = new QHBoxLayout();
        collaboratorLayout4->setSpacing(6);
        collaboratorLayout4->setObjectName(QString::fromUtf8("collaboratorLayout4"));
        collaboratorPicLabel4 = new QLabel(collaboratorsWidget);
        collaboratorPicLabel4->setObjectName(QString::fromUtf8("collaboratorPicLabel4"));
        collaboratorPicLabel4->setMaximumSize(QSize(100, 100));
        collaboratorPicLabel4->setPixmap(QPixmap(QString::fromUtf8(":/images/default_profile_pic.png")));
        collaboratorPicLabel4->setScaledContents(true);

        collaboratorLayout4->addWidget(collaboratorPicLabel4);

        nicknameLabel4 = new QLabel(collaboratorsWidget);
        nicknameLabel4->setObjectName(QString::fromUtf8("nicknameLabel4"));
        nicknameLabel4->setMaximumSize(QSize(16777215, 35));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush5(QColor(255, 85, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush5);
        nicknameLabel4->setPalette(palette3);
        nicknameLabel4->setFont(font);
        nicknameLabel4->setAutoFillBackground(true);

        collaboratorLayout4->addWidget(nicknameLabel4);


        verticalLayout->addLayout(collaboratorLayout4);


        horizontalLayout->addWidget(collaboratorsWidget);

        Notepad->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Notepad);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1052, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        Notepad->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Notepad);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Notepad->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Notepad);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Notepad->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        mainToolBar->addAction(actionPrint);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCopy);
        mainToolBar->addAction(actionCut);
        mainToolBar->addAction(actionPaste);
        mainToolBar->addAction(actionUndo);
        mainToolBar->addAction(actionRedo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionExit);

        retranslateUi(Notepad);

        QMetaObject::connectSlotsByName(Notepad);
    } // setupUi

    void retranslateUi(QMainWindow *Notepad)
    {
        Notepad->setWindowTitle(QCoreApplication::translate("Notepad", "Notepad", nullptr));
        actionPrint->setText(QCoreApplication::translate("Notepad", "Print", nullptr));
#if QT_CONFIG(tooltip)
        actionPrint->setToolTip(QCoreApplication::translate("Notepad", "Print file", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionPrint->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("Notepad", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("Notepad", "Exit notepad", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCopy->setText(QCoreApplication::translate("Notepad", "Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCut->setText(QCoreApplication::translate("Notepad", "Cut", nullptr));
#if QT_CONFIG(shortcut)
        actionCut->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("Notepad", "Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionUndo->setText(QCoreApplication::translate("Notepad", "Undo", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRedo->setText(QCoreApplication::translate("Notepad", "Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("Notepad", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        collaboratorPicLabel1->setText(QString());
        nicknameLabel1->setText(QCoreApplication::translate("Notepad", "Unconnected", nullptr));
        collaboratorPicLabel2->setText(QString());
        nicknameLabel2->setText(QCoreApplication::translate("Notepad", "Unconnected", nullptr));
        collaboratorPicLabel3->setText(QString());
        nicknameLabel3->setText(QCoreApplication::translate("Notepad", "Unconnected", nullptr));
        collaboratorPicLabel4->setText(QString());
        nicknameLabel4->setText(QCoreApplication::translate("Notepad", "Unconnected", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Notepad", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("Notepad", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Notepad: public Ui_Notepad {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTEPAD_H
