//////////////////////////////////////////////////////////////////////
// CSCOT5992SigCap methods - these use an OPOS 5992 SigCap control
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _CPPUNIT
#include "Common.h"                 // common includes
#endif // _CPPUNIT

#include "SCOT5992SigCap.h"

#define COMP_ID ID_OD
#define T_ID    _T("SCOT5992 SigCap")

CSCOT5992SigCap::CSCOT5992SigCap()
{
}


CSCOT5992SigCap::~CSCOT5992SigCap()
{
}

long CSCOT5992SigCap::ClearDisplay()
{
  long rc = 0;
  long myLong = 0;
  _TCHAR ClearCommand[] = {0x05, 0x04, 0x31, 0x30, 0x0 };
  CString string = ClearCommand;
  BSTR bstr = string.AllocSysString();
  rc = m_Sigcap.DirectIO(0, &myLong, &bstr);
  SysFreeString(bstr);
  return rc;
}

