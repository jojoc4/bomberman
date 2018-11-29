/**
 * @author Jonatan Baumgartner
 */
#include "mapbloc.h"

MapBloc::MapBloc(){

}

/**
 * @brief MapBloc constructor
 * @param type (type of bloc, see the wiki)
 */
MapBloc::MapBloc(int type){
    type=type;
    if(type<=2){
        traversable=false;
    }else{
        traversable=true;
    }
}

/**
 * @brief change type
 * @param type (type of bloc, see the wiki)
 */
void MapBloc::setType(int type){
    type=type;
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
