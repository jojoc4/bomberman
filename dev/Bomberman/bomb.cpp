#include "bomb.h"
#include "bomb.h"



/**
 * @brief bomb default constructor
 */
Bomb::Bomb()
{
    this->type=0;
    this->range=0;
    this->position=QPoint(0,0);
    ptrItemOnScene=nullptr;
    owner = nullptr;

    counter = 0;
    nbCycle = 0;
    stepExplosion = 0;
    isExploded = false;
    isExplosionOver = false;


}

/**
 * @brief bomb constructor
 * @param type, range, line and column
 */
Bomb::Bomb(int type, int range, QPoint position, Player* pOwner)
{
    this->type=type;
    this->range=range;
    this->position=position;
    ptrItemOnScene=nullptr;
    counter = 0;
    nbCycle = 0;
    stepExplosion = 0;
    isExploded = false;
    isExplosionOver = false;

    owner = pOwner;
}

Bomb::~Bomb(){

}
void Bomb::explode(){
    delete this;
}
void Bomb::incCounterBomb(){
    counter++;
}

int Bomb::getValCounterBomb(){
    return counter;
}
int Bomb::getType(){
    return type;
}

int Bomb::getRange(){
    return range;
}

QPoint Bomb::getPosition(){
    return position;
}

QGraphicsPixmapItem *Bomb::getPtrItemOnScene()
{
    return ptrItemOnScene;
}

void Bomb::setPtrItemOnScene(QGraphicsPixmapItem *item)
{
    ptrItemOnScene= item;
}

void Bomb::resetCounter(){
    counter = 0;
    nbCycle++;
}

int Bomb::getNbCycle(){
    return nbCycle;
}
void Bomb::postStepExplosion()
{
    stepExplosion++;
}
int Bomb::getStepExplosion()
{
    return stepExplosion;
}
void Bomb::addFireExplosion(QGraphicsPixmapItem* item){
    bombExplosionElement.push_back(item);
}
QList<QGraphicsPixmapItem*>* Bomb::getItemsExplosion(){
    return &bombExplosionElement;
}

void Bomb::resetNbCycle()
{
    nbCycle = 0;
}
void Bomb::setExploded(){
    isExploded = true;
}
bool Bomb::getExploded(){
    return isExploded;
}

void Bomb::setExplosionOver()
{
    isExplosionOver = true;
}

bool Bomb::getExplosionOver()
{
    return isExplosionOver;
}
Player* Bomb::getOwner(){
    return owner;
}
int Bomb::getNbDestroyedBlock(int id){
    if(id < 4 && id >= 0)
        return nbDestroyedBlock[id];
    else
        return 0;
}

void Bomb::addDestroyedBlock(int id, int value){
    if(id < 4 && id >= 0)
        nbDestroyedBlock[id] = value;
}
