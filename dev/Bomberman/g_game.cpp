#include "g_game.h"
#include "bomb.h"
#include "map.h"
#include "player.h"
#include "game.h"
#include "mapbloc.h"
#include "ai_player.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QLabel>
#include <QGraphicsView>
#include <QLayout>
#include <QAction>

#define MAIN_TIMER_MS 20

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30

#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30

#define TEXTURE_FIRE_SIZE_X 12
#define TEXUTRE_FIRE_SIZE_Y 12

#define TEXTURE_FIRE_UP 36
#define TEXTURE_FIRE_CENTER 0
#define TEXTURE_FIRE_LEFT 72
#define TEXTURE_FIRE_BOTTOM 60
#define TEXTURE_FIRE_RIGHT 48
#define TEXTURE_FIRE_HORIZONTAL 12
#define TEXTURE_FIRE_VERTICAL 24

#define TEXTURE_BOMB_X 16
#define TEXTURE_BOMB_Y 16

#define TEXTURE_PLAYER_X 16
#define TEXTURE_PLAYER_Y 25
#define TEXTURE_PLAYER_Y_OFFSET 8

#define TEXTURE_BOMB_STEP_1_X 0
#define TEXTURE_BOMB_STEP_2_X 16
#define TEXTURE_BOMB_STEP_3_X 32
#define TEXTURE_BOMB_STEP_X_Y 0

#define TEXTURE_SUPERBOMB_STEP_1_X 48
#define TEXTURE_SUPERBOMB_STEP_2_X 64
#define TEXTURE_SUPERBOMB_STEP_3_X 80
#define TEXTURE_SUPERBOMB_STEP_X_Y 0

#define TEXTURE_SUPERBOMB_X 16
#define TEXTURE_SUPERBOMB_Y 16

#define PLAYER_TEXTURE_NUMBER 3
#define PLAYER_TEXTURE_MULTIPLE 4

#define PLAYER_INVISIBILITY_STEP 20

#define LEFT_EXPLOSION_BLOC_ARRAY 0
#define RIGHT_EXPLOSION_BLOC_ARRAY 1
#define BOTTOM_EXPLOSION_BLOC_ARRAY 2
#define TOP_EXPLOSION_BLOC_ARRAY 3

#define TIME_BLINKING_BOMB_1 10
#define TIME_BLINKING_BOMB_2 20
#define TIME_BLINKING_BOMB_3 30
#define TIME_BLINKING_BOMB_4 40
#define TIME_BEFORE_EXPLOSION 10

#define FINAL_STEP_EXPLOSION 4

#define NB_CYCLE_DISPLAY 2

/**
 * @brief G_Game::G_Game
 * @param theGame :pointer of the game
 * @param parent : pointer of the parent which launch this widget
 */
G_Game::G_Game(Game *theGame, QWidget *parent)
    : QWidget(parent), game(theGame), iAmAwesome(false), timeKeeper(-1), counterAnimP1(0), counterAnimP2(0), p1Moving(false),
      p1MovingDir(-1), nbTouchesP1(0), p2Moving(false), p2MovingDir(-1), nbTouchesP2(0), gameEnd(false)
{
    this->move(-50,-50);
    Player *p1 = game->getPlayer(1);
    this->textPlayer1 = new QLabel(QString("Joueur 1:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2\n ").arg(p1->getNbBomb()).arg(p1->getPuissance()));
    p1->setPosition(QPoint(this->game->getMap()->getPlayerSpawn(false).x()*TEXTURE_BLOCS_X, this->game->getMap()->getPlayerSpawn(false).y()*TEXTURE_BLOCS_Y));

    Player *p2 = game->getPlayer(2);
    this->textPlayer2 = new QLabel(QString("Joueur 2:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2\n ").arg(p2->getNbBomb()).arg(p2->getPuissance()));
    p2->setPosition(QPoint(this->game->getMap()->getPlayerSpawn(true).x()*TEXTURE_BLOCS_X, this->game->getMap()->getPlayerSpawn(true).y()*TEXTURE_BLOCS_Y));


    if(game->getGameMode() == Game::LOCAL_VS_AI)
    {
        aiPlayer = static_cast<AI_Player*>(game->getPlayer(2));
        aiPlayer->setGameWidget(this);
    }

    this->vLayout = new QVBoxLayout();
    this->vLayout->setSpacing(0);
    this->vLayout->setContentsMargins(0,0,0,0);
    this->vLayout->addWidget(textPlayer1, 0, Qt::AlignTop);
    this->vLayout->addWidget(textPlayer2, 0, Qt::AlignBottom);

    this->container = new QGraphicsView();
    this->container->setFixedSize(NB_BLOCS_X*TEXTURE_BLOCS_X + 1, NB_BLOCS_Y*TEXTURE_BLOCS_Y + 1); //A map is 30x30 blocks, so 900 pixels
    this->container->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->container->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scene = new QGraphicsScene(container);
    this->scene->setSceneRect(container->rect());
    this->container->setScene(scene);

    this->hLayout = new QHBoxLayout();
    this->hLayout->addWidget(container);
    this->hLayout->addLayout(vLayout);

    this->setLayout(hLayout);

    this->container->setFocusPolicy( Qt::NoFocus );

    //Load the textures at the beginning, better than loading them on display
    this->allBlocks = QPixmap(QString(":/resources/img/Blocs.png"));
    this->p1Texture = QPixmap(QString(":/resources/img/Bomberman.png"));
    this->p2Texture = QPixmap(QString(":/resources/img/Bombermanj2.png"));
    this->bombTexture = QPixmap(QString(":/resources/img/Bombe.png"));
    this->explosionTexture = QPixmap(QString(":/resources/img/Explosions.png"));

    //set the background color once at the beginning
    this->scene->setBackgroundBrush(Qt::gray);

    this->awesomeAct = new QAction(this);
    awesomeAct->setShortcut(QKeySequence("Ctrl+H"));
    addAction(awesomeAct);
    connect(awesomeAct, SIGNAL(triggered(bool)), this, SLOT(beAwesome()));
}

