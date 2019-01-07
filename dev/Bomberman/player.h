/**
* contains the player infos
*
* @author	Julien Chappuis
* @version	1.0
*/
#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>
#include <QGraphicsItem>

class QPoint;

class Player
{
public:

    enum Direction: short
    {
        LEFT=0,
        UP=1,
        RIGHT=2,
        DOWN=3
    };

    Player();

    void moveThread();
    void manualMove();

    QPoint getPosition() const;
    bool isDead() const;
    int getNbBomb() const;
    int getPuissance() const;
    short getDirection() const;
    bool getVisible() const;
    bool getSuperBomb() const;
    bool getInvincible() const;
    bool getAutoDrop() const;

    void setPuissance(int puissance);
    void receiveBomb(int nb);
    void dropBomb();
    void setPosition(QPoint np);
    void setDirection(short d);
    void setVisible(bool s);
    void setSuperBomb(bool s);
    void setInvincible(bool s);
    void setAutoDrop(bool s);

    void giveBonus();

    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene(QGraphicsItem *item);

private :
    QPoint position;
    bool dead;
    int nbBombe;
    int puissance;
    short direction;
    bool superBomb;
    bool visible;
    bool invincible;
    bool autoDrop;

    QGraphicsItem *ptrItemOnScene;

};

#endif
