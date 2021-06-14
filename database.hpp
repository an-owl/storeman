#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QSqlDatabase>
#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <getopt.h>
#include <QMessageBox>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"

class MainWindow;
class database{
public:
    char *dbfile;
    char *bakfile; //do not operate on only read for error checking only write on exit
    char *hash; //may become redundant stores hash for dbfiles
    MainWindow *mwhandle; //this thing is fucking stupid it sets where all the return values will go because its easier than setting up callbacks


    database(int argc, char **argv);
   ~database();
    int getall(bool ishidden = false);
    int getdata();
    bool insertdata(QStringList data);
    QStringList getFullRecord(int id);
    void update(int id, QStringList data);


private:
    QSqlDatabase db;
    int gettotal();
};


#endif // DATABASE_HPP
