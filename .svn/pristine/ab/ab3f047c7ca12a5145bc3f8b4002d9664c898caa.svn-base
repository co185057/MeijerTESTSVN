// OposControl.cpp: implementation of the COposControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OposControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COposControl::COposControl()
{
  m_bOpened       = FALSE;
  m_bClaimed      = FALSE;
  m_bEnabled      = FALSE;
  m_bCreated      = FALSE;
  m_Availability  = SCOT_DA_NOT_CONFIGURED;
  m_csFuncInErr.Empty();
}

COposControl::~COposControl()
{

}

BOOL COposControl::OpenControl(LPCTSTR profile, UINT nID, BOOL bLeaveEnabled)
{
	// added bLeaveEnable to param list because EAS devices should be left enabled
  long rc, rce;
  BOOL bRetVal = FALSE;
  m_csFuncInErr = _T("Open");

  if (Open(profile) == OPOS_SUCCESS)
  {
    m_bOpened = TRUE;
    m_csFuncInErr = _T("Claim");
    rc = ClaimDevice(1);             //TAR253061
    if (rc == OPOS_SUCCESS)
    {
      m_bClaimed = TRUE;
      m_csFuncInErr = _T("Enable");
      SetDeviceEnabled(TRUE);
      rc = GetResultCode();
      if (rc == OPOS_SUCCESS)
      {
        rc = CheckHealth(OPOS_CH_INTERNAL);
        if ((rc == OPOS_SUCCESS) || (rc == OPOS_E_ILLEGAL))
        {
		  SetDataEventEnabled(TRUE);
		  rc = GetResultCode();
		  if (rc == OPOS_SUCCESS)
		  {
			  m_Availability = SCOT_DA_LOADED;
			  bRetVal        = TRUE;
		  }
        }
        else
        {
          m_Availability = SCOT_DA_CONFIGURED_BUT_NOT_LOADED;
          m_csFuncInErr = _T("Check Health");
        }

        if (bLeaveEnabled)
          m_bEnabled = TRUE;
        else
          SetDeviceEnabled(FALSE);
      }
    }
  }

  if (!bRetVal)
  {
    rce = GetResultCodeExtended();
    CString csTmp = m_csFuncInErr;
    m_csFuncInErr.Format(_T("%s - rc = %d, rce = %d"), csTmp, rc, rce);
    CloseControl();
  }

  return bRetVal;

}

void COposControl::CloseControl()
{
  if (m_bEnabled)
  {
    SetDeviceEnabled(FALSE);
    m_bEnabled = FALSE;
  }
  if (m_bClaimed)
  {
    ReleaseDevice();     //TAR253061
    m_bClaimed = FALSE;
  }
  if (m_bOpened)
  {
    Close();
    m_bOpened = FALSE;
  }
}

BOOL COposControl::Initialize(LPCTSTR profile, CWnd * pParent, UINT nID, BOOL bLeaveEnabled)
{
  BOOL bRetVal = FALSE;
  m_csFuncInErr = _T("Create Window");


  if(Create(NULL, NULL, CRect(0,0,0,0), pParent, nID))
  {
    m_bCreated = TRUE;
	bRetVal = OpenControl(profile, nID, bLeaveEnabled);
    if (!bRetVal)
    {
      if (m_Availability == SCOT_DA_CONFIGURED_BUT_NOT_LOADED)
      {
        // Not healthy, so retry
        CloseControl();
        bRetVal = OpenControl(profile, nID, bLeaveEnabled);
      }
    }
  }

  if (!bRetVal)
  {
   UnInitialize();
  }

  return bRetVal;
}

void COposControl::UnInitialize()
{
  CloseControl();

  if (m_bCreated)
  {
    DestroyWindow();
    m_bCreated = FALSE;
  }

  m_Availability = SCOT_DA_NOT_CONFIGURED;
}
