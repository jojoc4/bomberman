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
    QList<QPoint*>* goBackPath;
    int index;
    bool goingBack;

    QPoint* positionToReach;
    bool reached;
    bool playing;
    int counterCenter;

    MapBloc* previousBlocP1;
    MapBloc* previousBlocP2;
    QMutex* mutex;
    QMutex* pathMutex;

    bool isOnNextPosition();

    QKeyEvent* nextMovement();
    void goToSafety();
    void stopMoving();
};


#endif // AI_PLAYER_H
