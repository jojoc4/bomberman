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
    virtual ~G_Game();

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Game *game;
    QLabel *textPlayer1;
    QLabel *textPlayer2;
    QGraphicsView *container;
    QGraphicsScene *scene;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

    void displayMap() const;
    void displayPlayers() const;

};

#endif // G_GAME_H
