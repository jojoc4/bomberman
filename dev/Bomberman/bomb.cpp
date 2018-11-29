/**
 * @author Jonatan Baumgartner
 */
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
