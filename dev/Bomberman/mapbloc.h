#ifndef MAPBLOC_H
#define MAPBLOC_H

#include <QString>

class MapBloc
{
public:

    MapBloc();
    MapBloc(int type);
    void setType(int type);
    int getType();
    bool getTraversable();

private:
    int type;
    bool traversable;
};

#endif // MAPBLOC_H
