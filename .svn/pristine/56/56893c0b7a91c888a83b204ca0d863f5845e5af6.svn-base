// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LookupTableHandler.cpp 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)LookupTableHandler.cpp    $Revision: 1 $ $Date: 6/20/08 1:12p $
 *
 * Copyright 2005 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/handlers/LookupTableHandler.cpp $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 4     3/11/05 1:16p Dm185016
 * Fixed compile problem
 * 
 * 3     3/03/05 3:18p Dm185016
 * TAR 291700
 * 
 * 2     2/28/05 3:44p Dm185016
 * lint
 * 
 * 1     2/14/05 7:05p Dm185016
 * Release Exclusive Access Event support
*/
#include "StdAfx.h"
#include "strutils.h"
#include "LookupTableHandler.h"
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


CLookupTableHandler::CLookupTableHandler(void) 
{
}


CLookupTableHandler::~CLookupTableHandler(void)
{
}


void CLookupTableHandler::registerHandlers(void)
{
    IMODTRACE_DEVELOP_EX(eDCSMEntryExit, eDCSMEntryExit);
    SETUP_DECLARED_HANDLER( XP_LOOKUP_TABLE_LOOKUP_ENTRY, Element );
}



DEFINE_START_ELEMENT(CLookupTableHandler) 
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository &repository = *GetLoader()->GetObjectRepository();

    m_pContent = repository.CreateNextLookupTable();
    m_pContent->SetTraceObject(GetTraceHandle());

    CLookupTableHandler::content_type pTable = dynamic_cast<CLookupTableHandler::content_type>(CLookupTableHandler::instance()->m_pContent);

    //SAVE_ATTRIBUTE_HANDLER(CLookupTableHandler, Name, XP_STATE_TABLE_ATTR_STATE_TABLE_NAME);
    StringType attrval = GET_ATTRIBUTE(XP_LOOKUP_TABLE_ATTR_LOOKUP_TABLE_NAME);
    if (attrval.size() > 0)
    {
        pTable->Name.Convert(attrval);
    }

    //SAVE_ATTRIBUTE_HANDLER(CLookupTableHandler, Name, XP_STATE_TABLE_ATTR_STATE_TABLE_NAME);
    StringType keytype = GET_ATTRIBUTE(XP_LOOKUP_TABLE_ATTR_KEY_TYPE);
    if (keytype.size() > 0)
    {
        pTable->KeyType.Convert(keytype);
    }

    //SAVE_ATTRIBUTE_HANDLER(CLookupTableHandler, Name, XP_STATE_TABLE_ATTR_STATE_TABLE_NAME);
    StringType valuetype = GET_ATTRIBUTE(XP_LOOKUP_TABLE_ATTR_VALUE_TYPE);
    if (attrval.size() > 0)
    {
        if (valuetype == XP_LOOKUP_TABLE_ATTR_VALUE_TYPE_STRING)
            pTable->ValueType.Value(CLookupTableImpl::TYPE_STRING);
        else if (valuetype == XP_LOOKUP_TABLE_ATTR_VALUE_TYPE_INT)
            pTable->ValueType.Value(CLookupTableImpl::TYPE_INTEGER);
    }

    return S_OK;
}
DEFINE_END_ELEMENT(CLookupTableHandler)
{
    _ASSERTE(GetLoader() != NULL);

    ISMObjectRepository *pOR = GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    return pOR->Add((CLookupTableHandler::content_type)m_pContent);
}


DEFINE_HANDLER_START_ELEMENT(CLookupTableHandler, Element)
{
    HRESULT hr = S_OK;

    _ASSERTE(CLookupTableHandler::instance()->GetLoader() != NULL);

    ISMObjectRepository *pOR = CLookupTableHandler::instance()->GetLoader()->GetObjectRepository();

    _ASSERTE(pOR != NULL);

    // Grab the value for the key attribute and save it in the table
    SAVE_ATTRIBUTE_HANDLER(CLookupTableHandler, Key, XP_LOOKUP_TABLE_ATTR_ENTRY_KEY);

    CLookupTableHandler::content_type pTable = 
        (CLookupTableHandler::content_type)CLookupTableHandler::instance()->m_pContent;

    // Grab the value for the key attribute and save it in the table
    switch (pTable->ValueType.Value())
    {
    case CLookupTableImpl::TYPE_INTEGER :
            {
                StringType attrval = GET_ATTRIBUTE(XP_LOOKUP_TABLE_ATTR_ENTRY_VALUE);
                if (attrval.size() > 0)
                {
                    if (_istdigit(attrval[0]))
                    {
                        pTable->Value.Convert(attrval);
                    }

                    // Look for a matching constant
                    else
                    {
                        IConstant *pConstant;
                        hr = pOR->GetConstant(&pConstant, attrval.c_str());
                        if (hr == S_OK)
                        {
                            IString sValue(pConstant->GetValue());
                            pTable->Value.Convert(StringType((const _TCHAR *)sValue));
                        }
                    }
                }
            }
            break;
        case CLookupTableImpl::TYPE_STRING :
            SAVE_ATTRIBUTE_HANDLER(CLookupTableHandler, Value, XP_LOOKUP_TABLE_ATTR_ENTRY_VALUE);
            break;

        default:
            hr = STATEM_INVALID_LOOKUPTABLE_TYPE;
    }

    // Add another entry to the state table for this entry set.
    ((CLookupTableHandler::content_type)CLookupTableHandler::instance()->m_pContent)->AddEntry( );

    return hr;
}


void CLookupTableHandler::GetXMLOutput(CXMLContent *, CXMLBuilder *)
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
