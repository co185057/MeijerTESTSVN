//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMBagAndEAS.CPP
//
// TITLE: Class header for Scan and Bag State
//
// $Header:
//
// AUTHOR:    Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMBagAndEAS.h"           // MGV this state


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("BagAndEAS")

IMPLEMENT_DYNCREATE(SMBagAndEAS, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

////////////////////////////
SMBagAndEAS::SMBagAndEAS()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////////////////////////
SMStateBase* SMBagAndEAS::Initialize()
{

  SMStateBase* returnState = SMBagAndEASBase::Initialize();

	
  // Get the Registry Values of the scale or Emulator, 
  long nScaleDisabled = GetDWORDValue("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\ScotApp\\Controls\\SmartScale", "DisableBagScale");
  // Compare the value to see if the emulator is running or scale is disabled, if so, skip bagging.
  if (nScaleDisabled == 1 && !m_bEASHappened )  
  {
    return SMBagAndEASBase::PSButton1();
  }


  return returnState;

}


////////////////////////////////////////////////////////
// Check the Registry value for the SmartScale in order to determine if the lane is
// a Fast Pay lane. (if the returned data value is 1, the smartscale is disabled.  JYC
long SMBagAndEAS::GetDWORDValue(const CString& key,const CString& value)
{
	long data = 0;
  DWORD iDataLength = sizeof(data);
	DWORD type = REG_DWORD;
  LONG lRC;
  HKEY hKey;


  lRC = ::RegOpenKeyEx(
    HKEY_LOCAL_MACHINE,
    key,
		0,
    KEY_QUERY_VALUE,
    &hKey);

  if (lRC==ERROR_SUCCESS)
  {
    lRC = ::RegQueryValueEx(
      hKey,
      value,
      NULL, 
      &type, 
		  (BYTE *)&data,
      &iDataLength);

    ::RegCloseKey(hKey);

    if (lRC!=ERROR_SUCCESS)
    {
      data = 0;
    }
  }

  return data;
}