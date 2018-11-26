#include "g_game.h"

G_Game::G_Game(Game *theGame, QWidget *parent) : QWidget(parent)
{
    this->game = theGame;
    this->labelPlayer1 = new QLabel("Joueur 1");
    this->labelPlayer2 = new QLabel("Joueur 2");
    Player *p1 = game->getPlayer(false);
    this->textPlayer1 = new QLabel(QString("Nombre de bombes: %1\n").arg(p1->getNbBomb()));

    Player *p2 = game->getPlayer(true);
    this->textPlayer2 = new QLabel(QString("Nombre de bombes: %1\n").arg(p2->getNbBomb()));

    this->vLayout = new QVBoxLayout();
    this->vLayout->addWidget(labelPlayer1);
    this->vLayout->addWidget(textPlayer1);
    this->vLayout->addWidget(labelPlayer2);
    this->vLayout->addWidget(textPlayer2);

    this->container = new QGraphicsView();
    this->scene = new QGraphicsScene(container);
    this->scene->setSceneRect(container->rect());
    this->container->setScene(scene);

    this->hLayout = new QHBoxLayout();
    this->hLayout->addWidget(container);
    this->hLayout->addLayout(vLayout);

}
