#ifndef _SM_RECALLING_ITEMS_
#define _SM_RECALLING_ITEMS_

#include "SMRecallingItemsBase.h"
//#include <queue>

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
SMRecallingItems : public SMRecallingItemsBase
{
public:
    SMRecallingItems();
    virtual SMStateBase * Initialize();
    virtual void UnInitialize(void);  
    virtual SMStateBase * PSButton1();
    virtual bool inProgress() {return false;} // Return here after timeout.
    virtual bool storeMode()  {return false;}
    virtual bool helpMode()   {return false;}

    //+SSCO Mobile
    virtual SMStateBase  *OnSkipBaggingAllowed(void);
    virtual SMStateBase  *OnBackToLGW(void); //430778   
    virtual SMStateBase* OnMatchedWt(void); // Smart Scale reported a matched weight
    virtual void FinishRecall();

	static long g_nTimeoutRecallingWaitForPos;    // RFQ 9545
	static bool g_bRecallingWaitForPos;           // RFQ 9545
    //-SSCO Mobile
	
	virtual SMStateBase *TBParse(MessageElement *me); //CRD 177093
	static CString csSuspendBarcode;

protected:
    virtual SMStateBase  *TimedOut();
    virtual SMStateBase *SASecMgrParse( const MessageElement*);
    virtual void ShowRecallingScreen();

DECLARE_DYNCREATE(SMRecallingItems)
};

#endif
