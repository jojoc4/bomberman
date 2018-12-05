#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    G_HomeScreen w;

    Game game;

    G_MapChooser b(&game);

    G_Game g(&game, nullptr);

    b.show();
    w.show();
    g.show();

    return a.exec();
}
