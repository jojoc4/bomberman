#include "mapbloc.h"

MapBloc::MapBloc()
{

}

MapBloc::MapBloc(int t){
    type=t;
    //voir les types dans la documentation sur git
    if(type<=2){
        traversable=false;
    }else{
        traversable=true;
    }
}

void MapBloc::setType(int t){
    type=t;
    //voir les types dans la documentation sur git
    if(type<=2){
        traversable=false;
    }else{
        traversable=true;
    }
}
int MapBloc::getType(){
    return type;
}
bool MapBloc::getTraversable(){
    return traversable;
}
