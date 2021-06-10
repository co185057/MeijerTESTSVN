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

#include "SMState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMInProgressBase : public 
#ifdef _BUILDLIB_
	SMStateBase
#else
	SMState
#endif
{
public:
  // Constructor - must tell InProgressBase the mode of the
  // current state that's creating it.
  SMInProgressBase(	const bool storeMode,
					const bool helpMode,
					const int  timeout);
  SMInProgressBase();
  SMStateBase *Deliver(	const bool storeMode,
						const bool helpMode,
						const int  timeout);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual SMStateBase* Initialize();
  virtual bool inProgress() {return true;}
  virtual bool storeMode()  {return fStoreMode;}
  virtual bool helpMode()   {return fHelpMode;}
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);  //TAR199913
  virtual SMStateBase  *OnWtDecrease(void); //TAR199913
  virtual void UnInitialize();
  virtual bool PSRemoteLegalForState();
  virtual bool PSAssistMenuLegalForState() { return true; };

	
protected:
  virtual SMStateBase  *TimedOut();
  virtual SMStateBase  *DMScanner();
  virtual SMStateBase  *DMCardReader();
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *DMAcceptor(void);               //TAR222852 CHU 052903
  virtual SMStateBase  *PSButtonHelp(void);             // TAR246429
  virtual SMStateBase *PSButton1(void);		            //rfc330995
  virtual SMStateBase  *DMParse(const MessageElement* ); //405090
  virtual SMStateBase  *SASecMgrParse( const MessageElement*);	//410299
  
//private:
  bool fStoreMode;
  bool fHelpMode;
  bool fAMNeedEnable;  // save the state of Assistmode button at RAP TAR246532
	
  DECLARE_DYNCREATE(SMInProgressBase)// MFC Runtime class/object information
};

//DECLARE_STATE(InProgress)

#endif
