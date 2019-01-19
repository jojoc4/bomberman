/**
* contains all the game elements
*
* @author	Julien Chappuis
* @version	0.1
*/
#ifndef GAME_H
#define GAME_H

#include <QPoint>

class Player;
class Map;

class Game
{
public:
    Game();
    virtual ~Game();

    Player* getPlayer(bool nbPlayer) const;
    Map* getMap() const;

    bool move(QPoint newPos, short newDirection, QPoint bloc, bool nbPlayer);

private:
    int type;
    Player* player1;
    Player* player2;
    Map* map;
};

#endif
