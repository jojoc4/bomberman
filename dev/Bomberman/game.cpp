#include "game.h"

Game::Game()
{
    player1 = new Player();
    player2 = new Player();
    map = new Map();
}

Game::~Game()
{
    delete player1;
    delete player2;
    delete map;
}

/**
 * @brief Game::keyoardThread method not implemented
 */
void Game::keyoardThread()
{

}
/**
 * @brief Game::start method not implemented
 */
void Game::start()
{

}

/**
* @brief Game::canMove make the changes in the player if it is a special bloc
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
        case 4:
            p->setNbBomb(p->getNbBomb()+1);
            break;
        case 5:
            //TODO bonus
            break;
        case 6:
            p->setPuissance(p->getPuissance()+1);
            break;
        default:
            break;
        }
        return true;
    }
    return false;
}

/**
 * @brief Game::getPlayer
 * @param nbPlayer (0 or false is player1 and 1 or true is player2)
 * @return pointer on the selected player
 */
Player* Game::getPlayer(bool nbPlayer)
{
    if(!nbPlayer)
        return this->player1;
    else
        return this->player2;
}

Map* Game::getMap()
{
    return this->map;
}
