#include "addlistener.hpp"
#include "ui_addlistener.h"
#include "Include/helper/wmessagebox.hpp"
#include "Include/command/command.h"
AddListener::AddListener(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddListener)
{
    ui->setupUi(this);
    CenterWidgets(parent);
}
QRegularExpression domainNamePattern("^(?!-)[A-Za-z0-9-]{1,63}(?<!-)(\\.[A-Za-z0-9-]{1,63})*$");
AddListener::~AddListener()
{

    delete ui;
}

int NewListener=120;

void AddListener::AddCallBackDomain(QString DomainName)
{
    QJsonObject Info;
    Info.insert("DomainName", DomainName);

    Body_t Body;
    Body.SubEvent = NewListener;
    Body.Info = Info; // Directly assigning QJsonObject to Info

    NewPackageCommand(Teamserver->User, Body);
}


void AddListener::on_buttonBox_accepted()
{
    QString DomainName=ui->DomainName_lineEdit->text();
    QRegularExpressionMatch match = domainNamePattern.match(DomainName);
    if (!match.hasMatch()) {
        WMessageBox::CriticalMessage("Add Listener","ERROR","Domain Not Valid");
        return;
    }
    auto ListenerName=ListenerMap[DomainName];
    if (ListenerName) {
        WMessageBox::CriticalMessage("Add Listener","ERROR","Domain Already Added!");
        return;
    }
    ui->DomainName_lineEdit->clear();
    this->hide();
    AddCallBackDomain(DomainName);
    qDebug() << "valid domain name";
}

