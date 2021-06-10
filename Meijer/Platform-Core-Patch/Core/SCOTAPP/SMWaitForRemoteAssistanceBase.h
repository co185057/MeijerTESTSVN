//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMWaitForRemoteAssistanceBase.h
//
// TITLE: Class header for wait for approval state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WaitForRemoteAssistanceBaseSTATE
#define _WaitForRemoteAssistanceBaseSTATE

#include "SMOperatorPasswordState.h"
//#include "SMSmAbort.h"				// Tar 194454 RF020702
//#include "SMSmAuthorization.h"		// Tar 194454 RF020702

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMWaitForRemoteAssistanceBase : public STATE(OperatorPasswordState)
{
public:

	SMWaitForRemoteAssistanceBase();
	SMWaitForRemoteAssistanceBase( LPCTSTR sMachineName, LPCTSTR sContextRemote );
	SMStateBase *Deliver( LPCTSTR sMachineName, LPCTSTR sContextRemote );
  virtual SMStateBase * Initialize(void);
  virtual void UnInitialize(void);
  virtual SMStateBase * TimedOut(void);
  virtual SMStateBase  *OnWtIncreaseNotAllowed(void); 
  virtual SMStateBase  *OnWtDecrease(void);
  virtual bool DeliverDescriptorList(CDescriptorList &dlParameterList);
  virtual bool IsLoginBypassAllowed() { return false; };
  
protected:
  virtual void         showScreen(bool passwordVerified);
  virtual SMStateBase  *stateAfterPasswordVerify();
  virtual SMStateBase *PSConnectRemote(CString&,CString&);

//  virtual SMStateBase  *PSButton1();         // Store Sign In
	CString m_sMachineName;
	CString m_sRemoteContext;
    CString csCurContext;    // Tar 238987

private:
  int  nSaidCount;

  DECLARE_DYNCREATE(SMWaitForRemoteAssistanceBase)// MFC Runtime class/object information
};


#endif
