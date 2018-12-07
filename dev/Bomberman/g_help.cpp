#include "g_help.h"

G_Help::G_Help(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Aide"));

    //setMinimumSize(600,400);

    QVBoxLayout* vbox = new QVBoxLayout(this);
    this->setLayout(vbox);

    QLabel* lblHelp = new QLabel(this);


    vbox->addWidget(new QLabel(tr("Raccourcis clavier")));
    vbox->addWidget(new QLabel(tr("Joueur 1 :")));
    vbox->addWidget(new QLabel(tr("W : avancer")));
    vbox->addWidget(new QLabel(tr("S : reculer")));
    vbox->addWidget(new QLabel(tr("A : aller à gauche")));
    vbox->addWidget(new QLabel(tr("D : aller à droite")));
    vbox->addWidget(new QLabel(tr("Barre d'espace : sauter sur les bombes")));
    vbox->addWidget(new QLabel(tr("Joueur 2")));
    vbox->addWidget(new QLabel(tr("Flèche haut : avancer")));
    vbox->addWidget(new QLabel(tr("Flèche bas : reculer")));
    vbox->addWidget(new QLabel(tr("Flèche gauche : aller à gauche")));
    vbox->addWidget(new QLabel(tr("Flèche droite : aller à droite")));
    vbox->addWidget(new QLabel(tr("Enter : sauter sur les bombes")));

}
