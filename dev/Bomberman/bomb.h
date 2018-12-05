#ifndef BOMB_H
#define BOMB_H

#include <QPoint>
#include <QGraphicsItem>

class Bomb
{
public:
    Bomb(int type, int range, QPoint position);

    void bombThread();

    int getType();
    int getRange();
    QPoint getPosition();
    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene();

private:
    int type;
    int range;
    QPoint position;
    QGraphicsItem *ptrItemOnScene;
};

#endif // BOMB_H
