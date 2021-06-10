#pragma once
#include "ACSIOClientApi.h"

namespace ACSIOClient
{
   BOOL IsLoaded();
   BOOL Load(LPCTSTR pszDll);
   void IsDisconnecting(BOOL bVal);

   static const int AUX_BUFFER_SIZE = 5000;
   static const int SEQUENCE_BUFFER_SIZE = 5000;

	HRESULT Initialize(DWORD dwCoInit, HCLIENT *hClient);
	HRESULT Uninitialize(HCLIENT hClient);
	HRESULT ConnectContextChannel(HCLIENT hClient);
	HRESULT ConnectSequenceChannel(HCLIENT hClient);
	HRESULT Advise(HCLIENT hClient, HANDLE hSyncEventObject);
	HRESULT Unadvise(HCLIENT hClient) ;
	HRESULT SendContext(HCLIENT hClient, LPCSTR pszContextName, LPCVOID pContextData);
	HRESULT GetContext(HCLIENT hClient, CONTEXTSINK lpFunc) ;
	HRESULT SendSequence(HCLIENT hClient, int nSequence, LPCVOID pKeyboard);
	HRESULT GetSequence(HCLIENT hClient, SEQUENCESINK lpFunc);
	bool GetErrorText(HCLIENT hClient, LPCSTR pszText, int nBufferSize);
	HRESULT AddFilter(HCLIENT hClient,	LPCSTR pszContextName);

	// Auxiliary named channels creation and I/O
	HRESULT ConnectAuxiliaryChannel(HCLIENT hClient);
	HRESULT SendRaw(HCLIENT hClient, int nBufferLength, BYTE *pBuffer);
	HRESULT GetRaw(HCLIENT hClient, int *nBufferLength, BYTE *pBuffer);
	HRESULT SetMaxQueueEntries(HCLIENT hClient, int iMaxEntries);

	// Helper method that populates a CString and converts to unicode implicitly.
	CString GetErrorText(HCLIENT hClient);

   void Unload();
}
