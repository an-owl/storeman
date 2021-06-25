#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
//#include <QVector>
#include "database.hpp"
#include "querysearch.h"
#include <qstringlist.h>
#include <QHeaderView>
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
    void handinRecord(QStringList *record);

public slots:
    void write_to_db(QStringList *);

private slots:
    void on_actionAdd_triggered();

    void on_actioninspect_triggered();

    void on_actionReturn_triggered();

    void on_actionrefresh_triggered();

    void on_MainWindow_destroyed();

    void on_dbtable_cellClicked(int row, int column);

    void on_actionshow_all_triggered(bool checked);

    void on_actionhide_triggered(bool checked);

    void on_searchButton_clicked();

    void on_actionAbout_triggered();

    void on_actionAcutally_Help_triggered();

private:
    Ui::MainWindow *ui;
    void refresh();
    int getid();
    QVector<bool> hidden;
    bool showhidden = false;

};
#endif // MAINWINDOW_H
