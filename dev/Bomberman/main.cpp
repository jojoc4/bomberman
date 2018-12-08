#include "game.h"
#include "g_mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;

    G_MainWidget mw(&game);
    mw.show();

    return a.exec();
}
