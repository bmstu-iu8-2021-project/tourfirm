#include "colorsetting.h"
#include "ui_colorsetting.h"
#include <menu.h>
#include<colorerror1.h>

colorSetting::colorSetting(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::colorSetting)
{
    ui->setupUi(this);
}

colorSetting::~colorSetting()
{
    delete ui;
}

void colorSetting::on_open_menu_clicked()
{
    this->close();
    emit openMenu();
}


void colorSetting::on_white_clicked()
{   if ((currentBackground == "background-color: white;") || (currentButtonColor == "background-color: white;")){ //если цвет фона совпадает с цветом текста выдаем окно с ошибкой
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
     emit giveTextColor("color: white;");
    }
}


void colorSetting::on_brown_clicked()
{   if ((currentBackground == "background-color: brown;") || (currentButtonColor == "background-color: brown;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
    emit giveTextColor("color: brown;");
    }
}


void colorSetting::on_black_clicked()
{   if ((currentBackground == "background-color: black;") || (currentButtonColor == "background-color: black;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
    emit giveTextColor("color: black;");
    }
}


void colorSetting::on_blue_clicked()
{
    if ((currentBackground == "background-color: blue;") || (currentButtonColor == "background-color: blue;")){
            colorError1 errorWindow;
            errorWindow.exec();
        } else{
    emit giveTextColor("color: blue;");
    }
}


void colorSetting::on_green_clicked()
{   if ((currentBackground == "background-color: green;") || (currentButtonColor == "background-color: green;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
    emit giveTextColor("color: rgb(0, 235,0);");
    }
}


void colorSetting::on_red_clicked()
{   if ((currentBackground == "background-color: red;") || (currentButtonColor == "background-color: red;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
    emit giveTextColor("color: red;");
    }
}


void colorSetting::on_grey_clicked()
{   if ((currentBackground == "background-color: grey;") || (currentButtonColor == "background-color: grey;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
    emit giveTextColor("color: grey;");
    }
}


void colorSetting::on_purple_clicked()
{
    if ((currentBackground == "background-color: purple;") || (currentButtonColor == "background-color: purple;")){
            colorError1 errorWindow;
            errorWindow.exec();
        } else{
    emit giveTextColor("color: purple;");
    }
}


void colorSetting::on_orange_clicked()
{    if ((currentBackground == "background-color: orange;") || (currentButtonColor == "background-color: orange;")){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
     emit giveTextColor("color: orange;");
    }
}
