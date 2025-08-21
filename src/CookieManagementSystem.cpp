// CookieManagementSystem.cpp
#include "BrowserDetector.h"
#include "CookieReader.h"
#include "ChooseBrowserDialog.h"
#include "CookieManagementSystem.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QMap>
#include <random>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <QShortcut>
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>
#include <QFileInfo>

CookieManagementSystem::CookieManagementSystem(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // Force the "Read Cookies" button to use the browser picker flow
    if (ui.pushButton_readcookies) {
        QObject::disconnect(ui.pushButton_readcookies, nullptr, nullptr, nullptr); // drop any old CSV auto- or Designer connections
        connect(ui.pushButton_readcookies, &QPushButton::clicked,
                this, &CookieManagementSystem::on_importFromBrowserButton_clicked);
        ui.pushButton_readcookies->setToolTip("Detect installed browsers, pick one, and import its cookies.");
    }

    // Search keyboard shortcut
    new QShortcut(QKeySequence("Ctrl+F"), this, [this] {
    ui.searchLineEdit->setFocus();
    });

    // Delete selected keyboard shortcut
    new QShortcut(QKeySequence("Ctrl+D"), this, [this] {
    deleteSelectedCookies();
    });

    // Diagnostics keyboard shortcut
new QShortcut(QKeySequence("Ctrl+I"), this, [this] {
    if (!m_lastDiagnostics.isEmpty())
        showDiagnosticsDialog(m_lastDiagnostics);
    else
        QMessageBox::information(this, "Diagnostics", "No diagnostics available yet.");
});

    // Table Customisation
    ui.textSummary->setStyleSheet(
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #2c3e50, stop:1 #bdc3c7); "
        "color: white; "
        "border-radius: 10px; "
        "padding: 10px; "
        "font-family: Consolas; "
        "font-size: 13px;"
    );

    ui.cookieTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background-color: #333; color: white; padding: 4px; border: none; }"
    );
    ui.cookieTable->setShowGrid(false);

    // Hide action buttons initially
    ui.pushButton_analysecookies->hide();
    ui.pushButton_deleteSelected->hide();
    ui.pushButton_deletecookies->hide();
    ui.pushButton_exportdatabase->hide();
    ui.checkBox_anonymise->hide();
    ui.pushButton_clearAllFilters->hide();

    // Initialize random number generator for tip shuffling
    srand(static_cast<unsigned int>(time(nullptr)));

    // Call setupTableWidget to set column headers
    setupTableWidget();

    // Ensure the category filter includes Unknown (overrides any Designer defaults)
    if (ui.categoryDropdown) {
        ui.categoryDropdown->blockSignals(true);
        ui.categoryDropdown->clear();
        ui.categoryDropdown->addItems({ "All", "Functional", "Analytics", "Marketing", "Unknown" });
        ui.categoryDropdown->blockSignals(false);
    }

    // Filter cookies by category
    connect(ui.categoryDropdown, &QComboBox::currentTextChanged, this, &CookieManagementSystem::filterCookiesByCategory);

    // Delete buttons
    connect(ui.pushButton_deleteSelected, &QPushButton::clicked, this, &CookieManagementSystem::deleteSelectedCookies);
    connect(ui.pushButton_deletecookies, &QPushButton::clicked, this, &CookieManagementSystem::deleteAllCookies);

    connect(ui.checkBox_anonymise, &QCheckBox::checkStateChanged, this, &CookieManagementSystem::applyAnonymisationToggle);

    // Text search
    connect(ui.searchLineEdit, &QLineEdit::textChanged, this, &CookieManagementSystem::filterByText);

    // Summary / Analysis
    connect(ui.pushButton_analysecookies, &QPushButton::clicked, this, &CookieManagementSystem::generateSummaryReport);

    // Educational guide
    connect(ui.pushButton_learnAboutCookies, &QPushButton::clicked, this, &CookieManagementSystem::showCookieGuide);

    connect(ui.pushButton_exportdatabase, &QPushButton::clicked, this, &CookieManagementSystem::exportDatabaseToCSV);

    connect(ui.pushButton_clearAllFilters, &QPushButton::clicked, this, &CookieManagementSystem::clearAllFilters);

    // Tool Tips
    ui.pushButton_readcookies->setToolTip("Detect installed browsers, pick one, and import its cookies.");
    ui.pushButton_analysecookies->setToolTip("Generate a cookie summary report and advice.");
    ui.pushButton_deleteSelected->setToolTip("Delete only the selected cookies.");
    ui.pushButton_deletecookies->setToolTip("Delete all cookies currently displayed.");
    ui.pushButton_learnAboutCookies->setToolTip("Learn about cookie types, privacy implications, and safety tips.");
    ui.searchLineEdit->setToolTip("Search by cookie name or domain.\nCase-insensitive and updates results as you type.");
    ui.categoryDropdown->setToolTip("Filter cookies by category (e.g. Functional, Analytics, Marketing).\nSelect 'All' to show every cookie.");
    ui.checkBox_anonymise->setToolTip("Enable this to hide cookie names and domains that may contain tracking IDs or sensitive identifiers.");
}

