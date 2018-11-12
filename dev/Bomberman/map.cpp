#include "map.h"
#include <fstream>
#include <iostream>
#include <QString>

using namespace std;

Map::Map()
{
    t= new MapBloc[30][30];
}

void Map::readFromFile(String p){
    //lecture depuis fichier
    ifstream file(p, ios::in);

    if(file){
        //ok
        for(int i = 0; i<30; i++){
            Qstring line;
            getline(file, line);
            for(int j = 0; j<30; j++){
                t[i][j] = new MapBloc(line[j]);
            }
        }

        file.close();
    }else{
        //error
    }
}
