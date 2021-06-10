#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ElseImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)ElseImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/ElseImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     3/17/05 2:46p Dm185016
 * <if>, <else-if>, <else> support
*/
#include "SMConstants.h"
#include "XMLContent.h"
#include "RuleImpl.h"

namespace STATE_MACHINE_NAMESPACE
{

class CElseImpl
    : public CRuleImplBase
    , public CPoolObject<CElseImpl>
{
public:

    CElseImpl();
    virtual ~CElseImpl(void);

    virtual int GetId() const;

    //virtual const _TCHAR *GetComment();

    DECLARE_STRING_PROPERTY(ExpressionValue);

    virtual StringType ToString();

    virtual _RuleTypes  GetRuleType() const;

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CElseImpl(CElseImpl &);                     // Hide the Copy construcor
    CElseImpl &operator =(const CElseImpl &);   // Hide the assignment operator

	static int counter;

    int nId;
};

}