// --- Heuristic cookie categoriser -------------------------------------------
static QString classifyCookieByHeuristics(const QString& name, const QString& domain)
{
    const QString n = name.toLower();
    const QString d = domain.toLower();

    // -------- Security / Functional (first so they don't get mislabeled) ----
    // Session/auth/csrf/host-only prefixes
    if (n.contains("session") || n == "sid" || n.startsWith("sess") ||
        n.contains("xsrf") || n.contains("csrf") ||
        n.startsWith("__secure-") || n.startsWith("__host-") ||
        n.contains("auth") || n.contains("loggedin") || n.contains("logged_in")) {
        return "Functional";   // could also be "Security", but we group both as green
    }

    // -------- Analytics ------------------------------------------------------
    // Google Analytics + UTM, Clarity, Hotjar, Amplitude, Mixpanel, Segment, Adobe
    if (n.startsWith("_ga") || n == "_gid" || n.startsWith("_gat") ||
        n.startsWith("__utm") /* __utma,__utmb,__utmc,__utmt,__utmz */ ||
        n == "_clck" || n == "_clsk" || d.contains("clarity.ms") ||
        n.startsWith("_hj") || d.contains("hotjar.com") ||
        n.startsWith("amplitude_id_") || n == "amplitude_session_id" ||
        n.startsWith("mp_") /* mixpanel */ || d.contains("mixpanel.com") ||
        n.startsWith("ajs_") /* Segment */ ||
        n == "s_cc" || n == "s_sq" /* Adobe Analytics */ ||
        d.contains("analytics") || d.contains("stats") || d.contains("matomo") ||
        d.contains("plausible.io")) {
        return "Analytics";
    }

    // -------- Marketing / Advertising ---------------------------------------
    // Google Ads / DoubleClick / Facebook / TikTok / LinkedIn / Twitter / Taboola, etc.
    if (n.startsWith("_gcl_") || n.startsWith("_gac_") ||
        n == "ide" || n == "anid" ||
        d.contains("doubleclick.net") || d.contains("googleadservices.com") ||
        n == "_fbp" || n == "fr" || d.contains("facebook.com") || d.contains("meta.com") ||
        d.contains("tiktok.com") || n.contains("tt_") ||
        d.contains("linkedin.com") || n == "bcookie" || n == "liap" || n == "lidc" ||
        d.contains("twitter.com") || n == "personalization_id" || n == "guest_id" ||
        d.contains("taboola.com") || d.contains("outbrain.com") ||
        d.contains("adservice") || d.contains("adsystem") || d.contains("adnxs.com")) {
        return "Marketing";
    }

    // -------- Functional (other common cases) --------------------------------
    if (n.contains("pref") || n.contains("consent") ||
        n.contains("lang") || n.contains("locale") ||
        n.contains("theme") || n.contains("cookieconsent") ||
        n == "csrftoken" || n == "remember_token") {
        return "Functional";
    }

    return "Unknown";
}

