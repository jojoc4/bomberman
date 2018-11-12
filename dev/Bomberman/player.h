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

private :
    QPoint position;
    int angle;
    bool dead;
    int nbBombe;


};

#endif // PLAYER_H
