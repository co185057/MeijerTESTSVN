#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/SelectImpl.h 1     6/20/08 1:12p Sa250050 $
/*
 * @(#)SelectImpl.h    $Revision: 1 $ $Date: 6/20/08 1:12p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/content/SelectImpl.h $
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
* 1     1/07/05 3:45p Dm185016
* Select support
*/
#include "SMConstants.h"
#include "XMLContent.h"
#include "RuleImpl.h"

namespace STATE_MACHINE_NAMESPACE
{

class CSelectImpl
    : public CRuleImplBase
    , public CPoolObject<CSelectImpl>
{
public:

    CSelectImpl();
    virtual ~CSelectImpl(void);

    virtual int GetId() const;

    //virtual const _TCHAR *GetComment();

    virtual StringType ToString();

    virtual _RuleTypes  GetRuleType() const;

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CSelectImpl(CSelectImpl &);                     // Hide the Copy construcor
    CSelectImpl &operator =(const CSelectImpl &);   // Hide the assignment operator

	static int counter;

    int nId;
};

}