void CookieManagementSystem::populateFromBrowser(const QVector<CookieItem>& items) {
    // mirror your 9-column layout: Select | ID | Platform | Category | Cookie Name | Domain | Description | Retention | Controller
    ui.cookieTable->setSortingEnabled(false);
    ui.cookieTable->setUpdatesEnabled(false);
    ui.cookieTable->setRowCount(items.size());

    const bool anonymise = ui.checkBox_anonymise->isChecked();

    for (int row = 0; row < items.size(); ++row) {
        const CookieItem& c = items[row];

        QString id, platform = c.sourceBrowser;
        QString category = classifyCookieByHeuristics(c.name, c.domain);
        QString name = c.name, domain = c.domain, description = c.value;
        QString retention = c.expiresAt.isValid() ? c.expiresAt.toString(Qt::ISODate) : QString();
        QString controller;

        if (anonymise) {
            name = anonymizeValue(name);
            domain = anonymizeValue(domain);
            if (!controller.isEmpty()) controller = anonymizeValue(controller);
        }

        // column 0: checkbox
        QWidget *w = new QWidget();
        auto *cb = new QCheckBox();
        auto *lay = new QHBoxLayout(w);
        lay->addWidget(cb); lay->setAlignment(Qt::AlignCenter); lay->setContentsMargins(0,0,0,0);
        ui.cookieTable->setCellWidget(row, 0, w);

        auto setRO = [&](int col, const QString& t){
            auto *it = new QTableWidgetItem(t);
            it->setFlags(it->flags() & ~Qt::ItemIsEditable);
            ui.cookieTable->setItem(row, col, it);
        };
        setRO(1, id);
        setRO(2, platform);
        setRO(3, category);
        setRO(4, name);
        setRO(5, domain);
        setRO(6, description);
        setRO(7, retention);
        setRO(8, controller);

                // category colouring
        if (auto *cat = ui.cookieTable->item(row, 3)) {
            cat->setTextAlignment(Qt::AlignCenter);
            const QString catTxt = category.toLower();
            if (catTxt == "marketing") {
                cat->setBackground(QColor("#f44336")); // red
                cat->setForeground(Qt::white);
            } else if (catTxt == "analytics") {
                cat->setBackground(QColor("#ff9800")); // orange
                cat->setForeground(Qt::white);
            } else if (catTxt == "functional" || catTxt == "security") {
                cat->setBackground(QColor("#4caf50")); // green
                cat->setForeground(Qt::white);
            } else {
                cat->setBackground(QColor("#9e9e9e")); // grey
                cat->setForeground(Qt::white);
            }
        }

        // keep a copy for anonymisation toggle if you use originalCookieData
        QStringList fields = {id, platform, category, c.name, c.domain, description, retention, controller};
        if (row >= originalCookieData.size()) originalCookieData.append(fields);
        else originalCookieData[row] = fields;
    }

    ui.cookieTable->setUpdatesEnabled(true);
    ui.cookieTable->setSortingEnabled(true);

    const bool has = ui.cookieTable->rowCount() > 0;
    ui.pushButton_analysecookies->setVisible(has);
    ui.pushButton_deleteSelected->setVisible(has);
    ui.pushButton_deletecookies->setVisible(has);
    ui.pushButton_exportdatabase->setVisible(has);
    ui.checkBox_anonymise->setVisible(has);
    ui.pushButton_clearAllFilters->setVisible(has);
    if (has) ui.textSummary->clear();

    ui.statusBar->showMessage(QString("Imported %1 cookies from browser").arg(ui.cookieTable->rowCount()));
}

