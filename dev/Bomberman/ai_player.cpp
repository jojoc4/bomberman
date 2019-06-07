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
#define PLAYER_OFFSET_X 8
#define PLAYER_OFFSET_Y 12

AI_Player::AI_Player(Game* p_game, Player* p_opponent, QPoint pos)
    : Player(), game(p_game), gameWidget(nullptr), opponent(p_opponent), index(0), goingBack(false),
      reached(false), playing(false), counterCenter(-1), previousBlocP1(nullptr), previousBlocP2(nullptr)
{
    this->setPosition(pos);
    this->path = new QList<MapBloc*>();
    this->goBackPath = new QList<QPoint*>();
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

    delete goBackPath;
    goBackPath = nullptr;

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

    if(blocP2 != previousBlocP2 ){
        this->path = game->getMap()->getShortestPath(blocP1,blocP2);
        goBackPath->push_front(new QPoint(blocP1->getPosition()));
        positionToReach = new QPoint(this->path->takeFirst()->getPosition());

        previousBlocP1 = blocP1;
        previousBlocP2 = blocP2;

        reached = false;
    }
    pathMutex->unlock();
}

bool AI_Player::isOnNextPosition()
{
    bool condition = false;

    pathMutex->lock();
    if(QPoint(this->position.x()/NB_BLOCS_X, this->position.y()/NB_BLOCS_Y) == *positionToReach){
        condition = true;
        //counterCenter = 0;
        msleep(4* ACTION_INTERVAL_MS);

        if(!this->path->isEmpty()){
            goBackPath->push_front(positionToReach);
            if(goBackPath->length() > std::max(this->puissance, opponent->getPuissance()))
                goBackPath->removeLast();

            if(this->path->first() == nullptr){
                this->path->removeFirst();
                stopMoving();
                msleep(3000);
            }

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
    QKeyEvent* key_press = nullptr;

    init();
    isOnNextPosition();

    /*
    if(counterCenter >= 0 && counterCenter < 4 && !reached){
        ++counterCenter;
        return;
    }else
        counterCenter = -1;
    */

    stopMoving();

    if(!reached)
        key_press = nextMovement();

    if(key_press != nullptr){
        emit AIEvent(key_press);
    }
}

void AI_Player::stopMoving()
{
    switch(direction)
    {
        case Player::UP:
            emit AIEvent(new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Up, Qt::NoModifier));
            break;
        case Player::DOWN:
            emit AIEvent(new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Down, Qt::NoModifier));
            break;
        case Player::LEFT:
            emit AIEvent(new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier));
            break;
        case Player::RIGHT:
            emit AIEvent(new QKeyEvent(QKeyEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier));
            break;
    }
}

QKeyEvent* AI_Player::nextMovement()
{
    QKeyEvent* event = nullptr;

    pathMutex->lock();

    if(game->getMap()->getMapBloc(*positionToReach)->getType() == MapBloc::DESTRUCTIBLE)
    {
        if(!goingBack){
            emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));
            goingBack = true;

            if(!invincible)
            {
                goToSafety();
            }
        }
        else
        {
            goingBack = false;
        }
    }

    if(positionToReach->x() < this->position.x()/NB_BLOCS_X)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    else if(positionToReach->x() > this->position.x()/NB_BLOCS_X)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    else if(positionToReach->y() < this->position.y()/NB_BLOCS_Y)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    else if(positionToReach->y() > this->position.y()/NB_BLOCS_Y)
        event = new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);

    pathMutex->unlock();
    return event;
}

void AI_Player::goToSafety()
{
    QPoint currentBlockPos = QPoint(this->position.x()/NB_BLOCS_X, this->position.y()/NB_BLOCS_Y);
    MapBloc* currentBlock = game->getMap()->getMapBloc(currentBlockPos);

    path->push_front(game->getMap()->getMapBloc(*positionToReach));
    path->push_front(currentBlock);

    QList<MapBloc*>* pathToSafety = game->getMap()->getPathToSafety(currentBlock);

    for(int i=0; i<pathToSafety->length(); ++i){
        path->push_front(pathToSafety->value(i));
    }

    path->push_front(nullptr);

    for(int i=pathToSafety->length()-1; i>=0; --i){
        path->push_front(pathToSafety->value(i));
    }

    positionToReach = new QPoint(path->takeFirst()->getPosition());
}

void AI_Player::run()
{
    while(isPlaying())
    {
        act();
        msleep(ACTION_INTERVAL_MS);
    }
}
