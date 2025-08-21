/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CookieWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushbtn_read;
    QPushButton *pushBtn_analyse;
    QPushButton *pushBtn_delete;
    QTableWidget *cookieTable;
    QLabel *lb_title;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CookieWindow)
    {
        if (CookieWindow->objectName().isEmpty())
            CookieWindow->setObjectName("CookieWindow");
        CookieWindow->resize(800, 600);
        centralwidget = new QWidget(CookieWindow);
        centralwidget->setObjectName("centralwidget");
        pushbtn_read = new QPushButton(centralwidget);
        pushbtn_read->setObjectName("pushbtn_read");
        pushbtn_read->setGeometry(QRect(40, 460, 131, 41));
        pushBtn_analyse = new QPushButton(centralwidget);
        pushBtn_analyse->setObjectName("pushBtn_analyse");
        pushBtn_analyse->setGeometry(QRect(330, 460, 131, 41));
        pushBtn_delete = new QPushButton(centralwidget);
        pushBtn_delete->setObjectName("pushBtn_delete");
        pushBtn_delete->setGeometry(QRect(610, 460, 131, 41));
        cookieTable = new QTableWidget(centralwidget);
        if (cookieTable->columnCount() < 5)
            cookieTable->setColumnCount(5);
        cookieTable->setObjectName("cookieTable");
        cookieTable->setGeometry(QRect(20, 80, 751, 351));
        cookieTable->setColumnCount(5);
        cookieTable->horizontalHeader()->setDefaultSectionSize(150);
        cookieTable->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
        lb_title = new QLabel(centralwidget);
        lb_title->setObjectName("lb_title");
        lb_title->setGeometry(QRect(180, 10, 371, 71));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arial Rounded MT Bold")});
        font.setPointSize(20);
        lb_title->setFont(font);
        CookieWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CookieWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        CookieWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CookieWindow);
        statusbar->setObjectName("statusbar");
        CookieWindow->setStatusBar(statusbar);

        retranslateUi(CookieWindow);

        QMetaObject::connectSlotsByName(CookieWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CookieWindow)
    {
        CookieWindow->setWindowTitle(QCoreApplication::translate("CookieWindow", "MainWindow", nullptr));
        pushbtn_read->setText(QCoreApplication::translate("CookieWindow", "Read Cookies", nullptr));
        pushBtn_analyse->setText(QCoreApplication::translate("CookieWindow", "Analyse Cookies", nullptr));
        pushBtn_delete->setText(QCoreApplication::translate("CookieWindow", "Delete Cookies", nullptr));
        lb_title->setText(QCoreApplication::translate("CookieWindow", "Cookie Management System", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CookieWindow: public Ui_CookieWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
