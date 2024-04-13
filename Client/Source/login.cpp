#include "login.hpp"
#include "ui_login.h"
#include "Include/helper/wmessagebox.hpp"
#include "Include/transport/connector.hpp"

Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    this->WLWindow=parent;
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ConnectingMessge=new QMessageBox();
    ConnectingMessge->setWindowTitle("Login");
    ConnectingMessge->setText("Connecting");
    ConnectingMessge->setInformativeText("Connecting To TeamServer...");
    ConnectingMessge->setIcon(QMessageBox::Information);
    ConnectingMessge->setStandardButtons(QMessageBox::Cancel );
    ConnectingMessge->setDefaultButton(QMessageBox::Cancel);
    QDesktopWidget *desktop = QApplication::desktop();

    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;

    screenWidth = desktop->width(); // get width of screen
    screenHeight = desktop->height(); // get height of screen

    windowSize = size(); // size of our application window
    width = windowSize.width();
    height = windowSize.height();

    // little computations
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;

    // move window to desired coordinates
    move ( x, y );

}

Login::~Login()
{
    delete ui;
}



void Login::mousePressEvent(QMouseEvent *event)
{

    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}






void Login::Run()
{
    this->WLWindow->show();
}

void Login::CloseLoginWindow()
{
    this->close();
}

void Login::on_lineEditUserName_returnPressed()
{
    on_pushButtonConnect_clicked();
}


void Login::on_lineEditPassword_returnPressed()
{
    on_pushButtonConnect_clicked();
}


void Login::on_lineEditIPAddress_returnPressed()
{
    on_pushButtonConnect_clicked();
}


void Login::on_lineEdirPort_returnPressed()
{
    on_pushButtonConnect_clicked();
}


void Login::on_pushButtonConnect_clicked()
{
        if(ui->lineEditIPAddress->text().isEmpty() || ui->lineEdirPort->text().isEmpty()  || ui->lineEditUserName->text().isEmpty()  || ui->lineEditPassword->text().isEmpty() ){
            WMessageBox::CriticalMessage("Login","Error","Fill Entry");
        }else{
            auto ConnectionInfo = new _ConnectionInfo();
            ConnectionInfo->Host=ui->lineEditIPAddress->text();
            ConnectionInfo->Port=ui->lineEdirPort->text();
            ConnectionInfo->SessionID="";
            ConnectionInfo->User=ui->lineEditUserName->text();
            ConnectionInfo->Password=ui->lineEditPassword->text();
            ConnectionInfo->Status=false;
            //Connect= new Wareed::Connector( ConnectionInfo );
            auto ConnectionInstant = new Connector( ConnectionInfo );
            Connect=ConnectionInstant;
            Teamserver=ConnectionInfo;
            ConnectingMessge->exec();
    }

}


void Login::on_pushButtonQuit_clicked()
{
    QApplication::quit();
}

