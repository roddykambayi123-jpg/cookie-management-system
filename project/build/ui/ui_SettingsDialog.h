/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *root;
    QTabWidget *tabs;
    QWidget *appearanceTab;
    QFormLayout *formAppearance;
    QLabel *lblTheme;
    QComboBox *themeCombo;
    QLabel *lblFont;
    QSpinBox *fontSizeSpin;
    QCheckBox *highContrastChk;
    QWidget *accessibilityTab;
    QFormLayout *formAccessibility;
    QCheckBox *tooltipsChk;
    QCheckBox *focusRectsChk;
    QCheckBox *largeHitTargetsChk;
    QDialogButtonBox *btnBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->setMinimumSize(QSize(520, 320));
        root = new QVBoxLayout(SettingsDialog);
        root->setObjectName("root");
        tabs = new QTabWidget(SettingsDialog);
        tabs->setObjectName("tabs");
        appearanceTab = new QWidget();
        appearanceTab->setObjectName("appearanceTab");
        formAppearance = new QFormLayout(appearanceTab);
        formAppearance->setObjectName("formAppearance");
        lblTheme = new QLabel(appearanceTab);
        lblTheme->setObjectName("lblTheme");

        formAppearance->setWidget(0, QFormLayout::ItemRole::LabelRole, lblTheme);

        themeCombo = new QComboBox(appearanceTab);
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->addItem(QString());
        themeCombo->setObjectName("themeCombo");

        formAppearance->setWidget(0, QFormLayout::ItemRole::FieldRole, themeCombo);

        lblFont = new QLabel(appearanceTab);
        lblFont->setObjectName("lblFont");

        formAppearance->setWidget(1, QFormLayout::ItemRole::LabelRole, lblFont);

        fontSizeSpin = new QSpinBox(appearanceTab);
        fontSizeSpin->setObjectName("fontSizeSpin");
        fontSizeSpin->setMinimum(10);
        fontSizeSpin->setMaximum(20);
        fontSizeSpin->setValue(12);

        formAppearance->setWidget(1, QFormLayout::ItemRole::FieldRole, fontSizeSpin);

        highContrastChk = new QCheckBox(appearanceTab);
        highContrastChk->setObjectName("highContrastChk");

        formAppearance->setWidget(2, QFormLayout::ItemRole::FieldRole, highContrastChk);

        tabs->addTab(appearanceTab, QString());
        accessibilityTab = new QWidget();
        accessibilityTab->setObjectName("accessibilityTab");
        formAccessibility = new QFormLayout(accessibilityTab);
        formAccessibility->setObjectName("formAccessibility");
        tooltipsChk = new QCheckBox(accessibilityTab);
        tooltipsChk->setObjectName("tooltipsChk");

        formAccessibility->setWidget(0, QFormLayout::ItemRole::FieldRole, tooltipsChk);

        focusRectsChk = new QCheckBox(accessibilityTab);
        focusRectsChk->setObjectName("focusRectsChk");

        formAccessibility->setWidget(1, QFormLayout::ItemRole::FieldRole, focusRectsChk);

        largeHitTargetsChk = new QCheckBox(accessibilityTab);
        largeHitTargetsChk->setObjectName("largeHitTargetsChk");

        formAccessibility->setWidget(2, QFormLayout::ItemRole::FieldRole, largeHitTargetsChk);

        tabs->addTab(accessibilityTab, QString());

        root->addWidget(tabs);

        btnBox = new QDialogButtonBox(SettingsDialog);
        btnBox->setObjectName("btnBox");
        btnBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        root->addWidget(btnBox);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Settings", nullptr));
        lblTheme->setText(QCoreApplication::translate("SettingsDialog", "Theme", nullptr));
        themeCombo->setItemText(0, QCoreApplication::translate("SettingsDialog", "System", nullptr));
        themeCombo->setItemText(1, QCoreApplication::translate("SettingsDialog", "Light", nullptr));
        themeCombo->setItemText(2, QCoreApplication::translate("SettingsDialog", "Dark", nullptr));

        lblFont->setText(QCoreApplication::translate("SettingsDialog", "Font size", nullptr));
        fontSizeSpin->setSuffix(QCoreApplication::translate("SettingsDialog", " pt", nullptr));
        highContrastChk->setText(QCoreApplication::translate("SettingsDialog", "High contrast mode", nullptr));
        tabs->setTabText(tabs->indexOf(appearanceTab), QCoreApplication::translate("SettingsDialog", "Appearance", nullptr));
        tooltipsChk->setText(QCoreApplication::translate("SettingsDialog", "Show helper tooltips", nullptr));
        focusRectsChk->setText(QCoreApplication::translate("SettingsDialog", "Always show focus rectangles", nullptr));
        largeHitTargetsChk->setText(QCoreApplication::translate("SettingsDialog", "Larger buttons and spacing", nullptr));
        tabs->setTabText(tabs->indexOf(accessibilityTab), QCoreApplication::translate("SettingsDialog", "Accessibility", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
