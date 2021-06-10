//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashierPasswordBase.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCashierPasswordBaseSTATE
#define _SmCashierPasswordBaseSTATE

#include "SMStoreModeState.h"


const enum PROMPT
{
  PROMPT_CASHIERID,
  PROMPT_CASHIERPW,
  PROMPT_CASHIERREENTER
};

#ifdef _SCOTSSF_

class DLLIMPORT
#else
class DLLEXPORT
#endif

SMSmCashierPasswordBase : public STATE(StoreModeState)
{

public:
  SMSmCashierPasswordBase();		     // constructor
  virtual SMStateBase * Initialize(void);        // required override
  virtual void          UnInitialize(void);

	PROMPT pInputPrompt;

  CString csCashierID;                           // Cashier ID
  CString csCashierPw;                           // Cashier password


protected:

  virtual SMStateBase  *PSButton8(void);         //
  virtual SMStateBase  *PSEnterKey(void);        // override for enter key
  virtual SMStateBase  *PSClearKey(void);
  virtual SMStateBase  *PSNumericKey(const long lKey);
//  virtual void promptForCashierID();
//  virtual void promptForCashierPw();
  virtual void ShowCashierIDSceen(); //TAR 395474
  virtual void promptForCashierData( PROMPT pInputPrompt );
  virtual SMStateBase * DMScanner(void); //Tar 417254
  virtual SMStateBase* HandleScannerData( const MessageElement* message);
  

  DECLARE_DYNCREATE(SMSmCashierPasswordBase)// MFC Runtime class/object information
};
//
// The following macro declares the function:
//
//    DLLEXPORT SMStateBase *makeSMSmCashierPassword(void);
// or
//    DLLEXPORT SMStateBase *makeSMSmCashierPasswordBase(void);
// 
// depending on whether _BUILDLIB_ is defined or not. This function
// creates the SMSmCashierPassword objects.  This function is called by
// RETURNSTATE(SmCashierPassword) instead of calling new STATE(SmCashierPassword) so
// that SCOTSSF works correctly.
//
// The function is implemented in SMSmCashierPasswordBase.cpp and SMSmCashierPassword.cpp
// by using the macro IMPLEMENT_STATE(SmCashierPassword)

//DECLARE_STATE(SmCashierPassword)

#endif
