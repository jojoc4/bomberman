#ifndef MAPBLOC_H
#define MAPBLOC_H


class MapBloc
{
public:
    MapBloc(int, bool);
    void setType(int);

private:
    int type;
    bool traverable;
};

#endif // MAPBLOC_H
