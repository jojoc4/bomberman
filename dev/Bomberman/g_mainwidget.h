/**
* widget that contains and show all the oder in a stacked layout
*
* @author	Jonatan Baumgartner
* @version	1.0
*/
#ifndef G_MAINWIDGET_H
#define G_MAINWIDGET_H

#include <QtWidgets>
#include "g_homescreen.h"
#include "g_mapchooser.h"
#include "g_game.h"
#include "game.h"

class G_MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit G_MainWidget(QWidget *parent = nullptr);
    QStackedLayout* layout;

public slots:
    void changeWidget(int index);
    void finishGame();

private:
    void createMapScreen();
    void createGame();

    G_Game* game;
    G_HomeScreen* homescreen;
    G_MapChooser* mapchooser;

    Game* gamePtr;
};

#endif