/**
 * @brief G_Game::~G_Game
 * Destroy all the components of the game
 */
G_Game::~G_Game()
{
    //don't forget to kill the timer
    if(timeKeeper != -1)
        killTimer(timeKeeper);
    delete textPlayer1;
    delete textPlayer2;
    delete scene;
    delete container;
    delete vLayout;
    delete hLayout;
}

/**
 * @brief G_Game::startGame
 * Start the game and invoke all the method
 */
void G_Game::startGame()
{
    //Build the graph for Dijkstra pathfinding
    this->game->getMap()->start();
    this->game->getMap()->wait();

    //initialize the AI player
    if(game->getGameMode() == Game::LOCAL_VS_AI)
    {
        aiPlayer->init();
        aiPlayer->togglePlaying();
        aiPlayer->start();
    }

    //create blocks for the map and display them
    this->createDisplayMap();
    //create players, give them their textures and display them
    this->createDisplayPlayers();
    //start the display timer
    this->timeKeeper = this->startTimer(MAIN_TIMER_MS, Qt::PreciseTimer);

    if(game->getGameMode() == Game::LOCAL_VS_AI)
    {
        aiPlayer->start();
    }

    gameEnd = false;
}

void G_Game::receiveAIEvent(QKeyEvent *event)
{
    QApplication::sendEvent(this, event);
}

/**
 * @brief G_Game::keyPressEvent
 * @param event contains the pressed key
 */
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
        {
            Player* player = game->getPlayer(1);
            QPoint pos = player->getPosition();
            dropBomb(QPoint(pos.x()/NB_BLOCS_X, pos.y()/NB_BLOCS_Y), player);
            break;
        }
        //Player 2
        case Qt::Key_Up :
            p2MovingDir = Player::UP;
            ++nbTouchesP2;
            p2Moving = true;
            break;
        case Qt::Key_Left :
            p2MovingDir = Player::LEFT;
            ++nbTouchesP2;
            p2Moving = true;
            break;
        case Qt::Key_Down :
            p2MovingDir = Player::DOWN;
            ++nbTouchesP2;
            p2Moving = true;
            break;
        case Qt::Key_Right :
            p2MovingDir = Player::RIGHT;
            ++nbTouchesP2;
            p2Moving = true;
            break;
        case Qt::Key_Return :
        {
            Player* player = game->getPlayer(2);
            QPoint pos = player->getPosition();
            dropBomb(QPoint(pos.x()/NB_BLOCS_X, pos.y()/NB_BLOCS_Y), player);
            break;
        }
    }
}

/**
 * @brief G_Game::keyReleaseEvent
 * Called when someone press the specific keys
 * @param event captured key pressed
 */
