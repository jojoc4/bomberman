#ifndef MAP_H
#define MAP_H

#include "mapbloc.h"
#include <QString>
#include <QPoint>

class Map
{
public:
    Map();
    void readFromFile(QString path);

    MapBloc getMapBloc(QPoint bloc);
    QPoint getPlayerSpawn(bool nbPlayer) const;

private:
    MapBloc** t;
    QPoint j1;
    QPoint j2;
};

#endif // MAP_H
