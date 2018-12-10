#include "bomb.h"

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
}

Bomb::~Bomb(){

    delete ptrItemOnScene;
    ptrItemOnScene = nullptr;
}

/**
 * @brief bomb thread started at bomb creation
 */
void Bomb::bombThread()
{
    //attendre

    //exploser
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

QGraphicsItem* Bomb::getPtrItemOnScene()
{
    return ptrItemOnScene;
}

void Bomb::setPtrItemOnScene(QGraphicsItem *item)
{
    ptrItemOnScene= item;
}
