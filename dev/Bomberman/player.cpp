#include "player.h"

Player::Player()
{

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

