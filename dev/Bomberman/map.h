#ifndef MAP_H
#define MAP_H

#include "mapbloc.h"
#include <QString>
#include <QPoint>

class Map
{
public:
    Map();
    void readFromFile(QString);

    MapBloc getMapBloc(int l, int c);
    QPoint getJ1();
    QPoint getJ2();

private:
    MapBloc** t;
    QPoint j1;
    QPoint j2;
};

#endif // MAP_H
