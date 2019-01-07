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

void Player::giveBonus(){
    visible=false;
    invincible=false;
    superBomb=false;
    autoDrop=false;

    int r = (qrand() % ((10 + 1) - 1) + 1);
    ///chance out of ten to have
    int invis=5;
    int invinc=1;
    int sb=1;

    if(r<=invis){
        visible=true;
    }else if(r<=invis+invinc){
        invincible=true;
    }else if(r<=invis+invinc+sb){
        superBomb=true;
    }else{
        autoDrop=true;
    }
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

void Player::receiveBomb(int nb)
{
    nbBombe += nb;
}

void Player::dropBomb()
{
    --nbBombe;
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


bool Player::getVisible() const{
    return visible;
}

bool Player::getSuperBomb() const{
    return superBomb;
}

bool Player::getInvincible() const{
    return invincible;
}

bool Player::getAutoDrop() const{
    return autoDrop;
}


void Player::setVisible(bool s){
    visible = s;
}

void Player::setSuperBomb(bool s){
    superBomb = s;
}

void Player::setInvincible(bool s){
    invincible = s;
}

void Player::setAutoDrop(bool s){
    autoDrop = s;
}
