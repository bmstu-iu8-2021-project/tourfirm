#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPixmap"
#include <QMessageBox>
#include <QtWidgets>
#include "menu.h" //чтобы открыть второе окно
#include "msgwarning.h" //окно предупреждения о неверном пароле
#include <QDebug>
#include "sha512.h"

bool checkLogin (const QString& login, QString& password, QSqlQuery* query){
    query->exec("SELECT * FROM Accounts where Login = '"+login+"'");
    while (query->next()){
        QString bdSalt = query->value("Salt").toString();

        SHA512 sha512;
        password = QString::fromStdString(sha512.hash((password+bdSalt).toStdString())); //составляем хэш (password+salt)
        QString bdPassword = query->value("Password").toString();
        if (bdPassword == password){
            return true;
        }
    }
    return false;
}

bool checkValidSymbols(const QString& qstr){
    std::string str = qstr.toStdString();
    for (size_t i = 0; i < str.length(); ++i){
        if (!(((str[i] > 47) && (str[i] <58)) || ((str[i] > 64) && (str[i] < 91)) || ((str[i] > 96) && (str[i] <123)))){ //разрешаем цифры, заглавные и строчные буквы
            return false;
        }
    }
    return true;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pixmapPlane( ":/files/plane" );
    QPixmap pixmapLeft( ":/files/leftImage" );
    QPixmap pixmapRight( ":/files/rightImage" );

    ui->leftPicture->setScaledPixmap(pixmapLeft);
    ui->rightPicture->setScaledPixmap(pixmapRight);
    ui->labelPlane->setScaledPixmap(pixmapPlane);

    ui->labelText->setAlignment(Qt::AlignCenter);

    window = new menu();//инициализируем окно
    connect(window, &menu::openLoginWindow, this, &MainWindow::show);

    accounts = QSqlDatabase::addDatabase("QSQLITE", "connectionToAccounts");
    accounts.setDatabaseName("./accounts.db");
    if (accounts.open()){
        qDebug("open");
    }
    query = new QSqlQuery(accounts);


}

MainWindow::~MainWindow()
{
    delete window;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString login = ui->lineLogin->text();
    QString password = ui->linePassword->text();

    if(!checkValidSymbols(login)){
        QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in login"));
    }
    if(!checkValidSymbols(password)){
        QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in password"));
    }

     if (checkLogin(login, password, query)){ //проверяем есть ли такой логин и пароль в бд
         close();
         window->show();
         ui->lineLogin->clear();
         ui->linePassword->clear();
    } else {
        msgWarning msgWindow;
        msgWindow.exec();
    }
}
