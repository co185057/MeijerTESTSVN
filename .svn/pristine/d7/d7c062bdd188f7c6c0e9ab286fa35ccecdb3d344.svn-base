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

	
  DECLARE_DYNCREATE(SMRemoteScannerConnect)// MFC Runtime class/object information
};

#endif
