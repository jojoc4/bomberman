#include "bomb.h"
#include "bomb.h"

#define LEFT_EXPLOSION_BLOC_ARRAY 0
#define TOP_EXPLOSION_BLOC_ARRAY 3

/**
 * Bomb()
 * bomb default constructor
 */
Bomb::Bomb()
    : type(BOMB), range(0), counter(0), nbCycle(0), stepExplosion(0), isExploded(false),
      owner(nullptr), position(QPoint(0,0)), ptrItemOnScene(nullptr)
{}

/**
 * Bomb(int type, int range, QPoint position, Player* pOwner)
 * bomb constructor
 * @param type, range, position, and owner
 */
Bomb::Bomb(int pType, int pRange, QPoint pPosition, Player* pOwner)
    : type(pType), range(pRange), counter(0), nbCycle(0), stepExplosion(0), isExploded(false),
      owner(pOwner), position(pPosition), ptrItemOnScene(nullptr)
{}

/**
 * ~Bomb
 * Destroy the bomb
 */
Bomb::~Bomb()
{

}

/**
 * explode()
 * Destroy the bomb after explosion
 */
void Bomb::explode()
{
    delete this;
}

/**
 * incCounterBomb()
 * Increase the counter of the bomb for animation state
 */
void Bomb::incCounterBomb()
{
    counter++;
}

/**
 * int getValCounterBomb()
 * @return value counter
 */
int Bomb::getValCounterBomb() const
{
    return counter;
}

/**
 * int getType()
 * @return type of the bomb
 */
int Bomb::getType() const
{
    return type;
}

/**
 * int getRange()
 * @return the power
 */
int Bomb::getRange() const
{
    return range;
}

/**
 * QPoint getPosition()
 * @return position of the bomb
 */
QPoint Bomb::getPosition() const
{
    return position;
}

/**
 * QGraphicsPixmapItem* getPtrItemOnScene()
 * @return QGraphicsPixmapItem*
 */
QGraphicsPixmapItem *Bomb::getPtrItemOnScene() const
{
    return ptrItemOnScene;
}

/**
 * setPtrItemOnScene
 * set the pixmap to display this object
 * @param item : QGrahicsPixmapItem
 */
void Bomb::setPtrItemOnScene(QGraphicsPixmapItem *item)
{
    ptrItemOnScene= item;
}

/**
 * resetCounter()
 * reset the counter and increment the nb_Cycle
 */
void Bomb::resetCounter()
{
    counter = 0;
    nbCycle++;
}

/**
 * getNbCycle
 * @return number of cycle
 */
int Bomb::getNbCycle() const
{
    return nbCycle;
}

/**
 * postStepExplosion()
 * increment the step for the animation of explosion
 */
void Bomb::postStepExplosion()
{
    stepExplosion++;
}

/**
 * int getStepExplosion()
 * get the value of stepExplosion
 * @return stepExplosion
 */
int Bomb::getStepExplosion() const
{
    return stepExplosion;
}

/**
 * addFireExplosion()
 * add a QGrapicsPixmaptem into the Qlist to display the explosion flame
 * @param item to add
 */
void Bomb::addFireExplosion(QGraphicsPixmapItem* item)
{
    bombExplosionElement.push_back(item);
}

/**
 * getItemsExplosion()
 * @return QList<QGrapicsPixmapItem*>* pointer of the list who contains the PixmapItem of the
 * explosion
 */
QList<QGraphicsPixmapItem*>* Bomb::getItemsExplosion()
{
    return &bombExplosionElement;
}

/**
 * resetNbCycle()
 * reset the number of cycle to zero
 */
void Bomb::resetNbCycle()
{
    nbCycle = 0;
}

/**
 * setExploded()
 * Set the status of the bomb exploded
 */
void Bomb::setExploded()
{
    isExploded = true;
}

/**
 * bool getExploded()
 * @return boolean if the bomb has exploded or not
 */
bool Bomb::getExploded() const
{
    return isExploded;
}

/**
 * getOwner()
 * getter
 * @return a pointer a pointer of the player who has drop the bomb
 */
Player* Bomb::getOwner() const
{
    return owner;
}

/**
 * getNbDestroyedBlock()
 * get the value of the pointed block
 * @param id index of the array
 * @return value pointed of the index
 */
int Bomb::getNbDestroyedBlock(int id) const
{
    if(id <= TOP_EXPLOSION_BLOC_ARRAY && id >= LEFT_EXPLOSION_BLOC_ARRAY)
        return nbDestroyedBlock[id];
    else
        return 0;
}

/**
 * addDestroyedBlock(int id, int value)
 * @param id index of the array
 * @param value to add into the array at the id index
 */
void Bomb::addDestroyedBlock(int id, int value)
{
    if(id <= TOP_EXPLOSION_BLOC_ARRAY && id >= LEFT_EXPLOSION_BLOC_ARRAY)
        nbDestroyedBlock[id] = value;
}
