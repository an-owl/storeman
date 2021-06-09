
#include "stddef.hpp"

#include "dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    dbhandle->getall();
}
void MainWindow::insertRecord(int y,QStringList record)
//this seems like a memory leak waiting to happen
//probably not going to work anyway
{
    qDebug() << "inserting into" << y;
    ui->dbtable->insertRow(y);
    ui->dbtable->setColumnCount(record.size());
    for(int i = 0;i < record.size();i++)
        //creates pointer to cell sets data and hands it to dbtable (and deletes it(leaks it))
    {
        QTableWidgetItem * cell = new QTableWidgetItem;
        cell->setData(0,record.at(i));
        ui->dbtable->setItem(y,i,cell);
    }
}

void MainWindow::settablegeometry(int x, int y)
{
    ui->dbtable->setRowCount(y);
    ui->dbtable->setColumnCount(x);
}


void MainWindow::on_actionAdd_triggered()
{

    Dialog entrywin(this);
    entrywin.setModal(true);
    entrywin.exec();
}


void MainWindow::insertdata(QStringList *record)
{
    dbhandle->insertdata(*record);
}