void CookieManagementSystem::on_importFromBrowserButton_clicked() {
    auto browsers = BrowserDetector::detectAll();
    if (browsers.isEmpty()) {
        QMessageBox::warning(this, "No browsers found", "I couldn't find any supported browsers.");
        return;
    }
    ChooseBrowserDialog dlg(this);
    dlg.setBrowsers(browsers);
    if (dlg.exec() != QDialog::Accepted || !dlg.hasSelection()) return;

    const BrowserInfo chosen = dlg.selected();
    auto reader = makeReader(chosen); m_currentReader = std::move(reader); m_currentBrowser = chosen;
    const QVector<CookieItem> cookies = m_currentReader ? m_currentReader->readAll() : QVector<CookieItem>{};

    // ---- Build Diagnostics ----
    QStringList lines;
    QFileInfo fi(chosen.cookieDbPath);
    lines << QString("Browser: %1").arg(chosen.name);
    lines << QString("Engine:  %1").arg(
        chosen.engine == BrowserEngine::Chromium ? "Chromium" :
        chosen.engine == BrowserEngine::Firefox  ? "Firefox"  :
        chosen.engine == BrowserEngine::Safari   ? "Safari"   : "Unknown");

    lines << QString("Path:    %1").arg(fi.absoluteFilePath());
    lines << QString("Exists:  %1").arg(fi.exists() ? "yes" : "no");
    lines << QString("Readable:%1").arg(fi.isReadable() ? "yes" : "no");
    if (fi.exists())
        lines << QString("Size:    %1 bytes").arg(QString::number(fi.size()));

    QString fmt = fi.suffix().toLower();
    if (fmt == "sqlite") fmt = "SQLite";
    else if (fi.fileName().contains("binarycookies", Qt::CaseInsensitive)) fmt = "BinaryCookies";
    else fmt = "Unknown";
    lines << QString("Store format guess: %1").arg(fmt);

    #if defined(Q_OS_MAC)
    if (chosen.engine == BrowserEngine::Safari) {
        lines << "";
        lines << "Safari note:";
        lines << "• On macOS, Safari cookies live inside app containers.";
        lines << "• If size is 0 or read fails, grant Full Disk Access:";
        lines << "  System Settings → Privacy & Security → Full Disk Access → add Terminal and the app.";
    }
    #endif

    lines << "";
    lines << QString("Rows returned: %1").arg(cookies.size());
    m_lastDiagnostics = lines.join('\n');
    // ---- End Diagnostics ----

    if (cookies.isEmpty()) {
        showDiagnosticsDialog(m_lastDiagnostics);
    }

    // Gate deletion buttons based on store writability
    const bool safariBinary = (chosen.engine == BrowserEngine::Safari)
                        && chosen.cookieDbPath.endsWith(".binarycookies", Qt::CaseInsensitive);
    ui.pushButton_deleteSelected->setEnabled(!safariBinary);
    ui.pushButton_deletecookies->setEnabled(!safariBinary);
    if (safariBinary) {
        ui.pushButton_deleteSelected->setToolTip("Safari BinaryCookies is read-only; deletions are not supported");
        ui.pushButton_deletecookies->setToolTip("Safari BinaryCookies is read-only; deletions are not supported");
    } else {
        ui.pushButton_deleteSelected->setToolTip("Delete only the selected cookies.");
        ui.pushButton_deletecookies->setToolTip("Delete all cookies currently displayed.");
    }

    populateFromBrowser(cookies);
}

void CookieManagementSystem::filterCookiesByCategory(const QString& category) {
    // --- handle “All” first ---
    if (category == "All") {
        for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
            ui.cookieTable->setRowHidden(i, false);
        }
        ui.statusBar->showMessage(
            QString("Displaying %1 cookies in the '%2' category.")
                .arg(ui.cookieTable->rowCount())
                .arg(category)
        );
        return;
    }

    // Now filter by an actual category value (e.g. “Functional”, “Analytics”, “Marketing”)
    for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
        // Column 3 is now “Category” after we inserted the checkbox in col 0
        bool match = (ui.cookieTable->item(i, 3)->text() == category);
        ui.cookieTable->setRowHidden(i, !match);
    }

    // Count how many remain visible
    int displayedRowCount = 0;
    for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
        if (!ui.cookieTable->isRowHidden(i)) {
            displayedRowCount++;
        }
    }

    ui.statusBar->showMessage(
        QString("Displaying %1 cookies in the '%2' category.")
            .arg(displayedRowCount)
            .arg(category)
    );
}

void CookieManagementSystem::applyAnonymisationToggle() {
    ui.textSummary->clear();
    for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
        // originalCookieData[i] still holds [0]=ID, [1]=Platform, [2]=Category,
        // [3]=Cookie Name, [4]=Domain, [5]=Description, [6]=Retention, [7]=Data Controller
        QStringList original = originalCookieData[i];
        QString name = original[3];   // cookie name
        QString domain = original[4]; // domain

        if (ui.checkBox_anonymise->isChecked()) {
            name = anonymizeValue(name);
            domain = anonymizeValue(domain);
        }

        // Write back into the *table* at the correct columns:
        //   column 4 = Cookie Name
        //   column 5 = Domain
        ui.cookieTable->item(i, 4)->setText(name);
        ui.cookieTable->item(i, 5)->setText(domain);
    }

    ui.statusBar->showMessage(
        ui.checkBox_anonymise->isChecked() ? "Anonymisation applied."
                                           : "Anonymisation removed."
    );
}

