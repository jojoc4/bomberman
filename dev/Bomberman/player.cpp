#include "player.h"

Player::Player() : dead(false), nbBombe(1), puissance(1), direction(0), superBomb(false), visible(false),
                    invincible(false), autoDrop(false), visibleState(false), cptInvisibility(0), ptrItemOnScene(nullptr)
{
    position = QPoint(0,0);
}

void Player::giveBonus(){
    visible=false;
    invincible=false;
    superBomb=false;
    autoDrop=false;
    counter=0;

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

int Player::getCounter() const{
    return counter;
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

bool Player::getVisibleState() const
{
    return this->visibleState;
}

int Player::getCptInvisibility() const
{
    return this->cptInvisibility;
}

void Player::setVisibleState(bool state)
{
    this->visibleState = state;
}

void Player::incrementCptInvisibility(int nb)
{
    cptInvisibility += nb;
    if(this->visibleState)
    {
        if(cptInvisibility >= 500)
        {
            this->visibleState = false;
            cptInvisibility = 0;
        }
    }
    else
    {
        if(cptInvisibility >= 2000)
        {
            this->visibleState = true;
            cptInvisibility = 0;
        }
    }
}

void Player::setCounter(int c)
{
    counter = c;
}

void Player::incrementCounter()
{
    ++counter;
}
