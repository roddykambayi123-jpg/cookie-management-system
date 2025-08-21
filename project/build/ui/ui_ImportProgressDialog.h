/********************************************************************************
** Form generated from reading UI file 'ImportProgressDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTPROGRESSDIALOG_H
#define UI_IMPORTPROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImportProgressDialog
{
public:
    QVBoxLayout *root;
    QLabel *stageLbl;
    QProgressBar *progressBar;
    QTextEdit *logEdit;
    QHBoxLayout *bottomRow;
    QSpacerItem *hsp;
    QPushButton *cancelBtn;

    void setupUi(QDialog *ImportProgressDialog)
    {
        if (ImportProgressDialog->objectName().isEmpty())
            ImportProgressDialog->setObjectName("ImportProgressDialog");
        ImportProgressDialog->setMinimumSize(QSize(420, 160));
        ImportProgressDialog->setModal(true);
        root = new QVBoxLayout(ImportProgressDialog);
        root->setObjectName("root");
        stageLbl = new QLabel(ImportProgressDialog);
        stageLbl->setObjectName("stageLbl");

        root->addWidget(stageLbl);

        progressBar = new QProgressBar(ImportProgressDialog);
        progressBar->setObjectName("progressBar");
        progressBar->setMinimum(0);
        progressBar->setMaximum(0);
        progressBar->setTextVisible(false);

        root->addWidget(progressBar);

        logEdit = new QTextEdit(ImportProgressDialog);
        logEdit->setObjectName("logEdit");
        logEdit->setReadOnly(true);
        logEdit->setMinimumHeight(80);

        root->addWidget(logEdit);

        bottomRow = new QHBoxLayout();
        bottomRow->setObjectName("bottomRow");
        hsp = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomRow->addItem(hsp);

        cancelBtn = new QPushButton(ImportProgressDialog);
        cancelBtn->setObjectName("cancelBtn");

        bottomRow->addWidget(cancelBtn);


        root->addLayout(bottomRow);


        retranslateUi(ImportProgressDialog);

        QMetaObject::connectSlotsByName(ImportProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportProgressDialog)
    {
        ImportProgressDialog->setWindowTitle(QCoreApplication::translate("ImportProgressDialog", "Importing cookies\342\200\246", nullptr));
        stageLbl->setText(QCoreApplication::translate("ImportProgressDialog", "Copying cookie database\342\200\246", nullptr));
        logEdit->setPlaceholderText(QCoreApplication::translate("ImportProgressDialog", "Log output\342\200\246", nullptr));
        cancelBtn->setText(QCoreApplication::translate("ImportProgressDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImportProgressDialog: public Ui_ImportProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTPROGRESSDIALOG_H
