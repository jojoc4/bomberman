#include "mapbloc.h"

MapBloc::MapBloc(){

}

/**
 * @brief MapBloc constructor
 * @param type (type of bloc, see the wiki)
 */
MapBloc::MapBloc(int type){
    this->type=type;
    if(this->type<=2){
        this->traversable=false;
    }else{
        this->traversable=true;
    }
    ptrItemOnScene=nullptr;
}

/**
 * @brief change type
 * @param type (type of bloc, see the wiki)
 */
void MapBloc::setType(int type){
    this->type=type;
    if(type<=2){
        this->traversable=false;
    }else{
        this->traversable=true;
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

    int r = (qrand() % ((10 + 1) - 1) + 1);
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
    return this->type;
}

bool MapBloc::getTraversable(){
    return this->traversable;
}

QGraphicsItem* MapBloc::getPtrItemOnScene()
{
    return ptrItemOnScene;
}

void MapBloc::setPtrItemOnScene(QGraphicsItem *item)
{
    ptrItemOnScene= item;
}
