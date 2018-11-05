#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <QPoint>

class Player;
class Map;

class Game
{
public:
    Game();

    void start();
    void keyoardThread();

private:
    int type;
    Player player1;
    Player player2;
    Map map;
};

#endif // GAME_H
