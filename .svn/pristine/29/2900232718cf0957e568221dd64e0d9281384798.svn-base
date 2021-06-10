#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ActionBaseRAP.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)RCMActionBase.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/ActionBaseRAP.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 5     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     10/29/04 8:53a Dm185016
*/

namespace rap
{

class CRemoteAPMgr;

class CActionBaseRAP
{

public:

    CActionBaseRAP(){};
    virtual ~CActionBaseRAP(){};

public:

    static CRemoteAPMgr &rapmgr();

private:

    // hide copy and assignment
	CActionBaseRAP(const CActionBaseRAP&);
	CActionBaseRAP& operator = (const CActionBaseRAP &);
};

}