#include "return.h"
#include "ui_return.h"

Return::Return(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Return)
{
    ui->setupUi(this);
}

Return::~Return()
{
    delete ui;
}
