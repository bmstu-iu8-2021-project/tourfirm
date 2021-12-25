#include "tour.h"
#include <tourFunctions.h>
#include <QSqlQueryModel>
#include <QSqlTableModel>

void execQueryToursSave(QSqlQuery* queryTours_, std::atomic<bool>* threadFinished, const QString& id, const QString& name, const QString& availableRooms, const QString& amountRooms, const QString& price, const QString& stars, const QString& charter){
    queryTours_->prepare("insert into Tours (id, name, available_rooms, amount_rooms, price, stars, charter) values('"+id+"', '"+name+"', '"+availableRooms+"', '"+amountRooms+"', '"+price+"', '"+stars+"', '"+charter+"')");
    if(queryTours_->exec()){
        qDebug()<<"data saved";
    } else {
        qDebug()<<"data NOT saved";
    }
    *threadFinished = true;
}

void execQueryToursUpdate(QSqlQuery* queryTours_, QSqlQuery* queryReservations_, std::atomic<bool>* threadFinished, const QString& id, const QString& name, QString& availableRooms, const QString& amountRooms, const QString& price, const QString& stars, const QString& charter){
    if (!name.isEmpty()){
         queryTours_->exec("UPDATE Tours SET name = '"+name+"' where id = '"+id+"'");
         updateNameInReservations(id, name, queryReservations_);
    }

    if (!amountRooms.isEmpty()){
    int newAvailableRoooms = amountRooms.toInt() - getAmountRooms(id, queryTours_).toInt() + getAvailableRooms(id,queryTours_).toInt(); //считаем новое значение количества доступных комнат
    std::string avRooms = std::to_string(newAvailableRoooms);
    availableRooms = makeQString(avRooms);
    queryTours_->exec("UPDATE Tours SET available_rooms = '"+availableRooms+"', amount_rooms = '"+amountRooms+"' where id = '"+id+"'");
    }

    if(!price.isEmpty()){
        queryTours_->exec("UPDATE Tours SET price ='"+price+"' where id = '"+id+"'");
    }

    if (!stars.isEmpty()){
        queryTours_->exec("UPDATE Tours SET stars ='"+stars+"' where id = '"+id+"'");
    }

    if (!charter.isEmpty()){
        queryTours_->exec("UPDATE Tours SET charter ='"+charter+"' where id = '"+id+"'");
    }
     *threadFinished = true;
}

void execQueryToursDelete(QSqlQuery* queryTours_, QSqlQuery* queryReservations_, QSqlQuery* queryClients_, std::atomic<bool>* threadFinished, const QString& id){
    updateClientsReservations_whenDeleteTour(id, queryClients_, queryReservations_);
    queryTours_->prepare("delete from Tours where id = '"+id+"'");

    if(queryTours_->exec()){
       qDebug()<<"data deleted";
    } else {
       qDebug()<<"data NOT deleted";
    }

    *threadFinished = true;
}


tour::tour(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::tour)
{
    ui->setupUi(this);
}

tour::~tour()
{
    delete ui;
}

