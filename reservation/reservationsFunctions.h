#ifndef RESERVATIONSFUNCTIONS_H
#define RESERVATIONSFUNCTIONS_H


#include <QString>
#include <QSqlQuery>


QString toQString(const std::string& str){
    QString qStr;
    for (size_t i = 0; i<str.length(); ++i){
        qStr += str[i];
    }
    return qStr;
}

QString getName(const QString& id, QSqlQuery* query, int mode){
    QString name;
    if (mode == 1){
        query->exec("SELECT * FROM Tours");
    } else {
        query->exec("SELECT * FROM Clients");
    }
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("name").toString();
        }
    }
    return name;
}

bool already_exist(const QString& id, QSqlQuery* query, int mode){
    switch (mode) {
    case 1:
        query->exec("SELECT * FROM Tours");
        break;
    case 2:
        query->exec("SELECT * FROM Clients");
        break;
    case 3:
        query->exec("SELECT * FROM Reservations");
    }
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return  true;
        }
    }
    return false;
}

bool valid_dates(const QString& dates){

    if (dates.length() == 21){
        QString day, month, year, day2, month2, year2;
        day+=dates[0];
        day+=dates[1];
        month += dates[3];
        month += dates[4];
        year += dates[6];
        year += dates[7];
        year += dates[8];
        year += dates[9];

        day2+=dates[11];
        day2+=dates[12];
        month2 += dates[14];
        month2 += dates[15];
        year2 += dates[17];
        year2 += dates[18];
        year2 += dates[19];
        year2 += dates[20];

          if ((dates[2] != '.') || (dates[5] != '.') || (dates[10] != '-') || (dates[13] != '.') ||(dates[16] != '.')){
              return false;
          }
          if ((day.toUInt() > 31) || (day.toUInt() < 1) || ((month == "02") && (day.toUInt() > 29))){
              return false;
          }
          if ((month.toUInt() == 0) || (month.toUInt() > 12)){
               return false;
          }
          if ((year.toUInt() < 2020) || (year.toUInt() > 2030)){
              return false;
          }

          if ((day2.toUInt() > 31) || (day2.toUInt() < 1) || ((month2 == "02") && (day2.toUInt() > 29))){
              return false;
          }
          if ((month2.toUInt() == 0) || (month2.toUInt() > 12)){
               return false;
          }
          if ((year2.toUInt() < 2020) || (year2.toUInt() > 2030)){
              return false;
          }
          if (year.toUInt() > year2.toUInt()){
              return false;
          }
          if (year == year2){
              if (month > month2){
                  return false;
              }
              if (month == month2){
                  if (day > day2){
                      return false;
                  }
              }
          }
          return true;
    }
    return false;
}

bool rooms_available (const QString& id, QSqlQuery* query){
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            QString available_rooms = query->value("available_rooms").toString();
            if (available_rooms == "0"){
                return false;
            } else{
                return true;
            }
        }
    }
    return true;

}

int checkDataReservation(const QString& id, const QString& id_hotel, const QString& id_client, const QString& dates, QSqlQuery* queryReservation,  QSqlQuery* queryTours,  QSqlQuery* queryClients, int checkMode){
//ПРИ UPDATE ЕСЛИ ПОЛЕ ПУСТО МОЖНО НЕ ДЕЛАТЬ ПРОВЕРКУ
    if (checkMode == 1){
       if ((id.isEmpty()) ||(id_hotel.isEmpty()) || (id_client.isEmpty()) || (dates.isEmpty())){
           return 1; //код ошибки, что есть незаполненные поля
       }
    }

    if (!id.toUInt()){
        return 2; //код ошикби, что введены некорректные данные id
    }
    if (checkMode == 1){
        if (already_exist(id, queryReservation, 3)){
            return 3; //код ошибки, что такой id уже существует
        }
    }
    if (!id_hotel.toUInt()){
        return 4; //код ошикби, что введены некорректные данные id_hotel
    }
    if (!already_exist(id_hotel, queryTours, 1)){
        return 5; //код ошибки, что тура с таким id нет
    }
    if (!id_client.toUInt()){
        return 6; //код ошикби, что введены некорректные данные id_client
    }
    if (!already_exist(id_client, queryClients, 2)){
        return 7; //код ошибки, что клиента с таким id нет
    }
    if (!valid_dates(dates)){
        return 8; //код ошибки, что дата неправлиьного формата
    }
    if(!rooms_available(id_hotel, queryTours)){
        return 9; //код ошибки, что нет свободных мест для бронирования этого тура
    }
    if ((id.isEmpty()) || ((id_hotel.isEmpty()) && (id_client.isEmpty()) && (dates.isEmpty()))){
        return 10; //код ошибки что не заполнены поля хотя бы 1 кроме id
}
    if (!already_exist(id, queryReservation, 3)){
        return 11; //код ошибки, что такого id нет, невозможно изменить поля
    }

return 0;
}

