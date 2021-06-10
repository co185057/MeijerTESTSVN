//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntry.h
//
// TITLE: Class header for customer mode Data Entry state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMDataEntrySTATE
#define _CMDataEntrySTATE

#include "SMCmDataEntryBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMCmDataEntry : public SMCmDataEntryBase
{
public:
    SMCmDataEntry();          // constructor
    SMStateBase *Deliver(LPCTSTR pPtr);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState() ;

protected:
    virtual SMStateBase  *PSButton1(void); //SOTF 7789
    virtual SMStateBase  *PSButton2(void); //SOTF 7789
    virtual SMStateBase  *PSButton3(void); //SOTF 7789
    virtual SMStateBase  *PSButton4(void); //SOTF 7789
    virtual SMStateBase  *PSButton5(void); //SOTF 7789
    virtual SMStateBase  *PSButton6(void);
    virtual SMStateBase  *PSButton7(void);
    virtual SMStateBase  *PSNumericKey(const long);
    virtual SMStateBase  *PSEnterKey(void);
    virtual SMStateBase  *PSDecimalKey(const long ch);
    virtual SMStateBase  *PSOtherSpecialKeys(const long);
    virtual SMStateBase  *PSBackSpaceKey(void);
    virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAltGrKey(void);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSCharKey(TCHAR c);
    virtual SMStateBase  *PSAlphaKey(const long lKey);
    virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSButtonGoBack(void);
    virtual SMStateBase  *PSClearKey(void);
    virtual SMStateBase  *PSSpaceKey(void);
    virtual SMStateBase  *DMScanner();
    virtual SMStateBase  *DMScannerEncrypted();
    virtual SMStateBase  *DMCardReader(void);
    virtual SMStateBase  *TimedOut(void);
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); //TAR335785
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);
    virtual SMStateBase  *OnTBNeedData(void);
    virtual SMStateBase  *OnBackToLGW(void);
    virtual CString      GetButtonCommand(int nButtonIndex);
    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData);

	virtual SMStateBase  *PSButtonHelp(void); //POS37599
	virtual SMStateBase *RAParse(MessageElement *me);   //POS73196/71821 - turn on the yellow light - rrs
#ifdef _TRADESHOWDEMO
#include "..\TradeShowDemo\SMCmDataEntryTSDemo.h"
#endif

    DECLARE_DYNCREATE(SMCmDataEntry)    // MFC Runtime class/object information
};

#endif
