#include "map.h"
#include "mapbloc.h"
#include <fstream>
#include <iostream>

#define NB_BLOCS_X 30
#define NB_BLOCS_Y 30

using namespace std;

#include <QDebug>

/**
 * @brief Map constructor
 */
Map::Map()
{
    tabMapBlocs = new MapBloc**[NB_BLOCS_X];
    for(int i = 0; i< NB_BLOCS_X; ++i)
    {
        tabMapBlocs[i] = new MapBloc*[NB_BLOCS_Y];

        for(int j = 0; j < NB_BLOCS_Y; ++j)
        {
            tabMapBlocs[i][j] = nullptr;
        }
    }

    lock = new QReadWriteLock(QReadWriteLock::Recursive);
}

/**
 * @brief Map::~Map
 */
Map::~Map()
{
    lock->lockForWrite();
    for(int i = 0; i<NB_BLOCS_X; ++i)
    {
        for(int j = 0; j<NB_BLOCS_Y; ++j)
        {
            delete tabMapBlocs[i][j];
            tabMapBlocs[i][j] = nullptr;
        }
        tabMapBlocs[i]=nullptr;
    }
    tabMapBlocs=nullptr;
    lock->unlock();

    delete lock;
    lock = nullptr;
}

/**
 * @brief Map::readFromFile(QString path)
 * read the map from a file
 * @param p (pathe to the map)
 */
void Map::readFromFile(QString path)
{
    ifstream file(path.toStdString(), ios::in);

    if(file)
    {
        lock->lockForWrite();
        for(int i = 0; i<NB_BLOCS_X; ++i)
        {
            std::string line;
            getline(file, line);
            for(int j = 0; j<NB_BLOCS_Y; ++j)
            {
                switch(line[j]){
                    case 'I':
                        tabMapBlocs[j][i] = new MapBloc(MapBloc::INDESTRUCTIBLE);
                        break;
                    case 'D':
                        tabMapBlocs[j][i] = new MapBloc(MapBloc::DESTRUCTIBLE);
                        break;
                    case 'F':
                        tabMapBlocs[j][i] = new MapBloc(MapBloc::BACKGROUND);
                        break;
                    case '1':
                        tabMapBlocs[j][i] = new MapBloc(MapBloc::BACKGROUND);
                        p1 = QPoint(j, i);
                        break;
                    case '2':
                        tabMapBlocs[j][i] = new MapBloc(MapBloc::BACKGROUND);
                        p2 = QPoint(j, i);
                        break;
                    default:
                        throw "Map read error";
                        break;
                }
            tabMapBlocs[j][i]->setPosition(QPoint(j,i));
            }
        }
        lock->unlock();

        file.close();
    }else{
        //TODO error
        throw exception();
    }
}

/**
 * @brief get a specifique mapbloc
 * @param bloc (QPoint line column)
 * @return specified Mapbloc
 */
MapBloc* Map::getMapBloc(QPoint bloc) const
{
    lock->lockForRead();
    MapBloc* b = tabMapBlocs[bloc.x()][bloc.y()];
    lock->unlock();

    return b;
}

/**
 * @brief Map::getPlayerSpawn
 * @param nbPlayer (0 or false is player1 and 1 or true is player2)
 * @return Qpoint representing the spawn case
 */
QPoint Map::getPlayerSpawn(bool nbPlayer) const
{
    if(!nbPlayer)
        return p1;
    else
        return p2;
}

/**
 * @brief Map::buildGraph
 * Creates the graph for Dijkstra's pathfinding, using adjacency lists in every bloc
 */
void Map::buildGraph()
{
    lock->lockForWrite();
    for(int i = 0; i<NB_BLOCS_X; ++i)
    {
        for(int j = 0; j<NB_BLOCS_Y; ++j)
        {
            MapBloc *bloc = tabMapBlocs[j][i];

            bloc->resetNeighbours();
            bloc->setSeen(false);
            bloc->setVisited(false);

            if(bloc->AIUsable() > 0)
            {
                if(tabMapBlocs[j+1][i]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j+1][i]);
                }if(tabMapBlocs[j][i-1]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j][i-1]);
                }if(tabMapBlocs[j-1][i]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j-1][i]);
                }if(tabMapBlocs[j][i+1]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j][i+1]);
                }
            }
        }
    }
    lock->unlock();
}

/**
 * @brief Map::getShortestPath
 * @param from MapBloc pointer to the departure
 * @param destination MapBloc pointer to the destination
 * @return QList of MapBloc pointers representing the path to follow
 */
