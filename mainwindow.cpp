//#include <QTableWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

QString filename;
MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->dbtable->setRowCount(5);
    //ui->dbtable->setColumnCount(5);

}

MainWindow::~MainWindow()
{
    delete ui;
}

