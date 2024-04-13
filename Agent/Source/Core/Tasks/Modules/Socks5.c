#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <winsock.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include "Core/Tasks/Modules/Socks5.h"
#include "Debug.h"
#include "Support/Memory.h"

#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "Ws2_32.lib")

#define SECONDS(x) (x * 1000)

#define AUTHENTICATION_ENABLED 0 // change to 1 for username password, leave 0 to require no authentication

#define METHOD_NUMBER 2

// hardcoded inactive creds, use proper creds check if you want this
#define PREDEF_USERNAME "hello"
#define PREDEF_PASSWORD "bello"
//#define HTTP_HEADER_POSTE "POST login HTTP/1.0\r\n"\
//"Host: %s\r\n"\
//"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36\r\n"\
//"Content-type: application/x-www-form-urlencoded\r\n"\
//"Content-length: %d\r\n\r\n"\

#define HTTP_HEADER_POSTE "POST / HTTP/1.0\r\n"                                                                                                                 \
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36\r\n" \
                          "Content-type: application/x-www-form-urlencoded\r\n"                                                                                 \
                          "Content-length: 0\r\n\r\n"

int verbose = 1;
HANDLE SessionConnectEvent = NULL;
int ProxyStatus = 0;
char initializeProxy[] = HTTP_HEADER_POSTE;

int method_no_auth_required(SOCKET c, int count, char *rv);
int method_username_password(SOCKET c, int count, char *rv);

typedef int (*fn)(SOCKET, int, char *);
static fn method_functions[METHOD_NUMBER] =
    {
        method_no_auth_required,
        method_username_password};

static char method_numbers[METHOD_NUMBER] =
    {
        0,
        2};

// in case other channels would be used instead of TCP
int internal_send(SOCKET s, const char *buf, int len, int flags)
{
    return send(s, buf, len, flags);
}

int internal_recv(SOCKET s, char *buf, int len, int flags)
{
    return recv(s, buf, len, flags);
}

int method_no_auth_required(SOCKET c, int count, char *rv)
{
    return TRUE;
}

int method_username_password(SOCKET c, int count, char *rv)
{
    char username[256], password[256];
    char *p;
    char answer[2];
    int ret;

    memset(username, 0, 256);
    memset(password, 0, 256);
    answer[0] = 0x01;
    answer[1] = 0x01;

    if ((unsigned char)rv[1] > count - 2)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) method_username_password: %ld > %ld\n", GetCurrentThreadId(), rv[1], count);}

        if ((ret = send(c, answer, 2, 0)) == SOCKET_ERROR)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) method_username_password %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
        }
        return FALSE;
    }
    p = rv + 2;
    memcpy_s(username, 256, p, (unsigned char)rv[1]);
    p = rv + 2 + rv[1];

    if ((unsigned char)p[0] + (unsigned char)rv[1] > count - 3)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) method_username_password: %ld > %ld\n", GetCurrentThreadId(), p[0] + rv[1] + 3, count);}
    }

    memcpy_s(password, 256, p + 1, (unsigned char)p[0]);

    if ((strncmp(username, PREDEF_USERNAME, strlen(PREDEF_USERNAME)) == 0) && (strncmp(password, PREDEF_PASSWORD, strlen(PREDEF_PASSWORD)) == 0))
    {
        answer[1] = 0x00;
        if ((ret = send(c, answer, 2, 0)) == SOCKET_ERROR)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) method_username_password failed %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
        }

        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) method_username_password succeed for: %s\n", GetCurrentThreadId(), username);}

        return TRUE;
    }

    if ((ret = send(c, answer, 2, 0)) == SOCKET_ERROR)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) method_no_auth_required3 %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
    }
    return FALSE;
}

int CheckAuthentication(SOCKET c, char *buf, int ret)
{
    int i, j;
    char answer[2];

    answer[0] = 5;

    for (i = 0; i < METHOD_NUMBER; i++)
        for (j = 0; j < buf[1]; j++)
            if (buf[j + 2] == method_numbers[i])
            {
                answer[1] = method_numbers[i];
                if ((ret = send(c, answer, 2, 0)) == SOCKET_ERROR)
                {
                    if (verbose){
                        YELLOW_DPRINT("[-] SOCKS thread(%d) CheckAuthentication: %ld\n", GetCurrentThreadId(), ret);}
                }
                return method_numbers[i];
            }

    answer[1] = (unsigned)0xFF;

    if ((ret = send(c, answer, 2, 0)) == SOCKET_ERROR)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) CheckAuthentication NO ACCEPTABLE METHODS %ld\n", GetCurrentThreadId(), ret);}
        return i;
    }

    return -1;
}

