#ifndef DIALOG_H
#define DIALOG_H

#include <QStringList>
#include <QString>
#include <QCryptographicHash>
#include <QDialog>
#include <cstdint>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QVector>
#include <QDateTime>

#include "mainwindow.h"
#include "stddef.hpp"


class MainWindow;
QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    MainWindow *mwhandle;

    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setinspect(QStringList record);

private slots:
    /*void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_rejected();*/



    void on_lineEdit_pass_editingFinished();

    void on_lineEdit_repeat_editingFinished();

    void on_dialogButtonBox_clicked(QAbstractButton *button);

private:
    Ui::Dialog *ui;
    uint64_t pwdhash;
    bool pwd1done, pwd2done; //stores if passwords have been typed on if both true its allowed to complain
    QFile itemfile;
    QStringList items;
    QStringList *record;
    int id; //stores id of current record used while inspecting

    int pwdbitch();
    int datagood();
    int getitems(int start = 0);//wrapper for saving
    int getitem(int y);//acutally gets item values
    int prepareRecord(int at);


    void setitemtable();
};

#endif // DIALOG_H
