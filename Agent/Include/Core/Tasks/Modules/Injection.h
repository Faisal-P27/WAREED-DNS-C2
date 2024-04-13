#pragma once
#include <windows.h>
#include <tlhelp32.h>

//Simple 100% detected!
int Inject(unsigned char *ShellCode,int size,int ProcessID);
