#include "client.h"
#include "ui_client.h"
#include <QMessageBox>
#include <clientFunctions.h>
#include <QSqlQueryModel>

void execQueryClientsSave(QSqlQuery* queryClients_, std::atomic<bool>* threadFinished, const QString& id, const QString& name, const QString& surname, const QString& birthday, const QString& tours_score){
    queryClients_->prepare("insert into Clients (id, name, surname, birthday, current_tour_id, tours_score) values('"+id+"', '"+name+"', '"+surname+"', '"+birthday+"', 0, '"+tours_score+"')");

    if(queryClients_->exec()){
        qDebug()<<"Data saved"
;    } else {
        qDebug()<<"Data NOT saved";
    }

    *threadFinished = true;
}


void execQueryClientsUpdate(QSqlQuery* queryClients_, QSqlQuery* queryReservations_, std::atomic<bool>* threadFinished, const QString& id, const QString& name, const QString& surname, const QString& birthday, const QString& tours_score){

    if (!name.isEmpty()){
        updateReservations(id, name, queryReservations_); //изменяем name_client в базе данных бронирования
        queryClients_->prepare("UPDATE Clients SET name = '"+name+"' where id ='"+id+"'");
        if (queryClients_->exec()){
        qDebug()<<"Data updated";
        } else{
             qDebug()<<"Data not updated";
        }
    }
    if (!surname.isEmpty()){
        queryClients_->prepare("UPDATE Clients SET surname = '"+surname+"' where id ='"+id+"'");
        if (queryClients_->exec()){
        qDebug()<<"Data updated";
        } else{
            qDebug()<<"Data not updated";
        }
    }
    if (!birthday.isEmpty()){
        queryClients_->prepare("UPDATE Clients SET birthday = '"+birthday+"' where id ='"+id+"'");
        if (queryClients_->exec()){
       qDebug()<<"Data updated";
        } else{
             qDebug()<<"Data not updated";
        }
    }
    if (!tours_score.isEmpty()){
        queryClients_->prepare("UPDATE Clients SET tours_score = '"+tours_score+"' where id ='"+id+"'");
        if (queryClients_->exec()){
        qDebug()<<"Data updated";
        } else {
             qDebug()<<"Data not updated";
        }
    }

    *threadFinished = true;
}


void execQueryClientsDelete(QSqlQuery* queryTours_, QSqlQuery* queryClients_, QSqlQuery* queryReservations_, std::atomic<bool>* threadFinished, const QString& id){

    updateToursReservations(id, queryReservations_, queryTours_);
    queryClients_->prepare("delete from Clients where id = '"+id+"'");

    if(queryClients_->exec()){
        qDebug()<<"Data deleted";
    } else {
        qDebug()<<"Data not deleted";
    }

    *threadFinished = true;
 }





client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
}

client::~client()
{
    delete ui;
}

void client::on_save_clicked()
{
    QString id, name, surname, birthday, tours_score;
    id = ui->line_id->text();
    name =ui->line_name->text(); //имя клиента
    surname = ui ->line_surname -> text(); //фамилия клиента
    birthday = ui->line_birthday->text(); //дата рождения клиента
    tours_score = ui->line_score->text(); //количество очков (забронированных туров за все время) для рейтинга

   switch(checkDataClient(id, name, surname, birthday, tours_score, queryClients_, 1)){ //проверяем корректность введеных данных
   case 1:
       QMessageBox::critical(this, tr("Error"), tr("All fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 3: QMessageBox::critical(this, tr("Error"), tr("Such id alredy exists"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid format of date! Valid format: XX.YY.ZZZZ"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of score"));
       break;
   default:
       if (*threadFinished_){
           *threadFinished_ = false;
           std::thread thread(execQueryClientsSave, std::ref(queryClients_), std::ref(threadFinished_), std::ref(id), std::ref(name), std::ref(surname), std::ref(birthday), std::ref(tours_score));
           thread.detach();
           QMessageBox::information(this, tr("Saved status"), tr("Data saved"));
           break;
       }
       QMessageBox::critical(this, tr("Saved status"), tr("Data not saved"));

    }

}

//ПРИ ИЗМЕНЕНИИ ИМЕНИ СДЕЛАТЬ ИЗМЕНЕНИЕ В БАЗЕ ДАННЫХ БРОНИРОВАНИЯ NAME CLIENT +

void client::on_update_clicked()
{
    QString id, name, surname, birthday, tours_score;
    id = ui->line_id->text();
    name =ui->line_name->text(); //имя клиента
    surname = ui ->line_surname -> text(); //фамилия клиента
    birthday = ui->line_birthday->text(); //дата рождения клиента
    tours_score = ui->line_score->text(); //количество очков (забронированных туров за все время) для рейтинга

   switch(checkDataClient(id, name, surname, birthday, tours_score, queryClients_, 2)){ //проверяем корректность введеных данных

   case 7:
       QMessageBox::critical(this, tr("Error"), tr("Fields must be filled"));
       break;
   case 2:
       QMessageBox::critical(this, tr("Error"), tr("Invaild id data"));
       break;
   case 6: QMessageBox::critical(this, tr("Error"), tr("Such id doesn't exists"));
       break;
   case 4: QMessageBox::critical(this, tr("Error"), tr("Invalid format of date! Valid format: XX.YY.ZZZZ"));
       break;
   case 5: QMessageBox::critical(this, tr("Error"), tr("Invalid data of score"));
       break;
   case 8: QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in name"));
       break;
   case 9: QMessageBox::critical(this, tr("Error"), tr("Invalid symbols in surname"));
       break;

   default:
       if (*threadFinished_){
                  *threadFinished_ = false;
                  std::thread thread(execQueryClientsUpdate, std::ref(queryClients_), std::ref(queryReservations_), std::ref(threadFinished_), std::ref(id), std::ref(name), std::ref(surname), std::ref(birthday), std::ref(tours_score));
                  thread.detach();
                  QMessageBox::information(this, tr("Update status"), tr("Data updated"));
                  break;
              }
              QMessageBox::critical(this, tr("Update status"), tr("Data not updated"));

   }
}


void client::on_reload_table_clicked()
{
    QSqlQueryModel* modelTours = new QSqlQueryModel();
    QSqlQuery* selectQuery = queryClients_;

    selectQuery -> prepare("SELECT * from Clients");
    selectQuery->exec();

    modelTours ->setQuery(*selectQuery);
    ui->tableView->setModel(modelTours);
}



//При удалении удалить также в бронировании где этот клиент и available rooms увеличить в туре
void client::on_delete_2_clicked()
{
    QString id;
    id = ui->line_id->text();

    if (!id.toUInt()){
        QMessageBox::critical(this, tr("Error"), tr("Invalid data in id"));
    }

    if (!id_exists(id, queryClients_) || (id.isEmpty())){
         QMessageBox::critical(this, tr("Error"), tr("id doesn't exist"));
    } else {
        if (*threadFinished_){
                   *threadFinished_ = false;
                   std::thread thread(execQueryClientsDelete, std::ref(queryTours_), std::ref(queryClients_), std::ref(queryReservations_), std::ref(threadFinished_), std::ref(id));
                   thread.detach();
                   QMessageBox::information(this, tr("Delete status"), tr("Data deleted"));
                   return;
               }
               QMessageBox::critical(this, tr("Delete status"), tr("Data not deleted"));
    }
}
