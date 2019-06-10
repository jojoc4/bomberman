/**
*  home windows where we choose gaming mode and start a game
*
* @author	Julien Chappuis
* @version	1.0
*/
#ifndef G_HOMESCREEN_H
#define G_HOMESCREEN_H

#include <QDialog>
#include <QtWidgets>

class G_Help;

class G_HomeScreen : public QWidget
{
    Q_OBJECT

public:
    G_HomeScreen(QWidget *parent = nullptr);

    short getGameMode() const;

    ~G_HomeScreen();

signals:
    void openNextWidget(int);

private :
    short gameMode;
    QRadioButton* radio_local;
    QRadioButton* radio_vs_AI;
    QRadioButton* radio_network;

    G_Help* helpWindow;

private slots:
    void validateChoice();
    void openHelp();

    void openMapChooser();
};

#endif
