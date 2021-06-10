// SCOTDevFactory.cpp : implementation file
//

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTDevFactory.h"
#include "SCOTIVIForms.h"
#include "SCOTPromptDisp.h"
#include "ncrdevmgr.h"
#include "NextGenUISigCap.h"
#include "SCOT5992SigCap.h"
#include "SCOTOPOSSigCap.h"
#include "SCOTDMTopaz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTDevFactory")

#define DEV_KEY_ROOT _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Devices\\")

typedef struct
{
  TCHAR szDevClass[30];
  UINT  nControlID;
} SCOTDEVCLASSCTRLINFO;

static SCOTDEVCLASSCTRLINFO devCtrlInfo[DMCLASS_MAX_CLASSES+6] =
{	
	_T("Printer") , 0,
	_T("Printer") , 0,
	_T("Printer") , 0,
	_T("Printer") , 0,
	_T("CashDrawer") , 0,
	_T("CashChanger") , 0,
	_T("Keylock") , 0,
	_T("LineDisplay") , 0,
	_T("MICR") , 0,
	_T("MSR") , 0,
	_T("Scale") , 0,
	_T("Scanner") , 0,
	_T("SignatureCapture") , IDC_OPOSSIGCAP1,
	_T("MISC") ,             0,
	_T("Encryptor") ,        IDC_OPOSPINPAD1, 
	_T("CashAcceptor") ,     0,
	_T("CoinAcceptor"),      0,
	_T("AuxDisplay") ,       IDC_IVIFORMS1,
	_T("MotionSensor") ,	   0,
	_T("ElectronicArticleSurveillant") ,	 0,
    _T("CashRecycler"),      0,				
    _T("PointCardRW"),      0,				//4.2 - Card Dispenser
    _T("TakeawayBelt"),      IDC_TAKEAWAYBELT,
};
/////////////////////////////////////////////////////////////////////////////
// CSCOTDevFactory

CSCOTDevFactory::CSCOTDevFactory()
{
  m_bInitialized = FALSE;
}

CSCOTDevFactory::~CSCOTDevFactory()
{
  UnInitialize();
}


BEGIN_MESSAGE_MAP(CSCOTDevFactory, CWnd)
	//{{AFX_MSG_MAP(CSCOTDevFactory)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CSCOTDevFactory, CWnd)
    //{{AFX_EVENTSINK_MAP(CSCOTDevFactory)
	ON_EVENT(CSCOTDevFactory, IDC_OPOSPINPAD1, 1 /* DataEvent */, OnDataEventPinpad1,  VTS_I4)
	ON_EVENT(CSCOTDevFactory, IDC_OPOSPINPAD1, 3 /* ErrorEvent*/, OnErrorEventPinpad1, VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	ON_EVENT(CSCOTDevFactory, IDC_OPOSSIGCAP1, 1 /* DataEvent */, OnDataEventSigcap1,  VTS_I4)
	ON_EVENT(CSCOTDevFactory, IDC_OPOSSIGCAP1, 3 /* ErrorEvent*/, OnErrorEventSigcap1, VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
	ON_EVENT(CSCOTDevFactory, IDC_IVIFORMS1,   1 /* DataEvent */, OnDataEventForms1,   VTS_I4)
	ON_EVENT(CSCOTDevFactory, IDC_IVIFORMS1,   3 /* ErrorEvent*/, OnErrorEventForms1,  VTS_I4 VTS_I4 VTS_I4 VTS_PI4)
    ON_EVENT(CSCOTDevFactory, IDC_TAKEAWAYBELT, 1 /* StatusUpdateEvent */, OnDataEventTakeawayBelt, VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSCOTDevFactory message handlers

long CSCOTDevFactory::Initialize(CWnd *pWnd)
{
  long rc = 0;

  // If we are already initialized, uninitialize ourselves
  if (m_bInitialized)
    UnInitialize();

  m_csDevicesInError.Empty();

  m_pParentWnd = pWnd;

  if(m_pParentWnd == NULL)
    m_pParentWnd = AfxGetMainWnd();

  m_bInitialized = Create( NULL, NULL, WS_DISABLED, CRect(0,0,0,0), m_pParentWnd, IDW_SDFWINDOW );

  if (!m_bInitialized)
  {
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_DF_NO_WND, _T("%d|%p"), IDW_SDFWINDOW, m_pParentWnd);
    m_csDevicesInError += _T("SCOT Device Factory\n");
  }

  return m_bInitialized ? 0 : -1;
}

CString CSCOTDevFactory::ReadRegString(HKEY hKey, LPCTSTR pszValue)
{
  DWORD dwType;
  DWORD dwLength;
  CString s;

  // Look for value; get data size.
  long nRC = RegQueryValueEx(
                              hKey,                   // handle of key to query
                              pszValue,               // address of name of value to query
                              NULL,                   // reserved
                              &dwType,                // address of buffer for value type
                              NULL,                   // address of data buffer: just return size.
                              &dwLength );            // address of data buffer size

  // If success and type is string, get the data.
  if ( nRC == ERROR_SUCCESS && dwType == REG_SZ )
  {
    LPTSTR p = s.GetBuffer( dwLength );
    RegQueryValueEx(
                    hKey,               // handle of key to query
                    pszValue,           // address of name of value to query
                    NULL,               // reserved
                    &dwType,            // address of buffer for value type
                    (PBYTE) p,          // address of data buffer
                    &dwLength );        // address of data buffer size
    s.ReleaseBuffer();
  }

  if (nRC != ERROR_SUCCESS)
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          nRC, _T("reading"), pszValue);
  }

  return s;
}

