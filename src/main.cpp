// main.cpp

#include "CookieManagementSystem.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CookieManagementSystem w;
    w.show();
    return a.exec();

    QFont appFont("Segoe UI", 10); // or "Poppins", "Inter", etc.
    QApplication::setFont(appFont);


}
