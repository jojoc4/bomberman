#ifndef BOMB_H
#define BOMB_H

#include <QPoint>

class Bomb
{
public:
    Bomb(int type, int range, QPoint position);

    void bombThread();

    int getType();
    int getRange();
    QPoint getPosition();

private:
    int type;
    int range;
    QPoint position;
};

#endif // BOMB_H
