// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/VariableHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)VariableHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/VariableHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 6     5/31/05 11:40a Dm185016
 * String variable type support
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
// 4     1/27/04 1:52p Dm185016
// DLL Support
// 
// 3     1/16/04 9:28a Dm185016
// Namespace support
*/
#include "StdAfx.h"
#include "variablehandler.h"
#include "SMObjectRepository.h"
#include "SMLoader.h"
#include "SMConstants.h"

// Do not care about unreferenced formal parameters
#pragma warning( disable : 4100 )

using namespace STATE_MACHINE_NAMESPACE;

CVariableHandler * CVariableHandler::m_pInstance = NULL;

CVariableHandler::CVariableHandler(void) 
{
}

CVariableHandler::~CVariableHandler(void)
{
}


void CVariableHandler::registerHandlers(void)
{    
    SETUP_DECLARED_HANDLER( XP_VAR_NAME,            VarName );
    SETUP_DECLARED_HANDLER( XP_VAR_COMMENT,         Comment );
    SETUP_DECLARED_HANDLER( XP_VAR_INITIAL_VALUE,   InitialValue );
    SETUP_DECLARED_HANDLER( XP_VAR_MAX_VALUE,       MaxValue );
    SETUP_DECLARED_HANDLER( XP_VAR_MIN_VALUE,       MinValue );
}


DEFINE_START_ELEMENT(CVariableHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = dynamic_cast<CVariableImpl*>(repository.CreateNextVariable());
    _ASSERTE(m_pContent!=NULL);
    if (m_pContent == NULL)
        return E_FAIL;

    m_pContent->SetTraceObject(GetTraceHandle());

    // Grab the value for the type attribute and save it
    SAVE_ATTRIBUTE_HANDLER(CVariableHandler, Type, XP_VARIABLE_TYPE_ATTR);
    SAVE_ATTRIBUTE_HANDLER(CVariableHandler, VarName, XP_VARIABLE_NAME_ATTR);

    return S_OK;
}
DEFINE_END_ELEMENT(CVariableHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pOR = GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    return pOR->Add(&variable());
}

DEFINE_HANDLER(CVariableHandler, VarName, variable);

DEFINE_HANDLER(CVariableHandler, Comment, variable);

DEFINE_HANDLER(CVariableHandler, InitialValue, variable);

DEFINE_HANDLER(CVariableHandler, MaxValue, variable);

DEFINE_HANDLER(CVariableHandler, MinValue, variable);


void CVariableHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
{
/*
    CXMLBuilder &builder = *((pTheBuilder == NULL) ? this : pTheBuilder);

    CVariableImpl &var = variable();

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
