// ReturnStateMsg.h: interface for the CReturnStateMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RETURNSTATEMSG_H__D389CFF6_6AB9_4336_AA5D_C3BAE630C4CA__INCLUDED_)
#define AFX_RETURNSTATEMSG_H__D389CFF6_6AB9_4336_AA5D_C3BAE630C4CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"
class CBaseTrxBroker;


#include "transbroker.h"

typedef    bool (CBaseTrxBroker::*RetStateMsgHandlerType)(PSMessageElement psMessage, long /*  we don't want to go crazy here to get the TBRC definition, so just use "long"*/ 
                                                          &retval, LPARAM lparam);

class CReturnStateMsg : public CTBMsgObj  
{
public:
   CReturnStateMsg(long flReturnState= TB_IGNORE);
	virtual ~CReturnStateMsg();

public:
   void SetRetState(long lRetState);
   void SetHandler(RetStateMsgHandlerType pfn)
   {
      m_pfnHandler= pfn;
   }
   long GetRetState();
   void SetMessage(const CString &csMessage);
   CString GetMessage();
   TCHAR *MessageToBuffer();
   RetStateMsgHandlerType m_pfnHandler;
private:
   long m_lRetState;
   CString m_csMessage;
   BYTE* Serialize(int &byteLength){return NULL;};
   void Parse(BYTE *pBuf){};
   int getLength(void){ return 0;};
	//BYTE* GetAndForgetExtensionData();


protected:

};

#endif // !defined(AFX_RETURNSTATEMSG_H__D389CFF6_6AB9_4336_AA5D_C3BAE630C4CA__INCLUDED_)
