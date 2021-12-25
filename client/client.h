#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "ui_client.h"

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = nullptr);
    ~client();

    void setDB(QSqlQuery *queryTours, QSqlQuery *queryClients, QSqlQuery *queryReservations, std::atomic<bool> *threadFinished){
        queryTours_ = queryTours;
        queryClients_ = queryClients;
        queryReservations_ = queryReservations;
        threadFinished_ = threadFinished;
    };

    void setStyle(const QString& background, const QString& textColor, QString buttonColor){
        setStyleSheet(background + textColor);
        ui->delete_2->setStyleSheet(textColor + buttonColor);
        ui->save->setStyleSheet(textColor + buttonColor);
        ui->reload_table->setStyleSheet(textColor + buttonColor);
        ui->update->setStyleSheet(textColor + buttonColor);
    }


private slots:

    void on_save_clicked();

    void on_update_clicked();

    void on_reload_table_clicked();

    void on_delete_2_clicked();

private:
    Ui::client *ui;

    QSqlQuery *queryTours_;
    QSqlQuery *queryClients_;
    QSqlQuery *queryReservations_;
    std::atomic<bool> *threadFinished_;
};

#endif // CLIENT_H
