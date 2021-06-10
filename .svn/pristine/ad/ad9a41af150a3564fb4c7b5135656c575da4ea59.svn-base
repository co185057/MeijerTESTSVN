// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface.cpp 3     10/21/08 4:06p Sm133076 $
/*
 * @(#)PSXInterface.h    $Revision: 3 $ $Date: 10/21/08 4:06p $
 *
 * Copyright 2004 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/exe - TestRCM/PSXInterface.cpp $
 * 
 * 3     10/21/08 4:06p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:14p Dm185016
 * Moved to new Repository.
// 
// 5     12/03/04 5:42p Dm185016
// signon protocol change
// 
// 4     11/16/04 11:19a Dm185016
// Added ChangeContext
// 
// 3     11/02/04 3:00p Dm185016
// New functions
// 
// 2     10/29/04 8:48a Dm185016
// Single source with SCOTAPP.
//
// 3     10/14/04 2:00p Dm185016
// Fixed problems recognizing the buttons
//
// 2     10/08/04 1:39p Dm185016
//
// 1     10/04/04 5:34p Dm185016
// New for 3.2 Patch E/4.0
*/

// Debug info is truncated to 255 warning
#pragma warning( disable : 4786 )
#pragma warning( push )

#include <stdafx.h>                 // MS MFC always needed first
#include <afxpriv.h>

#if defined(_SCOT) || defined(_SCOTSSF_)
#include "Common.h"                 // Common includes
#endif

#ifndef EMPTY_STRING
#define		EMPTY_STRING  _T("")
#endif

#include "PSXMfc.h"
#include "psevt.h"
#include "PSXInterface.h"           // This header
#include "UIDefines.h"
#include "RAPUIDefines.h"

#include "StringComparator.h"
#include <map>


// local defines //
#define COMP_ID     ID_PS
#define T_ID        _T("PSt")


const _TCHAR REMOTE_CONNECTION_PREFIX[]			= _T("RAP");
const int REMOTE_CONNECTION_PREFIX_LENGTH		= sizeof(REMOTE_CONNECTION_PREFIX)-1;


