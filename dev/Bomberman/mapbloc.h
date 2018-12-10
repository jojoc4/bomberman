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
    ~MapBloc();

    void setType(int type);
    int getType();
    bool getTraversable();

    void explode();

    QGraphicsItem* getPtrItemOnScene();
    void setPtrItemOnScene(QGraphicsItem *item);

private:
    int type;
    bool traversable;

    QGraphicsItem *ptrItemOnScene;
};

#endif
