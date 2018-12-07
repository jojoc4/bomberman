#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>
#include <QGraphicsItem>

class QPoint;

enum class Direction: short
{
    LEFT=0,
    UP=1,
    RIGHT=2,
    DOWN=3
};

class Player
{
public:
    Player();

    void moveThread();
    void manualMove();

    QPoint getPosition() const;
    bool isDead() const;
    int getNbBomb() const;
    int getPuissance() const;
    short getDirection() const;

    void setPuissance(int puissance);
    void setNbBomb(int nbBombe);
    void setPosition(QPoint np);
    void setDirection(short d);

    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene(QGraphicsItem *item);

private :
    QPoint position;
    bool dead;
    int nbBombe;
    int puissance;
    short direction;

    QGraphicsItem *ptrItemOnScene;

};

#endif // PLAYER_H
