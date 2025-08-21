#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CookieManagementSystem.h"
#include "CookieReader.h"
#include <memory>


class CookieManagementSystem : public QMainWindow
{
    Q_OBJECT
    QVector<QStringList> originalCookieData;


public:
    explicit CookieManagementSystem(QWidget *parent = nullptr);
    ~CookieManagementSystem();

private slots:
    void on_importFromBrowserButton_clicked();
    void filterCookiesByCategory(const QString &category);
    void applyAnonymisationToggle();
    void deleteSelectedCookies();
    void deleteAllCookies();
    void filterByText(const QString &text);
    void generateSummaryReport();
    void showCookieGuide();
    void exportDatabaseToCSV();
    void clearAllFilters();

private:
    void showDiagnosticsDialog(const QString& text);
    QString m_lastDiagnostics;
    void populateFromBrowser(const QVector<CookieItem>& items);
    void setupTableWidget();
    QString anonymizeValue(const QString &value);
    BrowserInfo m_currentBrowser;
    std::unique_ptr<CookieReader> m_currentReader;
    void refreshFromCurrentSource();
    Ui::CookieManagementSystemClass ui;
};
