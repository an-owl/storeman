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

    QStringList getAllNames();
    database(int argc, char **argv);
   ~database();
    int getall(bool ishidden);
    int getdata();
    bool insertdata(QStringList data);
    QStringList getFullRecord(int id);
    void update(int id, QStringList data);
    void returnRecord(QStringList *record);
    void setHidden(int id, bool hidden);

    QString manQuery(QString querytxt);

private:
    QSqlDatabase db;
    int gettotal();
};


#endif // DATABASE_HPP
