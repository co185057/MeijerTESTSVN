//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMRemoteScannerConnectBase.h
//
// Description: A SMRemoteScannerConnectBase is a SMSystemMessageOpPass.  It is
//   like SystemMessage3 (continue and cancel buttons), but an
//   operator/password is required to see this one.
//
// Author: Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RemoteScannerConnectBaseSTATE
#define _RemoteScannerConnectBaseSTATE

#ifndef _CPPUNIT
#include "SMOperatorPasswordState.h"
#else
#include "FakeSMOperatorPasswordStateBase.h"
#endif // _CPPUNIT

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
  SMRemoteScannerConnectBase : public STATE(OperatorPasswordState)
{
public:
#ifndef _CPPUNIT
    SMRemoteScannerConnectBase(bool bConnect);
    SMRemoteScannerConnectBase();
    ~SMRemoteScannerConnectBase();
    SMStateBase *Deliver(bool bConnect);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *PSButton1(void);
#endif // _CPPUNIT
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
#ifndef _CPPUNIT
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *stateAfterPasswordVerify(void);
    virtual bool PSRemoteLegalForState();
    virtual void UnInitialize(void);
    virtual bool storeMode(){ return m_bStoreMode;};
    
protected:

    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase *DMRemoteScannerConnect(bool bConnect); 
    virtual void showRemoteScreen(void);
    virtual void showLocalScreen(void);
    bool m_bStoreMode;
   
#endif // _CPPUNIT
    DECLARE_DYNCREATE(SMRemoteScannerConnectBase)// MFC Runtime class/object information
};

//DECLARE_STATE(RemoteScannerConnect)

#endif
