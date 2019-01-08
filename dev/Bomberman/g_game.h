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
    void gameOver();

public slots:
    void startGame();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void timerEvent(QTimerEvent *) override;

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

    QString textSupP1;
    QString textSupP2;

    QAction* awesomeAct;
    bool iAmAwesome;

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


    void refreshDisplay();

    void createDisplayMap();
    void updateDisplayMap();

    void createDisplayPlayers();
    void updateDisplayPlayers();
    void drawPlayer(bool which);
    void incCounterAnim(short which);

    void dropBomb(const QPoint& blockPos, Player* p);
    void setTextureBomb(Bomb* bomb, QRect square);
    void updateDisplayBombs();
    void updateBombAnimation(Bomb* bomb);

    void dislayExplosionBomb(Bomb* bomb);
    void drawFlameExplosion(QRect location, Bomb *bomb, short x, short y);
    void destroyBlocs(Bomb* bomb);

    void timerPlayers();
    void checkPlayerExplosion(Player* player1, Player* player2 , int x, int y);

    private slots:
    void beAwesome();
};

#endif
