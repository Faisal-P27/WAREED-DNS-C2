#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>


typedef struct ProxyInfo {
	
	char* host;
	int hostsize;
	char* port;
	int portsize;
}ProxyInfo;

int StartSocks5(void* PInfo);
int CloseProxy();