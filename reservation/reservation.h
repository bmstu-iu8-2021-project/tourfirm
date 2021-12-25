#ifndef RESERVATION_H
#define RESERVATION_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <ui_reservation.h>

namespace Ui {
class reservation;
}

class reservation : public QMainWindow
{
    Q_OBJECT

public:
    explicit reservation(QWidget *parent = nullptr);
    ~reservation();

    void setDB(QSqlQuery *queryTours, QSqlQuery *queryClients,QSqlQuery *queryReservations, QSqlQuery* queryMultipliers, std::atomic<bool> *threadFinished){
        queryTours_ = queryTours;
        queryClients_ = queryClients;
        queryReservations_ = queryReservations;
        queryMultipliers_ = queryMultipliers;
        threadFinished_ = threadFinished;
    };

    void setMyStyle(const QString& background, const QString& textColor, QString buttonColor){
    setStyleSheet(background + textColor);
    ui->delete_2->setStyleSheet(textColor + buttonColor);
    ui->save->setStyleSheet(textColor + buttonColor);
    ui->update->setStyleSheet(textColor + buttonColor);
    ui->reload->setStyleSheet(textColor + buttonColor);
    }

private slots:
    void on_save_clicked();

    void on_update_clicked();

    void on_delete_2_clicked();

    void on_reload_clicked();

private:
    Ui::reservation *ui;

    QSqlQuery *queryTours_;
    QSqlQuery *queryClients_;
    QSqlQuery *queryReservations_;
    QSqlQuery *queryMultipliers_;
    std::atomic<bool> *threadFinished_;

    bool boolScore;
};

#endif // RESERVATION_H
