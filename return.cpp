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

#include <QDateTime>
#include "return.h"
#include "ui_return.h"

Return::Return(QWidget *parent ,int initid ,QString initname ,QString initdate ,QByteArray initpwdhash) :
    QDialog(parent),
    ui(new Ui::Return)
{
    //initalizes important data
    id = initid;
    name = initname;
    date = initdate;
    pwdsha = initpwdhash;

    ui->setupUi(this);
}

Return::~Return()
{
    delete ui;
}


void Return::on_buttonBox_accepted()
/* complimentary record map
 *
 */
{
    //checks what radiobutton is checked
    QString condition;
    QString key;
    if (ui->button_single->isChecked()){
        key = QString::number(id);
        condition = "id";
    }
    else if (ui->button_multiple->isChecked()){
        key = date;
        condition = "date";
    }
    else{
        key = name;
        condition = "name";
    }

    QByteArray shahash;
    QString newpass = ui->lineEdit->text();
    if (( shahash = hashsha(newpass)) == pwdsha){
        QStringList * record = new QStringList;

        //sets arguments for updating record


        *record << key;
        *record << shahash;//.toHex();
        *record << hashmd5(newpass).toHex();
        *record << QDateTime::currentDateTime().toString();
        *record << condition;


        mwhandle->handinRecord(record);
    }

}

QByteArray Return::hashsha(QString pwd)
{
    QCryptographicHash hash(QCryptographicHash::Sha3_512);
    hash.addData(pwd.toUtf8());
    return hash.result().toHex();
}

QByteArray Return::hashmd5(QString pwd)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(pwd.toUtf8());
    return hash.result();
}
