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

class Bomb
{
public:
    Bomb();
    Bomb(int type, int range, QPoint position);
    virtual ~Bomb();

    void bombThread();

    int getType();
    int getRange();
    QPoint getPosition();
    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene(QGraphicsItem *item);

private:
    int type;
    int range;
    QPoint position;
    QGraphicsItem *ptrItemOnScene;
};

#endif
