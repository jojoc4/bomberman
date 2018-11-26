#ifndef G_MAPCHOOSER_H
#define G_MAPCHOOSER_H

#include <QDialog>
#include <QtWidgets>

class G_MapChooser : public QDialog
{
    Q_OBJECT
public:
    explicit G_MapChooser(QWidget *parent = nullptr);

signals:

public slots:

    void parcourirDossierCarte();
    void validerCarte();

private :
    QListWidget* listMaps;
    QDir* directory;
    QString filePath;
};

#endif // G_MAPCHOOSER_H
