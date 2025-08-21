/********************************************************************************
** Form generated from reading UI file 'CookieDetailsDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COOKIEDETAILSDIALOG_H
#define UI_COOKIEDETAILSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CookieDetailsDialog
{
public:
    QVBoxLayout *root;
    QGridLayout *grid;
    QLabel *labelSrc;
    QLineEdit *srcEdit;
    QLabel *labelDomain;
    QLineEdit *domainEdit;
    QLabel *labelName;
    QLineEdit *nameEdit;
    QLabel *labelValue;
    QPlainTextEdit *valueEdit;
    QLabel *labelSecure;
    QCheckBox *secureChk;
    QLabel *labelHttpOnly;
    QCheckBox *httpOnlyChk;
    QLabel *labelExp;
    QLineEdit *expEdit;
    QLabel *labelFlags;
    QLabel *flagsLbl;
    QDialogButtonBox *btnBox;

    void setupUi(QDialog *CookieDetailsDialog)
    {
        if (CookieDetailsDialog->objectName().isEmpty())
            CookieDetailsDialog->setObjectName("CookieDetailsDialog");
        CookieDetailsDialog->setMinimumSize(QSize(540, 420));
        root = new QVBoxLayout(CookieDetailsDialog);
        root->setObjectName("root");
        grid = new QGridLayout();
        grid->setObjectName("grid");
        labelSrc = new QLabel(CookieDetailsDialog);
        labelSrc->setObjectName("labelSrc");

        grid->addWidget(labelSrc, 0, 0, 1, 1);

        srcEdit = new QLineEdit(CookieDetailsDialog);
        srcEdit->setObjectName("srcEdit");
        srcEdit->setReadOnly(true);

        grid->addWidget(srcEdit, 0, 1, 1, 1);

        labelDomain = new QLabel(CookieDetailsDialog);
        labelDomain->setObjectName("labelDomain");

        grid->addWidget(labelDomain, 1, 0, 1, 1);

        domainEdit = new QLineEdit(CookieDetailsDialog);
        domainEdit->setObjectName("domainEdit");
        domainEdit->setReadOnly(true);

        grid->addWidget(domainEdit, 1, 1, 1, 1);

        labelName = new QLabel(CookieDetailsDialog);
        labelName->setObjectName("labelName");

        grid->addWidget(labelName, 2, 0, 1, 1);

        nameEdit = new QLineEdit(CookieDetailsDialog);
        nameEdit->setObjectName("nameEdit");
        nameEdit->setReadOnly(true);

        grid->addWidget(nameEdit, 2, 1, 1, 1);

        labelValue = new QLabel(CookieDetailsDialog);
        labelValue->setObjectName("labelValue");

        grid->addWidget(labelValue, 3, 0, 1, 1);

        valueEdit = new QPlainTextEdit(CookieDetailsDialog);
        valueEdit->setObjectName("valueEdit");
        valueEdit->setReadOnly(true);
        valueEdit->setMinimumHeight(120);

        grid->addWidget(valueEdit, 3, 1, 1, 1);

        labelSecure = new QLabel(CookieDetailsDialog);
        labelSecure->setObjectName("labelSecure");

        grid->addWidget(labelSecure, 4, 0, 1, 1);

        secureChk = new QCheckBox(CookieDetailsDialog);
        secureChk->setObjectName("secureChk");
        secureChk->setEnabled(false);

        grid->addWidget(secureChk, 4, 1, 1, 1);

        labelHttpOnly = new QLabel(CookieDetailsDialog);
        labelHttpOnly->setObjectName("labelHttpOnly");

        grid->addWidget(labelHttpOnly, 5, 0, 1, 1);

        httpOnlyChk = new QCheckBox(CookieDetailsDialog);
        httpOnlyChk->setObjectName("httpOnlyChk");
        httpOnlyChk->setEnabled(false);

        grid->addWidget(httpOnlyChk, 5, 1, 1, 1);

        labelExp = new QLabel(CookieDetailsDialog);
        labelExp->setObjectName("labelExp");

        grid->addWidget(labelExp, 6, 0, 1, 1);

        expEdit = new QLineEdit(CookieDetailsDialog);
        expEdit->setObjectName("expEdit");
        expEdit->setReadOnly(true);

        grid->addWidget(expEdit, 6, 1, 1, 1);

        labelFlags = new QLabel(CookieDetailsDialog);
        labelFlags->setObjectName("labelFlags");

        grid->addWidget(labelFlags, 7, 0, 1, 1);

        flagsLbl = new QLabel(CookieDetailsDialog);
        flagsLbl->setObjectName("flagsLbl");
        flagsLbl->setWordWrap(true);

        grid->addWidget(flagsLbl, 7, 1, 1, 1);


        root->addLayout(grid);

        btnBox = new QDialogButtonBox(CookieDetailsDialog);
        btnBox->setObjectName("btnBox");
        btnBox->setStandardButtons(QDialogButtonBox::Close);

        root->addWidget(btnBox);


        retranslateUi(CookieDetailsDialog);

        QMetaObject::connectSlotsByName(CookieDetailsDialog);
    } // setupUi

    void retranslateUi(QDialog *CookieDetailsDialog)
    {
        CookieDetailsDialog->setWindowTitle(QCoreApplication::translate("CookieDetailsDialog", "Cookie details", nullptr));
        labelSrc->setText(QCoreApplication::translate("CookieDetailsDialog", "Source", nullptr));
        labelDomain->setText(QCoreApplication::translate("CookieDetailsDialog", "Domain", nullptr));
        labelName->setText(QCoreApplication::translate("CookieDetailsDialog", "Name", nullptr));
        labelValue->setText(QCoreApplication::translate("CookieDetailsDialog", "Value", nullptr));
        labelSecure->setText(QCoreApplication::translate("CookieDetailsDialog", "Secure", nullptr));
        labelHttpOnly->setText(QCoreApplication::translate("CookieDetailsDialog", "HttpOnly", nullptr));
        labelExp->setText(QCoreApplication::translate("CookieDetailsDialog", "Expires", nullptr));
        labelFlags->setText(QCoreApplication::translate("CookieDetailsDialog", "Flags", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CookieDetailsDialog: public Ui_CookieDetailsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COOKIEDETAILSDIALOG_H
