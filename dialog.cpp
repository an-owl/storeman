#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    pwd1done = false;
    pwd2done = false;
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::pwdbitch()
{
    if (Dialog::pwd1done && Dialog::pwd2done){
        if(!(ui->lineEdit_pass->text() == ui->lineEdit_repeat->text()))
        /*{
            //ui->lineEdit_pass->text();
            QCryptographicHash pwdhash(QCryptographicHash::Sha3_512);
            QByteArray pwd = ui->lineEdit_pass->text().toLatin1();
            pwdhash.addData(pwd);
            ui->pwdout->setText(pwdhash.result().toHex());
            return 0;
        }
        else*/
        {
            ui->pwdout->setText("Passwords dont match");
            return 1;
        }
    }
    return -1;

}


void Dialog::on_lineEdit_pass_editingFinished()
{
    Dialog::pwd1done = true;
    pwdbitch();
}


void Dialog::on_lineEdit_repeat_editingFinished()
{
    Dialog::pwd2done = true;
    pwdbitch();
}

