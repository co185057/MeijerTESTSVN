// AMessageSink.h: interface for the AMessageSink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMESSAGESINK_H__F613752B_CEA2_4074_A119_2EE8B95ADB09__INCLUDED_)
#define AFX_AMESSAGESINK_H__F613752B_CEA2_4074_A119_2EE8B95ADB09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"
//#include "ScannerMsg.h"
//#include "CMOSMsg.h"
//#include "PinPadMsg.h"
//#include "PrinterMsg.h"
//#include "2X20Msg.h"

class AMessageSink  
{
public:
  AMessageSink(){};
  virtual ~AMessageSink(){};

	virtual void ReceiveMsg(CTBMsgObj * pBaseMsg) = 0;

};

#endif // !defined(AFX_AMESSAGESINK_H__F613752B_CEA2_4074_A119_2EE8B95ADB09__INCLUDED_)
