#include "transport/packager.hpp"
#include "helper/macro.hpp"
#include "login.hpp"
#include "helper/wmessagebox.hpp"
#include "athermainwindow.hpp"
#include "global.hpp"
#include "interacttab.hpp"
#include "generatepayload.h"
#include "c2config.h"
#include "generatewaiting.h"

//PrintConsole Type

Packager::Packager()
{





}


PPackage Packager::DecodePackage(const QString &Package)
{
    auto FullPackage = new _Package;
    auto JsonData = QJsonDocument::fromJson(Package.toUtf8());

    if (JsonData.isEmpty())
    {
        spdlog::critical("Invalid json");
        return nullptr;
    }

    if (JsonData.isObject())
    {
        auto PackageObject = JsonData.object();

        auto HeadObject = PackageObject["Head"].toObject();
        auto BodyObject = PackageObject["Body"].toObject();

        FullPackage->Head.Event = HeadObject["Event"].toInt();
        FullPackage->Head.Time = HeadObject["Time"].toString().toStdString();
        FullPackage->Head.User = HeadObject["User"].toString().toStdString();

        FullPackage->Body.SubEvent = BodyObject["SubEvent"].toInt();

        if (BodyObject["Info"].isObject())
        {
            FullPackage->Body.Info = BodyObject["Info"].toObject();
        }
    }
    else
    {
        spdlog::critical("Is not an Object: {}", QJsonDocument(JsonData).toJson().toStdString());
    }

    return FullPackage;
}



QJsonDocument Packager::EncodePackage(Package Package)
{
    QJsonObject JsonPackage;
    QJsonObject Head;
    QJsonObject Body;

    Head.insert("Event", Package.Head.Event);
    Head.insert("User", Package.Head.User.c_str());
    Head.insert("SessionID", Package.Head.SessionID.c_str());
    Head.insert("Time", Package.Head.Time.c_str());
    Head.insert("OneTime", Package.Head.OneTime.c_str());

    Body.insert("SubEvent", Package.Body.SubEvent);
    Body.insert("Info", Package.Body.Info); // Directly use QJsonObject

    JsonPackage.insert("Head", Head);
    JsonPackage.insert("Body", Body);

    return QJsonDocument(JsonPackage);
}


auto Packager::DispatchPackage( PPackage Package ) -> bool
{
    qDebug("DispatchPackage");
    switch ( Package->Head.Event )
    {
        case LoginTC:
            qDebug("Package->Head.Event LoginTC OK!");
            return DispatchLoginResult( Package );
        case NewSessionTC:
            qDebug("Package->Head.Event NewSessionTC OK!");
            return DispatchNewTargetSession( Package );
        case PrintConsoleTC:
            qDebug("Package->Head.Event PrintConsoleTC OK!");
            return DispatchPrintConsole( Package );
        case UpdateLastSeenTC:
                 qDebug("Package->Head.Event UpdateLastSeenTC OK!");
                 return DispatchUpdateLastSeen( Package );
        case NewListenerTC:
                 qDebug("Package->Head.Event NewListenerTC OK!");
                 return DispatchNewListener( Package );
        case UpdateListenerHitsTC:
             qDebug("Package->Head.Event UpdateListenerHitsTC OK!");
             return DispatchUpdateListenerHits( Package );

        case MessageBoxTC:
            qDebug("Package->Head.Event MessageBoxTC OK!");
            return DispatchNewMessageBox( Package );
        case NewPayloadTC:
            qDebug("Package->Head.Event NewPayloadTC OK!");
            return DispatchNewPayload( Package );
    case DelListenerTC:
        qDebug("Package->Head.Event DelListener OK!");
        return DispatchDelListener( Package );
    case C2ConfigTC:
        qDebug("Package->Head.Event ConfigTC OK!");
        return DispatchC2ConfigTC( Package );




        default:
            spdlog::info( "[PACKAGE] Event Id not found" );
            return false;
    }
}




