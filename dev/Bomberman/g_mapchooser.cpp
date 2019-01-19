#include "g_mapchooser.h"

#define TEXTURE_BLOCS_X 30
#define TEXTURE_BLOCS_Y 30

#define SCENE_XY 300

#define SCALED_IMAGE 10

#define TEXTURE_DESTR_X 0
#define TEXTURE_BOTH_Y 0
#define TEXTURE_INDE_X 30
#define TEXTURE_ASCOPE_X 90
#define TEXTURE_ANUMBER 60
#define TEXTURE_BONUS 120

#define WIDGET_GAME 2

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30

/**
 * @brief G_MapChooser::G_MapChooser
 * Constructor
 * @param ptrGame pointer of the game
 * @param parent pointer of the parent
 */
G_MapChooser::G_MapChooser(Game* ptrGame, QWidget *parent)
    : QWidget(parent), game(ptrGame), errorOnOpenMaps(false)
{
    setWindowTitle(tr("Choix des cartes"));

    QHBoxLayout* hbox = new QHBoxLayout(this);
    QVBoxLayout* vboxLeft = new QVBoxLayout();
    QVBoxLayout* vboxRight = new QVBoxLayout();

    listMaps = new QListWidget();
    listMaps->setSelectionMode( QAbstractItemView::SingleSelection ); // sélection que d'un seul element
    vboxLeft->addWidget(listMaps);

    QHBoxLayout *hbox_button = new QHBoxLayout();
    btnValidate = new QPushButton(tr("Valider"),this);
    QPushButton *button_parcourir = new QPushButton(tr("Parcourir"),this);

    hbox_button->addWidget(btnValidate);
    hbox_button->addWidget(button_parcourir);
    vboxLeft->addLayout(hbox_button);

    // QGraphicsView
    previewMap = new QGraphicsView();
    previewMap->resize(SCENE_XY,SCENE_XY);
    previewMapScene = new QGraphicsScene(previewMap);
    previewMapScene->setSceneRect(previewMap->rect());
    previewMap->setScene(previewMapScene);

    previewMap->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    previewMap->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // QGrapgics view
    QVBoxLayout* vBoxPainter = new QVBoxLayout();
    previewMap->setLayout(vBoxPainter);
    vboxRight->addWidget(previewMap);

    this->allBlocks = QPixmap(QString(":/resources/img/Blocs.png"));

    // Loading of the map
    directory = new QDir(QCoreApplication::applicationDirPath());
    directory->cd("maps");

    displayListMap();

    hbox->addLayout(vboxLeft);
    hbox->addLayout(vboxRight);

    connect(button_parcourir, &QPushButton::clicked, this, &G_MapChooser::browseFolderMaps);
    connect(btnValidate, &QPushButton::clicked, this, &G_MapChooser::validateMap);
    connect(listMaps, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(updateThumbnailsMap(QListWidgetItem *)));
}

/**
 * @brief G_MapChooser::~G_MapChooser
 * Delete this object
 */
G_MapChooser::~G_MapChooser()
{}

/**
 * @brief G_MapChooser::browseFolderMaps : SLOT
 * Choose a folder with some maps
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
        if(mapsList.length() < 1)
        {
            QMessageBox::critical(this, tr("Erreur "), tr("Aucune carte ne se trouve dans le dossier"), QMessageBox::Ok);
            btnValidate->setEnabled(false);
        }
        else
        {
            btnValidate->setEnabled(true);
            for ( const auto& i : mapsList  )
            {
                new QListWidgetItem(i, listMaps);
            }
            listMaps->setCurrentRow(0);

            QListWidgetItem * item = listMaps->item(0);
            getMap(item->text());
        }
    }
    else
    {
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

/**
 * @brief G_MapChooser::validateMap
 * selects the map and continues to the game screen
 */
void G_MapChooser::validateMap()
{
    emit(openNextWidget(WIDGET_GAME));
}

/**
 * @brief G_MapChooser::displayThumbnailsMap
 * Displays a thumbnail of the map
 */
void G_MapChooser::displayThumbnailsMap()
{
    if(errorOnOpenMaps){
        return;
    }
    this->previewMapScene->setBackgroundBrush(Qt::gray);
    this->previewMapScene->clear();

    for(int i = 0; i < NB_BLOCS_X; ++i)
    {
        for(int j = 0; j < NB_BLOCS_Y; ++j)
        {
            QPoint bloc(i, j);

            MapBloc* monBloc =  game->getMap()->getMapBloc(bloc);

            if(monBloc == nullptr)
            {
                return;
            }

            int type = monBloc->getType();

            qreal sizeX = this->previewMapScene->width()/NB_BLOCS_X;
            qreal sizeY = this->previewMapScene->height()/NB_BLOCS_Y;

            switch(type){
                case MapBloc::INDESTRUCTIBLE:
                {
                    QPixmap blocImage(allBlocks.copy(QRect(TEXTURE_INDE_X,
                                                           TEXTURE_BOTH_Y,
                                                           TEXTURE_BLOCS_X,
                                                           TEXTURE_BLOCS_Y)));
                    QPixmap blockImageScaled = blocImage.scaled(SCALED_IMAGE,SCALED_IMAGE,Qt::KeepAspectRatio);

                    QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blockImageScaled);
                    item->setPos(i*sizeX, j*sizeY);

                    monBloc->setPtrItemOnScene(item);
                    break;
                }
                case MapBloc::DESTRUCTIBLE:
                {
                    QPixmap blocImage(allBlocks.copy(QRect(TEXTURE_DESTR_X,
                                                           TEXTURE_BOTH_Y,
                                                           NB_BLOCS_X,
                                                           NB_BLOCS_Y)));
                    QPixmap blockImageScaled = blocImage.scaled(SCALED_IMAGE,SCALED_IMAGE,Qt::KeepAspectRatio);
                    QGraphicsPixmapItem *item = this->previewMapScene->addPixmap(blockImageScaled);
                    item->setPos(i*sizeX, j*sizeY);

                    monBloc->setPtrItemOnScene(item);

                    break;
                }
            }
        }
    }
}

/**
 * @brief G_MapChooser::getMap
 * Try to read a specified map file. If it fails, it throws an exception
 * @param name : fileName
 */
void G_MapChooser::getMap(QString name)
{
    try
    {
        game->getMap()->readFromFile(directory->absolutePath() + "/" + name);
        errorOnOpenMaps = false;
        displayThumbnailsMap();
    } catch(const char* error)
    {
        QMessageBox::critical(this, tr("Erreur - ouverture de carte"), tr("Fichier de carte non valide"), QMessageBox::Ok);
        errorOnOpenMaps = true;
    }
}
