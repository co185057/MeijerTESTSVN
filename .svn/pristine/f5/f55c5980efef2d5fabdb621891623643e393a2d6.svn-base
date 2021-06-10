//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMRemoteScannerConnect.h
//
// Description: A SMRemoteScannerConnect handle Remote Scanner connect/disconnect. An
//   operator/password is required to see this one.
//
// Author: Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RemoteScannerConnectSTATE
#define _RemoteScannerConnectSTATE

#include "SMRemoteScannerConnectBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMRemoteScannerConnect : public SMRemoteScannerConnectBase
{
public:
    SMRemoteScannerConnect( bool bConnect);// DMCheckHealth after msg?
    SMRemoteScannerConnect();
    SMStateBase *Deliver( bool bConnect);
    virtual SMStateBase  *Initialize(void);
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton2(void);
    virtual SMStateBase  *PSButton3(void);
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase  *stateAfterPasswordVerify(void);
    virtual bool PSRemoteLegalForState();
    virtual void UnInitialize(void);
    virtual bool storeMode();
    
protected:

    virtual void showScreen(bool passwordVerified);
    virtual SMStateBase *DMRemoteScannerConnect(bool bConnect); 
    virtual void showRemoteScreen(void);
    virtual void showLocalScreen(void);

    DECLARE_DYNCREATE(SMRemoteScannerConnect)// MFC Runtime class/object information
};

#endif
