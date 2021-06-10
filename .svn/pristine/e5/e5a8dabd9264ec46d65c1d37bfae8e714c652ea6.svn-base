//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStatePSBase.CPP
//
// TITLE: PS SMStateBase functions, implement all SMStateBase::PS* functions
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxctl.h>
#include <afxpriv.h>                // for USES_CONVERSION
#include "LaneStateData.h"

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Abstract base state
#include "SMContextHelp.h"          // Context level help state
#include "SMSystemMessage1.h"
#include "SMWaitForRemoteAssistance.h"
#include "ScotStringlist.h"
#include "SMTakeCard.h"
#include "SMInProgress.h"
#include "SMRAPDataNeeded.h"
#include "qping2.h"

//USSF Start
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
//USSF End
#else
#include "FakeSMStateBase.h"
#endif //_CPPUNIT
#include "SMCmDataEntryBase.h"


#ifndef _CPPUNIT
// TAR 285178 start
#define STUCK_KEY_THRESHOLD     50
#define STUCK_KEY_MSEC          100
#define STUCK_KEY_END_MSEC      5000
// TAR 285178 end
#endif //_CPPUNIT

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("SMpsBase")

#ifndef _CPPUNIT
// PS last status
PSEVT SMStateBase::nPSLastEvt = PS_UKEVT;  // last PS message type
int  SMStateBase::nPSLastParm = 0;         // last PS message parm 1
int  SMStateBase::nPSLastWParam = 0;         // last PS message parm 2
static long gSaveChOnKeyStroke = 0;

#endif // _CPPUNIT
static const TCHAR SOUNDPATH[] = _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion");

#ifndef _CPPUNIT
// Copied from PSThread.cpp
#define KBD_STATE_LEN       256
#define SET_KEY_STATE(statePtr, key) (statePtr[key] |= 0x80)
#define DEFINE_PSEVENTNAMES

#ifdef DEFINE_PSEVENTNAMES
static const struct sPSEventDescNodeType {
   int key;
   LPCTSTR szname;
}PSEventNameList[]= 
{
   {PS_UKEVT,            _T("PS_UKEVT")},
   {PS_HELP,            _T("PS_HELP")},
   {PS_BUTTON1,            _T("PS_BUTTON1")},
   {PS_BUTTON2,            _T("PS_BUTTON2")},
   {PS_BUTTON3,            _T("PS_BUTTON3")},
   {PS_BUTTON4,            _T("PS_BUTTON4")},
   {PS_BUTTON5,            _T("PS_BUTTON5")},
   {PS_BUTTON6,            _T("PS_BUTTON6")},
   {PS_BUTTON7,            _T("PS_BUTTON7")},
   {PS_BUTTON8,            _T("PS_BUTTON8")},
   {PS_MENUKEY1,            _T("PS_MENUKEY1")},
   {PS_MENUKEY2,            _T("PS_MENUKEY2")},
   {PS_MENUKEY3,            _T("PS_MENUKEY3")},
   {PS_MENUKEY4,            _T("PS_MENUKEY4")},
   {PS_MENUKEY5,            _T("PS_MENUKEY5")},
   {PS_MENUKEY6,            _T("PS_MENUKEY6")},
   {PS_MENUKEY7,            _T("PS_MENUKEY7")},
   {PS_MENUKEY8,            _T("PS_MENUKEY8")},
   {PS_MENUKEY9,            _T("PS_MENUKEY9")},
   {PS_MENUKEY10,           _T("PS_MENUKEY10")},
   {PS_MENUKEY11,           _T("PS_MENUKEY11")},
   {PS_MENUKEY12,           _T("PS_MENUKEY12")},
   {PS_MENUKEY13,           _T("PS_MENUKEY13")},
   {PS_MENUKEY14,           _T("PS_MENUKEY14")},
   {PS_MENUKEY15,           _T("PS_MENUKEY15")},
   {PS_RECEIPTUP,            _T("PS_RECEIPTUP")},
   {PS_RECEIPTDOWN,            _T("PS_RECEIPTDOWN")},
   {PS_LISTLOOKUP,            _T("PS_LISTLOOKUP")},
   {PS_HELPTOPICS,            _T("PS_HELPTOPICS")},
   {PS_ENTERKEY,            _T("PS_ENTERKEY")},
   {PS_CANCELKEY,            _T("PS_CANCELKEY")},
   {PS_CLEARKEY,              _T("PS_CLEARKEY")},
   {PS_NUMERICKEY,             _T("PS_NUMERICKEY")},
   {PS_DECIMALKEY,             _T("PS_DECIMALKEY")},
   {PS_ALPHAKEY,               _T("PS_ALPHAKEY")},
   {PS_VIDEODONE,              _T("PS_VIDEODONE")},
   {PS_UKRELEASE,              _T("PS_UKEVT")},
   {PS_UKSELCHANGE,            _T("PS_UKSELCHANGE")},
   {PS_ASSISTKEY,              _T("PS_ASSISTKEY")},
   {PS_ACTIVITY,               _T("PS_ACTIVITY")},
   {PS_MAINMENUKEY,            _T("PS_MAINMENUKEY")},
   {PS_SPACEKEY,               _T("PS_SPACEKEY")},
   {PS_GOBACK,                 _T("PS_GOBACK")},
   {PS_REMOTE_DISCONNECTED,    _T("PS_REMOTE_DISCONNECTED")},
   {PS_KEYSTROKE,              _T("PS_KEYSTROKE")},
   {PS_BUTTONTOGGLE,           _T("PS_BUTTONTOGGLE")},
   {PS_BUTTONLANE1STATUS,            _T("PS_BUTTONLANE1STATUS")},
   {PS_BUTTONLANE2STATUS,            _T("PS_BUTTONLANE2STATUS")},
   {PS_BUTTONLANE3STATUS,            _T("PS_BUTTONLANE3STATUS")},
   {PS_BUTTONLANE4STATUS,            _T("PS_BUTTONLANE4STATUS")},
   {PS_BUTTONLANE5STATUS,            _T("PS_BUTTONLANE5STATUS")},
   {PS_BUTTONLANE6STATUS,            _T("PS_BUTTONLANE6STATUS")},
   {PS_BUTTONLANE7STATUS,            _T("PS_BUTTONLANE7STATUS")},
   {PS_BUTTONLANE8STATUS,            _T("PS_BUTTONLANE8STATUS")},
   {PS_REMOTEBUTTON1,            _T("PS_REMOTEBUTTON1")},
   {PS_REMOTEBUTTON2,            _T("PS_REMOTEBUTTON2")},
   {PS_REMOTEBUTTON3,            _T("PS_REMOTEBUTTON3")},
   {PS_REMOTEBUTTON4,            _T("PS_REMOTEBUTTON4")},
   {PS_LISTLOOKUPTAB,            _T("PS_LISTLOOKUPTAB")},
   {PS_LISTSCROLL,               _T("PS_LISTSCROLL")},
   {PS_CONNECTREMOTE,            _T("PS_CONNECTREMOTE")},
   {PS_SWITCHCONTEXT,            _T("PS_SWITCHCONTEXT")},
   {PS_BACKSPACEKEY,             _T("PS_BACKSPACEKEY")},
   {PS_SHIFTKEY,                 _T("PS_SHIFTKEY")},
   {PS_OTHERSPECIALKEY,           _T("PS_OTHERSPECIALKEY")},
   {PS_ALPHANUMERICKEY,           _T("PS_ALPHANUMERICKEY")},
   {PS_RAPSIGNON,                   _T("PS_RAPSIGNON")},
   {PS_RAPSIGNOFF,                  _T("PS_RAPSIGNOFF")},
   {PS_SWITCHCONTEXTOTHER,          _T("PS_SWITCHCONTEXTOTHER")},
   {PS_RAPSCANDATA,                 _T("PS_RAPSCANDATA")},
   {PS_REMOTEDATA,                  _T("PS_REMOTEDATA")},
   {PS_CONNECTREMOTELAUNCHPAD,      _T("PS_CONNECTREMOTELAUNCHPAD")},
   {PS_ASSISTACCEPT,				_T("PS_ASSISTACCEPT")},				
   {PS_ASSISTDENIED,				_T("PS_ASSISTDENIED")},
   {PS_COMMANDBUTTON,               _T("PS_COMMANDBUTTON")},
   {PS_HOTKEY,                 _T("PS_HOTKEY")},
   {PS_CHARKEY,                _T("PS_CHARKEY")},
   {PS_ALTGRKEY,               _T("PS_ALTGRKEY")},
   {PS_COMMANDLIST,            _T("PS_COMMANDLIST")},   //RFQ 2605 - new event for CmdList
   {PS_ADDOPERATOR,            _T("PS_ADDOPERATOR")},
   {PS_ADDOPERATOR_AND_CONNECT,_T("PS_ADDOPERATOR_AND_CONNECT")},
   {PS_SYMBOLKEY,               _T("PS_SYMBOLKEY")},
// RFQ 7477
   {PS_RAPCHECKIN,            _T("PS_RAPCHECKIN")},   //POS81634
   {PS_RAPCHECKOUT,            _T("PS_RAPCHECKOUT")},   //POS81634
   {PS_RAPSCANNERENGAGED,      _T("PS_RAPSCANNERENGAGED")}, // POS181195
   {PS_RAPSCANNERAVAILABLE,    _T("PS_RAPSCANNERAVAILABLE")}, // POS181195
   {PS_RAPSCANNERUNAVAILABLE,  _T("PS_RAPSCANNERUNAVAILABLE")}, // POS181195
   {PS_RAPSCANNERDEACTIVATED,  _T("PS_RAPSCANNERDEACTIVATED")}, // CRD149415
   { PS_RAPPRINTERBITMAPLOADED,  _T("PS_RAPPRINTERBITMAPLOADED") },
   { PS_RAPPRINTERBITMAPNOTLOADED,  _T("PS_RAPPRINTERBITMAPNOTLOADED") },
   { PS_RAPPRINTERBITMAPLOADFAILED,  _T("PS_RAPPRINTERBITMAPLOADFAILED") },
// RFQ 7477
   {PS_LASTEVT,          _T("PS_LASTEVT")} // add entries above this line
};
#endif // DEFINE_PSEVENTNAMES

#endif //_CPPUNIT
#ifndef _CPPUNIT

TCHAR SMStateBase::VirtKeyToTChar(UINT vKey, long shiftState)
{
  BYTE keyboardState[KBD_STATE_LEN];
  UINT scanCode;
  BYTE asciiCodes[2];
  int ret;
  TCHAR returnChar = 0;

  scanCode = MapVirtualKey(vKey, 0);

  ZeroMemory(keyboardState, KBD_STATE_LEN);
  SET_KEY_STATE(keyboardState, vKey);

  if (shiftState & SHIFT_MASK)
    SET_KEY_STATE(keyboardState, VK_SHIFT);
  if (shiftState & CTRL_MASK)
    SET_KEY_STATE(keyboardState, VK_CONTROL);
  if (shiftState & ALT_MASK)
    SET_KEY_STATE(keyboardState, VK_MENU);

  ret = ToAscii(vKey, scanCode, keyboardState, (LPWORD) asciiCodes, 0);
  if (ret == 1)
  {
    returnChar = asciiCodes[0];
  }
  else
  {
    // If ret != 1, then we don't have a regular ASCII char, so we will return
    // 0 here, and let CallbackHandler deal with the Virtual key code instead
    // of an ASCII code.
    returnChar = 0;
  }

  return returnChar;
}