BOOL CSCOTDevFactory::GetDevProfile(SCOTDEVMAPINFO *pDevInfo)
{
		CString sKey;
	HKEY hKey1, hKey2;
	_TCHAR szDeviceName[MAX_PATH];
	long i, nRC;

	sKey = CString(DEV_KEY_ROOT) + pDevInfo->szDevClass;
	nRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, sKey, 0, KEY_READ, &hKey1);

  if (nRC != ERROR_SUCCESS)
  {
    ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
          nRC, _T("opening"), sKey);
  }

  BOOL bDevFound = FALSE;

	i = 0; // initialize enumeration index
	while ((nRC == ERROR_SUCCESS) && !bDevFound)
	{
		nRC = RegEnumKey(hKey1, i, szDeviceName, MAX_PATH);
		if (nRC == ERROR_SUCCESS)
		{
			nRC = RegOpenKeyEx(hKey1, szDeviceName, 0, KEY_READ, &hKey2);

			if (nRC == ERROR_SUCCESS)
			{
				// Look up the "Configure" value and compare with "Yes"
				CString sConfigure = ReadRegString(hKey2, _T("Configure")).Left(1);
				if ( sConfigure.CompareNoCase(_T("Y")) == 0)
				{
				  bDevFound = TRUE;
				  pDevInfo->csProfile = szDeviceName;
				}
			}
			else
			{
				ScotError(	RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
						nRC, _T("opening"), szDeviceName);
			}
			// Close Device Object Key
			RegCloseKey( hKey2 );
		}
		i++;
	}

	// Close Device Class Key
	RegCloseKey( hKey1 );


	if (!bDevFound) 
	{
		if ( (sKey.Right(28) == _T("ElectronicArticleSurveillant"))
             || (sKey.Right(12) == _T("TakeawayBelt")) )
		{
			// do nothing -- optional devices
		}
		else
		{
			bool bReportAsError = true;
			#ifdef _DEBUG
				bReportAsError = false;
			#endif // _DEBUG
			if ((pDevInfo->szDevClass && !(_tcscmp(pDevInfo->szDevClass, _T("SignatureCapture")))) || !bReportAsError)
			{
				ScotError(INFO, SM_CAT_CONFIGURATION, SCOTAPP_DF_NO_DEV_FOUND_INFO, _T("%s"), pDevInfo->szDevClass); 
			}
			else
			{
				ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_DF_NO_DEV_FOUND, _T("%s"), pDevInfo->szDevClass); 
			}
		}
	}
	return bDevFound;
}


long CSCOTDevFactory::CommonInitDevice(SCOTDEVMAPINFO* pDevMapInfo)
{
  long rc = -1;

  ASSERT(pDevMapInfo);

  if (pDevMapInfo)
  {
    rc = pDevMapInfo->pDevice->Initialize(pDevMapInfo->csProfile, 
                                         this, 
                                         m_pParentWnd, 
                                         pDevMapInfo->nControlID);
    if (rc == 0)
    {
      m_DevMap.SetAt(pDevMapInfo->lDevClass, pDevMapInfo);
    }
    else
    {
      m_csDevicesInError += pDevMapInfo->szDevClass;
      m_csDevicesInError += _T("\\");
      m_csDevicesInError += pDevMapInfo->csProfile;
      m_csDevicesInError += _T("\n");

      delete pDevMapInfo->pDevice;
      delete pDevMapInfo;
	  rc = -1;	// return 0 or -1 as calling function (SMOutOfServiceBase.cpp - 
				// InitDevice) handles errors of < 0 
    }
  }
  return rc;
}


