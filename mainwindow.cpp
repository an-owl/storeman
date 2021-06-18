
#include "stddef.hpp"

#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "return.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->dbtable->setColumnCount(record.size());
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
    dbhandle->insertdata(*record);
}

void MainWindow::settablegeometry(int x, int y)
{
    ui->dbtable->setRowCount(y);
    ui->dbtable->setColumnCount(x);
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


