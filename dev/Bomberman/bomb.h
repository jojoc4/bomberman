#ifndef BOMB_H
#define BOMB_H


class Bomb
{
public:
    Bomb(int, int);
    void bombThread();
    int getType();
    int getRange();
    int getLine();
    int getColumn();

private:
    int type;
    int range;
    int line;
    int column;
};

#endif // BOMB_H
