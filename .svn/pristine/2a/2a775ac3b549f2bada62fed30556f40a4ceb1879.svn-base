#pragma once
// $Header: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInterventionBase.h 3     10/21/08 4:07p Sm133076 $
/*
 * @(#)DisplayInterventionBase.h    $Revision: 3 $ $Date: 10/21/08 4:07p $
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
* $Log: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/remote console manager/src/rap/actions/DisplayInterventionBase.h $
 * 
 * 3     10/21/08 4:07p Sm133076
 * Patch G Migration
 * 
 * 6     10/20/06 9:50a Tn185011
 * Roll back to 244
 * 
 * 4     1/13/06 2:29p Dm185016
 * TAR 301329
 * 
 * 1     1/26/05 2:19p Dm185016
 * Moved to new Repository.
* 
* 2     1/12/05 5:50p Dm185016
* TAR 289897
* 
* 1     12/22/04 10:34a Dm185016
* TAR 288934 - New base class for updating buttons for intervention
* display.
*/

#include "SMConstants.h"
#include "Createable.h"
#include "RemoteAPMgr.h"
#include "ActionBaseRAP.h"
#include "DisplayControlParser.h"
#include "Pool.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;


class CDisplayInterventionBase
    : public CActionBaseRAP
    , public CDisplayControlParser
{

public:

    CDisplayInterventionBase() {};
    virtual ~CDisplayInterventionBase() {};
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseInterventionData(const _TCHAR *);

    virtual CIntervention *GetIntervention()=0;

    virtual bool OverrideDisplay()=0;
    virtual bool IsAddDescriptionToReceipt()=0;

    const _TCHAR *GetDescription() const;

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(DIBParms)
        DECLARE_KV_CSTRING(Description);
    DECLARE_KV_HANDLER_CLASS_END

    DIBParms m_parms;

private:

    // hide copy and assignment
	CDisplayInterventionBase(const CDisplayInterventionBase&);
	CDisplayInterventionBase& operator = (const CDisplayInterventionBase &);
};
