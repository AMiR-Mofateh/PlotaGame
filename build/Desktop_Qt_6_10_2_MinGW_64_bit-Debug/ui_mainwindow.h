/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLineEdit *Port;
    QLineEdit *Ip;
    QPushButton *Start;
    QPushButton *Exit;
    QListWidget *Log;
    QMenuBar *menubar;
    QMenu *menuServer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        formLayoutWidget = new QWidget(centralwidget);
        formLayoutWidget->setObjectName("formLayoutWidget");
        formLayoutWidget->setGeometry(QRect(10, 10, 771, 541));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName("formLayout");
        formLayout->setContentsMargins(0, 0, 0, 0);
        Port = new QLineEdit(formLayoutWidget);
        Port->setObjectName("Port");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, Port);

        Ip = new QLineEdit(formLayoutWidget);
        Ip->setObjectName("Ip");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, Ip);

        Start = new QPushButton(formLayoutWidget);
        Start->setObjectName("Start");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, Start);

        Exit = new QPushButton(formLayoutWidget);
        Exit->setObjectName("Exit");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, Exit);

        Log = new QListWidget(formLayoutWidget);
        Log->setObjectName("Log");

        formLayout->setWidget(1, QFormLayout::ItemRole::SpanningRole, Log);

        Start->raise();
        Port->raise();
        Ip->raise();
        Exit->raise();
        Log->raise();
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuServer = new QMenu(menubar);
        menuServer->setObjectName("menuServer");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuServer->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Start->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        menuServer->setTitle(QCoreApplication::translate("MainWindow", "Server", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
