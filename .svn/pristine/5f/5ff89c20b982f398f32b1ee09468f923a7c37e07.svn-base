// SCOTAuxScreenList.h: Lists the possible SCOT Aux Screen values
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOTAUXSCREENLIST_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_)
#define AFX_SCOTAUXSCREENLIST_H__2AC2B013_D031_11D3_BBAD_00A0C9EDD523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef const enum
{
  AUX_SCREEN_DEFAULT = 0,                 // Default must ALWAYS come first
  AUX_SCREEN_SIGNATURE_NEEDED = 1,        // Sigcap form must ALWAYS come second!
  AUX_SCREEN_ATTRACT,                     // Attract screen form if no dual-language
  AUX_SCREEN_ATTRACT_LANG,                // Attract screen form if dual-language on
  AUX_SCREEN_EBT_TYPE,                    // Type of EBT Benefit
  AUX_SCREEN_EBT_NO_FS_TYPE,              // Type of EBT Benefit - No Food Stamps
  AUX_SCREEN_CARD_MISREAD,                // MSR Card was misread
  AUX_SCREEN_CARD_PROCESSING,             // Card is currently processing
  AUX_SCREEN_CARD_TYPE,                   // Select type of card swiped
  AUX_SCREEN_CARD_TYPE_NO_EBT,            // Select type of card swiped - No EBT
  AUX_SCREEN_CARD_TYPE_NOCASH,            // Select type of card swiped - No cash
  AUX_SCREEN_CARD_TYPE_NO_CASH_NO_EBT,    // Select type of card swiped - No cash, No EBT
  AUX_SCREEN_CONFIRM_DEBIT_AMT,           // confirm amount charged to debit card
  AUX_SCREEN_CONFIRM_EBT_AMT,             // confirm amount charged to EBT card
  AUX_SCREEN_CONFIRM_FS_AMT,              // confirm amount charged to food stamps
  AUX_SCREEN_CONFIRM_FS_BAL,              // confirm amount charged to food stamps with balance
  AUX_SCREEN_FINISH_PAY,                  // Finish & Pay form if cash payment allowed
  AUX_SCREEN_FINISH_PAY_CARD,             // Finish & Pay form if cash payment NOT allowed
  AUX_SCREEN_LANE_CLOSED,                 // Displayed when lane is closed (out of service)
  AUX_SCREEN_LOADING,                     // displayed when lane is loading and used for testing
  AUX_SCREEN_SB_ITEMS_NOSWIPE_CARDCASH,   // displayed on Scan&Bag screen if items sold, no swipe, cash allowed
  AUX_SCREEN_SB_ITEMS_NOSWIPE_CARDONLY,   // displayed on Scan&Bag screen if items sold, no swipe, card only
  AUX_SCREEN_SB_NOITEMS_NOSWIPE,          // displayed on Scan&Bag screen if no items sold, no swipe
  AUX_SCREEN_SB_PREVSWIPE,                // displayed on Scan&Bag screen if card swiped
  AUX_SCREEN_SELECT_AMOUNT,               // Displayed to select Cash Back amount
  AUX_SCREEN_THANK_YOU,                   // Displayed when transaction is over and Thank-you screen is up
  AUX_SCREEN_USE_TOUCHSCREEN,             // Customer should use main touchscreen (this is default if no other screen specified)
  AUX_SCREEN_SSF1,                        // Reserved for use by SSF
  AUX_SCREEN_SSF2,                        // Reserved for use by SSF
  AUX_SCREEN_SSF3,                        // Reserved for use by SSF
  AUX_SCREEN_SSF4,                        // Reserved for use by SSF
  AUX_SCREEN_SELECT_PAYMENT_TYPE,         // Select Payment Type Form
} SCOT_AUX_SCREEN;

// This constant should be kept equal to the last screen
// which is currently AUX_SCREEN_SSF4

#define AUX_SCREEN_MAX  AUX_SCREEN_SELECT_PAYMENT_TYPE

// Just a guess, but we will reserve
// 40 screens per language for right now.
// Each screen will have a lang_base (0, 40, 80,etc)
// and then it's enum value will be added to the
// base to arrive at which slot it is stored in on
// the device.typedef struct AuxDispDpsEvent
// 
// Actually, at this point this is not how we load them.  Because we seem to be
// running out of memory storing forms, I have changed to just storing them
// one slot after another, will no base per language.  However, if the 5992 mem
// problems ever get resolved, I will go back to using this method as it is
// much more clean


const long AUX_SCREENS_PER_LANG = 40; 

#endif