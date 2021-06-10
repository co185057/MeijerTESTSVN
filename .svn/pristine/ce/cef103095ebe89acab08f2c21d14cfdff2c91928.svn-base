//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeeded.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RAPDataNeededSTATE
#define _RAPDataNeededSTATE


#include "SMRAPDataNeededBase.h"

#define DATANEEDED_ASSISTMODE    (DATANEEDED_LASTVIEW + 1)

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRAPDataNeeded : public SMRAPDataNeededBase
{
public:
   typedef enum
      {
      RAPDATA_LASTVIEW           // Highest element in the list, place others before it.
      } RAPDATANEEDEDVIEW;

    SMRAPDataNeeded(LPCTSTR sDesc, int View);
    SMRAPDataNeeded();
    SMStateBase *Deliver(LPCTSTR sDesc, int View);
    SMStateBase *Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode);

    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize(void);
    virtual void UnInitialize(void);
    virtual SMStateBase * TimedOut(void);
    virtual SMStateBase *HandleApproveVoidedItem();
    virtual SMStateBase *HandleConfirmSuspend();
    virtual SMStateBase *HandlePricePrompt();
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);
	SMStateBase *HandleAssistMode();
    virtual SMStateBase *HandleAssistedEndorsement();
    virtual SMStateBase *HandleAssistedTender();
    virtual SMStateBase *HandleMaxCouponLimit();
    virtual SMStateBase *HandleMaxCouponLimitFinalization();
    virtual SMStateBase *HandleInvalidBarcode();
    virtual SMStateBase *RAParse(MessageElement *me);
    virtual SMStateBase *SASecMgrParse( const MessageElement *me);
    virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation);
    virtual bool PSRemoteLegalForState();
    virtual bool storeMode();

protected:
    virtual void SetupViewData();
	CString m_savedViewText;
	bool bNegativeMediaTBNeedMoreData;

    virtual SMStateBase *OnRAPDataReceived(LPCTSTR sData);
    virtual void         showScreen(bool passwordVerified);
    virtual SMStateBase  *stateAfterPasswordVerify();
    virtual SMStateBase *BeginRAPView();
    virtual SMStateBase  *PSButtonGoBack();
    virtual SMStateBase *HandleRAPData();
    virtual SMStateBase *HandleScannerData( const MessageElement* );

	virtual SMStateBase  *OnNoMotionCashOutSensor(void);

	//POS32142 - rrs
    bool bNeedHoldWeight;
	
    bool bRAPDataNeededBypass; // POS50491

    DECLARE_DYNCREATE(SMRAPDataNeeded)// MFC Runtime class/object information
};

#endif
