#ifndef BUTTONCOLOR_H
#define BUTTONCOLOR_H

#include <QMainWindow>

namespace Ui {
class buttoncolor;
}

class buttoncolor : public QMainWindow
{
    Q_OBJECT

public:
    explicit buttoncolor(QWidget *parent = nullptr);
    ~buttoncolor();

    void setBackground(QString background){
        setStyleSheet(background);
    };

    void setColor(QString color){
        currentColor = color;
    };


signals:
    void openMenu();
    void giveButtonColor(QString buttonColor);

private slots:

    void on_black_clicked();

    void on_white_clicked();

    void on_blue_clicked();

    void on_brown_clicked();

    void on_lightblue_clicked();

    void on_grey_clicked();

    void on_open_menu_clicked();

    void on_yellow_clicked();

    void on_purple_clicked();

    void on_green_clicked();


private:
    Ui::buttoncolor *ui;
    QString currentColor;
};

#endif // BUTTONCOLOR_H
