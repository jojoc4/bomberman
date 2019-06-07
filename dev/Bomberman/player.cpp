#include "player.h"

#define INVIS 5
#define INVINC 1
#define SB 1
#define MAX_INVISIBLE_COUNTER 2000
#define MAX_VISIBLE_COUNTER 500

/**
 * @brief Player::Player
 * Constructor
 */
Player::Player()
    : dead(false), nbBombe(1), puissance(1), direction(0), superBomb(false), visible(false),
      invincible(false), autoDrop(false), visibleState(false), cptInvisibility(0), ptrItemOnScene(nullptr)
{
    position = QPoint(0,0);
}

Player::~Player()
{
    ptrItemOnScene = nullptr;
}

/**
 * @brief Player::giveBonus
 * Give a bonus in term of luck
 */
void Player::giveBonus()
{
    visible=false;
    invincible=false;
    superBomb=false;
    autoDrop=false;
    counter=0;

    int r = (qrand() % ((10 + 1) - 1) + 1);


    if(r<=INVIS)
    {
        //visible=true;
    }else if(r<=INVIS+INVINC)
    {
        invincible=true;
    }else if(r<=INVIS+INVINC+SB)
    {
        superBomb=true;
    }else
    {
        //autoDrop=true;
    }
}

/**
 * @brief Player::getPosition
 * @return  position of the player
 */
QPoint Player::getPosition() const
{
    return position;
}

/**
 * @brief Player::isDead
 * @return is the player dead
 */
bool Player::isDead() const
{
    return dead;
}

/**
 * @brief Player::getNbBomb
 * @return nb of bomb owned by the player
 */
int Player::getNbBomb() const
{
    return nbBombe;
}

/**
 * @brief Player::getPuissance
 * @return power for the bomb
 */
int Player::getPuissance() const
{
    return puissance;
}

/**
 * @brief Player::getDirection
 * @return direction of the player
 */
short Player::getDirection() const
{
    return this->direction;
}

/**
 * @brief Player::setPuissance
 * @param power : new power for the bombs
 */
void Player::setPuissance(int power)
{
    this->puissance=power;
}

/**
 * @brief Player::receiveBomb
 * Increase the number of available bomb
 * @param nb number of bomb receveid by the player
 */
void Player::receiveBomb(int nb)
{
    nbBombe += nb;
}

/**
 * @brief Player::dropBomb
 * Drop the bomb and decrement
 */

void Player::dropBomb()
{
    --nbBombe;
}

/**
 * @brief Player::setPosition
 * @param np new position where the player is supposed to be
 */
void Player::setPosition(QPoint np)
{
    position=np;
}
/**
 * @brief Player::getPtrItemOnScene
 * @return pointer of the pixmap
 */
QGraphicsPixmapItem* Player::getPtrItemOnScene() const
{
    return ptrItemOnScene;
}

/**
 * @brief Player::setPtrItemOnScene
 * @param item to add to the scene
 */
void Player::setPtrItemOnScene(QGraphicsPixmapItem *item)
{
    ptrItemOnScene = item;
}

/**
 * @brief Player::setDirection
 * @param d
 *
 */
void Player::setDirection(short d)
{
    if(d>=0 && d<4)
        this->direction = d;
}

/**
 * @brief Player::getVisible
 * @return boolean if player is visible
 */
bool Player::getVisible() const
{
    return visible;
}

/**
 * @brief Player::getSuperBomb
 * @return if the player has the opportunity to drop a superbomb
 */
bool Player::getSuperBomb() const
{
    return superBomb;
}

/**
 * @brief Player::getInvincible
 * @return boolean if the player cannot be kill
 */
bool Player::getInvincible() const
{
    return invincible;
}

/**
 * @brief Player::getAutoDrop
 * @return
 */
bool Player::getAutoDrop() const
{
    return autoDrop;
}

/**
 * @brief Player::getCounter
 * @return counter
 */
int Player::getCounter() const
{
    return counter;
}

/**
 * @brief Player::setVisible
 * @param s : boolean if the player use visible improvement or not
 */
void Player::setVisible(bool s)
{
    visible = s;
}

/**
 * @brief Player::setSuperBomb
 * @param s : number of superbomb
 */
void Player::setSuperBomb(bool s)
{
    superBomb = s;
}

/**
 * @brief Player::setInvincible
 * @param s boolean if the player is invincible
 */
void Player::setInvincible(bool s)
{
    invincible = s;
}

/**
 * @brief Player::setAutoDrop
 * @param s
 */
void Player::setAutoDrop(bool s){
    autoDrop = s;
}

/**
 * @brief Player::getVisibleState
 * @return if the player is visible or not
 */
bool Player::getVisibleState() const
{
    return this->visibleState;
}

/**
 * @brief Player::getCptInvisibility
 * @return the value of the counter into the player for the invisbility
 */
int Player::getCptInvisibility() const
{
    return this->cptInvisibility;
}

/**
 * @brief Player::setVisibleState
 * @param state
 */
void Player::setVisibleState(bool state)
{
    this->visibleState = state;
}

/**
 * @brief Player::incrementCptInvisibility
 * @param nb
 */
void Player::incrementCptInvisibility(int nb)
{
    cptInvisibility += nb;
    if(this->visibleState)
    {
        if(cptInvisibility >= MAX_VISIBLE_COUNTER)
        {
            this->visibleState = false;
            cptInvisibility = 0;
        }
    }
    else
    {
        if(cptInvisibility >= MAX_INVISIBLE_COUNTER)
        {
            this->visibleState = true;
            cptInvisibility = 0;
        }
    }
}

/**
 * @brief Player::setCounter
 * @param c
 */
void Player::setCounter(int c)
{
    counter = c;
}

/**
 * @brief Player::incrementCounter
 */
void Player::incrementCounter()
{
    ++counter;
}

/**
 * @brief Player::die
 * put the boolean dead true because the player died
 */
void Player::die()
{
    dead = true;
}

