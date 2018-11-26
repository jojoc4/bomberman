#include "g_game.h"

G_Game::G_Game(QWidget *parent, Game *theGame) : QMainWindow(parent)
{
    this->game = theGame;
    this->labelPlayer1 = new QLabel("Joueur 1");
    this->labelPlayer2 = new QLabel("Joueur 2");
    Player p1 = game->getPlayer(1);
    this->textPlayer1 = new QLabel(QString("Nombre de bombes: %1\n"
                                            "Puissance des bombes: %2\n"
                                            "Bonus: %3").arg(p1.getNbBombes()).arg(p1.getPuissance()).arg(p1.getBonus()));

    Player p2 = game->getPlayer(1);
    this->textPlayer2 = new QLabel(QString("Nombre de bombes: %1\n"
                                            "Puissance des bombes: %2\n"
                                            "Bonus: %3").arg(p2.getNbBombes()).arg(p2.getPuissance()).arg(p2.getBonus()));

    this->vLayout = new QVBoxLayout();
    this->hLayout = new QVBoxLayout();
}
