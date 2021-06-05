//TODO:
//implliment hash check


#include "mainwindow.h"
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


    //renders window
    w.show();
    return a.exec();

}
