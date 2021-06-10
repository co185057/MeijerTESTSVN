//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInProgressBase.h
//
// Description: The InProgressBase state is used when the transition from a
//        state to the next state might take some time.  InProgressBase is a
//        "chameleon" state with respect to Customer Mode, Store Mode,
//        and Help Mode - it becomes the mode of the creating state and
//        therefore does not inherit from the above classes.
//
//
// Author:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _InProgressBaseSTATE
#define _InProgressBaseSTATE



#ifdef _CPPUNIT 
#include "FakeSMStateBase.h"
#include "FakeMessageElement.h"

#else
#include "SMState.h"
#endif

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMInProgressBase : public STATE(State)
{
public:
  // Constructor - must tell InProgressBase the mode of the
  // current state that's creating it.
  
#ifndef _CPPUNIT
  SMInProgressBase(	const bool storeMode,
					const bool helpMode,
					const int  timeout);
  virtual ~SMInProgressBase();
#endif //CPPUNIT
  SMInProgressBase();
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);  //TAR199913
#ifndef _CPPUNIT
  SMStateBase *Deliver(	const bool storeMode,
						const bool helpMode,
						const int  timeout);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
#endif // _CPPUNIT
  virtual SMStateBase* Initialize();
#ifndef _CPPUNIT
  virtual bool inProgress() {return true;}
  virtual bool storeMode()  {return fStoreMode;}
  virtual bool helpMode()   {return fHelpMode;}
  virtual SMStateBase  *OnWtDecrease(void); //TAR199913
  virtual void UnInitialize();
  virtual bool PSRemoteLegalForState();
  virtual bool PSAssistMenuLegalForState() { return true; };

	
protected:
  virtual SMStateBase  *TimedOut();
  virtual SMStateBase  *DMScanner();
  virtual SMStateBase  *DMCardReader();
  virtual SMStateBase  *OnBackToLGW(void);// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *OnTBNeedData(void); //SSCOADK-493
#endif //CPPUNIT
  virtual SMStateBase  *DMAcceptor(void);//TAR222852 CHU 052903
#ifndef _CPPUNIT
  virtual SMStateBase  *PSButtonHelp(void);             // TAR246429
  virtual SMStateBase *PSButton1(void);		            //rfc330995
  virtual SMStateBase  *DMParse(const MessageElement* ); //405090
  virtual SMStateBase  *SASecMgrParse( const MessageElement*);	//410299
  virtual SMStateBase  *TBParse(MessageElement* );	// same parse for all
#endif //_CPPUNIT

//private:
  bool fStoreMode;
  bool fHelpMode;
  bool fAMNeedEnable;  // save the state of Assistmode button at RAP TAR246532


  DECLARE_DYNCREATE(SMInProgressBase)// MFC Runtime class/object information
};

//DECLARE_STATE(InProgress)

#endif
