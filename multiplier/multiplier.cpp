#include "multiplier.h"
#include "ui_multiplier.h"
#include <QSqlQueryModel>
#include <QMessageBox>
#include <multipliersFunctions.h>


void execQueryMultiplierUpdate(QSqlQuery* queryMultipliers_, std::atomic<bool> *threadFinished, const  QString& koef, const QString& percent){

    queryMultipliers_->prepare("UPDATE Multiplier SET multiplier = '"+koef+"' where percent ='"+percent+"'");
    if (queryMultipliers_->exec()){
         qDebug()<<"Data updated";
    } else{
         qDebug()<<"Data not updated";
    }

    *threadFinished = true;
}

multiplier::multiplier(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::multiplier)
{
    ui->setupUi(this);
}

multiplier::~multiplier()
{
    delete ui;
}


void multiplier::on_reload_table_clicked()
{
    QSqlQueryModel* modelMultipliers = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryMultipliers_;

    selectQuery -> prepare("SELECT * from Multiplier");
    selectQuery->exec();

    modelMultipliers ->setQuery(*selectQuery);
    ui->tableView->setModel(modelMultipliers);
}


void multiplier::on_pushButton_clicked()
{
    QString percent = ui->line_percent->text();
    QString koef = ui->line_multiplier->text();
    switch(checkInputData(percent, koef, queryMultipliers_)){
    case 1:
        QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
        break;
    case 2:
        QMessageBox::critical(this, tr("Error"), tr("Invalid data of percent"));
        break;
    case 3:
        QMessageBox::critical(this, tr("Error"), tr("Invalid data of multiplier"));
        break;
    case 4:
        QMessageBox::critical(this, tr("Error"), tr("Multiplier must be more than 1"));
        break;
    case 5:
        QMessageBox::critical(this, tr("Error"), tr("Multipliers must increase"));
        break;
    case 6:
        QMessageBox::critical(this, tr("Error"), tr("Multiplier must be not more than 10"));
        break;
    default:

        if (*threadFinished_){
            *threadFinished_ = false;
            std::thread thread(execQueryMultiplierUpdate, std::ref(queryMultipliers_), std::ref(threadFinished_), std::ref(koef), std::ref(percent));
            thread.detach();
            QMessageBox::information(this, tr("Update status"), tr("Data updated"));
            return;
        }
       QMessageBox::critical(this, tr("Error"), tr("Wait other thread finished"));

   }
}

