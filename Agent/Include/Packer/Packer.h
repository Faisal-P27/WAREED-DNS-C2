#pragma once
#ifndef PACKAGE_H
#define PACKAGE_H

#include <stdlib.h>
#include <stdint.h>
#include "../Support/Memory.h"

typedef struct {
    void*   Buffer;
    size_t  Length;
    size_t  Size;
} PACKAGE, * PPACKAGE;

PPACKAGE PackageCreate();


void PackageAddInt32(
    PPACKAGE package,
    uint32_t iData
);

void PackageAddInt64(
    PPACKAGE Package,
    uint64_t dataInt
);

void PackageAddBytes(
    PPACKAGE package,
    unsigned char* data,
    size_t dataSize
);

void PackageAddPad(
    PPACKAGE package,
    unsigned char* data,
    size_t dataSize
);

#pragma once
void PackageDestroy(
    PPACKAGE package
);

int PackageTransmit(
    PPACKAGE Package,
    void** Response,
    uint64_t*  Size
);

void PackageTransmitError(
    uint32_t CommandID,
    uint32_t ErrorCode
);


#endif