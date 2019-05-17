#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QList>
#include <iostream>
#include <boost/heap/priority_queue.hpp>

template <typename T>
class PriorityQueue;

/*
 * NODES
 */

/**
 * class PriorityQueueNode : used by PriorityQueue as the elements it contains. This is useful as it contains the priority of each node.
 */
template <typename T>
class PriorityQueueNode
{
    public:
        PriorityQueueNode(T* content, int priority = 1)
        {
            this->content = content;
            this->priority = priority;
        }

        virtual ~PriorityQueueNode()
        {
            this->content = nullptr; //not deleted because it can still be used somewhere else in the program
        }

        int getPriority() const { return priority; }

        T* getContent() const { return content; }

        bool operator> (const PriorityQueueNode<T> &other)
        {
            return this.priority > other.priority;
        }
        bool operator>= (const PriorityQueueNode<T> &other)
        {
            return this.priority >= other.priority;
        }
        bool operator< (const PriorityQueueNode<T> &other)
        {
            return this.priority < other.priority;
        }
        bool operator<= (const PriorityQueueNode<T> &other)
        {
            return this.priority <= other.priority;
        }

    private:
        T* content;
        int priority;

        void setPriority(int p)
        {
            if(p > 0 && p < priority)
            {
                priority = p;
            }
        }

        void decPriority()
        {
            --priority;
        }

        //friend void PriorityQueue<T>::setPriorityAt(int, int);
        friend class PriorityQueue<T>;
};


/*
 * MINIMUM PRIORITY QUEUE
 */

/**
 * class PriorityQueue : used to implement Dijkstra's pathfinding algorithm. This is useful when building the shortest-path graph
 */
template <typename T>
class PriorityQueue
{
    public:
        PriorityQueue()
        {
            this->elems = new QList<PriorityQueueNode<T>*>();
            this->elems->push_back(nullptr);
            this->taille = 1;
        }
        virtual ~PriorityQueue()
        {
            for(int i=1; i < taille; ++i)
            {
                delete this->elems->at(i++);
            }
            delete this->elems;
        }

        /**
         * @brief tasMin : returns the element with the minimum priority without deleting it from the queue
         * @return the element with the minimum priority
         */
        T* tasMin()
        {
            if(!elems->isEmpty() && elems->size() > 1)
                return this->elems->at(1)->getContent();
            else
                return nullptr;
        }

        /**
         * @brief extraireMin : takes the elements with minimum priority and returns it
         * @return the element with the minimum priority
         */
        T* extraireMin()
        {
            T* temp = nullptr;

            if(!estVide()){
                temp = elems->takeAt(1)->getContent();
                rendreMinimier(1, (--taille)-1);
            }

            return temp;
        }

        /**
         * FOR TESTING ONLY! WILL BE REMOVED LATER AS THIS IS DANGEROUS!
         * @brief tasAt
         * @param index
         * @return
         */
        PriorityQueueNode<T>* at(int index) const
        {
            if(!elems->isEmpty() && index > 0 && index < elems->size())
                return elems->at(index);
            else
                return nullptr;
        }

        bool estVide() const
        {
            return (elems->isEmpty() || elems->size() < 2);
        }

        int getTaille() const { return taille; }

        /**
         * @brief inserer : inserts an element in the queue and sorts it by the priority
         * @param elem : the element to insert
         * @param priority : the element's priority
         * @return index of the element in the queue
         */
        int inserer(T* elem, int p = 1)
        {
            /*
            this->elems->push_back(new PriorityQueueNode<T>(elem, priority));

            //sort the list (one pass on inserted element is enough, since we're only adding one element at a time)
            insertionPass(taille++);
            */

            //check priority validity
            p = (p < 1) ? 1 : p;

            elems->push_back(new PriorityQueueNode<T>(elem, p));
            return minimierPass(taille++);
        }

        int setPriorityAt(int index, int priority)
        {
            //check index validity
            if(!elems->isEmpty() && index > 0 && index < elems->size()){
                elems->at(index)->setPriority(priority);
                index = rendreMinimier(1, index, index);
            }

            return index;
        }

    private:
        QList<PriorityQueueNode<T>*> *elems;
        int taille;

        /**
         * @brief minimierPass
         * @param index : index of the inserted element
         * @return final index of the element
         */
        int minimierPass(int index)
        {
            while(index > 1 && elems->at(index/2)->priority > elems->at(index)->priority)
            {
                elems->swap(index, index/2);
                index /= 2;
            }

            return index;
        }

        /**
         * @brief rendreMinimier : makes the tree a min-heap
         * @param left
         * @param right
         */
        int rendreMinimier(int left, int right, int itemToTrack = 0)
        {
            int index = right;
            while(index > 1)// && elems->at(index/2)->priority > elems->at(index)->priority)
            {
                if(elems->at(index/2)->priority > elems->at(index)->priority)
                {
                    elems->swap(index, index/2);
                    itemToTrack = (index == itemToTrack) ? index/2 : itemToTrack;

                }
                --index;
            }
            return itemToTrack;
        }

        /**
         * @brief insertionPass : a pass of the list sorting
         * @param index : index of the first (rightmost) item to sort
         */
        void insertionPass(int index)
        {
            while(index > 1 && elems->at(index)->getPriority() < elems->at(index-1)->getPriority())
            {
                elems->swap(index, index-1);
                --index;
            }
        }

        /**
         * @brief rendreMinimier : sorts the list, using the insertion sort algorithm
         */
        void rendreMinimier()
        {
            for(int i=2; i<taille; ++i)
            {
                insertionPass(i);
            }
        }
};

#endif // PRIORITYQUEUE_H
