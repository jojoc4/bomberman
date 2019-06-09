#include "g_mainwidget.h"
#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"

G_MainWidget::G_MainWidget(QWidget *parent)
    : QWidget(parent), mapchooser(nullptr), game(nullptr), mapDirPath(QString(""))
{
    homescreen = new G_HomeScreen(this);

    layout = new QStackedLayout(this);
    layout->addWidget(homescreen);
    layout->setCurrentWidget(homescreen);
    this->setLayout(layout);

}

void G_MainWidget::changeWidget(int index){
    switch(index)
    {
        case 1 :
            createMapScreen();
        case 2 :
            createGame();
            mapDirPath = mapchooser->getDirectoryPath();
    }
    if(index == 2)
        emit(startGame());
    layout->setCurrentIndex(index);
}

void G_MainWidget::finishGame()
{
    if(gamePtr != nullptr)
        delete gamePtr;

    gamePtr = new Game();

    deleteG_Game();

    changeWidget(1);
}

void G_MainWidget::createMapScreen()
{
    gamePtr = new Game(homescreen->getGameMode());

    if(mapchooser != nullptr)
        deleteG_MapChooser();
    mapchooser = new G_MapChooser(gamePtr, this, mapDirPath);
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
    layout->removeWidget(game);
    delete game;
    game = nullptr;
}

void G_MainWidget::deleteG_MapChooser(){
    layout->removeWidget(mapchooser);
    delete mapchooser;
    mapchooser = nullptr;
}
