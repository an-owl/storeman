#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "database.hpp"
#include <qstringlist.h>
#include <QItemSelectionModel>

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
    void insertdata(QStringList* record);
    void updateRecord(int id, QStringList *record);

public slots:
    void write_to_db(QStringList *);

private slots:
    void on_actionAdd_triggered();

    void on_actioninspect_triggered();

private:
    Ui::MainWindow *ui;
    void refresh();
};
#endif // MAINWINDOW_H
