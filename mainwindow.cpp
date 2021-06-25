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

#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "return.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeMode::Fixed);
    ui->dbtable->setColumnWidth(0,60);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeMode::Stretch);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeMode::ResizeToContents);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeMode::ResizeToContents);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeMode::ResizeToContents);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeMode::ResizeToContents);
    ui->dbtable->horizontalHeader()->setSectionResizeMode(6,QHeaderView::ResizeMode::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadDefaultTable()
//setup default table
{
    //dbhandle->mwhandle = this;
    dbhandle->getall(false);

}
void MainWindow::insertRecord(int y,QStringList record)
//this seems like a memory leak waiting to happen
//probably not going to work anyway
{
    qDebug() << "inserting into" << y;
    ui->dbtable->insertRow(y);
    ui->dbtable->setColumnCount(record.size()-1);
    for(int i = 0;i < record.size()-1;i++)//exits loop before reading state of hidden
        //creates pointer to cell sets data and hands it to dbtable (and deletes it(leaks it))
    {
        QTableWidgetItem * cell = new QTableWidgetItem;
        cell->setData(0,record.at(i));
        ui->dbtable->setItem(y,i,cell);
    }
    if (record.at((record.size()-1)) == "0") //checks against last item in record. -1 because of index starting at 0
        MainWindow::hidden.append(false);
    else
        MainWindow::hidden.append(true);

}

void MainWindow::write_to_db(QStringList *record)
{
    dbhandle->insertdata(*record); //if there is a crash on save its this idk why it happens. im afraid of it. (Demons?)
}

void MainWindow::settablegeometry(int x, int y)
{
    ui->dbtable->setRowCount(y);
    ui->dbtable->setColumnCount(x-1);
}


void MainWindow::on_actionAdd_triggered()
{
    QStringList com = dbhandle->getAllNames();
    Dialog entrywin(this,&com);
    entrywin.setModal(true);
    entrywin.exec();
    //refresh();
    refresh();
}

int MainWindow::getid()
//gets id of selected row
{
    QItemSelectionModel *select =ui->dbtable->selectionModel();
    if (select->hasSelection()){
        int row = select->selectedRows(0)[0].row();//if i fits i sits. gets selected row as int
        int id = ui->dbtable->item(row,0)->data(0).toUInt(); //gets id from (row id column)
        return id;
    }
    else{
        return -1;
    }
}

void MainWindow::on_actioninspect_triggered()
{
    int id;
    if ((id = getid()) != -1){
        Dialog inspectwin;
        inspectwin.setModal(true);
        inspectwin.setinspect(dbhandle->getFullRecord(id));
        inspectwin.exec();
    }
    else
        ui->statusbar->showMessage("no row selected");

}

void MainWindow::refresh()
//updates mainwindow to reflect current program state
{
    //add option to configure hidden state here
    //idk why without clearContents the nwe rows will not be inserted
    hidden.clear();
    ui->dbtable->clearContents();
    ui->dbtable->setRowCount(0);
    dbhandle->getall(showhidden);
}

void MainWindow::updateRecord(int id, QStringList *record)
//updates comments and condition of record id
{
    dbhandle->update(id, *record);
}

void MainWindow::on_actionReturn_triggered()
{
        int id;
        if ((id = getid()) != -1 ){
            QStringList record = dbhandle->getFullRecord(id);
            Return returnwin(this,record.at(0).toUInt(),record.at(1),record.at(5),record.at(2).toUtf8());
            returnwin.setModal(true);
            returnwin.exec();
            refresh();
        }
        else
            ui->statusbar->showMessage("no row selected");
}

void MainWindow::handinRecord(QStringList *record)
{
    dbhandle->returnRecord(record);
}


void MainWindow::on_actionrefresh_triggered()
{
    refresh();
}


void MainWindow::on_MainWindow_destroyed()
{
    dbhandle->~database();
}

void MainWindow::on_dbtable_cellClicked(int row, int column)
{
    if (hidden[row] == true){
        ui->actionhide->setChecked(true);
    }
    else
        ui->actionhide->setChecked(false);
}


void MainWindow::on_actionshow_all_triggered(bool checked)
{
    showhidden = checked;
    refresh();
}


void MainWindow::on_actionhide_triggered(bool checked)
{
        dbhandle->setHidden(getid(),checked);
        refresh();
        if (showhidden == false)
            ui->actionhide->setChecked(false);
}



void MainWindow::on_searchButton_clicked()
{
    QuerySearch search(ui->search_bar->text(),DEFAULTTABLE);
    search.exec();
    ui->dbtable->clearContents();
    ui->dbtable->setRowCount(0);
    dbhandle->manQuery(*search.query);
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About","<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
    "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
    "p, li { white-space: pre-wrap; }\n"
    "</style></head><body style=\" font-family:'Noto Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
    "<h1 style=\" margin-top:18px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:xx-large; font-weight:600;\">About</span></h1>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This is a program designed for the NZCF to manage stores inventory digitally, to make managment easier although it is not restricted to that purpose.</p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; m"
                            "argin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">More information can be found at <a href=\"https://github.com/an-owl/storeman\"><span style=\" text-decoration: underline; color:#007af4;\">github</span></a></p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; text-decoration: underline; color:#007af4;\"><br /></p>\n"
    "<h1 style=\" margin-top:18px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:xx-large; font-weight:600;\">basic usage </span></h1>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin"
                            "-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">-f &quot;path to database&quot; is requireed to use this program if one is not located there one will be created</p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
    "<h1 style=\" margin-top:18px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:xx-large; font-weight:600;\">Liscences</span></h1>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Copyright (c) 2021 Charles Kern-Smith</p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"
                            "\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans','sans-serif';\">This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.</span></p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Noto Sans','sans-serif';\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans','sans-serif';\">This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General "
                            "Public License for more details.</span></p>\n"
    "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Noto Sans','sans-serif';\"><br /></p>\n"
    "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Noto Sans','sans-serif';\">You should have received a copy of the GNU General Public License along with this program.  If not, see &lt;</span><a href=\"https://www.gnu.org/licenses/\"><span style=\" text-decoration: underline; color:#007af4;\">https://www.gnu.org/licenses/</span></a><span style=\" font-family:'Noto Sans','sans-serif';\">&gt;.</span></p></body></html>");
}


void MainWindow::on_actionAcutally_Help_triggered()
{
    QMessageBox::aboutQt(this,"About Qt");
}

