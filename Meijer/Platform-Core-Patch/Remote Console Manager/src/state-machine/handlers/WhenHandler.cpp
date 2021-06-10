// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/WhenHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)WhenHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2003 by NCR Corporation,
 *
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of NCR Corporation. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with NCR.
 */
/*
* $Log4
*/
#include "StdAfx.h"
#include "WhenHandler.h"
#include "SelectHandler.h"
#include "XMLStateMachine.h"
#include "SMLoader.h"
#include "SMConstants.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CWhenHandler::CWhenHandler(void) 
{
}

CWhenHandler::~CWhenHandler(void)
{
}


void CWhenHandler::SetOwner(CSelectHandler *pOwner) { m_pOwner = pOwner; }


void CWhenHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    //SETUP_DECLARED_HANDLER( XP_MSG_NAME,				Name );
    //SETUP_DECLARED_HANDLER( XP_MSG_COMMENT,				Comment );
    //SETUP_DECLARED_HANDLER( XP_MSG_VALUE,				Value );
}



//DEFINE_START_ELEMENT_BASIC(CWhenHandler, MsgImpl);
DEFINE_START_ELEMENT(CWhenHandler) 
{
    IMODTRACE_DEVELOP();

    CWhenImpl *pWhen = newpooledobject CWhenImpl;
    m_pContent = pWhen;
    m_pContent->SetTraceObject(GetTraceHandle());

    StringType sAttr = GET_ATTRIBUTE(XP_SELECT_ACTION_ATTR_EXPRESSION);
    ITRACE_DEVELOP(_T("attr:  ") + IString(sAttr.c_str()));
    SAVE_ATTRIBUTE(CWhenHandler, ExpressionValue, XP_SELECT_ACTION_ATTR_EXPRESSION, when);
    ITRACE_DEVELOP(_T("attribute value:  ") + IString(CWhenHandler::instance()->when().ExpressionValue.Value().c_str()));

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();
    _ASSERTE(pSM != NULL);

    IExpression *pExpression;
    HRESULT hr = pSM->GetExpression(&pExpression
                                  , CWhenHandler::instance()->
                                        when().ExpressionValue.Value().c_str());
    if (hr != S_OK)
        return hr;

    pWhen->SetExpression(pExpression);

    m_pOwner->AddWhenClause(pWhen);

    return S_OK;
}


DEFINE_END_ELEMENT(CWhenHandler)
{
    IMODTRACE_DEVELOP();

    _ASSERTE(GetLoader() != NULL);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();

    _ASSERTE(pSM != NULL);

    //return pSM->Add(&msg());
    return S_OK;
}


void CWhenHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
/*
    CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    CMsgImpl &state = *(CMsgImpl *)pContent;

    builder.StartElement(_L(REFUND_IN));
    
    builder.AddAttribute(ATTR_ID, refundIn.Reason__Id);
    builder.AddElement(_L(REASON), refundIn.Reason);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperNum__Id);
    builder.AddElement(_L(SUPER_NUM), refundIn.SuperNum);    
    builder.AddAttribute(ATTR_ID, refundIn.SuperPass__Id);
    builder.AddElement(_L(SUPER_PASS), refundIn.SuperPass);    
    builder.AddAttribute(ATTR_ID, refundIn.Price__Id);
    builder.AddElement(_L(PRICE), refundIn.Price);    

    builder.EndElement(_L(REFUND_IN));
*/
}
