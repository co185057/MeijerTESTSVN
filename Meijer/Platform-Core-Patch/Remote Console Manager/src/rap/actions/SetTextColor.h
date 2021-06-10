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

class CSetTextColor
    //: public CRCMKVActionBase<CTransactionBegin>
    : public CPSXAction
    , public CCreateable<CSetTextColor, IAction>
//    , public CPool<CSetTextColor>
{

public:

    CSetTextColor();
	virtual ~CSetTextColor();
    
    virtual HRESULT Execute(bool *, IMessage &);

protected:

    HRESULT ParseOperands();

private:

    DECLARE_KV_HANDLER_CLASS_BEGIN(STCParms)
        DECLARE_KV_CSTRING(ControlName)
		DECLARE_KV_CSTRING(ColorName)
    DECLARE_KV_HANDLER_CLASS_END

    STCParms m_parms;

private:

    // hide copy and assignment
	CSetTextColor(const CSetTextColor&);
	CSetTextColor& operator = (const CSetTextColor &);
};

