#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    pwd1done = false;
    pwd2done = false;
    ui->setupUi(this);

    //opens and reads ITEMFILE toitems to use in intem col of table
    //should contain all possible options for "item" field in database
    itemfile.setFileName(ITEMFILE);
    itemfile.open(QIODevice::ReadOnly);
    if (itemfile.isOpen()){
        while (true)
        {
            QString line = QString::fromLocal8Bit(itemfile.readLine());
            if (line.isNull())
                break;
            else
                items.append(line);
        }
        qDebug() << "read" <<items << "from file";
    }
    //removes nweline characters and sets up table
    items.replaceInStrings("\n","");
    setitemtable();
}

void Dialog::setitemtable()
//linitalizes itemtable
//col 0 contains itemfile contents
//col 1 initalized with 0 aparenntly it can only contain integers

{
    for (int i = 0; i < items.length() ;i++)
    {
        ui->itemtable->insertRow(i);
        QTableWidgetItem *cell = new QTableWidgetItem;
        cell->setData(0,items.at(i));
        cell->setFlags(cell->flags() & ~Qt::ItemIsEditable);
        ui->itemtable->setItem(i,0,cell);
    }
    for (int i = 0; i < items.length() ;i++)
    {
        QTableWidgetItem *cell = new QTableWidgetItem;
        cell->setData(0,0);
        ui->itemtable->setItem(i,1,cell);
    }
}

Dialog::~Dialog()
{
    delete ui;
}

int Dialog::pwdbitch()
//checks if passwords match
//returns 0 if true
//returns 2 if false
///returns 4 if only one or more password is not entered
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
            return 2;
        }
        else
        {
            ui->pwdout->setText("");
            return 0;
        }
    }
    return 4;
}

int Dialog::datagood()
//checks if form is completed returns 0 for true status is flag
//remember to write down what flags mean
{
    int status = 0;

        if (ui->lineEdit_name->text() == "")
            status = (status | 1);

        status = (pwdbitch() | status);

        //get number of items bust be greater than 1
    return status;
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
void Dialog::on_dialogButtonBox_clicked(QAbstractButton *button)
//called when button is pressed
{
    if ((QPushButton *)button== ui->dialogButtonBox->button(QDialogButtonBox::Discard))
    //closes window and disgards data
    {
        qDebug() << "disgardgin window"; //that was my 3rd try i dont care enough
        close();
    }

    if ((QPushButton *)button== ui->dialogButtonBox->button(QDialogButtonBox::Save)){
        qDebug() << "saving data";

    }
}

