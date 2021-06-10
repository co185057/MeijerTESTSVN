//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashierPassword.h
//
// TITLE: Class header for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmCashierPasswordSTATE
#define _SmCashierPasswordSTATE

#include "SMSmCashierPasswordBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCashierPassword : public SMSmCashierPasswordBase
{
public:
    SMSmCashierPassword();					  // constructor
    virtual SMStateBase * Initialize(void);
    virtual void          UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSNumericKey(const long lKey);
    virtual void ShowCashierIDSceen();
    virtual void promptForCashierData( PROMPT pInputPrompt );
    virtual SMStateBase * DMScanner(void);
    virtual SMStateBase* HandleScannerData( const MessageElement* message);

    DECLARE_DYNCREATE(SMSmCashierPassword)     // MFC Runtime class/object information
};

#endif