void sendReply(SOCKET c, char replyField, char addressType, char *addr, char *port)
{
    char answer[300];
    char null[20];
    int ret;

    memset(answer, 0, 300);
    memset(null, 0, 20);

    // if addr or port set to NULL, we will send nulls instead of the address
    // it isn't RFC compliant but I do not support info leak either.
    if (addr == NULL)
        addr = null;
    if (port == NULL)
        port = null;

    answer[0] = 5;

    answer[1] = replyField;
    answer[3] = addressType;

    switch (addressType)
    {
    case 3:
        memcpy_s(answer + 4, 296, (void *)(addr + 1), (unsigned char)(addr[0]));
        memcpy_s(answer + 4 + (unsigned char)(addr[0]), 396 - (unsigned char)(addr[0]), port, 2);
        break;
    case 4:
        memcpy_s(answer + 4, 296, addr, 16);
        memcpy_s(answer + 20, 280, port, 2);
        ret = 22;
        break;
    default:
        memcpy_s(answer + 4, 296, addr, 4);
        memcpy_s(answer + 8, 292, port, 2);
        ret = 10;
        break;
    }
    if ((ret = send(c, answer, ret, 0)) == SOCKET_ERROR)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) sendReply send error: %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
    }
}

int getAddressInfo(SOCKADDR_IN *sockaddrin, SOCKADDR_IN6 *sockaddrin6, char *buf, int ret)
{
    ADDRINFOA hints;
    ADDRINFOA *result = NULL;

    char domain[256];

    // IPv4
    if (buf[3] == 1)
    {
        if (ret != 10)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) getAddressInfo IPv4 selected, length mismatch: %ld\n", GetCurrentThreadId(), ret);}
            return -1;
        }
        sockaddrin->sin_family = AF_INET;
        memcpy_s(&(sockaddrin->sin_port), 2, buf + 8, 2);
        memcpy_s(&(sockaddrin->sin_addr), 4, buf + 4, 4);

        char *s = (char *)malloc(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(sockaddrin->sin_addr), s, INET_ADDRSTRLEN);
        if (verbose){
            YELLOW_DPRINT("[+] SOCKS thread(%d) getAddressInfo CONNECT IPv4: %s:%hd\n", GetCurrentThreadId(), s, htons(sockaddrin->sin_port));}
        free(s);
    }
    // DNS
    if (buf[3] == 3)
    {
        if ((7 + (unsigned char)buf[4]) != ret)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) getAddressInfo DNS selected, length mismatch: %ld\n", GetCurrentThreadId(), ret);}
            return -1;
        }
        ZeroMemory(&hints, sizeof(hints));
        ZeroMemory(domain, 256);

        // change for IPv6?
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        memcpy_s(domain, 256, (void *)(buf + 5), (unsigned char)(buf[4]));

        if ((ret = GetAddrInfoA(domain, "1", &hints, &result)) != 0)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) getAddressInfo GetAddrInfoA failed with error: %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
            return -1;
        }
        memcpy_s(sockaddrin, sizeof(SOCKADDR_IN), result->ai_addr, sizeof(SOCKADDR_IN));
        memcpy_s(&(sockaddrin->sin_port), 2, buf + ((unsigned char)buf[4]) + 5, 2);

        char *s = (char *)malloc(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(sockaddrin->sin_addr), s, INET_ADDRSTRLEN);
        if (verbose){
            YELLOW_DPRINT("[+] SOCKS thread(%d) getAddressInfo CONNECT DNS: %s(%s):%hd\n", GetCurrentThreadId(), domain, s, htons(sockaddrin->sin_port));}
        free(s);
    }
    // IPv6
    if (buf[3] == 4)
    {
        if (ret != 22)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) getAddressInfo IPv6 selected, length mismatch: %ld\n", GetCurrentThreadId(), ret);}
            return -1;
        }
        sockaddrin6->sin6_family = AF_INET6;
        memcpy_s(&(sockaddrin6->sin6_port), 2, buf + 20, 2);
        memcpy_s(&(sockaddrin6->sin6_addr), 30, buf + 4, 16);

        char *s = (char *)malloc(INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &(sockaddrin6->sin6_addr), s, INET6_ADDRSTRLEN);
        if (verbose){
            YELLOW_DPRINT("[+] SOCKS thread(%d) getAddressInfo CONNECT IPv6: %s:%hd\n", GetCurrentThreadId(), s, htons(sockaddrin6->sin6_port));}
        free(s);
    }

    return 0;
}

