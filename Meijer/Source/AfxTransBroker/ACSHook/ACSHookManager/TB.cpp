#include "StdAfx.h"
#include "TB.h"
#include "Utils.h"
#include "TraceImpl.h"

namespace TB
{
   namespace
   {
      HMODULE lib = 0;

	   typedef long (*TBGetTerminalNumber_ptr_t)(void);
      
      TBGetTerminalNumber_ptr_t TBGetTerminalNumber;
   }

   BOOL Load(LPCTSTR pszDll)
   {
      Trace trace(_T("TB::Load"));
      Unload();
       
      _ASSERT(!lib);
       
      // cleanup any residual allocations
      lib = LoadLibrary(pszDll);
      
      if( !lib )
      {
			trace(DCM_ERROR) << _T("Failed to Load the Transaction Broker library '") << pszDll << _T("'. Error: ") << Utils.GetLastErrorString().GetString() << NCR::Util::endl;
         return FALSE;
      }
	   
      BOOL bRet = TRUE;

	   //
      // get the function pointers and ensure that we get
	   // all of them successfully before returning
	   //
	   bRet = bRet && ( TBGetTerminalNumber = (TBGetTerminalNumber_ptr_t)GetProcAddress(lib, "TBGetTerminalNumber") );

	   if( !bRet )
	   {
		   trace(DCM_ERROR, _T("Failed to GetProcAddress on one or more of the TransBroker.dll Functions."));
	   }

      return bRet;
   }


   void Unload()
   {
		if ( lib != NULL )
		{
			if( !FreeLibrary(lib) )
			{
				ATLTRACE2(_T("Uninitialize - Failed to unload the TransBroker.dll library."));
			}
		}

	   TBGetTerminalNumber = NULL;
	   lib = NULL;
   }

   long GetTerminalNumber(void)
   {
	   _ASSERTE(TBGetTerminalNumber);
	   return TBGetTerminalNumber();
   }
}
