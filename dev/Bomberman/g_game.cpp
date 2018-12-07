/**
 * @author Teo Schaffner
 */


#include "g_game.h"
#include "bomb.h"
#include "map.h"
#include <QDebug>
#include <QPixmap>
#include <QGraphicsPixmapItem>

G_Game::G_Game(Game *theGame, QWidget *parent) : QWidget(parent), counterAnimP1(0), counterAnimP2(0), p1Moving(false), p2Moving(false)
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

    this->displayMap();
    this->displayPlayers();
}

G_Game::~G_Game()
{
    delete game; //This one at least is really necessary

    delete textPlayer1;
    delete textPlayer2;
    delete scene;
    delete container;
    delete vLayout;
    delete hLayout;
}

void G_Game::keyPressEvent(QKeyEvent* event)
{

}

void G_Game::resizeEvent(QResizeEvent* event)
{
    this->displayMap();
}

void G_Game::displayMap()
{
    int sizeX = this->scene->width()/30;
    int sizeY = this->scene->height()/30;

    Map* theMap = this->game->getMap();
    MapBloc* bloc = nullptr;


    this->scene->setBackgroundBrush(Qt::gray);

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

                //this->scene->addRect((i%30)*sizeX, (i/30)*sizeY, sizeX, sizeY, QPen(Qt::black), QBrush(Qt::black));
                break;
            }
            case 2: //destructible
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->scene->addPixmap(blocImage);
                item->setPos((i%30)*sizeX, (i/30)*sizeY);

                bloc->setPtrItemOnScene(item);

                //this->scene->addRect((i%30)*sizeX, (i/30)*sizeY, sizeX, sizeY, QPen(Qt::black),QBrush(Qt::blue));
                break;
            }
            case 3: //background
            {
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

void G_Game::displayPlayers()
{
    QPoint p1Pos = game->getPlayer(false)->getPosition();
    QPoint p2Pos = game->getPlayer(true)->getPosition();

    if(p1Moving)
        incCounterAnim(1);

    if(p2Moving)
        incCounterAnim(2);


    int line = this->game->getPlayer(false)->getDirection();
    QPixmap texture(p1Texture.copy(counterAnimP1*16, line*25, 16, 25));
    QGraphicsPixmapItem *item = this->scene->addPixmap(texture);
    item->setPos(p1Pos.x(), p1Pos.y());

    line = this->game->getPlayer(true)->getDirection();
    texture = p2Texture.copy(counterAnimP2*16, line*25, 16, 25);
    item = this->scene->addPixmap(texture);
    item->setPos(p2Pos.x(), p2Pos.y());

    //bloc->setPtrItemOnScene(item);


}


void G_Game::incCounterAnim(short which)
{
    if(which == 1)
    {
        counterAnimP1++;
        if(counterAnimP1 == 3)
            counterAnimP1 -= 3;
    }
    if(which == 2)
    {
        counterAnimP2++;
        if(counterAnimP2 == 3)
            counterAnimP2 -= 3;
    }
}































