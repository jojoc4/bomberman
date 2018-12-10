#include "map.h"
#include <fstream>
#include <iostream>

using namespace std;


/**
 * @brief Map constructor
 */
Map::Map()
{
    t = new MapBloc**[30];
    for(int i = 0; i< 30; i++){
        t[i] = new MapBloc*[30];
    }
}

Map::~Map(){
    for(int i = 0; i<30; i++){
        for(int j = 0; j<30; j++){
            delete t[i][j];
            t[i][j] = nullptr;
        }
        t[i]=nullptr;
    }
    t=nullptr;
    delete j1;
    delete j2;
}

/**
 * @brief read the map form a file
 * @param p (pathe to the map)
 */
void Map::readFromFile(QString path){
    ifstream file(path.toStdString(), ios::in);

    if(file){
        for(int i = 0; i<30; i++){
            std::string line;
            getline(file, line);
            for(int j = 0; j<30; j++){
                switch(line[j]){
                    case 'I':
                        t[j][i] = new MapBloc(1);
                    break;
                    case 'D':
                        t[j][i] = new MapBloc(2);
                    break;
                    case 'F':
                        t[j][i] = new MapBloc(3);
                    break;
                    case '1':
                        t[j][i] = new MapBloc(3);
                        j1 = QPoint(j, i);
                    break;
                    case '2':
                        t[j][i] = new MapBloc(3);
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
MapBloc* Map::getMapBloc(QPoint bloc){
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
