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
                                               "Puissance des bombes: %2\n ").arg(p1->getNbBomb()).arg(p1->getPuissance()));
    p1->setPosition(QPoint(this->game->getMap()->getPlayerSpawn(false).x()*30, this->game->getMap()->getPlayerSpawn(false).y()*30));

    Player *p2 = game->getPlayer(true);
    this->textPlayer2 = new QLabel(QString("Joueur 2:\nNombre de bombes: %1\n"
                                               "Puissance des bombes: %2\n ").arg(p2->getNbBomb()).arg(p2->getPuissance()));
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
    this->timerPlayers();
    this->refreshDisplay();
}

void G_Game::timerPlayers(){
    textSupP1 = "";
    textSupP2 = "";
    Player* p = game->getPlayer(false);
    if(p->getAutoDrop()){
        textSupP1 = "Bonus: pose automatique";
        p->incrementCounter();
        //drop a bomb each second
        if(p->getCounter()>=50){
            p->setCounter(0);
            QPoint pos = p->getPosition();
            dropBomb(QPoint(pos.x()/30, pos.y()/30), p);
        }
    }else if(p->getInvincible()){
        textSupP1 = "Bonus: invincible";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setInvincible(false);
        }
    }else if(p->getVisible()){
        textSupP1 = "Bonus: Invisible";
    }else if(p->getSuperBomb()){
        textSupP1 = "Bonus: super bombe";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setSuperBomb(false);
        }
    }

    //same for player 2
    p = game->getPlayer(true);
    if(p->getAutoDrop()){
        textSupP2 = "Bonus: pose automatique";
        p->incrementCounter();
        //drop a bomb each second
        if(p->getCounter()>=50){
            p->setCounter(0);
            QPoint pos = p->getPosition();
            dropBomb(QPoint(pos.x()/30, pos.y()/30), p);
        }
    }else if(p->getInvincible()){
        textSupP2 = "Bonus: invincible";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setInvincible(false);
        }
    }else if(p->getVisible()){
        textSupP2 = "Bonus: Invisible";
    }else if(p->getSuperBomb()){
        textSupP2 = "Bonus: super bombe";
        p->incrementCounter();
        if(p->getCounter()>=250){
            p->setSuperBomb(false);
        }
    }
}

void G_Game::refreshDisplay()
{
    this->updateDisplayMap();
    this->updateDisplayPlayers();

    if(bombs.size() > 0)
        this->updateDisplayBombs();

    Player *p1 = game->getPlayer(false);
    Player *p2 = game->getPlayer(true);
    this->textPlayer1->setText(QString("Joueur 1:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2\n%3").arg(p1->getNbBomb()).arg(p1->getPuissance()).arg(textSupP1));

    this->textPlayer2->setText(QString("Joueur 2:\nNombre de bombes: %1\n"
                                           "Puissance des bombes: %2\n%3").arg(p2->getNbBomb()).arg(p2->getPuissance()).arg(textSupP2));
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
        QGraphicsPixmapItem *ptrItem = bloc->getPtrItemOnScene();

        int type = bloc->getType();

        switch(type)
        {
            case 3: //background
            {
                if(ptrItem != nullptr)
                {
                    scene->removeItem(ptrItem);
                    bloc->setPtrItemOnScene(nullptr);
                }
                break;
            }
            case 4: //upgrade nbre
            {
                if(ptrItem != nullptr){
                    ptrItem->setPixmap(allBlocks.copy(QRect(60, 0, 30, 30)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(60, 0, 30, 30))));
                    item->setPos((i/30)*sizeX, (i%30)*sizeY);
                    bloc->setPtrItemOnScene(item);
                }
                break;
            }
            case 5: //bonus
            {
                if(ptrItem != nullptr){
                    ptrItem->setPixmap(allBlocks.copy(QRect(120, 0, 30, 30)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(120, 0, 30, 30))));
                    item->setPos((i/30)*sizeX, (i%30)*sizeY);
                    bloc->setPtrItemOnScene(item);
                }
                break;
            }
            case 6: //upgrade power
            {
                if(ptrItem != nullptr){
                    ptrItem->setPixmap(allBlocks.copy(QRect(90, 0, 30, 30)));
                }
                else
                {
                    QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(allBlocks.copy(QRect(90, 0, 30, 30))));
                    item->setPos((i/30)*sizeX, (i%30)*sizeY);
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
    //Player 1
    Player* ptrP1 = game->getPlayer(false);
    QGraphicsPixmapItem *p1 = static_cast<QGraphicsPixmapItem*>(ptrP1->getPtrItemOnScene());
    QPoint p1Pos = ptrP1->getPosition();

    switch(p1MovingDir)
    {
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

    if(!ptrP1->getVisible()) //Player is not invisible
    {
        drawPlayer(false);
    }
    else
    {
        ptrP1->incrementCptInvisibility(20);
        if(!ptrP1->getVisibleState())
        {
            p1->setPixmap(QPixmap(p1Texture.copy(0, 0, 2, 2)));
        }
        else
        {
            drawPlayer(false);
        }
    }

    //Player 2
    Player* ptrP2 = game->getPlayer(true);
    QGraphicsPixmapItem *p2 = static_cast<QGraphicsPixmapItem*>(ptrP2->getPtrItemOnScene());
    QPoint p2Pos = ptrP2->getPosition();

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


    if(!ptrP2->getVisible()) //Player is not invisible
    {
        drawPlayer(true);
    }
    else
    {
        ptrP2->incrementCptInvisibility(20);
        if(!ptrP2->getVisibleState())
        {
            p2->setPixmap(QPixmap(p2Texture.copy(0, 0, 2, 2)));
        }
        else
        {
            drawPlayer(true);
        }
    }
}

void G_Game::drawPlayer(bool which)
{
    Player* ptrPlayer = game->getPlayer(which);
    //Pointers to the QGraphicsItem of each player
    QGraphicsPixmapItem *ptrItem = static_cast<QGraphicsPixmapItem*>(ptrPlayer->getPtrItemOnScene());

    //Increment the animation counter for the player
    QPixmap texture;
    int line = ptrPlayer->getDirection();
    if(!which)
    {
        if(p1Moving)
            incCounterAnim(1);
        texture = QPixmap(p1Texture.copy(counterAnimP1/4*16, line*25, 16, 25));
    }
    else
    {
        if(p2Moving)
            incCounterAnim(2);
        texture = QPixmap(p2Texture.copy(counterAnimP2/4*16, line*25, 16, 25));
    }

    //Move the player
    ptrItem->setPos(ptrPlayer->getPosition().x()-8, ptrPlayer->getPosition().y()-17);

    //QPixmap texture(p1Texture.copy(counterAnimP1/4*16, line*25, 16, 25));
    ptrItem->setPixmap(texture);
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
        Bomb* theBomb = new Bomb(0, p->getPuissance(), blockPos,p);
        //Bomb* theBomb = new Bomb(0, 2, blockPos);

        bombs.push_back(theBomb);

        QPixmap texture(bombTexture.copy(32, 0, 16, 16));
        QGraphicsPixmapItem *item = this->scene->addPixmap(texture);
        item->setPos(blockPos.x()*30 + 8, blockPos.y()*30 + 8);
        theBomb->setPtrItemOnScene(item);
        p->dropBomb();
    }
}
/**
 * @brief G_Game::updateDisplayBombs
 * Update the display of the bombs
 */
