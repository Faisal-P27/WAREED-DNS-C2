#pragma once



void* Memset(void* dest, int val, unsigned long long len);
void* Memcpy(void* dest, const void* src, unsigned long long len);
void* Memmove(void* dest, const void* src, unsigned long long len);
int Memcmp(const void* str1, const void* str2, unsigned long long count);


void *HMalloc(unsigned long long size);
void *HRealloc(void *ptr, unsigned long long newSize, unsigned long long oldSize);
void HFree(void *ptr, unsigned long long MemSize);