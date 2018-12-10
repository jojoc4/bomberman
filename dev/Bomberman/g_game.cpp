#include "g_game.h"
#include "bomb.h"
#include "map.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

G_Game::G_Game(Game *theGame, QWidget *parent) : QWidget(parent), counterAnimP1(0), counterAnimP2(0), p1Moving(false),
                                                    p1MovingDir(-1), nbTouchesP1(0), p2Moving(false), p2MovingDir(-1),
                                                    nbTouchesP2(0)
{
    this->game = theGame;
    Player *p1 = game->getPlayer(false);
    this->textPlayer1 = new QLabel(QString("Joueur 1:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2").arg(p1->getNbBomb()).arg(p1->getPuissance()));
    p1->setPosition(QPoint(this->game->getMap()->getPlayerSpawn(false).x()*30, this->game->getMap()->getPlayerSpawn(false).y()*30));

    Player *p2 = game->getPlayer(true);
    this->textPlayer2 = new QLabel(QString("Joueur 2:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2").arg(p2->getNbBomb()).arg(p2->getPuissance()));
    p2->setPosition(QPoint(this->game->getMap()->getPlayerSpawn(true).x()*30, this->game->getMap()->getPlayerSpawn(true).y()*30));

    this->vLayout = new QVBoxLayout();
    this->vLayout->setSpacing(0);
    this->vLayout->setContentsMargins(0,0,0,0);
    this->vLayout->addWidget(textPlayer1, 0, Qt::AlignTop);
    this->vLayout->addWidget(textPlayer2, 0, Qt::AlignBottom);

    this->container = new QGraphicsView();
    this->container->setFixedSize(900, 900); //A map is 30x30 blocks, so 900 pixels
    this->container->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->container->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scene = new QGraphicsScene(container);
    this->scene->setSceneRect(container->rect());
    this->container->setScene(scene);

    this->hLayout = new QHBoxLayout();
    this->hLayout->addWidget(container);
    this->hLayout->addLayout(vLayout);

    this->setLayout(hLayout);

    //Load the textures at the beginning, better than loading them on display
    this->allBlocks = QPixmap(QString(":/resources/img/Blocs.png"));
    this->p1Texture = QPixmap(QString(":/resources/img/Bomberman.png"));
    this->p2Texture = QPixmap(QString(":/resources/img/Bombermanj2.png"));

    //set the background color once at the beginning
    this->scene->setBackgroundBrush(Qt::gray);

    /*
    //create blocks for the map and display them
    this->createDisplayMap();
    //create players, give them their textures and display them
    this->createDisplayPlayers();
    //start the display timer
    this->timeKeeper = this->startTimer(20, Qt::PreciseTimer);
    */
}

G_Game::~G_Game()
{
    //don't forget to kill the timer
    killTimer(timeKeeper);
    delete textPlayer1;
    delete textPlayer2;
    delete scene;
    delete container;
    delete vLayout;
    delete hLayout;
}

void G_Game::startGame()
{
    //create blocks for the map and display them
    this->createDisplayMap();
    //create players, give them their textures and display them
    this->createDisplayPlayers();
    //start the display timer
    this->timeKeeper = this->startTimer(20, Qt::PreciseTimer);
}

void G_Game::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    //Player 1
    case Qt::Key_W :
        p1MovingDir = Player::UP;
        ++nbTouchesP1;
        p1Moving = true;
        break;
    case Qt::Key_A :
        p1MovingDir = Player::LEFT;
        ++nbTouchesP1;
        p1Moving = true;
        break;
    case Qt::Key_S :
        p1MovingDir = Player::DOWN;
        ++nbTouchesP1;
        p1Moving = true;
        break;
    case Qt::Key_D :
        p1MovingDir = Player::RIGHT;
        ++nbTouchesP1;
        p1Moving = true;
        break;
    case Qt::Key_Space :
        break;
    //Player 2
    case Qt::Key_5 :
        p2MovingDir = Player::UP;
        ++nbTouchesP2;
        p2Moving = true;
        break;
    case Qt::Key_1 :
        p2MovingDir = Player::LEFT;
        ++nbTouchesP2;
        p2Moving = true;
        break;
    case Qt::Key_2 :
        p2MovingDir = Player::DOWN;
        ++nbTouchesP2;
        p2Moving = true;
        break;
    case Qt::Key_3 :
        p2MovingDir = Player::RIGHT;
        ++nbTouchesP2;
        p2Moving = true;
        break;
    case Qt::Key_Return :
        break;
    }
}