void CookieManagementSystem::setupTableWidget() {
    ui.cookieTable->setColumnCount(9);
    ui.cookieTable->setHorizontalHeaderLabels({
        "Select", "ID", "Platform", "Category", "Cookie Name", "Domain",
        "Description", "Retention Period", "Data Controller"
    });

    // Hide “ID” (col 1), “Platform” (col 2), “Description” (col 6).
    ui.cookieTable->setColumnHidden(1, true);
    ui.cookieTable->setColumnHidden(2, true);
    ui.cookieTable->setColumnHidden(6, true);

    // Resize policies for better presentation:
    //  - Column 0 (Select) should be just wide enough for the checkbox:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    //  - Column 3 (Category) can also be narrow:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    //  - Column 4 (Cookie Name) should stretch to fill most of the space:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    //  - Column 5 (Domain) should also stretch but less than Cookie Name:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);

    //  - Column 7 (Retention Period) should be moderate width:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);

    //  - Column 8 (Data Controller) should be the same:
    ui.cookieTable->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);

    // (If you prefer fixed widths, you could instead call setColumnWidth(int col, int pixels).)
}

void CookieManagementSystem::deleteSelectedCookies() {
    ui.textSummary->clear();
    QVector<int> toDelete;
    int rowCount = ui.cookieTable->rowCount();

    // 1) Find checked rows
    for (int i = 0; i < rowCount; ++i) {
        QWidget* widget = ui.cookieTable->cellWidget(i, 0);
        if (!widget) continue;
        QCheckBox* cb = widget->findChild<QCheckBox*>();
        if (cb && cb->isChecked()) {
            toDelete.append(i);
        }
    if (m_currentBrowser.engine == BrowserEngine::Firefox) {
    ui.statusBar->showMessage("If rows reappear, close Firefox to unlock the DB. We match by name+host (exact or dotted).", 7000);
    }
    }

    int count = toDelete.size();
    if (count == 0) {
        ui.statusBar->showMessage("No cookies selected to delete.");
        return;
    }

    // Build list of items to delete from the browser store
    QVector<CookieItem> itemsToDelete;
    itemsToDelete.reserve(count);
    for (int row : toDelete) {
        CookieItem ci; ci.name = ui.cookieTable->item(row, 4)->text(); ci.domain = ui.cookieTable->item(row, 5)->text();
        itemsToDelete.push_back(ci);
    }

    // 2) Confirm deletion (optional)
    QString prompt = QString("Are you sure you want to delete %1 selected %2?")
        .arg(count)
        .arg(count == 1 ? "cookie" : "cookies");
    if (QMessageBox::question(this, "Confirm Delete", prompt, QMessageBox::Yes | QMessageBox::No)
        != QMessageBox::Yes) {
        ui.statusBar->showMessage("Deletion cancelled.");
        return;
    }

    if (m_currentReader) {
        QString err;
        if (!m_currentReader->deleteCookies(itemsToDelete, &err)) {
            if (!err.isEmpty())
                QMessageBox::warning(this, "Delete failed", "Could not delete from browser store: " + err);
        }
    }

    // 3) Remove in reverse so indices don’t shift out from under us
// 3) Remove in reverse so indices don’t shift
    std::sort(toDelete.begin(), toDelete.end(), std::greater<int>());
    for (int row : toDelete) {
        ui.cookieTable->removeRow(row);
        if (row < originalCookieData.size())
            originalCookieData.removeAt(row);
    }

    // Always refresh from the active source to reflect true backend state
    refreshFromCurrentSource();

    ui.statusBar->showMessage(
        QString("Deleted %1 %2.").arg(count).arg(count == 1 ? "cookie" : "cookies")
    );
}


void CookieManagementSystem::deleteAllCookies() {
    if (QMessageBox::question(this, "Confirm Delete All",
        "Are you sure you want to delete ALL cookies?") != QMessageBox::Yes) {
        ui.statusBar->showMessage("Deletion cancelled.");
        return;
    }

    // Collect all visible rows to delete
    QVector<CookieItem> itemsToDelete;
    const int rowCount = ui.cookieTable->rowCount();
    itemsToDelete.reserve(rowCount);
    for (int i = 0; i < rowCount; ++i) {
        if (ui.cookieTable->isRowHidden(i)) continue; // only visible ones
        CookieItem ci; ci.name = ui.cookieTable->item(i, 4)->text(); ci.domain = ui.cookieTable->item(i, 5)->text();
        itemsToDelete.push_back(ci);
    }

    if (m_currentReader && !itemsToDelete.isEmpty()) {
        QString err;
        if (!m_currentReader->deleteCookies(itemsToDelete, &err)) {
            if (!err.isEmpty())
                QMessageBox::warning(this, "Delete failed", "Could not delete from browser store: " + err);
        }
    }

    // Refresh from source to reflect actual backend state
    refreshFromCurrentSource();
    ui.statusBar->showMessage("All visible cookies deleted (refreshed from source).");
}

