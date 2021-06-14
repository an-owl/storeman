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
    explicit Return(QWidget *parent = nullptr,int id = 0, QString name = "",QString date = "", QByteArray pwdhash = "");
    ~Return();

private:
    Ui::Return *ui;
};

#endif // RETURN_H
