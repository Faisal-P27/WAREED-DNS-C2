#include "athermainwindow.hpp"

#include <QApplication>
#include "Include/global.hpp"
#include "login.hpp"
#include "addlistener.hpp"
#include "generatepayload.h"
#include "setlistener.h"
#include "c2config.h"
#include "about.h"
#include "generatewaiting.h"
int main(int argc, char** argv )
{

    AtherApp  = new QApplication(argc, argv);
    AtherApp->setQuitOnLastWindowClosed(false);
    QString AppDir = qApp->applicationDirPath();

    UIMainWindow=new AtherMainWindow(new QWidget);
    UILogin=new Login(new QWidget);
    UIMainWindow->Initializer();
    UIAddListener=new AddListener(new QWidget);
    UIGeneratePayload=new GeneratePayload(new QWidget);
    UISetListener=new SetListener(new QWidget);
    UIC2Config=new C2Config(new QWidget);
    UIAbout=new about(new QWidget);
    UIGenerateWaiting=new GenerateWaiting(new QWidget);


    int aExit=AtherApp->exec();
    qDebug("Application Closed");
    return aExit;
}
