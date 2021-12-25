#include "menu.h"
#include "ui_menu.h"
#include "backgroundsetting.h"
#include <colorsetting.h>
#include <buttoncolor.h>




menu::menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);

    currentBackground = "background-color: rgb(235, 235, 235);";
    currentTextColor = "color: black;";
    currentButtonColor = "background-color: lightblue;";
    setStyleSheet(currentBackground + currentTextColor);
    setButtonColor(currentButtonColor);

    windowBackground = new backgroundSetting();//инициализируем окно backgroundColor

    connect(windowBackground, &backgroundSetting::giveBackground, this, &menu::setBackground); //для передачи информации о новом цвете фона в окно меню
    connect(windowBackground, &backgroundSetting::openMenu, this, &menu::show); //для того чтобы открыть меню из окна backgroundSetting
    connect(this, &menu::giveColor_to_backgroundsetting, windowBackground, &backgroundSetting::setColor); //для того чтобы передать информация о текущем цвете символов в окно backgroundSetting

    windowColor = new colorSetting(); //инициализируем окно colorSetting
    connect(windowColor, &colorSetting::giveTextColor, this, &menu::setColor); //для передачи информации о новом цвете текста в окно меню
    connect(windowColor, &colorSetting::openMenu, this, &menu::show); //для того чтобы открыть меню из окна colorSetting
    connect(this, &menu::giveBackground_to_colorsetting, windowColor, &colorSetting::setBackground); //чтобы получить информацию в окне colorSetting о текущем цвете фона
    connect(this, &menu::giveButtonColor_to_colosetting, windowColor, &colorSetting::setButtonColor); //чтобы получить информацию в окне colorSetting о текущем цвете кнопок


    windowButtonColor = new buttoncolor();
    connect(windowButtonColor, &buttoncolor::openMenu, this, &menu::show); //для открытия окна меню из онка изменения цвета кнопок
    connect(windowButtonColor, &buttoncolor::giveButtonColor, this, &menu::setButtonColor); //для передачи информации о новом цвете кнопок в меню
    connect(this, &menu::giveBackground_to_buttoncolor, windowButtonColor, &buttoncolor::setBackground); //для изменения фона в окне buttoncolor
    connect(this, &menu::giveColor_to_buttoncolor, windowButtonColor, &buttoncolor::setColor); //чтобы получить информацию в окне buttoncolor о текущем цвете текста

    toursWindow = new tour(); //инициализируем окно текущих туров
    connect(this, &menu::giveDBinfo, toursWindow, &tour::setDB); //чтобы получить информацию о базах данных в окне toursWindow
    connect(this, &menu::giveStyleToTours, toursWindow, &tour::setStyle);

    clientsWindow = new client(); //инициализируем окно клиентов
    connect(this, &menu::giveDBinfo2, clientsWindow, &client::setDB); //чтобы получить информацию о базах данных в окне toursClients
    connect(this, &menu::giveStyleToClients, clientsWindow, &client::setStyle);

    reservationsWindow = new reservation();
    connect(this, &menu::giveDBinfo3, reservationsWindow, &reservation::setDB);
    connect(this, &menu::giveStyleToReservations, reservationsWindow, &reservation::setMyStyle);

    multiplierWindow = new multiplier();
    connect(this, &menu::giveDBinfo4, multiplierWindow, &multiplier::setDB);
    connect(this, &menu::giveStyleToMultiplier, multiplierWindow, &multiplier::setMyStyle);

    threadFinished = new std::atomic<bool>(true);


    //база данных текущих туров
    tours = QSqlDatabase::addDatabase("QSQLITE", "connectionToTours");
    tours.setDatabaseName("./tours.db");
    queryTours = new QSqlQuery(tours);


   // база данных клиентов
    clients = QSqlDatabase::addDatabase("QSQLITE", "connectionToClients");
    clients.setDatabaseName("./clients.db");
    queryClients = new QSqlQuery(clients);

    //база данных клиентов
    reservations = QSqlDatabase::addDatabase("QSQLITE", "connectionToReservations");
    reservations.setDatabaseName("./reservations.db");
    queryReservations = new QSqlQuery(reservations);

    //база данных найстройки множителя
    multipliers = QSqlDatabase::addDatabase("QSQLITE", "connectionToMultiplier");
    multipliers.setDatabaseName("./multiplier.db");
    queryMultipliers = new QSqlQuery(multipliers);

    if(tours.open()){
       qDebug()<<"tours opened in menu window!";
    } else{
       qDebug()<<"tours NOT opened in menu window!";
    }
    if(clients.open()){
       qDebug()<<"clients opened in menu window!";
    } else{
       qDebug()<<"clients NOT opened in menu window!";
    }
    if(reservations.open()){
       qDebug()<<"reservations opened in menu window!";
    } else{
       qDebug()<<"reservations NOT opened in menu window!";
    }
    if (multipliers.open()){
        qDebug()<<"multipliers opened in menu window!";
     } else {
        qDebug()<<"multipliers NOT opened in menu window!";
     }
}

