
#include "global.hpp"
#include "login.hpp"
#include "athermainwindow.hpp"
#include "interacttab.hpp"
#include "addlistener.hpp"
#include "generatepayload.h"
#include "setlistener.h"
#include "about.h"
#include "generatewaiting.h"
//UI
class Login* UILogin;
class AtherMainWindow* UIMainWindow;
class AddListener* UIAddListener;
class GeneratePayload* UIGeneratePayload;
class SetListener* UISetListener;
class C2Config* UIC2Config;
class about* UIAbout;
class GenerateWaiting* UIGenerateWaiting;
//APP
QApplication* AtherApp;



ConnectionInfo* Teamserver;

//Target
QMap<QString,TargetInfo*> TargetInfoMap;
//Listener
QMap<QString,ListenerInfo*> ListenerMap;
//Connector
class Connector* Connect;



std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;

}



size_t Hash(std::string str) {
    size_t hash = 0;
    for (char ch : str) {
        hash = hash * 31 + ch; // A simple hash function
    }
    return hash;
}
