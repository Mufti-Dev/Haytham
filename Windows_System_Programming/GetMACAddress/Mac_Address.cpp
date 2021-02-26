// This program gets mac address of a system which is running windows (OS)
// Windows System APIs GetAdapterInfo() used. 

#include "stdafx.h"

TCHAR* GetMACAddress()
{
	DWORD _macAddress = 0;
	IP_ADAPTER_INFO _adapterInfo[16];
	
	DWORD dwBufLen = sizeof(_adapterInfo);
	DWORD dwStatus = GetAdaptersInfo(_adapterInfo, &dwBufLen);
	assert(dwStatus == ERROR_SUCCESS);

	PIP_ADAPTER_INFO _pAdapterInfo = _adapterInfo;
	
	TCHAR string [256];
	do
	{
/*		_macAddress = _pAdapterInfo->Address [5] + 
			_pAdapterInfo->Address [4] * 256 + 
			_pAdapterInfo->Address [3] * 256 * 256 + 
			_pAdapterInfo->Address [2] * 256 * 256 * 256;
*/
		/*printf("MAC Address: %02X-%02X-%02X-%02X-%02X-%02X\n", 
			_pAdapterInfo->Address[0], _pAdapterInfo->Address[1], 
			_pAdapterInfo->Address[2], _pAdapterInfo->Address[3], 
			_pAdapterInfo->Address[4], _pAdapterInfo->Address[5]);*/
	
		_stprintf (string, _T("%02X-%02X-%02X-%02X-%02X-%02X"), 
			_pAdapterInfo->Address[0], _pAdapterInfo->Address[1], 
			_pAdapterInfo->Address[2], _pAdapterInfo->Address[3], 
			_pAdapterInfo->Address[4], _pAdapterInfo->Address[5]);

		_pAdapterInfo = _pAdapterInfo->Next;

	}while(_pAdapterInfo);

	return string;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR *_pMacAddress = GetMACAddress();
	_tprintf(_T("MACAddress: %s\n"), _pMacAddress);
	return 0;
}

