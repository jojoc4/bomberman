#include "game.h"

Game::Game()
{
    player1 = new Player();
    player2 = new Player();
    map = new Map();
}

void Game::keyoardThread()
{

}

void Game::start()
{

}

/**
 * @brief Game::getPlayer
 * @param nbPlayer (0 or false is player1 and 1 or true is player2)
 * @return constant reference on the selected player
 */
Player* Game::getPlayer(bool nbPlayer) const
{
    if(!nbPlayer)
        return this->player1;
    else
        return this->player2;
}
