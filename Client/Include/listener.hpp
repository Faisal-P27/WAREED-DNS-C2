#ifndef LISTENER_H
#define LISTENER_H

#include <QWidget>
#include "global.hpp"
namespace Ui {
class Listener;
}

class Listener : public QWidget
{
    Q_OBJECT

public:
    explicit Listener(QWidget *parent = nullptr);
    ~Listener();

    void AddNewCell(int row, int column, QString item);
    void AddNewListener(ListenerInfo *NewListener);
    void UpdateListenerHits(ListenerInfo *HListener, QString Hits);
    void DelListener(ListenerInfo *DelListener);
private slots:
    void on_pushButton_AddListener_clicked();

    void on_pushButton_RemoveListener_clicked();

private:
    Ui::Listener *ui;
};

#endif // LISTENER_H
