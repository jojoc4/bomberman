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

    status = 0;
    nbCycle = 0;
    stepExplosion = 0;
    isExploded = false;
}

/**
 * @brief bomb constructor
 * @param type, range, line and column
 */
Bomb::Bomb(int type, int range, QPoint position)
{
    this->type=type;
    this->range=range;
    this->position=position;
    ptrItemOnScene=nullptr;
    status = 0;
    nbCycle = 0;
    stepExplosion = 0;
    isExploded = false;
}

Bomb::~Bomb(){

}
void Bomb::explode(){
    delete this;
}
void Bomb::updateStatus(){
    status++;
}

int Bomb::getStatus(){
    return status;
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

void Bomb::resetStatus(){
    status = 0;
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
QGraphicsPixmapItem* Bomb::addFireExplosion(QGraphicsPixmapItem* item){
    bombExplosionElement.push_back(item);
    return item;
}
QVector<QGraphicsPixmapItem*> Bomb::getItemsExplosion(){
    return bombExplosionElement;
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