long CSCOTDevFactory::InitDevice(CSCOTEncryptor **ppDevice)
{
  long rc = -1;
  ASSERT (ppDevice != NULL);

  SCOTDEVMAPINFO* pDevMapInfo = NewDevInfo(DMCLASS_ENCRYPTOR);
  
  if (GetDevProfile(pDevMapInfo))
  {
	  m_csEncryptorModel = pDevMapInfo->csProfile; //SR672
    if (pDevMapInfo->csProfile.CompareNoCase(_T("NCR5992Pinpad.1")) == 0)
      *ppDevice = new CSCOT5992Pinpad;
    else if (pDevMapInfo->csProfile.CompareNoCase(_T("NCR5945Pinpad.1")) == 0)
      *ppDevice = new CSCOT5945Pinpad;
    else if (pDevMapInfo->csProfile.CompareNoCase(_T("Protege")) == 0)
      *ppDevice = new CSCOTC2000Pinpad;
    else if (pDevMapInfo->csProfile.CompareNoCase(_T("Emulator")) == 0)
      *ppDevice = new CSCOTEmulatorPinpad;
    else
      *ppDevice = new CSCOTOPOSPinpad;
  }
// TAR 193729 - Need the default device case.
else
  *ppDevice = new CSCOTBAPE;

  pDevMapInfo->pDevice = *ppDevice;

  rc = CommonInitDevice(pDevMapInfo);

  if (rc != 0)
  {
    *ppDevice = NULL;
  }

  return rc;
}

long CSCOTDevFactory::InitDevice(CSCOTSigCap** ppDevice)
{
  long rc = -1;
  ASSERT (ppDevice != NULL);

  SCOTDEVMAPINFO* pDevMapInfo = NewDevInfo(DMCLASS_SIGCAP);
  
  if (GetDevProfile(pDevMapInfo))
  {
	m_csSigCapModel = pDevMapInfo->csProfile; //SR672
    if (pDevMapInfo->csProfile.CompareNoCase(_T("NCR5992Sigcap.1")) == 0)
    {
      *ppDevice = new CSCOT5992SigCap();
    }
    else if (pDevMapInfo->csProfile.CompareNoCase(_T("NextGenUISigCap")) == 0)
    {
      *ppDevice = new CNextGenUISigCap();
    }
    else
    {
      *ppDevice = new CSCOTOPOSSigCap();
    }
  }
  else
  {
	m_csSigCapModel = _T("Topaz"); //SR672
    *ppDevice = new CSCOTDMTopaz();
  }


  pDevMapInfo->pDevice = *ppDevice;

  rc = CommonInitDevice(pDevMapInfo);

  if (rc != 0)
  {
    *ppDevice = NULL;
  }

  return rc;
}

long CSCOTDevFactory::InitDevice(CSCOTAuxDisp** ppDevice)
{
  long rc = -1;
  ASSERT (ppDevice != NULL);

  SCOTDEVMAPINFO* pDevMapInfo = NewDevInfo(DMCLASS_FORMS);
  
  if (GetDevProfile(pDevMapInfo))
  {
	m_csAuxDispModel = pDevMapInfo->csProfile; //SR672
    if (pDevMapInfo->csProfile.CompareNoCase(_T("NCR5992Form.1")) == 0)
      *ppDevice = new CSCOTIVIForms;
    else if (pDevMapInfo->csProfile.CompareNoCase(_T("OPOSPinPrompts")) == 0)
      *ppDevice = new CSCOTPromptDisp;
    else
      *ppDevice = new CSCOTNullAuxDisp;
  }
  else
    *ppDevice = new CSCOTNullAuxDisp;

  pDevMapInfo->pDevice = *ppDevice;

  rc = CommonInitDevice(pDevMapInfo);

  if (rc != 0)
  {
    *ppDevice = NULL;
  }

  return rc;
}



long CSCOTDevFactory::InitDevice(CSCOTTakeawayBelt** ppDevice)
{
    long rc = -1;
    ASSERT (ppDevice != NULL);

    SCOTDEVMAPINFO* pDevMapInfo = NewDevInfo(DMCLASS_TAKEAWAYBELT);
  
    if (GetDevProfile(pDevMapInfo))
    {
        *ppDevice = new CSCOTTakeawayBelt;

	    pDevMapInfo->pDevice = *ppDevice;

	    rc = CommonInitDevice(pDevMapInfo);	
	
	    // assure there is at least a NULL device loaded
	    if (rc != 0)
	        *ppDevice = NULL;
	    else	// if the device loaded successfully, set the global	
	        dm.fStateTakeawayBelt = true;	// system does have a Takeaway Belt
    }
    else	// there is no hardware device installed as it is optional
    {
        delete pDevMapInfo;
        *ppDevice = NULL;
	    rc = 0;	// zero is a good return code, 
    }

    return rc;
}

