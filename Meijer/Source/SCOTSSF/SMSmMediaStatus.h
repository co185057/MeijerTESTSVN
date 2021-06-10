//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatus.h
//
// TITLE: Class header for store mode MediaStatus state
//
// AUTHOR:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMMEDIASTATUSSTATE
#define _SMMEDIASTATUSSTATE

#include "SMSmMediaStatusBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmMediaStatus : public SMSmMediaStatusBase
{
public:
    SMSmMediaStatus();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton4(void);
    virtual SMStateBase  *PSButton5(void);
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *DMCashChangerInOperable(void);
    virtual SMStateBase *PSReceiptUp(void);
    virtual SMStateBase *PSReceiptDown(void);
    virtual void initializeHopperList();
    virtual void initializeCurrencyList();
    virtual void initializePrinterList();
    virtual void initializeCashAcceptorStatusLine();
    virtual void initializeCoinAcceptorStatusLine();
    virtual void UpdateDeviceStatus(); 
    virtual long DMCheckHealth(long nDevClass);
    virtual void getHopperStatus(const CString denominationCode,
        CString& denominationStatus);

    virtual CString DisplayCoinDispenserStatus(void);
    virtual void InitializeCoinDispenserStatus(void);
    virtual void initializeCardDispenserStatus(void);
    virtual SMStateBase * ReloadCashDevices(void);

    DECLARE_DYNCREATE(SMSmMediaStatus)// MFC Runtime class/object information
};

#endif