/**
* contains the current map infos and can load a map from file
*
* @author	Jonatan Baumgartner
* @version	1.0
*/
#ifndef MAP_H
#define MAP_H

#include <QString>
#include <QPoint>

class MapBloc;

class Map
{
public:
    Map();
    virtual ~Map();
    void readFromFile(QString path);

    MapBloc* getMapBloc(QPoint bloc) const;
    QPoint getPlayerSpawn(bool nbPlayer) const;

private:
    MapBloc*** t;
    QPoint j1;
    QPoint j2;
};

#endif
