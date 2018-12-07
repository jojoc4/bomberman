/**
 * @author Teo Schaffner
 */


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
    this->container->setFixedSize(900, 900);
    this->container->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->container->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->scene = new QGraphicsScene(container);
    this->scene->setSceneRect(container->rect());
    this->container->setScene(scene);

    this->hLayout = new QHBoxLayout();
    this->hLayout->addWidget(container);
    this->hLayout->addLayout(vLayout);

    this->setLayout(hLayout);

    this->allBlocks = QPixmap(QString(":/resources/img/Blocs.png"));
    this->p1Texture = QPixmap(QString(":/resources/img/Bomberman.png"));
    this->p2Texture = QPixmap(QString(":/resources/img/Bombermanj2.png"));


    this->scene->setBackgroundBrush(Qt::gray);
    this->createDisplayMap();
    this->createDisplayPlayers();
    this->timeKeeper = this->startTimer(20, Qt::PreciseTimer);
}

G_Game::~G_Game()
{
    killTimer(timeKeeper);
    delete textPlayer1;
    delete textPlayer2;
    delete scene;
    delete container;
    delete vLayout;
    delete hLayout;
}

void G_Game::keyPressEvent(QKeyEvent* event)
{
    switch(event->key())
    {
    //Player 1
    case Qt::Key_W :
        p1MovingDir = Player::UP;
        ++nbTouchesP1;
        break;
    case Qt::Key_A :
        p1MovingDir = Player::LEFT;
        ++nbTouchesP1;
        break;
    case Qt::Key_S :
        p1MovingDir = Player::DOWN;
        ++nbTouchesP1;
        break;
    case Qt::Key_D :
        p1MovingDir = Player::RIGHT;
        ++nbTouchesP1;
        break;
    case Qt::Key_Space :
        break;
    //Player 2
    case Qt::Key_5 :
        p2MovingDir = Player::UP;
        ++nbTouchesP2;
        break;
    case Qt::Key_1 :
        p2MovingDir = Player::LEFT;
        ++nbTouchesP2;
        break;
    case Qt::Key_2 :
        p2MovingDir = Player::DOWN;
        ++nbTouchesP2;
        break;
    case Qt::Key_3 :
        p2MovingDir = Player::RIGHT;
        ++nbTouchesP2;
        break;
    case Qt::Key_Return :
        break;
    }

    //this->updateDisplayPlayers();
}

void G_Game::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W || event->key() == Qt::Key_A || event->key() == Qt::Key_S || event->key() == Qt::Key_D)
    {
        if(--nbTouchesP1 == 0)
            p1MovingDir = -1;
    }
    else if(event->key() == Qt::Key_5 || event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3)
    {
        if(--nbTouchesP2 == 0)
        p2MovingDir = -1;
    }
}

void G_Game::timerEvent(QTimerEvent *event)
{
    this->updateDisplayPlayers();
}

void G_Game::resizeEvent(QResizeEvent* event)
{
    //this->createDisplayMap();
}

void G_Game::createDisplayMap()
{
    int sizeX = this->scene->width()/30;
    int sizeY = this->scene->height()/30;

    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;

    for(int i=0; i<900; ++i)
    {
        bloc = theMap->getMapBloc(QPoint(i/30, i%30));

        int type = bloc->getType();

        switch(type){
            case 1: //indestructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(30, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i%30)*sizeX, (i/30)*sizeY);

                bloc->setPtrItemOnScene(item);
                break;
            }
            case 2: //destructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i%30)*sizeX, (i/30)*sizeY);

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

void G_Game::createDisplayPlayers()
{
    QPoint p1Pos = game->getPlayer(false)->getPosition();
    QPoint p2Pos = game->getPlayer(true)->getPosition();

    int line = this->game->getPlayer(false)->getDirection();
    QPixmap texture(p1Texture.copy(counterAnimP1*16, line*25, 16, 25));
    QGraphicsPixmapItem *item = this->scene->addPixmap(texture);
    item->setPos(p1Pos.x(), p1Pos.y());
    this->game->getPlayer(false)->setPtrItemOnScene(item);

    line = this->game->getPlayer(true)->getDirection();
    texture = p2Texture.copy(counterAnimP2*16, line*25, 16, 25);
    item = this->scene->addPixmap(texture);
    item->setPos(p2Pos.x(), p2Pos.y());
    this->game->getPlayer(true)->setPtrItemOnScene(item);
}

void G_Game::updateDisplayPlayers()
{
    QPoint p1Pos = game->getPlayer(false)->getPosition();
    QPoint p2Pos = game->getPlayer(true)->getPosition();
    QGraphicsItem *p1 = this->game->getPlayer(false)->getPtrItemOnScene();
    QGraphicsItem *p2 = this->game->getPlayer(true)->getPtrItemOnScene();
    bool whichPlayer = false;

    if(p1Moving)
        incCounterAnim(1);

    if(p2Moving)
        incCounterAnim(2);

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

    this->scene->removeItem(p1);
    this->scene->removeItem(p2);
    p1->setPos(this->game->getPlayer(false)->getPosition());
    p2->setPos(this->game->getPlayer(true)->getPosition());
    createDisplayPlayers();

}

void G_Game::incCounterAnim(short which)
{
    if(which == 1)
    {
        counterAnimP1++;
        if(counterAnimP1 == 6)
            counterAnimP1 -= 3;
    }
    if(which == 2)
    {
        counterAnimP2++;
        if(counterAnimP2 == 6)
            counterAnimP2 -= 3;
    }
}































