#include "game.h"
#include "g_mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    G_MainWidget mw(nullptr);
    mw.show();

    return a.exec();
}
