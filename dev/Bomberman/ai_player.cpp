#include "ai_player.h"
#include "mapbloc.h"
#include "game.h"
#include "map.h"

AI_Player::AI_Player(Game* p_game, Player* p_opponent)
    : Player(), game(p_game), opponent(p_opponent)
{
    this->path = new QList<MapBloc*>();
}
