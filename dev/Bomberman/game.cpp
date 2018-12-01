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
