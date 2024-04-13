#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocket>
#include <QAbstractSocket>
#include "global.hpp"
#include "packager.hpp"


class Connector : public QTcpSocket
{
private:
    QWebSocket*           Socket     = nullptr;
    ConnectionInfo* Teamserver = nullptr;
    Packager* Packager   = nullptr;
   // Util::ConnectionInfo* Teamserver = nullptr;
   // HavocSpace::Packager* Packager   = nullptr;

public:
    QString ErrorString = nullptr;

    Connector(ConnectionInfo* ConnectionInfo);
    ~Connector() noexcept;

    bool Disconnect();

    void SendLogin();
   // void SendPackage( Util::Packager::PPackage package );
    void SendPackage( PPackage Package );

public Q_SLOTS:
    bool Connect();
    void OnConnect();
    void OnClosed();
    void OnReceive( const QByteArray& Message );
};

#endif // CONNECTOR_H
