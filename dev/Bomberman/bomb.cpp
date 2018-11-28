/**
 * @author Jonatan Baumgartner
 */
#include "bomb.h"

/**
 * @brief bomb constructor
 * @param type, range, line and column
 */
Bomb::Bomb(int t, int r, int l, int c)
{
    type=t;
    range=r;
    line=l;
    column=c;
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

int Bomb::getLine(){
    return line;
}

int Bomb::getColumn(){
    return column;
}
