
#pragma once

void XORData(unsigned char *b, unsigned long long length, const unsigned char *Key, int KeyLen);
char* EncryptData(unsigned char* Bytes,int Length,int *OutSize);
unsigned char* DecryptData(char* StrData,int Length);