/********************************************************************************
** Form generated from reading UI file 'CookieManagementSystem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COOKIEMANAGEMENTSYSTEM_H
#define UI_COOKIEMANAGEMENTSYSTEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CookieManagementSystemClass
{
public:
    QWidget *centralWidget;
    QLabel *lb_title;
    QGroupBox *buttonLayout;
    QPushButton *pushButton_deleteSelected;
    QPushButton *pushButton_deletecookies;
    QPushButton *pushButton_analysecookies;
    QPushButton *pushButton_readcookies;
    QGroupBox *bottomLayout;
    QPushButton *pushButton_learnAboutCookies;
    QTextEdit *textSummary;
    QPushButton *pushButton_exportdatabase;
    QGroupBox *topFilterLayout;
    QComboBox *categoryDropdown;
    QLineEdit *searchLineEdit;
    QGroupBox *middleLayout;
    QTableWidget *cookieTable;
    QCheckBox *checkBox_anonymise;
    QPushButton *pushButton_clearAllFilters;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CookieManagementSystemClass)
    {
        if (CookieManagementSystemClass->objectName().isEmpty())
            CookieManagementSystemClass->setObjectName("CookieManagementSystemClass");
        CookieManagementSystemClass->resize(1186, 884);
        centralWidget = new QWidget(CookieManagementSystemClass);
        centralWidget->setObjectName("centralWidget");
        lb_title = new QLabel(centralWidget);
        lb_title->setObjectName("lb_title");
        lb_title->setGeometry(QRect(440, 50, 321, 51));
        QFont font;
        font.setFamilies({QString::fromUtf8("Sitka Text Semibold")});
        font.setPointSize(24);
        font.setBold(true);
        lb_title->setFont(font);
        buttonLayout = new QGroupBox(centralWidget);
        buttonLayout->setObjectName("buttonLayout");
        buttonLayout->setGeometry(QRect(10, 470, 1171, 81));
        pushButton_deleteSelected = new QPushButton(buttonLayout);
        pushButton_deleteSelected->setObjectName("pushButton_deleteSelected");
        pushButton_deleteSelected->setGeometry(QRect(580, 20, 131, 41));
        pushButton_deleteSelected->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_deleteSelected {\n"
"    background-color: #F44336;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        pushButton_deletecookies = new QPushButton(buttonLayout);
        pushButton_deletecookies->setObjectName("pushButton_deletecookies");
        pushButton_deletecookies->setGeometry(QRect(1010, 20, 141, 41));
        pushButton_deletecookies->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_deletecookies {\n"
"    background-color: #F44336;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        pushButton_analysecookies = new QPushButton(buttonLayout);
        pushButton_analysecookies->setObjectName("pushButton_analysecookies");
        pushButton_analysecookies->setGeometry(QRect(380, 20, 131, 41));
        pushButton_analysecookies->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_analysecookies {\n"
"    background-color: #FF9800;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        pushButton_readcookies = new QPushButton(buttonLayout);
        pushButton_readcookies->setObjectName("pushButton_readcookies");
        pushButton_readcookies->setGeometry(QRect(20, 20, 141, 41));
        pushButton_readcookies->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_readcookies {\n"
"    background-color: #2196F3;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        bottomLayout = new QGroupBox(centralWidget);
        bottomLayout->setObjectName("bottomLayout");
        bottomLayout->setGeometry(QRect(10, 560, 1171, 261));
        pushButton_learnAboutCookies = new QPushButton(bottomLayout);
        pushButton_learnAboutCookies->setObjectName("pushButton_learnAboutCookies");
        pushButton_learnAboutCookies->setGeometry(QRect(10, 170, 141, 41));
        pushButton_learnAboutCookies->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_learnAboutCookies {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        textSummary = new QTextEdit(bottomLayout);
        textSummary->setObjectName("textSummary");
        textSummary->setGeometry(QRect(170, 20, 991, 211));
        textSummary->setReadOnly(true);
        pushButton_exportdatabase = new QPushButton(bottomLayout);
        pushButton_exportdatabase->setObjectName("pushButton_exportdatabase");
        pushButton_exportdatabase->setGeometry(QRect(10, 50, 141, 41));
        pushButton_exportdatabase->setStyleSheet(QString::fromUtf8("QPushButton#pushButton_exportdatabase {\n"
"    background-color: #9C27B0;\n"
"    color: white;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"}"));
        topFilterLayout = new QGroupBox(centralWidget);
        topFilterLayout->setObjectName("topFilterLayout");
        topFilterLayout->setGeometry(QRect(0, 0, 1191, 51));
        categoryDropdown = new QComboBox(topFilterLayout);
        categoryDropdown->addItem(QString());
        categoryDropdown->addItem(QString());
        categoryDropdown->addItem(QString());
        categoryDropdown->addItem(QString());
        categoryDropdown->setObjectName("categoryDropdown");
        categoryDropdown->setGeometry(QRect(880, 10, 281, 31));
        searchLineEdit = new QLineEdit(topFilterLayout);
        searchLineEdit->setObjectName("searchLineEdit");
        searchLineEdit->setGeometry(QRect(10, 10, 841, 31));
        middleLayout = new QGroupBox(centralWidget);
        middleLayout->setObjectName("middleLayout");
        middleLayout->setGeometry(QRect(20, 100, 1151, 361));
        cookieTable = new QTableWidget(middleLayout);
        cookieTable->setObjectName("cookieTable");
        cookieTable->setGeometry(QRect(10, 10, 1131, 341));
        checkBox_anonymise = new QCheckBox(centralWidget);
        checkBox_anonymise->setObjectName("checkBox_anonymise");
        checkBox_anonymise->setGeometry(QRect(20, 60, 301, 22));
        pushButton_clearAllFilters = new QPushButton(centralWidget);
        pushButton_clearAllFilters->setObjectName("pushButton_clearAllFilters");
        pushButton_clearAllFilters->setGeometry(QRect(1050, 60, 121, 31));
        CookieManagementSystemClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CookieManagementSystemClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 1186, 37));
        CookieManagementSystemClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CookieManagementSystemClass);
        mainToolBar->setObjectName("mainToolBar");
        CookieManagementSystemClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(CookieManagementSystemClass);
        statusBar->setObjectName("statusBar");
        CookieManagementSystemClass->setStatusBar(statusBar);

        retranslateUi(CookieManagementSystemClass);

        QMetaObject::connectSlotsByName(CookieManagementSystemClass);
    } // setupUi

    void retranslateUi(QMainWindow *CookieManagementSystemClass)
    {
        CookieManagementSystemClass->setWindowTitle(QCoreApplication::translate("CookieManagementSystemClass", "CookieManagementSystem", nullptr));
        lb_title->setText(QCoreApplication::translate("CookieManagementSystemClass", "Cookie Management System", nullptr));
        buttonLayout->setTitle(QString());
        pushButton_deleteSelected->setText(QCoreApplication::translate("CookieManagementSystemClass", "Delete Selected", nullptr));
        pushButton_deletecookies->setText(QCoreApplication::translate("CookieManagementSystemClass", "Delete Cookies", nullptr));
        pushButton_analysecookies->setText(QCoreApplication::translate("CookieManagementSystemClass", "Analyse Cookies", nullptr));
        pushButton_readcookies->setText(QCoreApplication::translate("CookieManagementSystemClass", "Read Cookies", nullptr));
        bottomLayout->setTitle(QString());
        pushButton_learnAboutCookies->setText(QCoreApplication::translate("CookieManagementSystemClass", "Learn about Cookies", nullptr));
        textSummary->setHtml(QCoreApplication::translate("CookieManagementSystemClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'.AppleSystemUIFont'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Segoe UI'; font-size:12pt;\">Cookie Summary Report will appear here...</span></p></body></html>", nullptr));
        pushButton_exportdatabase->setText(QCoreApplication::translate("CookieManagementSystemClass", "Export Database", nullptr));
        topFilterLayout->setTitle(QString());
        categoryDropdown->setItemText(0, QCoreApplication::translate("CookieManagementSystemClass", "All", nullptr));
        categoryDropdown->setItemText(1, QCoreApplication::translate("CookieManagementSystemClass", "Functional", nullptr));
        categoryDropdown->setItemText(2, QCoreApplication::translate("CookieManagementSystemClass", "Analytics", nullptr));
        categoryDropdown->setItemText(3, QCoreApplication::translate("CookieManagementSystemClass", "Marketing", nullptr));

        searchLineEdit->setText(QString());
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("CookieManagementSystemClass", "Search cookies by name or domain...", nullptr));
        middleLayout->setTitle(QString());
        checkBox_anonymise->setText(QCoreApplication::translate("CookieManagementSystemClass", "Mask sensitive cookie names and domains", nullptr));
        pushButton_clearAllFilters->setText(QCoreApplication::translate("CookieManagementSystemClass", "Clear All Filters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CookieManagementSystemClass: public Ui_CookieManagementSystemClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COOKIEMANAGEMENTSYSTEM_H
