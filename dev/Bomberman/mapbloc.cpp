#include "mapbloc.h"

/**
 * @brief MapBloc::MapBloc
 * Constructor
 */
MapBloc::MapBloc()
{
    ptrItemOnScene = nullptr;
    type = BlocType::UNDEFINED;
    traversable = false;
}

/**
 * @brief MapBloc constructor
 * @param type (type of bloc, see the wiki)
 */
MapBloc::MapBloc(int type)
{
    this->type=type;
    if(this->type<=BlocType::DESTRUCTIBLE)
    {
        this->traversable=false;
    }
    else
    {
        this->traversable=true;
    }
    ptrItemOnScene=nullptr;
}

/**
 * @brief MapBloc::~MapBloc
 * Destructor
 */
MapBloc::~MapBloc()
{

}

/**
 * @brief change type
 * @param type (type of bloc, see the wiki)
 */
void MapBloc::setType(int type)
{
    this->type=type;
    if(type<=2)
    {
        this->traversable=false;
    }
    else
    {
        this->traversable=true;
    }
}

/**
 * @brief called when the bloc explode, changing his type in void, bonus or improvment
 */
void MapBloc::explode()
{
    ///chance out of ten to have
    int bonus=2;
    int rangeImprovment=2;
    int numberImprovment=2;

    int r = (qrand() % ((10 + 1) - 1) + 1);
    if(r<=bonus)
    {
        setType(BlocType::BONUS);
    }
    else if(r<=bonus+rangeImprovment)
    {
        setType(BlocType::UPGRADE_POWER);
    }

    else if(r<=bonus+rangeImprovment+numberImprovment)
    {
        setType(BlocType::UPGRADE_NUMBER);
    }
    else
    {
        setType(BlocType::BACKGROUND);
    }
}
/**
 * @brief MapBloc::getType
 * @return
 */
int MapBloc::getType() const
{
    return this->type;
}
/**
 * @brief MapBloc::getTraversable
 * @return
 */
bool MapBloc::getTraversable() const
{
    return this->traversable;
}
/**
 * @brief MapBloc::getPtrItemOnScene
 * @return
 */
QGraphicsPixmapItem* MapBloc::getPtrItemOnScene() const
{
    return ptrItemOnScene;
}
/**
 * @brief MapBloc::setPtrItemOnScene
 * @param item
 */
void MapBloc::setPtrItemOnScene(QGraphicsPixmapItem *item)
{
    ptrItemOnScene= item;
}
