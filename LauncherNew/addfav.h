#ifndef ADDFAV_H
#define ADDFAV_H

#include <QDialog>

namespace Ui {
    class addfav;
}

class addfav : public QDialog
{
    Q_OBJECT

public:
    explicit addfav(QWidget *parent = 0);
    ~addfav();
    QString address;

private:
    Ui::addfav *ui;

private slots:
    void accepted();
    void rejected();
};

#endif // ADDFAV_H
