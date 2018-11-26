#ifndef MAP_H
#define MAP_H

#include "mapbloc.h"
#include <QString>

class Map
{
public:
    Map();
    void readFromFile(QString);

    int j1x,j1y,j2x,j2y;

    MapBloc getMapBloc(int l, int c);
    int getJ1x();
    int getJ1y();
    int getJ2x();
    int getJ2y();

private:
    MapBloc** t;
};

#endif // MAP_H
