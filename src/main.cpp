// main.cpp

#include "CookieManagementSystem.h"
#include <QApplication>
#include <QFont>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application font (before showing the window)
    QFont appFont("Segoe UI", 10); // or "Poppins", "Inter", etc.
    QApplication::setFont(appFont);

    // Set application icon
    a.setWindowIcon(QIcon(":/resources/cookie.jpg"));

    CookieManagementSystem w;
    w.show();

    return a.exec();
}