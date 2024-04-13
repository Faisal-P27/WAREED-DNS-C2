#ifndef COMMAND_HPP
#define COMMAND_HPP
#include "global.hpp"
#include "interacttab.hpp"


//class command;
 extern QMap<QString, QString> CommandInputList;

 extern std::vector<Command_t> CommandList;
auto NewPackageCommand(const QString &TeamserverName, Body_t Body) -> void;

 enum class CommandsCode {
     SHELL                   = 1,
     INJECT_SHELLCODE        = 2,
     PROXY_START             = 3,
     PROXY_STOP              = 4,
     ADD_DOMAIN              = 5,
     DELETE_DOMAIN            = 6,
     LIST_DOMAIN              = 7,
     SELF_KILL                 = 12,
     SELF_WIPE                 = 13,
     SELF_DEL                = 14,
     TASK_LIST                = 15,
     TASK_DEL                = 16,
     TASKS_CLEAN                = 17,
     SLEEP                   = 11,
     EXIT                    = 99,

 };

class CommandExecute
{
public:
    class command* CommandsInstance;
    void InitialiseBody(QString TaskID,CommandsCode CommandID,Body_t &Body);
    auto ProcList( QString TaskID, bool FromProcessManager ) -> void;
    auto ProcModule( QString TaskID, int SubCommand, QString Args ) -> void;
    auto Sleep( QString TaskID, QString seconds ) -> void;
    auto ShellcodeInject(QString TaskID, QString TargetPID, QString Path) -> void;
    auto ProxyStart(QString TaskID, QString CallBackHostName, QString AgentConnectPort, QString ProxyListenerPort)-> void;
    auto ProxyStop(QString TaskID, QString CallBackHostName, QString AgentConnectPort, QString ProxyListenerPort)-> void;


    CommandExecute();
    void AddCallBackDomain(QString TaskID, QString CallBackDomain);
    void DeleteCallBackDomain(QString TaskID, QString NewCallBackDomain);
    void ListCallBackDomain(QString TaskID);

    void SelfKill(QString TaskID);
    void SelfWipe(QString TaskID);
    void SelfDel(QString TaskID);
    void TaskList(QString TaskID);
    void TasksClean(QString TaskID);
    void TaskDel(QString TaskID, QString DelTaskID);
};




class command

{
public:
    command();
    CommandExecute  Execute;
    //static std::vector<Command_t>   CommandList;
    class interactTab* TabInteract;
    TargetInfo*     Target;
    auto DispatchCommand( bool Send, QString TaskID, const QString& commandline ) -> bool;
    bool DispatchShellCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchSleepCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchInjectCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchProxyStartCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchProxyStopCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchAddDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchDeleteDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchListDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchHelpCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchSelfKillCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchWipeCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchSelfDelCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchTaskListCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchTasksCleanCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchTaskDelCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
    bool DispatchClearCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands);
};


#endif // COMMAND_HPP
