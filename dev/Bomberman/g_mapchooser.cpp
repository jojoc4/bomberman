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
    previewMap->setFixedSize(450,450);
    previewMapScene = new QGraphicsScene(previewMap);
    previewMapScene->setSceneRect(previewMap->rect());
    previewMap->setScene(previewMapScene);

    // position QGrapicsView
    QVBoxLayout* vBoxPainter = new QVBoxLayout();
    previewMap->setLayout(vBoxPainter);
    vboxRight->addWidget(previewMap);

    this->allBlocks = QPixmap(QString(":/resources/img/Blocs.png"));

    // Chargement de la carte
    directory = new QDir(QCoreApplication::applicationDirPath());
    directory->cdUp();
    directory->cdUp();
    directory->cdUp();


    displayListMap();
    QListWidgetItem * item = listMaps->item(1);
    getMap(item->text());

    hbox->addLayout(vboxLeft);
    hbox->addLayout(vboxRight);


    connect(button_parcourir, &QPushButton::clicked, this, &G_MapChooser::browseFolderMaps);
    connect(button_valider, &QPushButton::clicked, this, &G_MapChooser::validateMaps);
    connect(listMaps, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateThumbnailsMap(QListWidgetItem *)));
}
/**
 * @brief G_MapChooser::parcourirDossierCarte : SLOT
 * Permet de parcourir un dossier et d'y trouver toutes les cartes.
 *
 */
void G_MapChooser::browseFolderMaps()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Ouvrir dossier"),
                                                   directory->absolutePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    directory = new QDir(dir);
    displayListMap();
}

void G_MapChooser::displayListMap()
{
    QStringList mapsList;
    if (directory->exists())
    {
        mapsList = directory->entryList(QStringList() << "*.nmm" << "*.nmm",QDir::Files);
        for ( const auto& i : mapsList  )
        {
            new QListWidgetItem(i, listMaps);
        }
    } else {
        QMessageBox::critical(this, tr("Erreur "), tr("Le dossier spécifié n'existe pas"), QMessageBox::Ok);
    }
}
void G_MapChooser::updateThumbnailsMap(QListWidgetItem *item)
{
    getMap(item->text());
    displayThumbnailsMap();

}
/**
 * @brief G_MapChooser::validerCarte
 * Sélectionne la carte à afficher et quitte le programme.
 */
void G_MapChooser::validateMaps()
{
    if(!listMaps->selectedItems().isEmpty())
    {
        QString element = listMaps->selectedItems().first()->text();
        game->getMap()->readFromFile(directory->absolutePath() + "/" + element);
    } else {
        QMessageBox::information(this, tr("Choix des cartes"), tr("Aucune carte n'a été chargée"), QMessageBox::Ok);
    }

}

void G_MapChooser::displayThumbnailsMap(){
    this->previewMapScene->setBackgroundBrush(Qt::gray);
    this->previewMapScene->clear();
    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            QPoint bloc(i,j);
            MapBloc* monBloc =  game->getMap()->getMapBloc(bloc);
            int type = monBloc->getType();

            qreal sizeX = this->previewMapScene->width()/30;
            qreal sizeY = this->previewMapScene->height()/30;

            switch(type){
            case 1:
            {
                //previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::blue),QBrush(Qt::blue));
                QPixmap blocImage(allBlocks.copy(QRect(30, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blocImage);
                item->setPos(i*sizeX, j*sizeY);

                monBloc->setPtrItemOnScene(item);
                break;
            }
            case 2:
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
                QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blocImage);
                item->setPos(i*sizeX, j*sizeY);

                monBloc->setPtrItemOnScene(item);

                break;
            }
            case 3:
                //previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::red),QBrush(Qt::red));
                break;
            default :
                previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::black),QBrush(Qt::black));
            }
        }
    }
}


void G_MapChooser::resizeEvent(QResizeEvent *){
    displayThumbnailsMap();
}
void G_MapChooser::getMap(QString name){
    try{
        game->getMap()->readFromFile(directory->absolutePath() + "/" + name);
        displayThumbnailsMap();
    } catch(const char* error){
        QMessageBox::information(this, tr("Erreur - ouverture de carte"), tr("Aucune carte valide dans le dossier !"), QMessageBox::Ok);
    }

}
