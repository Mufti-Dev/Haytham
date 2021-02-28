// DisableNIC.cpp : Defines the entry point for the console application.
//

// This program disables network interface card (NIC) on windows host
// Author: Mufti Md

#include "stdafx.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

int main(int argc, char* argv[])
{
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    LPVOID lpMsgBuf = NULL;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
    PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
    PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
    IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
    IP_ADAPTER_PREFIX *pPrefix = NULL;

    if (argc != 2) 
	{
        printf(" Usage: getadapteraddresses family\n");
        printf("        getadapteraddresses 4 (for IPv4)\n");
        printf("        getadapteraddresses 6 (for IPv6)\n");
        printf("        getadapteraddresses A (for both IPv4 and IPv6)\n");
        exit(1);
    }
	if (atoi(argv[1]) == 4)
        family = AF_INET;
    else if (atoi(argv[1]) == 6)
        family = AF_INET6;

    outBufLen = sizeof (IP_ADAPTER_ADDRESSES);
    pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    if (pAddresses == NULL) 
	{
        printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
        exit(1);
    }

	// Make an initial call to GetAdaptersAddresses to get the 
    // size needed into the outBufLen variable
    if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
        FREE(pAddresses);
        pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
    }

	if (pAddresses == NULL) 
	{
        printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
        exit(1);
    }
    // Make a second call to GetAdapters Addresses to get the
    // actual data we want
    printf("Memory allocated for GetAdapterAddresses = %d bytes\n", outBufLen);
    printf("Calling GetAdaptersAddresses function with family = ");
    if (family == AF_INET)
        printf("AF_INET\n");
    if (family == AF_INET6)
        printf("AF_INET6\n");
    if (family == AF_UNSPEC)
        printf("AF_UNSPEC\n\n");

    dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
	
	if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {
            printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
                   pCurrAddresses->Length);
            printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
            printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) {
                for (i = 0; pUnicast != NULL; i++)
                    pUnicast = pUnicast->Next;
                printf("\tNumber of Unicast Addresses: %d\n", i);
            } else
                printf("\tNo Unicast Addresses\n");

            pAnycast = pCurrAddresses->FirstAnycastAddress;
            if (pAnycast) {
                for (i = 0; pAnycast != NULL; i++)
                    pAnycast = pAnycast->Next;
                printf("\tNumber of Anycast Addresses: %d\n", i);
            } else
                printf("\tNo Anycast Addresses\n");

            pMulticast = pCurrAddresses->FirstMulticastAddress;
            if (pMulticast) {
                for (i = 0; pMulticast != NULL; i++)
                    pMulticast = pMulticast->Next;
                printf("\tNumber of Multicast Addresses: %d\n", i);
            } else
                printf("\tNo Multicast Addresses\n");

            pDnServer = pCurrAddresses->FirstDnsServerAddress;
            if (pDnServer) {
                for (i = 0; pDnServer != NULL; i++)
                    pDnServer = pDnServer->Next;
                printf("\tNumber of DNS Server Addresses: %d\n", i);
            } else
                printf("\tNo DNS Server Addresses\n");

            printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
            printf("\tDescription: %wS\n", pCurrAddresses->Description);
            printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

            if (pCurrAddresses->PhysicalAddressLength != 0) 
			{
                printf("\tPhysical address: ");
                for (i = 0; i < (int) pCurrAddresses->PhysicalAddressLength; i++) 
				{
                    if (i == (pCurrAddresses->PhysicalAddressLength - 1))
                        printf("%.2X\n",
                               (int) pCurrAddresses->PhysicalAddress[i]);
                    else
                        printf("%.2X-",
                               (int) pCurrAddresses->PhysicalAddress[i]);
                }
            }
            printf("\tFlags: %ld\n", pCurrAddresses->Flags);
            printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
            printf("\tIfType: %ld\n", pCurrAddresses->IfType);
            printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
            printf("\tIpv6IfIndex (IPv6 interface): %u\n", pCurrAddresses->Ipv6IfIndex);
            printf("\tZoneIndices (hex): ");
            for (i = 0; i < 16; i++)
                printf("%lx ", pCurrAddresses->ZoneIndices[i]);
            printf("\n");

            pPrefix = pCurrAddresses->FirstPrefix;
            if (pPrefix) 
			{
                for (i = 0; pPrefix != NULL; i++)
                    pPrefix = pPrefix->Next;
                printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
            } 
			else
                printf("\tNumber of IP Adapter Prefix entries: 0\n");

			if(wcscmp(pCurrAddresses->FriendlyName, L"Local Area Connection") == 0)
			{
				PMIB_IFTABLE ifTable;
				PMIB_IFROW pMibIfRow;
				DWORD dwSize = 0;
				DWORD dwRetVal = 0;

				bool bEnable = false;

				// Allocate memory for our pointers.
				ifTable = (MIB_IFTABLE*) malloc(sizeof(MIB_IFTABLE));
				pMibIfRow = (MIB_IFROW*) malloc(sizeof(MIB_IFROW));
				
				if (GetIfTable(ifTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
				{
					free(ifTable);
					ifTable = (MIB_IFTABLE *) malloc (dwSize);
				}
				// Make a second call to GetIfTable to get the actual
				// data we want.
				if (NO_ERROR == (dwRetVal = GetIfTable(ifTable, &dwSize, 0)))
				{
					if (ifTable->dwNumEntries > 0)
					{
						pMibIfRow->dwIndex = pCurrAddresses->IfIndex;
						if (NO_ERROR == (dwRetVal = GetIfEntry(pMibIfRow)))
						{
							printf("Description: %s\n", pMibIfRow->bDescr);
							printf("Name: %s\n", pMibIfRow->wszName);
							if (bEnable)
							{
								pMibIfRow->dwAdminStatus = MIB_IF_ADMIN_STATUS_UP;
							}
							else
							{
								pMibIfRow->dwAdminStatus = MIB_IF_ADMIN_STATUS_DOWN;
							}

							// finally, set new state of the interface
							if (NO_ERROR == (dwRetVal = SetIfEntry(pMibIfRow)))
							{
								printf("Error in SetIfEntry: %d\n", dwRetVal);
							}
						}
						else
						{
							printf("Error in GetIfEntry: %d\n", dwRetVal);
						}
					}
				}
				else
				{
					printf("Error in GetIfTable:%d\n", dwRetVal);
				}

				free(ifTable);
			}

            printf("\n");

            pCurrAddresses = pCurrAddresses->Next;
        }
    } 
	else 
	{
        printf("Call to GetAdaptersAddresses failed with error: %d\n",
               dwRetVal);
        if (dwRetVal == ERROR_NO_DATA)
            printf("\tNo addresses were found for the requested parameters\n");
        else 
		{
            if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, 
				dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   // Default language
                (LPTSTR) &lpMsgBuf, 0, NULL)) 
			{
                printf("\tError: %s", lpMsgBuf);
                LocalFree(lpMsgBuf);
                FREE(pAddresses);
                exit(1);
            }
        }
    }
    FREE(pAddresses);

	return 0;
}

