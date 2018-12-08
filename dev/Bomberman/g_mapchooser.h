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

protected :


signals:
    void openNextWidget(int);

private slots :

    void browseFolderMaps();
    void validateMap();
    void updateThumbnailsMap(QListWidgetItem* item);

private :

    void displayThumbnailsMap();
    void displayListMap();


    void getMap(QString name);

    void resizeEvent(QResizeEvent *);



    QListWidget* listMaps;
    QDir* directory;
    QString filePath;

    QPushButton* btnValidate;

    QGraphicsView* previewMap;
    QGraphicsScene* previewMapScene;
    QPixmap allBlocks;
    Game* game;
};

#endif // G_MAPCHOOSER_H