void CookieManagementSystem::filterByText(const QString& text) {
    const int rowCount = ui.cookieTable->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        bool match = ui.cookieTable->item(i, 3)->text().contains(text, Qt::CaseInsensitive)
                  || ui.cookieTable->item(i, 4)->text().contains(text, Qt::CaseInsensitive);
        ui.cookieTable->setRowHidden(i, !match);
    }
}

void CookieManagementSystem::generateSummaryReport() {
    QMap<QString, int> countByCat;
    int totalVisible = 0;
    const int rowCount = ui.cookieTable->rowCount();

    // Count visible cookies by category
    for (int i = 0; i < rowCount; ++i) {
        if (ui.cookieTable->isRowHidden(i)) continue;
        ++totalVisible;
        QString category = ui.cookieTable->item(i, 3)->text();
        countByCat[category]++;
    }

    // Build report
    QString report;

    report += "🗂️ Icon Legend:\n";
    report += "• 🔴 High Privacy Risk (e.g. Marketing)\n";
    report += "• 🟡 Moderate Privacy Risk (e.g. Analytics)\n";
    report += "• 🟢 Low Privacy Risk (e.g. Functional, Security)\n";
    report += "• ⚪ Unknown / Other Category\n\n";

    report += QString("🔎 Total Visible Cookies: %1\n\n").arg(totalVisible);
    report += "📊 Cookies by Category:\n";

    for (auto it = countByCat.constBegin(); it != countByCat.constEnd(); ++it) {
        QString icon;
        QString cat = it.key();
        if (cat.compare("Marketing", Qt::CaseInsensitive) == 0)      icon = "🔴";
        else if (cat.compare("Analytics", Qt::CaseInsensitive) == 0) icon = "🟡";
        else if (cat.compare("Functional", Qt::CaseInsensitive) == 0
              || cat.compare("Security", Qt::CaseInsensitive) == 0)     icon = "🟢";
        else                                                         icon = "⚪";

        report += QString("• %1 %2: %3\n").arg(icon, cat, QString::number(it.value()));
    }

    QString dominantCategory;
    int dominantCount = 0;
    for (auto it = countByCat.constBegin(); it != countByCat.constEnd(); ++it) {
        if (it.value() > dominantCount) {
            dominantCategory = it.key();
            dominantCount = it.value();
        }
    }

    double dominanceRatio = totalVisible > 0 ? (double)dominantCount / totalVisible : 0.0;

    QString riskLevel;
    QString riskIcon;
    if (dominantCategory.compare("Marketing", Qt::CaseInsensitive) == 0 && dominanceRatio >= 0.5) {
        riskLevel = "HIGH";
        riskIcon = "🔴";
    }
    else if (dominantCategory.compare("Analytics", Qt::CaseInsensitive) == 0 && dominanceRatio >= 0.5) {
        riskLevel = "MEDIUM";
        riskIcon = "🟡";
    }
    else {
        riskLevel = "LOW";
        riskIcon = "🟢";
    }

    report += QString("\n🛡️ Privacy Risk Meter: %1 %2\n").arg(riskIcon, riskLevel);

    report += "\n🧠 Summary & Advice:\n";
    if (riskLevel == "HIGH") {
        report += "- Your cookie profile is heavily dominated by marketing trackers.\n"
                  "  → Consider using ad blockers and rejecting third-party cookies.\n";
    }
    else if (riskLevel == "MEDIUM") {
        report += "- Most of your cookies are used for analytics.\n"
                  "  → Consider disabling tracking or using private browsing for sensitive sessions.\n";
    }
    else {
        report += "- Your cookie profile appears privacy-friendly.\n"
                  "  → Continue reviewing cookies occasionally to maintain this balance.\n";
    }

    QStringList tips = {
        "Use incognito/private mode for sensitive browsing sessions.",
        "Regularly clear your browser's cookies and cache.",
        "Install privacy-focused extensions like uBlock Origin or Privacy Badger.",
        "Avoid accepting unnecessary cookies when prompted by websites.",
        "Use a browser that blocks third-party cookies by default.",
        "Limit cookie duration by using settings or add-ons that expire them quickly.",
        "Review cookies from time to time to remove suspicious ones.",
        "Disable JavaScript on untrusted websites to avoid trackers.",
        "Use a search engine that respects privacy, like DuckDuckGo or Startpage.",
        "Consider using a VPN to mask your browsing activity."
    };

    std::shuffle(tips.begin(), tips.end(), std::default_random_engine(static_cast<unsigned>(time(nullptr))));
    QStringList selectedTips = tips.mid(0, 3);

    report += "\n🧩 Privacy Recommendations (Random Tips):\n";
    for (const QString& tip : selectedTips) {
        report += "• " + tip + "\n";
    }

    bool anonymisationUsed = ui.checkBox_anonymise->isChecked();
    int maskedCount = 0;
    bool foundAnonymised = false;
    for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
        if (ui.cookieTable->isRowHidden(i)) continue;
        QString name = ui.cookieTable->item(i, 3)->text();
        QString domain = ui.cookieTable->item(i, 4)->text();
        QString controller = ui.cookieTable->item(i, 7)->text();
        if (name.startsWith("anon_"))    { foundAnonymised = true; maskedCount++; }
        if (domain.startsWith("anon_"))  { foundAnonymised = true; maskedCount++; }
        if (controller.startsWith("anon_")) { foundAnonymised = true; maskedCount++; }
    }

    if (anonymisationUsed && foundAnonymised) {
        report += QString("\n🔐 Anonymisation Summary:\n%1 field(s) were anonymised (name, domain, or data controller).\n").arg(maskedCount);
        report += "\n🧾 Anonymisation Legend:\n";
        report += "- Cookie names like '_ga', 'sessionid', 'SID' were anonymised to protect tracking/session identifiers.\n";
        report += "- Domains with patterns like 'login', 'track', or 'auth' were anonymised to mask behaviour or authentication sources.\n";
        report += "- Known third-party controllers (e.g. Meta, Google, Facebook, TikTok) were anonymised to protect attribution metadata.\n";
    }

    ui.textSummary->setPlainText(report);
    ui.statusBar->showMessage("Generated advanced cookie summary with risk analysis.");
}

