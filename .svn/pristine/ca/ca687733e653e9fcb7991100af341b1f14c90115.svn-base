#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append1To9.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)Append1To9.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/Append1To9.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 8     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 5     4/18/05 7:15p Dm185016
 * Keyboard support
 * 
 * 3     3/16/05 11:01a Dm185016
 * TAR 295691 - Transaction variable name now obtained from the XML
 * 
 * 2     2/28/05 4:06p Dm185016
 * lint
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 1     11/16/04 11:23a Dm185016
*/

/*lint -e1768 */

#include "SMConstants.h"
#include "Createable.h"
#include "RCMKVActionBase.h"
#include "PSXAction.h"
#include "ActionBaseRAP.h"
#include "KeyValueHandler.h"
#include "Pool.h"

const int WPARAM_VALUE_1	= 97;
const int WPARAM_VALUE_2	= 98;
const int WPARAM_VALUE_3	= 99;
const int WPARAM_VALUE_4	= 100;
const int WPARAM_VALUE_5	= 101;
const int WPARAM_VALUE_6	= 102;
const int WPARAM_VALUE_7	= 103;
const int WPARAM_VALUE_8	= 104;
const int WPARAM_VALUE_9	= 105;

const int WPARAM_VALUE_1_2	= 49;
const int WPARAM_VALUE_2_2	= 50;
const int WPARAM_VALUE_3_2	= 51;
const int WPARAM_VALUE_4_2	= 52;
const int WPARAM_VALUE_5_2	= 53;
const int WPARAM_VALUE_6_2	= 54;
const int WPARAM_VALUE_7_2	= 55;
const int WPARAM_VALUE_8_2	= 56;
const int WPARAM_VALUE_9_2	= 57;

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CAppend1To9
    //: public CRCMKVActionBase<CAppend1To9>
    : public CKeyValueHandler<CAppend1To9>
    , public CActionBaseRAP
    , public CPSXAction
    , public CCreateable<CAppend1To9, IAction>
//    , public CPool<CAppend1To9>
{

public:

    CAppend1To9();
	virtual ~CAppend1To9();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(A19Parms)
        DECLARE_KV_CSTRING(TransactionVariableName);
    DECLARE_KV_HANDLER_CLASS_END

    A19Parms m_parms;


// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_MAP();

    DECLARE_KV_HANDLER(OnWParam);

private:

    // hide copy and assignment
	CAppend1To9(const CAppend1To9&);
	CAppend1To9& operator = (const CAppend1To9 &);

    StringType m_szCharacter;
};
