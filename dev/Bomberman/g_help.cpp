#include "g_help.h"

#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30

#define TEXTURE_DESTR_X 0
#define TEXTURE_INDE_X 30
#define TEXTURE_ASCOPE_X 90
#define TEXTURE_ANUMBER 60
#define TEXTURE_BONUS 120


G_Help::G_Help(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Aide"));

    QVBoxLayout* vbox = new QVBoxLayout(this);
    this->setLayout(vbox);
    QPixmap allBlocks =  QPixmap(QString(":/resources/img/Blocs.png"));

    QPixmap destructible(allBlocks.copy(QRect(TEXTURE_DESTR_X, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));
    QPixmap indestructible(allBlocks.copy(QRect(TEXTURE_INDE_X, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));
    QPixmap aScope(allBlocks.copy(QRect(TEXTURE_ASCOPE_X, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));
    QPixmap aNumber(allBlocks.copy(QRect(TEXTURE_ANUMBER, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));
    QPixmap bonus(allBlocks.copy(QRect(TEXTURE_BONUS, 0, TEXTURE_BLOCS_X, TEXTURE_BLOCS_Y)));

    vbox->addWidget(new QLabel(tr("Bienvenue dans bomberman\n")));
    vbox->addWidget(new QLabel(tr("Le but du jeu est de tuer le deuxième\njoueur par l'explosion d'une bombe.")));
    vbox->addWidget(new QLabel(tr("Voici les différent type de bloc:")));

    QHBoxLayout* hb1 = new QHBoxLayout(this);
    QLabel* p1 = new QLabel();
    p1->setPixmap(destructible);
    hb1->addWidget(p1);
    hb1->addWidget(new QLabel(tr("Destructible avec une bombe")));
    vbox->addLayout(hb1);

    QHBoxLayout* hb2 = new QHBoxLayout(this);
    QLabel* p2 = new QLabel();
    p2->setPixmap(indestructible);
    hb2->addWidget(p2);
    hb2->addWidget(new QLabel(tr("Indestructible")));
    vbox->addLayout(hb2);

    QHBoxLayout* hb3 = new QHBoxLayout(this);
    QLabel* p3 = new QLabel();
    p3->setPixmap(aScope);
    hb3->addWidget(p3);
    hb3->addWidget(new QLabel(tr("Amélioration de la portée des bombes")));
    vbox->addLayout(hb3);

    QHBoxLayout* hb4 = new QHBoxLayout(this);
    QLabel* p4 = new QLabel();
    p4->setPixmap(aNumber);
    hb4->addWidget(p4);
    hb4->addWidget(new QLabel(tr("Ajoute une bombe")));
    vbox->addLayout(hb4);

    QHBoxLayout* hb5 = new QHBoxLayout(this);
    QLabel* p5 = new QLabel();
    p5->setPixmap(bonus);
    hb5->addWidget(p5);
    hb5->addWidget(new QLabel(tr("Donne un des bonus suivant:")));
    vbox->addLayout(hb5);

    vbox->addWidget(new QLabel(tr("invisibilité: vous rend invisible 1 seconde sur deux")));
    vbox->addWidget(new QLabel(tr("invincibilité: vous rend invincible quelques secondes")));
    vbox->addWidget(new QLabel(tr("Super bombe: vos bombes explose tout sur leurs passage")));
    vbox->addWidget(new QLabel(tr("pose automatique: une bombe est automatiquement posée sur chaque case où vous passez\n")));
    vbox->addWidget(new QLabel(tr("Raccourcis clavier")));
    vbox->addWidget(new QLabel(tr("Joueur 1 :")));
    vbox->addWidget(new QLabel(tr("WASD: se déplacer")));
    vbox->addWidget(new QLabel(tr("Barre d'espace : poser une bombes")));
    vbox->addWidget(new QLabel(tr("Joueur 2")));
    vbox->addWidget(new QLabel(tr("Flèches : se déplacer")));
    vbox->addWidget(new QLabel(tr("Enter : poser une bombes")));
    vbox->addWidget(new QLabel("\n\n"));
    vbox->addWidget(new QLabel("Projet p2 de Jonatan Baumgartner, Julien Chappuis et Téo Schaffner"));

}
