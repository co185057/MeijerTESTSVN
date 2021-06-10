// SCOTEASSensor.cpp: interface for the CSCOTMotionSensor and CSCOTEASDeactivator classes.
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "OposEasDeactivator.h"
#include "opos_eas.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTEASSensor")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSCOTEASDeactivator
//////////////////////////////////////////////////////////////////////

CSCOTEASDeactivator::CSCOTEASDeactivator()
{
}

CSCOTEASDeactivator::~CSCOTEASDeactivator()
{
}

//////////////////////////////////////////////////////////////////////
// CSCOTSensormatic methods 
//////////////////////////////////////////////////////////////////////

CSCOTSensormatic::CSCOTSensormatic()
{
}

CSCOTSensormatic::~CSCOTSensormatic()
{
}


long CSCOTSensormatic::Enable()
{
  long rc = 0;
  
  m_EASDeactivator.SetDeviceEnabled(TRUE);
  rc = m_EASDeactivator.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTSensormatic::Disable()
{
  long rc = 0;
  
  m_EASDeactivator.SetDeviceEnabled(FALSE);
  rc = m_EASDeactivator.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTSensormatic::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  m_pMainWnd = pMainWnd;

  if (!m_EASDeactivator.Initialize(profile, pParentWnd, nID, true))
  {
    CString csErr = m_EASDeactivator.GetErrorFunc();
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, csErr);
    rc = -1;
  }
  
  return rc;
}

long CSCOTSensormatic::UnInitialize()
{
  long rc = 0;
  
  m_EASDeactivator.UnInitialize();
  return rc;
}

void CSCOTSensormatic::DataEvent(long Status)
{
  long DMStatus;

  switch ( Status )
  {
	case EAS_STATUS_OK:
	  DMStatus = EAS_STATUS_OK;
	  break;

	case EAS_STATUS_DEACTIVATED:
	  DMStatus = EAS_STATUS_DEACTIVATED;
	  break;

	case EAS_STATUS_FAILURE:
	  DMStatus = EAS_STATUS_FAILURE;
	  ASSERT(FALSE);
	  break;
  }
 
  // leave this line in case we ever want to notify the app of a deactivation event
  mo.PostDM(DM_DATA, DMCLASS_EASDEACTIVATOR, 0, sizeof(VT_I4), (void*)&DMStatus);

}


void CSCOTSensormatic::ErrorEvent(	LONG ResultCode, 
									LONG ResultCodeExtended, 
									LONG ErrorLocus, 
									LONG* pErrorResponse)
{
}

// use this call so we can keep using the OposControl.cpp template that Scot Barnhart wrote
void CSCOTSensormatic::SetDataEventEnabled(long lEnableDeactivateMode)
{
  m_EASDeactivator.SetTagDeactivateMode(lEnableDeactivateMode);

}