void updateClients_whenAddReservation(const QString& id, const QString& newCurrentTourId, QSqlQuery* query){

    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            QString tour_id = query->value("current_tour_id").toString();
            if (tour_id =="0"){
                tour_id = newCurrentTourId;
            } else{
                tour_id = tour_id + ", " + newCurrentTourId;
            }

            QString score = query->value("tours_score").toString();
            int newScore = score.toInt() + 1;
            std::string strScore = std::to_string(newScore);
            score = toQString(strScore);


            query->prepare("UPDATE Clients SET current_tour_id = '"+tour_id+"', tours_score = '"+score+"' where id = '"+id+"'");
            if (query ->exec()){
                qDebug()<<"clients updated!!!";
            } else {
                qDebug()<<"clients not updated!!!!!!!";
            }
            return;
        }
    }

}


void updateTours_whenAddReservation(const QString& id, QSqlQuery* query){
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            QString available_rooms = query->value("available_rooms").toString();
            int newRooms = available_rooms.toInt() - 1;
            std::string strRooms = std::to_string(newRooms);
            available_rooms = toQString(strRooms);

            query->prepare("UPDATE Tours SET available_rooms = '"+available_rooms+"' where id = '"+id+"'");
            if (query ->exec()){
                qDebug()<<"tours updated!!!";
            } else {
                qDebug()<<"tours not updated!!!!!!!";
            }
            return;
        }
    }
}


QString getIdTour(const QString& id, QSqlQuery* query){
    QString id2;
    query->exec("SELECT * FROM Reservations");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("id_hotel").toString();
        }
    }
    return id2;
}

void  updateTours_whenUpdateReservation(const QString& old_id_hotel, const QString& id_hotel, QSqlQuery* query){

    //прибавлем available_rooms у old_id_hotel
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == old_id_hotel){
            QString available_rooms = query->value("available_rooms").toString();
            int newRooms = available_rooms.toInt() + 1;
            std::string strRooms = std::to_string(newRooms);
            available_rooms = toQString(strRooms);
            query->prepare("UPDATE Tours SET available_rooms = '"+available_rooms+"' where id = '"+old_id_hotel+"'");
            if (query ->exec()){
                qDebug()<<"tours updated!!!";
                break;

            } else {
                qDebug()<<"tours not updated!!!!!!!";
                break;
            }
        }
    }

    //уменьшаем available_rooms у id_hotel
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id_hotel){
            QString available_rooms = query->value("available_rooms").toString();
            int newRooms = available_rooms.toInt() - 1;
            std::string strRooms = std::to_string(newRooms);
            available_rooms = toQString(strRooms);
            query->prepare("UPDATE Tours SET available_rooms = '"+available_rooms+"' where id = '"+id_hotel+"'");
            if (query ->exec()){
                qDebug()<<"tours updated!!!";
                break;

            } else {
                qDebug()<<"tours not updated!!!!!!!";
                break;
            }
        }
    }
}

void decreaseScore(const QString& id, QSqlQuery* query){

    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId ==id){
            QString score = query->value("tours_score").toString();
            int newRooms = score.toInt() - 1;
            std::string strScores = std::to_string(newRooms);
            score = toQString(strScores);
            query->prepare("UPDATE Clients SET tours_score = '"+score+"' where id = '"+id+"'");
            if (query ->exec()){
                qDebug()<<"clients updated!!!";
                break;

            } else {
                qDebug()<<"clients not updated!!!!!!!";
                break;
            }
        }
    }
}

QString getIdClient(const QString& id, QSqlQuery* query){
    QString id2;
    query->exec("SELECT * FROM Reservations");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("id_client").toString();
        }
    }
    return id2;
}

