//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCmDataEntryBase.h
//
// TITLE: Class header for customer mode DataEntry state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CmDataEntryBaseSTATE
#define _CmDataEntryBaseSTATE

#include "SMCustomerModeState.h"
//USSF START
#include "SMUSSFManager.h"
//USSF END

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCmDataEntryBase : public SMCustomerModeState
{
public:
          SMCmDataEntryBase();			         // constructor
	SMStateBase *Deliver(LPCTSTR pPtr);          // RFC 197839&197844
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize(void);        // required override
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState() { return true; };
protected:
//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End
    virtual SMStateBase  *PSButton1(void);         // override for button 1
    virtual SMStateBase  *PSButton2(void);         // override for button 2
    virtual SMStateBase  *PSButton3(void);         // override for button 3
    virtual SMStateBase  *PSButton4(void);         // override for button 4
    virtual SMStateBase  *PSButton5(void);         // override for button 4
    virtual SMStateBase  *PSButton6(void);         // override for button 4
    virtual SMStateBase  *PSButton7(void);         // override for button 4
    virtual SMStateBase  *PSNumericKey(const long);	// a key 0-9 was pressed
    virtual SMStateBase  *PSEnterKey(void);        // override for enter key
    virtual SMStateBase  *PSDecimalKey(const long ch);
    virtual SMStateBase  *PSOtherSpecialKeys(const long);
	virtual SMStateBase  *PSBackSpaceKey(void);
	virtual SMStateBase  *PSShiftKey(void);
    virtual SMStateBase  *PSAlphaKey(const long lKey);
	virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSButtonGoBack(void);        // override for enter key
    virtual SMStateBase  *PSClearKey(void);        // override for clear key
    virtual SMStateBase  *PSSpaceKey(void);        // override for space key
 //   virtual SMStateBase  *PSCmdList(CString, CString, const long);  //RFQ 2605
    virtual SMStateBase  *DMScanner();             // scanned item
	virtual SMStateBase  *DMCardReader(void);  //RFQ 2605
    virtual SMStateBase  *TimedOut(void);          // override for time out 
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
    virtual SMStateBase  *OnWtDecrease(void);
    virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight
    virtual SMStateBase  *OnTBNeedData(void);
	virtual SMStateBase	 *OnBackToLGW(void);		// TAR 272200
    virtual CString      GetButtonCommand(int nButtonIndex);
    static CSCOTDataEntry sm_pDataNeededEntry;      // pointer to parameters
    static int nlaststate;
    static bool savebEASReEnableScanner;	// Tar 210186 RF 071902
	static bool bInSmCmDataEntry;           // TAR 371194
    CString csCurContext;    // Tar 238987
	bool fCapsInput;
	bool bShiftOn;
    int nRelativeButton[8];
	
    DECLARE_DYNCREATE(SMCmDataEntryBase)// MFC Runtime class/object information
};

#endif
