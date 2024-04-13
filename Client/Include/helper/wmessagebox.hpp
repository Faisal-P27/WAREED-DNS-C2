#ifndef WMESSAGEBOX_HPP
#define WMESSAGEBOX_HPP
#include "global.hpp"

class WMessageBox
{
public:
    WMessageBox();


    static void CriticalMessage(const QString &title,const QString &Text,const QString &TextInfo);
    static void GoodMessage(const QString &title, const QString &Text, const QString &TextInfo);
};

#endif // WMESSAGEBOX_HPP
