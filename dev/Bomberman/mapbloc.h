#ifndef MAPBLOC_H
#define MAPBLOC_H

#include <QString>
#include <QGraphicsItem>

class MapBloc
{
public:

    MapBloc();
    MapBloc(int type);
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

#endif // MAPBLOC_H
