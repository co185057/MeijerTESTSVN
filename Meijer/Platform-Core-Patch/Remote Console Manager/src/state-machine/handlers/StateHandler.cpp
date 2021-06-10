// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)TimerHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 3     2/28/05 3:44p Dm185016
 * lint
 * 
 * 2     2/10/05 5:58p Dm185016
 * Fixed problem with static ids used in SM.
 * 
 * 1     1/26/05 2:30p Dm185016
 * Moved to new Repository.
// 
// 6     10/04/04 6:49p Dm185016
// Trace object is no longer shared.
// 
// 4     4/30/04 1:01p Dm185016
// Removed unused tags
// 
// 3     1/27/04 1:52p Dm185016
// DLL Support
// 
// 2     1/16/04 9:25a Dm185016
// Namespace support, Removed <state>
*/
#include "StdAfx.h"
#include "Statehandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CStateHandler::CStateHandler(void) 
{
}

CStateHandler::~CStateHandler(void)
{
}


void CStateHandler::registerHandlers(void)
{
    SETUP_DECLARED_HANDLER( XP_STATE_NAME,				Name );
    SETUP_DECLARED_HANDLER( XP_STATE_COMMENT,			Comment );
    SETUP_DECLARED_HANDLER( XP_STATE_INITIAL,			Initial );
}


DEFINE_START_ELEMENT(CStateHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = dynamic_cast<CStateImpl*>(repository.CreateNextState());
    _ASSERTE(m_pContent!=NULL);
    if (m_pContent == NULL)
        return E_FAIL;

    m_pContent->SetTraceObject(GetTraceHandle());

    return S_OK;
}
DEFINE_END_ELEMENT(CStateHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pOR = GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    return pOR->Add(&state());
}

DEFINE_HANDLER(CStateHandler, Name, state);

DEFINE_HANDLER(CStateHandler, Comment, state);


DEFINE_HANDLER(CStateHandler, Initial, state);
DEFINE_HANDLER_START_ELEMENT(CStateHandler, Initial)
{
    CStateHandler::instance()->state().Initial.Value(true);

    return S_OK;
}
void CStateHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    //CStateImpl &state = *(CStateImpl *)pContent;

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
