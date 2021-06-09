#ifndef DIALOG_H
#define DIALOG_H

#include <QStringList>
#include <QString>
#include <QCryptographicHash>
#include <QDialog>
#include <cstdint>
#include "mainwindow.h"


class MainWindow;
QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    MainWindow *mwhandle;

    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void on_buttonBox_accepted();


private slots:
    /*void on_buttonBox_clicked(QAbstractButton *button);
    void on_buttonBox_rejected();*/



    void on_lineEdit_pass_editingFinished();

    void on_lineEdit_repeat_editingFinished();

private:
    Ui::Dialog *ui;
    uint64_t pwdhash;
    bool pwd1done, pwd2done; //stores if passwords have been typed on if both true its allowed to complain
    int pwdbitch();
};

#endif // DIALOG_H