void G_Game::keyReleaseEvent(QKeyEvent *event)
{
    //player 1
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_A || event->key() == Qt::Key_S || event->key() == Qt::Key_D)
    {
        --nbTouchesP1;
        if(nbTouchesP1 < 1) //Only stop moving if the key was the last one pressed
        {
            nbTouchesP1 = 0;
            p1MovingDir = -1;
            p1Moving = false;
        }
    }
    //player 2
    else if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Left || event->key() == Qt::Key_Down || event->key() == Qt::Key_Right)
    {
        --nbTouchesP2;
        if(nbTouchesP2 < 1)
        {
            nbTouchesP2 = 0;
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
    this->timerPlayers();
    this->refreshDisplay();
}

/**
 * timerPlayers()
 * Manage the improvment
 */
void G_Game::timerPlayers(){
    textSupP1 = "";
    textSupP2 = "";
    Player* p = game->getPlayer(1);
    if(p->getAutoDrop())
    {
        textSupP1 = "Bonus: pose automatique";
        p->incrementCounter();
        //drop a bomb each second
        if(p->getCounter()>=50){
            p->setCounter(0);
            QPoint pos = p->getPosition();
            dropBomb(QPoint(pos.x()/30, pos.y()/30), p);
        }
    }else if(p->getInvincible())
    {
        textSupP1 = "Bonus: invincible";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setInvincible(false);
        }
    }else if(p->getVisible())
    {
        textSupP1 = "Bonus: Invisible";
    }else if(p->getSuperBomb())
    {
        textSupP1 = "Bonus: super bombe";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setSuperBomb(false);
        }
    }

    //same for player 2
    p = game->getPlayer(2);
    if(p->getAutoDrop())
    {
        textSupP2 = "Bonus: pose automatique";
        p->incrementCounter();
        //drop a bomb each second
        if(p->getCounter()>=50)
        {
            p->setCounter(0);
            QPoint pos = p->getPosition();
            dropBomb(QPoint(pos.x()/30, pos.y()/30), p);
        }
    }else if(p->getInvincible())
    {
        textSupP2 = "Bonus: invincible";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setInvincible(false);
        }
    }else if(p->getVisible())
    {
        textSupP2 = "Bonus: Invisible";
    }else if(p->getSuperBomb())
    {
        textSupP2 = "Bonus: super bombe";
        p->incrementCounter();
        if(p->getCounter()>=250)
        {
            p->setSuperBomb(false);
        }
    }
}

/**
 * refreshDisplay()
 * Refresh the game display
 */
void G_Game::refreshDisplay()
{
    if(bombs.size() > 0)
        this->updateDisplayBombs();

    this->updateDisplayMap();
    this->updateDisplayPlayers();

    Player *p1 = game->getPlayer(1);
    Player *p2 = game->getPlayer(2);
    this->textPlayer1->setText(QString("Joueur 1:\nNombre de bombes: %1\n"
                                       "Puissance des bombes: %2\n%3").arg(p1->getNbBomb()).arg(p1->getPuissance()).arg(textSupP1));

    this->textPlayer2->setText(QString("Joueur 2:\nNombre de bombes: %1\n"
                                       "Puissance des bombes: %2\n%3").arg(p2->getNbBomb()).arg(p2->getPuissance()).arg(textSupP2));

    if(gameEnd)
    {

        QString msg;
        if(game->getPlayer(1)->isDead())
        {
            msg = tr("Le joueur 1 a perdu !");
        } else {
            msg = tr("Le joueur 2 a perdu !");
        }

        QMessageBox msgBox;
        msgBox.setText(msg);
        msgBox.exec();
        emit gameOver();
    }
}

/**
 * @brief G_Game::createDisplayMap
 * This function creates the QGraphicsItem items for each block of the map, adds them to the scene and saves a pointer to them in the MapBloc objects.
 */
void G_Game::createDisplayMap()
{
    int sizeX = this->scene->width()/NB_BLOCS_X;
    int sizeY = this->scene->height()/NB_BLOCS_Y;

    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;

    //takes all the blocks in the map, one after te other (i/30 for lines and i%30 for columns)
    for(int i=0; i<(NB_BLOCS_X*NB_BLOCS_Y); ++i)
    {
        bloc = theMap->getMapBloc(QPoint(i/NB_BLOCS_X, i%NB_BLOCS_Y));

        int type = bloc->getType();

        switch(type){
            case MapBloc::INDESTRUCTIBLE: //indestructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(TEXTURE_BLOCS_X, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y))); //only take the texture of the block (QPixmap.copy() returns a crop of the original Pixmap)
                //Add and move the new block to the scene
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i/NB_BLOCS_X)*sizeX, (i%NB_BLOCS_Y)*sizeY);

                //Keep track of the pointer to the block
                bloc->setPtrItemOnScene(item);
                break;
            }
            case MapBloc::DESTRUCTIBLE: //destructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i/NB_BLOCS_X)*sizeX, (i%NB_BLOCS_Y)*sizeY);

                bloc->setPtrItemOnScene(item);
                break;
            }
        }
    }

    theMap = nullptr;
    bloc = nullptr;
}

/**
 * @brief G_Game::updateDisplayMap
 */
