#include "StdAfx.h"
#include "ACSIOClient.h"
#include "ACS.h"
#include "Utils.h"
#include "Options.h"
#include "TraceImpl.h"

namespace ACSIOClient
{
   namespace
   {
      bool isLoaded = true;
		BOOL bDisconnecting = FALSE;
      HMODULE lib = 0;
   }

   BOOL IsDisconnecting()
   {
	   return bDisconnecting;
   }

   void IsDisconnecting(BOOL bVal)
   {
	   bDisconnecting = bVal;
   }

   BOOL IsLoaded()
   {
      return isLoaded;
   }


   BOOL Load(LPCTSTR pszDll)
   {
      Trace trace(_T("ACSIOClient::Load"));
      return TRUE;
   }


   void Unload()
   {
      Trace trace(_T("ACSIOClient::Unload"));
   }


   HRESULT Initialize(DWORD dwCoInit, HCLIENT *hClient)
   {
	   return ACSIOInitialize(dwCoInit, hClient);
   }


   HRESULT Uninitialize(HCLIENT hClient)
   {
	   return ACSIOUninitialize(hClient);
   }


   HRESULT ConnectContextChannel(HCLIENT hClient)
   {
      Trace trace(_T("ACSIOClient::ConnectContextChannel"));

	   HRESULT hr = ACSIOConnectContextChannel(hClient);
	   if( SUCCEEDED(hr) )
	   {
         int iEntries = Options::LongVal(Options::OptionValue(ACS::OPT_ACSIO_QUEUE_ENTRIES, ACS::OPT_ACSIO_QUEUE_ENTRIES_DEF));

		   if( iEntries > 0 )
		   {
			   hr = ACSIOSetMaxQueueEntries(hClient, iEntries);
		   }
		   else
		   {
			   trace(DCM_ERROR, _T("Failed to retrieve a valid value for ACSIO queue entries. Value: '%d'"), iEntries);
		   }
	   }

	   return hr;
   }


   HRESULT ConnectSequenceChannel(HCLIENT hClient)
   {
      Trace trace(_T("ACSIOClient::ConnectSequenceChannel"));

	   HRESULT hr = ACSIOConnectSequenceChannel(hClient);
	   if( SUCCEEDED(hr) )
	   {
		   int iEntries = Options::LongVal(Options::OptionValue(ACS::OPT_ACSIO_QUEUE_ENTRIES, ACS::OPT_ACSIO_QUEUE_ENTRIES_DEF));

		   if( iEntries > 0 )
		   {
			   hr = ACSIOSetMaxQueueEntries(hClient, iEntries);
		   }
		   else
		   {
			   trace(DCM_ERROR, _T("Failed to retrieve a valid value for ACSIO queue entries. Value: '%d'"), iEntries);
		   }
	   }

	   return hr;
   }


   HRESULT Advise(HCLIENT hClient, HANDLE hSyncEventObject)
   {
	   return ACSIOAdvise(hClient, hSyncEventObject);
   }


   HRESULT Unadvise(HCLIENT hClient)
   {
	   return ACSIOUnadvise(hClient);
   }


   HRESULT SendContext(HCLIENT hClient, LPCSTR pszContextName, LPCVOID pContextData)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOSendContext(hClient, (LPSTR)pszContextName, (void*)pContextData);
   }


   HRESULT GetContext(HCLIENT hClient, CONTEXTSINK lpFunc)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOGetContext(hClient, lpFunc);
   }


   HRESULT SendSequence(HCLIENT hClient, int nSequence, LPCVOID pKeyboard)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOSendSequence(hClient, nSequence, (void*)pKeyboard);
   }


   HRESULT GetSequence(HCLIENT hClient,  SEQUENCESINK lpFunc)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOGetSequence(hClient, lpFunc);
   }

   bool GetErrorText(HCLIENT hClient, LPCSTR pszText, int nBufferSize)
   {
      Trace trace(_T("ACSIOClient::GetErrorText"));

	   // ANSI INTERFACE PROVIDED BY ACSIO Client - DO NOT CONVERT TO GENERIC TEXT
	  if( bDisconnecting ) return false;

      try
	   {
			return ACSIOGetErrorText(hClient, (char*)pszText, nBufferSize) ? true : false;
	   }
	   catch(_com_error & cex)
	   {
			trace(DCM_ERROR) << _T("ACSIOGetErrorText EXCEPTION 0x") << std::hex << cex.Error() << std::dec << _T(": ")<< cex.ErrorMessage() << NCR::Util::endl;
		   pszText = NULL;
		   return false;
	   }
   }

   HRESULT AddFilter(HCLIENT hClient, LPCSTR pszContextName)
   {
	   return ACSIOAddFilter(hClient, (LPSTR)pszContextName);
   }

   HRESULT ConnectAuxiliaryChannel(HCLIENT hClient)
   {
	   return ACSIOConnectAuxiliaryChannel(hClient);
   }

   HRESULT SendRaw(HCLIENT hClient, int nBufferLength, BYTE *pBuffer)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOSendRaw(hClient, nBufferLength, pBuffer);
   }


   HRESULT GetRaw(HCLIENT hClient, int *nBufferLength, BYTE *pBuffer)
   {
	   if( bDisconnecting ) return ACSIO_NOT_INITIALIZED;
	   return ACSIOGetRaw(hClient, nBufferLength, pBuffer);
   }

   HRESULT SetMaxQueueEntries(HCLIENT hClient, int iMaxEntries)
   {
	   return ACSIOSetMaxQueueEntries(hClient, iMaxEntries);
   }

   CString GetErrorText(HCLIENT hClient)
   {
	   const BUF_SIZE = 4000;
	   char szError[BUF_SIZE];
	   int iError = BUF_SIZE;

      if( ACSIOGetErrorText(hClient, szError, iError) )
	   {
		   return (LPCTSTR)CA2T(szError);
	   }
	   else
	   {
		   return _T("");
	   }
   }
}

