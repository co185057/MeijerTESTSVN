#include "..\stdafx.h"
#include "DMInterface.h"

#define COMP_ID ID_DM		// used by the trace() calls to identify this component
#define T_ID    _T("DMX")

const long DM_EL_SUCCESS              = 0;
const long DM_EL_CREATE               = 1;

LPCTSTR g_sClassNames[] = 
   {
    _T("Receipt"),
    _T("Journal"),
	_T("Slip"),
	_T("Printer"),
    _T("CashDrawer"),
	_T("CashChanger"),
	_T("Keylock"),
	_T("LineDisplay"),
	_T("MICR"),
	_T("MSR"),
	_T("Scale"),
	_T("Scanner"),
	_T("SignatureCapture"),
	_T("MISC"),
	_T("Encryptor"),
	_T("CashAcceptor"),
	_T("CoinAcceptor"),
	_T("AuxDisplay"),
	_T("MotionSensor"),
	_T("ElectronicArticleSurveillant"),
	_T("CashRecycler"),     // Standard OPOS CashChanger - not Scot CashChanger
	_T("PointCardRW")		//4.2 card dispenser
   };

/* static */

BOOL CGenericOposIF::sm_bSkipStartupCheckHealth = FALSE;

#if 0
CGenericOposIF::CGenericOposIF(
   long nDeviceId )
   :  m_nDeviceId( nDeviceId )
   {
   }
#endif

/* virtual */
void CGenericOposIF::ReadRegistry( 
   HKEY hKey )
   {
   }

      
long CGenericOposIF::CreateOposControl( 
   LPCTSTR     sDeviceDescription,
   long        nCtrlId,
   LPCTSTR     sProfile,
   CWnd*       pParentWnd )
   {
   m_sProfile = sProfile;
   m_nControlId = nCtrlId;
   const CString sDeviceClass = 
      CString(g_sClassNames[ GetDeviceClass() ]) + _T('\\') + sProfile;
   m_deviceStatistics.csDescription = sDeviceDescription;
   m_deviceStatistics.cStatus = DM_DA_CONFIGURED_BUT_NOT_LOADED;
   long bResult = pParentWnd->SendMessage( WMU_CREATE_DEVICE, nCtrlId, (LPARAM)this );
   trace(L6, _T("DMX:OpenDevice/%s %s returned %d"),  _T("Create"), sDeviceClass, bResult  );
   return bResult != FALSE ? 0 : -1;;
   }


long CGenericOposIF::OpenDevice( 
   void )
   {
   LPCTSTR sDmCreatePhase = _T("Open");
   const CString sDeviceClass = 
      CString(g_sClassNames[ GetDeviceClass() ]) + _T('\\') + m_sProfile;
   long lResult = lResult = Open( m_sProfile );
      trace(L6, _T("DMX:OpenDevice/%s %s returned %d"), sDmCreatePhase, sDeviceClass, lResult  );
	if ( lResult == OPOS_SUCCESS )
      {
      // Get description from device. but not if it is in the registry 
      if ( m_deviceStatistics.csDescription.IsEmpty() != FALSE )
         m_deviceStatistics.csDescription = GetDeviceDescription();
      sDmCreatePhase = _T("Claim");
      lResult = ClaimDevice( 0 );
      trace(L6, _T("DMX:OpenDevice %s %s returned %d"), 
         sDmCreatePhase, sDeviceClass, lResult  );
      if ( lResult == OPOS_SUCCESS 
         || lResult == OPOS_E_ILLEGAL || lResult == OPOS_E_FAILURE )  //3.1 Error Handling. Still enable device when receiving nRc == OPOS_E_FAILURE
			{
         sDmCreatePhase = _T("SetDeviceEnabled");
			SetDeviceEnabled(TRUE);
         lResult = GetResultCode();
         trace(L6, _T("DMX:OpenDevice %s %s returned %d"), 
            sDmCreatePhase, sDeviceClass, lResult  );
			if ( lResult == OPOS_SUCCESS )
            {
				// Not all Service Objects (e.g. Wedge SOs) support the CheckHealth()
				// method. If its not supported (OPOS_E_ILLEGAL), we have no choice but
				// to assume the device is available.
            sDmCreatePhase = _T("CheckHealth");
            if ( sm_bSkipStartupCheckHealth == FALSE )
               {
               lResult = CheckHealth( OPOS_CH_INTERNAL );
               trace(L6, _T("DMX:OpenDevice%s %s returned %d"), 
                  sDmCreatePhase, sDeviceClass, lResult  );
               }
			//TAR 408163 - comment out special OPOS_E_NOHARDWARE - causes assist mode hangs
            if ( lResult == OPOS_E_ILLEGAL /*|| (GetDeviceClass() <= DMCLASS_PRINTER && lResult == OPOS_E_NOHARDWARE)*/ ) //3.1 Error Handling
               lResult = OPOS_SUCCESS;
   			if ( lResult == OPOS_SUCCESS )
               {
               sDmCreatePhase = _T("DoOneTimeInitialization");
               lResult = DoOneTimeInitialization();
               trace(L6, _T("DMX:OpenDevice/%s %s returned %d"), 
                  sDmCreatePhase, sDeviceClass, lResult  );
               if ( IsInputDevice() != FALSE )
                  SetDeviceEnabled( FALSE );
               }
            }
			}
      }
// Trace Configuration
   if ( lResult == OPOS_SUCCESS )
      {
      m_deviceStatistics.cStatus = DM_DA_LOADED;
      m_deviceStatistics.fDoEnable = IsInputDevice();

      trace(L6, _T("DMX:OpenDevice %s is available"), sDeviceClass );
      }
   else
      trace(L6, _T("DMX:OpenDevice %s is NOT available: %s returned %d"), 
         sDeviceClass, sDmCreatePhase, lResult  );
   return lResult;
	}

