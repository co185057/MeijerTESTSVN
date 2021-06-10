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

#ifndef _CPPUNIT
#include "SMCustomerModeState.h"
//USSF START
#include "SMUSSFManager.h"
//USSF END
#else
#include "FakeSMCustomerModeState.h"
#endif //_CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMCmDataEntryBase : public SMCUSTOMERMODESTATE
{
public:
          SMCmDataEntryBase();			         // constructor
	SMStateBase *Deliver(LPCTSTR pPtr);          // RFC 197839&197844
#ifndef _CPPUNIT
    virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
    virtual SMStateBase * Initialize(void);        // required override
    virtual void UnInitialize(void);
    virtual bool PSAssistMenuLegalForState() { return true; };
#endif //_CPPUNIT
	//SR801 - change to public from protected
	static SCOTDATAENTRY sm_pDataNeededEntry;      // pointer to parameters
protected:
#ifndef _CPPUNIT
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
    //+SR828
    virtual SMStateBase  *PSAltGrKey(void);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSCharKey(TCHAR c);
    //-SR828
    virtual SMStateBase  *PSAlphaKey(const long lKey);
	virtual SMStateBase  *PSAlphaNumericKey(const long lKey);
    virtual SMStateBase  *PSButtonGoBack(void);        // override for enter key
    virtual SMStateBase  *PSClearKey(void);        // override for clear key
    virtual SMStateBase  *PSSpaceKey(void);        // override for space key
#endif
    virtual SMStateBase  *DMScanner();             // scanned item
	virtual SMStateBase  *DMScannerEncrypted();   //  SR 93.4
#ifndef _CPPUNIT
	virtual SMStateBase  *DMCardReader(void);  //RFQ 2605
    virtual SMStateBase  *TimedOut(void);          // override for time out 
    virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
#endif
    virtual SMStateBase  *OnWtDecrease(void);
	
#ifndef _CPPUNIT
    virtual SMStateBase  *OnMatchedWt(void);		// Smart Scale reported a matched weight
    virtual SMStateBase  *OnTBNeedData(void);
	virtual SMStateBase	 *OnBackToLGW(void);		// TAR 272200
    virtual CString      GetButtonCommand(int nButtonIndex);

    virtual SMStateBase *OnRAPUnsolicitedDataReceived(LPCTSTR sData); //SSCOP-3803
#endif
    virtual SMStateBase* HandleScannerData( const MessageElement* message);
#ifndef _CPPUNIT
    static int nlaststate;
	static bool bInSmCmDataEntry;           // TAR 371194
    CString csCurContext;    // Tar 238987
	bool fCapsInput;
	bool bShiftOn;
    bool bAltGrOn; //SR828
    int nRelativeButton[8];
#endif //_CPPUNIT
private:
#ifndef _CPPUNIT
    enum PSProceduresBase::HKStatus m_HKStatus; //SR828
   
	//SR801
	bool bDNEnableSecurity; //set to true if Security is enable in dataneeded
    static bool ms_bRAPHasScanned; //SSCOP-3803
#endif //_CPPUNIT
    DECLARE_DYNCREATE(SMCmDataEntryBase)// MFC Runtime class/object information
};

#endif
