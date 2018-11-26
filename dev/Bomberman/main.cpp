#include "g_homescreen.h"
#include "g_mapchooser.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    G_HomeScreen w;

    G_MapChooser b;

    b.show();
    w.show();

    return a.exec();
}
