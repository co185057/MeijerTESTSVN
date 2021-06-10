#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayControlParser.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayControlParser.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
 *
 * Copyright 2006 by NCR Corporation,
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayControlParser.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 3     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 1     1/13/06 2:26p Dm185016
 * TAR 301329
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RemoteAPMgr.h"
#include "DescriptionParser.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


class CDisplayControlParser
    : public CDescriptionParser
{

public:

    CDisplayControlParser() {};
    virtual ~CDisplayControlParser() {};

protected:

    ///<summary>Obtains all the controls names from the xml file.</summary>
    HRESULT ParseOperands();

    const _TCHAR *GetDescription() const;
    const _TCHAR *GetInstructionTextVariableName() const;
    const _TCHAR *GetLaneButtonTextControlName() const;
    const _TCHAR *GetLaneStatus1ControlName() const;
    const _TCHAR *GetLaneStatus2ControlName() const;
    const _TCHAR *GetLaneStatus3ControlName() const;

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DCPParms)
        DECLARE_KV_CSTRING(DataSource);
        DECLARE_KV_CSTRING(Desc);
        DECLARE_KV_CSTRING(Description);
        DECLARE_KV_CSTRING(InstructionTextVariableName);
        DECLARE_KV_CSTRING(LaneButtonTextControlName);
        DECLARE_KV_CSTRING(LaneStatus1ControlName);
        DECLARE_KV_CSTRING(LaneStatus2ControlName);
        DECLARE_KV_CSTRING(LaneStatus3ControlName);
    DECLARE_KV_HANDLER_CLASS_END

    DCPParms m_parms;

private:

    // hide copy and assignment
	CDisplayControlParser(const CDisplayControlParser&);
	CDisplayControlParser& operator = (const CDisplayControlParser &);
};
