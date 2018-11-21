#ifndef MAPBLOC_H
#define MAPBLOC_H

#include <QString>

class MapBloc
{
public:

    MapBloc();
    MapBloc(int t);
    void setType(int);
    int getType();
    bool getTraversable();

private:
    int type;
    bool traversable;
};

#endif // MAPBLOC_H
