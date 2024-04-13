#include "Include/transport/connector.hpp"
#include "Include/helper/wmessagebox.hpp"
#include "Include/login.hpp"
#include "Include/helper/macro.hpp"
#include <QCryptographicHash>
#include <QMap>
#include <QBuffer>
QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

Connector::Connector(ConnectionInfo* ConnectionInfo)
{
 Teamserver              = ConnectionInfo;
Socket              = new QWebSocket();
const QString& Server   = "ws://"+ Teamserver->Host + ":" + this->Teamserver->Port + "/AtherAPI";
QObject::connect( Socket, &QWebSocket::connected,     this, &Connector::OnConnect );
QObject::connect( Socket, &QWebSocket::disconnected,  this, &Connector::OnClosed );
// QObject::connect( Socket, &QWebSocket::errorOccurred, this, &Connector::socketError );

Socket->open( QUrl( Server ) );


}

Connector::~Connector() noexcept
{

}





void Connector::SendLogin()
{
    Package Package;
    Head_t Head;
    Body_t Body;
    Head.Event = LoginCA;
    Head.User = this->Teamserver->User.toStdString();
    Head.Time = QTime::currentTime().toString("hh:mm:ss").toStdString();
    QJsonObject Info;
    Info.insert("User", this->Teamserver->User);
    Info.insert("Password", QString::fromLocal8Bit(QCryptographicHash::hash(this->Teamserver->Password.toLocal8Bit(), QCryptographicHash::Sha3_256).toHex()));

    Body.SubEvent = LoginCA;
    Body.Info = Info;

    Package.Head = Head;
    Package.Body = Body;
    SendPackage(&Package);
}

bool Connector::Connect()
{
    this->Packager = new class Packager;
    SendLogin();
    return 1;
}

void Connector::OnConnect()
{
   QObject::connect( Socket, &QWebSocket::binaryMessageReceived, this, &Connector::OnReceive );

    Connect();
}

void Connector::OnClosed()
{

WMessageBox::CriticalMessage("Erorr","Connection Error","Fail To Connect to TeamServer");
}


void Connector::OnReceive(const QByteArray &Message)
{

    qDebug() << "RecivedNewMassge\n" ;
    auto Package = Packager::DecodePackage( Message );

    if ( Package != nullptr ){
        qDebug() << "Package OK\n" ;

        Packager->DispatchPackage( Package );
        qDebug() << "Packager->DispatchPackage OK\n" ;

    }

    else{
        spdlog::critical( "Got Invalid json" );
    }
}

void Connector::SendPackage( PPackage Package )
{
    //Set SessionID Before Sending
    Package->Head.SessionID=this->Teamserver->SessionID.toStdString();
    Socket->sendBinaryMessage( Packager->EncodePackage( *Package ).toJson( QJsonDocument::Compact ) );
}

