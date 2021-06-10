#pragma once

#include "SMConstants.h"
#include "Createable.h"
#include "KeyValueHandler.h"
#include "ActionBaseRAP.h"
#include "RCMActionBase.h"
#include "RemoteAPMgr.h"
#include "Evaluateable.h"

namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

class CIsTriliteColorGreen
    : public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CIsTriliteColorGreen, IAction>
{

public:

    CIsTriliteColorGreen();
	virtual ~CIsTriliteColorGreen();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const;

    virtual void SetValue(long);

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(ICTCParms)
        DECLARE_KV_INT(Green)
    DECLARE_KV_HANDLER_CLASS_END

    ICTCParms m_parms;

private:

    // hide copy and assignment
	CIsTriliteColorGreen(const CIsTriliteColorGreen &);
	CIsTriliteColorGreen& operator = (const CIsTriliteColorGreen &);
};
