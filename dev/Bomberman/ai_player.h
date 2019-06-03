#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "player.h"
#include <QWidget>
#include <QList>

class Game;
class MapBloc;
class QKeyEvent;

class AI_Player : public Player
{
public:
    AI_Player(Game* p_game, Player* p_opponent, QPoint pos);
    virtual ~AI_Player();

    bool isOnNextPosition();
    QPoint getNextPosition() const;

    void init();

    virtual void act(QWidget* widget);

private:
    Game* game;
    Player* opponent;
    QList<MapBloc*>* path;
    QPoint positionToReach;
    bool reached;

    MapBloc* previousBlocP1;
    MapBloc* previousBlocP2;

    QKeyEvent* nextMovement();
};


#endif // AI_PLAYER_H
