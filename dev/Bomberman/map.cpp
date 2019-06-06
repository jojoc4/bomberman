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

    //mutex = new QMutex(QMutex::NonRecursive);
    lock = new QReadWriteLock(QReadWriteLock::NonRecursive);
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
        //mutex->lock();
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
        //mutex->unlock();
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
    //mutex->lock();
    lock->lockForRead();
    MapBloc* b = tabMapBlocs[bloc.x()][bloc.y()];
    lock->unlock();
    //mutex->unlock();

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
    //mutex->lock();
    lock->lockForWrite();
    for(int i = 0; i<NB_BLOCS_X; ++i)
    {
        for(int j = 0; j<NB_BLOCS_Y; ++j)
        {
            MapBloc *bloc = tabMapBlocs[j][i];

            bloc->resetNeighbours();
            bloc->setSeen(false);
            bloc->setVisited(false);

            if(bloc->AIUsable())
            {
                if(tabMapBlocs[j][i-1]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j][i-1]);
                    //qDebug() << "(" << j << ";" << i << ") -> "  << "(" << j << ";" << i-1 << ")";
                }if(tabMapBlocs[j][i+1]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j][i+1]);
                    //qDebug() << "(" << j << ";" << i << ") -> "  << "(" << j << ";" << i+1 << ")";
                }if(tabMapBlocs[j-1][i]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j-1][i]);
                    //qDebug() << "(" << j << ";" << i << ") -> "  << "(" << j-1 << ";" << i << ")";
                }if(tabMapBlocs[j+1][i]->AIUsable()){
                    bloc->addNeighbour(tabMapBlocs[j+1][i]);
                    //qDebug() << "(" << j << ";" << i << ") -> "  << "(" << j+1 << ";" << i << ")";
                }
                //qDebug() << bloc << bloc->getPosition() << *(bloc->getNeighbours());
            }
        }
    }
    //mutex->unlock();
    lock->unlock();

    qDebug() << "Built!";
}

/**
 * @brief Map::getShortestPath
 * @param from MapBloc pointer to the departure
 * @param destination MapBloc pointer to the destination
 * @return QList of MapBloc pointers representing the path to follow
 */
QList<MapBloc*>* Map::getShortestPath(MapBloc* from, MapBloc* destination)
{
    QList<MapBloc*>* path = new QList<MapBloc*>();
    PriorityQueue<MapBloc>* queue = new PriorityQueue<MapBloc>();

    //mutex->lock();
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

    //qDebug() << "bug";
    currentNode->getContent()->setSeen(true);

    //Run through the graph to build the shortest-path tree
    do
    {
        //Add neighbours to the priority queue
        for(int i=0; i<currentNode->getContent()->getNeighbours()->size(); ++i)
        {
            //Only add if it has not already been visited
            MapBloc* b = currentNode->getContent()->getNeighbours()->at(i);
            if(!b->hasBeenVisited())
            {
                //The case where the Block is already in the queue is managed by function insert
                queue->insert(b, currentNode, currentNode->getPriority() + 1);
                b->setSeen(true);
            }
        }

        currentNode = queue->takeMin();
        currentNode->getContent()->setVisited(true);

        //qDebug() << "noeud : " << currentNode->getContent()->getPosition();
    }while(!destination->hasBeenVisited());// || !queue->isEmpty());

    //create path from destination to departure
    do
    {
        //qDebug() << currentNode->getContent()->getPosition();
        //qDebug() << " apres bug1";
        path->push_front(currentNode->getContent());
        //qDebug() << " apres bug2";
        currentNode = currentNode->getFatherNode();
        //qDebug() << " apres bug3" << currentNode;
    }while(currentNode != nullptr && currentNode->getContent() != from);

    //mutex->unlock();
    lock->unlock();

    currentNode = nullptr;

    return path;
}

void Map::run()
{
    buildGraph();
}
