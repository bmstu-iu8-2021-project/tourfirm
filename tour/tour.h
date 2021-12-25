#ifndef TOUR_H
#define TOUR_H

#include <QMainWindow>
#include "ui_tour.h"
#include"QDebug"
#include "QMessageBox"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>


namespace Ui {
class tour;
}

class tour : public QMainWindow
{
    Q_OBJECT

public:
    explicit tour(QWidget *parent = nullptr);
    ~tour();


    void setDB(QSqlQuery *queryTours, QSqlQuery *queryClients,  QSqlQuery *queryReservations, std::atomic<bool> *threadFinished){
        queryTours_ = queryTours;
        queryClients_ = queryClients;
        queryReservations_ = queryReservations;
        threadFinished_ = threadFinished;
    };

    void setStyle(const QString& background, const QString& textColor, QString buttonColor){
        setStyleSheet(background + textColor);
        ui->deleteButton->setStyleSheet(textColor + buttonColor);
        ui->save_button->setStyleSheet(textColor + buttonColor);
        ui->reload_table->setStyleSheet(textColor + buttonColor);
        ui->update_button->setStyleSheet(textColor + buttonColor);
    }

private slots:

    void on_reload_table_clicked();

    void on_update_button_clicked();

    void on_save_button_clicked();

    void on_deleteButton_clicked();

private:
    Ui::tour *ui;
    QSqlQuery *queryTours_;
    QSqlQuery *queryClients_;
    QSqlQuery *queryReservations_;

    std::atomic<bool>* threadFinished_;
};

#endif // TOUR_H
