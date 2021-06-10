//////////////////////////////////////////////////////////////////////////////////////////////////
//
// File:  SMRemoteScannerConnect.cpp
//
// Description: See h file
//
// Author:  Jeff Connelly
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV base state
#include "SMRemoteScannerConnect.h"       // MGV sys msg 3 state

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SystemMessage6")

IMPLEMENT_DYNCREATE(SMRemoteScannerConnect, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

/////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////
SMRemoteScannerConnect::SMRemoteScannerConnect
                (bool bConnect) // DMCheckHealth after msg?
// Construct our parent, most things are handled there
: SMRemoteScannerConnectBase(bConnect)

{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMRemoteScannerConnect::SMRemoteScannerConnect()
{
  IMPLEMENT_TIMESTAMP
}

////////////////////////////////////////
SMStateBase  *SMRemoteScannerConnect::Deliver(bool bConnect)
{
  SMRemoteScannerConnectBase::Deliver(bConnect);


  return this;
}


