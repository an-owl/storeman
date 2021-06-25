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




#include "mainwindow.h"
#include "database.hpp"
#include <QSqlDatabase>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>


int main(int argc, char *argv[])
{
    //creates window object i think, dont touch
    QApplication a(argc, argv);
    MainWindow w;

    database db(argc,argv);
    w.dbhandle = &db;
    db.mwhandle = &w;
    w.loadDefaultTable();


    //renders window
    w.show();
    return a.exec();

}
