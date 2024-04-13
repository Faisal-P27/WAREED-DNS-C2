#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <stdint.h>
#include "../Debug.h"

typedef struct {
    char*   Original;
    char*   Buffer;
    uint32_t  Size;
    uint32_t  Length;
    void* HeapAddress;
    int    Endian;
} PARSER, * PPARSER;

void  ParserNew(PPARSER parser, void* buffer, uint32_t size);
void  ParserDecrypt(PPARSER parser, unsigned char* Key, unsigned char* IV);
int   ParserGetInt32(PPARSER parser);
char* ParserGetBytes(PPARSER parser, uint32_t* size);
void  ParserDestroy(PPARSER Parser);

#endif