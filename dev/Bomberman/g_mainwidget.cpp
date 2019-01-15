#include "g_mainwidget.h"
#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"

G_MainWidget::G_MainWidget(QWidget *parent) : QWidget(parent)
{
    gamePtr = new Game();

    mapchooser = nullptr;
    game = nullptr;
    homescreen = new G_HomeScreen(this);

    layout = new QStackedLayout(this);
    layout->addWidget(homescreen);
    layout->setCurrentWidget(homescreen);
    this->setLayout(layout);

}

void G_MainWidget::changeWidget(int index){
    switch(index){
        case 1 :
            createMapScreen();
            //break;
        case 2 :
            createGame();
           // break;
    }
    if(index == 2)
        emit(startGame());
    layout->setCurrentIndex(index);
}

void G_MainWidget::finishGame()
{
    delete gamePtr;
    gamePtr = new Game();

    deleteG_Game();

    changeWidget(1);
}

void G_MainWidget::createMapScreen()
{
    if(mapchooser != nullptr)
        deleteG_MapChooser();
    mapchooser = new G_MapChooser(gamePtr,this);
    connect(mapchooser,SIGNAL(openNextWidget(int)),this,SLOT(changeWidget(int)));

    layout->addWidget(mapchooser);
}

void G_MainWidget::createGame()
{
    if(game != nullptr)
        deleteG_Game();

    game = new G_Game(gamePtr,this);
    layout->addWidget(game);
    connect(this, &G_MainWidget::startGame, game, &G_Game::startGame);
    connect(game, &G_Game::gameOver, this, &G_MainWidget::finishGame);
}

void G_MainWidget::deleteG_Game(){
    delete game;
    layout->removeWidget(game);
    game = nullptr;
}

void G_MainWidget::deleteG_MapChooser(){
    layout->removeWidget(mapchooser);
    delete mapchooser;
    mapchooser = nullptr;
}
