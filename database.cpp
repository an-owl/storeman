/*

    soremant is designed to replace oper managemnt of stores in the NZCF
    Copyright (C) 20201  Charles Kern-Smith

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    if (dbfile == nullptr){
        qFatal("No main db name provided");
        QMessageBox::information(0,"Error","Fatal:No Filename Provided");
    }
    if (bakfile == nullptr){
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
        query.prepare("SELECT id,name,item,qty,date,authorised,returned,hidden FROM " DEFAULTTABLE " WHERE hidden = FALSE;");

    else
        query.prepare("SELECT id,name,item,qty,date,authorised,returned,hidden FROM " DEFAULTTABLE ";");

    if (!query.exec())
        qDebug() << "getall error:" << query.lastError();

    int y = 0;
    while(query.next())
    //iterates through query rows sends whole record to mainwindow
    {
        QStringList record;
        for (int x = 0;x < DEFCOLS+1; x++){ //+1 adds extra loop for hidden state
            record << query.value(x).toString();
        }
        qDebug() << "sending" << record;
        mwhandle->insertRecord(y,record);
        y++;
    }
    return 0;
}

QStringList database::getAllNames()
//gets all names in database where hidden = false and returns without duplicates
//only false bcause you dont want names that wont be used showing up in results
{
    QStringList names;
    QSqlQuery query;
    query.prepare("SELECT name FROM " DEFAULTTABLE
                  " WHERE hidden = FALSE;");
    if (query.exec()){
        while (query.next()){
            names << query.value(0).toString();//im a retard
        }
        names.removeDuplicates();
        qDebug() << "got names" << names;
        return names;

    }
    else{
        qDebug() << "query failed" << query.lastError();
        return names;
    }
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

database::~database()
{
    db.close();
    if (bakfile != nullptr){
        QFile bak(database::bakfile);
        bak.remove();
        QFile dbmainfile(database::dbfile);
        dbmainfile.copy(database::bakfile);
    }
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

void database::setHidden(int id, bool hidden)
//changes the database state of hidden to hidden
{
    QSqlQuery query;
    query.prepare("UPDATE " DEFAULTTABLE
                  " SET hidden = :hidden"
                  " WHERE id = :id;");
    query.bindValue(":hidden",hidden);
    query.bindValue(":id", id);
    if (query.exec())
        qDebug() << "record" << id << "changed to" << hidden;
    else
        qWarning() << "record not changed" << query.lastError();
}

void database::returnRecord(QStringList *record)
{
    //stores query in string because i couldnt get bindvalue to work on the left side of the condition
    QString querystring = ("UPDATE " DEFAULTTABLE
                           " SET pwdhashmd5 = :pwdmd5, returned = :returned "
                           "WHERE  = :value AND pwdhashsha512 = :pwdsha;");
    querystring.insert(72,record->at(4));
    //if qerystring is changed AT ALL make sure above line is correct


    QSqlQuery query;
    query.prepare(querystring);

    query.bindValue(":pwdsha",record->at(1));
    query.bindValue(":pwdmd5",record->at(2));
    query.bindValue(":returned",record->at(3));

    //if its going to cock up it will be one of these VV
    //query.bindValue(":condition",record->at(4));
    query.bindValue(":value",record->at(0));
    if (query.exec())
        qDebug() << "recoed closed" << query.numRowsAffected();
    else
        qWarning() << "record not closed" << query.lastError();
}

QString database::manQuery(QString querytxt)
{
    QSqlQuery query;
    query.prepare(querytxt);
    if (!query.exec()){
        return query.lastError().text();
    }

    int y = 0;
    while(query.next())
    //iterates through query rows sends whole record to mainwindow
    {
        QStringList record;
        for (int x = 0;x < DEFCOLS+1; x++){ //+1 adds extra loop for hidden state
            record << query.value(x).toString();
        }
        qDebug() << "sending" << record;
        mwhandle->insertRecord(y,record);
        y++;
    }
    return 0;

}
