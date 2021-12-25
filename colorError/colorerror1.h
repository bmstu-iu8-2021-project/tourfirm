#ifndef COLORERROR1_H
#define COLORERROR1_H

#include <QDialog>

namespace Ui {
class colorError1;
}

class colorError1 : public QDialog
{
    Q_OBJECT

public:
    explicit colorError1(QWidget *parent = nullptr);
    ~colorError1();

private slots:
    void on_pushButton_clicked();

private:
    Ui::colorError1 *ui;
};

#endif // COLORERROR1_H
