#ifndef GENERATEWAITING_H
#define GENERATEWAITING_H

#include <QDialog>

namespace Ui {
class GenerateWaiting;
}

class GenerateWaiting : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateWaiting(QWidget *parent = nullptr);
    ~GenerateWaiting();

private:
    Ui::GenerateWaiting *ui;
};

#endif // GENERATEWAITING_H
