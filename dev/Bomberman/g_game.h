#ifndef G_GAME_H
#define G_GAME_H

#include <QMainWindow>
#include <QLabel>

class G_Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit G_Game(QWidget *parent = nullptr);

signals:

public slots:

private:
    QLabel labelPlayer1;
    QLabel labelPlayer2;
    QLabel textPlayer1;
    QLabel textPlayer2;
};

#endif // G_GAME_H
