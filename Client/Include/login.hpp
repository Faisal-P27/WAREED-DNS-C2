#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMouseEvent>
#include "global.hpp"
namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    void Run();
    QWidget* WLWindow;
    QMessageBox* ConnectingMessge;
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_lineEditUserName_returnPressed();

    void on_lineEditPassword_returnPressed();

    void on_lineEditIPAddress_returnPressed();

    void on_lineEdirPort_returnPressed();

    void on_pushButtonConnect_clicked();

    void on_pushButtonQuit_clicked();

private:
    void CloseLoginWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    Ui::Login *ui;
};

#endif // LOGIN_H
