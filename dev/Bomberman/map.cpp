#include "map.h"
#include <fstream>
#include <iostream>
#include <QString>

using namespace std;

Map::Map()
{
    // t pointe sur un tableau ** pointeur.
    t = new MapBloc**[30];

    for(int i = 0; i < 30; ++i){
        t[i] = new MapBloc*[30];
    }


}

void Map::readFromFile(QString p){
    //lecture depuis fichier
    ifstream file(p.toStdString(), ios::in);

    if(file){
        //ok
        for(int i = 0; i<30; i++){
            string line;
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
