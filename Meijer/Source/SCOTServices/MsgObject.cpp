//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  MsgObject.CPP
//
// TITLE:   Class implementation file (describe class here)
//
// $Header: /Fastlane/4.2/Development/Core/Source/SCOTServices/MsgObject.cpp 6     12/09/08 5:01p De200005 $
//
// AUTHOR:  DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// PD -093097 Changed WaitMessage to use TimeOut and MessageElement pointer
// PD -093097 Format, Tabify, convert from CCmdTarget to CObject
// DcL-102897 Made changes to critical section of code: make section static within
//    file, and moved unlock method to after wait for message Also changed
//    ASSERTS into VERIFYS based of release vs debug build research it did
//      during my vacation time.
// DcL-111097 Added changes to accomadate DM message, changed timeout to milliseconds instead of
//    seconds. Added default timeout value of 10 seconds
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxmt.h>                  // Multithreading header
#include "MsgObject.h"
#include "ErrorObject.h"
#include <ncrdevmgr.h> // SSCOP-95

#define COMP_ID ID_MO
#define T_ID _T("MO")
#define Trace if (pTraceObj) pTraceObj->Trace

static CCriticalSection CritSec;
extern void Reveal(const MessageElement *me, CString& csFormattedMessage);
//DLLEXPORT long g_dwKeyPresstime;  //TAR198849

//////////////////////////////////////////////////////////////////////////////////////////////////
// MsgObject
long MsgObject::PostIt(const MessageElement *me)
{
  if (!hw) return -1;

  Trace(L7, _T("MsgObject::PostIt:BeforeLock()"));    

  CSingleLock sl( &CritSec, false );
  sl.Lock();                        //enter critical section
  Trace(L7, _T("MsgObject::PostIt:AfterLock()"));    

  CString csMessageTextLine;
  ::Reveal(me, csMessageTextLine);
  
  BOOL retStatus = PostMessage(hw,ScotMessageID,0,(LPARAM)me);
  if (!retStatus)
  {
	  // failed to post a message, this should never happen in 32 bit Windows
	  // Application message queue grows dynamically as needed
	  DWORD nLastError = GetLastError();
	  Trace(L3, _T("MsgObject::PostIt:!!!! FAILED On PostMessage!!!!,LastError=%d"), nLastError);    
  }

  Trace((csMessageTextLine.Find(_T("ERROR:")) == 0) ? L0 : L7, _T("MsgObject::PostIt:%s"), csMessageTextLine);

  sl.Unlock();                      //exit critical section
  return 0;
}
    
///////////////////////////////////////////////////////////////////////////////////////////////////
MsgObject::MsgObject(HWND hWnd, TraceObject* pTO):hw(hWnd)	// constructor
{
    //ScotMessageID = RegisterWindowMessage(SCOTMSGID);
	ScotMessageID = SCOT_MESSAGE;
	pTraceObj = pTO;
}
     
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for Presentation Services
    
    long MsgObject::PostPS( const int  nPS, const long lPrm, const long wPrm, const CString& strContext, const long lHandlingFlags ) // SSCOI-45254
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
    	
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_PS;                //identifier, message comes from PS
      mePtr->psInfo.nEvt = nPS;
      mePtr->psInfo.lParm = lPrm;
	  mePtr->psInfo.wParm = wPrm;
      mePtr->psInfo.pszContext = new TCHAR [strContext.GetLength() + sizeof(TCHAR)];
	  _tcscpy( mePtr->psInfo.pszContext, strContext );
      mePtr->lParam = lHandlingFlags;
	  	
      return PostIt(mePtr);
    }
    /*
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting functions for Terminal Services
    
    long MsgObject::PostTB(const int  nTB,
                           const int  Len,
                           const void *Data)
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
    	
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_TB;                //identifier, message comes from TS
      mePtr->tsInfo.nEvt = nTB;
    	
      mePtr->tsInfo.Length = Len;
      mePtr->tsInfo.Data = new _TCHAR[Len+1];
      memset(mePtr->tsInfo.Data,0,Len+1);
      memcpy(mePtr->tsInfo.Data,Data,Len);
    	
      return PostIt(mePtr);
    }
    */
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for Device Manager
    
