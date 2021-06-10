//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmConfirmCancelTransaction.h
//
// TITLE: Class header for store mode ConfirmCancelTransaction state
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SmConfirmCancelTransactionSTATE
#define _SmConfirmCancelTransactionSTATE

#include "SMStoreModeState.h"

#ifdef _SCOTSSF_
	class DLLEXPORT
#else
	class DLLIMPORT
#endif
SMSmConfirmCancelTransaction : public STATE(StoreModeState)
{
public:
    SMSmConfirmCancelTransaction();
    SMStateBase  *Initialize(void);
    
protected:
    SMStateBase  *PSButton1(void);
    SMStateBase  *PSButton2(void);
    
    DECLARE_DYNCREATE(SMSmConfirmCancelTransaction)// MFC Runtime class/object information
};

//DECLARE_STATE(SmConfirmCancelTransaction)

#endif