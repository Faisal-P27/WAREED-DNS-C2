#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>







unsigned char* ResolveDomain(char* DoaminName);
int ExtractDNSServerAddress();