long MsgObject::PostDM(
   const int  Msg,
   const int  Class,
   const int  DeviceId,
   const long lCodeOrLength,
   const void * vDPtr,
   const long lDirectIONumber,
   const long lDirectIONumData)
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
    	
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_DM;                //identifier, message comes from PS
      mePtr->dmInfo.Msg = Msg;
      mePtr->dmInfo.Class = Class;
      mePtr->dmInfo.DeviceId = DeviceId;
      mePtr->dmInfo.lCodeOrLength = lCodeOrLength;
      if (vDPtr && lCodeOrLength>0)  //SR 93.4 Removed the 1000 byte limit
      {
        mePtr->dmInfo.vDPtr = new _TCHAR[lCodeOrLength+1];
        memset(mePtr->dmInfo.vDPtr, 0, (lCodeOrLength+1) * sizeof(_TCHAR));
        if(Msg == DM_DATA && Class == DMCLASS_SCALE &&
            lCodeOrLength == 4) // SSCOP-95 //SSCOB-2522
        {
          memcpy(mePtr->dmInfo.vDPtr, vDPtr, lCodeOrLength);
        }
        else
        {
          memcpy(mePtr->dmInfo.vDPtr, vDPtr, lCodeOrLength * sizeof(_TCHAR));
        }
      }
      else
      {
        mePtr->dmInfo.vDPtr = NULL;
      }
      mePtr->dmInfo.lDirectIONumber = lDirectIONumber;
      mePtr->dmInfo.lDirectIONumData = lDirectIONumData;
    	
      return PostIt(mePtr);
    }
     
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for general purpose
    
    long MsgObject::PostGP(const GPEVT nGP)
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
    	
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_GP;                //identifier, message comes from PS
      mePtr->gpInfo.nEvt = nGP;
      return PostIt(mePtr);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Added for RFC 265463
    // Posting function for general purpose, including passing of parameter values
    
    long MsgObject::PostGPwithParams(const GPEVT nGP,
                                     const WPARAM wParam,
                                     const LPARAM lParam)
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
    	
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_GP;                //identifier, message comes from PS
      mePtr->gpInfo.nEvt = nGP;
      mePtr->psInfo.wParm = wParam;
      mePtr->psInfo.lParm = lParam;
      return PostIt(mePtr);
    }
    // End of addition for RFC 265463

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for Security Manager   
	long MsgObject::PostSecMgr(	const int nEventType,	 // event type
								const CString& csParms) // string containing a list of comma-delimited keyword value pairs
	{
      MessageElement * mePtr = new MessageElement; //create and new message element
	  //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_SECMGR;               //identifier, message comes from Security Manager
	  mePtr->secmgrInfo.nEvt = nEventType;
      mePtr->secmgrInfo.szParms = new TCHAR [csParms.GetLength() + sizeof(TCHAR)];
	  _tcscpy( mePtr->secmgrInfo.szParms, csParms );
	  	
      return PostIt(mePtr);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for Reporting 
    long MsgObject::PostRP(const int Msg,
                           const WPARAM wParam,
                           const LPARAM lParam)
                          
    {
      MessageElement * mePtr = new MessageElement; //create and new message element
      //fill in the message structure with the information pass in from the parameters of this function
      mePtr->id = ID_RP;                //identifier, message comes from PS
      mePtr->rpInfo.Msg = Msg;
	  mePtr->rpInfo.wParam = wParam;
      mePtr->rpInfo.lParam = lParam;
      return PostIt(mePtr);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Posting function for Reporting 
    long MsgObject::PostCU(const int Msg,
        const CString& csParms) // string containing a list of comma-delimited keyword value pairs
        
        
    {
        MessageElement * mePtr = new MessageElement; //create and new message element
        //fill in the message structure with the information pass in from the parameters of this function
        mePtr->id = ID_CU;                //identifier, message comes from PS
        mePtr->rpInfo.Msg = Msg;
        if (csParms.GetLength())
        {
            mePtr->cuInfo.szParms = new TCHAR [csParms.GetLength() + sizeof(TCHAR)];
            _tcscpy( mePtr->cuInfo.szParms, csParms );
        }
        else
        {
            mePtr->cuInfo.szParms = NULL;
        }
        return PostIt(mePtr);
    }

#define ELEMENT_COUNT(a) (sizeof(a) / sizeof(a[0]))

/////////////////////////////////////////////////////
void Reveal(const MessageElement *me, CString& csFormattedMessage)
{
	CString csText;
	CString Msg = _T("NotSet");     // Tar 192458 HC 012102 Added Tracing
	LPTSTR DM_Devices[] = {
		_T("RECEIPT"),
		_T("JOURNAL"),
		_T("SLIP"),
		_T("PRINTER"),
		_T("CASHDRAWER"),
		_T("CASHCHANGER"),
		_T("KEYLOCK"),
		_T("LINEDISPLAY"),
		_T("MICR"),
		_T("MSR"),
		_T("SCALE"),
		_T("SCANNER"),
		_T("SIGCAP"),
		_T("MISC"),
		_T("ENCRYPTOR"),
		_T("CASHACCEPTOR"),
		_T("COINACCEPTOR"),
      _T("AUXDISPLAY"),
      _T("MOTIONSENSOR"),
      _T("EASDEACTIVATOR"),
      _T("CASHRECYCLER"),
		_T("MAX_CLASSES")};

	LPTSTR TB_Devices[] = {
		_T("UNKNOWN"),
		_T("CMOS"),
		_T("DISPLAYINTERMEDIATE"),
		_T("DISPLAYPRIMARY"),
		_T("DISPLAYSECONDARY"),
		_T("DISPLAYIMMEDIATE"),
		_T("DRAWER1"),
		_T("DRAWER2"),
		_T("EPSMODULE"),
		_T("KEYBOARD"),
		_T("KEYLOCK,			"),
		_T("MSR"),
		_T("PRINTERJOURNAL"),
		_T("PRINTERRECEIPT"),
		_T("PRINTERSLIP"),
		_T("SCALE"),
		_T("SERIAL"),
		_T("SCANNER"),
		_T("UNKNOWN"),
        _T("DISPLAYDATANEEDED")};


        LPTSTR PS_Events[] = {
        _T("UKEVT"),
        _T("HELP"),
        _T("BUTTON1"),
        _T("BUTTON2"),
        _T("BUTTON3"),
        _T("BUTTON4"),
        _T("BUTTON5"),
        _T("BUTTON6"),
        _T("BUTTON7"),
        _T("BUTTON8"),
        _T("MENUKEY1"),
        _T("MENUKEY2"),
        _T("MENUKEY3"),
        _T("MENUKEY4"),
        _T("MENUKEY5"),
        _T("MENUKEY6"),
        _T("MENUKEY7"),
        _T("MENUKEY8"),
        _T("MENUKEY9"),
        _T("MENUKEY10"),
        _T("MENUKEY11"),
        _T("MENUKEY12"),
        _T("MENUKEY13"),
        _T("MENUKEY14"),
        _T("MENUKEY15"),
        _T("RECEIPTUP"),
        _T("RECEIPTDOWN"),
        _T("LISTLOOKUP"),
        _T("HELPTOPICS"),
        _T("ENTERKEY"),
        _T("CANCELKEY"),
        _T("CLEARKEY"),
        _T("NUMERICKEY"),
        _T("DECIMALKEY"),
        _T("ALPHAKEY"),
        _T("VIDEODONE"),
        _T("UKRELEASE"),
        _T("UKSELCHANGE"),
        _T("ASSISTKEY"),
        _T("ACTIVITY"),
        _T("MAINMENU"),
        _T("SPACEKEY"),
        _T("GOBACK"),
        _T("REMOTE_DISCONNECTED"),
        _T("KEYSTROKE"),
        _T("BUTTONTOGGLE"),
        _T("BUTTONLANE1STATUS"),
        _T("BUTTONLANE2STATUS"),
        _T("BUTTONLANE3STATUS"),
        _T("BUTTONLANE4STATUS"),
        _T("BUTTONLANE5STATUS"),
        _T("BUTTONLANE6STATUS"),
        _T("BUTTONLANE7STATUS"),
        _T("BUTTONLANE8STATUS"),
        _T("REMOTEBUTTON1"),
        _T("REMOTEBUTTON2"),
        _T("REMOTEBUTTON3"),
        _T("REMOTEBUTTON4"),
        _T("LISTLOOKUPTAB"),
        _T("LISTSCROLL"),
        _T("CONNECTREMOTE"),
        _T("SWITCHCONTEXT")};


      //TRACE("%s(%d) :%d\n", __FILE__, __LINE__, me->id);
	  switch (me->id)
	  {
	  case ID_PS:
		if (ELEMENT_COUNT(PS_Events) > me->psInfo.nEvt)
			Msg.Format(_T(" ID_PS : Len=%d nEvt=%d(%s) lParm=%ld\n"),
			  me->nLength, me->psInfo.nEvt, PS_Events[me->psInfo.nEvt], me->psInfo.lParm);
		else
			Msg.Format(_T("DEBUG INFORMATION: ID_PS Event %d out of array bounds!!!"), me->psInfo.nEvt);    
		break;
	  case ID_TRANSBROKER:
		  csText = (LPTSTR)me->pData;
		if (ELEMENT_COUNT(TB_Devices) > me->nDeviceID)
			Msg.Format(_T(" ID_TRANSBROKER : Dev=%d(%s) Len=%d text=[%s]\n"),
			  me->nDeviceID, TB_Devices[me->nDeviceID], me->nLength, csText);
		else
			Msg.Format(_T("ID_TRANSBROKER non-standard device %d received \n"), me->nDeviceID);  //TB likely to send alternate device IDs  
		break;
	  case ID_DM:
		if (ELEMENT_COUNT(DM_Devices) > me->dmInfo.Class)
			Msg.Format(_T(" ID_DM : Len=%d Msg=%d Class=%d(%s)-%d lCodeOrLength=%ld\n"),
			  me->nLength, me->dmInfo.Msg, me->dmInfo.Class, 
           DM_Devices[me->dmInfo.Class],
           me->dmInfo.DeviceId,
			  me->dmInfo.lCodeOrLength);
		else
			Msg.Format(_T("DEBUG INFORMATION: ID_DM device class %d out of array bounds!!!"), me->dmInfo.Class);    
		break;


	  case ID_GP:					   // general purpose notify
		Msg.Format(_T(" ID_GP : Dev=%d Len=%d nEvt=%d\n"),
		  me->nDeviceID, me->nLength, me->gpInfo.nEvt);
		break;
	  }

      // Tar 192458 HC 012102 Added Tracing
      if (Msg != _T("NotSet"))
      {
	    TRACE(_T("%s\n"),Msg);
	    csFormattedMessage = _T(">>POSTED message:") + Msg;
      }
      else
      {
//TK commented out        TRACE(_T("%s - %d - %d"), Msg, me->id, me->saInfo.Msg);
        CString csTemp1;
        CString csTemp2;
        csTemp1.Format(_T("%d"), me->id);
//TK commented out        csTemp2.Format(_T("%d"), me->saInfo.Msg);
        csFormattedMessage = _T(">>POSTED message:") + Msg + _T(" - ") + csTemp1 + _T(" - ") + csTemp2;
      }
      // Tar 192458 HC 012102 Added Tracing End
}
