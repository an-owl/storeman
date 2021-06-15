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

