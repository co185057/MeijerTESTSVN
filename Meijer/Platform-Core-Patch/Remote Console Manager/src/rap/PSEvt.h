/////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSEvt.H
//
// TITLE:   Display Presentation Services Interface Procedures (High Level)
//
// This module handles all DPS calls for the SCOT application so that no other
// component need know the details of DPS calls and all other components are
// insulated from changes to the DPS API.
//
// From here we call the PSInterface routines
//
// 
// 081502: Tar 212025 CHu/FFR- Make function virtual so that they can overidden in SSF
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PSEVT
#define _PSEVT

const enum PSEVT
{
      PS_UKEVT,
      PS_HELP,
      PS_BUTTON1,
      PS_BUTTON2,
      PS_BUTTON3,
      PS_BUTTON4,
      PS_BUTTON5,
      PS_BUTTON6,
      PS_BUTTON7,
      PS_BUTTON8,
      PS_MENUKEY1,
      PS_MENUKEY2,
      PS_MENUKEY3,
      PS_MENUKEY4,
      PS_MENUKEY5,
      PS_MENUKEY6,
      PS_MENUKEY7,
      PS_MENUKEY8,
      PS_MENUKEY9,
      PS_MENUKEY10,
      PS_MENUKEY11,
      PS_MENUKEY12,
      PS_MENUKEY13,
      PS_MENUKEY14,
      PS_MENUKEY15,
      PS_RECEIPTUP,
      PS_RECEIPTDOWN,
      PS_LISTLOOKUP,
      PS_HELPTOPICS,
      PS_ENTERKEY,                    // Enter pressed on 101-keyboard
      PS_CANCELKEY,                   // Esc pressed on 101-keyboard
      PS_CLEARKEY,                    // BkSpc pressed on 101-keyboard
      PS_NUMERICKEY,                  // 0-9 pressed on 101-keyboard, key(char)lParm1, or decimal / period
      PS_DECIMALKEY,				  // decimal / period
      PS_ALPHAKEY,                    // 'A'-'Z' pressed on 101-keyboard, key(char)lParm1
      PS_VIDEODONE,
      PS_UKRELEASE,                   // lParm1=released button
      PS_UKSELCHANGE,                 // lParm1=IDC selected change
      PS_ASSISTKEY,                   // key from POS keyboard
      PS_ACTIVITY,                    // PS is active, don't timeout
      PS_MAINMENUKEY,                 // AM mode main menu button - TAR# 123183
	  PS_SPACEKEY,
		//@@@INFOPOINT
		PS_GOBACK,
		PS_REMOTE_DISCONNECTED,
		PS_KEYSTROKE,
      PS_BUTTONTOGGLE,                 // RAP Assistmode toggle button.
      PS_BUTTONLANE1STATUS,            // RAP Assistmode lane status "buttons".
      PS_BUTTONLANE2STATUS,
      PS_BUTTONLANE3STATUS,
      PS_BUTTONLANE4STATUS,
      PS_BUTTONLANE5STATUS,
      PS_BUTTONLANE6STATUS,
      PS_BUTTONLANE7STATUS,
      PS_BUTTONLANE8STATUS,
	  PS_REMOTEBUTTON1,				// Done button on Assistance Needed screen RFC 209075 RJF 073102
	  PS_REMOTEBUTTON2,
	  PS_REMOTEBUTTON3,
	  PS_REMOTEBUTTON4,
	  PS_LISTLOOKUPTAB,				// NewUI - new event for lookup item screen tab selection
	  PS_LISTSCROLL,					// NewUI - new event for scrolling produce lists
	  PS_CONNECTREMOTE,				// NewUI - new event for remote connecting
	  PS_SWITCHCONTEXT,              // Rel 3.2 Requirement 4.5.1 - Single Mode RAP 
	  PS_BACKSPACEKEY,
	  PS_SHIFTKEY,
	  PS_OTHERSPECIALKEY,
	  PS_ALPHANUMERICKEY,
	  PS_RAPSIGNON,
	  PS_RAPSIGNOFF,
      PS_SWITCHCONTEXTOTHER,
      PS_RAPSCANDATA,
      PS_REMOTEDATA,
	  PS_CONNECTREMOTELAUNCHPAD,
	  PS_ASSISTACCEPT,
	  PS_ASSISTDENIED,
      PS_COMMANDBUTTON,   //RFQ 986
     PS_HOTKEY,
     PS_CHARKEY,
	  PS_ALTGRKEY,
	  PS_COMMANDLIST,     //  RFQ 2605
	  PS_LASTEVT    // do not add any new entries below PS_LASTEVT, add above this line
	  // *******************
	  // any addition to this enum list should have a corresponding entry in PSEventNameList also
	  // *******************
};
    
#endif
