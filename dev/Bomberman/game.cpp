#include "game.h"

/**
 * @brief Game::Game
 * Constructor
 */
Game::Game()
{
    player1 = new Player();
    player2 = new Player();
    map = new Map();
}
/**
 * @brief Game::~Game
 * Destructor
 */
Game::~Game()
{
    delete player1;
    delete player2;
    delete map;
}


/**
* @brief Game::canMove make the changes in the player and in the map if it is a special bloc
* @param newPos : new position of the player
* @param newDirection : direction of the player
* @param bloc : bloc where the player is staying at the moment
* @param nbPlayer : which player
* @return false if impossible move, else true
*/
bool Game::move(QPoint newPos, short newDirection, QPoint bloc, bool nbPlayer)
{
    MapBloc* mb = map->getMapBloc(bloc);
    if(mb->getTraversable())
    {
        Player* p = getPlayer(nbPlayer);
        p->setDirection(newDirection);
        p->setPosition(newPos);
        switch (mb->getType())
        {
        case MapBloc::UPGRADE_NUMBER :
            p->receiveBomb(1);
            mb->setType(MapBloc::BACKGROUND);
            break;
        case MapBloc::BONUS:
            p->giveBonus();
            mb->setType(MapBloc::BACKGROUND);
            break;
        case MapBloc::UPGRADE_POWER:
            p->setPuissance(p->getPuissance()+1);
            mb->setType(MapBloc::BACKGROUND);
            break;
        default:
            break;
        }
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief Game::getPlayer
 * @param nbPlayer (0 or false is player1 and 1 or true is player2)
 * @return pointer on the selected player
 */
Player* Game::getPlayer(bool nbPlayer) const
{
    if(!nbPlayer)
        return this->player1;
    else
        return this->player2;
}
/**
 * @brief Game::getMap
 * @return
 */
Map* Game::getMap() const
{
    return this->map;
}
