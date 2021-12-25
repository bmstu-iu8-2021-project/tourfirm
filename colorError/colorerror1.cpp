#include "colorerror1.h"
#include "ui_colorerror1.h"

colorError1::colorError1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colorError1)
{
    ui->setupUi(this);
}

colorError1::~colorError1()
{
    delete ui;
}

void colorError1::on_pushButton_clicked()
{
    hide();
}
