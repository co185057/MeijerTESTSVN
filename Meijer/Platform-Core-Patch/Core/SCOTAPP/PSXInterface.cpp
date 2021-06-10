// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/PSXInterface.cpp 12    3/16/10 2:36p Sm133076 $
/*
 * @(#)PSXInterface.h    $Revision: 12 $ $Date: 3/16/10 2:36p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Core/SCOTAPP/PSXInterface.cpp $
 * 
 * 12    3/16/10 2:36p Sm133076
 * 
 * 11    3/01/10 9:55a Sm133076
 * SOTF 7387 FL TLOG Displays Manager Override Number = Not Found
 * 
 * 10    1/05/10 8:39p Cu185008
 * Migrating Core to G2.
 * 
 * 9     4/29/09 4:06p Sm133076
 * 381428 Meijer lab 4.32.6 FL Reports: Daily Summary Report by Cashier
 * are reporting incorrect operator ID
 * 
 * 8     1/15/09 5:10p Sa250050
 * Patch G Migration 337 --> 339
 * 
 * 7     12/16/08 2:09p Sa250050
 * Patch G Build 337 Migration
 * 
 * 6     11/14/08 2:35p Sa250050
 * TAR387302
 * 
 * 5     11/06/08 2:44p Sa250050
 * RFQ 2183 - DG35 - Patch G Migration
 * 
 * 4     10/30/08 4:17p Sm133076
 * Patch G Migration
 * 
 * 3     8/26/08 5:37p Sa250050
 * RFC380920
 * 
 * 1     6/23/08 4:57p Sa250050
 * FastLane 4.0 E5.3 Build 291 - Core Source
 * 
 * 26    3/05/08 9:23a De200005
 * TAR371639
 * 
 * 25    1/21/08 10:26a Tn185011
 * Merge the fix from E3 Hotfix into E5
 * 
 * 5     9/20/07 5:36p Db185023
 * This conditional was added in order to fix 352443 - 4.02.08.19a
 * Carrefour FastLane appears to hang 5 out of 300 transactions.
 * Add the check for UI::EVENTREMOTEDATA in order to allow LaunchPadNet to
 * sigon. LaunchPadNet signon 
 * takes two steps. First the connect remote, than a remote data contining
 * the id/password for TB validation.
 * Add the check for UI::EVENTDISCONNECTREMOTE just for insurance but I
 * don't think it is used right now.
 * 
 * 4     8/15/07 2:12p Sk185127
 * RFC 356672
 * 
 * 3     8/01/07 11:02a Dk185053
 * TAR 356672
 * Mapped the newly added button
 * 
 * 2     6/12/07 11:23a Tn185011
 * Comment out the section caused Desktop RAP and HHRAP cannot connect to
 * Lane
 * 
 * 1     6/07/07 3:15p Dk185053
 * Prevent the unneccesary event from Launchpad and RCM to scotapp
 * 
 * 22    10/11/06 2:53p Ah150000
 * store mode alphanumeric keyboard support
 * 
 * 21    10/06/06 4:28p Ah150000
 * 
 * 20    10/04/06 5:15p Bd123405
 * Reverse partial import of THD alpha lowercase/uppercase fix.
 * 
 * 19    9/26/06 4:26p Bd123405
 * RFQ 986 - Basic Assist Mode Enhancements
 * 
 * 18    8/22/06 5:05p Mb185050
 * TAR 330580 - THD - toggle issue at RAP & shift key
 * 
 * 17    4/12/06 3:52p Wr185001
 * TAR 320367 - Add an interface so that SSF functions can 
 * hook into Psx Event handling.  Also Add a way that Psx Event button
 * clicks can be mapped via an API, rather than modifying the static array
 * inside PsxInterface all the time.
 * 
 * 16    1/30/06 4:11p Jv185012
 * TAR 316675:  4.2 E Build 211(Restricted Shell&NonAdmin) : XP PRO -
 * Reports do not work.  -->  Merged changes for TAR 304421 from 4.0D into
 * 4.0E.
 * 
 * 15    1/10/06 7:43p Mg185024
 * 4.2 Card Dispenser
 * 
 * 14    1/04/06 4:27p Mg185024
 * 4.2 provide subtle on-screen volume control
 * 
 * 13    7/21/05 8:36a Wr185001
 * TAR 305613 - #define of 'error' in ScotAppErrorObject.h and common.h
 * was causing Microsoft header files to fail the ScotApp compile.  
 * During ScotApp builds, 'error' will not be defined.  
 * For backwards SSF compatibility, 'error'  WILL be #defined via a
 * conditional compile #ifndef _SCOT.  This will allow legacy 3rd party
 * non ScotApp code and SSF code to compile.  
 * 
 * 
 * 12    5/31/05 11:10a Bd123405
 * 
 * 10    4/22/05 5:06p Tb185000
 * TAR #?
 * Dr. Watson when scanning barcode from HHRap.
 * 
 * 9     4/08/05 11:40p Dp185016
 * cash management screen
 * 
 * 7     4/01/05 11:24a Bd123405
 * TAR 297254
 * 
 * 6     05-03-24 11:17a Bj185005
 * 
 * 5     3/22/05 9:02a Dm185016
 * TAR 295699
 * 
 * 4     3/17/05 2:58p Tp151000
 * TAR 295473
 * 
 * 3     3/08/05 11:42a Cb160006
 * TAR 294958
 * 
 * 2     2/22/05 6:27p Tp151000
 * 293972
 * 
 * 1     1/26/05 2:08p Bd123405
 * Initial check-in of 4.0 Patch B
// 
// 14    1/18/05 3:04p De200005
// TAR 285290 (handle Alt-F4)
// 
// 13    1/17/05 8:27p Dm185016
// Added method trace for the event handler.
// 
// 12    1/13/05 5:31p Dm185016
// Remote scanner data support.
// 
// 11    12/20/04 6:57p Dm185016
// TAR 287899 - More try/catch.
// 
// 10    12/14/04 11:30a Dm185016
// HH support
// 
// 9     12/09/04 5:27p Dm185016
// TAR 287899 - Added workaround to prevent crash.  Added tracing.
// 
// 8     12/08/04 6:25p Dm185016
// TAR 287900 - Remote connection no longer handled at all by SCOTAPP.
// Was causing assert in RAProcedure and blinking in and out of
// WaitForAssistance.
// 
// 7     11/16/04 2:23p Dm185016
// Added launchpad connection
// 
// 6     11/15/04 3:09p Dm185016
// Update to talk to RAP.net
// 
// 5     11/11/04 10:10a Dm185016
// Moved "RAP" prefix to header file
// 
// 4     10/26/04 1:45p Dm185016
// Fixed problems routing events.
// 
// 3     10/14/04 2:00p Dm185016
// Fixed problems recognizing the buttons
// 
// 2     10/08/04 1:39p Dm185016
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
#define     EMPTY_STRING  _T("")
#endif

#include "PSXMfc.h" 
#include "psevt.h" 
#include "PSXInterface.h"           // This header
#include "RAPUIDefines.h"
#include "RCMKeyValue.h"


#include "StringComparator.h"
#include <map>

//USSF Start
#include <afxdisp.h>
#include "USSFProcedures.h"
#include "SMUSSFManager.h"
#include "USSFCommon.h"
#include "USSFUtility.h"
//USSF End

// local defines //
#define COMP_ID     ID_PS
#define T_ID        _T("PSt")

#include "MethodTrace.h"



//BEGIN_MAPPING(CPSXInterface::mappingEntries)
CPSXInterface::SMappingEntry CPSXInterface::m_mappingEntries[] = {
    MAP_ENTRY_N_W_L(_T("SpaceKey"),             PS_KEYSTROKE, 32, 0)
    MAP_ENTRY_N_W_L(_T("ShiftKey1"),            PS_KEYSTROKE, VK_SHIFT, 0)
    MAP_ENTRY_N_W_L(_T("AltGrKey"),             PS_KEYSTROKE, VK_ALTGR, 0)//VK_ALTGR is a macro we defined in CPSXInterface.h. (Microsoft does not define a VK_XXXX code for the AltGr key.)
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
    MAP_ENTRY_N_0_0(_T("CMButton8Global"),      PS_BUTTON8)
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
	MAP_ENTRY_N_0_L(_T("AlphaNumP3"),           PS_KEYSTROKE, 0)
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
    MAP_ENTRY_N_0_0(_T("SmCashManagementRemoveList"),        PS_UKEVT)//dp185016 generic cash management
    MAP_ENTRY_N_0_0(_T("SmCashManagementUnloadList"),        PS_UKEVT)
    MAP_ENTRY_N_0_0(_T("SmCashManagementLoadList"),        PS_UKEVT)
	MAP_ENTRY_N_0_0(_T("Load"),        PS_UKEVT)   //4.2 card dispenser
	MAP_ENTRY_N_0_0(_T("ButtonVolumeControl"),        PS_BUTTON7)   //4.2 provide subtle on-screen volume control
    MAP_ENTRY_N_0_0_METHOD(_T("TabSelected"),   PS_LISTLOOKUPTAB, TabSelected)
    MAP_ENTRY_N_0_0_METHOD(_T("ProductImage"),  PS_LISTLOOKUP, ProductImage)
	MAP_ENTRY_N_0_0_METHOD(_T("QuickPickImage"),PS_LISTLOOKUP, QuickPickImage)
    MAP_ENTRY_N_0_0_METHOD(_T("TenderImage"),   PS_LISTLOOKUP, TenderImage)
    MAP_ENTRY_N_0_0_METHOD(_T("LanguageImage"), PS_LISTLOOKUP, LanguageImage)
    MAP_ENTRY_N_0_0_METHOD(_T("TareList"),      PS_LISTLOOKUP, TareList)
    MAP_ENTRY_N_0_0_METHOD(_T("CashBackList"),  PS_LISTLOOKUP, CashBackList)
    MAP_ENTRY_N_0_0_METHOD(_T("EBTCashBackList"),   PS_LISTLOOKUP, EBTCashBackList)
    MAP_ENTRY_N_0_L(_T("AlphaNumP1_LCase"),           PS_KEYSTROKE, 0)							
    MAP_ENTRY_N_0_L(_T("AlphaNumP2_LCase"),           PS_KEYSTROKE, 0)
	MAP_ENTRY_N_0_L(_T("AlphaNumP3_LCase"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("AlphaNumP4_LCase"),           PS_KEYSTROKE, 0)
	MAP_ENTRY_N_0_L(_T("SmAlphaNumP1"),           PS_KEYSTROKE, 0)							
    MAP_ENTRY_N_0_L(_T("SmAlphaNumP2"),           PS_KEYSTROKE, 0)
	MAP_ENTRY_N_0_L(_T("SmAlphaNumP3"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_L(_T("SmAlphaNumP4"),           PS_KEYSTROKE, 0)
    MAP_ENTRY_N_0_0_METHOD(_T("HotKeys"),        PS_HOTKEY, HotKey)
    MAP_ENTRY_N_0_0_METHOD(_T("Line1AlphaNumericKeys"),            PS_CHARKEY, CharKey)							
    MAP_ENTRY_N_0_0_METHOD(_T("Line2AlphaNumericKeys"),            PS_CHARKEY, CharKey)							
    MAP_ENTRY_N_0_0_METHOD(_T("Line3AlphaNumericKeys"),            PS_CHARKEY, CharKey)							
    MAP_ENTRY_N_0_0_METHOD(_T("Line4AlphaNumericKeys"),            PS_CHARKEY, CharKey)

END_MAPPING()


CPSXInterface::CMapping CPSXInterface::m_externalMapping;

CCriticalSection CPSXInterface::m_lockMap;


CPSXInterface::CPSXInterface()
: m_bAssistKeyQueued(false)
{
    InitializeMapping();
}


CPSXInterface::~CPSXInterface()
{
}


//************************************************************************ 
//
// Function:        InitializeMapping
// Purpose:         This function is called to add the entries to the mapping
//                  table the first time it is needed
// Parameters:      None
// Returns:         void
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
// Function:        FindEntry
// Purpose:         This function is called to find the mapping entry that 
//                  corresponds with the given control name
// Parameters:      [in] strControlName - name of control that generated the event
// Returns:         ptr to the correct mapping entry
//
//************************************************************************
CPSXInterface::SMappingEntry *CPSXInterface::FindEntry(const CString& strControlName)
{
    CMapping::iterator iter = m_mapping.find(strControlName);
    if (iter == m_mapping.end())
        return NULL;
        
    return iter->second;
}


//************************************************************************ 
//
// Function:        FindExternalEntry
// Purpose:         This function is called to find the mapping entry that 
//                  corresponds with the given control name
// Parameters:      [in] strControlName - name of control that generated the event
// Returns:         ptr to the correct mapping entry
//
//************************************************************************
CPSXInterface::SMappingEntry *CPSXInterface::FindExternalEntry(const CString& strControlName)
{
    CSingleLock lockRequest(&m_lockMap);
	lockRequest.Lock();//TAR371639
    CMapping::iterator iter = m_externalMapping.find(strControlName);
    if (iter == m_externalMapping.end())
        return NULL;
        
    return iter->second;
}




//************************************************************************ 
//
// Function:        AddExternalPsxEventMappingEntry
// Purpose:         Add a click event translation hanndler to the list
//                  of translation handlers. 
// Parameters:      [in] KeyName - name of control that generated the event
//					[in] evtType - event type 
//					[in] wParam	 - wparam to be placed in the event, if any
//					[in] bUseWParam - true if wparam is to be replaced in the event
//					[in] lParam	- lParam to be placed into the event, if any 
//					[in] bUseLParam - true if the lParam is to be placed into the event
//					[in] pMappingFxn - function to call to translate the event
// Returns:         void
//
//************************************************************************

void CPSXInterface::AddExternalPsxEventMappingEntry( LPCTSTR KeyName, 
													PSEVT evtType,
													long  wParam,
													bool  bUseWParam,
													long  lParam, 
													bool  bUseLParam,
													PMAPPINGFXN pMappingFxn )
{
    CSingleLock lockRequest(&m_lockMap);
    lockRequest.Lock();//TAR371639

	CPSXInterface::SMappingEntry *pMapping = new CPSXInterface::SMappingEntry();
	pMapping->keyname = _tcsdup( KeyName );
	pMapping->evtType = evtType;
	pMapping->wparm = bUseWParam ? wParam : 0;
	pMapping->wparmUsage = bUseWParam ? USE_VALUE : IGNORE_VALUE;
	pMapping->lparm = bUseLParam ? lParam : 0;
	pMapping->lparmUsage = bUseLParam ? USE_VALUE : IGNORE_VALUE;
	pMapping->pMappingFxn = pMappingFxn;

    m_externalMapping.insert(std::make_pair( pMapping->keyname, pMapping));
}


//************************************************************************ 
//
// Function:        RemoveExternalPsxEventMappingEntry
// Purpose:         This function is called to find the mapping entry that 
//                  corresponds with the given control name
// Parameters:      [in] strControlName - name of control that generated the event
// Returns:         ptr to the correct mapping entry
//
//************************************************************************

bool CPSXInterface::RemoveExternalPsxEventMappingEntry( LPCTSTR strControlName )
{
    CSingleLock lockRequest(&m_lockMap);
    lockRequest.Lock();//TAR371639

    CMapping::iterator iter = m_externalMapping.find(strControlName);
    if (iter != m_externalMapping.end())
	{
		CPSXInterface::SMappingEntry *p = ( CPSXInterface::SMappingEntry * ) iter->second;
		free ( p->keyname );
		delete p;
		m_externalMapping.erase( iter );
		return true;
	}

	return false;
}



///////////////////////////////////////////////////////////////////////

//************************************************************************ 
//
// Function:        PSXEventHandler
// Purpose:         This function is called by the UI subsystem when an event occurs
//                  (i.e. button click)
// Parameters:      [in] pPSX - pointer to an instance of PSX object
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         void
// Calls:           PostPs
// Side Effects:
// Notes:           
//
//************************************************************************
void CPSXInterface::EventHandler( CPSX *pPSX, const CString& strRemoteConnectionName, const CString& strControlName, const CString& strContextName, const CString& strEventName, const COleVariant& vParam )
{
    TRACE_METHOD(_T("CPSXInterface::EventHandler"));

//USSF START
    trace( L5, _T("+EventHandler Event( %s,%s,%s,%s )"), strRemoteConnectionName, strControlName, strContextName, strEventName );

	USES_CONVERSION;

	CString csUSSFRemoteConnection = strRemoteConnectionName, csUSSFControlName = strControlName, csUSSFContextName = strContextName, csUSSFEventName = strEventName;
	COleVariant vUSSFVParam = vParam;

	CString csAltContext;
	if (SMStateBase::pUSSFNative != NULL)
	{
		SMStateBase::pUSSFNative -> StoreUSSFData(_T("PSXCONNECTNAME"), (COleVariant)csUSSFRemoteConnection);
		SMStateBase::pUSSFNative -> StoreUSSFData(_T("PSXCONTROLNAME"), (COleVariant)csUSSFControlName);
		SMStateBase::pUSSFNative -> StoreUSSFData(_T("PSXCONTEXTNAME"), (COleVariant)csUSSFContextName);
		SMStateBase::pUSSFNative -> StoreUSSFData(_T("PSXEVENTNAME"), (COleVariant)csUSSFEventName);
		SMStateBase::pUSSFNative -> StoreUSSFData(_T("PSXVPARAM"), vParam);	
		SMStateBase *nextState = SMStateBase::pUSSFNative -> ProcessEventPartial(csUSSFEventName, csUSSFContextName);
		if (nextState != STATE_NULL)
		{
			if ((nextState > STATE_ERROR) || (nextState < 0))
			{
				trace(L5, _T("USSF Returned non-NULL State; Deleting."));
				delete nextState;
			}
			trace(L5, _T("USSF Returned non-NULL State; Event Bypassed."));
			return;
		}
		// Store Data Variables Back (in case changed).
		USSFutil.GetTransactionVariable(_T("USSF_PSXCONNECTNAME"), csUSSFRemoteConnection);
		USSFutil.GetTransactionVariable(_T("USSF_PSXCONTROLNAME"), csUSSFControlName);
		USSFutil.GetTransactionVariable(_T("USSF_PSXCONTEXTNAME"), csUSSFContextName);
		USSFutil.GetTransactionVariable(_T("USSF_PSXEVENTNAME"), csUSSFEventName);
		USSFutil.GetTransactionVariable(_T("USSF_PSXVPARAM"), vUSSFVParam);

		trace( L5, _T("USSF EventHandler Event( %s,%s,%s,%s )"), csUSSFRemoteConnection, csUSSFControlName, csUSSFContextName, csUSSFEventName );
	}
//USSF END

    CPSXInterface &psxi = *CPSXInterface::instance();
    bool bHandled = false;

    SPSXEvent evt;
//USSF START
    try
    {
        evt.strRemoteConnectionName    = csUSSFRemoteConnection;
        evt.strControlName             = csUSSFControlName;
        evt.strContextName             = csUSSFContextName;
        evt.strEventName               = csUSSFEventName;
        evt.vParam                      = vUSSFVParam;
        evt.nEvent                      = -1;
//USSF END

        psxi.CrackPSXEvent( pPSX, &evt);
    }
    catch (...)
    {
        trace( L2, _T("PSXInterface::EventHandler caught failure in CrackPSXEvent") );
    }

    // if it's an assist key and a previous one has 
    // not been processed discard the keystroke
    if( (evt.nEvent == PS_ASSISTKEY) && psxi.GetAssistKeyQueued() )
    {
        trace( L2, _T("PSXInterface::EventHandler - Received an Assist Mode key press without having processed the previous one.  Discarding..."));
        bHandled = true;
    }
    
    PPSXEVENTHANDLER pHandler = NULL;
    if (!((evt.nEvent==PS_ASSISTDENIED) || (evt.nEvent==PS_ASSISTACCEPT)))				
	{
    try
    {
        for(CEventHandlers::iterator iter = psxi.m_vHandlers.begin();
            !bHandled && (iter != psxi.m_vHandlers.end());
            iter++)
        {
            pHandler = *iter;
            bHandled = (pHandler)(pPSX, &evt);
        }
    }
    catch (...)
    {
        trace( L2, _T("PSXInterface::EventHandler caught failure in registered event handler") );
    }
	}

    try
    {
        if (!bHandled)
            psxi.LocalEventHandler(pPSX, &evt);
    }
    catch (...)
    {
        trace( L2, _T("PSXInterface::EventHandler caught failure in local event handler!") );
    }

    trace( L6, _T("-EventHandler Event( %s,%s,%s,%s )"), strRemoteConnectionName, strControlName, strContextName, strEventName );
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
// Function:        LocalEventHandler
// Purpose:         This function is called by the UI subsystem when an event occurs
//                  (i.e. button click)
// Parameters:      [in] pPSX - pointer to an instance of PSX object
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         void
// Calls:           PostPs
// Side Effects:
// Notes:           
//
//************************************************************************
void CPSXInterface::CrackPSXEvent( CPSX *pPSX
                                 , PPSXEVENT pEvent )
{
    USES_CONVERSION;
    SPSXEvent &evt = *pEvent;

	if( SUCCEEDED( VariantChangeType( &evt.vParam, &evt.vParam, 0, VT_I4 ) ) )
	{
        trace( L6, _T("Default settings event") );
		evt.wParam = evt.vParam.lVal;
		evt.lParam = evt.vParam.lVal;
	}
    	else
	{
		evt.wParam = 0;
		evt.lParam = 0;
	}

	// This conditional was added in order to fix 352443 - 4.02.08.19a Carrefour FastLane appears to hang 5 out of 300 transactions.
	// Add the check for UI::EVENTREMOTEDATA in order to allow LaunchPadNet to sigon. LaunchPadNet signon 
	// takes two steps. First the connect remote, than a remote data contining the id/password for TB validation.
	// Add the check for UI::EVENTDISCONNECTREMOTE just for insurance but I don't think it is used right now.
	if (((evt.strRemoteConnectionName == _T("AUTOMATION")) || (evt.strRemoteConnectionName.Find(LAUNCHPAD_PREFIX) == 0)) &&
		(evt.strEventName != UI::EVENTCONNECTREMOTE && evt.strEventName != UI::EVENTREMOTEDATA && evt.strEventName != UI::EVENTDISCONNECTREMOTE))
	{
		return;
	}


    if (UI::EVENTCLICK == evt.strEventName)
    {
        trace( L7, _T("Button Click detected") );
        SMappingEntry *pMappingEntry = FindExternalEntry((const _TCHAR *)evt.strControlName);
		if	( pMappingEntry == NULL )
		{
			pMappingEntry = FindEntry((const _TCHAR *)evt.strControlName);
		}
        if (pMappingEntry != NULL)
        {
            trace( L7, _T("Entry Found") );
            bool bHandleThisEntry = true;

            // If the entry needs preprocessing first, then call that entry
            if (pMappingEntry->pMappingFxn != NULL)
            {
				trace( L7, _T("PsxInterface: Calling mapping fxn") );
				try
				{
					bHandleThisEntry = 
						(*(pMappingEntry->pMappingFxn))( pMappingEntry, &evt );
				}
				catch(...)
				{
					trace( L2, _T("Exception caught Calling mapping fxn in PsxInterface") );
				}
            }

            if (bHandleThisEntry)
            {
        trace( L7, _T("Entry Handled") );
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
                trace( L7, _T("Thinks its an assist key") );
                evt.nEvent = (PSEVT)(PS_ASSISTKEY);
                evt.lParam = evt.wParam;
                evt.wParam = 0;
            }

            //RFQ 986
            if ( (strControlName.Left(7) == _T("CmdBtn_")) && (strControlName.GetLength() >= 7) )
            {
                trace( L7, _T("Processing command button %s"), strControlName);
                evt.nEvent = (PSEVT)(PS_COMMANDBUTTON);
                CString *pcsCommand = new CString;
                CString *pcsControl = new CString;

                *pcsCommand = OLE2T(pEvent->vParam.bstrVal);
                *pcsControl = strControlName;
                evt.wParam = (LPARAM) pcsCommand;
                evt.lParam = (LPARAM) pcsControl;
            }
            //RFQ 986
			//RFQ 2605  - CmdList handling
			if ( (strControlName.Left(8) == _T("CmdList_")) &&  (strControlName.GetLength() >= 8) )
            {
                trace( L7, _T("Processing CmdList %s"), strControlName);
				CString *pcsControl = new CString;
                *pcsControl = strControlName;
                evt.nEvent = (PSEVT)(PS_COMMANDLIST);
                evt.lParam = (LPARAM) pcsControl;
                evt.wParam = evt.wParam;
            }
			//End RFQ 2605

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
        evt.nEvent = PS_SWITCHCONTEXTOTHER;
    }
    else if( evt.strEventName == UI::EVENTCONNECTREMOTE )
    {
        trace( L6, _T("PSXInterface::remote connection:  %s"), evt.strRemoteConnectionName );

		CString temp = evt.vParam.bstrVal;
        trace( L6,  _T("%s temp:  %s"), UI::EVENTREMOTEDATA, temp);
		if (-1 != temp.Find(KEY_CT_OPERATION_SIGNON_AUTO))
		{
			//add operator id to table
			StripAndAddOperator(temp, evt.strRemoteConnectionName);		// 409017
			
		}

        if( evt.strRemoteConnectionName.Find(REMOTE_CONNECTION_PREFIX) == 0 )
        {
            trace( L6, _T("PSXInterface::Connecting to RAP station:  %s"), evt.strRemoteConnectionName );
            evt.nEvent = PS_CONNECTREMOTE;
        }
        else if( evt.strRemoteConnectionName.Find(LAUNCHPAD_PREFIX) == 0 )
        {
            trace( L6, _T("PSXInterface::Connecting to Launchpad:  %s"), evt.strRemoteConnectionName );
            COleVariant vParam = _T("");
            pPSX->AcceptConnection( (const _TCHAR *)evt.strRemoteConnectionName, vParam, TRUE );
        }
        else if( evt.strRemoteConnectionName == _T("AUTOMATION") )
        {
            trace( L6, _T("PSXInterface::Connecting to AUTOMATION") );
            COleVariant vParam = _T("");
            pPSX->AcceptConnection( (const _TCHAR *)evt.strRemoteConnectionName, vParam, TRUE );
        }
        else
        {
            trace( L6, _T("PSXInterface::Accepting connection:  %s"), evt.strRemoteConnectionName );
            COleVariant vParam = _T("");
            pPSX->AcceptConnection( (const _TCHAR *)evt.strRemoteConnectionName, vParam, FALSE );
        }
    }
    else if( evt.strEventName == UI::EVENTDISCONNECTREMOTE )
    {
        trace( L6, _T("PSXInterface::remote disconnection:  %s"), evt.strRemoteConnectionName );
        SMStateBase::addOperatorID(evt.strRemoteConnectionName, _T(""));
        if( evt.strRemoteConnectionName.Find(REMOTE_CONNECTION_PREFIX) == 0 )
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
        CString temp = evt.vParam.bstrVal;
		trace( L6,  _T("%s temp:  %s"), UI::EVENTREMOTEDATA, MaskedParam(temp));
        //  +TAR 409017
		if (-1 != temp.Find(KEY_CT_OPERATION_SIGNON_AUTO))
		{
			//add operator id to table
			StripAndAddOperator(temp, evt.strRemoteConnectionName);
		}
		//  -TAR 409017
		else if (-1 != temp.Find(KEY_CT_OPERATION_SIGNON))
        {
            evt.nEvent = PS_RAPSIGNON;
        }
        else if (-1 != temp.Find(KEY_CT_OPERATION_SCANDATA))
        {
            evt.nEvent = PS_RAPSCANDATA;
        }
        else if (-1 != temp.Find(KEY_CT_ASSIST_DENIED))		
        {
            evt.nEvent = PS_ASSISTDENIED;
        }
        else if (-1 != temp.Find(KEY_CT_ASSIST_ALLOWED))
        {
            evt.nEvent = PS_ASSISTACCEPT;
        }
        else if (-1 != temp.Find(KEY_CT_OPERATION_BIOMET))
        {
            evt.nEvent = PS_RAPBIOMET;
        }
        else
        {
            evt.nEvent = PS_REMOTEDATA;
        }
    }
    else if(UI::EVENTQUIT == evt.strEventName) //TAR 258290
    {
        trace(L2, _T("ALT-F4 pressed, STOP of application initiated") );
        mo.PostGP(GP_STOPSCOT);
        mo.PostGP(GP_STOP);
        AfxGetApp()->PostThreadMessage(WM_QUIT,0,0);
        return;
    }

    if (!evt.nEvent)                    // unknown event
    {
      evt.nEvent = PS_UKEVT;
    }

    trace( L6, _T("PSXInterface::CrackPSXEvent:  %d,%d,%d,%s"), evt.nEvent, evt.lParam, evt.wParam, evt.strContextName );
}


//************************************************************************ 
//
// Function:        LocalEventHandler
// Purpose:         This function is called by the UI subsystem when an event occurs
//                  (i.e. button click)
// Parameters:      [in] pPSX - pointer to an instance of PSX object
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         void
// Calls:           PostPs
// Side Effects:
// Notes:           
//
//************************************************************************
void CPSXInterface::LocalEventHandler( CPSX *pPSX
                                     , PPSXEVENT pEvent )
{
    trace( L6, _T("PSXEventHandler Event( %s,%s,%s,%s )"), pEvent->strRemoteConnectionName, pEvent->strControlName, pEvent->strContextName, pEvent->strEventName );
        

    switch (pEvent->nEvent)
    {
        case PS_RAPSIGNON :
            trace( L6, _T("Ignoring event") );
            break;
        case PS_CONNECTREMOTE :
            trace( L6, _T("Ignoring event") );
            break;
        case PS_REMOTE_DISCONNECTED :
            trace( L6, _T("Ignoring event") );
            break;
		case PS_CONNECTREMOTELAUNCHPAD :
			trace( L6, _T("Ignoring event") );
			break;
        case -1 :
            pEvent->nEvent = PS_UKEVT;        
        case PS_SWITCHCONTEXTOTHER :
        default :
            trace( L6, _T("PostPS %d,%d,%d,%s")
                 , pEvent->nEvent
                 , pEvent->lParam
                 , pEvent->wParam
                 , pEvent->strContextName );
#if defined(_SCOT) || defined(_SCOTSSF_)

            if( pEvent->nEvent == PS_ASSISTKEY )
            {
                SetAssistKeyQueued(true);
            }

            mo.PostPS( pEvent->nEvent
                     , pEvent->lParam
                     , pEvent->wParam
                     , (const _TCHAR *)pEvent->strContextName );
            break;
#endif
    }
}


//************************************************************************ 
//
// Function:        TabSelected
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::TabSelected( SMappingEntry_ *pMappingEntry
                               , PPSXEVENT pEvent )
{
    USES_CONVERSION;
	if (pEvent->strContextName.Find(_T("ProduceFavorites")) == -1)
        return false;
    pMappingEntry->wparmUsage = IGNORE_VALUE;

    CString *pTemp = new CString;
    *pTemp = OLE2T(pEvent->vParam.bstrVal);
    pMappingEntry->lparm = (LPARAM) pTemp;
    pMappingEntry->lparmUsage = USE_VALUE;

    return true;
}

// +TAR 409017 
//************************************************************************ 
//
// Function:        StripAndAddOperator
// Purpose:         This function is called to add the operator to the table
// Parameters:      [in] temp - CString to strip id from
// Returns:         void
//
//************************************************************************
void CPSXInterface::StripAndAddOperator(CString temp, CString strRemoteConnectionName)
{
	CString csDelim = _T("UserId[text]=");
 
	 int iFind = 0;
	 CString csTok;
	 bool bStatus = false;

	 iFind = temp.Find(csDelim);
	 if(iFind > 0)
	 {
		 csTok = temp.Mid(iFind+csDelim.GetLength());
		 iFind = csTok.Find(_T(";"));
		 if (iFind > 0)
		 {
		 	csTok = csTok.Left(iFind);
			bStatus = true;
		 }
		 if (!bStatus)
		 csTok.Empty();

		 trace( L6,  _T("operator id extracted is:  %s"), csTok);
	 }


	if(!csTok.IsEmpty())
    {
		SMStateBase::addOperatorID(strRemoteConnectionName, csTok);
    }
}
// -TAR 409017 

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
// Function:        ProductImage
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::ProductImage( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
	if (pEvent->strContextName.Find(_T("ProduceFavorites")) == -1) 
        return false;

    SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}

//************************************************************************ 
//
// Function:        QuickPickImage
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::QuickPickImage( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
    SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}


//************************************************************************ 
//
// Function:        TenderImage
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
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
// Function:        LanguageImage
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
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
// Function:        TareList
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
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
// Function:        CashBackList
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
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
// Function:        EBTCashBackList
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] strRemoteConnectionName - remote connection name
//                  [in] strControlName - name of control that generated the event
//                  [in] strContextName - name of context
//                  [in] strEventName - name of event - Click, ...
//                  [in] vParam - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
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

//************************************************************************ 
//
// Function:        HotKey
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct...
//                  [in] pEvent - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::HotKey( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
   if (pEvent->strContextName.Find(_T("Keyboard"))!=0) 
        return false;

    SetupEntry( pMappingEntry, pEvent->vParam );

    return true;
}

//************************************************************************ 
//
// Function:        CharKey
// Purpose:         This function is called to validate the control of the same name
// Parameters:      [in] pMappingEntry - pointer to the mapping struct
//                  [in] pEvent - extra event specific parameter
// Returns:         true if the control was handled; false otherwise
//
//************************************************************************
bool CPSXInterface::CharKey( SMappingEntry_ *pMappingEntry
                                , PPSXEVENT pEvent )
{
   if (pEvent->strContextName.Find(_T("Keyboard"))!=0) 
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
    //  The SCOT AuxDisplay sometimes spoofs numeric data
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
#endif



CString CPSXInterface::MaskedParam(const CString& csParam)
{	
	CString csVal(csParam);
	CString csReturn = _T("");
	CString csTemp = csVal;	
	int nLength = csVal.GetLength();	
	bool bFlag = true; 

	do{			
		int nStart = csVal.Find(cMASKED);
		if(nStart > -1)   
		{
			int nIndexEq = csVal.Find(cEQUAL, nStart);
			int nIndexCol = csVal.Find(cSEMICOLON, nIndexEq);
			if(nIndexEq > -1)
			{
				csTemp = csVal.Left(nIndexEq + 1);
				csTemp += cASTERISK;
			}

			if((nIndexCol > -1) && (nIndexCol < nLength))
            {
				csVal = csVal.Mid(nIndexCol, nLength - nIndexCol);
			}
			else
			{
				bFlag = false;
			}

		}
		else
		{
			return csTemp;
		}		
		csReturn += csTemp;
	}while(bFlag);

	return csReturn;
}
#pragma warning( pop )
