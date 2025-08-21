#include "ChooseBrowserDialog.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QPushButton>

ChooseBrowserDialog::ChooseBrowserDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Import Cookies – Pick a browser");

    auto* lay = new QVBoxLayout(this);

    m_list = new QListWidget(this);
    m_list->setSelectionMode(QAbstractItemView::SingleSelection);
    lay->addWidget(m_list);

    m_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    m_box->button(QDialogButtonBox::Ok)->setText("Import");
    m_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    lay->addWidget(m_box);

    connect(m_list, &QListWidget::itemSelectionChanged, this, [this]{
        m_box->button(QDialogButtonBox::Ok)->setEnabled(m_list->currentRow() >= 0);
    });
    connect(m_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ChooseBrowserDialog::setBrowsers(const QVector<BrowserInfo>& browsers) {
    m_browsers = browsers;
    m_list->clear();
    for (const auto& b : m_browsers) {
        m_list->addItem(b.name + "  [" + b.profilePath + "]");
    }
}

bool ChooseBrowserDialog::hasSelection() const {
    return m_list->currentRow() >= 0;
}

BrowserInfo ChooseBrowserDialog::selected() const {
    int i = m_list->currentRow();
    if (i < 0 || i >= m_browsers.size()) return BrowserInfo{};
    return m_browsers[i];
}