/**
* contains all the game elements
*
* @author	Julien Chappuis
* @version	0.1
*/
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
    virtual ~Game();

    void start();
    void keyoardThread();

    Player* getPlayer(bool nbPlayer);
    Map* getMap();

    bool move(QPoint newPos, short newDirection, QPoint bloc, bool nbPlayer);

private:
    int type;
    Player* player1;
    Player* player2;
    Map* map;
};

#endif
