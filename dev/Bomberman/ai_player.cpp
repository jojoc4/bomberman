#include "ai_player.h"
#include "mapbloc.h"
#include "game.h"
#include "map.h"
#include <QKeyEvent>
#include <QMutex>

#define ACTION_INTERVAL_MS 20 //Timer interval for the actions of the AI players.

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30
#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30

#define TEXTURE_PLAYER_X 16
#define TEXTURE_PLAYER_Y 25
#define PLAYER_OFFSET_X TEXTURE_BLOCS_X/2
#define PLAYER_OFFSET_Y TEXTURE_BLOCS_Y/2

AI_Player::AI_Player(Game* p_game, Player* p_opponent, QPoint pos)
    : Player(), game(p_game), gameWidget(nullptr), opponent(p_opponent),
      reached(false), playing(false), previousBlocP1(nullptr), previousBlocP2(nullptr)
{
    this->setPosition(pos);
    this->path = new QList<MapBloc*>();
    this->positionToReach = new QPoint();
    this->mutex = new QMutex(QMutex::NonRecursive);
    this->pathMutex = new QMutex(QMutex::NonRecursive);
}

AI_Player::~AI_Player()
{
    mutex->lock();
    if(playing)
        playing = false;

    this->terminate();
    this->wait();

    game = nullptr;
    opponent = nullptr;
    gameWidget = nullptr;

    delete path;
    path = nullptr;

    previousBlocP1 = nullptr;
    previousBlocP2 = nullptr;

    mutex->unlock();

    delete mutex;
    mutex = nullptr;
    pathMutex = nullptr;
}

void AI_Player::togglePlaying()
{
    mutex->lock();
    playing = !playing;
    mutex->unlock();
}

bool AI_Player::isPlaying() const
{
    mutex->lock();
    bool p = playing;
    mutex->unlock();
    return p;
}

void AI_Player::setGameWidget(QWidget* gameWidget)
{
    this->gameWidget = gameWidget;
    connect(this, SIGNAL(AIEvent(QKeyEvent*)), this->gameWidget, SLOT(receiveAIEvent(QKeyEvent*)));
}

void AI_Player::init()
{
    QPoint p1 = this->getPosition();
    MapBloc* blocP1 = game->getMap()->getMapBloc(QPoint(p1.x()/NB_BLOCS_X, p1.y()/NB_BLOCS_Y));

    pathMutex->lock();
    QPoint p2 = opponent->getPosition();
    MapBloc* blocP2 = game->getMap()->getMapBloc(QPoint(p2.x()/NB_BLOCS_X, p2.y()/NB_BLOCS_Y));
    /*
    qDebug() << "atteindre: " << getNextPosition();
    qDebug() << "actuel: " << blocP1->getPosition();
    */
    if(blocP2 != previousBlocP2 ){
        //qDebug() << "Le chemin a change";
        this->path = game->getMap()->getShortestPath(blocP1,blocP2);
        positionToReach = new QPoint(this->path->takeFirst()->getPosition());

        previousBlocP1 = blocP1;
        previousBlocP2 = blocP2;

        reached = false;
    }
    pathMutex->unlock();
}

QPoint* AI_Player::getNextPosition() const
{
    pathMutex->lock();
    return positionToReach;
    pathMutex->unlock();
}

bool AI_Player::isOnNextPosition()
{
    bool condition = false;

    pathMutex->lock();
    if(QPoint(this->getPosition().x()/NB_BLOCS_X, this->getPosition().y()/NB_BLOCS_Y) == *positionToReach){
        condition = true;
        if(!this->path->isEmpty()){
            positionToReach = new QPoint(this->path->takeFirst()->getPosition());
        }
        else{
            reached = true;
        }
    }
    pathMutex->unlock();

    return condition;
}

/**
 * @brief AI_Player::act : the method used by the AI to move and drop bombs
 */
void AI_Player::act()
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
        emit AIEvent(key_release);
    }

    init();
    isOnNextPosition();
    if(!reached)
        key_press = nextMovement();

    if(key_press != nullptr){
        emit AIEvent(key_press);
    }
}

QKeyEvent* AI_Player::nextMovement()
{
    QKeyEvent* event = nullptr;
    QPoint actuel = this->getPosition();

    pathMutex->lock();

    if(positionToReach->x() < actuel.x()/NB_BLOCS_X)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    else if(positionToReach->x() > actuel.x()/NB_BLOCS_X)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    else if(positionToReach->y() < actuel.y()/NB_BLOCS_Y)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    else if(positionToReach->y() > actuel.y()/NB_BLOCS_Y)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);

    pathMutex->unlock();
    return event;
}

void AI_Player::run()
{
    while(isPlaying())
    {
        act();
        msleep(ACTION_INTERVAL_MS);
    }
}
