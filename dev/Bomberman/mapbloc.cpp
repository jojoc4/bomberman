#include "mapbloc.h"

MapBloc::MapBloc()
{

}

MapBloc::MapBloc(int type){
    type=t;
    //voir les types dans la documentation sur git
    if(type<=2){
        traverable=false;
    }else{
        traverable=true;
    }
}

void MapBloc::setType(int t){
    type=t;
    //voir les types dans la documentation sur git
    if(type<=2){
        traverable=false;
    }else{
        traverable=true;
    }
}
int MapBloc::getType(){
    return type;
}
bool MapBloc::getTraversable(){
    return traversable;
}