void G_Game::updateDisplayBombs()
{
    for(Bomb* bomb:bombs){
        if(bomb->getExploded())
        {
            if(bomb->getValCounterBomb() == 10)
            {
                dislayExplosionBomb(bomb);
                bomb->resetCounter();
                bomb->postStepExplosion();
            } else {
                bomb->incCounterBomb();
            }
        } else {
            updateBombAnimation(bomb);
        }
    }
}
/**
 * @brief G_Game::updateBombAnimation
 * Update the display before the explosion
 * @param bomb
 */
void G_Game::updateBombAnimation(Bomb* bomb){
    bomb->incCounterBomb();

    if(bomb->getNbCycle() == 2){
        bomb->setExploded();
    }
    int valCounter = bomb->getValCounterBomb();
    if(bomb->getType() == 0){

        switch(valCounter){
        case 10 :
        {
            QRect square(0, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 20 :
        {

            QRect square(16, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 30 :
        {
            QRect square(32, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 40 :
        {
            QRect square(16, 0, 16, 16);
            setTextureBomb(bomb,square);
            bomb->resetCounter();
            break;
        }
        default :{}

        }
    } else {
        switch(valCounter){
        case 10 :
        {
            QRect square(48, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 20 :
        {

            QRect square(64, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 30 :
        {
            QRect square(80, 0, 16, 16);
            setTextureBomb(bomb,square);
            break;
        }
        case 40 :
        {
            QRect square(16, 0, 16, 16);
            setTextureBomb(bomb,square);
            bomb->resetCounter();
            break;
        }
        default :{}
        }
    }
}
/**
 * @brief G_Game::setTextureBomb
 *
 * @param bomb
 * @param square
 */
void G_Game::setTextureBomb(Bomb* bomb, QRect square){
    QPixmap texture(bombTexture.copy(square));
    QGraphicsPixmapItem *item = bomb->getPtrItemOnScene();
    item->setPixmap(texture);
    bomb->setPtrItemOnScene(item);
}

void G_Game::dislayExplosionBomb(Bomb *bomb){
    // delete previous explosion flame
    QList<QGraphicsPixmapItem*>* listElement = bomb->getItemsExplosion();
    for(QGraphicsPixmapItem* itemToDelete:*(listElement)){
        scene->removeItem(itemToDelete);
        listElement->removeOne(itemToDelete);
    }
    int i = bomb->getStepExplosion();


    //this->updateDisplayPlayers();
    if(i == 4){
        destroyBlocs(bomb);
        scene->removeItem(bomb->getPtrItemOnScene());
        bombs.removeOne(bomb);
        delete bomb;
        this->updateDisplayMap();
        bomb->getOwner()->receiveBomb(1);
        return;
    }
    QRect center(i*12, 0, 12, 12);
    QRect right(i*12, 48, 12, 12);
    QRect left(i*12, 72, 12, 12);
    QRect bottom(i*12, 60, 12, 12);
    QRect up(i*12, 36, 12, 12);

    QRect vertical(i*12,12,12,12);
    QRect horizontal(i*12,24,12,12);

    int powerBomb = bomb->getRange();
    drawFlameExplosion(center,bomb,0,0);

    drawFlameExplosion(right,bomb,-powerBomb,0);
    for(int i = 1; i < powerBomb; i++){
        drawFlameExplosion(horizontal,bomb,i,0);
        drawFlameExplosion(horizontal,bomb,-i,0);
        drawFlameExplosion(vertical,bomb,0,i);
        drawFlameExplosion(vertical,bomb,0,-i);
    }
    drawFlameExplosion(left,bomb,powerBomb,0);
    drawFlameExplosion(bottom,bomb,0,-powerBomb);
    drawFlameExplosion(up,bomb,0,powerBomb);

}
void G_Game::drawFlameExplosion(QRect location,Bomb* bomb,short x,short y){
    QPoint position = bomb->getPosition();
    QGraphicsPixmapItem* newItem = nullptr;

    QPixmap texture(explosionTexture.copy(location));

    texture = texture.scaled(30,30,Qt::KeepAspectRatio);

    //newItem = scene->addPixmap(texture);
    //bomb->addFireExplosion(newItem);
    //Modifier addFireExplosion pour renvoyer void
    newItem = bomb->addFireExplosion(new QGraphicsPixmapItem());

    newItem->setPixmap(texture);
    newItem->setPos((position.x()+x)*30,(position.y()+y)*30);

    scene->addItem(newItem);
}

void G_Game::destroyBlocs(Bomb* bomb){
    QPoint position = bomb->getPosition();
    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;
    int puissance = bomb->getRange();

    Player* posPlayer1 = game->getPlayer(false);
    Player* posPlayer2 = game->getPlayer(true);

    // superbomb
    if(bomb->getType() == 1){
        for(int x = -puissance; x <= puissance; x++){
            if(position.x()+x < 30 && position.x()+x >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x()+x,position.y()));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x()+x,position.y());
                }
            }
        }
        for(int y = -puissance; y <= puissance; y++){
            if(position.y()+y < 30 && position.y()+y >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x(),position.y()+y));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x(),position.y()+y);
                }
            }
        }
    } else {
        // normal
        for(int x = 0; x <= puissance; x++){
            if(position.x()+x < 30 && position.x()+x >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x()+x,position.y()));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    x = puissance;
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x()+x,position.y());
                }
            }
        }
        for(int x = 0; x >= -puissance; x--){
            if(position.x()+x < 30 && position.x()+x >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x()+x,position.y()));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    x = puissance;
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x()+x,position.y());
                }
            }
        }
        for(int y = 0; y <= puissance; y++){
            if(position.y()+y < 30 && position.y()+y >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x(),position.y()+y));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    y = puissance;
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x(),position.y()+y);
                }
            }
        }
        for(int y = 0; y >= -puissance; y--){
            if(position.y()+y < 30 && position.y()+y >= 0){
                bloc = theMap->getMapBloc(QPoint(position.x(),position.y()+y));
                if(bloc->getType() != 1 && bloc->getType() != 3){
                    bloc->explode();
                    y = puissance;
                    checkPlayerExplosion(posPlayer1,posPlayer2,position.x(),position.y()+y);
                }
            }
        }
    }
}

void G_Game::checkPlayerExplosion(Player *player1, Player *player2 , int x, int y)
{
    qDebug() << " j1 : " << player1->getPosition().rx()/30;
    qDebug() << " j1 : " << player1->getPosition().ry()/30;

    qDebug() << " j2 : " << player2->getPosition().rx()/30;
    qDebug() << " j2 : " << player2->getPosition().ry()/30;



    if(player1->getPosition().rx()/30 == x && player1->getPosition().ry()/30 == y){
        qDebug() << "p1 : mort";
    }
    if(player2->getPosition().rx()/30 == x && player2->getPosition().ry()/30 == y){
        qDebug() << "p2 : mort";
    }
}

void G_Game::beAwesome()
{
    if(!iAmAwesome)
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe2.png"));
    else
        this->bombTexture = QPixmap(QString(":/resources/img/Bombe.png"));

    iAmAwesome = !iAmAwesome;
}
