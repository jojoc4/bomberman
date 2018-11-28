/**
 * @author Jonatan Baumgartner
 */
#include "map.h"
#include <fstream>
#include <iostream>
#include <QString>

using namespace std;


/**
 * @brief Map constructor
 */
Map::Map()
{
    t = new MapBloc*[30];
    for(int i = 0; i< 30; i++){
        t[i] = new MapBloc[30];
    }
}

/**
 * @brief read the map form a file
 * @param tpathe to the map
 */
void Map::readFromFile(QString p){
    ifstream file(p.toStdString(), ios::in);

    if(file){
        for(int i = 0; i<30; i++){
            std::string line;
            getline(file, line);
            for(int j = 0; j<30; j++){
                switch(line[j]){
                    case 'I':
                        t[i][j] = MapBloc(1);
                    break;
                    case 'D':
                        t[i][j] = MapBloc(2);
                    break;
                    case 'F':
                        t[i][j] = MapBloc(3);
                    break;
                    case '1':
                        t[i][j] = MapBloc(3);
                        j1 = QPoint(i,j);
                    break;
                    case '2':
                        t[i][j] = MapBloc(3);
                        j2 = QPoint(i,j);
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
    }
}

/**
 * @brief get a specifique mapbloc
 * @param line and column
 * @return specified Mapbloc
 */
MapBloc Map::getMapBloc(int l, int c){
    return t[l][c];
}

QPoint Map::getJ1(){
    return j1;
}

QPoint Map::getJ2(){
    return j2;
}
