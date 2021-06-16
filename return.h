#ifndef RETURN_H
#define RETURN_H

#include <QDialog>
#include <QCryptographicHash>
#include "mainwindow.h"

class MainWindow;
namespace Ui {
class Return;
}

class Return : public QDialog
{
    Q_OBJECT

public:
    explicit Return(QWidget *parent = nullptr, int initid = 0, QString initname = "", QString initdate = "", QByteArray initpwdhash = "");
    ~Return();

    MainWindow *mwhandle;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Return *ui;

    //contains relevant record data
    int id;
    QString name;
    QString date;
    QByteArray pwdsha;
    QByteArray hashsha(QString pwd);
    QByteArray hashmd5(QString pwd);

};

#endif // RETURN_H
