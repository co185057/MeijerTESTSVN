/**********************************************************************************/
//	BaseTBScotMsg.cpp   :   implementation  file for CBaseTBScotMsg  class            
//                                                                                
//                                                                                
// $Header: 
// $Revision:  
//                                                                                
//	CHANGES: Start                                                                
//	CHANGES: End                                                                  
//                                                                                
/***********************************************************************************/


#include "stdafx.h"
#include <afxmt.h>                  // Multithreading header
#include "basetrxBroker.h"
#include "ScotMsg.h"
#include "TBDefs.h"
#include "BaseTBScotMsg.h"
#include "TraceSystem.h"
#include "customertrxbroker.h"

#define T_ID    _T("CBaseTBScotMsg")

static  CCriticalSection ScotMsgCriticSec;

// initialize message properties, 
CBaseTBScotMsg::CBaseTBScotMsg()
{
    MsgType   = SCOT_MESSAGE;
    MsgSource = ID_TRANSBROKER;
    MsgWParam = 0;
    hScotInputWnd = NULL;
}

CBaseTBScotMsg::~CBaseTBScotMsg()
{
}

long CBaseTBScotMsg::SendScotMsg(TBDEVICEID nDevice, UINT nMsgLen, void* pMsgData, LPARAM lParam)
{
    // this message structure will be deleted by TB's ReleaseMessage 
    PSMessageElement pScotMsg = new SMessageElement;
    
    TCHAR* pNewMsgData = NULL;
    if ((nMsgLen) && (pMsgData))
    {
        pNewMsgData = new TCHAR[nMsgLen+1];
        pNewMsgData[ nMsgLen] = NULL;
        _tcsncpy(pNewMsgData, (TCHAR *)pMsgData, nMsgLen); 
    }
    
    pScotMsg->id        = MsgSource;
    pScotMsg->nDeviceID = nDevice;
    pScotMsg->nLength   = nMsgLen;
    pScotMsg->pData     = pNewMsgData;
    pScotMsg->lParam    = lParam;  
    
    long retVal = PostScotMsg(pScotMsg);
    return retVal;
}

long CBaseTBScotMsg::SendScotMsg(CTBMsgObj *pMsgObj)
{
    return SendScotMsg(pMsgObj->GetDeviceId(), 0, NULL, (LPARAM) pMsgObj);
}


long CBaseTBScotMsg::PostScotMsg(const PSMessageElement TB2ScotMsg)
{
    tbtrace( TM_ENTRY, _T("+PostScotMsg"));
    if (!hScotInputWnd) 
    {
        tbtrace(TM_ERROR, _T("Invalid SCOT window handle posting message"));
        tbtrace(TM_ERROR, _T("PostScotMsg Exit - return TB_E_INVALIDWINDOW"));
        return TB_E_INVALIDWINDOW;
    }
    
    CSingleLock sl( &ScotMsgCriticSec, false );
    sl.Lock();                        //enter critical section
    PostMessage(hScotInputWnd,SCOT_MESSAGE,0,(LPARAM) TB2ScotMsg);
    sl.Unlock();                      //exit critical section
    
    return TB_SUCCESS;
}


bool CBaseTBScotMsg::SetScotWndHandle(HWND hScotWindow)
{
    if (!hScotWindow)
        return false;
    
    hScotInputWnd = hScotWindow;
    return true;
}

HWND CBaseTBScotMsg::GetScotWndHandle()
{
    return hScotInputWnd;
}


long CBaseTBScotMsg::SendSCOTShutdownMsg(WPARAM wParam, LPARAM lParam)
{
    if (!hScotInputWnd) return TB_E_INVALIDWINDOW;
    
    CSingleLock sl( &ScotMsgCriticSec, false );
    sl.Lock();                        //enter critical section
    PostMessage(hScotInputWnd, WMU_SCOTWM_CLOSE, wParam, lParam);
    sl.Unlock();                      //exit critical section
    
    return TB_SUCCESS;
}


// Send a message to force a return status code
long CBaseTBScotMsg::SendReturnStateMsg( long lRetStat, RetStateMsgHandlerType i_pfn, const TCHAR *szMessage)
{
    tbtrace(TM_INFO, _T("SendReturnStateMsg %s"), CCustomerTrxBroker::instance()->GetTBSTATEName(lRetStat));
    CReturnStateMsg *pMsg = new CReturnStateMsg(lRetStat); // delete done in ReleaseMessageLParam()
    if (szMessage)
        pMsg->SetMessage(szMessage);
    pMsg->SetHandler(i_pfn);
    return SendScotMsg(pMsg);
}

// Send a message to display on RAP as "POS Message: " on the e-receipt
long CBaseTBScotMsg::SendRAPDisplayMsg( CString csRAPMsg )
{
    return SendReturnStateMsg( TB_SENDMESSAGETOSCOT, NULL, (LPCTSTR) csRAPMsg );
}