/////////////////////////////////////////////////
SMStateBase *SMStateBase::PSParse(const MessageElement *me)
{
  // save all the PS parameters
  nPSLastEvt = (PSEVT)me->psInfo.nEvt;
  nPSLastParm = me->psInfo.lParm;
  nPSLastWParam = me->psInfo.wParm;
  
  // NewUI
  CString* pTemp;
  CString csTemp;
  // TAR 285178 start
  static unsigned long lLastKeystroke = 0;
  static unsigned long lLastKeystrokeTime = 0;
  static unsigned long lEventLogged = 0;
  static unsigned long lKeystrokeCount = 0;
  unsigned long lCurrKeystroke;
  unsigned long lCurrPSEventTime;

  SMStateBase* pStateKeystrock=NULL;
  CString csCurrentContext = ps.GetCurrentContext(); //SSCOADK-5341
  // TAR 285178 end

  // NewUI
  trace(L6,_T("Parse PS %d, wparam:0x%x, lparam:0x%x"),nPSLastEvt,me->psInfo.wParm, nPSLastParm);
  
  //+SSCOADK-5341
  if(ShouldDiscardEvent(me->psInfo))
  {
    trace(L6,_T("Ignoring click event [%d]. Click originated in context [%s], but current context is [%s]."),
    me->psInfo.nEvt,
    me->psInfo.pszContext,
    (LPCTSTR)csCurrentContext);
    return STATE_NULL;
  }
  //-SSCOADK-5341

  // TAR 285178 start
  if (   (lEventLogged == lLastKeystroke)
      && (lEventLogged != 0)
      && (lKeystrokeCount > STUCK_KEY_THRESHOLD))
  {
     lCurrPSEventTime = GetTickCount();
     if ((lCurrPSEventTime - lLastKeystrokeTime) > STUCK_KEY_END_MSEC)
     {
        trace(L4, _T("PSParse detecting key unstuck, last event wparam:0x%x & lparam:0x%x, keystroke count %d, timeout %d msec."), 
                     (lEventLogged >> 16), 
                     (lEventLogged & 0xFFFF), 
                     lKeystrokeCount, 
                     (lCurrPSEventTime - lLastKeystrokeTime));
        ScotError(INFO, SM_CAT_DEVICE_PERIPHERAL, SCOTAPP_KEY_NO_LONGER_STUCK, _T("%d"), lKeystrokeCount);
        lKeystrokeCount = 0;
        lEventLogged = 0;
     }
  }
  // TAR 285178 end
	
  { // these brackets help the auto-format
	if (nPSLastEvt == PS_KEYSTROKE)
	{
        // TAR 285178 start - detect and ignore "stuck" keyboard
        lCurrKeystroke = (me->psInfo.wParm << 16) | (me->psInfo.lParm & 0xFFFF);
        if (lCurrKeystroke == lLastKeystroke)
        {
            lCurrPSEventTime = GetTickCount();
            if ((lCurrPSEventTime - lLastKeystrokeTime) < STUCK_KEY_MSEC)
            {
               if (lKeystrokeCount++ >= STUCK_KEY_THRESHOLD)
               {
                  if (lEventLogged != lCurrKeystroke)
                  {
                     ScotError(RETURN, SM_CAT_DEVICE_PERIPHERAL, SCOTAPP_KEY_STUCK, _T("wParm=0x%x, lParm=0x%x"), me->psInfo.wParm, me->psInfo.lParm);
                     lEventLogged = lCurrKeystroke;
                  }
                  trace(L4,_T("Parse PS %d ignoring repeated keystroke, wparam:0x%x, lparam:0x%x"), nPSLastEvt, me->psInfo.wParm, nPSLastParm);
                  lLastKeystrokeTime = lCurrPSEventTime;
                  return STATE_NULL;
               }
            }
            lLastKeystrokeTime = lCurrPSEventTime;
        }
        else
        {
            lKeystrokeCount = 0;
        }
        lLastKeystroke = lCurrKeystroke;
        // TAR 285178 end

		// further process keystroke and determine which key is actually pressed
      pStateKeystrock=PSKeystroke(me->psInfo.wParm, me->psInfo.lParm);
	}
	else
	{
		gSaveChOnKeyStroke = me->psInfo.lParm; 
	}
	sExtStateEventNotifyIn  sNotifyIn;
	sExtStateEventNotifyOut sNotifyOut;
	sNotifyIn.InitStruct();
	sNotifyOut.InitStruct();
	SMStateBase* pState = PreNotifySCOTEvent2TB(me, sNotifyIn, sNotifyOut);
	if (!sNotifyOut.bAllowDefaultProcessing)
	{
		trace(L4, _T("PreNotifyPSEvent2TB() returns to a new state"));
		return pState;
	}


    switch (nPSLastEvt)
    {
    case PS_HELP:
	  //USSF Start
	  USSFHOOK(_T("PSXButtonHelp"), nextState = PSButtonHelp(), return nextState;);
      //return PSButtonHelp();
	  //USSF End


    case PS_ALPHANUMERICKEY:
		 //USSF Start
		 USSFDATA(_T("PSXALPHANUMERICKEY"), gSaveChOnKeyStroke);
		 USSFDATA(_T("PSXDATA"), gSaveChOnKeyStroke);
		 USSFHOOK(_T("PSXKeyAlphaNumeric"), nextState = PSAlphaNumericKey(gSaveChOnKeyStroke), return nextState;);
		 //return PSAlphaNumericKey(gSaveChOnKeyStroke);
		 //USSF End
    case PS_DECIMALKEY:
		 //USSF Start
		 USSFDATA(_T("PSXDECMALKEY"), gSaveChOnKeyStroke);
		 USSFDATA(_T("PSXDATA"), gSaveChOnKeyStroke);
		 USSFHOOK(_T("PSXKeyDecimal"), nextState = PSDecimalKey(gSaveChOnKeyStroke), return nextState;);
		 //return PSDecimalKey(gSaveChOnKeyStroke);
		 //USSF End
    case PS_OTHERSPECIALKEY:
		 //USSF Start
		 USSFDATA(_T("PSXOTHERSPECIALKEY"), gSaveChOnKeyStroke);
		 USSFDATA(_T("PSXDATA"), gSaveChOnKeyStroke);
		 USSFHOOK(_T("PSXKeyOtherSpecial"), nextState = PSOtherSpecialKeys(gSaveChOnKeyStroke), return nextState;);
		 //return PSOtherSpecialKeys(gSaveChOnKeyStroke);
		 //USSF End
    case PS_SHIFTKEY:
		 //USSF Start
		 USSFHOOK(_T("PSXKeyShift"), nextState = PSShiftKey(), return nextState;);
		 //return PSShiftKey();
		 //USSF End
    case PS_ALTGRKEY:
		 return PSAltGrKey();
    case PS_SYMBOLKEY:
		 return PSSymbolKey();
    case PS_ENTERKEY:
		 //USSF Start
		 USSFHOOK(_T("PSXKeyEnter"), nextState = PSEnterKey(), return nextState;);
		 //return PSEnterKey();
		 //USSF End
    case PS_CANCELKEY:
		 //USSF Start
		 USSFHOOK(_T("PSXKeyCancel"), nextState = PSCancelKey(), return nextState;);
		 //return PSCancelKey();
		 //USSF End
    case PS_BACKSPACEKEY:
		 //USSF Start
		 USSFHOOK(_T("PSXKeyBackSpace"), nextState = PSBackSpaceKey(), return nextState;);
		 //return PSBackSpaceKey();
		 //USSF End
    case PS_CLEARKEY:
		 //USSF Start
		 USSFHOOK(_T("PSXKeyClear"), nextState = PSClearKey(), return nextState;);
		 //return PSClearKey();
		 //USSF End
	 case PS_GOBACK:
		 //USSF Start
		 USSFHOOK(_T("PSXButtonGoBack"), nextState = PSButtonGoBack(), return nextState;);
		 //return PSButtonGoBack();
		 //USSF End
    case PS_BUTTON1:
	  //USSF Start
	  USSFHOOK(_T("PSXButton1"), nextState = PSButton1(), return nextState;);
      //return PSButton1();
	  //USSF End
    case PS_BUTTON2:
	  //USSF Start
	  USSFHOOK(_T("PSXButton2"), nextState = PSButton2(), return nextState;);
      //return PSButton2();
	  //USSF End
    case PS_BUTTON3:
	  //USSF Start
	  USSFHOOK(_T("PSXButton3"), nextState = PSButton3(), return nextState;);
      //return PSButton3();
	  //USSF End
    case PS_BUTTON4:
	  //USSF Start
	  USSFHOOK(_T("PSXButton4"), nextState = PSButton4(), return nextState;);
      //return PSButton4();
	  //USSF End
    case PS_BUTTON5:
	  //USSF Start
	  USSFHOOK(_T("PSXButton5"), nextState = PSButton5(), return nextState;);
      //return PSButton5();
	  //USSF End
    case PS_BUTTON6:
	  //USSF Start
	  USSFHOOK(_T("PSXButton6"), nextState = PSButton6(), return nextState;);
      //return PSButton6();
	  //USSF End
    case PS_BUTTON7:
	  //USSF Start
	  USSFHOOK(_T("PSXButton7"), nextState = PSButton7(), return nextState;);
      //return PSButton7();
	  //USSF End
    case PS_BUTTON8:
	  //USSF Start
	  USSFHOOK(_T("PSXButton8"), nextState = PSButton8(), return nextState;);
      //return PSButton8();
	  //USSF End
    case PS_MENUKEY1:
	  //USSF Start
	  USSFHOOK(_T("PSXKeyMenu1"), nextState = PSMenuKey1(), return nextState;);
      //return PSMenuKey1();
	  //USSF End
    case PS_MENUKEY2:
	  //USSF Start
	  USSFHOOK(_T("PSXKeyMenu2"), nextState = PSMenuKey2(), return nextState;);
      //return PSMenuKey2();
	  //USSF End
    case PS_MENUKEY3:
      return PSMenuKey3();
    case PS_MENUKEY4:
      return PSMenuKey4();
    case PS_MENUKEY5:
      return PSMenuKey5();
    case PS_MENUKEY6:
      return PSMenuKey6();
    case PS_MENUKEY7:
      return PSMenuKey7();
    case PS_MENUKEY8:
      return PSMenuKey8();
    case PS_MENUKEY9:
      return PSMenuKey9();
    case PS_MENUKEY10:
      return PSMenuKey10();
    case PS_MENUKEY11:
      return PSMenuKey11();
    case PS_MENUKEY12:
      return PSMenuKey12();
    case PS_MENUKEY13:
      return PSMenuKey13();
    case PS_MENUKEY14:
      return PSMenuKey14();
    case PS_MENUKEY15:
      return PSMenuKey15();
    case PS_HELPTOPICS:
      return PSHelpTopics();
    case PS_LISTLOOKUP:
	  if(0 == me->psInfo.lParm)
	  {
		  pTemp =(CString *) me->psInfo.wParm;
		  csTemp= *pTemp;
		  delete pTemp;
		  //USSF Start
		  USSFDATA(_T("PSXLISTINDEX"), csTemp);
		  USSFHOOK(_T("PSXListLookup"), nextState = PSListLookup(csTemp), return nextState;);
		  //return PSListLookup(csTemp);
		  //USSF End
	  }
	  else if(1 == me->psInfo.lParm)
	  {
		  _TCHAR buf[50];
		  _itot(me->psInfo.wParm,buf,10);
		  //USSF Start
		  USSFDATA(_T("PSXLISTINDEX"), buf);
		  USSFHOOK(_T("PSXListLookup"), nextState = PSListLookup(buf), return nextState;);
		  //return PSListLookup(buf);
		  //USSF End
	  }
    case PS_RECEIPTUP:
	  //USSF Start
	  USSFHOOK(_T("PSXKeyReceiptUp"), nextState = PSReceiptUp(), return nextState;);
      //return PSReceiptUp();
	  //USSF End
    case PS_RECEIPTDOWN:
	  //USSF Start
	  USSFHOOK(_T("PSXKeyReceiptDown"), nextState = PSReceiptDown(), return nextState;);
      //return PSReceiptDown();
	  //USSF End
    case PS_VIDEODONE:
	  //USSF Start
	  USSFHOOK(_T("PSXKeyVideoDone"), nextState = PSVideoDone(), return nextState;);
      //return PSVideoDone();
	  //USSF End
	case PS_SPACEKEY:	
	  //USSF Start
	  USSFHOOK(_T("PSXKeySpace"), nextState = PSSpaceKey(), return nextState;);
	  //return PSSpaceKey();
	  //USSF End
    case PS_NUMERICKEY:  // Kept in for "spoof numeric"
	  //USSF Start
	  USSFDATA(_T("PSXNUMERICKEY"), gSaveChOnKeyStroke);
	  USSFDATA(_T("PSXDATA"), gSaveChOnKeyStroke);
	  USSFHOOK(_T("PSXKeyNumeric"), nextState = PSNumericKey(gSaveChOnKeyStroke), return nextState;);
      //return PSNumericKey(gSaveChOnKeyStroke);
	  //USSF End
	case PS_KEYSTROKE:
        {
			 return pStateKeystrock;
        }
	case PS_MAINMENUKEY:
	  //USSF Start (REMOVED)
      return PSMainMenuKey();// AM mode main menu button - TAR# 123183
	  //USSFHOOK(_T("PSXKeyMainMenu"), nextState = PSMainMenuKey(), return nextState;);
	  //USSF End (REMOVED)
    case PS_ASSISTKEY:
      return PSAssistKey(me->psInfo.lParm);
    case PS_COMMANDBUTTON:  //RFQ 986
        {
            pTemp =(CString *) me->psInfo.wParm;
            CString csContext = me->psInfo.pszContext;
		    CString csCommand = *pTemp;
            delete pTemp;
            pTemp =(CString *) me->psInfo.lParm;
            CString csControl = *pTemp;
            delete pTemp;
            return PSCmdButton(csContext, csControl, csCommand);
        }
    case PS_COMMANDLIST:  
        {
            CString csContext = me->psInfo.pszContext;
            pTemp =(CString *) me->psInfo.lParm;
            CString csControl = *pTemp;
            delete pTemp;
			//me->psInfo.lParm is the index of the list item clicked
            return PSCmdList(csContext, csControl, me->psInfo.wParm);
        }
    case PS_ACTIVITY:
      // ignore but reset timeout
      nTimeIdleMsec = 0;            // reset time counter
      return STATE_NULL;

    case PS_SIGNATURE_START:
      return DMSigcapStarted();
    case PS_SIGNATURE_END:
      USSFHOOK(_T("PSXButton1"), nextState = PSButton1(), return nextState;);
	 case PS_REMOTE_DISCONNECTED:
         {
             if (ps.RemoteMode())
             {
				 //USSF Start
				 USSFDATA(_T("PSXONREMOTEDISCONNECTED"), me->psInfo.lParm);
				 USSFDATA(_T("PSXDATA"), me->psInfo.lParm);
				 USSFHOOK(_T("PSXOnRemoteDisconnected"), nextState = PSOnRemoteDisconnected(me->psInfo.lParm), return nextState;);
                 //return PSOnRemoteDisconnected(me->psInfo.lParm);
				 //USSF End
             }
             else
             {
                 return STATE_NULL;
             }
         }
    case PS_BUTTONTOGGLE:
       return PSButtonToggle();
    case PS_BUTTONLANE1STATUS:
    case PS_BUTTONLANE2STATUS:
    case PS_BUTTONLANE3STATUS:
    case PS_BUTTONLANE4STATUS:
    case PS_BUTTONLANE5STATUS:
    case PS_BUTTONLANE6STATUS:
    case PS_BUTTONLANE7STATUS:
    case PS_BUTTONLANE8STATUS:
       return PSButtonLaneStatus((nPSLastEvt - PS_BUTTONLANE1STATUS) + 1);
	case PS_REMOTEBUTTON1:					// RFC 209075 RJF 073102
	   return PSRemoteButton1();
// NewUI
    case PS_LISTLOOKUPTAB:
		nTimeIdleMsec = 0;			 // TAR 300455
		pTemp =(CString *) me->psInfo.lParm;
		csTemp = *pTemp;
		delete pTemp;
		//USSF Start
		USSFDATA(_T("PSXLISTINDEX"), csTemp);
		USSFHOOK(_T("PSXListLookupTab"), nextState = PSListLookupTab(csTemp), return nextState;);
		//return PSListLookupTab(csTemp);
		//USSF End
		
    case PS_LISTSCROLL:
 	  nTimeIdleMsec = 0;			// TAR 300455
	  //USSF Start
	  USSFDATA(_T("PSXLISTSCROLL"), me->psInfo.wParm);
	  USSFDATA(_T("PSXDATA"), me->psInfo.wParm);
	  USSFHOOK(_T("PSXListScroll"), nextState = PSListScroll(me->psInfo.wParm), return nextState;);
      //return PSListScroll(me->psInfo.wParm);
	  //USSF End

    // Rel 3.2 Requirement 4.5.1 - Single Mode RAP
    case PS_SWITCHCONTEXT:  
      g_csAssistModeStateName = CString( *(CString*)(me->psInfo.wParm) );
      trace(L6,_T("Request to switch to AM - Context = %s"),g_csAssistModeStateName);
	  return HandleNewContext(); 
	  
	case PS_CONNECTREMOTE:
	{  
		SMStateBase* pState = PSConnectRemote( CString( *(CString*)(me->psInfo.lParm) ), CString( *(CString*)(me->psInfo.wParm) ) );
		delete (CString*)me->psInfo.wParm;
		delete (CString*)me->psInfo.lParm;
		return pState;
	}
    // Ignore the following
    case PS_ADDOPERATOR_AND_CONNECT :
      trace(L6,_T("PS_ADDOPERATOR_AND_CONNECT ignored"));
      return STATE_NULL;
    case PS_ADDOPERATOR :
      trace(L6,_T("PS_ADDOPERATOR ignored"));
      return STATE_NULL;
    case PS_RAPSIGNON :
      trace(L6,_T("RAP signon ignored"));
      return STATE_NULL;
    case PS_RAPSIGNOFF:
      trace(L6,_T("RAP sign off ignored"));
      return STATE_NULL;
    case PS_SWITCHCONTEXTOTHER:
    { 
      //RFQ 986 - process PS_SWITCHCONTEXTOTHER
      CString csNewContext = me->psInfo.pszContext;
      trace(L6,_T("Context switch to context %s"),csNewContext);
	  return PSSwitchContext((BOOL) me->psInfo.wParm, csNewContext); 
    }
    case PS_RAPSCANDATA:
      trace(L6,_T("RAP ScanData ignored"));
      return STATE_NULL;
    case PS_REMOTEDATA:
      trace(L6,_T("RAP RemoteData ignored"));
      return STATE_NULL;
	case PS_CONNECTREMOTELAUNCHPAD:
	  trace(L6,_T("LaunchPad ConnectRemote ignored"));
	  return STATE_NULL;
	case PS_ASSISTACCEPT:					
		return PSAssistAccepted();
	case PS_ASSISTDENIED:
		return PSAssistDenied();
  //+dp185016 support glory device
	case PS_UKEVT:
		{
			CString strContext = _T("");
			m_pAppPSX->GetContext(strContext);
			if( (strContext.Find( _T("CashManagement") ) != -1) || (strContext.Find( _T("CardManagement") ) != -1)) // tesco cash management
			{
   				return PSKeystroke(me->psInfo.wParm, me->psInfo.lParm);
			}
		}
		break;		//TAR 380227
  //-dp185016
  
   case PS_HOTKEY:
		pTemp =(CString *) me->psInfo.wParm;
		if(pTemp)
      {
         csTemp = *pTemp;
		   delete pTemp;
		   return PSHotKey(csTemp);
      }
      break;

   case PS_CHARKEY:
		pTemp =(CString *) me->psInfo.wParm;
		if(pTemp)
      {
         csTemp = *pTemp;
		   delete pTemp;
         if(csTemp.GetLength()>0)
         {
		      return PSCharKey(csTemp.GetAt(0));
         }
      }
      break;
	}
  }// these brackets help the auto-format
	
  // catch all unknown PS messages
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_SM_PS_UNKNOWN_MSG,
    _T("%d|%x"),
    nPSLastEvt,
    nPSLastParm);

  return STATE_NULL;
}

