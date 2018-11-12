#include "game.h"

Game::Game()
{

}

void Game::keyoardThread()
{

}

void Game::start()
{

}

const Player& Game::getPlayer(const bool nbPlayer) const
{
    if(!nbPlayer)
        return player1;
    else
        return player2;
}