long CSCOTDevFactory::UnInitialize()
{
  POSITION pos = m_DevMap.GetStartPosition();
  SCOTDEVMAPINFO* pDevInfo;
  long lDevClass;
  while (pos)
  {
    m_DevMap.GetNextAssoc(pos, lDevClass, pDevInfo);
    if (pDevInfo)
    {
      pDevInfo->pDevice->UnInitialize();
      delete pDevInfo->pDevice;
      delete pDevInfo;
    }
  }

  m_DevMap.RemoveAll();
  DestroyWindow();

  m_bInitialized = FALSE;

  return 0;
}

void CSCOTDevFactory::OnDataEvent(long lDevClass, long Status) 
{
  SCOTDEVMAPINFO* pDevInfo;
  if (m_DevMap.Lookup(lDevClass, pDevInfo))
  {  
    ASSERT(pDevInfo->pDevice);
    pDevInfo->pDevice->DataEvent(Status);
  }
  else
  {
    ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_DF_UNKNOWN_DATA_EVENT, _T("%d|%d"), 
          lDevClass, Status);
  }
}


void CSCOTDevFactory::OnErrorEvent(long lDevClass,
                                   LONG ResultCode, 
                                   LONG ResultCodeExtended, 
                                   LONG ErrorLocus, 
                                   LONG* pErrorResponse)
{
  SCOTDEVMAPINFO* pDevInfo;
  CString csDev;

  if (m_DevMap.Lookup(lDevClass, pDevInfo))
  {  
    ASSERT(pDevInfo->pDevice);
    pDevInfo->pDevice->ErrorEvent(ResultCode, 
                                  ResultCodeExtended,
                                  ErrorLocus,
                                  pErrorResponse);
    csDev.Format(_T("%s\\%s"), pDevInfo->szDevClass, pDevInfo->csProfile);
  }
  else
  {
    csDev.Format(_T("Unknown device (Device Class %d)"), lDevClass);
  }

  ScotError(RETURN, SM_CAT_INTERNAL_ERROR, SCOTAPP_DF_ERROR_EVENT, _T("%s|%d|%d"), 
        csDev, ResultCode, ResultCodeExtended);
}


CSCOTDevFactory::SCOTDEVMAPINFO* CSCOTDevFactory::NewDevInfo(long lDevClass)
{
  SCOTDEVMAPINFO* pDevMapInfo = new SCOTDEVMAPINFO;
  if (pDevMapInfo)
  {
    pDevMapInfo->lDevClass  = lDevClass;
    pDevMapInfo->szDevClass = devCtrlInfo[lDevClass].szDevClass;
    pDevMapInfo->nControlID = devCtrlInfo[lDevClass].nControlID;
    pDevMapInfo->csProfile.Empty();
  }

  return pDevMapInfo;
}


void CSCOTDevFactory::OnDataEventSigcap1(long Status) 
{
  OnDataEvent(DMCLASS_SIGCAP, Status);
}

void CSCOTDevFactory::OnErrorEventSigcap1(LONG ResultCode, 
                                          LONG ResultCodeExtended, 
                                          LONG ErrorLocus, 
                                          LONG* pErrorResponse)
{
  OnErrorEvent(DMCLASS_SIGCAP,
               ResultCode, 
               ResultCodeExtended,
               ErrorLocus,
               pErrorResponse);
}


void CSCOTDevFactory::OnDataEventPinpad1(long Status) 
{
  OnDataEvent(DMCLASS_ENCRYPTOR, Status);
}

void CSCOTDevFactory::OnErrorEventPinpad1(LONG ResultCode, 
                                          LONG ResultCodeExtended, 
                                          LONG ErrorLocus, 
                                          LONG* pErrorResponse)
{
  OnErrorEvent(DMCLASS_ENCRYPTOR,
               ResultCode, 
               ResultCodeExtended,
               ErrorLocus,
               pErrorResponse);
}


void CSCOTDevFactory::OnDataEventForms1(long Status) 
{
  OnDataEvent(DMCLASS_FORMS, Status);
}

void CSCOTDevFactory::OnErrorEventForms1(LONG ResultCode, 
                                          LONG ResultCodeExtended, 
                                          LONG ErrorLocus, 
                                          LONG* pErrorResponse)
{
  OnErrorEvent(DMCLASS_FORMS,
               ResultCode, 
               ResultCodeExtended,
               ErrorLocus,
               pErrorResponse);
}

void CSCOTDevFactory::OnDataEventEASDeact1(long Status) 
{
  OnDataEvent(DMCLASS_EASDEACTIVATOR, Status);
}


void CSCOTDevFactory::OnDataEventTakeawayBelt(long Status)
{
    OnDataEvent(DMCLASS_TAKEAWAYBELT, Status);
}
