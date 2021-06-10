#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/Operator.h 3     1/12/10 11:55p Cu185008 $
/*
 * @(#)Operator.h    $Revision: 3 $ $Date: 1/12/10 11:55p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/Remote Console Manager/src/rap/Operator.h $
 * 
 * 3     1/12/10 11:55p Cu185008
 * G2 Migration.
 * 
 * 2     2/28/05 4:05p Dm185016
 * lint
 * 
 * 1     1/26/05 2:17p Dm185016
 * Moved to new Repository.
* 
* 1     11/11/04 10:04a Dm185016
*/

#include "BaseObject.h"
//#include <hash_map>
#include <map>

namespace rap
{

typedef long COperatorKeyType;

class COperator
    : public CBaseObject
{

public:

    COperator(const _TCHAR *pszOpId, const _TCHAR *pszOpPwd);

    virtual ~COperator() {};

    virtual COperatorKeyType GetId();

    const _TCHAR *GetOperatorId() const;
    const _TCHAR *GetOperatorPwd() const;

private:

    COperator();

    COperatorKeyType m_Id;

    StringType m_sOperatorId;
    StringType m_sOperatorPwd;

    static COperatorKeyType m_lCount;
};

}