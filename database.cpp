/*TODO
 *error check database files
 *copy dbfile to bakfile
 *hash files and save hashes
 *
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
        initdb.prepare("CREATE TABlE " DEFAULTTABLE " (id int,name char,pwdhashsha512 blob,item char,qty int,date char,authorised char,returned char,pwdhashmd5,hidden bool,condition char,comments char);");
        if (initdb.exec() == false)
            qCritical("failed to initalize database");


    }
}

int database::getall(bool ishidden)
{
    //fills mian table with all enteries (default state)
    QSqlQuery query;
    //if ishidden is true returns hidden TRUE && FALSE otherwise ust returns where hidden is FASLE
    if (ishidden == false)
        query.prepare("SELECT id,name,item,qty,date,authorised,returned FROM " DEFAULTTABLE " WHERE hidden = FALSE;");

    else
        query.prepare("SELECT id,name,item,qty,date,authorised,returned FROM " DEFAULTTABLE ";");

    if (!query.exec())
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




    query.prepare("INSERT INTO " DEFAULTTABLE
                  " (id,name,pwdhashsha512,item,qty,date,authorised,condition,comments,hidden)"
                  "VALUES "
                  "(:id,:name,:passwdhash512,:item,:qty,:date,:authorised,:condition,:comments,FALSE);");

    query.bindValue(":id",gettotal());
    query.bindValue(":name",data.at(0));
    query.bindValue(":passwdhash512",data.at(1));
    query.bindValue(":item",data.at(2));
    query.bindValue(":qty",data.at(3));
    query.bindValue(":date",data.at(4));
    query.bindValue(":authorised",data.at(5));
    query.bindValue(":condition",data.at(6));
    query.bindValue(":comments",data.at(7));


    //bindvalues prevents sql injection
    //add comments and condition

    if (query.exec())
    {
        qDebug() << "saved successfully";
        return true;
    }
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
    //can be used to get next id number
    ///returns number of records returns -1 and bitches on error
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT COUNT(id) FROM " DEFAULTTABLE";");
    query.exec();

    if (query.first()){
        return (query.value(0).toInt());
    }
    else
    {
        qWarning("Error getting number of records database returned");
        qWarning() << query.lastError();
        return -1;
    }
}

QStringList database::getFullRecord(int id)
/*takes id as argument returns full record as qstringlist
 *[0] id
 *[1] name
 *[2] pwdhashsha512
 *[3] item
 *[4] qty
 *[5] date
 *[6] authorised
 *[7] returned
 *[8] pwdhashmd5
 *[9] hidden
 *[10] condition
 *[11] comments
 */
{
#define FIELDS 12
    QStringList result;
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("SELECT "
                  "id, name, pwdhashsha512, item, qty, date, authorised, returned, pwdhashmd5, hidden, condition, comments"
                  //update fields if you touch this
                  " FROM " DEFAULTTABLE
                  " WHERE id = :id");
    query.bindValue(":id",id);
    //error checks query
    //if good then do stuff
    //else gib error
    if (query.exec()){
        query.first();
        for (int i = 0; i < FIELDS;i++)
        {
            result << query.value(i).toString();
        }
        qDebug() << "get record good";
    }
    else{
        qWarning("get record bad");
        qWarning() << query.lastError();
    }
    qDebug() << result;
    return result;
#undef FIELDS
}

database::~database(){
    db.close();
}

void database::update(int id, QStringList data)
{
    QSqlQuery query;
    query.prepare("UPDATE " DEFAULTTABLE
                  " SET condition = :condition, comments = :comments"
                  " WHERE id = :id;");
    query.bindValue(":condition",data.at(1));
    query.bindValue(":comments",data.at(0));
    query.bindValue(":id",id);
    if (query.exec()){
        qDebug() << "update successful";
    }
    else{
        qWarning() << "update failed";
        qWarning() << query.lastError();
    }

}
