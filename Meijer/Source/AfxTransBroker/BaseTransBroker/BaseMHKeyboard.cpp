// BaseMHKeyboard.cpp: implementation of the CBaseMHKeyboard class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "BaseMHKeyboard.h"
#include "TraceSystem.h"
#include "basecurrentkeyboard.h"
#include "BaseObjectManager.h"


#define T_ID  _T("CBaseMHKeyboard")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseMHKeyboard::CBaseMHKeyboard()
{
    
}

CBaseMHKeyboard::~CBaseMHKeyboard()
{
    
}

/*************************************************************************
* KeyPress - Send a key code. This is very useful in supporting the full 
*       Store Mode POS keyboard that mimics the host application’s 
*       functionality. This function is mainly used in the Assist Mode.
*       Base TB does not implement this. It must be done at a customer level.
*
* Parameters: 
*  USHORT uKey - ASCII key value
*
* Returns: TBRC - acknowledge the request
*************************************************************************/
TBRC CBaseMHKeyboard::KeyPress(const USHORT uKey)
{
//    tbtrace( TM_ENTRY, _T("+-KeyPress(%d)- Empty Base Functionality"), uKey);
    return TB_SUCCESS;
}

/*************************************************************************
* ProcessMessage - Process the message posted by TB. TB analyzes the message 
*       and returns the state of the host application, based on the presentation 
*       data in the message. Dynamic cast to a KeyboardMsg, and calls Refresh
*       to get data into the CurrentKeyboard class.
*
* Parameters: 
*  PSMessageElement psMessage - Pointer to the TB message
*
* Returns: TBSTATE - the current state of the host application
*************************************************************************/
TBSTATE CBaseMHKeyboard::ProcessMessage(CTBMsgObj *pMsgObj)
{
    tbtraceIO(_T("ProcessMessage"));
    TBSTATE rc = TB_IGNORE;
    CKeyboardMsg *pMsg= dynamic_cast<CKeyboardMsg *> (pMsgObj);
    ASSERT(pMsg);
    if (pMsg) {
        CBaseCurrentKeyboard* pCurr = CCustomerCurrentKeyboard::instance();
        pCurr->Refresh(pMsg);
    }
    return rc;
}

