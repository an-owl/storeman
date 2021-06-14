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
    explicit Return(QWidget *parent = nullptr);
    ~Return();

private:
    Ui::Return *ui;
};

#endif // RETURN_H
