#ifndef MULTIPLIERSFUNCTIONS_H
#define MULTIPLIERSFUNCTIONS_H

#include <QSqlQuery>
#include <QString>




int checkInputData(const QString& percent, const QString& koef, QSqlQuery* queryMultipliers_){

 if((percent.isEmpty()) || (koef.isEmpty())){
     return 1; //ошибка, что есть пустые поля
 }

 if ((percent != "50") && (percent != "60") && (percent != "70") && (percent != "80") && (percent != "90")) {
     return 2; //ошибка неверный percent
 }
 if (!koef.toDouble()){
     return 3; //ошибка неверный множитель
 }
 if (koef.toDouble() < 1){
     return 4; //множитель должен быть болье 1
 }

 //проверка что по возрастанию все 5 коэффициентов
 std::vector<double> koefs;
     queryMultipliers_->exec("SELECT * FROM Multiplier");
 while (queryMultipliers_->next()){
     QString currentPercent = queryMultipliers_->value("percent").toString();
     if (currentPercent == percent){
         koefs.push_back(koef.toDouble());
     } else{
         koefs.push_back(queryMultipliers_->value("multiplier").toDouble());
     }
 }

 for (size_t i = 0; i < koefs.size() - 1; ++i){
     if (koefs[i] > koefs[i+1]){
         return 5;
     }
 }
 if (koef.toDouble() > 10){
     return 6;
 }


 return 0;
}

#endif // MULTIPLIERSFUNCTIONS_H
