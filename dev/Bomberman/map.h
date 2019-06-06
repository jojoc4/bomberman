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
#include <QMutex>
#include <QReadWriteLock>
#include "priorityqueue.h"

class MapBloc;

class Map : public QThread
{
    Q_OBJECT
public:
    Map();
    virtual ~Map();
    void readFromFile(QString path);

    MapBloc* getMapBloc(QPoint bloc) const;
    QPoint getPlayerSpawn(bool nbPlayer) const;

    QList<MapBloc*>* getShortestPath(MapBloc* from, MapBloc* destination);

protected:
    virtual void run() override;

private:
    MapBloc*** tabMapBlocs;
    QPoint p1;
    QPoint p2;

    //QMutex* mutex;
    QReadWriteLock* lock;

    void buildGraph();
};

#endif
