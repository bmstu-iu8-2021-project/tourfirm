#ifndef MULTIPLIER_H
#define MULTIPLIER_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <ui_multiplier.h>

namespace Ui {
class multiplier;
}

class multiplier : public QMainWindow
{
    Q_OBJECT

public:
    explicit multiplier(QWidget *parent = nullptr);
    ~multiplier();

    void setDB(QSqlQuery* queryMultipliers, std::atomic<bool> *threadFinished){
        queryMultipliers_ = queryMultipliers;
        threadFinished_ = threadFinished;
    };
    void setMyStyle(const QString& background, const QString& textColor, QString buttonColor){
    setStyleSheet(background + textColor);
    ui->pushButton->setStyleSheet(textColor + buttonColor);
    ui->reload_table->setStyleSheet(textColor + buttonColor);
    }


private slots:

    void on_reload_table_clicked();

    void on_pushButton_clicked();

private:
    Ui::multiplier *ui;

    QSqlQuery *queryMultipliers_;
    std::atomic<bool> *threadFinished_;
};

#endif // MULTIPLIER_H
