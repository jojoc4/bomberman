#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>
#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(Game* g, QWidget *parent = nullptr);
    QStackedLayout* layout;

private:
    G_Game* game;
    G_HomeScreen* homescreen;
    G_MapChooser* mapchooser;
};

#endif // MAINWIDGET_H
