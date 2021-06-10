#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleKey.h 1     6/20/08 1:13p Sa250050 $
/*
 * @(#)RuleKey.h    $Revision: 1 $ $Date: 6/20/08 1:13p $
 *
 * Copyright 2003 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Patch/Remote Console Manager/src/state-machine/RuleKey.h $
 * 
 * 1     6/20/08 1:13p Sa250050
 * RFQ 1730 - DG34 - FastLane 4.0 E5.3 Base start
 * 
 * 1     1/26/05 2:27p Dm185016
 * Moved to new Repository.
* 
* 6     1/18/05 3:55p Dm185016
* UNICODE support
* 
* 4     8/17/04 6:23p Dm185016
* 
* 3     1/27/04 1:56p Dm185016
* DLL Support
* 
* 2     1/16/04 9:06a Dm185016
* Namespace support
* 
* 1     1/06/04 8:58a Dm185016
* First version
*/

#include "SMConstants.h"
#include "statem.h"

namespace STATE_MACHINE_NAMESPACE
{

typedef _TCHAR RuleKeyType[10];

class STATEM_API CRuleKey
{
public:
    CRuleKey(int _nMsgID, int _nStateID);
    CRuleKey();
    ~CRuleKey();
    
    void SetMsgID(int _msgID);
    void SetStateID(int _stateID);

    operator const _TCHAR * ( ) const;

private:
    CRuleKey(const CRuleKey &);               // Hide the Copy construcor
    CRuleKey &operator =(const CRuleKey &);   // Hide the assignment operator

    void UpdateKey();

    int m_nMsgID;
    int m_nStateID;

    RuleKeyType m_Key;
};

}