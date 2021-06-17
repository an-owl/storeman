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
    return items.length();

}


void Dialog::setinspect(QStringList record)
//sets up dialog window for inspecting record
//a bit hackey make better in future
{
    /*
    *record.at() refrence list
    *[0] id
    *[1] name
    *[2] pwdhashsha512
    *[3] item
    *[4] qty
    *[5] date
    *[6] authorised
    *[7] returned
    *[8] pwdhashmd5
    *[9] hidden
    *[10] condition
    *[11] comments
    */

    //It's a Surprise Tool That Will Help Us Later
    id = record.at(0).toUInt();

    //sets text in editboxes
    ui->lineEdit_name->setText(record.at(1));
    ui->lineEdit_pass->setText(record.at(5));
    ui->lineEdit_repeat->setText(record.at(7));
    ui->box_condition->setPlainText(record.at(10));
    ui->box_comments->setPlainText(record.at(11));

    //sets line edits to read only to display data and not change it
    ui->lineEdit_name->setReadOnly(true);
    ui->lineEdit_pass->setReadOnly(true);
    ui->lineEdit_repeat->setReadOnly(true);
    ui->lineEdit_pass  ->setEchoMode(QLineEdit::Normal);
    ui->lineEdit_repeat->setEchoMode(QLineEdit::Normal);

    //changes labels to reflect their contents
    ui->label_pass->setText("Date");
    ui->label_repeat->setText("Returned");

    //set table data keys
    //this may not be the best way to do this
    QTableWidgetItem *auth = new QTableWidgetItem("Authorised",0);
    QTableWidgetItem *item = new QTableWidgetItem("Item",0);
    QTableWidgetItem *qty  = new QTableWidgetItem("Qty",0);
    //set table data
    QTableWidgetItem *authval = new QTableWidgetItem(record.at(6),0);
    QTableWidgetItem *itemval = new QTableWidgetItem(record.at(3),0);
    QTableWidgetItem *qtyval  = new QTableWidgetItem(record.at(4),0);

    //clears tableand sets new contents
    ui->itemtable->clear();
    ui->itemtable->horizontalHeader()->hide();
    ui->itemtable->setRowCount(3);
    ui->itemtable->setItem(0,0,auth);
    ui->itemtable->setItem(0,1,authval);
    ui->itemtable->setItem(1,0,item);
    ui->itemtable->setItem(1,1,itemval);
    ui->itemtable->setItem(2,0,qty);
    ui->itemtable->setItem(2,1,qtyval);

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

void Dialog::save()
{
    int max_pass = items.size();
    QStringList *record = new QStringList;

    *record << ui->lineEdit_name->text();

    QCryptographicHash pwdhash(QCryptographicHash::Sha3_512);
    QByteArray pwd = ui->lineEdit_pass->text().toUtf8();
    pwdhash.addData(pwd);
    *record << pwdhash.result().toHex();

    //these are here because i dont want to change the stuff in database.cpp
    //*record << items[getitems(at)];
    //*record << QString::number(getitem(getitems(at)));
    *record << "";
    *record << "";

    *record << QDateTime::currentDateTime().toString();
    *record << "$USER";
    *record << ui->box_condition->toPlainText();
    *record << ui->box_comments->toPlainText();

    for (int i = 0;i < max_pass; i++ ) {
        int qty;
        if ((qty = getitem(i)) != 0){
            record->replace(3,QString::number(qty));
            record->replace(2,items.at(i));
            mwhandle->write_to_db(record);

        }
    }
}

/*int Dialog::prepareRecord(int at)
{
    *stores record data in standard fromat
     * [0] id leave balnk here
     * [1] name
     * [2] passwordhash
     * [3] item
     * [4] qty
     * [5] time
     * [6] authorised
     * [7] condition
     * [8] comments
     * [8] hidden leave balnk
     *\/
    if ((getitems(at)) == items.size())
        return getitems(at);

    //bundles up entire record into qstringlist
    record = new QStringList;
    *record << ui->lineEdit_name->text();

    //hash and add password
    QCryptographicHash pwdhash(QCryptographicHash::Sha3_512);
    QByteArray pwd = ui->lineEdit_pass->text().toUtf8();
    pwdhash.addData(pwd);
    *record << pwdhash.result().toHex();


    *record << items[getitems(at)];
    *record << QString::number(getitem(getitems(at)));
    *record << QDateTime::currentDateTime().toString();
    *record << "$USER";
    *record << ui->box_condition->toPlainText();
    *record << ui->box_comments->toPlainText();

    qDebug() << *record;
    return getitems(at)+1;
}*/

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
        if (id != -1){
            othersave();
            close();
            return;
        }
        if (datagood() == 0){
            qDebug() << "saving data";

            save();

            /*int current = 0;
            int last = 0;

            while (current != items.length()){
                current=prepareRecord(last);
                //save record()
                mwhandle->write_to_db(record);
                last = current;
            }
            */
            close();

        }
        else
            qWarning("Data bad");
    }
}

void Dialog::othersave()
//just updates condition and comments
{
    record = new QStringList;
    *record << ui->box_comments->toPlainText();
    *record << ui->box_condition->toPlainText();
    mwhandle->updateRecord(id, record);
}
