#ifndef RETURN_H
#define RETURN_H

#include <QDialog>

namespace Ui {
class Return;
}

class Return : public QDialog
{
    Q_OBJECT

public:
    explicit Return(QWidget *parent = nullptr, int initid = 0, QString initname = "", QString initdate = "", QByteArray initpwdhash = "");
    ~Return();

private:
    Ui::Return *ui;

    //contains relevant record data
    int id;
    QString name;
    QString date;
    QByteArray pwdsha;

};

#endif // RETURN_H