//BEGIN_MAPPING(CPSXInterface::mappingEntries)
CPSXInterface::SMappingEntry CPSXInterface::m_mappingEntries[] = {
    MAP_ENTRY_N_W_L(_T("SpaceKey"),             PS_KEYSTROKE, 32, 0)
    MAP_ENTRY_N_W_L(_T("ShiftKey1"),            PS_KEYSTROKE, VK_SHIFT, 0)
    MAP_ENTRY_N_W_L(_T("Backspace"),            PS_KEYSTROKE, VK_BACK, 0)
    MAP_ENTRY_N_W_L(_T("PeriodKey"),            PS_KEYSTROKE, 0xbe, 0)
    MAP_ENTRY_N_W_L(_T("GreaterThanKey"),       PS_KEYSTROKE, 0xbe, 1)
    MAP_ENTRY_N_W_L(_T("BackslashKey"),         PS_KEYSTROKE, 0xbf, 0)
    MAP_ENTRY_N_W_L(_T("QuestionMarkKey"),      PS_KEYSTROKE, 0xbf, 1)
    MAP_ENTRY_N_W_L(_T("LeftBracket"),          PS_KEYSTROKE, 0xdb, 0)
    MAP_ENTRY_N_W_L(_T("LeftBrace"),            PS_KEYSTROKE, 0xdb, 1)
    MAP_ENTRY_N_W_L(_T("RightBracket"),         PS_KEYSTROKE, 0xdd, 0)
    MAP_ENTRY_N_W_L(_T("RightBrace"),           PS_KEYSTROKE, 0xdd, 1)
    MAP_ENTRY_N_W_L(_T("SlashKey"),             PS_KEYSTROKE, 0xdc, 0)
    MAP_ENTRY_N_W_L(_T("PipeKey"),              PS_KEYSTROKE, 0xdc, 1)
    MAP_ENTRY_N_W_L(_T("EqualSignKey"),         PS_KEYSTROKE, 0xbb, 0)
    MAP_ENTRY_N_W_L(_T("PlusSignKey"),          PS_KEYSTROKE, 0xbb, 1)
    MAP_ENTRY_N_W_L(_T("ApostropheKey"),        PS_KEYSTROKE, 0xc0, 0)
    MAP_ENTRY_N_W_L(_T("TildeKey"),             PS_KEYSTROKE, 0xc0, 1)
    MAP_ENTRY_N_W_L(_T("AtSignKey"),            PS_KEYSTROKE, 50, 1)
    MAP_ENTRY_N_W_L(_T("ExclamationPointKey"),  PS_KEYSTROKE, 49, 1)
    MAP_ENTRY_N_W_L(_T("SharpKey"),             PS_KEYSTROKE, 51, 1)
    MAP_ENTRY_N_W_L(_T("DollarSignKey"),        PS_KEYSTROKE, 52, 1)
    MAP_ENTRY_N_W_L(_T("PercentSignKey"),       PS_KEYSTROKE, 53, 1)
    MAP_ENTRY_N_W_L(_T("AmpersandKey"),         PS_KEYSTROKE, 55, 1)
    MAP_ENTRY_N_W_L(_T("AsteriskKey"),          PS_KEYSTROKE, 56, 1)
    MAP_ENTRY_N_W_L(_T("OpenParenthesisKey"),   PS_KEYSTROKE, 57, 1)
    MAP_ENTRY_N_W_L(_T("CloseParenthesisKey"),  PS_KEYSTROKE, 48, 1)
    MAP_ENTRY_N_W_L(_T("MinusKey"),             PS_KEYSTROKE, 0xbd, 0)
    MAP_ENTRY_N_W_L(_T("UnderscoreKey"),        PS_KEYSTROKE, 0xbd, 1)
    MAP_ENTRY_N_W_L(_T("ColonKey"),             PS_KEYSTROKE, 0xba, 1)
    MAP_ENTRY_N_W_L(_T("SemicolonKey"),         PS_KEYSTROKE, 0xba, 0)
    MAP_ENTRY_N_W_L(_T("CommaKey"),             PS_KEYSTROKE, 0xbc, 0)
    MAP_ENTRY_N_W_L(_T("LessThanKey"),          PS_KEYSTROKE, 0xbc, 1)
    MAP_ENTRY_N_W_L(_T("SingleQuoteKey"),       PS_KEYSTROKE, 0xde, 0)
    MAP_ENTRY_N_W_L(_T("DoubleQuoteKey"),       PS_KEYSTROKE, 0xde, 1)
    MAP_ENTRY_N_0_0(_T("ButtonGoBack"),         PS_GOBACK)
    MAP_ENTRY_N_0_0(_T("ButtonStoreLogIn"),     PS_GOBACK)
    MAP_ENTRY_N_0_0(_T("ButtonHelp"),           PS_HELP)
    MAP_ENTRY_N_0_0(_T("CMButton1"),            PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("SMButton1"),            PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1Lg"),          PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1Med"),         PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1MidiList"),    PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1StoreLogIn"),  PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1Global"),      PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton1LookUp"),      PS_BUTTON1)
    MAP_ENTRY_N_0_0(_T("CMButton2"),            PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("SMButton2"),            PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton2Lg"),          PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton2Med"),         PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton2MidiList"),    PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton2StoreLogIn"),  PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton2KeyInCode"),   PS_BUTTON2)
    MAP_ENTRY_N_0_0(_T("CMButton3"),            PS_BUTTON3)
    MAP_ENTRY_N_0_0(_T("SMButton3"),            PS_BUTTON3)
    MAP_ENTRY_N_0_0(_T("CMButton3MidiList"),    PS_BUTTON3)
    MAP_ENTRY_N_0_0(_T("CMButton3Global"),      PS_BUTTON3)
    MAP_ENTRY_N_0_0(_T("CMButton3Med"),         PS_BUTTON3)
    MAP_ENTRY_N_0_0(_T("CMButton4"),            PS_BUTTON4)
    MAP_ENTRY_N_0_0(_T("SMButton4"),            PS_BUTTON4)
    MAP_ENTRY_N_0_0(_T("CMButton4MidiList"),    PS_BUTTON4)
    MAP_ENTRY_N_0_0(_T("CMButton4Lg"),          PS_BUTTON4)
    MAP_ENTRY_N_0_0(_T("CMButton4LgFinish"),    PS_BUTTON4)
    MAP_ENTRY_N_0_0(_T("CMButton5"),            PS_BUTTON5)
    MAP_ENTRY_N_0_0(_T("SMButton5"),            PS_BUTTON5)
    MAP_ENTRY_N_0_0(_T("CMButton5MidiList"),    PS_BUTTON5)
    MAP_ENTRY_N_0_0(_T("CMButton5Lg"),          PS_BUTTON5)
    MAP_ENTRY_N_0_0(_T("CMButton5LgFinish"),    PS_BUTTON5)
    MAP_ENTRY_N_0_0(_T("CMButton6"),            PS_BUTTON6)
    MAP_ENTRY_N_0_0(_T("SMButton6"),            PS_BUTTON6)
    MAP_ENTRY_N_0_0(_T("CMButton6MidiList"),    PS_BUTTON6)
    MAP_ENTRY_N_0_0(_T("CMButton6Lg"),          PS_BUTTON6)
    MAP_ENTRY_N_0_0(_T("CMButton6LgFinish"),    PS_BUTTON6)
    MAP_ENTRY_N_0_0(_T("CMButton7"),            PS_BUTTON7)
    MAP_ENTRY_N_0_0(_T("SMButton7"),            PS_BUTTON7)
    MAP_ENTRY_N_0_0(_T("CMButton7MidiList"),    PS_BUTTON7)
    MAP_ENTRY_N_0_0(_T("CMButton7Lg"),          PS_BUTTON7)
    MAP_ENTRY_N_0_0(_T("CMButton7LgFinish"),    PS_BUTTON7)
    MAP_ENTRY_N_0_0(_T("CMButton8"),            PS_BUTTON8)
    MAP_ENTRY_N_0_0(_T("SMButton8"),            PS_BUTTON8)
    MAP_ENTRY_N_0_0(_T("CMButton8MidiList"),    PS_BUTTON8)
    MAP_ENTRY_N_0_0(_T("CMButton8Lg"),          PS_BUTTON8)
    MAP_ENTRY_N_0_0(_T("CMReceiptScrollUp"),    PS_RECEIPTUP)
    MAP_ENTRY_N_0_0(_T("SMReceiptScrollUp"),    PS_RECEIPTUP)
    MAP_ENTRY_N_0_0(_T("CMReceiptScrollDown"),  PS_RECEIPTDOWN)
    MAP_ENTRY_N_0_0(_T("SMReceiptScrollDown"),  PS_RECEIPTDOWN)
    MAP_ENTRY_N_W_0(_T("CMProduceSelectNext"),  PS_LISTSCROLL, UI::PageDown)
    MAP_ENTRY_N_W_0(_T("CMProduceSelectPrev"),  PS_LISTSCROLL, UI::PageUp)
    MAP_ENTRY_N_W_0(_T("CMTareDown"),           PS_LISTSCROLL, UI::PageDown)
    MAP_ENTRY_N_W_0(_T("CMTareUp"),             PS_LISTSCROLL, UI::PageUp)
    MAP_ENTRY_N_0_L(_T("NumericP1"),            PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("KeyBoardP1"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("AlphaNumP1"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("AlphaNumP2"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("AlphaNumP4"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_W_L(_T("NumericP2"),            PS_KEYSTROKE, 96, 0)
    MAP_ENTRY_N_W_L(_T("KeyBoardP2"),           PS_KEYSTROKE, 96, 0)
    MAP_ENTRY_N_W_L(_T("NumericP3"),            PS_KEYSTROKE, 12, 0)
    MAP_ENTRY_N_W_L(_T("SMNumericKeyBoard"),    PS_KEYSTROKE, 12, 0)
    MAP_ENTRY_N_W_L(_T("NumericP4"),            PS_KEYSTROKE, 13, 0)
    MAP_ENTRY_N_W_L(_T("KeyBoardP4"),           PS_KEYSTROKE, 13, 0)
    MAP_ENTRY_N_W_L(_T("AlphaNumKey3"),         PS_KEYSTROKE, 13, 0)
    MAP_ENTRY_N_W_L(_T("KeyBoardP3"),           PS_KEYSTROKE, 110, 0)
    MAP_ENTRY_N_0_0(_T("RemoteButton1"),        PS_REMOTEBUTTON1)
    MAP_ENTRY_N_0_0(_T("ButtonToggle"),         PS_BUTTONTOGGLE)
    MAP_ENTRY_N_0_0(_T("ButtonMainMenu"),       PS_MAINMENUKEY)
    MAP_ENTRY_N_0_0(_T("Menu1"),                PS_MENUKEY1)
    MAP_ENTRY_N_0_0(_T("Menu2"),                PS_MENUKEY2)
    MAP_ENTRY_N_0_0(_T("Menu3"),                PS_MENUKEY3)
    MAP_ENTRY_N_0_0(_T("Menu4"),                PS_MENUKEY4)
    MAP_ENTRY_N_0_0(_T("Menu5"),                PS_MENUKEY5)
    MAP_ENTRY_N_0_0(_T("Menu6"),                PS_MENUKEY6)
    MAP_ENTRY_N_0_0(_T("Menu7"),                PS_MENUKEY7)
    MAP_ENTRY_N_0_0(_T("Menu8"),                PS_MENUKEY8)
    MAP_ENTRY_N_0_0(_T("Menu9"),                PS_MENUKEY9)
    MAP_ENTRY_N_0_0(_T("Menu10"),               PS_MENUKEY10)
    MAP_ENTRY_N_0_0(_T("Menu11"),               PS_MENUKEY11)
    MAP_ENTRY_N_0_0(_T("Menu12"),               PS_MENUKEY12)
    MAP_ENTRY_N_0_0(_T("Menu13"),               PS_MENUKEY13)
    MAP_ENTRY_N_0_0(_T("Menu14"),               PS_MENUKEY14)
    MAP_ENTRY_N_0_0(_T("Menu15"),               PS_MENUKEY15)
    MAP_ENTRY_N_0_0(_T("RemoteButton1"),        PS_MENUKEY2)
    MAP_ENTRY_N_0_0(_T("RemoteButton1"),        PS_REMOTEBUTTON1)

    MAP_ENTRY_N_0_0_METHOD(_T("TabSelected"),   PS_LISTLOOKUPTAB, TabSelected)
    MAP_ENTRY_N_0_0_METHOD(_T("ProductImage"),  PS_LISTLOOKUP, ProductImage)
    MAP_ENTRY_N_0_0_METHOD(_T("TenderImage"),   PS_LISTLOOKUP, TenderImage)
    MAP_ENTRY_N_0_0_METHOD(_T("LanguageImage"), PS_LISTLOOKUP, LanguageImage)
    MAP_ENTRY_N_0_0_METHOD(_T("TareList"),      PS_LISTLOOKUP, TareList)
    MAP_ENTRY_N_0_0_METHOD(_T("CashBackList"),  PS_LISTLOOKUP, CashBackList)
    MAP_ENTRY_N_0_0_METHOD(_T("EBTCashBackList"),   PS_LISTLOOKUP, EBTCashBackList)
END_MAPPING()


CPSXInterface::CPSXInterface()
{
	InitializeMapping();
}


CPSXInterface::~CPSXInterface()
{
}


//************************************************************************ 
//
// Function:		InitializeMapping
// Purpose:			This function is called to add the entries to the mapping
//                  table the first time it is needed
// Parameters:      None
// Returns:	        void
//
//************************************************************************
void CPSXInterface::InitializeMapping()
{
    // Loop thru the mapping entries table until the last entry is found, which
    // is denoted by a NULL pointer for the key name.  Add each entry into the 
    // search map
    _TCHAR *pKeyname = NULL;
    int i=0;
    for (pKeyname = m_mappingEntries[i].keyname; 
         pKeyname != NULL; 
         i++, pKeyname = m_mappingEntries[i].keyname)
    {
        m_mapping.insert(std::make_pair(pKeyname, &m_mappingEntries[i]));
    }
}


//************************************************************************ 
//
// Function:		FindEntry
// Purpose:			This function is called to find the mapping entry that 
//                  corresponds with the given control name
// Parameters:      [in] strControlName - name of control that generated the event
// Returns:	        ptr to the correct mapping entry
//
//************************************************************************
CPSXInterface::SMappingEntry *CPSXInterface::FindEntry(const CString& strControlName)
{
    CMapping::iterator iter = m_mapping.find(strControlName);
    if (iter == m_mapping.end())
        return NULL;
        
    return iter->second;
}

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
void CPSXInterface::EventHandler( CPSX *pPSX, const CString& strRemoteConnectionName, const CString& strControlName, const CString& strContextName, const CString& strEventName, const COleVariant& vParam )
{
    CPSXInterface &psxi = *CPSXInterface::instance();
    bool bHandled = false;
    CEventHandlers handlers = psxi.m_vHandlers;

    SPSXEvent evt;
    evt.strRemoteConnectionName    = strRemoteConnectionName;
    evt.strControlName             = strControlName;
    evt.strContextName             = strContextName;
    evt.strEventName               = strEventName;
    evt.vParam                      = vParam;

    psxi.CrackPSXEvent( pPSX, &evt);
    
    for(CEventHandlers::iterator iter = handlers.begin();
	    !bHandled && (iter != handlers.end());
		iter++)
    {
        bHandled = (*iter)(pPSX, &evt);
    }

    if (!bHandled)
        psxi.LocalEventHandler(pPSX, &evt);
}


///<summary>This method adds an event handler to the chain to allow other
///objects to share the PSX instance created by SCOTAPP. </summary>
///<param name="pEventHandler">Pointer to the event handler being registered</param>
void CPSXInterface::Register(PPSXEVENTHANDLER pEventHandler)
{
    CPSXInterface &psxi = *CPSXInterface::instance();
    psxi.m_vHandlers.insert(pEventHandler);
}


///<summary>This method removes an event handler from the chain to prevent it
///from receiving events. </summary>
///<param name="pEventHandler">Pointer to the event handler being removed</param>
void CPSXInterface::Unregister(PPSXEVENTHANDLER pEventHandler)
{
    CPSXInterface &psxi = *CPSXInterface::instance();
    psxi.m_vHandlers.erase(pEventHandler);
}


//************************************************************************ 
//
// Function:		LocalEventHandler
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
void CPSXInterface::CrackPSXEvent( CPSX *pPSX
                                 , PPSXEVENT pEvent )
{
	USES_CONVERSION;
	CString csTemp;
    SPSXEvent &evt = *pEvent;

	if (evt.vParam.vt == VT_BSTR)
	{
		csTemp = evt.vParam.bstrVal;
		evt.wParam = _ttoi(csTemp);
	}
	else if (evt.vParam.vt == VT_I4)
	{
		evt.wParam = evt.vParam.iVal;
		evt.lParam = evt.vParam.iVal;
	}

    if (UI::EVENTCLICK == evt.strEventName)
    {
        SMappingEntry *pMappingEntry = FindEntry((const _TCHAR *)evt.strControlName);
        if (pMappingEntry != NULL)
        {
            bool bHandleThisEntry = true;

            // If the entry needs preprocessing first, then call that entry
            if (pMappingEntry->pMappingFxn != NULL)
            {
                bHandleThisEntry = 
                    (*(pMappingEntry->pMappingFxn))( pMappingEntry, &evt );
            }

            if (bHandleThisEntry)
            {
                evt.nEvent = pMappingEntry->evtType;
                if (pMappingEntry->wparmUsage == USE_VALUE)
                    evt.wParam = pMappingEntry->wparm;
                if (pMappingEntry->lparmUsage == USE_VALUE)
                    evt.lParam = pMappingEntry->lparm;
            }
        }

        // Handle all assist keys 1 - 200. Assist key button list control names must 
        // start with "Key".
        else
		{
			CString strControlName = (const _TCHAR *)evt.strControlName;
			if ( (strControlName.Left(3) == _T("Key")) &&  (!_istalpha(strControlName.GetAt(3)))
				 && (strControlName.GetLength() >= 4) )
			{
				evt.nEvent = (PSEVT)(PS_ASSISTKEY);
				evt.lParam = evt.wParam;
				evt.wParam = 0;
			}
		}
	}
	else if( evt.strEventName == UI::EVENTKEYDOWN )
	{
		evt.nEvent = PS_KEYSTROKE;
		evt.wParam = LOWORD( evt.vParam.lVal );
        evt.lParam = HIWORD( evt.vParam.lVal );
    }
	else if( evt.strEventName == UI::EVENTCHANGECONTEXT )
	{
		evt.nEvent = PS_SWITCHCONTEXT;
    }
	else if( evt.strEventName == UI::EVENTCONNECTREMOTE )
	{
		CString strPrefix = evt.strRemoteConnectionName.Left(REMOTE_CONNECTION_PREFIX_LENGTH);
		if( strPrefix == REMOTE_CONNECTION_PREFIX )
		{
			int nColonPosition = csTemp.Find(':');
			CString csContextRemote = csTemp.Left(nColonPosition);
            CString csTemp2 = &(((LPCTSTR)csTemp)[nColonPosition + 1]);
            nColonPosition = csTemp2.Find(':');
            CString machineName = csTemp2.Left(nColonPosition);
			CString loginID = &(((LPCTSTR)csTemp2)[nColonPosition + 1]);
#if defined(_SCOT) || defined(_SCOTSSF_)
            rp.SetOperator(loginID);
#endif
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
			
			evt.nEvent = PS_CONNECTREMOTE;
			CString* pstr1 = new CString;	
			*pstr1 = machineName;
			CString* pstr2 = new CString;	
			*pstr2 = csContextRemote;
			evt.lParam = (LPARAM)pstr1;
			evt.wParam = (WPARAM)pstr2;
		}
		else if( evt.strRemoteConnectionName == _T("AUTOMATION") )
		{
			COleVariant vParam = _T("");
			pPSX->AcceptConnection( (const _TCHAR *)evt.strRemoteConnectionName, vParam, TRUE );
		}
		else
		{
			COleVariant vParam = _T("");
			pPSX->AcceptConnection( (const _TCHAR *)evt.strRemoteConnectionName, vParam, FALSE );
        }
	}
	else if( evt.strEventName == UI::EVENTDISCONNECTREMOTE )
	{
		CString strPrefix = evt.strRemoteConnectionName.Left(REMOTE_CONNECTION_PREFIX_LENGTH);
		if( strPrefix == REMOTE_CONNECTION_PREFIX )
		{
            evt.nEvent = PS_REMOTE_DISCONNECTED;
            evt.lParam = evt.vParam.boolVal;
        }
	}

	// Rel 3.2 Requirement 4.5.1 - Single Mode RAP
	else if( evt.strEventName == UI::EVENTSWITCHASSISTMODE ) 
	{
        evt.nEvent = PS_SWITCHCONTEXT;
		CString* pstr1 = new CString;	
		CString* pstr2 = new CString;	
		//*pstr1 = OLE2T(vParam.bstrVal);
		*pstr1 = EMPTY_STRING;
		*pstr2 = (const _TCHAR *)evt.strContextName;
		evt.lParam = (LPARAM)pstr1;
		evt.wParam = (WPARAM)pstr2;
    }

	// Rel 3.2 Patch E - RAP.Net Signon event
	else if( evt.strEventName == UI::EVENTREMOTEDATA ) 
	{
        CString temp;
        temp = evt.vParam.bstrVal;
        if (-1 != temp.Find(_T("sign-on")))
        {
            evt.nEvent = PS_RAPSIGNON;
		    CString* pstr1 = new CString;	
		    *pstr1 = temp;
        }
    }

    if (!evt.nEvent)                    // unknown event
    {
      evt.nEvent = PS_UKEVT;
    }

    trace( L6, _T("PSXInterface::CrackPSXEvent:  %d,%d,%d,%s"), evt.nEvent, evt.lParam, evt.wParam, evt.strContextName );
}


//************************************************************************ 
//
// Function:		LocalEventHandler
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
void CPSXInterface::LocalEventHandler( CPSX *pPSX
                                     , PPSXEVENT pEvent )
{
    trace( L6, _T("PSXEventHandler Event( %s,%s,%s,%s,%s )"), pEvent->strRemoteConnectionName, pEvent->strControlName, pEvent->strContextName, pEvent->strEventName );
		
    trace( L6, _T("PostPS %d,%d,%d,%s"), pEvent->nEvent, pEvent->lParam, pEvent->wParam, pEvent->strContextName );
#if defined(_SCOT) || defined(_SCOTSSF_)
    mo.PostPS( pEvent->nEvent, pEvent->lParam, pEvent->wParam, (const _TCHAR *)pEvent->strContextName );
#else
	switch (pEvent->nEvent)
	{
		case PS_SWITCHCONTEXT :
            break;
		case PS_RAPSIGNON :
			SignOn(pEvent);
            break;
		case PS_RAPSIGNOFF :
			SignOff(pEvent);
            break;
		case PS_CONNECTREMOTE :
			ConnectToRemote(pEvent);
            break;
		case PS_REMOTE_DISCONNECTED :
			DisconnectFromRemote(pEvent);
            break;
		default :
			ForwardInput(pEvent);
	}
#endif
}


//************************************************************************ 
//
// Function:		TabSelected
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::TabSelected( SMappingEntry_ *pMappingEntry
                               , PPSXEVENT pEvent )
{
	USES_CONVERSION;
    if (pEvent->strContextName != _T("ProduceFavorites"))
        return false;

    pMappingEntry->wparmUsage = IGNORE_VALUE;

    CString *pTemp = new CString;
	*pTemp = OLE2T(pEvent->vParam.bstrVal);
    pMappingEntry->lparm = (LPARAM) pTemp;
	pMappingEntry->lparmUsage = USE_VALUE;

    return true;
}


void CPSXInterface::SetupEntry( SMappingEntry_ *pMappingEntry
                      , const COleVariant& vParam )
{
	USES_CONVERSION;

    //TAR 225285 check if sending back a string buffer, assign a string pointer to wParm and mark lParam to be 0
	// to differentiate the else if case.
	if (vParam.vt == VT_BSTR)
	{
 		CString *pTemp = new CString;
		*pTemp = OLE2T(vParam.bstrVal);
		pMappingEntry->wparm = (WPARAM) pTemp;
		pMappingEntry->wparmUsage = USE_VALUE;
		pMappingEntry->lparm = 0;
		pMappingEntry->lparmUsage = USE_VALUE;
	}
	else if (vParam.vt == VT_I4)
    {
		pMappingEntry->wparmUsage = IGNORE_VALUE;
		pMappingEntry->lparm = 1;
		pMappingEntry->lparmUsage = USE_VALUE;
    }
}


//************************************************************************ 
//
// Function:		ProductImage
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::ProductImage( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("ProduceFavorites") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:		TenderImage
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::TenderImage( SMappingEntry_ *pMappingEntry
                               , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("Finish") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:		LanguageImage
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::LanguageImage( SMappingEntry_ *pMappingEntry
                                 , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("SelectLanguage") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:		TareList
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::TareList( SMappingEntry_ *pMappingEntry
                            , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("SelectTare") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:		CashBackList
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::CashBackList( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("SelectPayment") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:		EBTCashBackList
// Purpose:			This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:	        true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::EBTCashBackList( SMappingEntry_ *pMappingEntry
                                   , PPSXEVENT pEvent )
{
    if ( pEvent->strContextName != _T("SelectPaymentEBT") )
        return false;

	SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


#if defined(_SCOT) || defined(_SCOTSSF_)
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
    error(RETURN,SM_CAT_SYSTEM_CALL, SCOTAPP_PSTHREAD_EXCEPTION,_T("%s"),szText);

    e->Delete();
  }
  catch(...)
  {
    // handle all exceptions
    error(RETURN,SM_CAT_SYSTEM_CALL, SCOTAPP_PSTHREAD_EXCEPTION2);

  }
  return 0;
}
#endif
