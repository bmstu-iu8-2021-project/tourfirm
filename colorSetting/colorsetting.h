#ifndef COLORSETTING_H
#define COLORSETTING_H

#include <QMainWindow>

namespace Ui {
class colorSetting;
}

class colorSetting : public QMainWindow
{
    Q_OBJECT

public:
    explicit colorSetting(QWidget *parent = nullptr);
    ~colorSetting();

    void setBackground(QString background){
        setStyleSheet(background);
        currentBackground = background;
    };

    void setButtonColor (QString buttonColor){
        currentButtonColor = buttonColor;
    }


signals:
    void openMenu();
    void giveTextColor(QString newTextColor);

private slots:

    void on_open_menu_clicked();

    void on_white_clicked();

    void on_brown_clicked();

    void on_black_clicked();

    void on_blue_clicked();

    void on_green_clicked();

    void on_red_clicked();

    void on_grey_clicked();

    void on_purple_clicked();

    void on_orange_clicked();

private:
    Ui::colorSetting *ui;
    QString currentBackground;
    QString currentButtonColor;
};

#endif // COLORSETTING_H
