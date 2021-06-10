//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateUEBASE.h
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSTATEUEBASE
#define _SMSTATEUEBASE

protected:
  // start up
  virtual long SMStateBase::UserExit00(void);   // initialize user exit
	
  // let user exits check incomming TB Displays and cause re-direction
  virtual SMStateBase *SMStateBase::UserExit01(void);	// before SCOT/IS
  virtual SMStateBase *SMStateBase::UserExit02(void);	// after SCOT/IS
	
  // Customer Reciept print top line exit
  virtual long SMStateBase::UserExit15(CString &);
	
  // Before TBItemSold is sent to TSNT
  virtual SMStateBase *SMStateBase::UserExit16(const bool, const long, const CString);
	
  // shut down
  virtual void SMStateBase::UserExit99(void);   // un-initialize user exit
	
#endif