void CookieManagementSystem::showCookieGuide() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Learn About Cookies");
    dialog->resize(600, 600);

    QTextBrowser* browser = new QTextBrowser(dialog);
    browser->setOpenExternalLinks(true);
    browser->setHtml(R"(
        <h3>🍪 What Are Cookies?</h3>
        <p>Cookies are small text files stored on your device by websites you visit. They help sites remember your preferences, login info, or activity between sessions.</p>
        <h4>🔹 Types of Cookies:</h4>
        <ul>
            <li><b>Functional</b> – Required for core features like logging in or saving settings.</li>
            <li><b>Analytics</b> – Help site owners understand usage and performance trends.</li>
            <li><b>Marketing</b> – Track you across websites to deliver targeted advertisements.</li>
            <li><b>Personalization</b> – Tailor content or layout based on your behavior.</li>
            <li><b>Security</b> – Maintain safe login sessions and detect suspicious activity.</li>
        </ul>
        <h4>🔐 Privacy Considerations:</h4>
        <ul>
            <li>Third-party cookies often track you across multiple websites.</li>
            <li>Many cookies persist after you close your browser.</li>
            <li>You can clear, block, or restrict cookies from your browser settings.</li>
        </ul>
        <h4>📜 UK Legal Context:</h4>
        <p>Under UK GDPR and PECR regulations, websites must request your consent before setting non-essential cookies (e.g., analytics or marketing).</p>
        <h4>🛡️ Best Practices:</h4>
        <ul>
            <li>Regularly review and clear cookies you don’t recognize.</li>
            <li>Use incognito/private browsing for sensitive sessions.</li>
            <li>Consider browser add-ons like <i>Privacy Badger</i> or <i>uBlock Origin</i>.</li>
            <li>Disable third-party cookies where possible for better privacy.</li>
        </ul>
       <p><b>🔗 Learn more:</b> <a href='https://ico.org.uk/for-the-public/online/cookies/'>https://ico.org.uk/for-the-public/online/cookies/</a></p>
    )");

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok, dialog);
    connect(buttons, &QDialogButtonBox::accepted, dialog, &QDialog::accept);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(browser);
    layout->addWidget(buttons);

    dialog->setLayout(layout);
    dialog->exec();
}

