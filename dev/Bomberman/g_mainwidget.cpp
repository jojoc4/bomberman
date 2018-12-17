#include "g_mainwidget.h"

G_MainWidget::G_MainWidget(Game* g, QWidget *parent) : QWidget(parent)
{
    homescreen = new G_HomeScreen(this);
    gamePtr = g;

    //game->resize(100,100);
    layout = new QStackedLayout(this);
    layout->addWidget(homescreen);
    layout->setCurrentWidget(homescreen);
    this->setLayout(layout);

}
void G_MainWidget::changeWidget(int index){
    switch(index){
        case 1 :
            createMapScreen();
        case 2 :
            createGame();
    }
    layout->setCurrentIndex(index);
}

void G_MainWidget::createMapScreen()
{
     mapchooser = new G_MapChooser(gamePtr, this);
     layout->addWidget(mapchooser);
}

void G_MainWidget::createGame()
{
    game = new G_Game(gamePtr, this);
    layout->addWidget(game);
    connect(mapchooser, &G_MapChooser::startGame, game, &G_Game::startGame);
}
