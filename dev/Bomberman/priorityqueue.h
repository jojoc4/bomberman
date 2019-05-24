#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QList>

template <typename T>
class PriorityQueue; //implementation further down. Needed it here for the Node.

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
        PriorityQueueNode(T* content, PriorityQueueNode<T>* fatherNode = nullptr, int priority = 1)
        {
            this->content = content;
            this->fatherNode = fatherNode;
            this->priority = (priority >= 0) ? priority : (fatherNode != nullptr) ? getFatherPriority() +1 : 0;
        }

        virtual ~PriorityQueueNode()
        {
            this->content = nullptr; //not deleted because it can still be used somewhere else in the program
        }

        int getPriority() const { return priority; }

        T* getContent() const { return content; }

        PriorityQueueNode<T>* getFatherNode() const { return fatherNode; }

        int getFatherPriority() const
        {
            return (fatherNode == nullptr) ? 0 : fatherNode->getPriority();
        }

    private:
        T* content;
        PriorityQueueNode<T>* fatherNode;
        int priority;

        void setPriority(int p)
        {
            if(p > 0 && p < priority)
                priority = p;
            else if(p == 0 && fatherNode == nullptr)
                priority = p;
        }

        void decPriority()
        {
            if(priority > 0)
                --priority;
        }

        void changeFatherNode(PriorityQueueNode<T>* father, int newPriority = 1)
        {
            if(father != nullptr)
            {
                fatherNode = father;
                setPriority(newPriority);
            }
        }

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
         * @brief getMin : returns the element with the minimum priority without deleting it from the queue
         * @return the element with the minimum priority
         */
        PriorityQueueNode<T>* getMin() const
        {
            if(!elems->isEmpty() && elems->size() > 1)
                return this->elems->at(1);
            else
                return nullptr;
        }

        /**
         * @brief takeMin : takes the elements with minimum priority and returns it
         * @return the element with the minimum priority
         */
        PriorityQueueNode<T>* takeMin()
        {
            PriorityQueueNode<T>* temp = nullptr;

            if(!isEmpty()){
                temp = elems->takeAt(1);
                makeMinimier(1, (--taille)-1);
            }

            return temp;
        }

        /**
         * FOR TESTING ONLY! WILL BE REMOVED LATER AS THIS IS DANGEROUS!
         * @brief at
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

        bool isEmpty() const
        {
            return (elems->isEmpty() || elems->size() < 2);
        }

        int getTaille() const { return taille; }

        int searchElem(T* elem)
        {
            for(int i=1; i<taille; ++i)
            {
                if(elems->at(i)->getContent() == elem)
                    return i;
            }

            return 0;
        }

        /**
         * @brief insert : inserts an element in the queue and sorts it by the priority
         * @param elem : the element to insert
         * @param priority : the element's priority
         * @return index of the element in the queue
         */
        int insert(T* elem, PriorityQueueNode<T>* father, int p = 1)
        {
            //check priority validity, set default value if invalid (least priority)
            p = (p < 1) ? INT_MAX : p;

            //only insert element in list if not already there. Update priority otherwise
            int pos = searchElem(elem);
            if(pos == 0)
            {
                elems->push_back(new PriorityQueueNode<T>(elem, father, p));
                taille++;
            }
            else
            {
                elems->at(pos)->setPriority(p);
            }
            return minimierPass(taille-1);
        }

        /*
        int setPriorityAt(int index, int priority)
        {
            //check index validity
            if(!elems->isEmpty() && index > 0 && index < elems->size()){
                elems->at(index)->setPriority(priority);
                index = rendreMinimier(1, index, index);
            }

            return index;
        }
        */

        int updateNodeAt(int index, int newPriority, PriorityQueueNode<T>* newFather)
        {
            //check index validity
            if(!elems->isEmpty() && index > 0 && index < elems->size())
            {
                elems->at(index)->changeFatherNode(newFather, newPriority);
                index = makeMinimier(1, index, index);
            }
            return index;
        }

    private:
        //QList is an array, but we use it as a binary tree for our min-heap.
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
         * @brief makeMinimier : makes the tree a min-heap
         * @param left
         * @param right
         */
        int makeMinimier(int left, int right, int itemToTrack = 0)
        {
            if(right>taille-1)
                right = taille-1;

            int index = right;

            if(left<1)
                left = 1;

            while(index > left)// && elems->at(index/2)->priority > elems->at(index)->priority)
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
};

#endif // PRIORITYQUEUE_H