menu::~menu()
{
    delete ui;
}

void menu::setButtonColor(QString newButtonColor){
    currentButtonColor = newButtonColor;
    ui->booking->setStyleSheet(newButtonColor + currentTextColor);
    ui->client->setStyleSheet(newButtonColor + currentTextColor);
    ui->tours->setStyleSheet(newButtonColor + currentTextColor);
    ui->coefficient->setStyleSheet(newButtonColor + currentTextColor);
}

void menu::setColor (QString newTextColor){
    currentTextColor = newTextColor;
    setStyleSheet(currentBackground + currentTextColor);
    ui->booking->setStyleSheet(currentButtonColor + currentTextColor);
    ui->client->setStyleSheet(currentButtonColor + currentTextColor);
    ui->tours->setStyleSheet(currentButtonColor + currentTextColor);
    ui->coefficient->setStyleSheet(currentButtonColor + currentTextColor);
}




void menu::on_changeColorBackground_triggered(){ //открываем окно с настройкой фонового цвета
    emit giveColor_to_backgroundsetting(currentTextColor);
    close();
    windowBackground->show();
}


void menu::on_exit_triggered()  //кнопка выхода из аккаунта
{
    if (toursWindow->isVisible()){
        toursWindow->close();
    }
    if (clientsWindow->isVisible()){
            clientsWindow->close();
        }
    if (reservationsWindow->isVisible()){
            reservationsWindow->close();
        }

    if (multiplierWindow->isVisible()){
        multiplierWindow->close();
    }
    this->close();
    emit openLoginWindow();
}


void menu::on_changeColorSymbols_triggered() //открываем окно с натсройкой цвета текста
{   emit giveBackground_to_colorsetting(currentBackground);
    emit giveButtonColor_to_colosetting(currentButtonColor);
    windowColor->show();
    close();
}


void menu::on_action_triggered() //открываем окно с настройкой цвета кнопок
{
        emit giveBackground_to_buttoncolor(currentBackground);
        emit giveColor_to_buttoncolor(currentTextColor);
        windowButtonColor->show();
        close();
}


void menu::on_tours_clicked()
{
    emit giveDBinfo(queryTours, queryClients, queryReservations, threadFinished);
    emit giveStyleToTours(currentBackground, currentTextColor, currentButtonColor);
    toursWindow->show();
}


void menu::on_client_clicked()
{
    emit giveDBinfo2(queryTours, queryClients, queryReservations, threadFinished);
    emit giveStyleToClients(currentBackground, currentTextColor, currentButtonColor);
    clientsWindow->show();
}


void menu::on_booking_clicked()
{
     emit giveDBinfo3(queryTours, queryClients, queryReservations, queryMultipliers, threadFinished);
     emit giveStyleToReservations(currentBackground, currentTextColor, currentButtonColor);
     reservationsWindow->show();
}


void menu::on_coefficient_clicked()
{
    emit giveDBinfo4(queryMultipliers, threadFinished);
    emit giveStyleToMultiplier(currentBackground, currentTextColor, currentButtonColor);
    multiplierWindow->show();
}
