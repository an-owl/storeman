#include "return.h"
#include "ui_return.h"

Return::Return(QWidget *parent ,int id ,QString name ,QString date ,QByteArray pwdhash) :
    QDialog(parent),
    ui(new Ui::Return)
{
    ui->setupUi(this);
}

Return::~Return()
{
    delete ui;
}
