#include "g_game.h"

G_Game::G_Game(QWidget *parent, Game *theGame) : QMainWindow(parent)
{
    this->game = theGame;
    this->labelPlayer1 = new QLabel("Joueur 1");
    this->labelPlayer2 = new QLabel("Joueur 2");
    this->textPlayer1 = new QLabel(
                QString("Nombre de bombes: %1\n"
                        "Puissance des bombes: %2\n"
                        "Bonus: %3").arg(game->))
}
