#include "g_mainwidget.h"

G_MainWidget::G_MainWidget(Game* g, QWidget *parent) : QWidget(parent)
{
    if(g != nullptr)
        gamePtr = g;
    else
        gamePtr = new Game();

    mapchooser = nullptr;
    game = nullptr;
    homescreen = new G_HomeScreen(this);


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

void G_MainWidget::finishGame()
{
    delete game;
    gamePtr = new Game();
    changeWidget(1);
}

void G_MainWidget::createMapScreen()
{
    if(mapchooser != nullptr)
        delete mapchooser;
    mapchooser = new G_MapChooser(gamePtr, this);
    layout->addWidget(mapchooser);
}

void G_MainWidget::createGame()
{
    if(game != nullptr)
        delete game;
    game = new G_Game(gamePtr, this);
    layout->addWidget(game);
    connect(mapchooser, &G_MapChooser::startGame, game, &G_Game::startGame);
}

