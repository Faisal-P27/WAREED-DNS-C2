#ifndef GLOBAL_HPP
#define GLOBAL_HPP






#include <QApplication>
#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QAction>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSpacerItem>
#include <QTabWidget>
#include <QWidget>
#include <QDialog>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include "QTime"
#include <QStringList>
#include <QFileDialog>
#include "QScreen"
#include "QCompleter"
#include "QDesktopWidget"
#include <string>
#include <map>
#include <iostream>
#include <any>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#define DPRINT(...)  qDebug(__VA_ARGS__)
using namespace std;

typedef struct Head
   {
       int Event;

       string User;
       string SessionID;
       string Time;
       string OneTime;
   } Head_t;

   typedef struct Body
   {
       int SubEvent;
      // QMap<string, string> Info; // TODO: make it QJsonObject
       QJsonObject Info;
   } Body_t ;

   typedef struct _Package
   {
       Head_t Head;
       Body_t Body;
   } Package, *PPackage;


typedef struct _ConnectionInfo
{
    QString SessionID;
    QString Host;
    QString Port;
    QString User;
    QString Password;
    bool Status;

} ConnectionInfo;


typedef struct _TargetSession
{

    QString ID;
    QString UserName;
    QString Computer;
    QString OS;
    QString PID;
    QString LastPing;
    qint64  LastPing1970;
    QString Privilege;



}TargetSession;



typedef struct _TargetInfo
{

    TargetSession* TargetData;
    class interactTab* UITabInteract;


}TargetInfo;

typedef struct _ListenerInfo{
    QString     DomainName;
    QString     Protocol;
    QString     BindIP;
    QString     BindPort;
    QString     Status;
    QString     Hits;

}ListenerInfo;


typedef struct Command
{
    QString     CommandString;
    QString     Description;
    QString     Usage;
    QString     Example;
    int RequiredArgs;

} Command_t;



extern QMap<QString,TargetInfo*> TargetInfoMap;
extern QMap<QString,ListenerInfo*> ListenerMap;

//UI
extern class Login* UILogin ;
extern class AtherMainWindow* UIMainWindow;
extern class AddListener* UIAddListener;
extern class GeneratePayload* UIGeneratePayload;
extern class SetListener* UISetListener;
extern class C2Config* UIC2Config;
extern class about* UIAbout;
extern class GenerateWaiting* UIGenerateWaiting;

//APP
extern QApplication* AtherApp;

//Connection
extern ConnectionInfo* Teamserver;
extern class Connector* Connect;






std::string gen_random(const int len);
size_t Hash(std::string str) ;




inline void CenterWidgets(QWidget *widget, QWidget *host = Q_NULLPTR) {
    if (!host)
        host = widget->parentWidget();

    if (host) {
        auto hostRect = host->geometry();
        widget->move(hostRect.center() - widget->rect().center());
    }
    else {
        QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
        int x = (screenGeometry.width() - widget->width()) / 2;
        int y = (screenGeometry.height() - widget->height()) / 2;
        widget->move(x, y);
    }
}


#endif // GLOBAL_HPP
