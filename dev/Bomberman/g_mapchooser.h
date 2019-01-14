/**
* widget to choose the map we want to play
*
* @author	Julien Chappuis
* @version	0.1
*/
#ifndef G_MAPCHOOSER_H
#define G_MAPCHOOSER_H

#include <QtWidgets>
#include "map.h"
#include "game.h"

class G_MapChooser : public QWidget
{
    Q_OBJECT
public:
    explicit G_MapChooser(Game* ptrGame, QWidget *parent = nullptr);

    ~G_MapChooser();

protected :


signals:
    void openNextWidget(int);
    void startGame();

private slots :

    void browseFolderMaps();
    void validateMap();
    void updateThumbnailsMap(QListWidgetItem* item);

private :

    void displayThumbnailsMap();
    void displayListMap();
    void resizeEvent(QResizeEvent *);

    void getMap(QString name);

    QListWidget* listMaps;
    QDir* directory;
    QString filePath;

    QPushButton* btnValidate;

    QGraphicsView* previewMap;
    QGraphicsScene* previewMapScene;
    QPixmap allBlocks;
    Game* game;
};

#endif
