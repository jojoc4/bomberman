#include "mainwindow.h"
#include <QApplication>
#include "map.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Map m = Map();
    try{
        m.readFromFile("map1.nmm");
    }catch (char* c){
       qDebug() << c;
    }

    return a.exec();
}
