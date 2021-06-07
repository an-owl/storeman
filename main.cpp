//TODO:
//implliment hash check


#include "mainwindow.h"
#include "database.hpp"
#include <QSqlDatabase>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


int main(int argc, char *argv[])
{
    //creates window class i think, dont touch
    QApplication a(argc, argv);
    MainWindow w;

    database db(argc,argv);
    w.dbhandle = &db;
    db.mwhandle = &w;
    w.loadDefaultTable();


    //renders window
    w.show();
    return a.exec();

}
