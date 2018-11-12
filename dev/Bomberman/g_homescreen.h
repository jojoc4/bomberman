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
};

#endif // G_HOMESCREEN_H
