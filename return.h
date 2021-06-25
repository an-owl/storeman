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

#ifndef RETURN_H
#define RETURN_H

#include <QDialog>
#include <QCryptographicHash>
#include "mainwindow.h"

class MainWindow;
namespace Ui {
class Return;
}

class Return : public QDialog
{
    Q_OBJECT

public:
    explicit Return(QWidget *parent = nullptr, int initid = 0, QString initname = "", QString initdate = "", QByteArray initpwdhash = "");
    ~Return();

    MainWindow *mwhandle;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Return *ui;

    //contains relevant record data
    int id;
    QString name;
    QString date;
    QByteArray pwdsha;
    QByteArray hashsha(QString pwd);
    QByteArray hashmd5(QString pwd);

};

#endif // RETURN_H
