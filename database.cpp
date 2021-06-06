/*TODO
 *error check database files
 *copy dbfile to bakfile
 *hash files and save hashes
*/

#define DEFAULTTABLE "storestable"

#include "database.hpp"



database::database(int argc, char **argv){

    //retrieves database file names
    int option;
    while ((option = getopt(argc,argv,"f:b:h:")) != -1){
        switch (option){
            case 'f':
                dbfile = new char[strlen(optarg)];
                dbfile = optarg;
                qDebug() << dbfile << "set as main db";
            break;
            case 'b':
                bakfile = new char[strlen(optarg)];
                bakfile = optarg;
                qDebug() << bakfile << "set as main db";
            break;
            case 'h':
                hash = new char[strlen(optarg)];
                hash = optarg;
                qDebug() << hash << "set as main db";
        }

    }


    //just some debugging help
    if (dbfile == NULL){
        qFatal("No main db name provided");
        QMessageBox::information(0,"Error","Fatal:No Filename Provided");
    }
    if (bakfile == NULL){
        qWarning("No backup name provided");
    }


    //opens database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbfile);
    if (!db.open()){
        qCritical("Failed to open Database");
    }
    else
        qDebug() << "sucssfully opened" << dbfile;

    QStringList tables = db.tables();

    qDebug() << "Current table list" << tables;
    if (tables.indexOf(DEFAULTTABLE) == -1){
        QSqlQuery initdb;
        initdb.prepare("CREATE TABlE " DEFAULTTABLE " (id int,name char,pwdhashsha512 blob,item char,qty int,date int,authorised char,returned int,pwdhashmd5,hidden bool,condition char,comments char);");
        if (initdb.exec() == false){
            qCritical("failed to initalize database");
        }

    }
}



database::~database(){
    db.close();
}
