#include "mapbloc.h"
//chance out of 10 to obtain this type of block after explosion
#define BONUSS 2
#define RANGEI 2
#define NUMBERI 2

/**
 * @brief MapBloc::MapBloc
 * Constructor
 */
MapBloc::MapBloc()
    : type(BlocType::UNDEFINED), traversable(false), ptrItemOnScene(nullptr)
{}

/**
 * @brief MapBloc::MapBloc(int type)
 * MapBloc constructor
 * @param type (type of bloc, see the wiki)
 */
MapBloc::MapBloc(int pType)
    : type(pType), ptrItemOnScene(nullptr)
{
    this->type = type;
    if(this->type <= BlocType::DESTRUCTIBLE)
    {
        this->traversable = false;
    }
    else
    {
        this->traversable = true;
    }

    this->seen = false;
    this->visited = false;
}

/**
 * @brief MapBloc::~MapBloc
 * Destructor
 */
MapBloc::~MapBloc()
{

}

/**
 * @brief MapBloc::setType(int type)
 * change type
 * @param type (type of bloc, see the wiki)
 */
void MapBloc::setType(int type)
{
    this->type = type;
    if(type <= BlocType::DESTRUCTIBLE)
    {
        this->traversable = false;
    }
    else
    {
        this->traversable = true;
    }
}

/**
 * @brief called when the bloc explode, changing his type in void, bonus or improvment
 */
void MapBloc::explode()
{
    ///chance out of ten to have
    int r = (qrand() % ((10 + 1) - 1) + 1);

    if(r <= BONUSS)
    {
        setType(BlocType::BONUS);
    }
    else if(r <= BONUSS+RANGEI)
    {
        setType(BlocType::UPGRADE_POWER);
    }

    else if(r <= BONUSS+RANGEI+NUMBERI)
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

bool MapBloc::AIUsable() const
{
    if(type == BACKGROUND || type == UPGRADE_NUMBER || type == UPGRADE_POWER)
        return true;
    else
        return false;
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


void MapBloc::addNeighbour(MapBloc* bloc)
{
    bool alreadyThere = false;
    for(MapBloc* m : neighbours)
    {
        if(m == bloc){
            alreadyThere = true;
            break;
        }
    }

    if(!alreadyThere)
        neighbours.push_back(bloc);
}

void MapBloc::resetNeighbours()
{
    neighbours.clear();
}

QList<MapBloc*>* MapBloc::getNeighbours()
{
    return &neighbours;
}

bool MapBloc::hasBeenSeen() const
{
    return seen;
}

bool MapBloc::hasBeenVisited() const
{
    return visited;
}

void MapBloc::setSeen(bool seen)
{
    this->seen = seen;
}

void MapBloc::setVisited(bool visited)
{
    this->visited = visited;
}
