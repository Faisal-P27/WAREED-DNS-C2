#include "Packer/Packer.h"
#include "Support/Memory.h"
#include "Debug.h"


#define DefaultBufferSize 64

void Int64ToBuffer(unsigned char* Buffer, uint64_t Value)
{
    Buffer[7] = Value & 0xFF;
    Value >>= 8;

    Buffer[6] = Value & 0xFF;
    Value >>= 8;

    Buffer[5] = Value & 0xFF;
    Value >>= 8;

    Buffer[4] = Value & 0xFF;
    Value >>= 8;

    Buffer[3] = Value & 0xFF;
    Value >>= 8;

    Buffer[2] = Value & 0xFF;
    Value >>= 8;

    Buffer[1] = Value & 0xFF;
    Value >>= 8;

    Buffer[0] = Value & 0xFF;
}

void Int32ToBuffer(unsigned char* Buffer, uint32_t Size)
{
    (Buffer)[0] = (Size >> 24) & 0xFF;
    (Buffer)[1] = (Size >> 16) & 0xFF;
    (Buffer)[2] = (Size >> 8) & 0xFF;
    (Buffer)[3] = (Size)&0xFF;
}

void PackageAddInt32(PPACKAGE Package, uint32_t dataInt)
{
    Package->Buffer =
        HRealloc(Package->Buffer, Package->Length + sizeof(uint32_t),Package->Length);

    Int32ToBuffer((unsigned char *)Package->Buffer + Package->Length, dataInt);

    Package->Size = Package->Length;
    Package->Length += sizeof(uint32_t);
}

void PackageAddInt64(PPACKAGE Package, uint64_t dataInt)
{
    Package->Buffer =
        HRealloc(Package->Buffer, Package->Length + sizeof(uint64_t),Package->Length);

    Int64ToBuffer((unsigned char *)Package->Buffer + Package->Length, dataInt);

    Package->Size = Package->Length;
    Package->Length += sizeof(uint64_t);
}

void PackageAddPad(PPACKAGE Package, unsigned char* Data, size_t Size)
{
    Package->Buffer = HRealloc(Package->Buffer, Package->Length + Size,Package->Length);

    Memcpy((unsigned char *)Package->Buffer + (Package->Length), Data, Size);

    Package->Size = Package->Length;
    Package->Length += Size;
}

void PackageAddBytes(PPACKAGE Package, unsigned char* Data, size_t Size)
{
    PackageAddInt32(Package, Size);

    Package->Buffer = HRealloc(Package->Buffer, Package->Length + Size,Package->Length);

    Int32ToBuffer((unsigned char *)Package->Buffer + (Package->Length - sizeof(uint32_t)),
                  Size);

    Memcpy((unsigned char *)Package->Buffer + Package->Length, Data, Size);

    Package->Size = Package->Length;
    Package->Length += Size;
}

PPACKAGE PackageCreate()
{
    PPACKAGE Package = NULL;
    Package = HMalloc(sizeof(PACKAGE));
    Package->Buffer = HMalloc(sizeof(unsigned char));
    Package->Length = 0;
    Package->Size=0;
    //PackageAddInt32(Package, 0);
    return Package;
}


void PackageDestroy(PPACKAGE Package)
{
    if (!Package)
        return;

    if (!Package->Buffer)
        return;

    Memset(Package->Buffer, 0, Package->Length);
    HFree(Package->Buffer,Package->Length);
    Package->Buffer = NULL;

    Memset(Package, 0, sizeof(PACKAGE));
    HFree(Package,sizeof(PACKAGE));
    Package = NULL;
}