bool Packager::DispatchLoginResult(PPackage Package)
{
    qDebug("DispatchLoginResult");
    QJsonObject Info = Package->Body.Info;
    QString SessionID = Info.value("SessionID").toString();

    if (SessionID.isEmpty()){
        qDebug() << "Login Failed";
        qDebug() << "SessionID[" << SessionID << "]";

        WMessageBox::CriticalMessage("Login", "Login Fail", "Password Wrong!");
    } else {
        qDebug() << "Login Good";
        qDebug() << "SessionID[" << SessionID << "]";


        UILogin->ConnectingMessge->close();
        UIMainWindow->SetTeamServerTabName();
        UIMainWindow->show();
        UILogin->hide();
        Teamserver->Status = true;
    }
    return true;
}


bool Packager::DispatchNewTargetSession(PPackage Package)
{
    auto TargetData = new TargetSession();

    QJsonObject Info = Package->Body.Info;

    QString SessionID = Info.value("SessionID").toString();
    QString HostName = Info.value("HostName").toString();
    QString LastPing = Info.value("LastPing").toString();
    qint64 LastPing1970 = Info.value("LastPing1970").toVariant().toLongLong();
    QString OsVersion = Info.value("OS").toString();
    // QString ProcessElevated = Info.value("ProcessElevated").toString();
    QString UserName = Info.value("UserName").toString();
    QString ProcessID = Info.value("ProcessID").toString();
    QString Privilege = Info.value("Privilege").toString();
    if (LastPing1970 == 0) {
        qDebug() << "Invalid LastPing1970 value.";
        delete TargetData;
        return false;
    }

    qint64 CurrentLastPing1970 = QDateTime::currentSecsSinceEpoch();
    qDebug() << "LastPing1970=" << LastPing1970;
    TargetData->LastPing1970 = CurrentLastPing1970 - LastPing1970;

    TargetData->ID = SessionID;
    TargetData->Computer = HostName;
    TargetData->OS = OsVersion;
    TargetData->UserName = UserName;
    TargetData->PID = ProcessID;
    TargetData->Privilege = Privilege;
    TargetData->LastPing = LastPing;

    qDebug() << "SessionID [" << SessionID << "]";
    qDebug() << "HostName [" << HostName << "]";
    qDebug() << "LastPing [" << LastPing << "]";
    qDebug() << "LastPing1970 [" << LastPing1970 << "]";
    qDebug() << "OSVersion [" << OsVersion << "]";
    qDebug() << "UserName [" << UserName << "]";
    qDebug() << "ProcessID [" << ProcessID << "]";

    UIMainWindow->AddNewSession(TargetData);
    qDebug() << "END from DispatchNewTargetSession";
    return true;
}


bool Packager::DispatchNewListener(PPackage Package)
{
    auto NewListener = new ListenerInfo();

    // Assuming Package->Body.Info is a QJsonObject
    QJsonObject Info = Package->Body.Info;

    NewListener->BindIP = Info.value("BindIP").toString();
    NewListener->BindPort = Info.value("BindPort").toString();
    NewListener->DomainName = Info.value("DomainName").toString();
    NewListener->Protocol = Info.value("Protocol").toString();
    NewListener->Hits = Info.value("Hits").toString();
    NewListener->Status = Info.value("Status").toString();

    qDebug() << "NewListener->BindIP [" << NewListener->BindIP << "]";
    qDebug() << "NewListener->BindPort [" << NewListener->BindPort << "]";
    qDebug() << "NewListener->DomainName [" << NewListener->DomainName << "]";
    qDebug() << "NewListener->Protocol [" << NewListener->Protocol << "]";
    qDebug() << "NewListener->Hits [" << NewListener->Hits << "]";
    qDebug() << "NewListener->Status [" << NewListener->Status << "]";

    UIMainWindow->AddNewListenerInfo(NewListener);
    return true; // Using true for clarity and consistency with boolean return types
}



bool Packager::DispatchDelListener(PPackage Package)
{
    auto DelListener = new ListenerInfo();
    QJsonObject Info = Package->Body.Info; // Assuming Info is a QJsonObject

    DelListener->BindIP = Info.value("BindIP").toString();
    DelListener->BindPort = Info.value("BindPort").toString();
    DelListener->DomainName = Info.value("DomainName").toString();
    DelListener->Protocol = Info.value("Protocol").toString();
    DelListener->Hits = Info.value("Hits").toString();
    DelListener->Status = Info.value("Status").toString();

    qDebug() << "DelListener->BindIP [" << DelListener->BindIP << "]";

    UIMainWindow->DelListenerInfo(DelListener);
    return true;
}







