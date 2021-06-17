#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QSqlDatabase>
#include <QDebug>
#include <QString>
#include <getopt.h>
#include <QMessageBox>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include "mainwindow.h"

class MainWindow;
class database{
public:
    char *dbfile = nullptr;
    char *bakfile = nullptr; //do not operate on only read for error checking only write on exit
    char *hash = nullptr; //may become redundant stores hash for dbfiles
    MainWindow *mwhandle; //this thing is fucking stupid it sets where all the return values will go because its easier than setting up callbacks


    database(int argc, char **argv);
   ~database();
    int getall(bool ishidden);
    int getdata();
    bool insertdata(QStringList data);
    QStringList getFullRecord(int id);
    void update(int id, QStringList data);
    void returnRecord(QStringList *record);

private:
    QSqlDatabase db;
    int gettotal();
};


#endif // DATABASE_HPP
