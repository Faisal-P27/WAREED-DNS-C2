
#pragma once

// Node definition
typedef struct DomainNode
{
    char *DomainName;
    struct DomainNode *next;
} DomainNode;

typedef struct DomainList
{
    DomainNode *head;
    DomainNode *currentNode;

}DomainList;

typedef struct HamsInfo
{

    char *SessionID;
    int Sleep;
    int Jetter;
    DomainList *C2Domains;
    int C2DomainsCount;
    char *Key;
    char *MagicValue;
} HamsInfo;

HamsInfo *Hams;


