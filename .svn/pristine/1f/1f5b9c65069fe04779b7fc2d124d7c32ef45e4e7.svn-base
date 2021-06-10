//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTB.CPP
//
// TITLE: TB SMState functions, implement all SMState::TB* functions
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
#include "common.h"
#include "SMState.h"
#include "SCOTMultiStringList.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SmStateTB")

#define MAX_AMKEYS	50	// change this 50 to the required number of keys on this assistmode screen

#define SCOTSSF_EVENT_LOG _T("SCOTSSF")        // EventLog Source

USHORT SMState::m_nAssistKeyVirtualKeyValue[200];

CNCREventLog   ssfEventLog ;   // eventlog object to log events in NT eventlog

USHORT SMState::TBGetKeyCode(const int nKey)
{
	CString csKeyName;
	
  // TAR 139814 -- test scotssfmsg file
  HRESULT retVal = ssfEventLog.Open(SCOTSSF_EVENT_LOG);
	ASSERT(retVal == S_OK);
	if (retVal == S_FALSE)
	{
	  TRACE(_T("Error in CNCREventLog object creation"));
	  // TAR 243814 Move LogEvent function here.
	  /*ssfEventLog.LogEvent(evlClassBasic,
			   evlTypeInformation,
			   SM_CAT_CONFIGURATION,
			   SCOTSSF_SM_MSG_START);*/ // RFC 262774 - Scotssf message file will need to define catagories
	}
	ssfEventLog.Close () ;
  // 
   if (TBGetKeyLabel(nKey,csKeyName))	// if there is a label
	{
		return uKeyCodeList[nKey];
	}
  	
  return 0;
	//return SMStateBase::TBGetKeyCode(nKey);  // default is zero based
}


/////////////////////////////////////////////////////////////////////////////
long SMState::TBGetKeyLabel(const int nKey,CString &csLabel)
{
	csLabel = _T("");
	if ((nKey > 0) && (nKey <= MAX_AMKEYS))
	{
		csLabel = csKeyLabeList[nKey];
	}
	return csLabel.GetLength();
	// return SMStateBase::TBGetKeyLabel(nKey, csLabel);  // default is zero based
}


USHORT *SMState::TBEmulatorKeys(void)
{
	static USHORT nKeys[MAX_AMKEYS+1] = 
		{	0,
				1,   2,   3,  4,   5,
				6,   7,   8,  9,  10,
				11,  12, 13, 14,  15,
				16,  17, 18, 19,  20,
				21,  22, 23, 24,  25,
				26,  27, 28, 29,  30,
				31,  32, 33, 34,  35,
				36,  37, 38, 39,  40,
				41,  42, 43, 44,  45,
				46,  47, 48, 49,  50
		};
	return nKeys;
	// return SMStateBase::TBEmulatorKeys();  // default is zero based
}

#define ASSISTKEY_LOCAL       1
#define ASSISTKEY_REMOTE      2
#define ASSISTKEY_INSIDE_TXN  4
#define ASSISTKEY_OUTSIDE_TXN 8

void SMState::TBLoadAssistModeKeyboard(void)
{
	
	CSCOTMultiStringList* csStringList;
	csKeyLabeList[0] = _T(""); 
	CString csFileName = co.csConfigFilePath + _T("\\AssistKeyData.dat");
	csStringList = new CSCOTMultiStringList(csFileName);
	BOOL bRetStatus = csStringList->Initialize();
	if (!bRetStatus)
	{
		CString csMsg;
		csMsg.Format(_T("Could not build AssistKey list with file: %s\n"), csFileName);
		TRACE(_T("%s"), csMsg);     // debug output window
		trace(L0, _T("%s"), csMsg); // log file
	}
	else
	{
      BOOL bRemoteDeviceMode = ps.RemoteMode();

		for(int i = 1; i < csStringList->GetSize(); i++)
		{
			csKeyLabeList[i] = csStringList->GetLabel(i);
			uKeyCodeList[i] = csStringList->GetCode(i);
			m_nAssistKeyVirtualKeyValue[i] = _ttol(csStringList->GetFieldValue(i, 3));  // Get Virtual Key value
			if (csKeyLabeList[i].GetLength())
			{
                 int nKeyMode = 0;
                 int nCurrentMode = 0;
    		    if (csStringList->GetStatus(i))
                nKeyMode = _ttol(csStringList->GetFieldValue(i, 2));

                nCurrentMode |= (!bRemoteDeviceMode ? ASSISTKEY_LOCAL : 0);
                nCurrentMode |= (bRemoteDeviceMode ? ASSISTKEY_REMOTE : 0);
                nCurrentMode |= ((fInSignedOn || fInSignedOff) ? ASSISTKEY_INSIDE_TXN : 0);
                nCurrentMode |= (!(fInSignedOn || fInSignedOff) ? ASSISTKEY_OUTSIDE_TXN : 0);

                if ((nCurrentMode & nKeyMode) != nCurrentMode)
                {
                uKeyCodeList[i] = 0;
                }
			}  // of if (csKeyLabeList[i].GetLength())
           
      } // end of for loop
	} // of else case of if (bRetStatus)
	csStringList->UnInitialize();
	delete csStringList;
    // New UI
    m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_FALSE );
	ps.SetKeyLabel(_T("Key1"), -1, NULL);
	ps.SetKeyLabel(_T("Key16"), -1, NULL);
	ps.SetKeyLabel(_T("Key18"), -1, NULL);
	ps.SetKeyLabel(_T("Key27"), -1, NULL);
	ps.SetKeyLabel(_T("Key28"), -1, NULL);
	ps.SetKeyLabel(_T("Key29"), -1, NULL);
	ps.SetKeyLabel(_T("Key49"), -1, NULL);
    m_pAppPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
    // NewUI
} // end of function, void SMState::TBLoadAssistModeKeyboard(void)
