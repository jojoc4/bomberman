#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "player.h"
#include <QList>

class Game;
class MapBloc;

class AI_Player : public Player
{
public:
    AI_Player(Game* p_game, Player* p_opponent, QPoint pos);


    bool isOnNextPosition();
    QPoint getNextPosition();

    void init();

private:
    Game* game;
    Player* opponent;
    QList<MapBloc*>* path;

    QPoint positionToReach;
    int index = 0;
};

#endif // AI_PLAYER_H
