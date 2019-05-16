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

    MapBloc(char* n);
    MapBloc(int type);
    virtual ~MapBloc();

    int getType() const;
    bool getTraversable() const;
    QGraphicsPixmapItem* getPtrItemOnScene() const;

    void setType(int type);
    void setPtrItemOnScene(QGraphicsPixmapItem *item);

    void explode();

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

    char* getNom() const {return nom;}

private:
    int type = 0;
    bool traversable = false;
    char* nom;

    QGraphicsPixmapItem *ptrItemOnScene = nullptr;

    //DIJKSTRA PATHFINDING
    bool seen;
    bool visited;
    QList<MapBloc*> neighbours;
};



#endif
