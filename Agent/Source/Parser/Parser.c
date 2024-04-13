
#include "Parser/Parser.h"
#include "Debug.h"
#include "Support/Memory.h"

void ParserNew(PPARSER parser, void* Buffer, uint32_t size)
{
    if (parser == NULL){
        return;
        }

    parser->Original = HMalloc(size);

    Memcpy(parser->Original, Buffer, size);
    parser->Buffer = parser->Original ;
    parser->Length = size;
    parser->Size = size;
}

int ParserGetInt32(PPARSER parser)
{
    signed int intBytes = 0;

    if (parser->Length < 4)
        return 0;

    Memcpy(&intBytes, parser->Buffer, 4);

    parser->Buffer += 4;
    parser->Length -= 4;

    if (!parser->Endian)
        return (int)intBytes;
    else
        return (int)_byteswap_ulong(intBytes);
}

char* ParserGetBytes(PPARSER parser, uint32_t* size)
{
    uint32_t  Length = 0;
    char*   outdata = NULL;

    if (parser->Length < 4)
        return NULL;

    Memcpy(&Length, parser->Buffer, 4);
    parser->Buffer += 4;

    if (parser->Endian)
        Length = _byteswap_ulong(Length);
    outdata = parser->Buffer;
    if (outdata == NULL)
        return NULL;

    parser->Length -= 4;
    parser->Length -= Length;
    parser->Buffer += Length;

    if (size != NULL)
        *size = Length;

    return outdata;
}

void ParserDestroy(PPARSER Parser)
{
    if (Parser->Original)
    {

        Memset(Parser->Original, 0, Parser->Size); 
        HFree(Parser->Original,Parser->Size);
        Parser->Original = NULL;
        HFree(Parser,sizeof(PARSER));

    }
}


