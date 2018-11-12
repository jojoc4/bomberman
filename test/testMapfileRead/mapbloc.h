#ifndef MAPBLOC_H
#define MAPBLOC_H


class MapBloc
{
public:
    MapBloc();
    MapBloc(int);
    void setType(int);
    int getType();
    bool getTraversable();

private:
    int type;
    bool traversable;
};

#endif // MAPBLOC_H
