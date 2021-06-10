
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "IsTriliteColorGreen.h"
#include "RemoteAPMgr.h"
#include "RCMKeyValue.h"
#include "Message.h"
#include "KVParmList.h"


BEGIN_KV_HANDLER_CLASS(CIsTriliteColorGreen::ICTCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CIsTriliteColorGreen::ICTCParms, KEY_CT_OPERATION_SET_LIGHT_STATE_GREEN, Green)
END_KV_HANDLER_CLASS()
DEFINE_KV_INT(CIsTriliteColorGreen::ICTCParms, Green)
BEGIN_KV_HANDLER_CLASS_CLEAR(CIsTriliteColorGreen::ICTCParms)
    CLEAR_KV_INT(Green, 0)
END_KV_HANDLER_CLASS_CLEAR()


CIsTriliteColorGreen::CIsTriliteColorGreen()
{
    CRCMActionBase::SetName(_T("CIsTriliteColorGreen"));
}


CIsTriliteColorGreen::~CIsTriliteColorGreen()
{
}


//-------------------------------------------------------------------------
// Methods from IEvaluateable
//-------------------------------------------------------------------------
long CIsTriliteColorGreen::GetValue() const { return 0; }
void CIsTriliteColorGreen::SetValue(long) {}


HRESULT CIsTriliteColorGreen::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = m_parms.ParseKeys((_TCHAR *)message.GetData(), GetTraceHandle());
    if (hr != S_OK)
        return hr;
 
    if (m_parms.GetGreen() != 0)
        *pResult = true;
    else
        *pResult = false;
    
    return S_OK;
}


template <>
IAction* CCreateable<CIsTriliteColorGreen, IAction>::NewInstance()
{
    IAction *pAction = new CIsTriliteColorGreen;

    return pAction;
}


template <>
void CCreateable<CIsTriliteColorGreen, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(IsTriliteColorGreen)
DEFINE_ACTION_RELEASE_FXN(IsTriliteColorGreen)
