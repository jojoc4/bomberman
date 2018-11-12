#ifndef MAP_H
#define MAP_H

#include "mapbloc.h"


class Map
{
public:
    Map();
    void readFromFile(QString);

private:
    MapBloc* t;
};

#endif // MAP_H
