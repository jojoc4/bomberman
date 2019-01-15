#include "g_mainwidget.h"
#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"

/**
 * @brief G_MainWidget::G_MainWidget
 * Constructor
 * @param parent : pointer of the parent who create this widget
 */

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

/**
 * @brief G_MainWidget::changeWidget
 * Slot : receive a number and create the asked widget
 * @param index : no widget
 */
void G_MainWidget::changeWidget(int index)
{
    switch(index)
    {
        case Widget::G_MAPCHOOSER :
            createMapScreen();
            //break;
        //case Widget::G_GAME :
            createGame();
            // break;

    }
    if(index ==  Widget::G_GAME)
        emit(startGame());
    layout->setCurrentIndex(index);
}


/**
 * @brief G_MainWidget::finishGame
 * Called when the game ends. Destroy the old game and restart it
 */
void G_MainWidget::finishGame()
{
    delete gamePtr;
    gamePtr = new Game();

    deleteG_Game();

    changeWidget(Widget::G_MAPCHOOSER);
}


/**
 * @brief G_MainWidget::createMapScreen
 * Create the mapScreen Widget
 */
void G_MainWidget::createMapScreen()
{
    if(mapchooser != nullptr)
        deleteG_MapChooser();
    mapchooser = new G_MapChooser(gamePtr,this);
    connect(mapchooser,SIGNAL(openNextWidget(int)),this,SLOT(changeWidget(int)));

    layout->addWidget(mapchooser);
}


/**
 * @brief G_MainWidget::createGame
 * Create the game Widget
 */
void G_MainWidget::createGame()
{
    if(game != nullptr)
        deleteG_Game();

    game = new G_Game(gamePtr,this);
    layout->addWidget(game);
    connect(this, &G_MainWidget::startGame, game, &G_Game::startGame);
    connect(game, &G_Game::gameOver, this, &G_MainWidget::finishGame);
}

/**
 * @brief G_MainWidget::deleteG_Game
 * Delete g_game widget and his left over
 */
void G_MainWidget::deleteG_Game()
{
    delete game;
    layout->removeWidget(game);
    game = nullptr;
}


/**
 * @brief G_MainWidget::deleteG_MapChooser
 * Delete g_mapChooser widget and his left over
 */
void G_MainWidget::deleteG_MapChooser()
{
    layout->removeWidget(mapchooser);
    delete mapchooser;
    mapchooser = nullptr;
}
