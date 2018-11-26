#ifndef GAME_H
#define GAME_H

#include "player.h"

#include <QPoint>

#include "map.h"

class Player;
class Map;

class Game
{
public:
    Game();

    void start();
    void keyoardThread();

     const Player& getPlayer(const short nbPlayer)const;

     const Player& getPlayer(const bool nbPlayer) const;

private:
    int type;
    Player player1;
    Player player2;
    Map map;
};

#endif // GAME_H
