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

    void setType(int type);
    int getType();
    bool getTraversable();
    QGraphicsPixmapItem* getPtrItemOnScene();

    void explode();

    void setPtrItemOnScene(QGraphicsPixmapItem *item);

private:
    int type;
    bool traversable;

    QGraphicsPixmapItem *ptrItemOnScene;
};

#endif
