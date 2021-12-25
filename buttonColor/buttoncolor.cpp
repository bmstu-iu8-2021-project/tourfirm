#include "buttoncolor.h"
#include "ui_buttoncolor.h"
#include <colorerror1.h>

buttoncolor::buttoncolor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::buttoncolor)
{
    ui->setupUi(this);
}

buttoncolor::~buttoncolor()
{
    delete ui;
}

void buttoncolor::on_open_menu_clicked()
{

    this->close();
    emit openMenu();
}


void buttoncolor::on_grey_clicked()
{
         emit giveButtonColor("background-color: rgb(185, 185, 185);");
}


void buttoncolor::on_black_clicked()
{        if (currentColor == "color: black;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
         emit giveButtonColor("background-color: black;");
    }
}


void buttoncolor::on_white_clicked()
{        if (currentColor == "color: white;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
         emit giveButtonColor("background-color: white;");
    }
}


void buttoncolor::on_blue_clicked()
{    if (currentColor == "color: blue;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
         emit giveButtonColor("background-color: blue;");
    }
}


void buttoncolor::on_brown_clicked()
{         if (currentColor == "color: brown;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
         emit giveButtonColor("background-color: brown;");
    }
}


void buttoncolor::on_lightblue_clicked()
{
         emit giveButtonColor("background-color: lightblue;");
}



void buttoncolor::on_yellow_clicked()
{       if (currentColor == "color: yellow;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
          emit giveButtonColor("background-color: yellow;");
    }
}


void buttoncolor::on_purple_clicked()
{          if (currentColor == "color: purple;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
          emit giveButtonColor("background-color: purple;");
    }
}


void buttoncolor::on_green_clicked()
{          if (currentColor == "color: green;"){
        colorError1 errorWindow;
        errorWindow.exec();
    } else{
          emit giveButtonColor("background-color: green;");
    }
}
