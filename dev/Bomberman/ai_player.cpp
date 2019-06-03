#include "ai_player.h"
#include "mapbloc.h"
#include "game.h"
#include "map.h"
#include "g_game.h"
#include <QKeyEvent>

#define ACTION_TIMER_MS 40 //Timer interval for the actions of the AI players.

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30
#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30

#define TEXTURE_PLAYER_X 16
#define TEXTURE_PLAYER_Y 25
#define PLAYER_OFFSET_X TEXTURE_BLOCS_X/2
#define PLAYER_OFFSET_Y TEXTURE_BLOCS_Y/2

AI_Player::AI_Player(Game* p_game, Player* p_opponent, QPoint pos)
    : Player(), game(p_game), opponent(p_opponent), previousBlocP1(nullptr), previousBlocP2(nullptr), reached(false)
{
    this->setPosition(pos);
    this->path = new QList<MapBloc*>();
}

AI_Player::~AI_Player()
{
    game = nullptr;
    opponent = nullptr;
    delete path;
    path = nullptr;
    previousBlocP1 = nullptr;
    previousBlocP2 = nullptr;
}

void AI_Player::init(){
    QPoint p1 = this->getPosition();
    QPoint p2 = opponent->getPosition();

    MapBloc* blocP1 = game->getMap()->getMapBloc(QPoint(p1.x()/NB_BLOCS_X, p1.y()/NB_BLOCS_Y));
    MapBloc* blocP2 = game->getMap()->getMapBloc(QPoint(p2.x()/NB_BLOCS_X, p2.y()/NB_BLOCS_Y));
    /*
    qDebug() << "atteindre: " << getNextPosition();
    qDebug() << "actuel: " << blocP1->getPosition();
    */
    if(blocP2 != previousBlocP2 ){
        //qDebug() << "Le chemin a change";
        this->path = game->getMap()->getShortestPath(blocP1,blocP2);
        positionToReach = QPoint(this->path->takeFirst()->getPosition());

        previousBlocP1 = blocP1;
        previousBlocP2 = blocP2;

        reached = false;
    }
}

QPoint AI_Player::getNextPosition() const {
    return positionToReach;
}

bool AI_Player::isOnNextPosition(){
    bool condition = false;

    if(QPoint(this->getPosition().x()/NB_BLOCS_X, this->getPosition().y()/NB_BLOCS_Y) == getNextPosition()){
        condition = true;
        if(!this->path->isEmpty()){
            positionToReach = QPoint(this->path->takeFirst()->getPosition());
        }
        else{
            reached = true;
        }
    }

    return condition;
}

void AI_Player::act(QWidget* widget)
{
    QKeyEvent* key_release = nullptr;
    QKeyEvent* key_press = nullptr;

    /*
    qDebug() << "Joueur " << this->position.x() /30 << this->position.y()/30 ;
    qDebug() << "case" << this->getNextPosition();
    */

    switch(direction)
    {
        case Player::UP:
            key_release = new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Up, Qt::NoModifier);
            break;
        case Player::DOWN:
            key_release = new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Down, Qt::NoModifier);
            break;
        case Player::LEFT:
            key_release = new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
            break;
        case Player::RIGHT:
            key_release = new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
            break;
    }

    if(key_release != nullptr){
        QApplication::sendEvent(widget, key_release);
    }

    init();
    isOnNextPosition();
    if(!reached)
        key_press = nextMovement();

    if(key_press != nullptr){
        QApplication::sendEvent(widget, key_press);
    }
}

QKeyEvent* AI_Player::nextMovement()
{
    QPoint destination = this->getNextPosition();
    QPoint actuel = this->getPosition();

    if(destination.x() < actuel.x()/30)
        return new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);

    else if(destination.x() > actuel.x()/30)
        return new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);

    else if(destination.y() < actuel.y()/30)
        return new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);

    else if(destination.y() > actuel.y()/30)
        return new QKeyEvent(QKeyEvent::KeyPress,Qt::Key_Down, Qt::NoModifier);

    return nullptr;
}
