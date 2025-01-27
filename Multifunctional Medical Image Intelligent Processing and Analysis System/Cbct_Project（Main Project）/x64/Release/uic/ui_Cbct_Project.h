/********************************************************************************
** Form generated from reading UI file 'Cbct_Project.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CBCT_PROJECT_H
#define UI_CBCT_PROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cbct_ProjectClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cbct_ProjectClass)
    {
        if (Cbct_ProjectClass->objectName().isEmpty())
            Cbct_ProjectClass->setObjectName("Cbct_ProjectClass");
        Cbct_ProjectClass->resize(600, 400);
        menuBar = new QMenuBar(Cbct_ProjectClass);
        menuBar->setObjectName("menuBar");
        Cbct_ProjectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Cbct_ProjectClass);
        mainToolBar->setObjectName("mainToolBar");
        Cbct_ProjectClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(Cbct_ProjectClass);
        centralWidget->setObjectName("centralWidget");
        Cbct_ProjectClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Cbct_ProjectClass);
        statusBar->setObjectName("statusBar");
        Cbct_ProjectClass->setStatusBar(statusBar);

        retranslateUi(Cbct_ProjectClass);

        QMetaObject::connectSlotsByName(Cbct_ProjectClass);
    } // setupUi

    void retranslateUi(QMainWindow *Cbct_ProjectClass)
    {
        Cbct_ProjectClass->setWindowTitle(QCoreApplication::translate("Cbct_ProjectClass", "Cbct_Project", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Cbct_ProjectClass: public Ui_Cbct_ProjectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CBCT_PROJECT_H
