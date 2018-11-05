#ifndef BOMB_H
#define BOMB_H


class Bomb
{
public:
    Bomb(int, int);
    void bombThread();

private:
    int type;
    int scope;
};

#endif // BOMB_H
