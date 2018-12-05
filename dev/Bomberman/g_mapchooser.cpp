#include "g_mapchooser.h"

/**
 * @brief Constructeur de la classe G_MapChooser
 * @param Herite de QWidget
 */
G_MapChooser::G_MapChooser(Game* ptrGame, QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("Choix des cartes"));
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QVBoxLayout* vboxLeft = new QVBoxLayout();
    QVBoxLayout* vboxRight = new QVBoxLayout();

    game = ptrGame;

    listMaps = new QListWidget();
    listMaps->setSelectionMode( QAbstractItemView::SingleSelection ); // sélection que d'un seul element
    vboxLeft->addWidget(listMaps);
    vboxLeft->addStretch();

    QHBoxLayout *hbox_button = new QHBoxLayout();
    QPushButton *button_valider = new QPushButton(tr("Valider"),this);
    QPushButton *button_parcourir = new QPushButton(tr("Parcourir"),this);

    hbox_button->addWidget(button_valider);
    hbox_button->addWidget(button_parcourir);
    vboxLeft->addLayout(hbox_button);

    // QGraphicsView
    previewMap = new QGraphicsView();
    previewMapScene = new QGraphicsScene(previewMap);
    previewMapScene->setSceneRect(previewMap->rect());
    previewMap->setScene(previewMapScene);

    // position QGrapicsView
    QVBoxLayout* vBoxPainter = new QVBoxLayout();
    previewMap->setLayout(vBoxPainter);
    vboxRight->addWidget(previewMap);

    // Chargement de la carte
    QDir dossier(QCoreApplication::applicationDirPath());

    dossier.cdUp();
    dossier.cdUp();
    dossier.cdUp();
    qDebug() << dossier.absolutePath() + "/mapTest.nmm";
    game->getMap()->readFromFile(dossier.absolutePath() + "/mapTest.nmm");
    displayThumbnailsMap();

    hbox->addLayout(vboxLeft);
    hbox->addLayout(vboxRight);


    connect(button_parcourir, &QPushButton::clicked, this, &G_MapChooser::browseFolderMaps);
    connect(button_valider, &QPushButton::clicked, this, &G_MapChooser::validateMaps);
}
/**
 * @brief G_MapChooser::parcourirDossierCarte : SLOT
 * Permet de parcourir un dossier et d'y trouver toutes les cartes.
 *
 */
void G_MapChooser::browseFolderMaps()
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
void G_MapChooser::validateMaps()
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

void G_MapChooser::displayThumbnailsMap(){
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            QPoint bloc(i,j);
            MapBloc* monBloc = game->getMap()->getMapBloc(bloc);
            int type = monBloc->getType();

            switch(type){
            case 1:
                previewMapScene->addRect(j*(width()/30.),i*(height()/30.),previewMap->width()/30.,previewMap->height()/30.,QPen(Qt::blue),QBrush(Qt::blue));
                break;
            case 2:
                previewMapScene->addRect(j*(width()/30.),i*(height()/30.),previewMap->width()/30.,previewMap->height()/30.,QPen(Qt::yellow),QBrush(Qt::yellow));
                break;
            case 3:
                previewMapScene->addRect(j*(width()/30.),i*(height()/30.),previewMap->width()/30.,previewMap->height()/30.,QPen(Qt::red),QBrush(Qt::red));
                break;
            default :
                previewMapScene->addRect(j*(width()/30.),i*(height()/30.),previewMap->width()/30.,previewMap->height()/30.,QPen(Qt::black),QBrush(Qt::black));
            }

        }
    }
}