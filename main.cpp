//TODO:
//implliment hash check


#include "mainwindow.h"
#include <QSqlDatabase>
#include <QApplication>
#include <QMessageBox>
#include <sqlite3.h>
#include <QDebug>
#include <string>
#include <getopt.h>

int get_filenames(int argc, char *argv[], QString *dbfile, QString *dbbackup){
    int option;
    while ((option = getopt(argc,argv,"f:b:h:")) != -1){
            switch(option){
                case 'f':
                    *dbfile = optarg;
                    qDebug() << *dbfile << ":set as main db";
                break;

                case 'b':
                    *dbbackup= optarg;
                    qDebug() << *dbbackup << ":set as bckup db";
                break;

                case 'h':
                    qDebug() << optarg << "set as hashfile";
                break;

        }
        if (dbfile == NULL){
            qFatal("No main db name provided");
            QMessageBox::information(0,"Error","Fatal:No Filename Provided");
        }
        if (dbbackup == NULL){
            qWarning("No backup name provided");
        }

    }
    return 0;
}

int main(int argc, char *argv[])
{
    //get command line options
    QString dbfile;
    QString dbbackup;

    get_filenames(argc,argv,&dbfile,&dbbackup);


    QApplication a(argc, argv);
    MainWindow w;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");


    db.setDatabaseName("sotresdb.db");
    if (!db.open()){

        qDebug() << "Failed to open Database";
        return 1;
    }
    else
        qDebug() << "sucssfullty opened" << dbfile;


    w.show();
    return a.exec();

}
