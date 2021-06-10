// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateTableHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)StateTableHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/StateTableHandler.cpp $
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
// 1     11/09/04 3:38p Dm185016
// Support for state lookup tables
*/
#include "StdAfx.h"
#include "StateTableHandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"
#include "SMError.h"
#include "StateLookupTableImpl.h"
#include "LoggableManager.h"
#include "Loggable.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;


CStateTableHandler::CStateTableHandler(void) 
{
}


CStateTableHandler::~CStateTableHandler(void)
{
}


void CStateTableHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    SETUP_DECLARED_HANDLER( XP_STATE_TABLE_STATE, State );
}



DEFINE_START_ELEMENT(CStateTableHandler) 
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = repository.CreateNextStateLookupTable();
    m_pContent->SetTraceObject(GetTraceHandle());

    //SAVE_ATTRIBUTE_HANDLER(CStateTableHandler, Name, XP_STATE_TABLE_ATTR_STATE_TABLE_NAME);
    StringType attrval = GET_ATTRIBUTE(XP_STATE_TABLE_ATTR_STATE_TABLE_NAME);
    if (attrval.size() > 0)
    {
        CStateTableHandler::content_type pTable = (CStateTableHandler::content_type)CStateTableHandler::instance()->m_pContent;
        pTable->Name.Convert(attrval);
    }

    return S_OK;
}
DEFINE_END_ELEMENT(CStateTableHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pOR = GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    return pOR->Add((CStateTableHandler::content_type)m_pContent);
}


DEFINE_HANDLER_START_ELEMENT(CStateTableHandler, State)
{
    _ASSERTE(CStateTableHandler::instance()->GetLoader() != NULL);

    ISMObjectRepository *pOR = CStateTableHandler::instance()->GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    // Grab the value for the key attribute and save it in the table
    SAVE_ATTRIBUTE_HANDLER(CStateTableHandler, Key, XP_STATE_TABLE_ATTR_KEY);

    // Grab the value for the state-name attribute and save it in the table
    StringType attrval = GET_ATTRIBUTE(XP_STATE_TABLE_ATTR_STATE_NAME);
    if (attrval.size() > 0)
    {
        int nStateID = pOR->GetStateID(attrval.c_str());
        CStateTableHandler::content_type pTable = (CStateTableHandler::content_type)CStateTableHandler::instance()->m_pContent;
        pTable->StateName.Convert(attrval);
        pTable->StateId.Value(nStateID);
    }

    // Add another entry to the state table for this entry set.
    ((CStateTableHandler::content_type)CStateTableHandler::instance()->m_pContent)->AddEntry( );

    return S_OK;
}


void CStateTableHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
    //CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

//  CRuleImpl &state = *(CRuleImpl *)pContent;

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
