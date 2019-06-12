#include "ai_player.h"
#include "mapbloc.h"
#include "game.h"
#include "map.h"
#include <QKeyEvent>
#include <QMutex>

#define ACTION_INTERVAL_MS 20 //Timer interval for the actions of the AI players.

//For block's position
#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30

//For display
#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30
#define TEXTURE_PLAYER_X 16
#define TEXTURE_PLAYER_Y 25
#define PLAYER_OFFSET_X 8
#define PLAYER_OFFSET_Y 12

AI_Player::AI_Player(Game* p_game, Player* p_opponent, QPoint pos)
    : Player(), game(p_game), gameWidget(nullptr), opponent(p_opponent), goingBack(false),
      reached(false), playing(false), previousBlocP1(nullptr), previousBlocP2(nullptr)
{
    this->setPosition(pos);
    this->path = new QList<MapBloc*>();
    this->pathToSafety = new QList<MapBloc*>();
    this->resumeBloc = nullptr;
    this->positionToReach = new QPoint();
    this->mutex = new QMutex(QMutex::NonRecursive);
    this->pathMutex = new QMutex(QMutex::Recursive);
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
    resumeBloc = nullptr;

    delete path;
    path = nullptr;

    delete pathToSafety;
    pathToSafety = nullptr;

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

/**
 * @brief AI_Player::setGameWidget
 * @param gameWidget
 * Sets the widget on which the game is displayed
 */
void AI_Player::setGameWidget(QWidget* gameWidget)
{
    this->gameWidget = gameWidget;
    connect(this, SIGNAL(AIEvent(QKeyEvent*)), this->gameWidget, SLOT(receiveAIEvent(QKeyEvent*)));
}

/**
 * @brief AI_Player::init
 * Initializes the player : gets a path to the opponent if he moved
 */
void AI_Player::init(bool forceNewPath)
{
    MapBloc* blocP1 = game->getMap()->getMapBloc(QPoint(this->position.x()/NB_BLOCS_X, this->position.y()/NB_BLOCS_Y));

    pathMutex->lock();
    QPoint p2 = opponent->getPosition();
    MapBloc* blocP2 = game->getMap()->getMapBloc(QPoint(p2.x()/NB_BLOCS_X, p2.y()/NB_BLOCS_Y));

    //if opponent moved
    if(forceNewPath || blocP2 != previousBlocP2 ){
        if(this->path != nullptr)
            delete this->path;

        //take new path from current position, but only if not going to safety
        if(pathToSafety->isEmpty() == true){
            this->path = game->getMap()->getShortestPath(blocP1,blocP2);
            delete positionToReach;
            positionToReach = new QPoint(this->path->takeFirst()->getPosition());
        }else{
            this->path = game->getMap()->getShortestPath(resumeBloc,blocP2);
        }

        previousBlocP1 = blocP1;
        previousBlocP2 = blocP2;

        reached = false;
    }
    pathMutex->unlock();
}

/**
 * @brief AI_Player::isOnNextPosition
 * Checks if the player has reached the next position on his path, or on his path to safety (in case of a bomb explosion)
 */
void AI_Player::isOnNextPosition()
{
    pathMutex->lock();

    QPoint* currentPos = new QPoint(this->position.x()/NB_BLOCS_X, this->position.y()/NB_BLOCS_Y);

    //only if reached position, get a new one from path or safety path
    if(*currentPos == *positionToReach){
        msleep(4* ACTION_INTERVAL_MS); //sleep for having enough time to go to center of the block

        //safety path has priority!
        if(this->pathToSafety->isEmpty() == false)
        {
            //If reached a safe place, wait there for a while.
            if(this->pathToSafety->first() == nullptr){
                this->pathToSafety->removeFirst();
                stopMoving();
                msleep(3000);

                //And get a new path to the opponent (the map may have changed, as a bomb has exploded)
                this->goingBack = false;
                init(true);
            }
            else{
                delete positionToReach;
                positionToReach = new QPoint(this->pathToSafety->takeFirst()->getPosition());
            }
        }
        //Take the next destination in the path to the opponent
        else if(this->path->isEmpty() == false)
        {
            delete positionToReach;
            positionToReach = new QPoint(this->path->takeFirst()->getPosition());
        }
        //If no path left, stop
        else{
            reached = true;
        }
    }
    pathMutex->unlock();
}

/**
 * @brief AI_Player::stopMoving
 * Stop the AI's movement
 */
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

/**
 * @brief AI_Player::nextMovement
 * move to the next position
 */
void AI_Player::nextMovement()
{
    pathMutex->lock();

    //if destination is destructible, drop a bomb
    if(game->getMap()->getMapBloc(*positionToReach)->getType() == MapBloc::DESTRUCTIBLE)
    {
        if(!this->goingBack){
            actionDropBomb();
        }
    }

    //go to the right direction, according to the next position in the path
    if(positionToReach->x() < this->position.x()/NB_BLOCS_X)
        emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Left, Qt::NoModifier));
    else if(positionToReach->x() > this->position.x()/NB_BLOCS_X)
        emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Right, Qt::NoModifier));
    else if(positionToReach->y() < this->position.y()/NB_BLOCS_Y)
        emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Up, Qt::NoModifier));
    else if(positionToReach->y() > this->position.y()/NB_BLOCS_Y)
        emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Down, Qt::NoModifier));

    pathMutex->unlock();
}

/**
 * @brief AI_Player::goToSafety
 * Gets a path to a safe position from a bomb
 */
void AI_Player::goToSafety()
{
    pathMutex->lock();

    QPoint currentBlockPos = QPoint(this->position.x()/NB_BLOCS_X, this->position.y()/NB_BLOCS_Y); //current bloc's index on map
    this->resumeBloc = game->getMap()->getMapBloc(currentBlockPos); //where to resume after using the safe place

    //Get a path to a safe place
    this->pathToSafety = game->getMap()->getPathToSafety(this->resumeBloc);

    //for telling that it is needed to wait for the bomb to explode now that the safe place is reached
    this->pathToSafety->push_back(nullptr);

    //use the new path right away
    if(this->positionToReach != nullptr)
        delete this->positionToReach;
    this->positionToReach = new QPoint(pathToSafety->takeFirst()->getPosition());

    pathMutex->unlock();
}

/**
 * @brief AI_Player::actionDropBomb
 * drop a bomb and go to safety
 */
void AI_Player::actionDropBomb()
{
    emit AIEvent(new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_Return, Qt::NoModifier));

    //if not invincible, find a safe place
    if(!invincible)
    {
        goingBack = true;
        goToSafety();
    }
}

/**
 * @brief AI_Player::run
 * The AI playing loop
 */
void AI_Player::run()
{
    while(isPlaying())
    {
        init();
        isOnNextPosition();

        stopMoving();

        //If opponent reached, drop a bomb and go to safety
        if(reached){
            actionDropBomb();
            reached = false;
        }

        nextMovement();

        msleep(ACTION_INTERVAL_MS);
    }
}

