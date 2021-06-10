//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSThread.CPP
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//#include <afxtempl.h>               // MS MFC template header
//#include <afxmt.h>                  // MS MFC template header
//#include <afxctl.h>
#include <afxpriv.h>

//#include "SMState.h"
#include "Common.h"                 // Common includes
#include "PSThread.h"               // This header

// local defines //
#define COMP_ID     ID_PS
#define T_ID        _T("PSt")

///////////////////////////////////////////////////////////////////////

//************************************************************************ 
//
// Function:		PSXEventHandler
// Purpose:			This function is called by the UI subsystem when an event occurs
//                  (i.e. button click)
// Parameters:      [in] pPSX - pointer to an instance of PSX object
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        void
// Calls:           PostPs
// Side Effects:
// Notes:           
//
//************************************************************************
void PSXEventHandler( CPSX *pPSX, const CString& strRemoteConnectionName, const CString& strControlName, const CString& strContextName, const CString& strEventName, const COleVariant& vParam )
{
	USES_CONVERSION;
	PSEVT nEvent = PS_UKEVT;
    long lParm = 0;
    long wParm = 0;
	CString csTemp;
    trace( L6, _T("PSXEventHandler Event( %s,%s,%s,%s )"), strRemoteConnectionName, strControlName, strContextName, strEventName );

	// TAR 285290 start
	if( UI::EVENTQUIT == strEventName )
	{
		// TAR 285552 start
		trace( L2, _T("ALT-F4 stop of application initiated") );
		mo.PostGP(GP_STOPSCOT);
		mo.PostGP(GP_STOP);
		// TAR 285552 end
		AfxGetApp()->PostThreadMessage( WM_QUIT, 0, 0 );
		return;
	}
	// TAR 285290 end
	
	if (vParam.vt == VT_BSTR)
	{
		csTemp = vParam.bstrVal;
		wParm = _ttoi(csTemp);
	}
	else if (vParam.vt == VT_I4)
	{
		wParm = vParam.iVal;
		lParm = vParam.iVal;
	}
	
    if( UI::EVENTCLICK == strEventName )
	{
		if (strControlName == _T("SpaceKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 32;			//Space Key
			lParm = 0;
		}
	
		if (strControlName == _T("ShiftKey1"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = VK_SHIFT;		// Shift key	
			lParm = 0;
		}	
		if (strControlName == _T("Backspace"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = VK_BACK;		// Bkspc key	
			lParm = 0;
		}	
		if (strControlName == _T("PeriodKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbe;		// Period Key (not decimal key)	
			lParm = 0;
		}	
		if (strControlName == _T("GreaterThanKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbe;	
			lParm = 1;
		}	
		if (strControlName == _T("BackslashKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbf;		
			lParm = 0;
		}	
		if (strControlName == _T("QuestionMarkKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbf;		
			lParm = 1;
		}	
	    if (strControlName == _T("LeftBracket"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdb;		
			lParm = 0;
		}	
	    if (strControlName == _T("LeftBrace"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdb;	
			lParm = 1;
		}	
	    if (strControlName == _T("RightBracket"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdd;		
			lParm = 0;
		}	
	    if (strControlName == _T("RightBrace"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdd;	
			lParm = 1;
		}	
	    if (strControlName == _T("SlashKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdc;	
			lParm = 0;
		}	
	    if (strControlName == _T("PipeKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xdc;	
			lParm = 1;
		}	
	    if (strControlName == _T("EqualSignKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbb;		
			lParm = 0;
		}	
	    if (strControlName == _T("PlusSignKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbb;	
			lParm = 1;
		}	
	    if (strControlName == _T("ApostropheKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xc0;		
			lParm = 0;
		}	
	    if (strControlName == _T("TildeKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xc0;			
			lParm = 1;
		}	

		if (strControlName == _T("AtSignKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 50;		
			lParm = 1;
		}	
		if (strControlName == _T("ExclamationPointKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 49;			
			lParm = 1;
		}	
		if (strControlName == _T("SharpKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 51;		
			lParm = 1;
		}	
		if (strControlName == _T("DollarSignKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 52;		
			lParm = 1;
		}	
		if (strControlName == _T("PercentSignKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 53;			
			lParm = 1;
		}	
		if (strControlName == _T("AmpersandKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 55;			
			lParm = 1;
		}	
		if (strControlName == _T("AsteriskKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 56;		
			lParm = 1;
		}	
		if (strControlName == _T("OpenParenthesisKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 57;			
			lParm = 1;
		}	
		if (strControlName == _T("CloseParenthesisKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 48;		
			lParm = 1;
		}	
		if (strControlName == _T("MinusKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbd;		
			lParm = 0;
		}	
		if (strControlName == _T("UnderscoreKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbd;		
			lParm = 1;
		}

		if (strControlName == _T("ColonKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xba;	
			lParm = 1;
		}	
		if (strControlName == _T("SemicolonKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xba;	
			lParm = 0;
		}	

		if (strControlName == _T("CommaKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbc;	
			lParm = 0;
		}	
		if (strControlName == _T("LessThanKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xbc;		
			lParm = 1;
		}	

		if (strControlName == _T("SingleQuoteKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xde;	
			lParm = 0;
		}	

		if (strControlName == _T("DoubleQuoteKey"))
		{
			nEvent = (PSEVT) PS_KEYSTROKE;
			wParm = 0xde;	
			lParm = 1;
		}	
		if(strControlName == _T("ButtonGoBack") || strControlName == _T("ButtonStoreLogIn"))
		{
			nEvent = nEvent = (PSEVT) PS_GOBACK;
		}
		if (strControlName == _T("ButtonHelp"))
		{
			nEvent = (PSEVT) PS_HELP;
		}

		if ((strControlName == _T("CMButton1")) || (strControlName == _T("SMButton1")) ||
			(strControlName == _T("CMButton1Lg")) || (strControlName == _T("CMButton1Med")) ||
			(strControlName == _T("CMButton1MidiList")) || (strControlName == _T("CMButton1StoreLogIn")) ||
			(strControlName == _T("CMButton1Global")) || (strControlName == _T("CMButton1LookUp")))
		{
			nEvent = (PSEVT)(PS_BUTTON1);
		}
		if ((strControlName == _T("CMButton2")) || (strControlName == _T("SMButton2")) ||
			(strControlName == _T("CMButton2Lg")) || (strControlName == _T("CMButton2Med")) ||
			(strControlName == _T("CMButton2MidiList")) || (strControlName == _T("CMButton2StoreLogIn")) ||
			(strControlName == _T("CMButton2KeyInCode")))
		{
			nEvent = (PSEVT)(PS_BUTTON2);
		}
		if ((strControlName == _T("CMButton3")) || (strControlName == _T("SMButton3")) ||
			(strControlName == _T("CMButton3MidiList")) || (strControlName == _T("CMButton3Global")) ||
			(strControlName == _T("CMButton3Med")))
		{
			nEvent = (PSEVT)(PS_BUTTON3);
		}
		if ((strControlName == _T("CMButton4")) || (strControlName == _T("SMButton4")) ||
			(strControlName == _T("CMButton4MidiList")) || (strControlName == _T("CMButton4Lg")) ||
			(strControlName == _T("CMButton4LgFinish")))
		{
			nEvent = (PSEVT)(PS_BUTTON4);
		}
		if ((strControlName == _T("CMButton5")) || (strControlName == _T("SMButton5")) ||
			(strControlName == _T("CMButton5MidiList")) || (strControlName == _T("CMButton5Lg")) ||
			(strControlName == _T("CMButton5LgFinish")))
		{
			nEvent = (PSEVT)(PS_BUTTON5);
		}
		if ((strControlName == _T("CMButton6")) || (strControlName == _T("SMButton6")) ||
			(strControlName == _T("CMButton6MidiList")) || (strControlName == _T("CMButton6Lg")) ||
			(strControlName == _T("CMButton6LgFinish")))
		{
			nEvent = (PSEVT)(PS_BUTTON6);
		}
		if ((strControlName == _T("CMButton7")) || (strControlName == _T("SMButton7")) ||
			(strControlName == _T("CMButton7MidiList")) || (strControlName == _T("CMButton7Lg")) ||
			(strControlName == _T("CMButton7LgFinish")))
		{
			nEvent = (PSEVT)(PS_BUTTON7);
		}
		if ((strControlName == _T("CMButton8")) || (strControlName == _T("SMButton8")) ||
			(strControlName == _T("CMButton8MidiList")) || (strControlName == _T("CMButton8Lg")) ||
			(strControlName == _T("CMButton8Global")) || (strControlName == _T("CMButton8LgFinish")))
		{
			nEvent = (PSEVT)(PS_BUTTON8);
		}
		if ((strControlName == _T("CMReceiptScrollUp")) || (strControlName == _T("SMReceiptScrollUp")))
		{
			nEvent = (PSEVT)(PS_RECEIPTUP);
		}
		if ((strControlName == _T("CMReceiptScrollDown")) || (strControlName == _T("SMReceiptScrollDown")))
		{
			nEvent = (PSEVT)(PS_RECEIPTDOWN);
		}
		if ( ((strContextName == co.GetContextPrefix() + _T("ProduceFavorites")) &&  (strControlName == _T("ProductImage"))) ||
             ((strContextName == _T("Finish")) &&  (strControlName == _T("TenderImage"))) ||
			 ((strContextName == _T("SelectLanguage")) &&  (strControlName == _T("LanguageImage"))) ||
			 ((strContextName == _T("SelectTare")) &&  (strControlName == _T("TareList"))) ||
			 ((strContextName == _T("SelectPayment")) &&  (strControlName == _T("CashBackList"))) ||
			 ((strContextName == _T("SelectPaymentEBT")) &&  (strControlName == _T("EBTCashBackList"))) ) //tar 226722
		{
	         nEvent = PS_LISTLOOKUP;
			 //TAR 225285 check if sending back a string buffer, assign a string pointer to wParm and mark lParam to be 0
			 // to differentiate the else if case.
			 if (vParam.vt == VT_BSTR)
			 {
 				 CString *pTemp = new CString;
				 *pTemp = OLE2T(vParam.bstrVal);
				 wParm = (WPARAM) pTemp;
				 lParm = 0;
			 }
			 else if (vParam.vt == VT_I4)
				 lParm = 1;

		}
		if ( strControlName == _T("CMProduceSelectNext") || strControlName == _T("CMTareDown"))
		{
			nEvent = PS_LISTSCROLL;
            wParm = UI::PageDown;			// Specify scroll next
		}
		if ( strControlName == _T("CMProduceSelectPrev") || strControlName == _T("CMTareUp"))
		{
			nEvent = PS_LISTSCROLL;
            wParm = UI::PageUp;			// Specify Scoll prev
		}
		if ( (strControlName == _T("NumericP1")) || (strControlName == _T("KeyBoardP1")) ||
             (strControlName == _T("AlphaNumP1")) || (strControlName == _T("AlphaNumP2")) ||
             (strControlName == _T("AlphaNumP4")) )
		{
			nEvent = PS_KEYSTROKE;
			lParm = 0;
		}
		if ( (strControlName == _T("NumericP2")) || (strControlName == _T("KeyBoardP2")) )
		{
			nEvent = PS_KEYSTROKE;
			wParm = 96;		// number 0
			lParm = 0;
		}
		if ( (strControlName == _T("NumericP3")) || (strControlName == _T("SMNumericKeyBoard")))
		{
			nEvent = PS_KEYSTROKE;
			wParm = 12;			// Clear key
			lParm = 0;
		}
		if ( (strControlName == _T("NumericP4")) || (strControlName == _T("KeyBoardP4")) ||
			 (strControlName == _T("AlphaNumKey3")))
		{
			nEvent = PS_KEYSTROKE;
			wParm = 13;			// Enter key	
			lParm = 0;
		}
		if ( (strControlName == _T("KeyBoardP3")) )
		{
			nEvent = PS_KEYSTROKE;
			wParm = 110;		// Decimal point	
			lParm = 0;
		}
        // Handel all assist keys 1 - 200. Assist key button list control names must 
        // start with "Key".
        if ( (strControlName.Left(3) == _T("Key")) &&  (!_istalpha(strControlName.GetAt(3)))
             && (strControlName.GetLength() >= 4) )
		{
            nEvent = (PSEVT)(PS_ASSISTKEY);
			lParm = wParm;
			wParm = 0;
		}
		if ( strControlName == _T("RemoteButton1") )
		{
			nEvent = PS_REMOTEBUTTON1;
		}
		if ( (strContextName == co.GetContextPrefix() + _T("ProduceFavorites")) &&  (strControlName == _T("TabSelected")) ||
        {
			nEvent = PS_LISTLOOKUPTAB;
			CString *pTemp = new CString;
			*pTemp = OLE2T(vParam.bstrVal);
			lParm = (LPARAM) pTemp;
		}
		if (strControlName == _T("ButtonToggle"))
		{
			nEvent = (PSEVT) PS_BUTTONTOGGLE;
		}
        if (strControlName == _T("ButtonMainMenu")) //tar 241410
        {
            nEvent = (PSEVT) PS_MAINMENUKEY;
        }
        if (strControlName == _T("Menu1"))
        {
            nEvent = (PSEVT) PS_MENUKEY1;
        }
        if (strControlName == _T("Menu2"))
        {
            nEvent = (PSEVT) PS_MENUKEY2;
        }
        if (strControlName == _T("Menu3"))
        {
            nEvent = (PSEVT) PS_MENUKEY3;
        }
        if (strControlName == _T("Menu4"))
        {
            nEvent = (PSEVT) PS_MENUKEY4;
        }
        if (strControlName == _T("Menu5"))
        {
            nEvent = (PSEVT) PS_MENUKEY5;
        }
        if (strControlName == _T("Menu6"))
        {
            nEvent = (PSEVT) PS_MENUKEY6;
        }
        if (strControlName == _T("Menu7"))
        {
            nEvent = (PSEVT) PS_MENUKEY7;
        }
        if (strControlName == _T("Menu8"))
        {
            nEvent = (PSEVT) PS_MENUKEY8;
        }
        if (strControlName == _T("Menu9"))
        {
            nEvent = (PSEVT) PS_MENUKEY9;
        }
        if (strControlName == _T("Menu10"))
        {
            nEvent = (PSEVT) PS_MENUKEY10;
        }
        if (strControlName == _T("Menu11"))
        {
            nEvent = (PSEVT) PS_MENUKEY11;
        }
        if (strControlName == _T("Menu12"))
        {
            nEvent = (PSEVT) PS_MENUKEY12;
        }
        if (strControlName == _T("Menu13"))
        {
            nEvent = (PSEVT) PS_MENUKEY13;
        }
        if (strControlName == _T("Menu14"))
        {
            nEvent = (PSEVT) PS_MENUKEY14;
        }
        if (strControlName == _T("Menu15"))
        {
            nEvent = (PSEVT) PS_MENUKEY15;
        }
	}
	else if( strEventName == _T("KeyDown") )
	{
		nEvent = PS_KEYSTROKE;
		wParm = LOWORD( vParam.lVal );
        lParm = HIWORD( vParam.lVal );
    }
	else if( strEventName == UI::EVENTCONNECTREMOTE )
	{
		if( strRemoteConnectionName == _T("RAP") )
		{
			int nColonPosition = csTemp.Find(':');
			CString csContextRemote = csTemp.Left(nColonPosition);
            CString csTemp2 = &(((LPCTSTR)csTemp)[nColonPosition + 1]);
            nColonPosition = csTemp2.Find(':');
            CString machineName = csTemp2.Left(nColonPosition);
			CString loginID = &(((LPCTSTR)csTemp2)[nColonPosition + 1]);
            rp.SetOperator(loginID);
			if( csContextRemote == _T("cRemote") )
			{
				csContextRemote = _T("SmAssistMode");
			}
			else if( csContextRemote == _T("cStoreMode") )
			{
				csContextRemote = _T("SmAuthorization");
			}

			// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
 			else if( csContextRemote == _T("cSingleAM4") ) 
			{
			    csContextRemote = _T("SmSingleAM4");
			}
		    else if( csContextRemote == _T("cSingleAM6") )
			{
  			    csContextRemote = _T("SmSingleAM6");
			}
      	    else if( csContextRemote == _T("cSingleAM8") )
			{
			    csContextRemote = _T("SmSingleAM8");
			}
			
			nEvent = PS_CONNECTREMOTE;
			CString* pstr1 = new CString;	
			*pstr1 = machineName;
			CString* pstr2 = new CString;	
			*pstr2 = csContextRemote;
			lParm = (LPARAM)pstr1;
			wParm = (WPARAM)pstr2;
		}
		else if( strRemoteConnectionName == _T("AUTOMATION") )
		{
			pPSX->AcceptConnection( strRemoteConnectionName, TRUE );
		}
		else
		{
			pPSX->AcceptConnection( strRemoteConnectionName, FALSE );
        }
	}
	else if( strEventName == _T("DisconnectRemote") )
	{
		if( strRemoteConnectionName == _T("RAP") )
		{
            nEvent = PS_REMOTE_DISCONNECTED;
            lParm = (LPARAM)vParam.boolVal;
        }
	}

	// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
	else if( strEventName == _T("SwitchAssistMode") ) 
	{
        nEvent = PS_SWITCHCONTEXT;
		CString* pstr1 = new CString;	
		CString* pstr2 = new CString;	
		//*pstr1 = OLE2T(vParam.bstrVal);
		*pstr1 = EMPTY_STRING;
		*pstr2 = strContextName;
		lParm = (LPARAM)pstr1;
		wParm = (WPARAM)pstr2;
    }

    if (!nEvent)                    // unknown event
    {
      nEvent = PS_UKEVT;
    }
		
    trace( L6, _T("PostPS %d,%d,%d,%s"), nEvent, lParm, wParm, strContextName );
    mo.PostPS( nEvent, lParm, wParm, strContextName );
}



///////////////////////////////////////////////////////////////////////
// Call back function for PSCreateDC, PS will call back to here
///////////////////////////////////////////////////////////////////////
BOOL CALLBACK CallBack_Handler(const long Msg,
                               const WPARAM wParam,
                               const LPARAM lParam)
{
  PSEVT nEvent = PS_UKEVT;
  int nWhat = (int)wParam;
  long lParm = 0;
  long wParm = 0;
  try
  {
    trace(L9,_T("Callback_Handler %d,%d/0x%x,%d/0x%x"),Msg,wParam,wParam,lParam,lParam);

    switch (Msg)
    {

    case DPSN_BUTTON_RELEASED:
       switch(nWhat)
          {
          case IDC_BUTTONGOBACK:
             nEvent = PS_GOBACK;
             break;
          default:
            if (nWhat>=IDC_BUTTON1 && nWhat<=IDC_BUTTON8)
               nEvent = (PSEVT)(PS_BUTTON1 + nWhat - IDC_BUTTON1);
	      }
		break;
	
	// +Tar227804
	//	The SCOT AuxDisplay sometimes spoofs numeric data
    // thru this code.  It expects PSThread to send the complete
    // long int to the App thru PSNumericKey();
    // This is used mainly for CashBack.

	case DPSN_AUXDISP_SPOOF_NUMERIC:
	   nEvent = PS_NUMERICKEY;     
	   lParm = (long) lParam;      
	   break;
    //-Tar227804

	}
    if (!nEvent)                    // unknown event
    {
      nEvent = PS_UKEVT;
      lParm = Msg;
    }
		
    trace( L6,_T("PostPS %d,%d,%d,%s"), nEvent, lParm, wParm, _T("") );
    mo.PostPS( nEvent, lParm, wParm, _T("") );

    return 0;

  }
  catch(CException *e)
  {
    _TCHAR szText[100];
    e->GetErrorMessage(szText,100);
    ScotError(RETURN,SM_CAT_SYSTEM_CALL, SCOTAPP_PSTHREAD_EXCEPTION,_T("%s"),szText);

    e->Delete();
  }
  catch(...)
  {
    // handle all exceptions
    ScotError(RETURN,SM_CAT_SYSTEM_CALL, SCOTAPP_PSTHREAD_EXCEPTION2);

  }
  return 0;
}
