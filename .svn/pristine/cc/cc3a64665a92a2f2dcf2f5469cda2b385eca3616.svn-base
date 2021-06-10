
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SetText.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "PSXMfc.h"
#include "verifypsx.h"
#include "RCMLaneXML.h"
#include "LocaleEx.h"

BEGIN_KV_HANDLER_CLASS(CSetText::STParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetText::STParms, KEY_CONTROL_NAME, ControlName)
	DEFINE_KV_HANDLER_MAP_ENTRY(CSetText::STParms, KEY_CONTEXT_NAME, ContextName)
	DEFINE_KV_HANDLER_MAP_ENTRY(CSetText::STParms, KEY_EV_STRING_ID, StringId)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSetText::STParms, ControlName)
DEFINE_KV_CSTRING(CSetText::STParms, ContextName)
DEFINE_KV_CSTRING(CSetText::STParms, StringId)

void CSetText::STParms::Clear()
{
    m_ControlName.Empty();
	m_ContextName.Empty();
    m_StringId.Empty();
}


CSetText::CSetText()
{
    CRCMActionBase::SetName(_T("CSetText"));
}


CSetText::~CSetText()
{
}


HRESULT CSetText::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());

    return S_OK;
}


HRESULT CSetText::Execute(bool *pResult, IMessage &)
{
    HRESULT hr = ParseOperands();
    if (hr != S_OK)
	{ return hr; }

    *pResult = true;

    if (m_parms.GetControlName().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_CONTROL_NAME));
        return STATEM_MISSING_PARAMETER;
    }
    if (m_parms.GetStringId().IsEmpty())
    {
        ITRACE_ERROR(_T("Missing parameter:  ") + IString(KEY_EV_STRING_ID));
        return STATEM_MISSING_PARAMETER;
    }

	CString csContextName = m_parms.GetContextName();

	bool bUseAllContexts = false;
	if( (csContextName.IsEmpty()) || (csContextName == KEY_CONTEXT_NAME_ALL) )
	{
		bUseAllContexts = true;
	}

    CString sPatternText = GetString( m_parms.GetStringId() );
    IString sString((const _TCHAR *)sPatternText);

    IString sDateFormat = (const _TCHAR *)CLocale::GetDateFormat(GetTraceHandle());
    IString sText;
    sText.format(sString, (const _TCHAR *)sDateFormat);

    COleVariant vText = sText;

	ITRACE_DEVELOP(_T("Setting control text to:  ") + sText);
	VERIFY_PSX_AND_LOG(psx().SetConfigProperty(m_parms.GetControlName(), (bUseAllContexts ? UI::ALL_CONTEXTS : csContextName), UI::PROPERTYTEXTFORMAT, vText));

    return S_OK;
}


template <>
IAction* CCreateable<CSetText, IAction>::NewInstance()
{
    return new CSetText;
}


template <>
void CCreateable<CSetText, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetText)
DEFINE_ACTION_RELEASE_FXN(SetText)
