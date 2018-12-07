#ifndef G_HOMESCREEN_H
#define G_HOMESCREEN_H

#include <QDialog>
#include <QtWidgets>
#include "g_help.h"

class G_HomeScreen : public QWidget
{
    Q_OBJECT

public:
    G_HomeScreen(QWidget *parent = 0);

    ~G_HomeScreen();


private :
    QRadioButton* radio_local;
    QRadioButton* radio_network;

    G_Help* helpWindow;

private slots:
    void validateChoice(void);
    void openHelp();
};

#endif // G_HOMESCREEN_H
