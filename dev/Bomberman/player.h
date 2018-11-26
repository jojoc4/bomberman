#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>

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

private :
    QPoint position;
    int angle;
    bool dead;
    int nbBombe;
    int puissance;

};

#endif // PLAYER_H
