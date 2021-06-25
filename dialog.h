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
#include <QCompleter>

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

    explicit Dialog(QWidget *parent = nullptr, QStringList *autocom = nullptr);
    ~Dialog();
    void setinspect(QStringList record);

private slots:

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
    int id = -1; //stores id of current record used while inspecting

    int pwdbitch();
    int datagood();
    int getitems(int start = 0);//wrapper for saving
    int getitem(int y);//acutally gets item values
    int prepareRecord(int at);
    void save();
    void othersave();


    void setitemtable();
};

#endif // DIALOG_H
