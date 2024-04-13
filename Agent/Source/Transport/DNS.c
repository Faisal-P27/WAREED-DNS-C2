#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <windns.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "Dnsapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include "Debug.h"
#include "Support/Memory.h"
#include "Support/Network.h"
#include "Macro/Macro.h"
#include "Config.h"

struct in_addr *dnsArray = NULL;
// Count the number of DNS server addresses and allocate memory
int dnsCount = 0;
int dnsID = 0;
char DefulteDNS[] = DefaultDNSServer;
int ExtractDNSServerAddress()
{

    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;
    IP_ADAPTER_ADDRESSES *pAddresses = NULL, *pCurrAddresses = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    struct sockaddr_in *pAddrIn = NULL;

    // Call the function once to get the required buffer size
    GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen);

    // Allocate memory
    pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

    // Call the function again to get the adapter information
    if ((dwRetVal = GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen)) == NO_ERROR)
    {
        for (pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
        {
            // Loop through each DNS server address for the adapter
            for (pDnServer = pCurrAddresses->FirstDnsServerAddress; pDnServer != NULL; pDnServer = pDnServer->Next)
            {
                pAddrIn = (struct sockaddr_in *)pDnServer->Address.lpSockaddr;

                // Skip the address if it is 0.0.0.0
                if (pAddrIn->sin_addr.s_addr == 0)
                {
                    continue;
                }

                dnsCount++;
                dnsArray = (struct in_addr *)realloc(dnsArray, dnsCount * sizeof(struct in_addr));
                dnsArray[dnsCount - 1] = pAddrIn->sin_addr;
            }
        }
    }
    else
    {
        DPRINT("GetAdaptersAddresses failed with error: %d\n", dwRetVal);
        return 1;
    }

    // Print and use DNS addresses
    for (int i = 0; i < dnsCount; ++i)
    {
        DPRINT("DNS %d: %s\n", i + 1, inet_ntoa(dnsArray[i]));
    }

    // Free allocated memory
    free(pAddresses);
    // free(dnsArray);

    return 0;
}

#if defined(_WIN32) || defined(_WIN64)

int ChangeDnsServer()
{
    DPRINT("dnsID [%d] dnsCount [%d]", dnsID, dnsCount);
    if (dnsID + 1 >= dnsCount)
    {
        dnsID = 0;
    }
    else
    {
        dnsID++; // goto next dns server
    }
    return 1;
}

unsigned char *ResolveDomain(char *DoaminName)
{

    PDNS_RECORD pDnsRecord = NULL;
    DNS_STATUS status = {0};
    ;
    IP6_ADDRESS addr = {0};
    IP4_ARRAY serverArray = {0};
    LPVOID ResultIpv6Address = NULL;
    PDNS_RECORD p = NULL;
    // Initialize the IP4_ARRAY
    serverArray.AddrCount = 1;

    if (dnsCount > 0)
    {
        serverArray.AddrArray[0] = dnsArray[dnsID].S_un.S_addr;
        DPRINT("DNS Server: %s\n", inet_ntoa(dnsArray[dnsID]));
    }
    else
    {
        serverArray.AddrArray[0] = inet_addr(DefulteDNS);
        DPRINT("DNS Server Defulte: %s\n", DefulteDNS);
    }

    unsigned char *ipv6Address = HMalloc(INET6_ADDRSTRLEN);

    status = DnsQuery_A(
        DoaminName,
        DNS_TYPE_AAAA,
        DNS_QUERY_BYPASS_CACHE,
        &serverArray, // use default DNS servers
        &pDnsRecord,
        NULL);

    if (status != ERROR_SUCCESS)
    {
        RED_DPRINT("DnsQuery_A failed with error %d\n", status);
        ChangeDnsServer();
        return 0;
    }
    p = pDnsRecord;

    if (p->wType == DNS_TYPE_AAAA && p != NULL)
    {
        addr = p->Data.AAAA.Ip6Address;
        ResultIpv6Address = &p->Data.AAAA.Ip6Address;
        Memcpy(ipv6Address, ResultIpv6Address, 16);
        DnsRecordListFree(pDnsRecord, DnsFreeRecordList);
        return ipv6Address;
    }

    RED_DPRINT("Resolve Doamin Unkonw Error");
    return 0;
}

#endif
