#include "g_homescreen.h"

G_HomeScreen::G_HomeScreen(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Accueil"));

    QGroupBox *groupBox_radio = new QGroupBox("Mode de jeu ",this);
    QRadioButton *radio_local = new QRadioButton(tr("2 joueurs sur le même pc"),this);
    radio_local->setChecked(true);
    QRadioButton *radio_network = new QRadioButton(tr("2 joueurs en réseau"),this);

    QVBoxLayout *vbox_radio = new QVBoxLayout(this);
    vbox_radio->addWidget(radio_local);
    vbox_radio->addWidget(radio_network);
    vbox_radio->addStretch(1);
    groupBox_radio->setLayout(vbox_radio);

    QPushButton *btn_lancer = new QPushButton(tr("&Lancer le jeu"));
    QPushButton *btn_aide = new QPushButton(tr("&Aide"));
    QPushButton *btn_quitter = new QPushButton(tr("&Quitter"));
    QVBoxLayout *vbox_buttons = new QVBoxLayout(this);
    vbox_buttons->addWidget(groupBox_radio);
    vbox_buttons->addWidget(btn_lancer);
    vbox_buttons->addWidget(btn_aide);
    vbox_buttons->addWidget(btn_quitter);

}


G_HomeScreen::~G_HomeScreen()
{

}
