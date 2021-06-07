#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "database.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class database;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    database *dbhandle;


    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadDefaultTable();
    void insertRecord(int x,QStringList record);
    void settablegeometry(int x,int y);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