QList<MapBloc*>* Map::getShortestPath(MapBloc* from, MapBloc* destination, bool useDestructibleBlocks)
{
    QList<MapBloc*>* path = new QList<MapBloc*>();
    PriorityQueue<MapBloc>* queue = new PriorityQueue<MapBloc>();

    lock->lockForRead();

    //Dijkstra's algorithm :

    //Set all blocks as not-visited and not-seen
    for(int i = 0; i<NB_BLOCS_X; ++i)
    {
        for(int j = 0; j<NB_BLOCS_Y; ++j)
        {
            tabMapBlocs[j][i]->setSeen(false);
            tabMapBlocs[j][i]->setVisited(false);
        }
    }

    //Add first node (where the player currently is, his starting place)
    PriorityQueueNode<MapBloc>* currentNode = queue->at(queue->insert(from, 0));

    currentNode->getContent()->setSeen(true);

    //Run through the graph to build the shortest-path tree
    do
    {
        //Add neighbours to the priority queue
        for(int i=0; i<currentNode->getContent()->getNeighbours()->size(); ++i)
        {
            if(useDestructibleBlocks || (!useDestructibleBlocks && currentNode->getContent()->getTraversable()) )
            {
                //Only add if it has not already been visited
                MapBloc* b = currentNode->getContent()->getNeighbours()->at(i);
                if(!b->hasBeenVisited())
                {
                    //The case where the Block is already in the queue is managed by function insert
                    queue->insert(b, currentNode, currentNode->getPriority() + b->getBasePriority());
                    b->setSeen(true);
                }
            }
        }

        currentNode = queue->takeMin();
        currentNode->getContent()->setVisited(true);

    }while(!destination->hasBeenVisited());

    //create path from destination to departure
    do
    {
        path->push_front(currentNode->getContent());
        currentNode = currentNode->getFatherNode();
    }while(currentNode != nullptr && currentNode->getContent() != from);

    lock->unlock();

    currentNode = nullptr;
    delete queue;
    queue = nullptr;

    return path;
}

MapBloc* Map::findSafePlace(MapBloc* from) const
{
    MapBloc* safeDestination = from;
    int x = from->getPosition().x();
    int y = from->getPosition().y();

    bool top = true;
    bool bottom = true;
    bool left = true;
    bool right = true;

    int distance = 1;

    lock->lockForRead();
    do{
        //TOP
        if(top && tabMapBlocs[x][y-distance]->getTraversable()){
            if(tabMapBlocs[x-1][y-distance]->getTraversable()){
                safeDestination = tabMapBlocs[x-1][y-distance];
                break;
            }
            if(tabMapBlocs[x+1][y-distance]->getTraversable()){
                safeDestination = tabMapBlocs[x+1][y-distance];
                break;
            }
        }
        else if(!tabMapBlocs[x][y-distance]->getTraversable()){
            top = false;
        }

        //LEFT
        if(left && tabMapBlocs[x-distance][y]->getTraversable()){
            if(tabMapBlocs[x-distance][y-1]->getTraversable()){
                safeDestination = tabMapBlocs[x-distance][y-1];
                break;
            }
            if(tabMapBlocs[x-distance][y+1]->getTraversable()){
                safeDestination = tabMapBlocs[x-distance][y+1];
                break;
            }
        }
        else if(!tabMapBlocs[x][y-distance]->getTraversable()){
            left = false;
        }

        //BOTTOM
        if(bottom && tabMapBlocs[x][y+distance]->getTraversable()){
            if(tabMapBlocs[x-1][y+distance]->getTraversable()){
                safeDestination = tabMapBlocs[x-1][y+distance];
                break;
            }
            if(tabMapBlocs[x+1][y+distance]->getTraversable()){
                safeDestination = tabMapBlocs[x+1][y+distance];
                break;
            }
        }
        else if(!tabMapBlocs[x][y+distance]->getTraversable()){
            bottom = false;
        }

        //RIGHT
        if(right && tabMapBlocs[x+distance][y]->getTraversable()){
            if(tabMapBlocs[x+distance][y-1]->getTraversable()){
                safeDestination = tabMapBlocs[x+distance][y-1];
                break;
            }
            if(tabMapBlocs[x+distance][y+1]->getTraversable()){
                safeDestination = tabMapBlocs[x+distance][y+1];
                break;
            }
        }
        else if(!tabMapBlocs[x][y-distance]->getTraversable()){
            right = false;
        }

        ++distance;
    }while(top || left || bottom || right);

    lock->unlock();

    return safeDestination;
}

QList<MapBloc*>* Map::getPathToSafety(MapBloc* from)
{
    return getShortestPath(from, findSafePlace(from), false);
}

void Map::run()
{
    this->buildGraph();
}
