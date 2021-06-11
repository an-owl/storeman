#include "dialog.h"
#include "ui_dialog.h"
/*TODO
 * setup autocomplete for names using completer
*/

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
        unsigned int zero = 0;
        QTableWidgetItem *cell = new QTableWidgetItem;
        cell->setData(0,zero);
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

        if (getitems() == -1)
            status = (status | 8);

    return status;
}


int Dialog::getitem(int y)
{
    int qty;
    //QVariant qty;
    QTableWidgetItem *cell;
    cell = ui->itemtable->item(y,1);
    qty = cell->data(0).toInt();
    qDebug() << "at" << y << "got" << qty;
    return qty;

}


int Dialog::getitems(int start)
//returns number of items set
//i changed this so many times i will ned to come back and clean it up
{
    for (int i = start; i < items.length(); i++)
    {
        if (getitem(i) != 0)
            return i;
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


int Dialog::prepareRecord(int at = 0)
//process and sorts data to be saved
{
    /*stores record data in standard fromat
     * [0] name
     * [1] passwordhash
     * [2] item
     * [3] qty
     * [4] condition
     * [5] comments
     */

    //add name
    record[0] = (ui->lineEdit_name->text().toUtf8());

    //hash and add password
    QCryptographicHash pwdhash(QCryptographicHash::Sha3_512);
    QByteArray pwd = ui->lineEdit_pass->text().toUtf8();
    pwdhash.addData(pwd);
    record[1] = pwdhash.result();

    int doagain = getitems(at);
    //gets item and qty sets return 0 if done
    record[2] = items[getitems(at)].toUtf8();
    //this is a bit fucking wierd
    //(int at) is set to the last item that was read
    //getitems(at) starts the search from the last item that was read
    //this returns the value of the next thing to be read
    //so you call this again with its own return value to get the next thing
    //if return == items.length() you have reached the end
    record[3].clear();
    //record[3].append(sizeof(getitem(getitems(at))),getitems(at));
    record[3].append(getitem(getitems(at)));

    record[4] = QDateTime::currentDateTime().toString().toUtf8();

    record[5] = "$USER";

    record[6] = ui->box_condition->toPlainText().toUtf8();
    record[7] = ui->box_comments->toPlainText().toUtf8();

    qDebug() << record;

    return ++doagain;
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
        if (datagood() == 0){
            qDebug() << "saving data";

            record.resize(8);
            int current = 0;
            int last = 0;
            //while ((current = prepareRecord(last)) != last){

            while (current != items.length()){
                current=prepareRecord(last);
                //save record()
                record.clear();

                record.resize(8);
                last = current;
            }
        }
        else
            qWarning("Data bad");
    }
}

