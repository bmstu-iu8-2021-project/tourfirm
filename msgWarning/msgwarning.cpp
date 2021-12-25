#include "msgwarning.h"
#include "ui_msgwarning.h"
#include "scaledpixmap.h"

msgWarning::msgWarning(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::msgWarning)
{
    ui->setupUi(this);

    QPixmap pixmapSad( ":/files/sadEmoji" );

    ui->img->setScaledPixmap(pixmapSad);
}

msgWarning::~msgWarning()
{
    delete ui;
}

void msgWarning::on_pushButton_clicked()
{
    hide();
}

