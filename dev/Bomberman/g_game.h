#ifndef G_GAME_H
#define G_GAME_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsView>
#include <QLayout>
#include "player.h"
#include "game.h"

class G_Game : public QWidget
{
    Q_OBJECT
public:
    G_Game(Game *theGame, QWidget *parent = nullptr);

signals:

public slots:

private:
    Game *game;
    QLabel *labelPlayer1;
    QLabel *labelPlayer2;
    QLabel *textPlayer1;
    QLabel *textPlayer2;
    QGraphicsView *container;
    QGraphicsScene *scene;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

};

#endif // G_GAME_H
