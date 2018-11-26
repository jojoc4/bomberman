#include "g_mapchooser.h"

/**
 * @brief Constructeur de la classe G_MapChooser
 * @param Herite de QWidget
 */
G_MapChooser::G_MapChooser(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("Choix des cartes");
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QVBoxLayout* vboxLeft = new QVBoxLayout();
    QVBoxLayout* vboxRight = new QVBoxLayout();

    listMaps = new QListWidget();
    listMaps->setSelectionMode( QAbstractItemView::SingleSelection ); // sélection que d'un seul element
    vboxLeft->addWidget(listMaps);
    vboxLeft->addStretch();

    QHBoxLayout *hbox_button = new QHBoxLayout();
    QPushButton *button_valider = new QPushButton("Valider");
    QPushButton *button_parcourir = new QPushButton("Parcourir");

    hbox_button->addWidget(button_valider);
    hbox_button->addWidget(button_parcourir);
    vboxLeft->addLayout(hbox_button);

    // QPainter
    previewMap = new QGraphicsView();
    QVBoxLayout* vBoxPainter = new QVBoxLayout();
    previewMap->setLayout(vBoxPainter);
    vboxRight->addWidget(previewMap);


    hbox->addLayout(vboxLeft);
    hbox->addLayout(vboxRight);


    connect(button_parcourir, &QPushButton::clicked, this, parcourirDossierCarte);
    connect(button_valider, &QPushButton::clicked, this, validerCarte);
}
/**
 * @brief G_MapChooser::parcourirDossierCarte : SLOT
 * Permet de parcourir un dossier et d'y trouver toutes les cartes.
 *
 */
void G_MapChooser::parcourirDossierCarte()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    directory = new QDir(dir);
    QStringList mapsList;
    if (directory->exists())
    {
        mapsList = directory->entryList(QStringList() << "*.nma" << "*.nma",QDir::Files);
        for ( const auto& i : mapsList  )
        {
            new QListWidgetItem(i, listMaps);
        }
    }
}/**
 * @brief G_MapChooser::validerCarte
 * Sélectionne la carte à affichier et quitte le programme.
 */
void G_MapChooser::validerCarte()
{
    if(!listMaps->selectedItems().isEmpty())
    {
        QString element = listMaps->selectedItems().first()->text();
        filePath = directory->absolutePath() + element;
        qDebug() << filePath;
    } else {
        QMessageBox::information(this, "Choix des cartes", tr("Aucune carte n'a été chargée"), QMessageBox::Ok);
    }

}
