#pragma once

#include "rcmapi.h"
#include "SMConstants.h"
#include "Createable.h"
#include "Evaluateable.h"
#include "KeyValueHandler.h"
#include "RCMActionBase.h"
#include "ActionBaseRAP.h"
#include "Pool.h"
#include "Message.h"

using namespace STATE_MACHINE_NAMESPACE;
using namespace rap;

namespace fastlane
{

class RCMANAGER_API CCompareToCurrentContext
    : virtual public CRCMActionBase
    , public CActionBaseRAP
    , public IEvaluateable
    , public CCreateable<CCompareToCurrentContext, IAction>
//    , public CPool<CGetCurrentContext>
{

public:
	CCompareToCurrentContext();
	virtual ~CCompareToCurrentContext();
    
    virtual HRESULT Execute(bool *, IMessage &);

    //-------------------------------------------------------------------------
    // Methods from IEvaluateable
    //-------------------------------------------------------------------------
    virtual long GetValue() const { return 0; }

    virtual void SetValue(long){}

protected:

    HRESULT ParseOperands();

// This section contains the handlers for the various K,V pairs
private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(SLCCParms)
        DECLARE_KV_CSTRING(Context);
    DECLARE_KV_HANDLER_CLASS_END

private:

    // hide copy and assignment
	CCompareToCurrentContext(const CCompareToCurrentContext&);
	CCompareToCurrentContext& operator = (const CCompareToCurrentContext &);

    SLCCParms m_parms;
};

}
