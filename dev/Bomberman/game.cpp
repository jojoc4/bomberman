#include "game.h"

/**
 * @brief Game::Game
 */
Game::Game()
{
    player1 = new Player();
    player2 = new Player();
    map = new Map();
}
/**
 * @brief Game::~Game
 */
Game::~Game()
{
    delete player1;
    delete player2;
    delete map;
}


/**
* @brief Game::canMove make the changes in the player and in the map if it is a special bloc
* @param newPos
* @param newDirection
* @param bloc
* @param nbPlayer
* @return faslse if impossible move, else true
*/
bool Game::move(QPoint newPos, short newDirection, QPoint bloc, bool nbPlayer){
    MapBloc* mb = map->getMapBloc(bloc);
    if(mb->getTraversable()){
        Player* p = getPlayer(nbPlayer);
        p->setDirection(newDirection);
        p->setPosition(newPos);
        switch (mb->getType()) {
        case MapBloc::AMELIORATION_NOMBRE :
            p->receiveBomb(1);
            mb->setType(MapBloc::FOND);
            break;
        case MapBloc::BONUS:
            p->giveBonus();
            mb->setType(MapBloc::FOND);
            break;
        case MapBloc::AMELIORATION_PUISSANCE:
            p->setPuissance(p->getPuissance()+1);
            mb->setType(MapBloc::FOND);
            break;
        default:
            break;
        }
        return true;
    } else {
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
