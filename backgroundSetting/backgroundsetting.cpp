#include "backgroundsetting.h"
#include "ui_backgroundsetting.h"
#include "menu.h"
#include <colorerror1.h>


backgroundSetting::backgroundSetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::backgroundSetting)
{
    ui->setupUi(this);
    setStyleSheet("background-color: white;");

    QPixmap pixmapBrush( ":/files/brush" );
    QPixmap pixmapBrush2( ":/files/brush" );

    ui->brush->setScaledPixmap(pixmapBrush);
    ui->brush2->setScaledPixmap(pixmapBrush2);

}


backgroundSetting::~backgroundSetting()
{
    delete ui;
}

void backgroundSetting::on_pushButton_clicked()
{
   this->close();
   emit openMenu();

}


void backgroundSetting::on_lightblue_clicked()
{
       setStyleSheet("background-color: rgb(1, 200, 255);");
       emit giveBackground("background-color: rgb(1, 200, 255);");
}


void backgroundSetting::on_blue_clicked()
{   if (currentColor == "color: blue;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: blue;");
     emit giveBackground("background-color: blue;");
    }
}


void backgroundSetting::on_black_clicked()
{   if (currentColor == "color: black;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: black;");
     emit giveBackground("background-color: black;");
    }
}


void backgroundSetting::on_white_clicked()
{   if (currentColor == "color: white;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: white;");
     emit giveBackground("background-color: white;");
    }
}


void backgroundSetting::on_orange_clicked()
{   if (currentColor == "color: orange;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: orange;");
     emit giveBackground("background-color: orange;");
    }
}


void backgroundSetting::on_purple_clicked()
{    if (currentColor == "color: purple;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: purple;");
     emit giveBackground("background-color: purple;");
    }
}


void backgroundSetting::on_green_clicked()
{   if (currentColor == "color: green;"){
        colorError1 windowError;
        windowError.exec();
    } else{
     setStyleSheet("background-color: green;");
     emit giveBackground("background-color: green;");
    }
}


void backgroundSetting::on_yellow_clicked()
{    if (currentColor == "color: yellow;"){
        colorError1 windowError;
        windowError.exec();
    } else {
     setStyleSheet("background-color: yellow;");
     emit giveBackground("background-color: yellow;");
    }
}


void backgroundSetting::on_grey_clicked()
{
     setStyleSheet("background-color: rgb(235, 235, 235);");
     emit giveBackground("background-color: rgb(235, 235, 235);");
}
