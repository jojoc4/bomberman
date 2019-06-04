#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include "player.h"
#include <QList>

class Game;
class MapBloc;
class QKeyEvent;
class QMutex;

class AI_Player : public QThread, public Player
{
    Q_OBJECT
public:
    AI_Player(Game* p_game, Player* p_opponent, QPoint pos);
    virtual ~AI_Player();

    void togglePlaying();
    bool isPlaying() const;

    QPoint getNextPosition() const;

    void setGameWidget(QWidget* gameWidget);

    void init();

    virtual void act() override;

signals:
    void AIEvent(QKeyEvent *event);

protected:
    virtual void run() override;

private:
    Game* game;
    QWidget* gameWidget;
    Player* opponent;
    QList<MapBloc*>* path;
    QPoint positionToReach;
    bool reached;
    bool playing;

    MapBloc* previousBlocP1;
    MapBloc* previousBlocP2;
    QMutex* mutex;
    QMutex* pathMutex;

    bool isOnNextPosition();

    QKeyEvent* nextMovement();
};


#endif // AI_PLAYER_H
