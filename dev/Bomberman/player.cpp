#include "player.h"

Player::Player()
{
    position = QPoint(0,0);
    angle = 0;
    dead = false;
    nbBombe = 1;
    puissance = 1;
}
void Player::manualMove()
{

}

void Player::moveThread()
{

}


QPoint Player::getPosition(){
    return position;
}

int Player::getAngle(){
    return angle;
}

bool Player::isDead(){
    return dead;
}

int Player::getNbBomb(){
    return nbBombe;
}

int Player::getPuissance(){
    return puissance;
}

