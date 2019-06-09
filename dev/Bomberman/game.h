/**
* contains all the game elements
*
* @author	Julien Chappuis
* @version	1.0
*/
#ifndef GAME_H
#define GAME_H

#include <QPoint>

class Player;
class AI_Player;
class Map;

class Game
{
public:
    Game(short gameMode = LOCAL_1V1);
    virtual ~Game();

    Player* getPlayer(short nbPlayer) const;
    Map* getMap() const;

    bool move(QPoint newPos, short newDirection, QPoint bloc, short nbPlayer);

    enum GameMode: short
    {
        LOCAL_1V1 = 1,
        LOCAL_VS_AI = 2,
        ONLINE_MULTI = 3
    };

    short getGameMode() const;

private:
    short gameMode;
    Player* player1;
    Player* player2;
    Map* map;
};

#endif