long CGenericOposIF::ReloadDevice(void)
{
    trace(L6, _T("DMX: ReloadDevice - Device Class=%d [%s]"),
          GetDeviceClass(), g_sClassNames[GetDeviceClass()]); 
    long lRc = CloseDevice();
    trace(L6, _T("DMX: ReloadDevice - CloseDevice returns %d"), lRc);

    lRc = OpenDevice();
    trace(L6, _T("DMX: ReloadDevice - OpenDevice returns %d"), lRc);

    return lRc;
}

long CGenericOposIF::MapOposErrorToDM( 
   long  nResult )
   {
   ASSERT( nResult != OPOS_SUCCESS );
   ASSERT( nResult != OPOS_E_BUSY );
   /*  SR672
   long nStatus = DM_ERROR_UNEXPECTED;
   if ( nResult == OPOS_E_FAILURE )
      nStatus = DM_ERROR_GENERALFAILURE;
   else if ( nResult == OPOS_E_TIMEOUT )
      nStatus = DM_ERROR_TIMEOUT;
   else if ( nResult == OPOS_E_NOHARDWARE )
      nStatus = DM_ERROR_NO_HARDWARE;
   else if ( nResult == OPOS_E_OFFLINE )
      nStatus = DM_ERROR_OFFLINE;
   return nStatus;
   */
   return nResult;
   }



void CGenericOposIF::PostDmEventToApplication( 
   long           nDeviceClass, 
   long           nDeviceId, 
   const CString& sData)
   {
	if (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces"))
	{
		//+Tar219151
		//if this starts with B3980, or it's MICR data
		//it's customer data we don't want to trace all of it.
		if (sData.Find(_T("B3980")) != -1) 
		{
		   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA(%s)"), 
				 g_sClassNames[nDeviceClass], nDeviceId, (sData.Mid(0,6)));
		}
		else if ((nDeviceClass == DMCLASS_MICR) || (nDeviceClass == DMCLASS_MSR))
		{
		   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA length(%d)"), 
				 g_sClassNames[nDeviceClass], nDeviceId, _tcsclen(sData));
		}
	}
	else //it's ok to trace it all
	{
   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA(%s)"), 
      g_sClassNames[nDeviceClass], nDeviceId, sData );
	}
	//-Tar219151
	try{
   mo.PostDM( DM_DATA, 
              nDeviceClass, 
              nDeviceId, 
              sData.GetLength(), 
              sData);
	}
	catch(...)
	{ 
		trace(L6, _T("Unexpected exception caught in CGenericOposIF::PostDmEventToApplication()\n"));
	}
   return;
   }


void CGenericOposIF::PostDmEventToApplication( 
   long  nEventType,
   long  nDeviceClass, 
   long  nDeviceId, 
   const CString& sData,
   long  lDirectIONumber,
   long  lDirectIONumData)
   {
	if (co.csUserExitString5 == _T("OmitCustSensitiveDataInTraces"))
	{
		//+Tar219151
		//if this starts with B3980, or it's MICR data
		//it's customer data we don't want to trace all of it.
		if (sData.Find(_T("B3980")) != -1) 
		{
		   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA(%s)"), 
				 g_sClassNames[nDeviceClass], nDeviceId, (sData.Mid(0,6)));
		}
		else if ((nDeviceClass == DMCLASS_MICR) || (nDeviceClass == DMCLASS_MSR))
		{
		   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA length(%d)"), 
				 g_sClassNames[nDeviceClass], nDeviceId, _tcsclen(sData));
		}
	}
	else //it's ok to trace it all
	{
   trace(L6, _T("DMX:PostDmEventToApplication %s-%d DM_DATA(%s)"), 
      g_sClassNames[nDeviceClass], nDeviceId, sData );
	}
	//-Tar219151
	try{
   mo.PostDM( nEventType, 
              nDeviceClass, 
              nDeviceId, 
              sData.GetLength(), 
              sData, 
              lDirectIONumber,
              lDirectIONumData);
   	}
	catch(...)
	{ 
		trace(L6, _T("Unexpected exception caught in CGenericOposIF::PostDmEventToApplication()\n"));
	}
   return;
   }


void CGenericOposIF::PostDmEventToApplication( 
   long           nEventType,
   long           nDeviceClass, 
   long           nDeviceId, 
   long           nStatusType )
   {
   ASSERT( nEventType == DM_STATUS || nEventType == DM_ERROR || nEventType == DM_DIRECTIO);
   if ( nStatusType != 0 )
      {
      trace(L6, _T("DMX:PostDmEventToApplication %s-%d %s(%d)"), 
         g_sClassNames[nDeviceClass], 
         nDeviceId, 
         nEventType == DM_STATUS ? _T("DM_STATUS") : _T("DM_ERROR"),
         nStatusType );
	  try{
			mo.PostDM( nEventType, nDeviceClass, nDeviceId, nStatusType, NULL );
		}
	 catch(...)
		{ 
		trace(L6, _T("Unexpected exception caught in CGenericOposIF::PostDmEventToApplication()\n"));
		}
      }
   return;
   }


CDeviceStatistics CGenericOposIF::GetDeviceStatistics( 
   void )
   {
   CDeviceStatistics resultStats( m_deviceStatistics );
   resultStats.fIsEnabled = GetDeviceEnabled() != FALSE;
   return resultStats;
   }



long CGenericOposIF::CloseDevice(
   void )
   {
   //SetDeviceEnabled( FALSE ); //SSCOP-3813 Remove this unnecessary call.
   ReleaseDevice();
   return Close();
   }