void G_Game::updateDisplayMap()
{
    int sizeX = this->scene->width()/NB_BLOCS_X;
    int sizeY = this->scene->height()/NB_BLOCS_Y;

    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;

    //takes all the blocks in the map, one after te other (i/30 for lines and i%30 for columns)
    for(int i=0; i<(NB_BLOCS_X*NB_BLOCS_Y); ++i)
    {
        bloc = theMap->getMapBloc(QPoint(i/NB_BLOCS_X, i%NB_BLOCS_Y));
        QGraphicsPixmapItem *ptrItem = bloc->getPtrItemOnScene();

        int type = bloc->getType();

        switch(type)
        {
            case MapBloc::BACKGROUND: //background
            {
                if(ptrItem != nullptr)
                {
                    scene->removeItem(ptrItem);
                    bloc->setPtrItemOnScene(nullptr);
                }
                break;
            }
            case MapBloc::UPGRADE_NUMBER: //upgrade nbre
            {
                if(ptrItem != nullptr)
                {
                    ptrItem->setPixmap(allBlocks.copy(QRect(2*TEXTURE_BLOCS_X,
                                                            0, TEXTURE_BLOCS_X,
                                                            TEXTURE_BLOCS_Y)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(2*TEXTURE_BLOCS_X,
                                                                                              0, TEXTURE_BLOCS_X,
                                                                                              TEXTURE_BLOCS_Y))));
                    item->setPos((i/NB_BLOCS_X)*sizeX, (i%NB_BLOCS_Y)*sizeY);
                    bloc->setPtrItemOnScene(item);
                }
                break;
            }
            case MapBloc::BONUS: //bonus
            {
                if(ptrItem != nullptr)
                {
                    ptrItem->setPixmap(allBlocks.copy(QRect(4*TEXTURE_BLOCS_X,
                                                            0, TEXTURE_BLOCS_X,
                                                            TEXTURE_BLOCS_Y)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(4*TEXTURE_BLOCS_X,
                                                                                              0, TEXTURE_BLOCS_X,
                                                                                              TEXTURE_BLOCS_Y))));
                    item->setPos((i/NB_BLOCS_X)*sizeX, (i%NB_BLOCS_Y)*sizeY);
                    bloc->setPtrItemOnScene(item);
                }
                break;
            }
            case MapBloc::UPGRADE_POWER: //upgrade power
            {
                if(ptrItem != nullptr){
                    ptrItem->setPixmap(allBlocks.copy(QRect(3*TEXTURE_BLOCS_X,
                                                            0, TEXTURE_BLOCS_X,
                                                            TEXTURE_BLOCS_Y)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(3*TEXTURE_BLOCS_X,
                                                                                              0, TEXTURE_BLOCS_X,
                                                                                              TEXTURE_BLOCS_Y))));
                    item->setPos((i/NB_BLOCS_X)*sizeX, (i%NB_BLOCS_Y)*sizeY);
                    bloc->setPtrItemOnScene(item);
                }
                break;
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
    Player* ptrP1 = this->game->getPlayer(1);
    Player* ptrP2 = this->game->getPlayer(2);

    QPoint p1Pos = ptrP1->getPosition();
    QPoint p2Pos = ptrP2->getPosition();

    int line = ptrP1->getDirection();                                                                //Useful for knowing which line of the player's texture file to use. One line per movement direction
    QPixmap texture(p1Texture.copy(counterAnimP1/PLAYER_TEXTURE_MULTIPLE*TEXTURE_PLAYER_X,
                                   line*TEXTURE_PLAYER_Y, TEXTURE_PLAYER_X, TEXTURE_PLAYER_Y));   //Take only the right texture
    QGraphicsPixmapItem *item = this->scene->addPixmap(texture);                                  //Add the texture to the scene and move it to its right place
    item->setPos(p1Pos.x()-TEXTURE_PLAYER_X/2,
                 p1Pos.y()-(TEXTURE_PLAYER_Y-TEXTURE_PLAYER_Y_OFFSET));                           //x-8 to center horizontally and y-18 because it makes more sense to take the feet into account, rather than head
    ptrP1->setPtrItemOnScene(item);                                                                  //keep track of the item in the player

    //Do the exact same for player 2
    line = ptrP2->getDirection();
    texture = p2Texture.copy(counterAnimP2/PLAYER_TEXTURE_MULTIPLE*TEXTURE_PLAYER_X,
                             line*TEXTURE_PLAYER_Y, TEXTURE_PLAYER_X, TEXTURE_PLAYER_Y);
    item = this->scene->addPixmap(texture);
    item->setPos(p2Pos.x()-TEXTURE_PLAYER_X/2,
                 p2Pos.y()-(TEXTURE_PLAYER_Y-TEXTURE_PLAYER_Y_OFFSET));
    ptrP2->setPtrItemOnScene(item);

    ptrP1 = nullptr;
    ptrP2 = nullptr;
}

/**
 * @brief G_Game::updateDisplayPlayers
 * This function updates the position of the players on the scene.
 */
