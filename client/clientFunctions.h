
#ifndef CLIENTFUNCTIONS_H
#define CLIENTFUNCTIONS_H

#include <QString>
#include <QSqlQuery>

bool validSymbols(const QString& qstr){
    std::string str = qstr.toStdString();
    for (size_t i = 0; i < str.length(); ++i){
        if (!(((str[i] > 47) && (str[i] <58)) || ((str[i] > 64) && (str[i] < 91)) || ((str[i] > 96) && (str[i] <123)))){ //разрешаем цифры (если тезка будет проще различить не только по id, заглавные и строчные английские буквы
            return false;
        }
    }
    return true;
}


bool id_exists(const QString& id, QSqlQuery* query){ //проверяем уникальность нового id в бд
    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return  true;
        }
    }
    return false;
}

bool is_date(const QString& date){

    if (date.length() == 10){
        QString day, month, year;
        day+=date[0];
        day+=date[1];
        month += date[3];
        month += date[4];
        year += date[6];
        year += date[7];
        year += date[8];
        year += date[9];

          if ((date[2] != '.') || (date[5] != '.')){
              return false;
          }
          if ((day.toUInt() > 31) || (day.toUInt() < 1) || ((month == "02") && (day.toUInt() > 29))){
              return false;
          }
          if ((month.toUInt() == 0) || (month.toUInt() > 12)){
               return false;
          }
          if ((year.toUInt() < 1000) || (year.toUInt() > 2021)){
              return false;
          }
          return true;
    }
    return false;
}

int checkDataClient(const QString& id, const QString& name, const QString& surname, const QString& birthday, const QString& tours_score, QSqlQuery* queryClients, int checkMode){


    if (checkMode == 1){
       if ((id.isEmpty()) ||(name.isEmpty()) || (surname.isEmpty()) || (birthday.isEmpty()) || (tours_score.isEmpty())){
           return 1; //код ошибки, что есть незаполненные поля
       }
    }
    if (!id.toUInt()){
        return 2; //код ошикби, что введены некорректные данные id
    }
    if ((checkMode == 1) && (id_exists(id, queryClients))){
           return 3; //код ошибки, что такой id уже существует
    }

    if ((!birthday.isEmpty()) && (!is_date(birthday))){
           return 4; // код ошибки, что дата введена в некорректном формате
    }

    if ((!tours_score.isEmpty()) && (!tours_score.toUInt())){
           return 5; //код ошибки, что введены некорректные данные общего количества баллов за бронирование
    }

    if ((!validSymbols(name)) && (!name.isEmpty())){
        return 8; //код ошикби что запрещенные символы в поле name
    }
    if ((!validSymbols(surname)) && (!surname.isEmpty())){
        return 9; //код ошикби что запрещенные символы в поле name
    }

    //для update:
    if (!id_exists(id, queryClients)){
        return 6; //ошибка что невозможно найти id, чтобы его изменить
    }

    if ((id.isEmpty()) || ((name.isEmpty()) && (surname.isEmpty()) && (birthday.isEmpty()) && (tours_score.isEmpty()))){
        return 7; //ошибка что не заполнены поля
}


    return 0;
}

void updateReservations(const QString& id_client, const QString& name_client, QSqlQuery* queryReservations_){

    queryReservations_->prepare("UPDATE Reservations SET name_client = '"+name_client+"' where id_client ='"+id_client+"'");
    if (queryReservations_->exec()){
    qDebug()<<"reservations updated!";
    } else {
         qDebug()<<"reservations NOT updated!";
    }

}

QString createQString(const std::string& str){
    QString qStr;
    for (size_t i = 0; i<str.length(); ++i){
        qStr += str[i];
    }
    return qStr;
}

void updateTour(const QString& id_hotel, QSqlQuery* queryTours){
    //увеличиваем available_rooms у id_hotel
    queryTours->exec("SELECT * FROM Tours");
    while (queryTours->next()){
        QString bdId = queryTours->value("id").toString();
        if (bdId == id_hotel){
            QString available_rooms = queryTours->value("available_rooms").toString();
            int newRooms = available_rooms.toInt() + 1;
            std::string strRooms = std::to_string(newRooms);
            available_rooms = createQString(strRooms);
            queryTours->prepare("UPDATE Tours SET available_rooms = '"+available_rooms+"' where id = '"+id_hotel+"'");
            if (queryTours ->exec()){
                qDebug()<<"tours updated!!!";
                break;

            } else {
                qDebug()<<"tours not updated!!!!!!!";
                break;
            }
        }
    }
}

void updateToursReservations(const QString& id_client,QSqlQuery* queryReservations_, QSqlQuery* queryTours_){
    queryReservations_->exec("SELECT * FROM Reservations");
    while (queryReservations_->next()){
        QString bdIdClient = queryReservations_->value("id_client").toString();
        if (bdIdClient == id_client){
            updateTour(queryReservations_->value("id_hotel").toString(), queryTours_); //убираем currentTour
            queryReservations_->prepare("delete from Reservations where id_client = '"+id_client+"'");
            if(queryReservations_->exec()){
                qDebug()<<"reservation deleted!";
            } else {
                qDebug()<<"reservation NOT deleted!";
            }
        }
    }
}

#endif // CLIENTFUNCTIONS_H
