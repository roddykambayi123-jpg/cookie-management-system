/********************************************************************************
** Form generated from reading UI file 'RiskLegendPage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RISKLEGENDPAGE_H
#define UI_RISKLEGENDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RiskLegendPage
{
public:
    QVBoxLayout *dockRoot;
    QGroupBox *groupCategories;
    QVBoxLayout *catLayout;
    QListWidget *catList;
    QGroupBox *groupRisk;
    QVBoxLayout *riskLayout;
    QListWidget *riskList;
    QPushButton *applyDockFiltersBtn;

    void setupUi(QWidget *RiskLegendPage)
    {
        if (RiskLegendPage->objectName().isEmpty())
            RiskLegendPage->setObjectName("RiskLegendPage");
        dockRoot = new QVBoxLayout(RiskLegendPage);
        dockRoot->setObjectName("dockRoot");
        groupCategories = new QGroupBox(RiskLegendPage);
        groupCategories->setObjectName("groupCategories");
        catLayout = new QVBoxLayout(groupCategories);
        catLayout->setObjectName("catLayout");
        catList = new QListWidget(groupCategories);
        new QListWidgetItem(catList);
        new QListWidgetItem(catList);
        new QListWidgetItem(catList);
        new QListWidgetItem(catList);
        new QListWidgetItem(catList);
        catList->setObjectName("catList");
        catList->setSelectionMode(QAbstractItemView::MultiSelection);

        catLayout->addWidget(catList);


        dockRoot->addWidget(groupCategories);

        groupRisk = new QGroupBox(RiskLegendPage);
        groupRisk->setObjectName("groupRisk");
        riskLayout = new QVBoxLayout(groupRisk);
        riskLayout->setObjectName("riskLayout");
        riskList = new QListWidget(groupRisk);
        new QListWidgetItem(riskList);
        new QListWidgetItem(riskList);
        new QListWidgetItem(riskList);
        new QListWidgetItem(riskList);
        riskList->setObjectName("riskList");
        riskList->setSelectionMode(QAbstractItemView::MultiSelection);

        riskLayout->addWidget(riskList);


        dockRoot->addWidget(groupRisk);

        applyDockFiltersBtn = new QPushButton(RiskLegendPage);
        applyDockFiltersBtn->setObjectName("applyDockFiltersBtn");

        dockRoot->addWidget(applyDockFiltersBtn);


        retranslateUi(RiskLegendPage);

        QMetaObject::connectSlotsByName(RiskLegendPage);
    } // setupUi

    void retranslateUi(QWidget *RiskLegendPage)
    {
        RiskLegendPage->setWindowTitle(QCoreApplication::translate("RiskLegendPage", "Legend & Filters", nullptr));
        groupCategories->setTitle(QCoreApplication::translate("RiskLegendPage", "Categories", nullptr));

        const bool __sortingEnabled = catList->isSortingEnabled();
        catList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = catList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("RiskLegendPage", "Marketing", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = catList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("RiskLegendPage", "Analytics", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = catList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("RiskLegendPage", "Functional", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = catList->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("RiskLegendPage", "Necessary", nullptr));
        QListWidgetItem *___qlistwidgetitem4 = catList->item(4);
        ___qlistwidgetitem4->setText(QCoreApplication::translate("RiskLegendPage", "Unknown", nullptr));
        catList->setSortingEnabled(__sortingEnabled);

        groupRisk->setTitle(QCoreApplication::translate("RiskLegendPage", "Risk Levels", nullptr));

        const bool __sortingEnabled1 = riskList->isSortingEnabled();
        riskList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem5 = riskList->item(0);
        ___qlistwidgetitem5->setText(QCoreApplication::translate("RiskLegendPage", "Critical", nullptr));
        QListWidgetItem *___qlistwidgetitem6 = riskList->item(1);
        ___qlistwidgetitem6->setText(QCoreApplication::translate("RiskLegendPage", "High", nullptr));
        QListWidgetItem *___qlistwidgetitem7 = riskList->item(2);
        ___qlistwidgetitem7->setText(QCoreApplication::translate("RiskLegendPage", "Medium", nullptr));
        QListWidgetItem *___qlistwidgetitem8 = riskList->item(3);
        ___qlistwidgetitem8->setText(QCoreApplication::translate("RiskLegendPage", "Low", nullptr));
        riskList->setSortingEnabled(__sortingEnabled1);

        applyDockFiltersBtn->setText(QCoreApplication::translate("RiskLegendPage", "Apply Filters", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RiskLegendPage: public Ui_RiskLegendPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RISKLEGENDPAGE_H
