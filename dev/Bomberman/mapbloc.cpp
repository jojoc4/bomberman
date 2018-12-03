/**
 * @author Jonatan Baumgartner
 */
#include "mapbloc.h"
#include <QRandomGenerator>

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

/**
 * @brief called when the bloc explode, changing his type in void, bonus or improvment
 */
void MapBloc::explode(){
    ///chance out of ten to have
    int bonus=2;
    int rangeImprovment=2;
    int numberImprovment=2;

    QRandomGenerator random = QRandomGenerator();
    int r = random.bounded(9)+1;
    if(r<=bonus){
        setType(5);
    }else if(r<=bonus+rangeImprovment){
        setType(6);
    }else if(r<=bonus+rangeImprovment+numberImprovment){
        setType(4);
    }else{
        setType(3);
    }


}

int MapBloc::getType(){
    return type;
}

bool MapBloc::getTraversable(){
    return traversable;
}
