/**
* widget that contains and show all the oder in a stacked layout
*
* @author	Jonatan Baumgartner
* @version	1.0
*/
#ifndef G_MAINWIDGET_H
#define G_MAINWIDGET_H

#include <QtWidgets>

class G_Game;
class G_HomeScreen;
class G_MapChooser;
class Game;

class G_MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit G_MainWidget(QWidget *parent = nullptr);
    QStackedLayout* layout;

signals :
    void startGame();

public slots:
    void changeWidget(int index);
    void finishGame();

private:
    void createMapScreen();
    void createGame();

    void deleteG_Game();
    void deleteG_MapChooser();

    G_Game* game;
    G_HomeScreen* homescreen;
    G_MapChooser* mapchooser;

    Game* gamePtr;
};

#endif
