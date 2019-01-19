/**
* contains all the bombs caracteristics and the explosion mechanisme
*
* @author	Jonatan Baumgartner
* @version	1
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

    int getType() const;
    int getRange() const;
    int getValCounterBomb() const;
    QPoint getPosition() const;
    QGraphicsPixmapItem* getPtrItemOnScene() const;
    Player* getOwner() const;
    int getNbCycle() const;
    int getStepExplosion() const;
    bool getExploded() const;
    QList<QGraphicsPixmapItem*>* getItemsExplosion();
    int getNbDestroyedBlock(int id) const;

    void addDestroyedBlock(int id, int value);

    void setPtrItemOnScene(QGraphicsPixmapItem *item);
    void incCounterBomb();
    void resetCounter();
    void postStepExplosion();
    void resetNbCycle();
    void setExploded();

    void addFireExplosion(QGraphicsPixmapItem*);

    void explode();

    enum type : short
    {
        BOMB = 0,
        SUPERBOMB=1
    };

    enum Direction: short
    {
        LEFT=0,
        UP=1,
        RIGHT=2,
        DOWN=3
    };

private:
    int type;
    int range;

    int counter;
    int nbCycle;
    int stepExplosion;

    int nbDestroyedBlock[4] = {0,0,0,0};

    bool isExploded;


    Player* owner;

    QPoint position;
    QGraphicsPixmapItem *ptrItemOnScene;

    QList<QGraphicsPixmapItem*> bombExplosionElement;



};

#endif