void tour::on_save_button_clicked()
{
    QString id, name, availableRooms, amountRooms, price, stars, charter;
    id = ui->line_id->text();
    name =ui->line_name->text(); //название тура/отеля
    amountRooms = ui->line_all_rooms->text(); //всего свмест в отеле
    availableRooms = amountRooms;
    price = ui->line_price->text(); //цена
    stars = ui->line_stars->text(); //класс тура (количество звезд отеля)
    charter = ui->line_charter->text(); //иннформация чартер или нет

   switch(checkDataTour(id, name, availableRooms, amountRooms, price, stars, charter, queryTours_, 1)){ //проверяем корректность введеных данных
   case 1:
       QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 3: QMessageBox::critical(this, tr("Error"), tr("Such id alredy exists"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of amount of rooms"));
       break;
   case 7: QMessageBox::critical(this, tr("Error"), tr("Invalid price data"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid stars data"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("The number of stars should be no more than 5"));
       break;
   case 10: QMessageBox::critical(this, tr("Error"), tr("Invalid charter data"));
       break;
   case 13: QMessageBox::critical(this, tr("Error"), tr("Invalid name symbols"));
       break;
   default:
       if (*threadFinished_){
           *threadFinished_ = false;
           std::thread thread(execQueryToursSave, std::ref(queryTours_), std::ref(threadFinished_),std::ref(id), std::ref(name), std::ref(availableRooms), std::ref(amountRooms), std::ref(price), std::ref(stars), std::ref(charter));
           thread.detach();
           QMessageBox::information(this, tr("Saved status"), tr("Data saved"));
           break;
       }
        QMessageBox::critical(this, tr("Error"), tr("Wait other thread finished"));
    }

}

//ПРИ ИЗМЕНЕНИИ ИМЕНИ СДЕЛАТЬ ИЗМЕНЕНИЕ В БАЗЕ ДАННЫХ БРОНИРОВАНИЯ NAME TOUR
void tour::on_update_button_clicked()
{
    QString id, name, availableRooms, amountRooms, price, stars, charter;
    id = ui->line_id->text();
    name =ui->line_name->text(); //название тура/отеля
    amountRooms = ui->line_all_rooms->text(); //всего мест в отеле
    availableRooms = amountRooms;
    price = ui->line_price->text(); //цена
    stars = ui->line_stars->text(); //класс тура (количество звезд отеля)
    charter = ui->line_charter->text(); //иннформация чартер или нет

   switch(checkDataTour(id, name, availableRooms, amountRooms, price, stars, charter, queryTours_, 2)){ //проверяем корректность введеных данных

   case 12:
       QMessageBox::critical(this, tr("Error"), tr("Fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 11: QMessageBox::critical(this, tr("Error"), tr("Such id doesn't exist"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of amount of rooms"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("New value of amount of rooms is less than reservations"));
       break;
   case 7: QMessageBox::critical(this, tr("Error"), tr("Invalid price data"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid stars data"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("The number of stars should be no more than 5"));
       break;
   case 10: QMessageBox::critical(this, tr("Error"), tr("Invalid charter data"));
       break;
   case 13:
       QMessageBox::critical(this, tr("Error"), tr("Invalid name symbols"));
              break;
   default:
       if (*threadFinished_){
           *threadFinished_ = false;
           std::thread thread(execQueryToursUpdate, std::ref(queryTours_), std::ref(queryReservations_), std::ref(threadFinished_),std::ref(id), std::ref(name), std::ref(availableRooms), std::ref(amountRooms), std::ref(price), std::ref(stars), std::ref(charter));
           thread.detach();
           QMessageBox::information(this, tr("Update status"), tr("Data updated"));
           break;
       }
      QMessageBox::critical(this, tr("Error"), tr("Wait other thread finished"));
   }
}


void tour::on_reload_table_clicked()
{
    QSqlQueryModel* modelTours = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryTours_;

    selectQuery -> prepare("SELECT * from Tours");
    selectQuery->exec();

    modelTours ->setQuery(*selectQuery);
    ui->tableView->setModel(modelTours);

}

void tour::on_deleteButton_clicked()
{
    QString id = ui->line_id->text();
    if ((!id.toUInt()) || (id.isEmpty())){
        QMessageBox::critical(this, tr("Error"), tr("Invalid id data"));
        return;
    }

    if (is_unique(id, queryTours_)){
         QMessageBox::critical(this, tr("Error"), tr("id doesn't exist"));
    } else {
        if (*threadFinished_){
            *threadFinished_ = false;
            std::thread thread(execQueryToursDelete, std::ref(queryTours_), std::ref(queryReservations_), std::ref(queryClients_), std::ref(threadFinished_),std::ref(id));
            thread.detach();
            QMessageBox::information(this, tr("Delete status"), tr("Data deleted"));
            return;
        }
       QMessageBox::critical(this, tr("Error"), tr("Wait other thread finished"));
    }
}

