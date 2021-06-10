//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMInSAProgressBase.h
//
// Description: The InSAProgress state is used when the transition from 
//			TB_ITEMSOLD to the next Security event, which is the SASERVER WLBD
//			lookup return, and might take some time.  If App doesn't wait long 
//			enough, the DBDClient will return local database info
//
// Author:  Leila Parker
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _InSAProgressBaseSTATE
#define _InSAProgressBaseSTATE

#include "SMState.h"

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif

SMInSAProgressBase : public 
#ifdef _BUILDLIB_
	SMStateBase
#else
	SMState
#endif
{
public:
  // Constructor - must tell InSAProgressBase the mode of the
  // current state that's creating it.
  SMInSAProgressBase(	const bool storeMode,
						const bool helpMode);
  SMInSAProgressBase();
  SMStateBase *Deliver(	const bool storeMode,
						const bool helpMode);

  virtual bool inProgress() {return true;}
  virtual bool storeMode()  {return fStoreMode;}
  virtual bool helpMode()   {return fHelpMode;}
  virtual SMStateBase  *Initialize(void);
  virtual SMStateBase  *OnDataBaseInfoComplete(void);
  virtual void UnInitialize(void);          // optional

protected:
  virtual SMStateBase  *TimedOut(void);
  virtual SMStateBase  *DMScanner(void);
  virtual SMStateBase  *DMCardReader(void);
  virtual SMStateBase  *OnBackToLGW(void);				// Smart Scale reported that the unmatched weight was removed
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void);
  virtual SMStateBase  *OnWtDecrease(void);
  virtual SMStateBase  *DMdf_EASNoMotion(void);
  virtual SMStateBase  *PSButtonHelp(void);       // TAR246429

  //229084 created a Base function for getNextStateForGoodItem LPM022503
  bool fStoreMode;
  bool fHelpMode;
	
  DECLARE_DYNCREATE(SMInSAProgressBase)// MFC Runtime class/object information
};

#endif
