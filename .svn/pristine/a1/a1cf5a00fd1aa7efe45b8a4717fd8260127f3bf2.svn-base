// ReturnStateMsg.cpp: implementation of the CReturnStateMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseTrxBroker.h"
#include "ReturnStateMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReturnStateMsg::CReturnStateMsg(long flReturnState /*= TB_IGNORE */)
: CTBMsgObj(TBDEV_RETURN_STATE),
  m_lRetState(flReturnState),
  m_csMessage(""),
  m_pfnHandler( NULL )
{

}

CReturnStateMsg::~CReturnStateMsg()
{

}

void CReturnStateMsg::SetRetState(long lRetState)
{
   m_lRetState= lRetState;
}

long CReturnStateMsg::GetRetState()
{
   return m_lRetState;
}

void CReturnStateMsg::SetMessage(const CString &csMessage)
{
   m_csMessage= csMessage;
}

CString CReturnStateMsg::GetMessage()
{
   return m_csMessage;
}

// this function is used to allocate a buffer and place 
// the text message member in it.  This buffer is then
// passed in the LPARAM of a MessageElement struct to FL
TCHAR *CReturnStateMsg::MessageToBuffer()
{
   int len= m_csMessage.GetLength();
   if (!len)
      return NULL;
   TCHAR *pmsg= new TCHAR[len + 1];
   _tcscpy(pmsg, m_csMessage.GetBuffer(0));
   return pmsg;
}
