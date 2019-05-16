#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <QList>
#include <iostream>

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

        void setPriority(int p)
        {
            if(p > 0)
                priority = p;
        }

        void incPriority()
        {
            ++priority;
        }

        void decPriority()
        {
            --priority;
        }

        int getPriority() const
        {
            return priority;
        }

        T* getContent() const
        {
            return content;
        }

        friend bool operator> (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2);
        friend bool operator>= (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2);

        friend bool operator< (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2);
        friend bool operator<= (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2);


    private:
        T* content;
        int priority;
};

template <typename T>
bool operator> (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2)
{
    return n1.priority > n2.priority;
}
template <typename T>
bool operator>= (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2)
{
    return n1.priority >= n2.priority;
}
template <typename T>
bool operator< (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2)
{
    return n1.priority < n2.priority;
}
template <typename T>
bool operator<= (const PriorityQueueNode<T> &n1, const PriorityQueueNode<T> &n2)
{
    return n1.priority <= n2.priority;
}


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

        T* tasMin()
        {
            if(!elems->isEmpty() && elems->size() > 1)
                return this->elems->at(1);
            else
                return nullptr;
        }

        T* extraireMin()
        {
            T* temp = nullptr;

            if(!estVide()){
                temp = elems->takeAt(1)->getContent();
                --taille;
                rendreMinimier();
            }

            return temp;
        }

        /**
         * FOR TESTING ONLY! WILL BE REMOVED LATER!
         * @brief tasAt
         * @param index
         * @return
         */
        PriorityQueueNode<T>* tasAt(int index)
        {
            if(!elems->isEmpty() && index > 0 && index < elems->size()){
                return elems->value(index);
            }else
                return nullptr;
        }

        void inserer(T* elem, int priority = 1)
        {
            this->elems->push_back(new PriorityQueueNode<T>(elem, priority));

            //sort the list (one pass is enough, since we're adding one element at a time
            insertionPass(taille++);
        }

        bool estVide() const
        {
            return (elems->isEmpty() || elems->size() < 2);
        }

        int getTaille() const
        {
            return this->taille;
        }

    protected:

    private:
        QList<PriorityQueueNode<T>*> *elems;
        int taille;

        /**
         * @brief rendreMinimier : sorting the list. algorithm similar to the insertion sort
         */
        void rendreMinimier()
        {
            for(int i=2; i<taille; ++i){
                insertionPass(i);
            }
        }

        /**
         * @brief insertionPass : a pass of the list sorting
         * @param index : index of the first (rightmost) item to sort
         */
        void insertionPass(int index)
        {
            /*
             * conditions for swapping:
             * index > 1
             * elems[index] < elems[index-1]
             */
            while(index > 1 && elems->value(index)->getPriority() < elems->value(index-1)->getPriority()){
                elems->swap(index, index-1);
                --index;
            }
        }
};

#endif // PRIORITYQUEUE_H
