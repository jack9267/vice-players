#include "addfav.h"
#include "ui_addfav.h"

addfav::addfav(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addfav)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(rejected()));
}

addfav::~addfav()
{
    delete ui;
}

void addfav::accepted()
{
    address = ui->lineEdit->text();
    close();
}

void addfav::rejected()
{
    address = "";
    close();
}
