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

    QPoint getPosition() const;
    bool isDead() const;
    int getNbBomb() const;
    int getPuissance() const;
    short getDirection() const;
    bool getVisible() const;
    bool getSuperBomb() const;
    bool getInvincible() const;
    bool getAutoDrop() const;
    bool getVisibleState() const;
    int getCptInvisibility() const;
    int getCounter() const;

    void setPuissance(int puissance);
    void receiveBomb(int nb);
    void dropBomb();
    void setPosition(QPoint np);
    void setDirection(short d);
    void setVisible(bool s);
    void setSuperBomb(bool s);
    void setInvincible(bool s);
    void setAutoDrop(bool s);
    void setVisibleState(bool state);
    void incrementCptInvisibility(int nb);
    void setCounter(int c);
    void incrementCounter();

    void giveBonus();
    void die();


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
    bool visibleState;
    int cptInvisibility;
    int counter;

    QGraphicsItem *ptrItemOnScene;

};

#endif
