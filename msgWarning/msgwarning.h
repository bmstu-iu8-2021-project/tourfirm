#ifndef MSGWARNING_H
#define MSGWARNING_H

#include <QDialog>

namespace Ui {
class msgWarning;
}

class msgWarning : public QDialog
{
    Q_OBJECT

public:
    explicit msgWarning(QWidget *parent = nullptr);
    ~msgWarning();

private slots:
    void on_pushButton_clicked();

private:
    Ui::msgWarning *ui;
};

#endif // MSGWARNING_H