QString getIdHotel(const QString& id, QSqlQuery* query){
    QString id2;
    query->exec("SELECT * FROM Reservations");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("id_hotel").toString();
        }
    }
    return id2;
}

void erasePartStr(QString& old_tour_id, QString newTourId){ //убирает в списке нескольких туров newTourId: из 12, 343, 65 сделает 12, 65 где newTourId = 343
    QString newStr;
    int first = 0;
    int last = 0;
    int similarN = 0;

    for (int i = 0; i < old_tour_id.length(); ++i) {
        if (old_tour_id[i] == newTourId[similarN]) {
            if (similarN == 0) {
                first = i;
            }
            if (similarN == (newTourId.length() - 1)) {
                if (i == (old_tour_id.length() - 1)) {
                    last = first + similarN;
                    if (first != 0) {
                        first -= 2;
                    }
                    break;

                }
                else {
                    if (old_tour_id[i + 1] == ',') { //если несколько туров: 12, 343, 21 убираем запятую и пробел поэтому +2
                        last = first + similarN + 2;
                        break;
                    }
                    else {
                        similarN = 0;
                        continue;
                    }
                }
            }
            ++similarN;
        }
        else {
            similarN = 0;
        }
    }

    for (int i = 0; i < old_tour_id.length(); ++i) {
        if ((i < first) || (i > last)) {
            newStr += old_tour_id[i];
        }
    }

    old_tour_id = newStr;
}

void updateClients_whenUpdateReservation(const QString& old_id_client, const QString& id_client, const QString& newTourId, QSqlQuery* query){
    QString emptyStr = "0";
    //уберем current_tour у old_id_client
    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == old_id_client){
            QString old_tour_id = query->value("current_tour_id").toString();
            if (old_tour_id == newTourId){ //если это единственный тур

                query->prepare("UPDATE Clients SET current_tour_id = '"+emptyStr+"' where id = '"+old_id_client+"'");
                if (query ->exec()){
                    qDebug()<<"Clients updated!!!";
                    break;

                } else {
                    qDebug()<<"Clients not updated!!!!!!!";
                    break;
                }

            } else {
                erasePartStr(old_tour_id, newTourId);
                query->prepare("UPDATE Clients SET current_tour_id = '"+old_tour_id+"' where id = '"+old_id_client+"'");
                if (query ->exec()){
                    qDebug()<<"Clients updated!!!";
                    break;

                } else {
                    qDebug()<<"Clients not updated!!!!!!!";
                    break;
                }
            }
        }
    }

    //добавим current_tour у id_client и добавим score к id_client

    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id_client){
            QString tour_id = query->value("current_tour_id").toString();
            if (tour_id == "0"){
                tour_id = newTourId;
            } else {
                tour_id += ", ";
                tour_id += newTourId;
            }
            /*
            QString score = query->value("tours_score").toString();
            int newScores = score.toInt() + 1;
            std::string strRooms = std::to_string(newScores);
            score = toQString(strRooms);*/ //добавление баллов при изменении не требуется

            query->prepare("UPDATE Clients SET current_tour_id = '"+tour_id+"' where id = '"+id_client+"'");
            if (query ->exec()){
                qDebug()<<"Clients updated!!!";
                break;

            } else {
                qDebug()<<"Clients not updated!!!!!!!";
                break;
            }
        }
    }
}

void updateClient_whenDeleteReservation(const QString& id_client, const QString& reservation_id, QSqlQuery* query){
    QString emptyStr = "0";
    //уберем current_tour у id_client
    query->exec("SELECT * FROM Clients");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id_client){
            QString old_tour_id = query->value("current_tour_id").toString();
            if (old_tour_id == reservation_id){ //если это единственная бронь

                query->prepare("UPDATE Clients SET current_tour_id = '"+emptyStr+"' where id = '"+id_client+"'");
                if (query ->exec()){
                    qDebug()<<"Clients updated!!!";
                    break;

                } else {
                    qDebug()<<"Clients not updated!!!!!!!";
                    break;
                }

            } else {
                erasePartStr(old_tour_id, reservation_id);
                query->prepare("UPDATE Clients SET current_tour_id = '"+old_tour_id+"' where id = '"+id_client+"'");
                if (query ->exec()){
                    qDebug()<<"Clients updated!!!";
                    break;

                } else {
                    qDebug()<<"Clients not updated!!!!!!!";
                    break;
                }
            }
        }
    }
}

