#include <QApplication>
#include "priorityqueue.h"
#include "mapbloc.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void afficher(PriorityQueue<int> &fp)
{
    int taille = fp.getTaille();
    for(int i=1; i < taille; ++i)
    {
        std::cout << *(fp.at(i)->getContent()) << " (" << fp.at(i)->getPriority() << ") - " ;
    }

    std::cout << std::endl;
}

void testInt(){
    PriorityQueue<int> fp;

    int nb1 = 10;
    int nb2 = 42;
    int nb3 = 9;
    int nb4 = 6;
    int nb5 = 8;
    int nb6 = 16;
    int nb7 = 12;
    int nb8 = 99;
    int nb9 = 51;

    fp.inserer(&nb1, nb1);
    afficher(fp);
    fp.inserer(&nb2, nb2);
    afficher(fp);
    fp.inserer(&nb3, nb3);
    afficher(fp);
    fp.inserer(&nb4, nb4);
    afficher(fp);
    fp.inserer(&nb5, nb5);
    afficher(fp);
    fp.inserer(&nb6, nb6);
    afficher(fp);
    fp.inserer(&nb7, nb7);
    afficher(fp);
    fp.inserer(&nb8, nb8);

    afficher(fp);

    cout << endl << "Mini : " << *fp.extraireMin() << endl;

    afficher(fp);

    fp.inserer(&nb9, nb9);

    afficher(fp);
}

void afficher(PriorityQueue<MapBloc> &fp)
{
    int taille = fp.getTaille();
    for(int i=1; i < taille; ++i)
    {
        //MapBloc
        std::cout << fp.at(i)->getContent()->getNom() << " (" << fp.at(i)->getPriority() << ") - " ;
    }

    std::cout << std::endl;
}

void testMapBloc(){
    PriorityQueue<MapBloc> fp;

    MapBloc *b1 = new MapBloc("bloc 1");
    MapBloc *b2 = new MapBloc("bloc 2");
    MapBloc *b3 = new MapBloc("bloc 3");
    MapBloc *b4 = new MapBloc("bloc 4");
    MapBloc *b5 = new MapBloc("bloc 5");
    MapBloc *b6 = new MapBloc("bloc 6");
    MapBloc *b7 = new MapBloc("bloc 7");

    fp.inserer(b1, 1);
    afficher(fp);
    fp.inserer(b2, 2);
    afficher(fp);
    fp.inserer(b3, 3);
    afficher(fp);
    fp.inserer(b4, 4);
    afficher(fp);
    fp.inserer(b5, 3);
    afficher(fp);
    fp.inserer(b6, 1);
    afficher(fp);

    fp.setPriorityAt(5, 1);

    afficher(fp);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    cout << endl << "Mini : " << fp.extraireMin()->getNom() << endl;
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    afficher(fp);

    fp.inserer(b7, 2);

    afficher(fp);

    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete b5;
    delete b6;
    delete b7;

    auto duration = duration_cast<nanoseconds>( t2 - t1 ).count();
    cout << endl << "temps : " << duration << endl;
}


int main(int argc, char *argv[])
{
    //testInt();
    testMapBloc();
    return 0;
}