#endif //_CPPUNIT

void SMStateBase::PSParseFinalProcessing(const MessageElement* me, SMStateBase* pRetState)
{
    CString csStateName = getStateName(pRetState);
    trace(L7, _T("PSParseFinalProcessing - statename = %s"), csStateName);
    if (csStateName.Compare(_T("SMInProgress")) == 0 ||
        csStateName.Compare(_T("SMRapDataNeeded")) == 0)
      return;

    if (me->lParam & NOTIFY_USERINPUTCOMPLETED)
    {
        trace(L7, _T("PSParseFinalProcessing - setting RemoteProxyUserInputCompleted to true"));
        CLaneStateData laneStateData(m_pAppPSX);
        laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// PS base functions not overriden in parent state
BSIGNORE(PSButton1)
BSIGNORE(PSButton2)
BSIGNORE(PSButton3)
BSIGNORE(PSButton4)
BSIGNORE(PSButton5)
BSIGNORE(PSButton6)
BSIGNORE(PSButton7)
BSIGNORE(PSButton8)
BSIGNORE(PSMenuKey1)
BSIGNORE(PSMenuKey2)
BSIGNORE(PSMenuKey3)
BSIGNORE(PSMenuKey4)
BSIGNORE(PSMenuKey5)
BSIGNORE(PSMenuKey6)
BSIGNORE(PSMenuKey7)
BSIGNORE(PSMenuKey8)
BSIGNORE(PSMenuKey9)
BSIGNORE(PSMenuKey10)
BSIGNORE(PSMenuKey11)
BSIGNORE(PSMenuKey12)
BSIGNORE(PSMenuKey13)
BSIGNORE(PSMenuKey14)
BSIGNORE(PSMenuKey15)
#ifndef _CPPUNIT
BSIGNORE(PSMainMenuKey)  // AM mode main menu button - TAR# 123183
BSIGNORE(PSHelpTopics)
// NewUI
//BSIGNORE(PSListLookup)
// NewUI
BSIGNORE(PSVideoDone)
//@@@INFOPOINT
BSIGNORE(PSButtonGoBack)
//@@@INFOPOINT

// NewUI
SMStateBase *SMStateBase::PSListLookup(CString)
{
  return STATE_NULL;                // discard any input
}

// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
SMStateBase *SMStateBase::HandleNewContext()  
{
  return STATE_NULL;                // discard any input
}

//RFQ 986 - Handle PS_SWITCHCONTEXTOTHER
SMStateBase *SMStateBase::PSSwitchContext(BOOL bRemote, CString csNewContext)
{
  return STATE_NULL;
}


SMStateBase *SMStateBase::PSListLookupTab(LPCTSTR)
{
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSListScroll(const long)
{
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSAssistAccepted(void)		
{
  return STATE_NULL;                // ignore
}

SMStateBase *SMStateBase::PSAssistDenied(void)
{
  return STATE_NULL;                // ignore
}
#endif //_CPPUNIT

SMStateBase *SMStateBase::PSConnectRemote( CString& strMachineName, CString& strRemoteContext )
{
	if ( !secEventParms.m_bMultiPackItem ) //TAR 256154 - assist mode issue
	{
        // TAR 269869 start
		if ( !fStartTrxInAM && !ra.GetRequestEnableAssistMode()) //SSCOB-257

		{
		   trace (L2, _T("SMStateBase::PSConnectRemote - ignoring stale request for remote assist mode."));
		   ps.DisconnectFromServer( TRUE );
		   return STATE_NULL;
		}
	}
	else
	{
		secEventParms.m_bMultiPackItem = false; //TAR 256154 - assist mode issue
	}
    // TAR 269869 end

	// +TAR 372968
    //SASendSCOTInput( CANCEL_ITEM, io.csItemCode, 0, 0, 1 );
	// -TAR 372968

    //459315+
    if (getAnchorState() == BES_SCANANDBAG)
    {
       TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
    }//459315- //+SSCOADK-778
    else if (getAnchorState() == BES_PRODUCEFAVORITES)
    {
        if (fInMultiSelectPickList) 
        {  
            if ((ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid) 
            { 
                CKEYVALUEPARMLIST parmList; 
                CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
                _bstr_t bstrParmList((const _TCHAR *)parmList); 
                BSTR bstrResult; 
                bstrResult = NULL; 
                SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult); 
                SysFreeString(bstrResult); 
                nSAPrevSCOTInputID = 0; 
            }
            if(!fSAInItemSale)
            {
                fInMultiSelectPickList = false;
                setAnchorState(BES_SCANANDBAG);
                TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );
            }
        }
    }//-SSCOADK-778
	
	CREATE_AND_DISPENSE(WaitForRemoteAssistance)( strMachineName, strRemoteContext );
}
#ifndef _CPPUNIT

SMStateBase *SMStateBase::PSAlphaNumericKey(const long)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
// NewUI
SMStateBase *SMStateBase::PSAssistKey(const long)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
#endif //_CPPUNIT

/////////////////////////////////////
// RFQ 986
// PSCmdButton
// 
// Purpose: Handle "Command" button press.  This method is called
//          when a click occurs on a button or button list named "CmdBtn_X..."
//          where any sequence of characters can follow the "_"
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   csCommand - contents of the control "Data", defined either in a button's "Data"
//               property or a button list's "List" property
//        		<Control Name="CmdBtn_Button1" Type="Button">
//                  <Data>control string</Data>
//              </Control>
//              <Control Name="CmdBtn_List1" Type="ButtonList">
//                  <List>
//                     <Button>Label1,-,control string 1</Button>
//                     <Button>Label1,-,control string 2</Button>
//                  </List>
//              </Control>
///////////////////////////////////////
SMStateBase *SMStateBase::PSCmdButton(CString csContext, CString csControl, CString csCommand)
{
    trace(L6,_T("+SMStateBase::PSCmdButton(%s, %s, %s)"), csContext, csControl, csCommand);

    if ( csCommand.IsEmpty() )
    {
        DMSayPhrase(KEYPADINVALID);
    }
    else
    {
        trace(L6,_T("PSCmdButton sending \"%s\" to TB"), csCommand);
#ifndef _CPPUNIT
        SendTBCommand(TBCMD_CMDSTRING, csCommand);
#endif //_CPPUNIT
       
		if (getAnchorState() == BES_SCANANDBAG) //SSCOADK-3483
        {
          TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ScanAndBag") );
        }
        
        if(g_bTBNeedMoreData)
        {
            
            //clear DataNeededEntry after sending data to TB
            SMCmDataEntryBase::sm_pDataNeededEntry.Clear();//SR801
            
            g_bTBNeedMoreData = false; // TAR 238061 
            
            if (!g_bFromRAP)
                SAClearSecurityCondition();     //SSCOB-10238
            
            //SSCOADK-493
            if (storeMode())  //SMSmDataEntry
            {
                CREATE_AND_DISPENSE(RAPDataNeeded)(NULL, DATANEEDED_GOTOINPROGRESS, NULL, storeMode());
            }
            else
            {
                CREATE_AND_DISPENSE(InProgress) (storeMode(), helpMode(),0);
            }
        }
        else
        {
            CREATE_AND_DISPENSE(InProgress) (storeMode(), helpMode(),0);
        }
        
    }
    trace(L6,_T("-SMStateBase::PSCmdButton()"));



    return STATE_NULL;                // discard any input
}

/////////////////////////////////////
// PSCmdList
// 
// Purpose: Handle "Click" event on a CmdList_ receipt control  This method is called
//          when a click occurs on a receipt list named "CmdList_X..."
//          where any sequence of characters can follow the "_"
// Parameters
//   csContext - name of the context 
//   csControl - name of the control
//   csCommand - index of the list item clicked
///////////////////////////////////////
SMStateBase *SMStateBase::PSCmdList(CString csContext, CString csControl,  const long nIndex )
{
    trace(L6,_T("+SMStateBase::PSCmdList(%s, %s, %d)"), csContext, csControl, nIndex);

    //NextGenUI: NXTUI-90 - UI Story - GridInput Screen
    //This will set the selected item in the receipt list 
    //when an index from NGUI is received.           
    CString csIndex; 
    csIndex.Format(_T("%i"), nIndex);
    m_pAppPSX->SetSelectedReceiptItem(csControl, csIndex);

    trace(L6,_T("-SMStateBase::PSCmdList() Base set selected receipt item"));
    return STATE_NULL;                // discard any input
}


#ifndef _CPPUNIT
SMStateBase *SMStateBase::PSNumericKey(const long)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
SMStateBase *SMStateBase::PSDecimalKey(const long)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
SMStateBase *SMStateBase::PSEnterKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
 SMStateBase *SMStateBase::PSBackSpaceKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSShiftKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSAltGrKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSSymbolKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSCancelKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}
SMStateBase *SMStateBase::PSClearKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSSpaceKey(void)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSButtonToggle(void)
   {
   DMSayPhrase(KEYPADINVALID);
   return STATE_NULL;                // discard any input
   }

SMStateBase *SMStateBase::PSButtonLaneStatus(long lLaneIndex)
   {
   DMSayPhrase(KEYPADINVALID);
   return STATE_NULL;                // discard any input
   }


//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::PSEchoInput(const long lKey)
{
  long rVal = ps.EchoInput((_TCHAR)lKey);
  if (rVal == -2) 
	  DMSayPhrase(KEYPADINVALID);

  // TAR 168140 - DPS is playing the sound already.
  // else
  //  DMSayPhrase(KEYPADVALID); 
  return rVal;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::PSEchoDecimalInput(const long lKey)
{
  long rVal = ps.EchoDecimalInput((_TCHAR)lKey);
  if (rVal == -2) 
	  DMSayPhrase(KEYPADINVALID);
  else
	  DMSayPhrase(KEYPADVALID);
  return rVal;
}

#endif //_CPPUNIT
//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::PSClearInput(void)
{
  //  return ps.ClearInput();
  // version D
  return PSBackSpaceInput();
}
#ifndef _CPPUNIT
SMStateBase *SMStateBase::PSClearAllInput(BOOL bSayValid)
{
  ps.ClearInput();
  return STATE_NULL;							  // discard any input
}

SMStateBase *SMStateBase::PSAlphaKey(const long lKey)
{
  return STATE_NULL;							  //remain in same state
}

SMStateBase *SMStateBase::PSOtherSpecialKeys(const long)
{
  DMSayPhrase(KEYPADINVALID);
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSKeystroke(const long wParam, const long lParam)
{
	/******cm150000: Added in merge 2******/
	static bShiftOn = false;
	static bControlOn = false;
	static bAltOn = false;
	static bNumLockOn = false;

	trace(L6, _T("SMStateBase::PSKeyStroke(wParam:%d, lParam:%d)"), wParam, lParam);
	trace(L6, _T("shift:%d, control:%d, alt:%d, numlock:%d"), bShiftOn, bControlOn, bAltOn, bNumLockOn);
	SMStateBase *retState = STATE_NULL;
	long LocalwParam = wParam;
	/******cm150000: End Merge 2******/

	TCHAR ch;

	/******cm150000: Begin Merge 2******/
    if (lParam & 0x001) bShiftOn   = true;
    if (lParam & 0x02) bControlOn = true;
    if (lParam & 0x04) bAltOn     = true;
	if (!lParam && (wParam == VK_NUMLOCK)) bNumLockOn = true;

    if (!lParam && (bControlOn || bShiftOn || bAltOn))
    {
        if ((LocalwParam == VK_SHIFT) && bShiftOn)
            bShiftOn = false;
        if ((LocalwParam == VK_CONTROL) && bControlOn)
            bControlOn = false;
        if ((LocalwParam == VK_MENU) && bAltOn)
            bAltOn = false;
    }
    else
    {
        if (!lParam && (LocalwParam == VK_SHIFT))    bShiftOn   = true;
        if (!lParam && (LocalwParam == VK_CONTROL))  bControlOn = true;
        if (!lParam && (LocalwParam == VK_MENU))     bAltOn   = true;
    }
    if (!lParam && bShiftOn && (LocalwParam >= VK_F1) && (LocalwParam <= VK_F10))
    {
        LocalwParam += 0x0A; // add decimal 10 to make it start with VK_11 
        bShiftOn = false;
    }
    if (!lParam && bControlOn && (LocalwParam >= VK_F1) && (LocalwParam <= VK_F10))
    {
        LocalwParam += 0x14; // add decimal 20 to make it start with VK_21
        bControlOn = false;
    }
	if (!lParam && bNumLockOn && (wParam != VK_NUMLOCK))
	{
		switch(wParam)
		{
			case VK_END    :
				LocalwParam = _T('1');
				break;
			case VK_HOME   :
				LocalwParam = _T('7');
				break;
			case VK_LEFT   :
				LocalwParam = _T('4');
				break;
			case VK_UP     :
				LocalwParam = _T('8');
				break;
			case VK_RIGHT  :
				LocalwParam = _T('6');
				break;
			case VK_DOWN   :
				LocalwParam = _T('2');
				break;
			case VK_INSERT :
				LocalwParam = _T('0');
				break;
			case VK_DELETE :
				LocalwParam = _T('.');
				break;
		}
		bNumLockOn = false;
	}

	if ((LocalwParam >= VK_F1) && (LocalwParam <= VK_F24))
		nPSLastParm = LocalwParam;
	else
		nPSLastParm = VirtKeyToTChar(LocalwParam, lParam);
    trace(L6, _T("Processed data: wParam:%d, lParam:%d, shift:%d, control:%d, alt:%d, numlock:%d"),nPSLastParm, lParam, bShiftOn,bControlOn, bAltOn, bNumLockOn);

//+TAR324840
    int cchData 
      = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, NULL, NULL );
    _TCHAR *pccbBuffer = new _TCHAR[(UINT)cchData];
    ::GetLocaleInfo( LOCALE_USER_DEFAULT,
                     LOCALE_SMONDECIMALSEP,
                     pccbBuffer, 
                     cchData); 
    _TCHAR cLocalizedDecimal = pccbBuffer[0];
    delete [] pccbBuffer; //SSCOP-4165
//-TAR324840

	if (LocalwParam == 44)// ',' sent from DPS
   {
	  nPSLastEvt = PS_DECIMALKEY;
      //retState = PSDecimalKey(LocalwParam);
	  gSaveChOnKeyStroke =  LocalwParam;
   }
	else
	{
		ch = VirtKeyToTChar(LocalwParam, lParam);
		gSaveChOnKeyStroke = ch;
/******cm150000: End Merge 2******/
        if (IsCharAlphaNumeric(ch) && IsCharAlpha(ch))
        {
			nPSLastEvt = PS_ALPHANUMERICKEY;
			// retState = PSAlphaNumericKey(ch);
        }
		else if (IsCharAlphaNumeric(ch) && !IsCharAlpha(ch)) 
		{
			nPSLastEvt = PS_NUMERICKEY;
			//retState = PSNumericKey(ch);
		}
		else if (ch == 46 || ch == cLocalizedDecimal)//TAR324840
		{
			nPSLastEvt = PS_DECIMALKEY;
			//retState = PSDecimalKey(ch);
		}

		else if((ch >= 0x21 && ch<=0x2F) 
			|| (ch >= 0x3A && ch<=0x40) 
			|| (ch >= 0x5B && ch<=0x60)
			|| (ch >= 0x7B && ch<=0x7E)) //  printable chars other than 0-9, a-z, A-Z, space 
		{
			nPSLastEvt = PS_OTHERSPECIALKEY;
			//retState = PSOtherSpecialKeys(ch);
		}
		else if (LocalwParam==VK_SHIFT)               // is Enter key		- cm150000: Modified Merge 2
		{
			nPSLastEvt = PS_SHIFTKEY;
			//retState=PSShiftKey();
 
		}
		else if (LocalwParam==(VK_ALTGR))               
		{
			nPSLastEvt = PS_ALTGRKEY; 
		}
		else if (LocalwParam==VK_RETURN)               // is Enter key		- cm150000: Modified Merge 2
		{
			nPSLastEvt = PS_ENTERKEY;
			//retState = PSEnterKey();
		}
		else if (LocalwParam==VK_ESCAPE)               // is Esc key			- cm150000: Modified Merge 2
		{
			nPSLastEvt = PS_CANCELKEY;
			//retState = PSCancelKey();
		}
		else if ((LocalwParam == VK_F7) || (LocalwParam==VK_DELETE)) // emulator generates this code,  this is the cancel key on the BAPE keyboard
		{
			nPSLastEvt = PS_CANCELKEY;
			//retState = PSCancelKey(); // have a cancel key DcL-
		}
		else if (LocalwParam==VK_BACK)          // is BkSpc key
		{
			nPSLastEvt = PS_BACKSPACEKEY;
			//retState = PSBackSpaceKey();       // have a clear key
		}
		else if (LocalwParam==VK_CLEAR)         // is Clear key
		{
			retState = PSClearKey();       // have a clear key
		}
		else if (LocalwParam==VK_SPACE)         // is Space key
		{
			retState = PSSpaceKey();
		}
	}
	
   return retState;                     

}

SMStateBase *SMStateBase::PSHotKey(CString& csHotKeyValue)
{
  return STATE_NULL;                // discard any input
}

SMStateBase *SMStateBase::PSCharKey(TCHAR c)
{
  return STATE_NULL;                // discard any input
}
#endif// _CPPUNIT
//////////////////
long SMStateBase::PSBackSpaceInput(void)
{
#ifndef _CPPUNIT
  //  DMSayPhrase("KeyPadValid");
  //  return ps.BackSpaceInput();
  // version D
  CString csItemSent;
  long lLen = ps.GetInput(csItemSent);
  if (lLen)
  {
    DMSayPhrase(KEYPADVALID);
    return ps.BackSpaceInput();	// clear or correction key should clear only one character
  }
  else
  {
    DMSayPhrase(KEYPADINVALID);
  }
#endif// _CPPUNIT
  return 0;
}
#ifndef _CPPUNIT

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::PSReceiptUp(void)
{
//*** NewUI ***//
  ps.ScrollUp();							  // scroll up receipt
//*** end of NewUI ***//
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::PSReceiptDown(void)
{
//*** NewUI ***//
	ps.ScrollDown();						  // scroll down receipt //*** NewUI ***//
//*** end of NewUI ***//
  return STATE_NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::PSButtonHelp(void)
{
  // may place context checking by szStateName here
  RETURNSTATE(ContextHelp)
}

#endif // _CPPUNIT
// start CRD126194
char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int sizein, int* sizeout)
{
  LPSTR pszOut = NULL;
  *sizeout=0;
  if (lpwszStrIn != NULL)
  {
	int nInputStrLen = sizein;

	// Double NULL Termination
	int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
	pszOut = new char [nOutputStrLen];

	if (pszOut)
	{
	  memset (pszOut, 0x00, nOutputStrLen);
	  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
	}
    *sizeout=nOutputStrLen-2;
  }
  return pszOut;
}
// end CRD126194
//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::PSInitialize(void)
{
#ifndef _CPPUNIT
//#ifndef _CPPUNIT
  _TCHAR szFile2[_MAX_PATH+1];
  _TCHAR szFile3[_MAX_PATH+1];
  _TCHAR szFile4[_MAX_PATH+1];
  _TCHAR szFile5[_MAX_PATH+1];

  _TCHAR szFileName[_MAX_PATH+1];

  //Tar 143309
  if (ps.GetPickQtyList()->GetSize())
       ps.GetPickQtyList()->RemoveAll();

  //Move all config files from SCOT to SCOT\CONFIG
  CString csDestPath = co.csConfigFilePath + _T("\\");
  // copy SCUDBMPs names file
  _tcscpy(szFileName, SCUDBMPS _T(".DAT"));
  _tmakepath(szFile2,NULL, co.csConfigFilePath, SCUDBMPS, _T("DAT"));
  CopyFileFromServer(szFileName,csDestPath+szFileName,false);
	
  // copy multiple language files
  if (co.GetfStateDualLanguage())
  {
      CString csSCUDMsgs;
      // copy each languages standard language MSG file 
      for(int i=SCOT_LANGUAGE_PRIMARY; i<co.GetNumberOfLanguagesSupported(); i++)
      {
          //copy language specific tare description file
          _tcscpy(szFileName, (co.Getlanguage(i).csTareFile));
          CopyFileFromServer(szFileName,csDestPath+szFileName,false);

      }
      
      // ++ Commented out for NXTUI-73 SystemFunctions buttons
      //ps.LoadLanguageList();
      // -- NXTUI-73
  }

	  CString csServerName;
  csServerName = rp.GetServerName();

  bool bSkip=false, bPinged=false;

  csServerName.MakeUpper();
  if(csServerName==_T("LOCALHOST"))
      bSkip=true;
  else if(csServerName==_T("127.0.0.1"))
      bSkip=true;
  
  if(!bSkip)
  {
      QPInitialize();

      bPinged = QuickPing2(csServerName,
                          1,
                          20,
                          50,
                          false);
  }
  if( bSkip || bPinged)
  {
      // copy PickList.xml file from Report Server //
      _tcscpy( szFileName, rp.GetServerSharePoint() );
      _tcscat( szFileName, XML_PICKLISTFILE );
      if( !CopyFile( szFileName, csDestPath + XML_PICKLISTFILE, FALSE ) )
      {
         trace( L6, _T("Failed to copy Picklist file from ReportServer ( %s )!"), szFileName );
      }
      else
      {
         trace( L6, _T("Copied Picklist file from ReportServer ( %s )"), szFileName );
      }
  }
  else
  {
     trace( L6, _T("Failed to ping the server ( %s )!"), csServerName );
  }

  ////////////////////////////////////////////////
 
  if (co.fStateAllowLookUpItem)
  {
      ps.LoadProdList();

	  PSSortQtyList(*ps.GetPickQtyList());
  }

  //strcpy(szFileName, SCOTTARE ".DAT");
  _tcscpy(szFileName, co.GetcsDataTareFile() + _T(".DAT"));

  _tmakepath(szFile5,NULL, co.csConfigFilePath,co.GetcsDataTareFile(),_T("DAT"));
  CopyFileFromServer(szFileName,csDestPath+szFileName,false);
 //TAR262985+
  // see if scotopts.000 exists or not
  // free the current profile name buffer and set the new options file name
  CString csFile1;
  CString csFile2;
  CString csFileName1, csFileName2;
  TBRC nTBCallStatus;
  int nTerminalNumber = TBGetTerminalNumber();
  
  csFile1.Format(co.csConfigFilePath + _T("\\") + SCOTTARE + _T(".%03d"),0);
  csFile2.Format(co.csConfigFilePath + _T("\\") + SCOTTARE + _T(".%03d"),nTerminalNumber);

  csFileName1.Format(SCOTTARE _T(".000"));
  csFileName2.Format(SCOTTARE _T(".%03d"),nTerminalNumber);
  nTBCallStatus = CopyFileFromServer(csFileName1,csFile1,false);	
  nTBCallStatus = CopyFileFromServer(csFileName2,csFile2,false);
  
  if (OpenSCOTFile(csFile1, false, false))  //see if SCOTTARE.000 exists
  {
      _tcscpy(szFile5, csFile1);
  }

	
  if (OpenSCOTFile(csFile2, false, false))
  {
      _tcscpy(szFile5, csFile2);
  }

  // Initialize sound //
  CString strProductRegPath( SOUNDPATH );
  dm.SoundInitPath( strProductRegPath );
#ifndef _CPPUNIT //TODO:cs185088
  SCOTPriceSoundInit( strProductRegPath );
#endif //_CPPUNIT 
  //TAR262985-
  if (fDoOptions)
  {
      long returnLong = 0;
	  ps.UnInitializeConfigData();
	  ps.LoadTenderList();		// make sure to reload the tender dat if <Load Options> is pressed at System Functions TAR163827 LPM022101
      returnLong = ps.InitializeConfigData(szFile4, szFile5);
      ps.LoadLanguageList();
      PSSendConfigOptionsToNextGenUI();
      return returnLong;
  }

  //tar 286744 Abort application for ps.Initialize errors. 
  long lReturn = ps.Initialize(
                szFile2,
                szFile3,
                szFile4,
                szFile5,
                co.GetnOperationsTareContainer1(),
                co.GetnOperationsTareContainer2(),
                co.GetfOperationsScaleMetric(),
                co.GetfStateLiveVideo());
  if(lReturn < 0) 
  {
      return lReturn;
  }
  // NewUI
  m_pAppPSX = ps.GetPSXObject();
  // NewUI

  // ++ NXTUI-73 SystemFunctions buttons
  ps.LoadLanguageList();
  // -- NXTUI-73

  ReadOperatorLoginBarcodeMask(co.GetcsOperationsOperatorBarcodeMask());  //FZ20020730

  PSXRC rc = m_pAppPSX->GetConfigProperty(_T("CMButton1MidiList"), _T("CmDataEntry"), UI::PROPERTYFONT, defaultFont);
  if(PSX_SUCCESS != rc)
  {	// failure so use original font for CMButtonXMidiList
	trace(L4, _T("Failed to get default buttonFont for CMButton1MidiList on CmDataEntry, err=%x"),rc);
  }
//#endif // _CPPUNIT
  PSSendConfigOptionsToNextGenUI();
#endif
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void SMStateBase::PSSortQtyList(CStringArray & csPickQtyList)
{
#ifndef _CPPUNIT
  
   BOOL bNotDone = TRUE;
   CString temp;
   int posFirst ;
   int posNext ;

   while (bNotDone)
   {
      bNotDone = FALSE;
      for(int pos = 0;pos < csPickQtyList.GetUpperBound();pos++)
	  {
          posFirst = pos;
		  posNext = pos +1;
		  
         if (csPickQtyList.GetAt(posFirst).CompareNoCase(csPickQtyList.GetAt(posNext)) > 0)
		 {
         temp = csPickQtyList.GetAt(posFirst);
         csPickQtyList.SetAt(posFirst, csPickQtyList.GetAt(posNext));
         csPickQtyList.SetAt(posNext, temp);
         bNotDone= true;
		 } 
       }

   }

#endif //_CPPUNIT
  return ;
}
#ifndef _CPPUNIT


bool SMStateBase::VerifyQtyItem(CString csItemCode)
{
    int bottom, top, mid;
	top = ps.GetPickQtyList()->GetSize()-1;

    bottom=0;
	
	while (bottom <= top)
	{
     mid= (top - bottom)/2 + bottom;
	 if (csItemCode.CompareNoCase(ps.GetPickQtyList()->GetAt(mid))<0)
		 top = mid-1;
	 else if (csItemCode.CompareNoCase(ps.GetPickQtyList()->GetAt(mid))>0)
		 bottom = mid+1;
	 else 
		 return true; //found
	}
    return false;  //Not found
}



//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::PSUnInitialize(void)
{
  return ps.UnInitialize();
}

//////////////////////////////////////////////////////////
void SMStateBase::VoidAllCoupons(void)    //Void all coupons
{
//	ps.VoidAllCoupons();
	long lCouponCount = lCoupons;    // Get coupon number
	CString csCode, csDesc;
	long lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType;
    long lCouponIndex = 1;          // the index of valued coupon

	while(lCouponCount)
	{
        // Get each not voided coupon and void 
		if(ps.GetCoupon(lCouponIndex, csDesc, csCode, lQty, lWgt, lPrc, lUPrc,lTcd, lBarcodeType)) // ; I believe that I am correcting an error by commenting it out - YW 
		{
			csItemSent = csCode;					  // save the item code
			lVoidQty   = lQty;						  // save the qty
			lVoidWgt   = lWgt;						  // save the weight
			lVoidTcd   = lTcd;						  // save the tare for later void
			lVoidPrc	 = lPrc;					  // save the price for later void
			lVoidUPrc  = lUPrc;					  // save the unit price for later void
			lVoidBarcodeType = lBarcodeType;		  // save the item's barcode type for later void
			TBItemSale(true, io.lTareCode, 0);  //void this coupon
		}

		lCouponCount--;
	}
	return;

}

 /////////////////////////////////////////////////////
void SMStateBase::SetFocusToVideo()
{
#ifdef NewUI
	if (!(co.fStateLiveVideo && co.fStateWelcomeScreenVideo))
#else
	if (!(co.fStateLiveVideo && co.fStateWelcomeScreenVideo && CMVideoLarge.hWnd!=NULL))
#endif
	{
		// no need to bring video to top
		// trace(L6, "No Need to bring the live video to top");
		return;

	}
	bool minimize = false;
	CWnd *pWnd = CWnd::FindWindow(NULL, _T("LiveVideo"));
	if (pWnd && pWnd->GetSafeHwnd())
	{
		if(!pWnd->GetFocus())
		{
			pWnd->BringWindowToTop();
		}
   }
   return;
}

SMStateBase *SMStateBase::PSOnRemoteDisconnected(const long lParm)
{
    
    ms_bKeepRemoteDevices = false; // TAR 234566 CMal092203
	CString csLReceiptFile = LRECEIPT_FILE;
	CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;

	GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
	GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));

	csLReceiptFile.ReleaseBuffer();
	csLReceiptTmpFile.ReleaseBuffer();

	if (lParm == 0/*PS_DIRTY_DISCONNECT*/)
	{
		// Revert to local DPS...
		// If this is a "Dirty disconnect", first disconnect from the server
		// and then display an error message
		ps.DisconnectFromServer( FALSE );
		// cm150000: Merge 2
		// Don't need this until remote device merge
		// dm.EndRemote();	
		// + TAR 294889
        if ( co.IsInAssistMenus() || fStateAssist ) 
		{
			if (dm.fStateTakeawayBelt)
			{
				CKeyValueParmList parmList;
				BSTR bstrResult;
				CKeyValue<const _TCHAR*>::Put(parmList, KEY_SC_OPERATION, KEY_SC_OPERATION_ASSIST_MODE_STOP);
				_bstr_t bstrParmList((const _TCHAR *)parmList);
				bstrResult = NULL;
				SecMgr.SecurityControl(lSecMgrCookie, bstrParmList, &bstrResult);
				::SysFreeString(bstrResult); // cs185088 mem
			} 
		}// - TAR 294889

        bool bWasRemoteLegalForState = PSRemoteLegalForState();
        if (co.IsInAssistMenus() || fStateAssist)
        {
            co.SetInAssistMenus(false);
			if (dm.fStateTakeawayBelt && ps.RemoteMode())
				TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
			else
				TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
            SendTBCommand(TBCMD_ASSISTMENUSET, false);
        }

        fStateAssist = false;
        // TAR 223767:  Tell TB that we are exiting AssistMode  (gd123860).
        tb.SetMonitorAll( false );

        // TAR 223767:  End remote devices
		trace (L6, _T("SMStateBase::PSOnRemoteDisconnected Disconnecting remoted devices."));
		dm.EndRemote();
        // TAR 223767:  End

        switch (nTBLastRealState)//tar 245596
        {
            case TB_TENDERACCEPTED:
            case TB_NEWCASHDRAWERSTATE:
                //if partial
                if(lBalanceDue>0)
                {
                    return createAnchorState();
                }
                else
                {
                    RETURNSTATE(InProgress); //wait for TB_TRXEND 
                }
            case TB_TRXEND:
            case TB_COMPLETEPRINTANDCUT:
                TRACE(_T("SMStateBase::PSOnRemoteDisconnected  Need to print Receipt"));
		        if (FileExist(csLReceiptTmpFile))
                {
                    TRACE(_T("SMStateBase::PSOnRemoteDisconnected  Printing %s"), csLReceiptFile);
	                PrintLastReceipt(csLReceiptTmpFile);  //print receipt
                }
                RETURNSTATE(InProgress);
            case TB_ENDORSEDOCUMENT: //print chit
                {
                    long nEndorsementsPending=0;
                    do
	        		{
                        LPTSTR szSlipDocumentDescription;
                        LPTSTR szSlipDocumentFront;
                        LPTSTR szSlipDocumentBack;
                    
    
        				szSlipDocumentDescription = NULL;
        				szSlipDocumentFront = NULL;
    	    			szSlipDocumentBack = NULL;
				
			    	    tb.GetDocEndorsementInfo(&nEndorsementsPending,
                        &szSlipDocumentDescription,  &szSlipDocumentFront, &szSlipDocumentBack);

    				    if(nEndorsementsPending)
	    			    {
			    		    trace (L0, _T("SMStateBase::PSOnRemoteDisconnected Printing endorsement data to a chit"));
    	    				PrintEndorsementChit(szSlipDocumentBack);
	    		    		// Suppress further endorsement message from TB since we are doing these
		    		    	// in a loop and don't need to be event driven at this time.
			    		    tb.EndorsementCompleted(true);
        				}
                        delete [] szSlipDocumentDescription;
	               		delete [] szSlipDocumentFront;
    		        	delete [] szSlipDocumentBack;
                    } while (nEndorsementsPending);
                    RETURNSTATE(InProgress);
                }
            case TB_CLOSED:
				{
					if (fSAInTransaction)
					{
						if(!co.fTenderDispenseChangeAtLaneForRemoteTendering && m_bWaitForCashDrawerClose)  //RAP Dispense Change
						{
							//RAP is disconnected when SCOT is waiting for Cash Drawer to be closed. 
							//Reset m_bWaitForCashDrawerClose and continue transaction
							m_bWaitForCashDrawerClose = true;
							nTimeOut = 0;
							return TBEndOfTransaction();  //continue the transaction 
						}
						// +TAR 298941
						if (getAnchorState() != BES_TAKECARD)
						{
							RETURNSTATE(TakeCard);
						}
						else
						{
							return STATE_NULL;
						}
						// -TAR 298941
					}
				}
			case TB_NEEDMOREDATA:
				{
					if(g_bTBNeedMoreData)
					{
						//need to go back to somewhere to handle the unhandled intervention
						RETURNSTATE(RAPDataNeeded)
					}
					else
                       return bWasRemoteLegalForState ? createAnchorState() : STATE_NULL;
				}
            default: 
                return bWasRemoteLegalForState ? createAnchorState() : STATE_NULL;
	    }
	}
	else 
	{
		return STATE_NULL;
	}
}


// This function returns whether the current state supports being run
// remotely at the RAP terminal.  If not, and if currently in remote mode,
// the remote link will be automatically disabled.
bool SMStateBase::PSRemoteLegalForState()
{

	return false;
}

// This function returns whether the current state supports being run
// through the Assist Menus.  If not, an automatic exit of assistmenu
// mode will occur. 
bool SMStateBase::PSAssistMenuLegalForState()
{

	return false;
}

long SMStateBase::SetButton(LPCTSTR szControlName,       // show a button   TAR 218915
			  	         LPCTSTR     szContent,  // button text
				         const bool fEnable)    // T=Enabled, F=Disable
{
   return ps.Button(szControlName, szContent, fEnable);
}


//************************************************************************ 
//
// Function:		OnExternalUIRequest
// Purpose:			Process external UI request, called by TBParse or other equivalent message handlers
// Parameters:      pointer to UI request structure defined in SCOTUICMD.h, included in SCOTMsg.h
// Returns:	        actual return value is saved in the structure
// Calls:           calls PSProcedure::ExternalUIRequestExec(), 
// Side Effects:    requested action will be completed and the status is saved in the argument's fields
// Notes:           As of now, TB is the only component sends UI requests even though this function can handle from any module
//
//************************************************************************
SMStateBase *SMStateBase::OnExternalUIRequest(MessageElement* msgElem)
{
    SMStateBase* pRetState = STATE_NULL;
    if (!msgElem) return pRetState;
    PS_UI_CMD pUIRequestData = (PS_UI_CMD) (LPARAM) (msgElem->lParam);
    if (pUIRequestData)
    {
        // need to loop over the below function if there are multiple requests
		long rc;
		try
		{
			rc = ps.ExternalUIRequestExec(pUIRequestData);
		}
		catch(CException *e)
		{
			_TCHAR szWork[1024]={NULL};
			e->GetErrorMessage(szWork, sizeof(szWork));
			trace(L0, _T("Handled exception:%s in PSProcedureBase::ExternalUIRequestExec()"), szWork);
			e->Delete(); //TAR247967

		}
    }
    return pRetState;
}

//************************************************************************ 
//
// Function:		FindItemID
// Purpose:			Find Item ID of current selection, used to find void item's id to pass it to TB for its further internal processing
// Parameters:      
//					[out] csGetItemID   --->  received item-id if a matching item is found
//					[in]  csReceipt     --->  control name of the receipt for current screen(XML file defines the current screen)
//					[in]  csReceiptSrc  --->  source name of the receipt for current screen(XML file defines the current screen)
//					[in]  lExtPrice     --->  item's extended price, used as first criteria to find a matching item
//					[in]  lUnitPrice    --->  item's unit price, not used currently
//					[in]  csItemCode    --->  item's barcode, used as second criteria to find a matching item
// Returns:	        true if a matching item is found, otherwise returns false
// Calls:           calls GetReceiptItemVariable(), ProcessPIMFindItem(), GetPSXObject()
// Side Effects:    no side effects intended
// Notes:           Added to support Post Item Modification (PIM)
//
//************************************************************************
bool SMStateBase::FindItemID(CString& csGetItemID, CString csReceipt, CString csReceiptSrc, long lExtPrice, long lUnitPrice, CString csItemCode)
{
	USES_CONVERSION;

	// check if the current item matched the search criteria
	CString csItemEntryId; // current selected item's id
	PSXRC rc = PSX_E_FAIL;
	if (!ps.GetPSXObject())
	{
		return false;
	}
	csItemEntryId = ps.GetSelection(csReceipt);
	if (!csItemEntryId.IsEmpty())
	{
		// verify against the search criteria
		COleVariant vValue;
		vValue.Clear();
		CString csVarName;

		csVarName = _T("ITEM_EXT_PRICE");
		rc = ps.GetPSXObject()->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, csVarName, vValue);
        vValue.ChangeType(VT_INT);
		long lCurrentItemExtPrice = vValue.lVal;
		vValue.Clear();

		csVarName = _T("ITEM_CODE");
		rc = ps.GetPSXObject()->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, csVarName, vValue);
        vValue.ChangeType(VT_BSTR);
		CString	csCurrentItemCode = OLE2T(vValue.bstrVal);
		vValue.Clear();
		CString csItemCode1 = csCurrentItemCode;
		CString csItemCode2 = csItemCode;
		if (csItemCode1 != csItemCode2)
		{
            ////tar 241704
            TrimSymbology(csItemCode1);
            TrimSymbology(csItemCode2);
		}
		if ((csItemCode1 == csItemCode2) && lExtPrice == lCurrentItemExtPrice)
		{
			csGetItemID = csItemEntryId;
			return true;
		}
	}

    // Allocate space for an empty UI request and reset it
	S_UI_CMD sLocalUICmd;
    PS_UI_CMD psLocalUICmd = &sLocalUICmd;
    memset(psLocalUICmd, 0, sizeof(S_UI_CMD)); 
	psLocalUICmd->InitUnionField();

    // set the command type
    psLocalUICmd->eStructType = UI_STR_FINDITEM;

    // Set the receipt name
    psLocalUICmd->Find.SetRcptName(csReceiptSrc);

    PS_UI_VAR pFetchProperty = new S_UI_VAR;
    memset(pFetchProperty, 0, sizeof(S_UI_VAR)); // to be safe, InitStruct has to do the same task of initialization
	pFetchProperty->InitStruct();
    pFetchProperty->SetVarName(_T("ITEM_EXT_PRICE"));
    pFetchProperty->vVarValue = COleVariant(lExtPrice).Detach();
    psLocalUICmd->Find.pUiVars[0] =  pFetchProperty;    


    PS_UI_VAR pFetchProperty1 = new S_UI_VAR;
    memset(pFetchProperty1, 0, sizeof(S_UI_VAR));
	pFetchProperty1->InitStruct();
    pFetchProperty1->SetVarName(_T("ITEM_CODE"));
    pFetchProperty1->vVarValue = COleVariant(csItemCode).Detach();
    psLocalUICmd->Find.pUiVars[1] =  pFetchProperty1;    

    PS_UI_VAR pFetchProperty2 = new S_UI_VAR;
    memset(pFetchProperty2, 0, sizeof(S_UI_VAR));
    pFetchProperty2->InitStruct();
    pFetchProperty2->SetVarName(_T("ITEM_VOID"));
    pFetchProperty2->vVarValue = COleVariant(VARIANT_FALSE).Detach();
    psLocalUICmd->Find.pUiVars[2] =  pFetchProperty2;

    PS_UI_VAR pFetchProperty3 = new S_UI_VAR;
    memset(pFetchProperty3, 0, sizeof(S_UI_VAR));
    pFetchProperty2->InitStruct();
    pFetchProperty2->SetVarName(_T("ITEM_VOID"));
    pFetchProperty2->vVarValue = COleVariant().Detach();
    psLocalUICmd->Find.pUiVars[3] =  pFetchProperty3;

	ps.ProcessPIMFindItem(psLocalUICmd);
	bool bRetState = false;
	if (psLocalUICmd->Find.nArrSize)
	{
		csGetItemID = psLocalUICmd->Find.ItemIdArray[0];
		bRetState = true;
	}
	psLocalUICmd->CleanUp();

	return bRetState;
}

void SMStateBase::TrimSymbology(CString& csItemCode)
{
    CString csOrigCode(csItemCode);
    int nItemCodeLen = csItemCode.GetLength();
	if(nItemCodeLen)
	{
		//TAR449584+
		for (int nCtr = 0; nCtr < nItemCodeLen; nCtr++)
		{
			if (csItemCode[nCtr] == LINK_ITEM 
				|| csItemCode[nCtr] == VOID_ITEM 
				|| csItemCode[nCtr] == INFO_ITEM)
			{
				csItemCode.SetAt(nCtr, _T(' '));
			}
			else
			{
				csItemCode.TrimLeft();
				nItemCodeLen = csItemCode.GetLength();
				break;
			}
		}
		//TAR449584-

		if (csItemCode.Left(2) == _T("B3") /*/EAN128*/ || csItemCode.Left(2) == _T("B1") /*code 39*/
			|| csItemCode.Left(2) == _T("B2") /*Interleave 2 of 5*/ )  //tar 351927
        {
			csItemCode = csItemCode.Right(nItemCodeLen - 2); //remove 1st 2 characters
        }
        else if (csItemCode.Left(3) == _T("]e0")  )  //RFC 412256
        {
			csItemCode = csItemCode.Right(nItemCodeLen - 3); //remove 1st 2 characters
        }
        else if (csItemCode.Left(3) == _T("]C1")) //401672 code 128
		{
			 csItemCode = csItemCode.Right(nItemCodeLen - 3); //remove 1st 3 characters
		}
		else if (csItemCode.Left(2) == _T("C1")) //401672 code 128
		{
			 csItemCode = csItemCode.Right(nItemCodeLen - 2); //remove 1st 2 characters
		}
		else if (csItemCode.Find(_T("]e0")) != -1  )  //TAR 412889
        {
			int nLocation = csItemCode.Find(_T("]e0"));
			csItemCode = csItemCode.Right(nItemCodeLen - nLocation - 3); //remove all leading characters including ]e0
		}
        else
        {
            int nIndex = 0;
            while(nIndex < nItemCodeLen)
            {
                if (!_istdigit(csItemCode[nIndex]))
                {
                    csItemCode.SetAt(nIndex, _T(' '));
                    nIndex++;
                }
                else
                    break;
			}
			csItemCode.TrimLeft();
        }
        if(csOrigCode != csItemCode)
        {
            trace(L6, _T("SMStateBase::TrimSymbology trimmed from %s to %s"), csOrigCode, csItemCode);
        }
	}
}
SMStateBase* SMStateBase::FillPSEvent2NotifyTB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{
	// try to get event name
	sNotifyIn.nEventID = nPSLastEvt;
	sNotifyIn.szEventName = PSEventNameList[nPSLastEvt].szname;
	if (nPSLastEvt == PS_ENTERKEY)
	{
		// pass the current key input buffer as data
		CString csItemSent;
		long lLen = ps.GetInput(csItemSent);
		sNotifyIn.SetEventData(PSEventNameList[nPSLastEvt].szname, csItemSent, lLen);
	}
	return STATE_NULL; // not used 
}


SMStateBase* SMStateBase::PreNotifySCOTEvent2TB(const MessageElement* pEvent, sExtStateEventNotifyIn& sNotifyIn, sExtStateEventNotifyOut& sNotifyOut)
{  
	SMStateBase* pretState = STATE_NULL;

	CString csStateName= this->GetRuntimeClass()->m_lpszClassName;
	if (csStateName.Find(_T("SMExtensionState")) != -1)
	{
		// allow TB to preprocess and direct further flow
		//sNotifyIn.InitStruct();
		sNotifyIn.pSCOTEvent = pEvent;
		if (pEvent != NULL)
		{
			if (pEvent->id == ID_PS)
			{
				FillPSEvent2NotifyTB(pEvent, sNotifyIn, sNotifyOut);
			}
			else if (pEvent->id == ID_DM)
			{
				// DM event handlers are supposed fill sNotifyIn event data information
				FillDMEvent2NotifyTB(pEvent, sNotifyIn, sNotifyOut);
			}
		}
		TBSTATE nTBStateOnEvent = tb.NotifyExtensionStateEvent(&sNotifyIn, &sNotifyOut);
		if (nTBStateOnEvent != TB_IGNORE)
		{
			SMessageElement TBMsgLocal;
			TBMsgLocal.id = ID_TRANSBROKER;
			 // TB message can specify TB state directly instead of waiting for TBProcessMessage
			TBMsgLocal.nDeviceID = TB_DEV_DISPLAYDATANEEDED;
			_TCHAR szTBState[25];
			_ltot( nTBStateOnEvent, szTBState, 10 );    
			CopySCOTString(TBMsgLocal.pData, szTBState);
			TBMsgLocal.nLength = _tcslen(szTBState);
			pretState = TBParse(&TBMsgLocal);
			delete[] TBMsgLocal.pData;
			sNotifyOut.bAllowDefaultProcessing = false;
		}
		else if (!sNotifyOut.bAllowDefaultProcessing)
		{
			// Allow TB process it and wait in current state
			if (sNotifyOut.bGoToInProgress)
			{
				// disable all buttons and if possible any input devices
				CREATE_AND_ASSIGN(InProgress, pretState)(false, false, sNotifyOut.nInProgressTimeOut);
			}
			pretState = STATE_NULL;
		}
		else
		{
				// continue below logic
		}
	}
	else
	{
		sNotifyOut.bAllowDefaultProcessing = true;
	}
	return pretState;
}

void SMStateBase::SetAlphaShiftState(CString csContext, bool bShiftOn)
{
    COleVariant v;
    if (bShiftOn == true)
    {
        v = (long) UI::Pushed;
		m_pAppPSX->SetConfigProperty(_T("ShiftKey1"), csContext, UI::PROPERTYSTATE, v);
		m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP2"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP3"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP4"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);    
        m_pAppPSX->SetConfigProperty(_T("AlphaNumP2_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("AlphaNumP3_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("AlphaNumP4_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);	
    }
    else
    {
        v = (long) UI::Normal;
        m_pAppPSX->SetConfigProperty(_T("ShiftKey1"), csContext, UI::PROPERTYSTATE, v);
        m_pAppPSX->SetConfigProperty(_T("AlphaNumP2_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pAppPSX->SetConfigProperty(_T("AlphaNumP3_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("AlphaNumP4_LCase"), csContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP2"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP3"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty(_T("SmAlphaNumP4"), csContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
}


/**
 * Method:      BuildCmdButtonList
 * Description: Used by DataEntry states (SMSmDataEntry and SMCmDataEntry) to populate
 *              a command  button list.
 *              
 * Parameters:  cDataEntry - pointer to dataentry object
 *              csCtxName  - name of context containing the buttonlist control
 *              csCtlName  - name of the button list control
 * Returns:     
 * Notes:       The cDataEntry object will have CmdButtonList definitions if
 *              the DataEntry view string or ini definition contains one or more
 *              "CmdButtonListX=data,logo,text"  definitions.
 *             
 */
void SMStateBase::BuildCmdButtonList(CSCOTDataEntry& cDataEntry, CString csCtxName, CString csCtlName)
{
	USES_CONVERSION;
	long nButtonCount = cDataEntry.GetCmdButtonListCount();
	
	TRACE(_T("+BuildCmdButtonList\n"));
    // reset the count to 0 to clear all previously set attributes
    COleVariant v = (long) 0;
    m_pAppPSX->ClearReceipt(_T("DataNeededReceipt"));  //! Clear all prevoius setting before building new
    m_pAppPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pAppPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );
	
    v = nButtonCount;
	
    CPSXRedrawLock csRedrawLock( csCtlName, _T("SMStateBase::BuildCmdButtonList"));
	
    m_pAppPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
    m_pAppPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );
	
	CString str = _T("");
    COleSafeArray  saTextArray, saLogoArray, saDataArray;
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saLogoArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	saDataArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

    COleVariant vReturn, v1, v2;
	TCHAR szPath[_MAX_PATH];


	CString csImagePath;
	GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
	
    for(long  i = 0; i < nButtonCount; i++ )
	{
        v1 = i;
		CString csData, csLogo, csText, csButtonDef;
		
		int iFind;
		
		
		csButtonDef = cDataEntry.GetCmdButtonDef(i);
		
		// ButtonDef is expected to be of the format
		//    buttondata,buttonimagename,buttontext
		// Button data and button image names are not allowed to contain commas, and
		// anything after the second comma is considered button text.
		iFind = csButtonDef.Find(_T(","));
		if (iFind != -1)
		{
			csData = csButtonDef.Left(iFind);
			if (iFind < csButtonDef.GetLength() - 1)  
			{
				//There's something after 1st comma, discard first part of string
				//and parse the rest of it.
				csButtonDef = csButtonDef.Mid(iFind + 1);
				iFind = csButtonDef.Find(_T(","));
				if (iFind != -1)
				{
					if (iFind > 0)
					{  // a button image is defined
						csLogo = csButtonDef.Left(iFind);
						_tmakepath(szPath,NULL,csImagePath,csLogo, NULL);
						
						csLogo = szPath;
					}
					if (iFind < csButtonDef.GetLength() - 1)
					{ // Text defined after the second comma
						csText = csButtonDef.Mid(iFind + 1);
					}
				}
			}
			else
			{
				TRACE(_T("BuildCmdButtonList button %d - bad button format %s\n"), i, csButtonDef);
			}
		}
		else
		{
			TRACE(_T("BuildCmdButtonList - bad button format %s\n"),csButtonDef);
		}
		
		// +TAR 314786
        COleVariant strDataVar(csData);
		VARIANT vData=strDataVar.Detach();
		BSTR bstrLogo = csLogo.AllocSysString();
		BSTR bstrText = csText.AllocSysString();
		saDataArray.PutElement(&i, &vData );
		saLogoArray.PutElement(&i, bstrLogo );
		saTextArray.PutElement(&i, bstrText );
        if (csData.GetLength() == 0)
        {
            COleVariant v1 = i;
            COleVariant v2 = (long) UI::Disabled; // disable
			m_pAppPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
        }
       
		::SysFreeString(bstrLogo);
		::SysFreeString(bstrText);  
		
    }
	csImagePath.ReleaseBuffer();

    v = saTextArray;
    m_pAppPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saLogoArray;
    m_pAppPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
    v = saDataArray;
    m_pAppPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
	
}

/**
 * Method:      BuildCmdList
 * Description: Used by DataEntry states (SMSmDataEntry and SMCmDataEntry) to populate
 *              a command list.
 *              
 * Parameters:  cDataEntry - pointer to dataentry object
 *              csReceiptSrc  - name of PSX receipt data source
 * Returns:     
 * Notes:       The cDataEntry object will have CmdListEntry definitions if
 *              the DataEntry view string or ini definition contains one or more
 *              "CmdListEntryX=value,Text"  definitions.
 *             
 */
void SMStateBase::BuildCmdList(CSCOTDataEntry& cDataEntry, const CString& csReceiptSrc)
{
	COleVariant vValue;
	CString csEntryId;
	CString csDef, csData, csText;

	int nEntryID;

	long nEntryCount = cDataEntry.GetCmdListEntryCount();
    m_pAppPSX->ClearReceipt(_T("DataNeededReceipt"));
	for(nEntryID = 0; nEntryID < nEntryCount; nEntryID++ )
	{
		csDef.Empty();
		csData.Empty();
		csText.Empty();
		csEntryId.Format(_T("%d"), nEntryID);
		m_pAppPSX->CreateReceiptItem(csReceiptSrc, csEntryId);
		csDef=cDataEntry.GetCmdListEntry(nEntryID);
		int iFind = csDef.Find(_T(","));
		if (iFind != -1)
		{
			csData = csDef.Left(iFind);
			csText = csDef.Mid(iFind + 1);
		}
		else
		{
			TRACE(_T("BuildCmdList - no entry list data defined %s\n"),csDef);
			csData = csEntryId;
			csText = csDef;
		}
		vValue = csData;
        m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csEntryId, _T("ITEM_CMDDATA"), vValue);
		vValue = ps.ConvertToEscapeSequence(csText, _T("\\n"), _T("\n"));
		m_pAppPSX->SetReceiptItemVariable(csReceiptSrc, csEntryId, _T("ITEM_SMTEXT"), vValue);
	}
	m_pAppPSX->UpdateReceiptControls(csReceiptSrc);

}

#endif //_CPPUNIT

void SMStateBase::PSSendConfigOptionsToNextGenUI(void)
{
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData(_T("NextGenUIShowVolumeControl"), co.IsShowVolumeControl());
    laneStateData.SendNextGenUIData(_T("NextGenUIVolumeControlMuteBypass"), co.fOperationsVolumeControlMuteBypass);
    laneStateData.SendNextGenUIData(_T("NextGenUIDualLanguage"), co.GetfStateDualLanguage());    
    laneStateData.SendNextGenUIData(_T("NextGenUICMRewardInfo"), co.fCMRewardInfo);
	laneStateData.SendNextGenUIData(_T("NextGenUIAllowLanguageSelectionDuringTransaction"),co.fStateAllowLanguageSelectionDuringTransaction);

	if(!_tcsicmp(co.csHWType, _T("SS90")))
	{
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowLookUpItem"), false);
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowKeyInCode"), false);
		laneStateData.SendNextGenUIData(_T("NextGenUIOwnBagButtonAtAttract"), false);
		laneStateData.SendNextGenUIData(_T("NextGenUIOwnBagButtonAtScanAndBag"), false);
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowPickListSearchButton"), false);
		laneStateData.SendNextGenUIData(_T("TerminalType"), COleVariant(co.csHWType));
	}
	else
	{
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowLookUpItem"), co.fStateAllowLookUpItem);
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowKeyInCode"), co.fStateAllowKeyInCode);
		laneStateData.SendNextGenUIData(_T("NextGenUIOwnBagButtonAtAttract"), co.fOperationsCustomerBagAllowed);
		laneStateData.SendNextGenUIData(_T("NextGenUIOwnBagButtonAtScanAndBag"), co.fOperationsCustomerBagAllowedInScanAndBag);
		laneStateData.SendNextGenUIData(_T("NextGenUIAllowPickListSearchButton"), co.GetfOperationsAllowPickListSearchButton());
	}
	
    laneStateData.SendNextGenUIData(_T("NextGenUICMTotalSavings"),co.fCMTotalSavings);
    laneStateData.SendNextGenUIData(_T("NextGenUICMTotalRewardPoints"),co.fCMTotalRewardPoints);

    laneStateData.SendNextGenUIData(_T("NextGenUIIsTaxLine"),co.fLocaleIsTaxLine);

	laneStateData.SendNextGenUIData(_T("NextGenUIAllowCancelAll"), co.m_bOperationsAllowCancelAll);
}

//+SSCOADK-5341
bool SMStateBase::ShouldDiscardEvent(const PSInfo &psInfo) const
{
    bool bResult = false;

    if(! ps.RemoteMode())
    {
            // To be safe, only do this for button clicks.
            if(psInfo.nEvt >= PS_BUTTON1 && psInfo.nEvt <= PS_BUTTON8)
            {
               const CString csOriginatingContext = psInfo.pszContext;
               if(! csOriginatingContext.IsEmpty())  // Event is not a context wildcard.
               {             
                   CString csCurrentContext;
                   m_pAppPSX->GetContext(csCurrentContext);

                   // If the click originated in a context different from the current context. 
                   if(csOriginatingContext != csCurrentContext)
                   {       
                       bResult = true;
                   }    
               }
           }    
    }

    return bResult;
}
//-SSCOADK-5341