SOCKET DoConnection(SOCKET c, char *buf, int ret)
{
    SOCKET sock;
    SOCKADDR_IN sockaddrin;
    SOCKADDR_IN6 sockaddrin6;

    if (buf[0] == 5)
    {
        if (getAddressInfo(&sockaddrin, &sockaddrin6, buf, ret) < 0)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection could not create socket structs\n", GetCurrentThreadId());}
            // this isnt "general SOCKS server failure", but there no better error code
            sendReply(c, 0x01, 0x01, NULL, NULL);
            return NULL;
        }

        // CONNECT
        if (buf[1] == 1)
        {
            if (buf[3] == 4)
            {
                if ((sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
                {
                    if (verbose){
                        YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection socket6() failed with: %ld\n", GetCurrentThreadId(), WSAGetLastError());}
                    sendReply(c, 0x01, 0x04, NULL, NULL);
                    return NULL;
                }

                if ((ret = connect(sock, (SOCKADDR *)&sockaddrin6, sizeof(sockaddrin6))) == SOCKET_ERROR)
                {
                    if (verbose){
                        YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection connect6() failed with: %ld\n", GetCurrentThreadId(), WSAGetLastError());}
                    sendReply(c, 0x05, 0x04, NULL, NULL);
                    return NULL;
                }
            }
            else
            {
                if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
                {
                    if (verbose){
                        YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection socket() failed with: %ld\n", GetCurrentThreadId(), WSAGetLastError());}
                    sendReply(c, 0x01, 0x01, NULL, NULL);
                    return NULL;
                }

                if ((ret = connect(sock, (SOCKADDR *)&sockaddrin, sizeof(sockaddrin))) == SOCKET_ERROR)
                {
                    if (verbose){
                        YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection connect() failed with: %ld\n", GetCurrentThreadId(), WSAGetLastError());}
                    sendReply(c, 0x05, 0x01, NULL, NULL);
                    return NULL;
                }
            }

            sendReply(c, 0x00, 0x01, NULL, NULL);

            return sock;
        }
        // BIND
        if (buf[1] == 2)
        {
            YELLOW_DPRINT("[+] SOCKS DoConnection BIND\n");
        }
        // UDP ASSOCIATE
        if (buf[1] == 3)
        {
            // SOCK_DGRAM
            YELLOW_DPRINT("[+] SOCKS DoConnection UDP ASSOCIATE\n");
        }
    }
    else
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) DoConnection unknown SOCKS version\n", GetCurrentThreadId());}
        return NULL;
    }

    return NULL;
}

void HandleClient(void *param)
{
    SOCKET sClientConnection = (SOCKET)param;
    SOCKET sRelayConnection;
    // fd_set fdset, fdsetclear;
    // timeval timeout;
    char buf[1024];
    int ret, recvn, sentn, sent;
    BOOL run = TRUE;
    int authnum = -1;
    u_long blocking = 0;

    HANDLE hEvents[2];

    YELLOW_DPRINT("[-] SOCKS thread(%d) recv request\n", GetCurrentThreadId());

    if ((ret = recv(sClientConnection, buf, sizeof buf, 0)) > 0)
    {
        SetEvent(SessionConnectEvent);
        if (buf[0] == 4)
        {
            
            // socks4
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient Socks4 request\n", GetCurrentThreadId());}
            goto exitthread;
        }
        else if (buf[0] == 5)
        {
            // socks5
            if (ret - 2 != buf[1])
            {
                if (verbose){
                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient wrong list length: %ld\n", GetCurrentThreadId(), ret);}
                goto exitthread;
            }

            if ((authnum = CheckAuthentication(sClientConnection, buf, ret)) < 0)
            {
                if (verbose){
                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient auth failed: %ld\n", GetCurrentThreadId(), authnum);}
                goto exitthread;
            }
        }
        else
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient unknown Socks version number\n", GetCurrentThreadId());}
            goto exitthread;
        }
    }
    else
    {
        SetEvent(SessionConnectEvent);
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient recv error: %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
        goto exitthread;
    }

    if (authnum == -1)
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient wrong authnum: %ld\n", GetCurrentThreadId(), authnum);}
        goto exitthread;
    }

    if (AUTHENTICATION_ENABLED)
    {
        if (verbose){
            YELLOW_DPRINT("[+] SOCKS thread(%d) HandleClient authentication invoked: %ld\n", GetCurrentThreadId(), authnum);}

        if ((ret = recv(sClientConnection, buf, sizeof buf, 0)) > 2)
        {
            if (!method_functions[AUTHENTICATION_ENABLED](sClientConnection, ret, buf))
            {
                if (verbose){
                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient authentication failed: %ld\n", GetCurrentThreadId(), authnum);}
                goto exitthread;
            }
        }
        else
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient authentication recv error: %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
            goto exitthread;
        }
    }

    if ((ret = recv(sClientConnection, buf, sizeof buf, 0)) > 6)
    {
        sRelayConnection = 0;
        if ((sRelayConnection = DoConnection(sClientConnection, buf, ret)) == NULL)
        {
            if (verbose){
                YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient no socket created\n", GetCurrentThreadId());}
            goto exitthread;
        }
    }
    else
    {
        if (verbose){
            YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient connection request recv error: %ld %ld\n", GetCurrentThreadId(), ret, WSAGetLastError());}
        goto exitthread;
    }

    hEvents[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
    hEvents[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

    if ((hEvents[0] == NULL) || ((hEvents[1] == NULL)))
    {
        YELLOW_DPRINT("[-] SOCKS thread(%d) CreateEvent failed with %ld\n", GetCurrentThreadId(), GetLastError());
        goto exitthread;
    }

    WSAEventSelect(sClientConnection, hEvents[0], FD_READ);
    WSAEventSelect(sRelayConnection, hEvents[1], FD_READ);

    // somebody please tell me how to handle properly the WSAWOULDBLOCK in blocking mode...

    while (run)
    {
        ret = WaitForMultipleObjects(2, hEvents, FALSE, SECONDS(30));

        if ((ret - WAIT_OBJECT_0) < 0 || (ret - WAIT_OBJECT_0) > (2 - 1))
        {
            YELLOW_DPRINT("[-] SOCKS thread(%d) WaitForMultipleObjects index out of range %ld\n", GetCurrentThreadId(), ret);
            run = FALSE;
        }

        switch (ret - WAIT_OBJECT_0)
        {
        case 0:
            if ((recvn = recv(sClientConnection, buf, sizeof buf, 0)) >= 0)
            {
                sent = 0;
                while (sent < recvn)
                {
                    do
                    {
                        if (!run)
                        {

                            break;
                        }
                        if ((sentn = send(sRelayConnection, buf + sent, recvn - sent, 0)) != SOCKET_ERROR)
                        {
                            sent += sentn;
                        }
                        else
                        {
                            if (WSAGetLastError() != 10035)
                            {
                                if (verbose){
                                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient select2 send error: %ld %ld\n", GetCurrentThreadId(), sentn, WSAGetLastError());}
                                run = FALSE;
                                break;
                            }
                        }
                    } while (WSAGetLastError() == 10035);
                    if(sentn==SOCKET_ERROR){
                        RED_DPRINT("[-] SOCKS thread(%d) HandleClient select2 send error: %ld %ld\n", GetCurrentThreadId(), sentn, WSAGetLastError());
                        break;
                    }
                }
            }
            else
            {
                // FIN recv'd
                if (recvn == 0)
                    goto exitthread;
                // error
                if (verbose){
                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient select2 recv error: %ld %ld\n", GetCurrentThreadId(), recvn, WSAGetLastError());}
                run = FALSE;
            }
            break;
        case 1:
            if ((recvn = recv(sRelayConnection, buf, sizeof buf, 0)) >= 0)
            {
                sent = 0;
                while (sent < recvn)
                {
                    do
                    {
                        if (!run)
                        {
                            break;
                        }
                        if ((sentn = send(sClientConnection, buf + sent, recvn - sent, 0)) != SOCKET_ERROR)
                        {
                            sent += sentn;
                        }
                        else
                        {
                            if (WSAGetLastError() != 10035)
                            {
                                if (verbose){
                                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient select1 send error: %ld %ld\n", GetCurrentThreadId(), sentn, WSAGetLastError());}
                                run = FALSE;
                                break;
                            }
                        }
                    } while (

                        WSAGetLastError() == 10035);
                    if(sentn==SOCKET_ERROR){
                     RED_DPRINT("[-] SOCKS thread(%d) HandleClient select1 send error: %ld %ld\n", GetCurrentThreadId(), sentn, WSAGetLastError());
                        break;
                    } 
                }

            }
            else
            {
                // FIN recv'd
                if (recvn == 0)
                    goto exitthread;
                // error
                if (verbose){
                    YELLOW_DPRINT("[-] SOCKS thread(%d) HandleClient select1 recv error: %ld %ld\n", GetCurrentThreadId(), recvn, WSAGetLastError());}
                run = FALSE;
                break;
            }

            break;

        default:

            break;
        }
    }

exitthread:
    YELLOW_DPRINT("[+] Exiting Thread \n");
    closesocket(sClientConnection);

    if (sRelayConnection != INVALID_SOCKET)
    {
        if (closesocket(sRelayConnection) == SOCKET_ERROR)
        {
            YELLOW_DPRINT("[-] Error closing relay connection socket: %d\n", WSAGetLastError());
        }
    }

    return;
}

int ProxyInfoDestroy(ProxyInfo *Info)
{
    HFree(Info->host,Info->hostsize);
    HFree(Info->port,Info->portsize);
    HFree(Info,sizeof(ProxyInfo));
    return 1;
}

int CloseProxy()
{

    if (ProxyStatus)
    {
        YELLOW_DPRINT("Closing Proxy\n");
        ProxyStatus = 0;
        SetEvent(SessionConnectEvent);      
        return 1;
    }
    YELLOW_DPRINT("Proxy Not Running!\n");
    return 0;
}

int StartSocks5(void *PInfo)

{
    ProxyInfo *Info = (ProxyInfo *)PInfo;
    if (ProxyStatus)
    {
        YELLOW_DPRINT("Proxy Already Running!\n");
        goto Clean;
    }
    SOCKET s = 0;
    WSADATA wsaData = {0};
    ADDRINFOW *result = NULL;

    int ret;

    YELLOW_DPRINT("[*] Firing up SOCKS server\n");
    YELLOW_DPRINT("[*] Coded by Balazs Bucsay <socksserver[at]rycon[dot]hu>\n");
    SessionConnectEvent = CreateEventA(NULL, FALSE, FALSE, "SEvent");
    if (!SessionConnectEvent)
    {
        YELLOW_DPRINT("[*] Fail To create CreateEventA\n");
        goto Clean;
    }
    SetEvent(SessionConnectEvent);
    if ((ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
    {
        YELLOW_DPRINT("[-] SOCKS WSAStartup() failed with error: %ld %ld\n", ret, WSAGetLastError());
        goto Clean;
    }
    ProxyStatus = 1;
    while (1)
    {
        WaitForSingleObject(SessionConnectEvent, INFINITE);

        if (!ProxyStatus)
        {
            YELLOW_DPRINT("Proxy Closed\n");
            break;
        }
        s = socket(AF_INET, SOCK_STREAM, 0);
        if (s == INVALID_SOCKET)
        {
            YELLOW_DPRINT("[-] SOCKS socket() failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return -1;
        }

        if (!WSAConnectByNameA(s, Info->host, Info->port, NULL, NULL, NULL, NULL, NULL, NULL))
        {
            // closesocket(s);
            YELLOW_DPRINT("[+] Fail To WSAConnectByNameA! error: %ld\n", WSAGetLastError());
            break;
        }
        
        if (ret = send(s, initializeProxy, sizeof(initializeProxy), 0) == SOCKET_ERROR)
        {
            if (verbose){
            YELLOW_DPRINT(" Fail To Send initializeProxy\n");
            }
            break;

        }
        YELLOW_DPRINT("[+] SOCKS Client connected, starting thread \n");
        _beginthread(HandleClient, 0, (void *)s);
        // getchar();
    }
    YELLOW_DPRINT("[*] Closing down SOCKS Server\n");



Clean:
        WSACleanup();
        if (s != INVALID_SOCKET || s!=0)
        {
            closesocket(s);
        }

        if (SessionConnectEvent)
        {
            CloseHandle(SessionConnectEvent);
        }
        SessionConnectEvent=0;
        ProxyStatus = 0;


    ProxyInfoDestroy(Info);
    return 0;
}
