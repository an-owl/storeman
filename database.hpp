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


class database{
public:
    char *dbfile;
    char *bakfile; //do not operate on only read for error checking only write on exit
    char *hash; //may become redundant stores hash for dbfiles
    QSqlDatabase db;

    database(int argc, char **argv);
   ~database();
};

#endif // DATABASE_HPP
