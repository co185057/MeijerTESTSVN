//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateRPBase.cpp
//
// TITLE: Report Parse Functions
//
//
// AUTHOR:Tara Duncan
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV abstract base state
#include "SMStateRPBase.h"

#define COMP_ID ID_RP               // base state component
#define T_ID    _T("RPBase")

CString SMStateBase::m_csCurrentReport = EMPTY_STRING;
CString SMStateBase::m_csRPT           = EMPTY_STRING;

///////////////////////////////////////////////////////////////////////////////
// Function:    RPInitialize                                  
// Class:       SMStateBase::RPInitialize                                              
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long SMStateBase::RPInitialize(void)
{
  long rc = 0;
  trace(L6,_T("+RPInit"));	
  trace(L6,_T("-RPInit %d"),rc);
  return rc;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    OnRPReceived                                  
// Class:       SMStateBase::OnRPReceived                                              
// Description: 
// Parameters:  long byteCount        
//              
//              
//              
//              
// Returns:      STATE_NULL                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRPReceived(long byteCount)
{
  trace(L6,_T("+OnRPReceived"));	
  trace(L6,_T("-OnRPReceived %d"),byteCount);
 return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    OnRPDisconnect                                  
// Class:       SMStateBase::OnRPDisconnect                                              
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     STATE_NULL                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnRPDisconnect()
{
	 return STATE_NULL;

}
///////////////////////////////////////////////////////////////////////////////
// Function:    SendRPDisconnect                                  
// Class:       MStateBase::SendRPDisconnect                                              
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     STATE_NULL                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::SendRPDisconnect()
{
	 return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    SendRPConnect                                  
// Class:       SMStateBase::SendRPConnect                                             
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     STATE_NULL                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::SendRPConnect()
{
	 return STATE_NULL;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    RPParse                                  
// Class:       SMStateBase::SendRPConnect                                             
// Description: Processes OnRPReceived, and sends number of bytes received to event handler
// Parameters:  Processes OnRPDisconnected        
//              
//              
//              
//              
// Returns:     retState                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::RPParse(MessageElement *me)
{
  int nEvent = me->rpInfo.Msg;
  CString csStateName;
  SMStateBase *retState = STATE_NULL;

  trace(L7,_T("+RPParse"));
  	switch(nEvent)
	{
		 case RP_RECEIVED:
            retState = OnRPReceived(me->rpInfo.lParam);  // Send number of bytes received to event handler
		    break;
         case RP_DISCONNECTED:
			 retState = OnRPDisconnect();
             break;

    }
	
  trace(L7,_T("-RPParse"));
  return retState;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    RPCallBack_Handler                                  
// Class:       CALLBACK RPCallBack_Handler                                            
// Description: Processes OnRPReceived, and sends number of bytes received to event handler
// Parameters:  Msd ID, wParam, lParam        
//              
//              
//              
//              
// Returns:     BOOL                                         
///////////////////////////////////////////////////////////////////////////////////
// this callback is passed to RP and is called back on the same thread
BOOL CALLBACK RPCallBack_Handler(const long Msg,
								 const WPARAM wParam,
								 const LPARAM lParam)                                                           
{
  int nWhat = (int)wParam;
  long lParm = 0;
  long wParm = 0;
  if(rp.m_bTelnetActive )
  {
	if (RP_DISCONNECTED == Msg)
	{
		// Disconnect
		 mo.PostRP(RP_DISCONNECTED, wParam, lParam);
	}
	else if (RP_RECEIVED == Msg)
	{
		mo.PostRP(RP_RECEIVED, wParam, lParam);
	}
  }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    RPUnInitialize                                  
// Class:       SMStateBase::RPUnInitialize                                           
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long SMStateBase::RPUnInitialize(void)// before SCOT
{
  long rc = 0;
  trace(L6,_T("+RPUnInit"));
  trace(L6,_T("-RPUnInit %d"),rc);
  return rc;
}
//////////////////////////////////////////////////////////////////////
//End of SMStateRPbase