void G_Game::updateDisplayPlayers()
{
    //Player 1
    Player* ptrP1 = game->getPlayer(1);
    QGraphicsPixmapItem *p1 = ptrP1->getPtrItemOnScene();
    QPoint p1Pos = ptrP1->getPosition();

    switch(p1MovingDir)
    {
        case Player::UP :
            this->game->move(QPoint(p1Pos.x(), p1Pos.y()-2), Player::UP, QPoint(p1Pos.x()/NB_BLOCS_X, (p1Pos.y()-2)/NB_BLOCS_Y), 1);
            break;
        case Player::LEFT :
            this->game->move(QPoint(p1Pos.x()-2, p1Pos.y()), Player::LEFT, QPoint((p1Pos.x()-2)/NB_BLOCS_X, p1Pos.y()/NB_BLOCS_Y), 1);
            break;
        case Player::DOWN :
            this->game->move(QPoint(p1Pos.x(), p1Pos.y()+2), Player::DOWN, QPoint(p1Pos.x()/NB_BLOCS_X, (p1Pos.y()+2)/NB_BLOCS_Y), 1);
            break;
        case Player::RIGHT :
            this->game->move(QPoint(p1Pos.x()+2, p1Pos.y()), Player::RIGHT, QPoint((p1Pos.x()+2)/NB_BLOCS_X, p1Pos.y()/NB_BLOCS_Y), 1);
    }

    if(!ptrP1->getVisible()) //Player is not invisible
    {
        drawPlayer(1);
    }
    else
    {
        ptrP1->incrementCptInvisibility(PLAYER_INVISIBILITY_STEP);
        if(!ptrP1->getVisibleState())
        {
            p1->setPixmap(QPixmap(p1Texture.copy(0, 0, 2, 2)));
        }
        else
        {
            drawPlayer(1);
        }
    }

    //Player 2
    Player* ptrP2 = game->getPlayer(2);
    QGraphicsPixmapItem *p2 = ptrP2->getPtrItemOnScene();
    QPoint p2Pos = ptrP2->getPosition();

    switch(p2MovingDir)
    {
        case Player::UP :
            this->game->move(QPoint(p2Pos.x(), p2Pos.y()-2), Player::UP, QPoint(p2Pos.x()/NB_BLOCS_X, (p2Pos.y()-2)/NB_BLOCS_Y), 2);
            break;
        case Player::LEFT :
            this->game->move(QPoint(p2Pos.x()-2, p2Pos.y()), Player::LEFT, QPoint((p2Pos.x()-2)/NB_BLOCS_X, p2Pos.y()/NB_BLOCS_Y), 2);
            break;
        case Player::DOWN :
            this->game->move(QPoint(p2Pos.x(), p2Pos.y()+2), Player::DOWN, QPoint(p2Pos.x()/NB_BLOCS_X, (p2Pos.y()+2)/NB_BLOCS_Y), 2);
            break;
        case Player::RIGHT :
            this->game->move(QPoint(p2Pos.x()+2, p2Pos.y()), Player::RIGHT, QPoint((p2Pos.x()+2)/NB_BLOCS_X, p2Pos.y()/NB_BLOCS_Y), 2);
    }


    if(!ptrP2->getVisible()) //Player is not invisible
    {
        drawPlayer(2);
    }
    else
    {
        ptrP2->incrementCptInvisibility(PLAYER_INVISIBILITY_STEP);
        if(!ptrP2->getVisibleState())
        {
            p2->setPixmap(QPixmap(p2Texture.copy(0, 0, 2, 2))); //A transparent part of the texture
        }
        else
        {
            drawPlayer(2);
        }
    }
}

/**
 * @brief G_Game::drawPlayer()
 * Draws the player's texture on the scene
 * @param which : player
 */
void G_Game::drawPlayer(short which)
{
    Player* ptrPlayer = game->getPlayer(which);
    //Pointers to the QGraphicsItem of each player
    QGraphicsPixmapItem *ptrItem = static_cast<QGraphicsPixmapItem*>(ptrPlayer->getPtrItemOnScene());

    //Increment the animation counter for the player
    QPixmap texture;
    int line = ptrPlayer->getDirection();
    if(which == 1)
    {
        if(p1Moving)
            incCounterAnim(1);
        texture = QPixmap(p1Texture.copy(counterAnimP1/PLAYER_TEXTURE_MULTIPLE*TEXTURE_PLAYER_X, line*TEXTURE_PLAYER_Y, TEXTURE_PLAYER_X, TEXTURE_PLAYER_Y));
    }
    else if(which == 2)
    {
        if(p2Moving)
            incCounterAnim(2);
        texture = QPixmap(p2Texture.copy(counterAnimP2/PLAYER_TEXTURE_MULTIPLE*TEXTURE_PLAYER_X, line*TEXTURE_PLAYER_Y, TEXTURE_PLAYER_X, TEXTURE_PLAYER_Y));
    }

    //Move the player
    ptrItem->setPos(ptrPlayer->getPosition().x()-TEXTURE_PLAYER_X/2, ptrPlayer->getPosition().y()-(TEXTURE_PLAYER_Y - TEXTURE_PLAYER_Y_OFFSET));
    ptrItem->setPixmap(texture);
}

/**
 * incCounterAni()m
 * useful to increase a counter telling which part of the player's texture to use
 * @param which : tells which player to increase
 */
void G_Game::incCounterAnim(short which)
{
    if(which == 1)
    {
        if(++counterAnimP1 == PLAYER_TEXTURE_MULTIPLE * PLAYER_TEXTURE_NUMBER)
            counterAnimP1 -= (PLAYER_TEXTURE_MULTIPLE * PLAYER_TEXTURE_NUMBER);
    }
    if(which == 2)
    {
        if(++counterAnimP2 == PLAYER_TEXTURE_MULTIPLE * PLAYER_TEXTURE_NUMBER)
            counterAnimP2 -= (PLAYER_TEXTURE_MULTIPLE * PLAYER_TEXTURE_NUMBER);
    }
}

