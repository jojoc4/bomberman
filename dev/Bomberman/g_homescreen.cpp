#include "g_homescreen.h"
#include "g_help.h"

/**
 * @brief G_HomeScreen::G_HomeScreen
 * Constructor
 * @param parent : pointer of the parent who create this widget
 */
G_HomeScreen::G_HomeScreen(QWidget *parent)
    : QWidget(parent), helpWindow(nullptr)
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
    QPushButton *btn_aide = new QPushButton(tr("&Aide / A propos"));
    QPushButton *btn_quitter = new QPushButton(tr("&Quitter"));
    QVBoxLayout *vbox_buttons = new QVBoxLayout(this);
    vbox_buttons->addWidget(groupBox_radio);
    vbox_buttons->addWidget(btn_lancer);
    vbox_buttons->addWidget(btn_aide);
    vbox_buttons->addWidget(btn_quitter);

    connect(btn_lancer,SIGNAL(clicked()), this, SLOT(openMapChooser()));
    connect(btn_quitter,SIGNAL(clicked()), parent, SLOT(close()));
    connect(btn_aide,SIGNAL(clicked()), this, SLOT(openHelp()));
    connect(this, SIGNAL(openNextWidget(int)), parent, SLOT(changeWidget(int)));
}
/**
 * @brief G_HomeScreen::~G_HomeScreen
 *
 * Keep clean the memory after the end
 */
G_HomeScreen::~G_HomeScreen()
{

}

/**
 * @brief G_HomeScreen::validateChoice
 * Validate the choice made by the user
 */
void G_HomeScreen::validateChoice()
{
    if (radio_local->isChecked())
    {

    }
    if (radio_network->isChecked())
    {
        QMessageBox::information(this, tr("Information"), tr("Le mode réseau n'est pas encore disponible"));
        radio_network->setChecked(false);
        radio_local->setChecked(true);
    }
}

/**
 * @brief G_HomeScreen::openHelp
 * Pop up the help
 */
void G_HomeScreen::openHelp()
{
    helpWindow = new G_Help();
    helpWindow->show();
}

/**
 * @brief G_HomeScreen::openMapChooser
 * Emit the signal to announced that the user is ready to choice the map
 */
void G_HomeScreen::openMapChooser()
{
    emit(openNextWidget(1));
}
