#include <stdlib.h>
#include "Debug.h"
void *Memset(void *dest, int val, unsigned long long len)
{
    unsigned char *ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}
void *Memcpy(void *dest, const void *src, unsigned long long len)
{
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}
void *Memmove(void *dest, const void *src, unsigned long long len)
{
    char *d = dest;
    const char *s = src;
    if (d < s)
        while (len--)
            *d++ = *s++;
    else
    {
        char *lasts = s + (len - 1);
        char *lastd = d + (len - 1);
        while (len--)
            *lastd-- = *lasts--;
    }
    return dest;
}

void *HMalloc(unsigned long long size)
{
    void *NewMemory = 0;

    NewMemory = malloc(size);
    if (!NewMemory)
    {
        RED_DPRINT("Memory allocation failed.\n");
        return NULL; // or some error handling
    }
    Memset((unsigned char*)NewMemory,0,size);
    return NewMemory;

}

void *HRealloc(void *ptr, unsigned long long newSize, unsigned long long oldSize)
{
    void *NewMemory = realloc(ptr, newSize);

    if (!NewMemory)
    {
        RED_DPRINT("Memory reallocation failed.\n");
        return NULL;
    }
    if(newSize > oldSize)
    {
        Memset((unsigned char*)NewMemory + oldSize, 0, newSize - oldSize);
    }

    return NewMemory;
}

void HFree(void *ptr, unsigned long long MemSize)
{
    if(ptr) 
    {   
        Memset((unsigned char*)ptr, 0,MemSize);
        free(ptr);
        DPRINT("Memory freed successfully.\n");
    }
    else 
    {
        RED_DPRINT("Attempted to free a NULL pointer.\n");
    }
}
