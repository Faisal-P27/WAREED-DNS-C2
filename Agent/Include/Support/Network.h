#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _DEFAULT_SOURCE
#define IPSIZE          INET6_ADDRSTRLEN            /* IP Address size                  */
#define DEFAULT_IPV6_PREFIX  128

int get_ipv6_numeric(const char* address, struct in6_addr* addr6) ;
int ExpandIPv6(char* ip_address, size_t size);

static int inet_pton_2(int af, const char *src, void *dst);
static char *inet_ntop4(const unsigned char *src, char *dst, socklen_t size);
static char *inet_ntop6(const unsigned char *src, char *dst, socklen_t size);

