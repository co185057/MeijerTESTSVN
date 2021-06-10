#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/WhenImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SMConstants.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/WhenImpl.h $
 * 
 * 1     6/20/08 1:12p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 2     3/11/05 1:20p Dm185016
 * <select> support
 * 
 * 1     1/26/05 2:29p Dm185016
 * Moved to new Repository.
* 
* 3     1/12/05 6:32p Dm185016
* Select support.
* 
* 2     12/22/04 11:24a Dm185016
* Made a base class of RuleImplBase, same as RuleImpl.
* 
* 1     10/29/04 9:28a Dm185016
*/
#include "SMConstants.h"
#include "XMLContent.h"
#include "RuleImpl.h"

namespace STATE_MACHINE_NAMESPACE
{

class CWhenImpl
    : public CRuleImplBase
    , public CPoolObject<CWhenImpl>
{
public:

    CWhenImpl();
    virtual ~CWhenImpl(void);

    virtual int GetId() const;

    //virtual const _TCHAR *GetComment();

    DECLARE_STRING_PROPERTY(ExpressionValue);

    virtual StringType ToString();

    virtual _RuleTypes  GetRuleType() const;

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CWhenImpl(CWhenImpl &);                     // Hide the Copy construcor
    CWhenImpl &operator =(const CWhenImpl &);   // Hide the assignment operator

	static int counter;

    int nId;
};

}