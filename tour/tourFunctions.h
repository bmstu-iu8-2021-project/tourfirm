#pragma once
#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H

#include <QString>
#include <QSqlQuery>
#include <QMessageBox>

bool checkSymbols(const QString& qstr){
    std::string str = qstr.toStdString();
    for (size_t i = 0; i < str.length(); ++i){
        if (!(((str[i] > 47) && (str[i] <58)) || ((str[i] > 64) && (str[i] < 91)) || ((str[i] > 96) && (str[i] <123)) || (str[i] == 32))){ //разрешаем цифры (если тезка будет проще различить не только по id, заглавные и строчные английские буквы
            return false;
        }
    }
    return true;
}


bool is_unique(const QString& id, QSqlQuery* query){ //проверяем уникальность нового id в бд
    if (!id.toUInt()){
        return false; //введены некорректные данные id
    }
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return  false;
        }
    }
    return true;
}

QString getAmountRooms(const QString& id, QSqlQuery* query){
    QString amount_rooms;
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("amount_rooms").toString();
        }
    }
    return amount_rooms;
}

QString  getAvailableRooms (const QString& id, QSqlQuery* query){
    QString available_rooms;
    query->exec("SELECT * FROM Tours");
    while (query->next()){
        QString bdId = query->value("id").toString();
        if (bdId == id){
            return query->value("available_rooms").toString();
        }
    }
    return available_rooms;
}

QString makeQString(const std::string& str){
    QString qStr;
    for (size_t i = 0; i<str.length(); ++i){
        qStr += str[i];
    }
    return qStr;
}

int checkDataTour(const QString& id, const QString& name, const QString& availableRooms, const QString& amountRooms, const QString& price, const QString& stars, const QString& charter, QSqlQuery* query, int checkMode){

    if (checkMode == 1){
        if ((id.isEmpty()) ||(name.isEmpty()) || (availableRooms.isEmpty()) || (amountRooms.isEmpty()) || (price.isEmpty()) || (stars.isEmpty()) || (charter.isEmpty())){
            return 1; //код ошибки, что есть незаполненные поля
        }
    }

    if ((!id.toUInt()) || (id.isEmpty())){
        return 2; //код ошикби, что введены некорректные данные id
    }
    if (checkMode == 1){ //эта проверка нужна в save, но не в update
       if (!is_unique(id, query)){
           return 3; //код ошибки, что такой id уже существует
       }
    }
    if ((!amountRooms.isEmpty()) && (!amountRooms.toUInt())){
            return 5; //код ошибки, что введены некорректные данные общего количества комнат
    }
    if (checkMode == 2){
        if (!amountRooms.isEmpty()){
            QString old_amount_rooms = getAmountRooms(id, query);
            QString old_available_rooms = getAvailableRooms (id, query);
            int amountReservations = old_amount_rooms.toInt() - old_available_rooms.toInt();
            if (amountRooms.toInt() < amountReservations){
                return 4; //ошибка, что новое значение общего числа комнат превышает количество текущих бронирований этого тура
            }
        }
    }
    if (checkMode == 1){
        if (amountRooms.toUInt() < availableRooms.toUInt()){
            return 6; //код ошибки, что не может количество доступных комнат быть больше числа всех комнат
        }
    }
    if ((!price.toUInt()) && (!price.isEmpty())){
        return 7; //код ошибки, что введены некорректные данные цены
    }
    if ((!stars.toUInt())  && (!stars.isEmpty())){
        return 8; //код ошибки, что введены некорректные данные количества звезд у тура
    }
    if ((stars.toUInt() > 5) && (!stars.isEmpty())){
        return 9; //код ошибки, что количество звезд у отеля не може превышать 5
    }
    if ((!checkSymbols(name)) && (!name.isEmpty())){
        return 13; //код ошики, что есть запрещенные символы для поля name
    }
    if ((!((charter == "0") || (charter == "1"))) && (!charter.isEmpty())){
             return 10; //код ошибки что введены некорректные данные о чартере
    }

    //корректность для изменения данных:
    if (is_unique(id, query)){
        return 11; //код ошибки, что такой id не существует (невозмоожно изменить ланные с таким id)
    }
    if ((name.isEmpty()) && (amountRooms.isEmpty()) && (price.isEmpty()) && (stars.isEmpty()) && (charter.isEmpty()) && (!availableRooms.isEmpty())) {
        return 12; //код ошибки, что поля не заполнены
    }


   return 0; //все данные введены корректно
}

void updateNameInReservations(const QString& id_hotel, const QString& name_hotel, QSqlQuery* queryReservations_){
    queryReservations_->prepare("UPDATE Reservations SET name_hotel = '"+name_hotel+"' where id_hotel ='"+id_hotel+"'");
    if (queryReservations_->exec()){
    qDebug()<<"reservations updated!";
    } else {
         qDebug()<<"reservations NOT updated!";
    }
}

void erasePart(QString& old_tour_id, QString newTourId){ //убирает в списке нескольких туров newTourId: из 12, 343, 65 сделает 12, 65 где newTourId = 343
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

void updateClient (const QString id_client, const QString& reservation_id, QSqlQuery* query){
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
                erasePart(old_tour_id, reservation_id);
                query->prepare("UPDATE Clients SET current_tour_id = '"+old_tour_id+"' where id = '"+old_tour_id+"'");
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

void updateClientsReservations_whenDeleteTour(const QString& id_hotel,QSqlQuery* queryClients_,QSqlQuery* queryReservations_){
    queryReservations_->exec("SELECT * FROM Reservations");
    while (queryReservations_->next()){
        QString bdIdHotel = queryReservations_->value("id_hotel").toString();
        if (bdIdHotel == id_hotel){
            updateClient(queryReservations_->value("id_client").toString(), queryReservations_->value("id").toString(), queryClients_); //убираем currentTour
            queryReservations_->prepare("delete from Reservations where id_hotel = '"+id_hotel+"'");
            if(queryReservations_->exec()){
                qDebug()<<"reservation deleted!";
            } else {
                qDebug()<<"reservation NOT deleted!";
            }
        }
    }
}


#endif // MYFUNCTIONS_H
