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

private:
    MapBloc** t;
};

#endif // MAP_H