/**
 * dropBomb()
 * creates a new bomb and displays it
 * @param blockPos : position (x,y) of the bloc
 * @param p : pointer of the player who drop the player
 */
void G_Game::dropBomb(const QPoint& blockPos, Player* p)
{
    if(p->getNbBomb() > 0)
    {
        for(Bomb* b:bombs)
        {
            if(b->getPosition() == blockPos)
                return;
        }
        Bomb* theBomb = new Bomb(p->getSuperBomb(), p->getPuissance(), blockPos, p);

        bombs.push_back(theBomb);

        QPixmap texture;
        if(theBomb->getType() == Bomb::BOMB){
            texture = bombTexture.copy(TEXTURE_BOMB_STEP_3_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
        } else {
            texture = bombTexture.copy(TEXTURE_SUPERBOMB_STEP_3_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
        }
        QGraphicsPixmapItem *item = this->scene->addPixmap(texture);
        item->setPos(blockPos.x()*NB_BLOCS_X + TEXTURE_BOMB_X/2, blockPos.y()*NB_BLOCS_Y + TEXTURE_BOMB_Y/2);
        theBomb->setPtrItemOnScene(item);
        p->dropBomb();
    }
}

/**
 * updateDisplayBombs()
 * Update the display of the bombs
 */
void G_Game::updateDisplayBombs()
{
    bool bombExploded = false;
    for(Bomb* bomb:bombs)
    {
        if(bomb->getExploded())
        {
            if(bomb->getValCounterBomb() == TIME_BEFORE_EXPLOSION)
            {
                bombExploded = dislayExplosionBomb(bomb);
                bomb->resetCounter();
                bomb->postStepExplosion();
                bombExploded = true;
            } else {
                bomb->incCounterBomb();
            }
        } else {
            updateBombAnimation(bomb);
        }
    }

    if(bombExploded)
        game->getMap()->start();
}

/**
 * updateBombAnimation
 * Update the display before the explosion
 * @param bomb : pointer of the current bomb
 */
void G_Game::updateBombAnimation(Bomb* bomb)
{
    bomb->incCounterBomb();

    if(bomb->getNbCycle() == NB_CYCLE_DISPLAY)
    {
        bomb->setExploded();
        destroyBlocs(bomb);
    }
    int valCounter = bomb->getValCounterBomb();
    if(bomb->getType() == Bomb::BOMB)
    {
        switch(valCounter){
            case TIME_BLINKING_BOMB_1 :
            {
                QRect square(TEXTURE_BOMB_STEP_1_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_2 :
            {

                QRect square(TEXTURE_BOMB_STEP_2_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_3 :
            {
                QRect square(TEXTURE_BOMB_STEP_3_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_4 :
            {
                QRect square(TEXTURE_BOMB_STEP_2_X, TEXTURE_BOMB_STEP_X_Y, TEXTURE_BOMB_X, TEXTURE_BOMB_Y);
                setTextureBomb(bomb,square);
                bomb->resetCounter();
                break;
            }
        }
    } else {
        switch(valCounter)
        {
            case TIME_BLINKING_BOMB_1 :
            {
                QRect square(TEXTURE_SUPERBOMB_STEP_1_X, TEXTURE_SUPERBOMB_STEP_X_Y, TEXTURE_SUPERBOMB_X, TEXTURE_SUPERBOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_2 :
            {

                QRect square(TEXTURE_SUPERBOMB_STEP_2_X, TEXTURE_SUPERBOMB_STEP_X_Y, TEXTURE_SUPERBOMB_X, TEXTURE_SUPERBOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_3 :
            {
                QRect square(TEXTURE_SUPERBOMB_STEP_3_X, TEXTURE_SUPERBOMB_STEP_X_Y, TEXTURE_SUPERBOMB_X, TEXTURE_SUPERBOMB_Y);
                setTextureBomb(bomb,square);
                break;
            }
            case TIME_BLINKING_BOMB_4 :
            {
                QRect square(TEXTURE_SUPERBOMB_STEP_2_X, TEXTURE_SUPERBOMB_STEP_X_Y, TEXTURE_SUPERBOMB_X, TEXTURE_SUPERBOMB_Y);
                setTextureBomb(bomb,square);
                bomb->resetCounter();
                break;
            }
        }
    }
}

/**
 * setTextureBomb()
 * Apply the blinking bomb before the explosion
 * @param bomb : pointer of the current bomb
 * @param square : position (x,y) of the texture in the ressources
 */
void G_Game::setTextureBomb(Bomb* bomb, QRect square)
{
    QPixmap texture(bombTexture.copy(square));
    QGraphicsPixmapItem *item = bomb->getPtrItemOnScene();
    item->setPixmap(texture);
}

/**
 * dislayExplosionBomb()
 * Call the function to display the explosion and manage the step of explosion
 * @param bomb : pointer of the explosing bomb
 */
bool G_Game::dislayExplosionBomb(Bomb *bomb)
{
    // delete previous explosion flame
    QList<QGraphicsPixmapItem*>* listElement = bomb->getItemsExplosion();
    for(QGraphicsPixmapItem* itemToDelete:*(listElement))
    {
        scene->removeItem(itemToDelete);
        delete itemToDelete;
        listElement->removeOne(itemToDelete);
    }
    int stepExplosion = bomb->getStepExplosion();

    if(stepExplosion == FINAL_STEP_EXPLOSION)
    {
        scene->removeItem(bomb->getPtrItemOnScene());
        bombs.removeOne(bomb);
        bomb->getOwner()->receiveBomb(1);
        delete bomb;

        this->updateDisplayMap();
        return true;
    }
    QRect center(stepExplosion*TEXTURE_FIRE_SIZE_X, TEXTURE_FIRE_CENTER, TEXTURE_FIRE_SIZE_X, TEXUTRE_FIRE_SIZE_Y);
    QRect right(stepExplosion*TEXTURE_FIRE_SIZE_X, TEXTURE_FIRE_RIGHT, TEXTURE_FIRE_SIZE_X, TEXUTRE_FIRE_SIZE_Y);
    QRect left(stepExplosion*TEXTURE_FIRE_SIZE_X, TEXTURE_FIRE_LEFT, TEXTURE_FIRE_SIZE_X, TEXUTRE_FIRE_SIZE_Y);
    QRect bottom(stepExplosion*TEXTURE_FIRE_SIZE_X, TEXTURE_FIRE_BOTTOM, TEXTURE_FIRE_SIZE_X, TEXUTRE_FIRE_SIZE_Y);
    QRect up(stepExplosion*TEXTURE_FIRE_SIZE_X, TEXTURE_FIRE_UP, TEXTURE_FIRE_SIZE_X, TEXUTRE_FIRE_SIZE_Y);

    QRect vertical(stepExplosion*TEXTURE_FIRE_SIZE_X,TEXTURE_FIRE_HORIZONTAL,TEXTURE_FIRE_SIZE_X,TEXUTRE_FIRE_SIZE_Y);
    QRect horizontal(stepExplosion*TEXTURE_FIRE_SIZE_X,TEXTURE_FIRE_VERTICAL,TEXTURE_FIRE_SIZE_X,TEXUTRE_FIRE_SIZE_Y);

    for(int i = bomb->getNbDestroyedBlock(LEFT_EXPLOSION_BLOC_ARRAY)+1; i < 0; ++i)
    {
        drawFlameExplosion(horizontal,bomb,i,0);
    }
    for(int i = bomb->getNbDestroyedBlock(RIGHT_EXPLOSION_BLOC_ARRAY)-1; i > 0; --i)
    {
        drawFlameExplosion(horizontal,bomb,i,0);
    }

    for(int i = -bomb->getNbDestroyedBlock(BOTTOM_EXPLOSION_BLOC_ARRAY)-1; i > 0; --i)
    {
        drawFlameExplosion(vertical,bomb,0,i);
    }
    for(int i = bomb->getNbDestroyedBlock(TOP_EXPLOSION_BLOC_ARRAY)-1; i > 0; --i)
    {
        drawFlameExplosion(vertical,bomb,0,-i);
    }

    drawFlameExplosion(center,bomb,0,0);

    if(bomb->getNbDestroyedBlock(LEFT_EXPLOSION_BLOC_ARRAY) != 0)
        drawFlameExplosion(left,bomb, bomb->getNbDestroyedBlock(LEFT_EXPLOSION_BLOC_ARRAY),0);
    if(bomb->getNbDestroyedBlock(RIGHT_EXPLOSION_BLOC_ARRAY) != 0)
        drawFlameExplosion(right,bomb,bomb->getNbDestroyedBlock(RIGHT_EXPLOSION_BLOC_ARRAY),0);
    if(bomb->getNbDestroyedBlock(BOTTOM_EXPLOSION_BLOC_ARRAY) != 0)
        drawFlameExplosion(bottom,bomb,0, -bomb->getNbDestroyedBlock(BOTTOM_EXPLOSION_BLOC_ARRAY));
    if(bomb->getNbDestroyedBlock(TOP_EXPLOSION_BLOC_ARRAY) != 0)
        drawFlameExplosion(up,bomb,0, -bomb->getNbDestroyedBlock(TOP_EXPLOSION_BLOC_ARRAY));

    return false;
}

/**
 * drawFlameExplosion()
 * Add and display the flame on the scene
 * @param location where to cut the explosionTexture
 * @param bomb : pointer of the explosing bomb
 * @param x : value x of the bloc where the flame is going to be
 * @param y : value y of the bloc where the flame is going to be
 */
void G_Game::drawFlameExplosion(QRect location,Bomb* bomb,int x,int y)
{
    QPoint position = bomb->getPosition();

    QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem();

    QPixmap texture(explosionTexture.copy(location));

    texture = texture.scaled(NB_BLOCS_X,NB_BLOCS_Y,Qt::KeepAspectRatio);

    bomb->addFireExplosion(newItem);

    newItem->setPixmap(texture);
    newItem->setPos((position.x()+x)*NB_BLOCS_X,(position.y()+y)*NB_BLOCS_Y);

    checkPlayerExplosion(game->getPlayer(1),game->getPlayer(2),position.x()+x,position.y()+y);

    scene->addItem(newItem);
}

/**
 * destroyBlocs()
 * Select and destroy the blocks who are in the scope
 * @param bomb : pointer of the explosing bomb
 */
void G_Game::destroyBlocs(Bomb* bomb)
{
    QPoint position = bomb->getPosition();
    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;
    int puissance = bomb->getRange();

    // right
    for(int x = 0; x <= puissance; ++x)
    {
        if(position.x()+x < NB_BLOCS_X && position.x()+x >= 0)
        {
            bloc = theMap->getMapBloc(QPoint(position.x()+x,position.y()));
            if(bloc->getType() == MapBloc::INDESTRUCTIBLE)
            {
                break;
            } else if (bloc->getType() != MapBloc::BACKGROUND)
            {
                bloc->explode();
                bomb->addDestroyedBlock(RIGHT_EXPLOSION_BLOC_ARRAY,x);

                if(bomb->getType() != Bomb::SUPERBOMB)
                    break;
            }
            bomb->addDestroyedBlock(RIGHT_EXPLOSION_BLOC_ARRAY,x);
        }
    }

    // left
    for(int x = -1; x >= -puissance; --x)
    {
        if(position.x()+x < NB_BLOCS_X && position.x()+x >= 0)
        {
            bloc = theMap->getMapBloc(QPoint(position.x()+x,position.y()));
            if(bloc->getType() == MapBloc::INDESTRUCTIBLE)
            {
                break;
            } else if (bloc->getType() != MapBloc::BACKGROUND)
            {
                bloc->explode();
                bomb->addDestroyedBlock(LEFT_EXPLOSION_BLOC_ARRAY,x);

                if(bomb->getType() != Bomb::SUPERBOMB)
                    break;
            }
            bomb->addDestroyedBlock(LEFT_EXPLOSION_BLOC_ARRAY,x);
        }
    }

    // bottom
    for(int y = 1; y <= puissance; ++y)
    {
        if(position.y()+y < NB_BLOCS_Y && position.y()+y >= 0)
        {
            bloc = theMap->getMapBloc(QPoint(position.x(),position.y()+y));
            if(bloc->getType() == MapBloc::INDESTRUCTIBLE)
            {
                break;
            } else if (bloc->getType() != MapBloc::BACKGROUND){
                bloc->explode();
                bomb->addDestroyedBlock(BOTTOM_EXPLOSION_BLOC_ARRAY,-y);

                if(bomb->getType() != Bomb::SUPERBOMB)
                    break;
            }
            bomb->addDestroyedBlock(BOTTOM_EXPLOSION_BLOC_ARRAY,-y);
        }
    }

    // top
    for(int y = 1; y <= puissance; ++y)
    {
        if(position.y()-y < NB_BLOCS_Y && position.y()-y >= 0)
        {
            bloc = theMap->getMapBloc(QPoint(position.x(),position.y()-y));
            if(bloc->getType() == MapBloc::INDESTRUCTIBLE)
            {
                break;
            } else if (bloc->getType() != MapBloc::BACKGROUND)
            {
                bloc->explode();
                bomb->addDestroyedBlock(TOP_EXPLOSION_BLOC_ARRAY,y);

                if(bomb->getType() != Bomb::SUPERBOMB)
                    break;
            }
            bomb->addDestroyedBlock(TOP_EXPLOSION_BLOC_ARRAY,y);
        }
    }
}

/**
 * checkPlayerExplosion
 * Detect if the player stay in the wrong place.
 * @param player1 : pointer of player1
 * @param player2 : pointer of player2
 * @param x : value x of the explosion
 * @param y : value y of the explosion
 */
void G_Game::checkPlayerExplosion(Player *player1, Player *player2 , int x, int y)
{
    if(player1->getPosition().rx()/NB_BLOCS_X == x && player1->getPosition().ry()/NB_BLOCS_Y == y && !gameEnd && !player1->getInvincible())
    {
        player1->die();
        gameEnd = true;
    }
    if(player2->getPosition().rx()/NB_BLOCS_X == x && player2->getPosition().ry()/NB_BLOCS_Y == y && !gameEnd && !player2->getInvincible())
    {
        player2->die();
        gameEnd = true;
    }
}

/**
 * beAwesome
 * adds awesomeness to your game!
 */
void G_Game::beAwesome()
{
    if(!iAmAwesome)
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe2.png"));
    else
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe.png"));

    iAmAwesome = !iAmAwesome;
}
