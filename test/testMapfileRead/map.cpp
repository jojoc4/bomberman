#include "map.h"
#include <fstream>
#include <iostream>
#include <QString>
#include <String>

using namespace std;

Map::Map()
{
    t = new MapBloc*[30];
    for(int i = 0; i< 30; i++){
        t[i] = new MapBloc[30];
    }
}

void Map::readFromFile(QString p){
    //lecture depuis fichier
    ifstream file(p.toStdString(), ios::in);

    if(file){
        //ok
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
                    case 'A':
                        t[i][j] = MapBloc(4);
                    break;
                    case 'B':
                        t[i][j] = MapBloc(5);
                    break;
                    case '1':
                        t[i][j] = MapBloc(3);
                        j1x =j;
                        j1y= i;
                    break;
                    case '2':
                        t[i][j] = MapBloc(3);
                        j2x =j;
                        j2y= i;
                    break;
                    default:
                        throw "Map read error";
                    break;
                }

            }
        }

        file.close();
    }else{
        //error
    }
}
