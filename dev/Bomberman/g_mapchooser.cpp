#include "g_mapchooser.h"

/**
 * @brief Constructor G_MapChooser
 * @param ptrGame Game*
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
    //vboxLeft->addStretch();

    QHBoxLayout *hbox_button = new QHBoxLayout();
    btnValidate = new QPushButton(tr("Valider"),this);
    QPushButton *button_parcourir = new QPushButton(tr("Parcourir"),this);

    hbox_button->addWidget(btnValidate);
    hbox_button->addWidget(button_parcourir);
    vboxLeft->addLayout(hbox_button);

    // QGraphicsView
    previewMap = new QGraphicsView();
    previewMap->resize(300,300);
    previewMapScene = new QGraphicsScene(previewMap);
    previewMapScene->setSceneRect(previewMap->rect());
    previewMap->setScene(previewMapScene);

    previewMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    previewMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // position QGraphicsView
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

    hbox->addLayout(vboxLeft);
    hbox->addLayout(vboxRight);

    connect(button_parcourir, &QPushButton::clicked, this, &G_MapChooser::browseFolderMaps);
    connect(btnValidate, &QPushButton::clicked, this, &G_MapChooser::validateMap);
    connect(listMaps, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateThumbnailsMap(QListWidgetItem *)));
    connect(this,SIGNAL(openNextWidget(int)),parent,SLOT(changeWidget(int)));
}

G_MapChooser::~G_MapChooser(){

}
/**
 * @brief G_MapChooser::browseFolderMaps : SLOT
 * Choose a folder with some maps
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
/**
 * @brief G_MapChooser::displayListMap
 * Display the list of maps available
 */
void G_MapChooser::displayListMap()
{
    QStringList mapsList;
    if (directory->exists())
    {
        mapsList = directory->entryList(QStringList() << "*.nmm" << "*.nmm",QDir::Files);
        listMaps->clear();
        if(mapsList.length() < 1){
            QMessageBox::critical(this, tr("Erreur "), tr("Aucune carte ne se trouve dans le dossier"), QMessageBox::Ok);
            btnValidate->setEnabled(false);
        } else {
            btnValidate->setEnabled(true);
            for ( const auto& i : mapsList  )
            {
                new QListWidgetItem(i, listMaps);
            }

            listMaps->setCurrentRow(0);

            QListWidgetItem * item = listMaps->item(0);
            getMap(item->text());
        }
    } else {
        QMessageBox::critical(this, tr("Erreur "), tr("Le dossier spécifié n'existe pas"), QMessageBox::Ok);
    }

}
/**
 * @brief G_MapChooser::updateThumbnailsMap
 * @param item
 * Update the thumbnails
 */
void G_MapChooser::updateThumbnailsMap(QListWidgetItem *item)
{
    getMap(item->text());
    displayThumbnailsMap();
}

void G_MapChooser::validateMap()
{
    emit(openNextWidget(2));
    emit(startGame());
}

void G_MapChooser::displayThumbnailsMap(){
    this->previewMapScene->setBackgroundBrush(Qt::gray);
    this->previewMapScene->clear();

    for(int i = 0; i < 30; i++){
        for(int j = 0; j < 30; j++){
            QPoint bloc(i, j);

            MapBloc* monBloc =  game->getMap()->getMapBloc(bloc);

            if(monBloc == nullptr){
                return;
            }

            int type = monBloc->getType();

            qreal sizeX = this->previewMapScene->width()/30;
            qreal sizeY = this->previewMapScene->height()/30;

            switch(type){
            case 1:
            {
                //previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::blue),QBrush(Qt::blue));
                QPixmap blocImage(allBlocks.copy(QRect(30, 0, 30, 30)));
                QPixmap blockImageScaled = blocImage.scaled(10,10,Qt::KeepAspectRatio);

                QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blockImageScaled);
                item->setPos(i*sizeX, j*sizeY);

                monBloc->setPtrItemOnScene(item);
                break;
            }
            case 2:
            {
                QPixmap blocImage(allBlocks.copy(QRect(0, 0, 30, 30)));
                QPixmap blockImageScaled = blocImage.scaled(10,10,Qt::KeepAspectRatio);


                QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blockImageScaled);
                item->setPos(i*sizeX, j*sizeY);

                monBloc->setPtrItemOnScene(item);

                break;
            }
            case 3:
                //previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::red),QBrush(Qt::red));
                break;
                // default :
                //previewMapScene->addRect(j*(sizeX),i*(sizeY),sizeX,sizeY,QPen(Qt::black),QBrush(Qt::black));
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
        QMessageBox::critical(this, tr("Erreur - ouverture de carte"), tr("Fichier de carte non valide"), QMessageBox::Ok);
    }
}
