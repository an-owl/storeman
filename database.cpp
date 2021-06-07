/*TODO
 *error check database files
 *copy dbfile to bakfile
 *hash files and save hashes
*/



#include "stddef.hpp"
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
        if (initdb.exec() == false)
            qCritical("failed to initalize database");


    }
}

int database::getall(){
    //fills mian table with all enteries (default state)
    QSqlQuery query("SELECT id,name,item,qty,date,authorised,returned FROM " DEFAULTTABLE " WHERE hidden = FALSE;");
    qDebug() << "getall error:" << query.lastError();

    int y = 0;
    while(query.next())
    //iterates through query rows sends whole record to mainwindow
    {
        QStringList record;
        for (int x = 0;x < DEFCOLS; x++){
            record << query.value(x).toString();
        }
        qDebug() << "sending" << record;
        mwhandle->insertRecord(y,record);
        y++;
    }

    return 0;
}


database::~database(){
    db.close();
}