QString CookieManagementSystem::anonymizeValue(const QString& value) {
    if (value.length() > 15 ||
        value.contains("user", Qt::CaseInsensitive) ||
        value.contains("session", Qt::CaseInsensitive) ||
        value.contains("ga", Qt::CaseInsensitive) ||
        value.contains("fbp", Qt::CaseInsensitive) ||
        value.contains("sid", Qt::CaseInsensitive) ||
        value.contains("ide", Qt::CaseInsensitive) ||
        value.contains("track", Qt::CaseInsensitive) ||
        value.contains("login", Qt::CaseInsensitive) ||
        value.contains("meta", Qt::CaseInsensitive) ||
        value.contains("facebook", Qt::CaseInsensitive) ||
        value.contains("google", Qt::CaseInsensitive) ||
        value.contains("tiktok", Qt::CaseInsensitive)) {
        return "anon_" + QString::number(qHash(value)).right(6);
    }
    return value;
}

void CookieManagementSystem::exportDatabaseToCSV() {
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Export Cookies",
        "",
        "CSV Files (*.csv)"
    );
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Export Failed", "Could not write to file.");
        return;
    }

    QTextStream out(&file);
    // Only columns 1..8 correspond to actual data fields (ID,Platform,Category,...)
    out << "ID,Platform,Category,Cookie Name,Domain,Description,Retention Period,Data Controller\n";

    int rowCount = ui.cookieTable->rowCount();
    int colCount = ui.cookieTable->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        if (ui.cookieTable->isRowHidden(row))
            continue;

        // Build a QStringList of the 8 data‐columns (skip column 0)
        QStringList cells;
        for (int col = 1; col < colCount; ++col) {
            QTableWidgetItem *item = ui.cookieTable->item(row, col);
            QString text = item ? item->text() : QString();

            // Escape any internal quotes
            text.replace("\"", "\"\"");
            cells << "\"" + text + "\"";
        }
        // Join them with commas and write one line
        out << cells.join(",") << "\n";
    }

    file.close();
    ui.statusBar->showMessage("Cookies exported to " + fileName);
}


void CookieManagementSystem::clearAllFilters() {
    ui.searchLineEdit->clear();
    ui.categoryDropdown->setCurrentIndex(0); // Assumes index 0 = "All"
    ui.checkBox_anonymise->setChecked(false);

    for (int i = 0; i < ui.cookieTable->rowCount(); ++i) {
        ui.cookieTable->setRowHidden(i, false);
    }
    ui.statusBar->showMessage("All filters cleared.");
}

void CookieManagementSystem::showDiagnosticsDialog(const QString& text) {
    QDialog* d = new QDialog(this);
    d->setWindowTitle("Import Diagnostics");
    d->resize(700, 500);

    auto *browser = new QTextBrowser(d);
    browser->setPlainText(text);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, d);
    auto *copyBtn = new QPushButton("Copy Report", d);
    buttons->addButton(copyBtn, QDialogButtonBox::ActionRole);

    connect(buttons, &QDialogButtonBox::accepted, d, &QDialog::accept);
    connect(copyBtn, &QPushButton::clicked, this, [browser]{
        QGuiApplication::clipboard()->setText(browser->toPlainText());
    });

    auto *layout = new QVBoxLayout(d);
    layout->addWidget(browser);
    layout->addWidget(buttons);
    d->setLayout(layout);
    d->exec();
}

void CookieManagementSystem::refreshFromCurrentSource() {
    if (!m_currentReader) return;
    const QVector<CookieItem> cookies = m_currentReader->readAll();

    // Apply deletion gating again based on last chosen browser
    const bool safariBinary = (m_currentBrowser.engine == BrowserEngine::Safari)
                           && m_currentBrowser.cookieDbPath.endsWith(".binarycookies", Qt::CaseInsensitive);
    ui.pushButton_deleteSelected->setEnabled(!safariBinary);
    ui.pushButton_deletecookies->setEnabled(!safariBinary);
    if (safariBinary) {
        ui.pushButton_deleteSelected->setToolTip("Safari BinaryCookies is read-only; deletions are not supported");
        ui.pushButton_deletecookies->setToolTip("Safari BinaryCookies is read-only; deletions are not supported");
    } else {
        ui.pushButton_deleteSelected->setToolTip("Delete only the selected cookies.");
        ui.pushButton_deletecookies->setToolTip("Delete all cookies currently displayed.");
    }

    populateFromBrowser(cookies);
}

CookieManagementSystem::~CookieManagementSystem() {
    
}

