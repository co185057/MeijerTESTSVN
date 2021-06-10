//////////////////////////////////////////////////////////////////////
// CSCOTDMTopaz methods - these use the SCOT 1.1 DM SigCap interface for Topaz
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _CPPUNIT
#include "Common.h"                 // common includes
#endif // _CPPUNIT

#include "SCOTDMTopaz.h"
#include "DMProcedures.h"           // DM procedures

#define COMP_ID ID_OD
#define T_ID    _T("SCOTDM Topaz")

CSCOTDMTopaz::CSCOTDMTopaz()
{

}


CSCOTDMTopaz::~CSCOTDMTopaz()
{

}


long CSCOTDMTopaz::Enable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTDMTopaz::Enable"));
	
  rc = dm.SigCapEnable();			  // enable the topaz sig cap
	
  trace(L6,_T("-CSCOTDMTopaz::Enable %d"),rc);
  return rc;
}

long CSCOTDMTopaz::Disable()
{
  long rc = 0;
  trace(L6,_T("+CSCOTDMTopaz::Disable"));
	
  rc = dm.SigCapDiable();			  // disable the encryptor
	
  trace(L6,_T("-CSCOTDMTopaz::Disable %d"),rc);
  return rc;
}


long CSCOTDMTopaz::Initialize(LPCTSTR profile, CWnd* pParentWnd, CWnd *pMainWnd, UINT nID)
{
  long rc = 0;
  
  m_csProfile = profile;

  // All of our initialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}

long CSCOTDMTopaz::UnInitialize()
{
  long rc = 0;
  
  // All of our uninitialization is done by DMInterface, so this
  // is a no-brainer.

  return rc;
}


long CSCOTDMTopaz::BeginCapture(LPCTSTR szCaptureForm, HWND hDrawWnd)
{
  long rc = 0;
  m_bHaveValidSig = FALSE;
  rc = Enable();
  if (rc == 0)
  {
		/******cm150000: Commented Out In Merged Code******/
    //rc = 	dm.SetSigCapNotifyStyle(1);
		/******cm150000: End Comment In Merged Code********/
    if (rc == 0)
    {
      rc = dm.SigCapSetWindowHandle((long*)&hDrawWnd);
      if (rc == 0)
      {
	      rc = dm.SigCapBeginCapture(szCaptureForm);
      }
    }
  }

  return rc;
}

long CSCOTDMTopaz::EndCapture()
{
  long rc = dm.SigCapEndCapture() ;
  if (rc == 0)
  {
    // We will assume that if this works we have a valid sig.  Not
    // sure that is necessarily the case, though
    m_bHaveValidSig = TRUE;
  }

  return rc;
}

long CSCOTDMTopaz::Get5991Format(BSTR* pData)
{
  long rc = dm.SigCapDataTo5991Format(pData);
  return rc;
}

