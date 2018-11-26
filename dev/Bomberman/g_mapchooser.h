#ifndef G_MAPCHOOSER_H
#define G_MAPCHOOSER_H

#include <QtWidgets>

class G_MapChooser : public QWidget
{
    Q_OBJECT
public:
    explicit G_MapChooser(QWidget *parent = nullptr);

protected :


signals:

public slots:

    void parcourirDossierCarte();
    void validerCarte();

private :
    QListWidget* listMaps;
    QDir* directory;
    QString filePath;

    QGraphicsView* previewMap;

};

#endif // G_MAPCHOOSER_H
