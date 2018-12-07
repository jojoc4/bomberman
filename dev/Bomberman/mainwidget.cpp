#include "mainwidget.h"

MainWidget::MainWidget(Game* g, QWidget *parent) : QWidget(parent)
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
}
