#pragma once

#include "SMConstants.h"
#include "Createable.h"
#include "PSXAction.h"
#include "KeyValueHandler.h"
#include "Pool.h"

#include "RCMKVActionBase.h"
#include "ActionBaseRAP.h"
#include "RemoteAPMgr.h"


namespace STATE_MACHINE_NAMESPACE 
{
    class IMessage;
}
using namespace STATE_MACHINE_NAMESPACE;

class CSetText
    : public CPSXAction
    , public CCreateable<CSetText, IAction>
{

public:

    CSetText();
	virtual ~CSetText();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(STParms)
        DECLARE_KV_CSTRING(ControlName)
		DECLARE_KV_CSTRING(ContextName)
		DECLARE_KV_CSTRING(StringId)
    DECLARE_KV_HANDLER_CLASS_END

    STParms m_parms;

private:

    // hide copy and assignment
	CSetText(const CSetText&);
	CSetText& operator = (const CSetText &);
};

