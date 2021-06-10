//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMRAPDataNeededBase.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RAPDataNeededBaseSTATE
#define _RAPDataNeededBaseSTATE

#ifndef _CPPUNIT
#include "SMOperatorPasswordState.h"
#include "SMUSSFManager.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif // _CPPUNIT
//USSF START
//USSF END
//#include "SMSmAbort.h"				// Tar 194454 RF020702
//#include "SMSmAuthorization.h"		// Tar 194454 RF020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMRAPDataNeededBase : public STATE(OperatorPasswordState)
{
public:

    SMRAPDataNeededBase();
#ifndef _CPPUNIT
	SMRAPDataNeededBase(LPCTSTR sDesc, int View);
	SMStateBase *Deliver(LPCTSTR sDesc, int View);
	SMStateBase *Deliver(LPCTSTR sDesc, int View, LPCTSTR pPtr, const bool storeMode);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif _CPPUNIT

  virtual SMStateBase * Initialize(void);
#ifndef _CPPUNIT
  virtual void UnInitialize(void);
  virtual SMStateBase * TimedOut(void);
  virtual SMStateBase *HandleApproveVoidedItem();
  virtual SMStateBase *HandleConfirmSuspend();
#endif //CPPUNIT
  virtual SMStateBase *HandlePricePrompt();
#ifndef _CPPUNIT
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *OnMatchedWt(void);				// Smart Scale reported a matched weight
  virtual SMStateBase *HandleAssistedEndorsement();
  virtual SMStateBase *HandleAssistedTender();
  virtual SMStateBase *HandleMaxCouponLimit();			//SR678
#endif //CPPUNIT
  virtual SMStateBase *HandleMaxCouponLimitFinalization();		//SR678
#ifndef _CPPUNIT
  virtual SMStateBase *HandleInvalidBarcode(); //SR879
#endif
  virtual SMStateBase *RAParse(MessageElement *me);
  virtual SMStateBase *SASecMgrParse( const MessageElement *me);
  virtual void OverrideSecurityException(LPCTSTR lpszAuthorization, LPCTSTR lpszOperation);
#ifndef _CPPUNIT 
  virtual bool PSRemoteLegalForState() {return true;};
  virtual bool storeMode();

  static int nlaststate;
#endif // _CPPUNIT	
protected:
    //++++METHODES STARTS HERE++++  
    /******cm150000: Modified in Merge 2******/
    virtual void SetupViewData();
    //virtual void SetupViewData() {ASSERT(FALSE);}
    /******cm150000: End Modified Merge 2******/

    //++++DATA MEMBERS STARTS HERE++++
    /******cm150000: Modify In Merge 2******/
    static CString m_sDesc;         // Prompt to appear above view desciprtion on RAP 
    static CString m_sView;         // String of RAP view to be displayed to RAP operator
    static CString m_sViewName;     // Message file Name for m_sView used for button text 040903
    /******cm150000: End Modify In Merge 2******/
    static PSTEXT m_ViewText;       // Message seen by customer during this state
#ifndef _CPPUNIT
//USSF Start
  friend SMUSSFManagerBase;
  friend SMUSSFManager;
//USSF End
  virtual SMStateBase *OnRAPDataReceived(LPCTSTR sData);
  virtual void         showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();

  // Uses m_View to determine what to ask for from RAP.
  virtual SMStateBase *BeginRAPView();

  // The following two functions are required to be overridden in SSF since
  // they make the application-specific decisions about what kind of data is 
  // needed. 

  // Called in Initialize() to set up the RAP view and the customer display
  // message.  This function must set m_ViewText and m_sView so that Initialize()
  // can paint the appropriate customer message.  Derived classes would call this 
  // if they cannot determine the VIEW type as an SSF view.


  // Uses m_View to determine what to do with data from RAP.
  
//  virtual SMStateBase  *PSButton1();         // Store Sign In
  virtual SMStateBase  *PSButtonGoBack();         // Store Sign In
	/******cm150000: Added In Merge 2******/
   virtual SMStateBase *HandleRAPData();
	/******cm150000: End Merge 2******/

  static CSCOTDataEntry sm_pDataNeededEntry;  // pointer to parameters
  static int m_saveDataNeededView;
  
  bool fStoreMode;

  //+jrparro TAR328916 - move this class members 'protected' so that it could overrriden/called in SSF
  //private:  
  int  nSaidCount;
#endif // _CPPUNIT
  int  m_origView;   // TAR 265143
#ifndef _CPPUNIT

   void SetupRAPLocalView(PSTEXT nDescID, CString csViewDetails, int nView);
   void SetupRAPLocalView(CString csDesc, CString csViewDetails, int nView);

#endif // _CPPUNIT
   virtual SMStateBase *HandleScannerData( const MessageElement* );    //SSCOADK-874
  DECLARE_DYNCREATE(SMRAPDataNeededBase)// MFC Runtime class/object information
};

//DECLARE_STATE(RAPDataNeeded)

#endif
