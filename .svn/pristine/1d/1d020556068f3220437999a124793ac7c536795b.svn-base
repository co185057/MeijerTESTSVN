// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/OtherwiseHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)OtherwiseHandler.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2004 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/OtherwiseHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 5     4/21/05 7:27p Dm185016
 * Cleaned up tracing
 * 
 * 3     2/28/05 3:44p Dm185016
 * lint
 * 
 * 2     2/21/05 8:05p Dm185016
 * Added debugging information to new and delete for pooled objects.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 1     12/22/04 11:25a Dm185016
// Conditional logic support.
*/
#include "StdAfx.h"
#include "OtherwiseHandler.h"
#include "XMLStateMachine.h"
#include "SMLoader.h"
#include "SMConstants.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

COtherwiseHandler::COtherwiseHandler(void) 
{
}

COtherwiseHandler::~COtherwiseHandler(void)
{
}


void COtherwiseHandler::SetOwner(CSelectHandler *pOwner) { m_pOwner = pOwner; }


void COtherwiseHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    //SETUP_DECLARED_HANDLER( XP_MSG_NAME,				Name );
    //SETUP_DECLARED_HANDLER( XP_MSG_COMMENT,				Comment );
    //SETUP_DECLARED_HANDLER( XP_MSG_VALUE,				Value );
}



//DEFINE_START_ELEMENT_BASIC(COtherwiseHandler, MsgImpl);
DEFINE_START_ELEMENT(COtherwiseHandler) 
{
    IMODTRACE_DEVELOP();

    m_pContent = newpooledobject CWhenImpl;
    m_pContent->SetTraceObject(GetTraceHandle());

    //TODO:  Need to ensure that the conditional used for this WhenImpl always returns true.

    return S_OK;
}


DEFINE_END_ELEMENT(COtherwiseHandler)
{
    IMODTRACE_DEVELOP();

    _ASSERTE(GetLoader() != NULL);

    CXMLStateMachine *pSM = GetLoader()->GetStateMachine();

    _ASSERTE(pSM != NULL);

    //return pSM->Add(&msg());
    return S_OK;
}


void COtherwiseHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CMsgImpl &state = *(CMsgImpl *)pContent;

/*
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
