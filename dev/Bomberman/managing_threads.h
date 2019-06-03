#ifndef AI_ACTION_THREAD_H
#define AI_ACTION_THREAD_H

#include <QObject>
#include <QWidget>
#include "ai_player.h"
#include "map.h"

/**
 * @brief The AI_Action_Thread class : calls the AI player's actions every given time
 */
class AI_Action_Thread : public QObject
{
public:
    void act(AI_Player* player, QWidget* widget = nullptr)
    {
        player->act(widget);
    }
};

/**
 * @brief The Rebuild_Graph_Thread class : calls the map's graph building function everytime a bomb explodes
 */
class Rebuild_Graph_Thread : public QObject
{
public:
    void buildGraph(Map* map)
    {
        map->buildGraph();
    }
};

#endif // AI_ACTION_THREAD_H