bool Packager::DispatchPrintConsole(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;
    QString ConsoleData = Info.value("ConsoleData").toString();
    QString SessionID = Info.value("SessionID").toString();

   auto TypeStr= Info.value("Type").toString();
   bool ok;
      int Type=TypeStr.toInt(&ok);
        if (!ok) {
         return false;
     }

    auto TargetInfo=TargetInfoMap[SessionID.toStdString().c_str()];
    if (!TargetInfo) {
        qDebug("Failed to found Target");
        return false;
    }

    TargetInfo->UITabInteract->TaskResult(false, "", ConsoleData, Type);

    return true;
}


bool Packager::DispatchUpdateLastSeen(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;
    QString SessionID = Info.value("SessionID").toString();
    QString LastSeen = Info.value("LastSeen").toString();

    auto TargetInfo = TargetInfoMap.value(SessionID);
    TargetInfo->TargetData->LastPing1970 = 0;
    TargetInfo->TargetData->LastPing = LastSeen;

    UIMainWindow->CheckSessionTable(TargetInfo);
    return true;
}


bool Packager::DispatchUpdateListenerHits(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;
    QString DomainName = Info.value("DomainName").toString();
    QString Hits = Info.value("Hits").toString();

    auto ListenerHits = ListenerMap.value(DomainName);
    if (!ListenerHits) {
        return false;
    }

    UIMainWindow->UpdateListenerHits(ListenerHits, Hits);

    return true;
}


bool Packager::DispatchNewMessageBox(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;
    QString TextInfo = Info.value("TextInfo").toString();
    QString Text = Info.value("Text").toString();
    QString Type = Info.value("Type").toString();
    QString Title = Info.value("Title").toString();

    if (Type == "Good") {
        WMessageBox::GoodMessage(Title, Text, TextInfo);
    } else if (Type == "Error") {
        WMessageBox::CriticalMessage(Title, Text, TextInfo);
    }

    return true;
}




bool Packager::DispatchNewPayload(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;
    QString Payload = Info.value("Payload").toString();
    QString OutPath = Info.value("Path").toString();
    QString Name = Info.value("Name").toString();
    QString Error = Info.value("Error").toString();
    if(Error=="Error"){
        WMessageBox::CriticalMessage("New Payload", "error compiling payload", "Error");
        UIGeneratePayload->CloseWindow();
        return false;
    }
    QByteArray decodedPayload = QByteArray::fromBase64(Payload.toUtf8());
    QString filePath = OutPath + "/" + Name;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        WMessageBox::CriticalMessage("New Payload", "error write payload", "Error");
        return false;
    }

    if (file.write(decodedPayload) == -1) {
        WMessageBox::CriticalMessage("New Payload", "error decode payload", "Error");
        file.close();
        return false;
    }

    file.close();

    WMessageBox::GoodMessage("New Payload","New Payload Generated Successfully","Good");
    UIGenerateWaiting->hide();
    UIGeneratePayload->CloseWindow();
    return true;
}

bool Packager::DispatchC2ConfigTC(PPackage Package)
{
    QJsonObject Info = Package->Body.Info;

    // Extracting configuration values from the JSON object
    QString MagicValue = Info.value("MagicValue").toString();
    QStringList Domains;
    QJsonArray domainsArray = Info.value("Domains").toArray();
    for (const QJsonValue &value : domainsArray) {
        Domains << value.toString();
    }
    QString DefaultDNSServer = Info.value("DefaultDNSServer").toString();
    QString ListenerPort = Info.value("ListenerPort").toString();
    int Sleep = Info.value("Sleep").toInt();
    int Jitter = Info.value("Jitter").toInt();
    int LookupDelay = Info.value("LookupDelay").toInt();

    // Assuming UIC2Config is properly initialized and has a method to set these values
    UIC2Config->SetC2Config(MagicValue, Domains, DefaultDNSServer, ListenerPort, Sleep, Jitter, LookupDelay);

    return true; // Indicate success
}


