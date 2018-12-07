#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"
#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;

    MainWidget mw(&game);
    mw.show();

    return a.exec();
}
