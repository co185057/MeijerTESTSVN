//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMState.h
//
// TITLE: Class implementation file for Base State
// This class is used for deriving all actual
// states like STAttract and FirtTimeUserState.
// This state provides default processing for
// all possible messages to be parsed, usually
// an error because these messages should have
// been handled by the derived state and not end
// up here.
//
// This class is an 'abstract' class because it has
// at leas one 'pure virtual' function 'virtual...=0;'
// This means this class cannot be instanciated itself,
// but other classes derived from this once can.
//
// This state automatically accepts these inputs and provides
// a standard action:
// PSKey101   : 101-Keyboard Numerics, ENT, ESC, if PS allows
// DMScale  : Weight from the scale
//
//
//
// AUTHOR:    Peter Denhoed, Updated DcL-
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATE
#define _SMSTATE

#include "SMStateBase.h"	

//////////////////////////////
#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMState : public SMStateBase
{
	
public:
  SMState();

  virtual SMStateBase *TBParse(MessageElement* );	// same parse for all
  virtual TBSTATE TBProcessMessage(MessageElement* me);

  #ifdef _TESTSSF 
	static int m_SSFMoneyCenterGiftCardAmount; // the amount selected from the Select Shoppin Card Amount
	static CString m_csGiftCardTrackData;	   // track data from CARD DISPENSER
	CString GetAccountNumber(void);				// extract account number
	bool IsValidShoppingCard(CString& cspCardData);  //Validate inserted card 
	bool IsCardDispenserOK();	//check the status of card dispenser
	bool WriteGiftCard(bool bNewCard); //Write to card dispenser
  #endif	
#include "SMStateDM.h"
#include "SMStatePS.h"
#include "SMStateSA.h"
#include "SMStateTB.h"
#include "SMStateUE.h"
#include "SMStateCU.h"
  DECLARE_DYNCREATE(SMState) // MFC Runtime class/object information
};

#endif

