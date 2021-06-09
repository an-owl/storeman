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

bool database::insertdata(QStringList data)
//inserts a single record in the database
{
    //converts incoming data to usable query
    QSqlQuery query;




    query.prepare("INSERT INTO" DEFAULTTABLE
                  "(id,name,item,qty,date,authorised,returned,hidden)"
                  "VALUES "
                  "(:id,:name,:passwdhash512,:item,:qty,:date,:authorised,hidden=FALSE);");
    query.bindValue(":name",data.at(0));
    query.bindValue(":passwdhash512",data.at(1));
    query.bindValue(":item",data.at(2));
    query.bindValue(":qty",data.at(3));
    query.bindValue(":date",data.at(4));
    query.bindValue(":authorised",data.at(5));
    query.bindValue(":id",gettotal());
    //bindvalues prevents sql injection
    //add comments and condition

    if (query.exec())
        return true;
    else
    {
        qWarning("Insert failed query returned");
        qWarning() << query.lastError();
        return false;
    }
}

int database::gettotal()
{
    //gets number of records in table
    ///returns number of records returns -1 and bitches on error
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT COUNT(id) FROM" DEFAULTTABLE);


    if (query.next()){
        return (query.value(0).toInt());
    }
    else
        qWarning("Error getting number of records database returned");
        qWarning() << query.lastError();
        return -1;
}

database::~database(){
    db.close();
}
