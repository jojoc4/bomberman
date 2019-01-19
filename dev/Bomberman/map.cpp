#include "map.h"
#include "mapbloc.h"
#include <fstream>
#include <iostream>

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30

using namespace std;

/**
 * @brief Map constructor
 */
Map::Map()
{
    t = new MapBloc**[NB_BLOCS_X];
    for(int i = 0; i< NB_BLOCS_X; ++i)
    {
        t[i] = new MapBloc*[NB_BLOCS_Y];

        for(int j = 0; j < NB_BLOCS_Y; ++j)
        {
            t[i][j] = nullptr;
        }
    }
}

/**
 * @brief Map::~Map
 */
Map::~Map()
{
    for(int i = 0; i<NB_BLOCS_X; ++i)
    {
        for(int j = 0; j<NB_BLOCS_Y; ++j)
        {
            delete t[i][j];
            t[i][j] = nullptr;
        }
        t[i]=nullptr;
    }
    t=nullptr;
}

/**
 * @brief Map::readFromFile(QString path)
 * read the map from a file
 * @param p (pathe to the map)
 */
void Map::readFromFile(QString path)
{
    ifstream file(path.toStdString(), ios::in);

    if(file)
    {
        for(int i = 0; i<NB_BLOCS_X; ++i)
        {
            std::string line;
            getline(file, line);
            for(int j = 0; j<NB_BLOCS_Y; ++j)
            {
                switch(line[j]){
                    case 'I':
                        t[j][i] = new MapBloc(MapBloc::INDESTRUCTIBLE);
                    break;
                    case 'D':
                        t[j][i] = new MapBloc(MapBloc::DESTRUCTIBLE);
                    break;
                    case 'F':
                        t[j][i] = new MapBloc(MapBloc::BACKGROUND);
                    break;
                    case '1':
                        t[j][i] = new MapBloc(MapBloc::BACKGROUND);
                        j1 = QPoint(j, i);
                    break;
                    case '2':
                        t[j][i] = new MapBloc(MapBloc::BACKGROUND);
                        j2 = QPoint(j, i);
                    break;
                    default:
                        throw "Map read error";
                    break;
                }

            }
        }

        file.close();
    }else{
        //TODO error
        throw exception();
    }
}

/**
 * @brief get a specifique mapbloc
 * @param p (QPoint line column)
 * @return specified Mapbloc
 */
MapBloc* Map::getMapBloc(QPoint bloc) const
{
    return t[bloc.x()][bloc.y()];
}

/**
 * @brief Map::getPlayerSpawn
 * @param nbPlayer (0 or false is player1 and 1 or true is player2)
 * @return Qpoint representing the spawn case
 */
QPoint Map::getPlayerSpawn(bool nbPlayer) const
{
    if(!nbPlayer)
        return j1;
    else
        return j2;
}
