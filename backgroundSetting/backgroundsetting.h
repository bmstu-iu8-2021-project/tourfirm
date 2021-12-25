#ifndef BACKGROUNDSETTING_H
#define BACKGROUNDSETTING_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class backgroundSetting;
}

class backgroundSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit backgroundSetting(QWidget *parent = nullptr);

    ~backgroundSetting();

    void setColor(QString color){
        currentColor = color;
    };

signals:
    void openMenu();
    void giveBackground(QString backgroundColor);


private slots:
    void on_pushButton_clicked();

    void on_lightblue_clicked();

    void on_blue_clicked();

    void on_black_clicked();

    void on_white_clicked();

    void on_orange_clicked();

    void on_purple_clicked();

    void on_green_clicked();

    void on_yellow_clicked();

    void on_grey_clicked();

private:
    Ui::backgroundSetting *ui;
    QString currentColor;

};

#endif // BACKGROUNDSETTING_H
