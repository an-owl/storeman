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
