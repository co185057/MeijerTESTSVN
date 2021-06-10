// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/TimerHandler.cpp 1     6/20/08 1:12p Sa250050 $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/TimerHandler.cpp $
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
// 4     10/04/04 6:49p Dm185016
// Trace object is no longer shared.
// 
// 2     1/27/04 1:52p Dm185016
// DLL Support
// 
// 1     1/16/04 9:28a Dm185016
// New <timer> support
*/
#include "StdAfx.h"
#include "Timerhandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CTimerHandler::CTimerHandler(void) 
{
}

CTimerHandler::~CTimerHandler(void)
{
}


void CTimerHandler::registerHandlers(void)
{    
    SETUP_DECLARED_HANDLER( XP_TIMER_NAME,            Name );
    SETUP_DECLARED_HANDLER( XP_TIMER_COMMENT,         Comment );
}


DEFINE_START_ELEMENT(CTimerHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = dynamic_cast<CTimerImpl*>(repository.CreateNextTimer());
    _ASSERTE(m_pContent!=NULL);
    if (m_pContent == NULL)
        return E_FAIL;

    m_pContent->SetTraceObject(GetTraceHandle());
    return S_OK;
}
DEFINE_END_ELEMENT(CTimerHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pOR = GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    return pOR->Add(&timer());
}

DEFINE_HANDLER(CTimerHandler, Name, timer);

DEFINE_HANDLER(CTimerHandler, Comment, timer);


void CTimerHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
/*
    CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    CTimerImpl &var = timer();

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