void updateTour_whenDeleteReservation(const QString& id_hotel, QSqlQuery* query){
    //прибавлем available_rooms у id_hotel
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id_hotel){
            QString available_rooms = query->value("available_rooms").toString();
            int newRooms = available_rooms.toInt() + 1;
            std::string strRooms = std::to_string(newRooms);
            available_rooms = toQString(strRooms);
            query->prepare("UPDATE Tours SET available_rooms = '"+available_rooms+"' where id = '"+id_hotel+"'");
            if (query ->exec()){
                qDebug()<<"tours updated!!!";
                break;

            } else {
                qDebug()<<"tours not updated!!!!!!!";
                break;
            }
        }
    }
}

double getMultiplier(int percent, QSqlQuery* queryMultipliers){
    queryMultipliers->exec("SELECT * FROM Multiplier");
    while (queryMultipliers->next()){
        int bdPercent = queryMultipliers->value("percent").toInt();
        if (bdPercent == percent){
            return queryMultipliers->value("multiplier").toDouble();
        }
    }
    return 1;
}


QString getPrice (const QString& id_hotel,QSqlQuery* queryTours_, QSqlQuery* queryMultipliers){
    QString price, available_rooms, all_rooms;
    std::string myPrice;
    double k = 1;
    queryTours_->exec("SELECT * FROM Tours");
    while (queryTours_->next()){
        QString bdId = queryTours_->value("id").toString();
        if (bdId == id_hotel){
            price = queryTours_->value("price").toString();
            available_rooms = queryTours_->value("available_rooms").toString();
            all_rooms = queryTours_->value("amount_rooms").toString();
            break;
        }
    }
    qDebug()<<"available ="<<available_rooms;
    double percent = (all_rooms.toDouble()-available_rooms.toDouble())/all_rooms.toDouble()*100;
    qDebug()<<"percent ="<<percent;
    if (percent >= 50) {
        if (percent >= 60){
            if (percent >= 70){
                if (percent >= 80){
                    if (percent >= 90){
                        k = getMultiplier(90, queryMultipliers); //если больше 90
                    } else{
                        k = getMultiplier(80, queryMultipliers); //если больше 80 но меньше 90
                    }
                } else{
                    k = getMultiplier(70, queryMultipliers); //если больше 70 но меньше 80
                }
            } else{
                k = getMultiplier(60, queryMultipliers); //если больше 60 но меньше 70
            }
        } else{
            k = getMultiplier(50, queryMultipliers); //если больше 50 но меньше 60
        }
    }
    int totalPrice = k*price.toInt();
    myPrice = std::to_string(totalPrice);
    price = toQString(myPrice);
    qDebug()<<"price_per_day = "<<price;
    return price;
}



QString calculcatePrice(const QString& dates, const QString& price_per_day){
    QString day, month, year, day2, month2, year2;
    int difference = 0;
    day+=dates[0];
    day+=dates[1];
    month += dates[3];
    month += dates[4];
    year += dates[6];
    year += dates[7];
    year += dates[8];
    year += dates[9];

    day2+=dates[11];
    day2+=dates[12];
    month2 += dates[14];
    month2 += dates[15];
    year2 += dates[17];
    year2 += dates[18];
    year2 += dates[19];
    year2 += dates[20];

    struct std::tm a = { 0,0,0,day.toInt(), month.toInt() - 1, year.toInt() - 1900 }; /* June 24, 2004 */ //сохраняем 1-ую дату
    struct std::tm b = { 0,0,0,day2.toInt(), month2.toInt() - 1, year2.toInt() - 1900 }; /* July 5, 2004 */  //сохраняем 2-ую дату
    std::time_t x = std::mktime(&a);
    std::time_t y = std::mktime(&b);
    if (x != (std::time_t)(-1) && y != (std::time_t)(-1)){
          difference = std::difftime(y, x) / (60 * 60 * 24) + 1;
           }
    difference *= price_per_day.toInt();
    std::string str = std::to_string(difference);
    qDebug()<<toQString(str);
    return toQString(str);
}

#endif // RESERVATIONSFUNCTIONS_H
