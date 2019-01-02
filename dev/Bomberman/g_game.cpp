#include "g_game.h"
#include "bomb.h"
#include "map.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QLabel>
#include <QGraphicsView>
#include <QLayout>
#include <QAction>
#include "player.h"
#include "game.h"

G_Game::G_Game(Game *theGame, QWidget *parent) : QWidget(parent), iAmAwesome(false), timeKeeper(-1), counterAnimP1(0), counterAnimP2(0), p1Moving(false),
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
    this->container->setFixedSize(901, 901); //A map is 30x30 blocks, so 900 pixels
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

G_Game::~G_Game()
{
    //don't forget to kill the timer
    if(timeKeeper!=-1)
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
    {
        Player* player = game->getPlayer(false);
        QPoint pos = player->getPosition();
        dropBomb(QPoint(pos.x()/30, pos.y()/30), player);
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
        Player* player = game->getPlayer(true);
        QPoint pos = player->getPosition();
        dropBomb(QPoint(pos.x()/30, pos.y()/30), player);
        break;
    }
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
    else if(event->key() == Qt::Key_Up || event->key() == Qt::Key_Left || event->key() == Qt::Key_Down || event->key() == Qt::Key_Right)
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
    this->refreshDisplay();
}

void G_Game::refreshDisplay()
{
    this->updateDisplayPlayers();

    if(bombs.size() > 0)
        this->updateDisplayBombs();



    Player *p1 = game->getPlayer(false);
    Player *p2 = game->getPlayer(true);
    this->textPlayer1->setText(QString("Joueur 1:\nNombre de bombes: %1\n"
                                       "Puissance des bombes: %2").arg(p1->getNbBomb()).arg(p1->getPuissance()));

    this->textPlayer2->setText(QString("Joueur 2:\nNombre de bombes: %1\n"
                                       "Puissance des bombes: %2").arg(p2->getNbBomb()).arg(p2->getPuissance()));
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
            item->setPos((i/30)*sizeX, (i%30)*sizeY);

            //Keep track of the pointer to the block
            bloc->setPtrItemOnScene(item);
            break;
        }
        case 2: //destructible
        {
            QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
            QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
            item->setPos((i/30)*sizeX, (i%30)*sizeY);

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

    theMap = nullptr;
    bloc = nullptr;
}

void G_Game::updateDisplayMap()
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
            item->setPos((i/30)*sizeX, (i%30)*sizeY);

            //Keep track of the pointer to the block
            bloc->setPtrItemOnScene(item);
            break;
        }
        case 2: //destructible
        {
            QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
            QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
            item->setPos((i/30)*sizeX, (i%30)*sizeY);

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
    QGraphicsPixmapItem *p1 = static_cast<QGraphicsPixmapItem*>(this->game->getPlayer(false)->getPtrItemOnScene());
    QGraphicsPixmapItem *p2 = static_cast<QGraphicsPixmapItem*>(this->game->getPlayer(true)->getPtrItemOnScene());

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
        //Ask the game object to move the player, heading to the right direction. The move() method checks hitboxes before moving.
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

    p1->setPos(this->game->getPlayer(false)->getPosition());
    p2->setPos(this->game->getPlayer(true)->getPosition());

    int line = this->game->getPlayer(false)->getDirection();
    QPixmap texture(p1Texture.copy(counterAnimP1/4*16, line*25, 16, 25));
    p1->setPixmap(texture);

    line = this->game->getPlayer(true)->getDirection();
    texture = p2Texture.copy(counterAnimP2/4*16, line*25, 16, 25);
    p2->setPixmap(texture);
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
        if(++counterAnimP1 == 12)
            counterAnimP1 -= 12;
    }
    if(which == 2)
    {
        if(++counterAnimP2 == 12)
            counterAnimP2 -= 12;
    }
}

/**
 * @brief G_Game::dropBomb
 * creates a new bomb and displays it
 * @param blockPos
 * @param p
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

        Bomb* theBomb = new Bomb(0, p->getPuissance(), blockPos);
        bombs.push_back(theBomb);

        QPixmap texture(bombTexture.copy(32, 0, 16, 16));
        QGraphicsPixmapItem *item = this->scene->addPixmap(texture);
        item->setPos(blockPos.x()*30 + 8, blockPos.y()*30 + 8);
        theBomb->setPtrItemOnScene(item);
        p->dropBomb();
    }
}

void G_Game::updateDisplayBombs()
{
    for(Bomb* bomb:bombs){
        if(bomb->getExploded()){
            if(bomb->getStatus() == 10){
                scene->removeItem(bomb->getPtrItemOnScene());
                explodeBomb(bomb);
                bomb->resetStatus();
                bomb->postStepExplosion();
            } else {
                bomb->updateStatus();
            }
        } else {
            updateBombAnimation(bomb);
        }
    }
}
void G_Game::updateBombAnimation(Bomb* bomb){
    bomb->updateStatus();
    if(bomb->getStatus() == 10)
    {
        QRect square(0, 0, 16, 16);
        shredTexture(bomb,square);
    }
    if(bomb->getStatus() == 20)
    {
        QRect square(16, 0, 16, 16);
        shredTexture(bomb,square);
    }
    if(bomb->getStatus() == 30)
    {
        QRect square(32, 0, 16, 16);
        shredTexture(bomb,square);
    }

    if(bomb->getStatus() == 40)
    {
        QRect square(16, 0, 16, 16);
        shredTexture(bomb,square);
        bomb->resetStatus();
    }
    if(bomb->getNbCycle() == 2){
        bomb->setExploded();
    }
}
void G_Game::shredTexture(Bomb* bomb, QRect square){
    QPixmap texture(bombTexture.copy(square));
    QGraphicsPixmapItem *item = bomb->getPtrItemOnScene();
    item->setPixmap(texture);
    bomb->setPtrItemOnScene(item);
}

void G_Game::explodeBomb(Bomb *bomb){
    QPoint position = bomb->getPosition();
    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;
    bloc = theMap->getMapBloc(position);

    // delete previous explosion flame

    for(QGraphicsItem* itemToDelete:bomb->getItemsExplosion()){
        scene->removeItem(itemToDelete);
    }


    int i = bomb->getStepExplosion();

    if(i == 4){
        bombs.removeOne(bomb);
        delete bomb;
        return;
    }
    QGraphicsPixmapItem* newItem = nullptr;
    // milieu

    QRect square(i*12, 0, 12, 12);
    QPixmap texture(explosionTexture.copy(square));

    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos(position.x()*30,position.y()*30);
    scene->addItem(newItem);

    // droite

    square.setRect(i*12, 48, 12, 12);
    texture = explosionTexture.copy(square);
    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos((position.x()+1)*30,(position.y())*30);
    scene->addItem(newItem);

    // gauche

    square.setRect(i*12, 72, 12, 12);
    texture = explosionTexture.copy(square);
    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos((position.x()-1)*30,(position.y())*30);
    scene->addItem(newItem);

    // bas

    square.setRect(i*12, 60, 12, 12);
    texture = explosionTexture.copy(square);
    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos((position.x())*30,(position.y()+1)*30);
    scene->addItem(newItem);

    // haut

    square.setRect(i*12, 36, 12, 12);
    texture = explosionTexture.copy(square);
    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos((position.x())*30,(position.y()-1)*30);
    scene->addItem(newItem);
}




void G_Game::beAwesome()
{
    if(!iAmAwesome)
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe2.png"));
    else
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe.png"));
}
