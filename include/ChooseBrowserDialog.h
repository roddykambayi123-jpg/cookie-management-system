#pragma once
#include <QDialog>
#include <QVector>
#include "BrowserDetector.h"

class QListWidget;
class QDialogButtonBox;

class ChooseBrowserDialog : public QDialog {
    Q_OBJECT
public:
    explicit ChooseBrowserDialog(QWidget* parent = nullptr);
    void setBrowsers(const QVector<BrowserInfo>& browsers);
    bool hasSelection() const;
    BrowserInfo selected() const;

private:
    QListWidget* m_list = nullptr;
    QDialogButtonBox* m_box = nullptr;
    QVector<BrowserInfo> m_browsers;
};