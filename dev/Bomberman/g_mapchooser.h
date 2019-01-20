/**
* widget to choose the map we want to play
*
* @author	Julien Chappuis
* @version	1.0
*/
#ifndef G_MAPCHOOSER_H
#define G_MAPCHOOSER_H

#include <QtWidgets>

class Map;
class Game;

class G_MapChooser : public QWidget
{
    Q_OBJECT
public:
    explicit G_MapChooser(Game* ptrGame, QWidget *parent = nullptr, QString startPath = "");
    ~G_MapChooser();

    QString getDirectoryPath() const;

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

    void getMap(QString name);

    QListWidget* listMaps;
    QDir* directory;

    QPushButton* btnValidate;

    QGraphicsView* previewMap;
    QGraphicsScene* previewMapScene;
    QPixmap allBlocks;
    Game* game;

    bool errorOnOpenMaps;
};

#endif
