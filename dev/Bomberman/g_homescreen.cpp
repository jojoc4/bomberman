#include "g_homescreen.h"

G_HomeScreen::G_HomeScreen(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("Accueil"));

    QGroupBox *groupBox_radio = new QGroupBox("Mode de jeu ",this);
    radio_local = new QRadioButton(tr("2 joueurs sur le même pc"),this);
    radio_local->setChecked(true);
    radio_network = new QRadioButton(tr("2 joueurs en réseau"),this);

    QVBoxLayout *vbox_radio = new QVBoxLayout(this);
    vbox_radio->addWidget(radio_local);
    vbox_radio->addWidget(radio_network);
    vbox_radio->addStretch(1);
    groupBox_radio->setLayout(vbox_radio);

    connect(radio_local, SIGNAL(clicked(bool)) , this, SLOT(validateChoice()));
    connect(radio_network, SIGNAL(clicked(bool)) , this, SLOT(validateChoice()));

    QPushButton *btn_lancer = new QPushButton(tr("&Lancer le jeu"));
    QPushButton *btn_aide = new QPushButton(tr("&Aide"));
    QPushButton *btn_quitter = new QPushButton(tr("&Quitter"));
    QVBoxLayout *vbox_buttons = new QVBoxLayout(this);
    vbox_buttons->addWidget(groupBox_radio);
    vbox_buttons->addWidget(btn_lancer);
    vbox_buttons->addWidget(btn_aide);
    vbox_buttons->addWidget(btn_quitter);


    connect(btn_quitter,SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_aide,SIGNAL(clicked()), this, SLOT(close()));

}


G_HomeScreen::~G_HomeScreen()
{

}
void G_HomeScreen::validateChoice(void)
{
    qDebug() << 1234;
    if (radio_local->isChecked())
    {

    }
    if (radio_network->isChecked())
    {
        QMessageBox::information(this, "Information", "Le mode réseau n'est pas encore disponible");
        radio_network->setChecked(false);
        radio_local->setChecked(true);
    }
}
