#include "g_homescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    G_HomeScreen w;
    w.show();

    return a.exec();
}
