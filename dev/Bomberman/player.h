#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>
#include <QGraphicsItem>

class QPoint;

class Player
{
public:
    Player();

    void moveThread();
    void manualMove();
    QPoint getPosition();
    int getAngle();
    bool isDead();
    int getNbBomb();
    int getPuissance();
    void setPuissance(int puissance);
    void setNbBomb(int nbBombe);
    void setPosition(QPoint np);
    void setAngle(int angle);

    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene();

private :
    QPoint position;
    int angle;
    bool dead;
    int nbBombe;
    int puissance;

    QGraphicsItem *ptrItemOnScene;

};

#endif // PLAYER_H
