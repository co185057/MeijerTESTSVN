// SCOTMotionSensor.cpp: interface for the CSCOTMotionSensor and CSCOTOnTrak classes.
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"                 // common includes
#include "SCOTMotionSensor.h"
#include "EASMotionSensor.h"		
#include "Scotmot.h"
#include "DMProcedures.h"           // DM procedures

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMP_ID ID_OD
#define T_ID    _T("SCOTMotionSensor")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction for CSCOTMotionSensor
//////////////////////////////////////////////////////////////////////

CSCOTMotionSensor::CSCOTMotionSensor()
{
}

CSCOTMotionSensor::~CSCOTMotionSensor()
{
}

//////////////////////////////////////////////////////////////////////
// CSCOTOnTrakMotionDetector methods 
//////////////////////////////////////////////////////////////////////

CSCOTOnTrak::CSCOTOnTrak()
{
}

CSCOTOnTrak::~CSCOTOnTrak()
{
}

long CSCOTOnTrak::Enable()
{
  long rc = 0;
  
  m_EASMotionSensor.SetDeviceEnabled(TRUE);
  rc = m_EASMotionSensor.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOnTrak::Disable()
{
  long rc = 0;
  
  m_EASMotionSensor.SetDeviceEnabled(FALSE);
  rc = m_EASMotionSensor.GetResultCode();
  return TranslateOPOSRetToSCOTRet(rc);
}

long CSCOTOnTrak::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;

  m_csProfile = profile;

  m_pMainWnd = pMainWnd;

  if (!m_EASMotionSensor.Initialize(profile, pParentWnd, nID, true))
  {
    CString csErr = m_EASMotionSensor.GetErrorFunc();
    ScotError(RETURN, SM_CAT_STARTUP_INIT, SCOTAPP_OPOS_CTRL_INIT_FAILURE, _T("%s|%s"), 
          profile, csErr);
    rc = -1;
  }
  
  return rc;
}

long CSCOTOnTrak::UnInitialize()
{
  long rc = 0;
  
  m_EASMotionSensor.UnInitialize();
  return rc;
}

void CSCOTOnTrak::DataEvent(long Status)
{
  long DMStatus;

  switch ( Status ) 
  {
	case MOT_STATUS_OK:
	  DMStatus = MOT_STATUS_OK;
	  break;

	case MOT_MOTION:
	  DMStatus = MOT_MOTION;
	  break;

	case MOT_NO_MOTION:
	  DMStatus = MOT_NO_MOTION;
	  break;

	case MOT_STATUS_FAILURE:
	  // send a failure message to the event viewer
	  ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_DEVFACTORY_MOTION_SENSOR_FAILED,_T("%d"), Status);
	  // want to simulate a motion / no_motion so the tag deactivate will go away
	  DMStatus = MOT_MOTION;
	  mo.PostDM(DM_DATA, DMCLASS_EASMOTION, sizeof(VT_I4), (void*)&DMStatus);	// post the mmotion message to DM
	  DMStatus = MOT_NO_MOTION; // now send no_motion
	  break;
  }

  mo.PostDM(DM_DATA, DMCLASS_EASMOTION, sizeof(VT_I4), (void*)&DMStatus);
  
}


void CSCOTOnTrak::ErrorEvent(LONG ResultCode, 
                             LONG ResultCodeExtended, 
                             LONG ErrorLocus, 
                             LONG* pErrorResponse)
{
}

// use this call so we can keep using the OposControl.cpp template that Scot Barnhart wrote
void CSCOTOnTrak::Claim(bool bClaim)
{
}

// use this call so we can keep using the OposControl.cpp template that Scot Barnhart wrote
void CSCOTOnTrak::SetDataEventEnabled(bool bEnable)
{
}

