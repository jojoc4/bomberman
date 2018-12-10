#include "g_mainwidget.h"

G_MainWidget::G_MainWidget(Game* g, QWidget *parent) : QWidget(parent)
{
    homescreen = new G_HomeScreen(this);
    mapchooser = new G_MapChooser(g, this);
    game = new G_Game(g, this);

    layout = new QStackedLayout(this);
    layout->addWidget(game);
    layout->addWidget(mapchooser);
    layout->addWidget(homescreen);

    layout->setCurrentWidget(homescreen);

    setLayout(layout);

    connect(mapchooser, &G_MapChooser::startGame, game, &G_Game::startGame);
}
void G_MainWidget::changeWidget(int index){
    layout->setCurrentIndex(index);
}
