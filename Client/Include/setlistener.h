#ifndef SETLISTENER_H
#define SETLISTENER_H

#include <QDialog>
#include "global.hpp"
namespace Ui {
class SetListener;
}

class SetListener : public QDialog
{
    Q_OBJECT

public:
    explicit SetListener(QWidget *parent = nullptr);
    ~SetListener();

    void ShowSetListener();
    QString GetSelectedComboBox();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SetListener *ui;
};

#endif // SETLISTENER_H