void G_Game::keyReleaseEvent(QKeyEvent *event)
{
    //player 1
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_A || event->key() == Qt::Key_S || event->key() == Qt::Key_D)
    {
        if(--nbTouchesP1 == 0) //Only stop moving if the key was the last one pressed
        {
            p1MovingDir = -1;
            p1Moving = false;
        }
    }
    //player 2
    else if(event->key() == Qt::Key_5 || event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3)
    {
        if(--nbTouchesP2 == 0)
        {
            p2MovingDir = -1;
            p2Moving = false;
        }
    }
}

/**
 * @brief G_Game::timerEvent
 * basically, the "thread" which is responsible of displaying the game. Simply calls the display function
 */
void G_Game::timerEvent(QTimerEvent*)
{
    this->updateDisplayPlayers();
}

/**
 * @brief G_Game::createDisplayMap
 * This function creates the QGraphicsItem items for each block of the map, adds them to the scene and saves a pointer to them in the MapBloc objects.
 */
void G_Game::createDisplayMap()
{
    int sizeX = this->scene->width()/30;
    int sizeY = this->scene->height()/30;

    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;

    //takes all the blocks in the map, one after te other (i/30 for lines and i%30 for columns)
    for(int i=0; i<900; ++i)
    {
        bloc = theMap->getMapBloc(QPoint(i/30, i%30));

        int type = bloc->getType();

        switch(type){
            case 1: //indestructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(30, 0, 30, 30))); //only take the texture of the block (QPixmap.copy() returns a crop of the original Pixmap)
                //Add and move the new block to the scene
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i/30)*sizeX, (i%30)*sizeY); //WRONG COORDINATES, BUT IT IS NORMAL! (INVERTED BECAUSE OF LOGIC WHEN LOADING MAP)

                //Keep track of the pointer to the block
                bloc->setPtrItemOnScene(item);
                break;
            }
            case 2: //destructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i/30)*sizeX, (i%30)*sizeY); //WRONG COORDINATES, BUT IT IS NORMAL! (INVERTED BECAUSE OF LOGIC WHEN LOADING MAP)

                bloc->setPtrItemOnScene(item);
                break;
            }
            case 3: //background
            {
                // Actually, does nothing, because the background is set by scene->setBackgroundBrush() earlier.
                break;
            }
            case 4: //upgrade nbre
            {
                break;
            }
            case 5: //bonus
            {
                break;
            }
            case 6: //upgrade power
            {
                break;
            }
            default :
            {

            }
        }
    }
}

/**
 * @brief G_Game::createDisplayPlayers
 * This function creates the QGraphicsItem items for the players and adds them to the scene and keeps a pointer to their items in the Player objects.
 */
void G_Game::createDisplayPlayers()
{
    QPoint p1Pos = game->getPlayer(false)->getPosition();
    QPoint p2Pos = game->getPlayer(true)->getPosition();

    int line = this->game->getPlayer(false)->getDirection();                //Useful for knowing which line of the player's texture file to use. One line per movement direction
    QPixmap texture(p1Texture.copy(counterAnimP1/4*16, line*25, 16, 25));   //Take only the right texture
    QGraphicsPixmapItem *item = this->scene->addPixmap(texture);            //Add the texture to the scene and move it to its right place
    item->setPos(p1Pos.x()-8, p1Pos.y()-18);                                //x-8 to center horizontally and y-18 because it makes more sense to take the feet into account, rather than head
    this->game->getPlayer(false)->setPtrItemOnScene(item);                  //keep track of the item in the player

    //Do the exact same for player 2
    line = this->game->getPlayer(true)->getDirection();
    texture = p2Texture.copy(counterAnimP2/4*16, line*25, 16, 25);
    item = this->scene->addPixmap(texture);
    item->setPos(p2Pos.x()-8, p2Pos.y()-20);
    this->game->getPlayer(true)->setPtrItemOnScene(item);
}

