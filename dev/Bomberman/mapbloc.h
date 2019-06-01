/**
* represents the bloc of a map
*
* @author	Jonatan Baumgartner
* @version	1.0
*/
#ifndef MAPBLOC_H
#define MAPBLOC_H

#include <QString>
#include <QGraphicsItem>

class MapBloc
{
public:

    MapBloc();
    MapBloc(int type);
    virtual ~MapBloc();

    int getType() const;
    bool getTraversable() const;
    bool AIUsable() const;
    QGraphicsPixmapItem* getPtrItemOnScene() const;

    void setType(int type);
    void setPtrItemOnScene(QGraphicsPixmapItem *item);

    void addNeighbour(MapBloc* bloc);
    void resetNeighbours();
    QList<MapBloc*>* getNeighbours();

    void setSeen(bool seen);
    void setVisited(bool visited);

    bool hasBeenSeen() const;
    bool hasBeenVisited() const;

    void explode();

    void setPosition(QPoint);
    QPoint getPosition();

    enum BlocType: short
    {
        UNDEFINED = 0,
        INDESTRUCTIBLE=1,
        DESTRUCTIBLE=2,
        BACKGROUND=3,
        UPGRADE_NUMBER=4,
        BONUS=5,
        UPGRADE_POWER=6
    };

private:
    int type = 0;
    bool traversable = false;

    QGraphicsPixmapItem *ptrItemOnScene = nullptr;

    QPoint position;

    //DIJKSTRA PATHFINDING
    bool seen;
    bool visited;
    QList<MapBloc*> neighbours;
};



#endif
