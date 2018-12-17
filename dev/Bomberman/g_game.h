/**
* Main gaming widget
*
* @author	Téo Schaffner
* @version	0.1
*/
#ifndef G_GAME_H
#define G_GAME_H

#include <QPixmap>

class Game;
class QLabel;
class QGraphicsView;
class QGraphicsScene;
class QVBoxLayout;
class QHBoxLayout;
class QAction;
class Bomb;
class Player;

class G_Game : public QWidget
{
    Q_OBJECT
public:
    G_Game(Game *theGame, QWidget *parent = nullptr);
    virtual ~G_Game();

signals:

public slots:
    void startGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *) override;

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
    QPixmap bombTexture;
    QPixmap explosionTexture;

    QAction* easterEgg;

    int timeKeeper;
    short counterAnimP1;
    short counterAnimP2;
    bool p1Moving;
    short p1MovingDir;
    short nbTouchesP1;
    bool p2Moving;
    short p2MovingDir;
    short nbTouchesP2;
    QVector<Bomb*> bombs;

    void createDisplayMap();
    void createDisplayPlayers();
    void updateDisplayPlayers();
    void incCounterAnim(short which);
    void dropBomb(const QPoint& blockPos, Player* p);
    void refreshDisplay();
    void updateDisplayBombs();

};

#endif
