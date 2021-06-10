#include "StdAfx.h"
#include "CompareToCurrentContext.h"
#include "RemoteAPMgr.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "StringComparator.h"
#include "PSXMfc.h"

using namespace fastlane;


BEGIN_KV_HANDLER_CLASS(CCompareToCurrentContext::SLCCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CCompareToCurrentContext::SLCCParms, KEY_CT_CONTEXT, Context)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CCompareToCurrentContext::SLCCParms, Context)


void CCompareToCurrentContext::SLCCParms::Clear()
{
    m_Context.Empty();
}


CCompareToCurrentContext::CCompareToCurrentContext()
{
    CRCMActionBase::SetName(_T("CCompareToCurrentContext"));
}


CCompareToCurrentContext::~CCompareToCurrentContext()
{
}


// Get all control names from the xml
HRESULT CCompareToCurrentContext::ParseOperands()
{ 
    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();
    if (iter == parms.end())
        return STATEM_MISSING_PARAMETER;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CCompareToCurrentContext::Execute(bool *pResult, IMessage &message)
{
    m_parms.Clear();

    HRESULT hr = ParseOperands();
    if (hr != S_OK)
        return hr;

    StringType sContextName = (const _TCHAR *)m_parms.GetContext();
    if (sContextName.empty())
        return STATEM_MISSING_PARAMETER;
         
	CString sCurrentContextName;

	psx().GetContext( sCurrentContextName );	

    *pResult = (sContextName == (const _TCHAR *)sCurrentContextName);

    return hr;
}


template <>
IAction* CCreateable<CCompareToCurrentContext, IAction>::NewInstance()
{
    IAction *pAction = new CCompareToCurrentContext;

    return pAction;
}


template <>
void CCreateable<CCompareToCurrentContext, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(CompareToCurrentContext)
DEFINE_ACTION_RELEASE_FXN(CompareToCurrentContext)
