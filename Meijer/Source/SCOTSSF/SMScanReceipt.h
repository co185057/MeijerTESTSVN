//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:     SMScanReceipt.H
//
// TITLE:    Class header for ScanReceipt State
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMScanReceiptState
#define _SMScanReceiptState

#include "SMCustomerModeState.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif

SMScanReceipt : public SMCustomerModeState 
{
public:
    SMScanReceipt();
    SMStateBase *Initialize(void);
    void UnInitialize(void);
		
    DECLARE_DYNAMIC(SMScanReceipt)   //MFC Runtime class/object information

protected:
	SMStateBase *PSButtonGoBack(void);
	SMStateBase *DMScanner();
	SMStateBase  *TimedOut(void);

};

#endif

