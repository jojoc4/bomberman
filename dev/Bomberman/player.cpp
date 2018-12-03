/**
 * @author Julien Chappuis
 */
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

void Player::setPuissance(int puissance){
    this->puissance=puissance;
}

void Player::setNbBomb(int nbBombe){
    this->nbBombe=nbBombe;
}

void Player::setPosition(QPoint np){
    position=np;
}

void Player::setAngle(int angle){
    this->angle=angle;
}
