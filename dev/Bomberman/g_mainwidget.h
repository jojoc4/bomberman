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

    enum Widget : short
    {
        G_MAPCHOOSER = 1,
        G_GAME=2
    };

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

    QStackedLayout* layout;
    G_HomeScreen* homescreen;
    G_MapChooser* mapchooser;
    G_Game* game;

    Game* gamePtr;

    QString mapDirPath;
};

#endif
