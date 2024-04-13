#ifndef PACKAGER_HPP
#define PACKAGER_HPP

#include <spdlog/spdlog.h>
#include "global.hpp"
using namespace spdlog;




class Packager
{
public:
    Packager();
    static PPackage DecodePackage(const QString& Package );
    QJsonDocument EncodePackage( Package Package );
    bool DispatchPackage( PPackage Package );


    bool DispatchLoginResult(PPackage Package);
    bool DispatchNewTargetSession(PPackage Package);
    bool DispatchPrintConsole(PPackage Package);
    bool DispatchNewListener(PPackage Package);
    bool DispatchDelListener(PPackage Package);
    bool DispatchUpdateLastSeen(PPackage Package);
    bool DispatchUpdateListenerHits(PPackage Package);
    bool DispatchNewMessageBox(PPackage Package);
    bool DispatchNewPayload(PPackage Package);
    bool DispatchC2ConfigTC(PPackage Package);
};
#endif // PACKAGER_HPP
