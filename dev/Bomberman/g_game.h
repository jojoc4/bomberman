#ifndef G_GAME_H
#define G_GAME_H

#include <QLabel>
#include <QGraphicsView>
#include <QLayout>
#include <QPixmap>
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
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    Game *game;
    QLabel *textPlayer1;
    QLabel *textPlayer2;
    QGraphicsView *container;
    QGraphicsScene *scene;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;

    QPixmap allBlocks;
    QPixmap p1Texture;
    QPixmap p2Texture;

    int timeKeeper;
    int counterAnimP1;
    int counterAnimP2;
    bool p1Moving;
    short p1MovingDir;
    short nbTouchesP1;
    bool p2Moving;
    short p2MovingDir;
    short nbTouchesP2;

    void createDisplayMap();
    void createDisplayPlayers();
    void updateDisplayPlayers();
    void incCounterAnim(short which);

};

#endif // G_GAME_H
