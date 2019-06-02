#include "ai_player.h"
#include "mapbloc.h"
#include "game.h"
#include "map.h"


AI_Player::AI_Player(Game* p_game, Player* p_opponent, QPoint pos)
    : Player(), game(p_game), opponent(p_opponent), previousBloc1(nullptr),previousBloc2(nullptr)
{

    this->setPosition(pos);
    index = 0;

}

void AI_Player::init(){

    QPoint p1 = this->getPosition();
    QPoint p2 = opponent->getPosition();


    MapBloc* blocP1 = game->getMap()->getMapBloc(QPoint(p1.x()/30, p1.y()/30));
    MapBloc* blocP2 = game->getMap()->getMapBloc(QPoint(p2.x()/30, p2.y()/30));
    qDebug() << blocP2;
    if(blocP2 != previousBloc2 ){
        qDebug() << "Le chemin a change";
        this->path = game->getMap()->getShortestPath(blocP1,blocP2);
        positionToReach = QPoint(this->path->first()->getPosition());

        previousBloc1 = blocP1;
        previousBloc2 = blocP2;
    }




}

QPoint AI_Player::getNextPosition(){
    return positionToReach;
}

bool AI_Player::isOnNextPosition(){
    bool condition = false;

    if(QPoint(this->getPosition().x()/30,this->getPosition().y()/30) != getNextPosition()){
        condition = false;
    } else {
        condition = true;
        if(!this->path->isEmpty()){
            positionToReach = QPoint(this->path->takeFirst()->getPosition());
        } else {

        }
    }
    return condition;
}
