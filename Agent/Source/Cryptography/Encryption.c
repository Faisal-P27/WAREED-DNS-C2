#include <stdlib.h>
#include "Cryptography/Base32.h"
#include "Support/String.h"
#include "Debug.h"
#include "Support/Memory.h"
#include "Hams.h"


void XORData(unsigned char *b, unsigned long long length, const unsigned char *Key,int KeyLen) {
    unsigned long long k = 0;
    for (unsigned long long i = 0; i < length; i++) {
        if (k >= KeyLen) {
            k = 0;
        }
        b[i] = b[i] ^ Key[k];
        k++;
    }
}





char* EncryptData(unsigned char* Bytes,int Length,int *OutSize){
    XORData(Bytes,Length,Hams->Key,strlen(Hams->Key));
    int outlen=BASE32_LEN(Length);
    int Totallen=outlen+1;
    DPRINT("need Encode len [%d]",outlen);

    char *Encoded=HMalloc(Totallen);
    *OutSize=Totallen;
    if (!Encoded) {
        RED_DPRINT("Memory allocation failed.\n");
        return NULL;  // or some error handling
    }
    int ret=base32_encode(Bytes,Length,Encoded);
    convertToLowercase(Encoded);
    replaceLetter(Encoded,outlen,'=','-');
    if(ret==-1){
        RED_DPRINT("Error Encode Data");
        return 0;
    }
    return Encoded;

}

unsigned char* DecryptData(char* StrData,int Length){
   
   int outlen=UNBASE32_LEN(Length);
   char *Plain=HMalloc(outlen);
    if (!Plain) {
        RED_DPRINT("Memory allocation failed.\n");
        return NULL;  // or some error handling
    }
   DPRINT("need decode len [%d]",outlen);
   int ret=base32_decode(StrData,Plain);
   DPRINT("Decode Size [%d]\n",ret);
    if(ret==-1){
        RED_DPRINT("Error Decode Data");
        return 0;
    }
    XORData(Plain,ret,Hams->Key,strlen(Hams->Key));
    return Plain;
}