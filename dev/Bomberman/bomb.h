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

class Bomb
{
public:
    Bomb();
    Bomb(int type, int range, QPoint position);
    virtual ~Bomb();

    static Map* ptrMap;

    int getType();
    int getRange();
    int getValCounterBomb();

    QPoint getPosition();
    QGraphicsPixmapItem* getPtrItemOnScene();

    void setPtrItemOnScene(QGraphicsPixmapItem *item);
    void incCounterBomb();
    void explode();

    void resetCounter();
    int getNbCycle();
    void postStepExplosion();
    int getStepExplosion();
    void resetNbCycle();

    void setExploded();
    bool getExploded();

    QGraphicsPixmapItem* addFireExplosion(QGraphicsPixmapItem*);
    QList<QGraphicsPixmapItem *> *getItemsExplosion();

    void setExplosionOver();
    bool getExplosionOver();


private:
    int type;
    int range;

    int counter;
    int nbCycle;
    int stepExplosion;

    bool isExploded;
    bool isExplosionOver;

    QPoint position;
    QGraphicsPixmapItem *ptrItemOnScene;

    QList<QGraphicsPixmapItem*> bombExplosionElement;
    QList<MapBloc*> destroyedBlock;
};

#endif