/**
 * @brief G_Game::updateDisplayPlayers
 * This function updates the position of the players on the scene.
 */
void G_Game::updateDisplayPlayers()
{
    QPoint p1Pos = game->getPlayer(false)->getPosition();
    QPoint p2Pos = game->getPlayer(true)->getPosition();
    //Pointers to the QGraphicsItem of each player
    QGraphicsItem *p1 = this->game->getPlayer(false)->getPtrItemOnScene();
    QGraphicsItem *p2 = this->game->getPlayer(true)->getPtrItemOnScene();

    //Increment the animation counter of each player
    if(p1Moving)
        incCounterAnim(1);

    if(p2Moving)
        incCounterAnim(2);

    //scene->addRect((p1Pos.y()/30)*30,(p1Pos.x()/30)*30,30, 30, QPen(Qt::red));

    //Move the players
    switch(p1MovingDir)
    {
    //Player 1
    case Player::UP :
        this->game->move(QPoint(p1Pos.x(), p1Pos.y()-2), Player::UP, QPoint(p1Pos.x()/30, (p1Pos.y()-2)/30), false);
        break;
    case Player::LEFT :
        this->game->move(QPoint(p1Pos.x()-2, p1Pos.y()), Player::LEFT, QPoint((p1Pos.x()-2)/30, p1Pos.y()/30), false);
        break;
    case Player::DOWN :
        this->game->move(QPoint(p1Pos.x(), p1Pos.y()+2), Player::DOWN, QPoint(p1Pos.x()/30, (p1Pos.y()+2)/30), false);
        break;
    case Player::RIGHT :
        this->game->move(QPoint(p1Pos.x()+2, p1Pos.y()), Player::RIGHT, QPoint((p1Pos.x()+2)/30, p1Pos.y()/30), false);
    }
    switch(p2MovingDir)
    {
    //Player 2
    case Player::UP :
        this->game->move(QPoint(p2Pos.x(), p2Pos.y()-2), Player::UP, QPoint(p2Pos.x()/30, (p2Pos.y()-2)/30), true);
        break;
    case Player::LEFT :
        this->game->move(QPoint(p2Pos.x()-2, p2Pos.y()), Player::LEFT, QPoint((p2Pos.x()-2)/30, p2Pos.y()/30), true);
        break;
    case Player::DOWN :
        this->game->move(QPoint(p2Pos.x(), p2Pos.y()+2), Player::DOWN, QPoint(p2Pos.x()/30, (p2Pos.y()+2)/30), true);
        break;
    case Player::RIGHT :
        this->game->move(QPoint(p2Pos.x()+2, p2Pos.y()), Player::RIGHT, QPoint((p2Pos.x()+2)/30, p2Pos.y()/30), true);
    }

    //Delete the items from the scene, because cannot change the QPixmap they use.
    this->scene->removeItem(p1);
    this->scene->removeItem(p2);
    //Set the new position of the players
    p1->setPos(this->game->getPlayer(false)->getPosition());
    p2->setPos(this->game->getPlayer(true)->getPosition());
    //Create the new textures for the players, now that their positions are correct after their movement.
    createDisplayPlayers();

}

/**
 * @brief G_Game::incCounterAnim
 * useful to increase a counter telling which part of the player's texture to use
 * @param which : tells which player to increase
 */
void G_Game::incCounterAnim(short which)
{
    if(which == 1)
    {
        ++counterAnimP1;
        if(counterAnimP1 == 12)
            counterAnimP1 -= 12;
    }
    if(which == 2)
    {
        ++counterAnimP2;
        if(counterAnimP2 == 12)
            counterAnimP2 -= 12;
    }
}


