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
    QGraphicsPixmapItem* getPtrItemOnScene() const;

    void setType(int type);
    void setPtrItemOnScene(QGraphicsPixmapItem *item);

    void explode();

    enum BlocType: short
    {
        UNDEFINED = 0,
        INDESTRUCTIBLE=1,
        DESTRUCTIBLE=2,
        FOND=3,
        AMELIORATION_NOMBRE=4,
        BONUS=5,
        AMELIORATION_PUISSANCE=6
    };

private:
    int type = 0;
    bool traversable = false;

    QGraphicsPixmapItem *ptrItemOnScene = nullptr;
};

#endif
