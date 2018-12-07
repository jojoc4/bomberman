/**
 * @author Julien Chappuis
 */
#include "player.h"

Player::Player()
{
    position = QPoint(0,0);
    dead = false;
    nbBombe = 1;
    puissance = 1;
    ptrItemOnScene = nullptr;
    direction = 0;
}

void Player::manualMove()
{

}

void Player::moveThread()
{

}

QPoint Player::getPosition() const {
    return position;
}

bool Player::isDead() const {
    return dead;
}

int Player::getNbBomb() const {
    return nbBombe;
}

int Player::getPuissance() const {
    return puissance;
}

short Player::getDirection() const
{
    return this->direction;
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

QGraphicsItem* Player::getPtrItemOnScene()
{
    return ptrItemOnScene;
}

void Player::setPtrItemOnScene(QGraphicsItem *item)
{
    ptrItemOnScene= item;
}

void Player::setDirection(short d)
{
    if(d>=0 && d<4)
        this->direction = d;
}
