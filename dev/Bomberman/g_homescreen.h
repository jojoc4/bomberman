#ifndef G_HOMESCREEN_H
#define G_HOMESCREEN_H

#include <QDialog>
#include <QtWidgets>

class G_HomeScreen : public QDialog
{
    Q_OBJECT

public:
    G_HomeScreen(QWidget *parent = 0);
    ~G_HomeScreen();


private :
    QRadioButton* radio_local;
    QRadioButton* radio_network;

    bool gameMode;

private slots:
    void validateChoice(void);
};

#endif // G_HOMESCREEN_H
