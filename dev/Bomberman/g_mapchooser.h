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

public slots:

    void browseFolderMaps();
    void validateMaps();

private :

    void displayThumbnailsMap();

    QListWidget* listMaps;
    QDir* directory;
    QString filePath;

    QGraphicsView* previewMap;
    QGraphicsScene* previewMapScene;

    Game* game;
};

#endif // G_MAPCHOOSER_H
