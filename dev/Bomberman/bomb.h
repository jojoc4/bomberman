/**
* contains all the bombs caracteristics and the explosion mechanisme
*
* @author	Jonatan Baumgartner
* @version	0.1
*/
#ifndef BOMB_H
#define BOMB_H

#include <QPoint>
#include <QGraphicsItem>
#include <QTimer>

class Map;
class MapBloc;
class Player;
class Bomb

{
public:
    Bomb();
    Bomb(int type, int range, QPoint position, Player *pOwner);
    virtual ~Bomb();

    int getType();
    int getRange();
    int getValCounterBomb();
    QPoint getPosition();
    QGraphicsPixmapItem* getPtrItemOnScene();
    Player* getOwner();
    int getNbCycle();
    int getStepExplosion();
    bool getExploded();
    QList<QGraphicsPixmapItem*>* getItemsExplosion();
    bool getExplosionOver();

    void setPtrItemOnScene(QGraphicsPixmapItem *item);
    void incCounterBomb();
    void resetCounter();
    void postStepExplosion();
    void resetNbCycle();
    void setExploded();

    void addFireExplosion(QGraphicsPixmapItem*);

    void setExplosionOver();

    void explode();

private:
    int type;
    int range;

    int counter;
    int nbCycle;
    int stepExplosion;

    bool isExploded;
    bool isExplosionOver;

    Player* owner;

    QPoint position;
    QGraphicsPixmapItem *ptrItemOnScene;

    QList<QGraphicsPixmapItem*> bombExplosionElement;

};

#endif
