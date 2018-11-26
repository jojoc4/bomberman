#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    G_HomeScreen w;

    G_MapChooser b;

    Game *game = new Game();
    G_Game g(game, nullptr);

    //b.show();
    //w.show();
    g.show();

    return a.exec();
}
