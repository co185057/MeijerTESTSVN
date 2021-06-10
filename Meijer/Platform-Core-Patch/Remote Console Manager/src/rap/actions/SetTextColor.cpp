
#include "StdAfx.h"

namespace fastlane {}
using namespace fastlane;

#include "SetTextColor.h"
#include "RCManager.h"
#include "RCMStateMachine.h"
#include "RCMKeyValue.h"
#include "KVParmList.h"
#include "PSXMfc.h"
#include "verifypsx.h"
#include "RCMLaneXML.h"

BEGIN_KV_HANDLER_CLASS(CSetTextColor::STCParms)
    DEFINE_KV_HANDLER_MAP_ENTRY(CSetTextColor::STCParms, KEY_CONTROL_NAME, ControlName)
	DEFINE_KV_HANDLER_MAP_ENTRY(CSetTextColor::STCParms, KEY_COLOR_NAME, ColorName)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(CSetTextColor::STCParms, ControlName)
DEFINE_KV_CSTRING(CSetTextColor::STCParms, ColorName)

void CSetTextColor::STCParms::Clear()
{
    m_ControlName.Empty();
    m_ColorName.Empty();
}


CSetTextColor::CSetTextColor()
{
    CRCMActionBase::SetName(_T("CSetTextColor"));
}


CSetTextColor::~CSetTextColor()
{
}


// Get all control names from the xml
HRESULT CSetTextColor::ParseOperands()
{ 
    m_parms.Clear();

    CParameterList &parms = GetParameters();
    CParameterList::iterator iter = parms.begin();

    if (iter == parms.end())
        return S_OK;

    return m_parms.ParseKeys((*iter).c_str(), GetTraceHandle());
}


HRESULT CSetTextColor::Execute(bool *pResult, IMessage &)
{
	HRESULT hr = ParseOperands();

    if (m_parms.GetControlName().IsEmpty())
    {
        ITRACE_ERROR(KEY_CONTROL_NAME _T(" parameter is missing."));
        return STATEM_MISSING_PARAMETER;
    }
    if (m_parms.GetColorName().IsEmpty())
    {
        ITRACE_ERROR(KEY_COLOR_NAME _T(" parameter is missing."));
        return STATEM_MISSING_PARAMETER;
    }

    StringType sTextColor(m_parms.GetColorName());

    COleVariant vTextColor;

    ITRACE_DEVELOP(_T("Text color:  ") + IString(sTextColor.c_str()));
    VERIFY_PSX_AND_LOG(psx().GetConfigColor(sTextColor.c_str(), vTextColor));
    VERIFY_PSX_AND_LOG(psx().SetConfigProperty(m_parms.GetControlName(), UI::ALL_CONTEXTS, UI::PROPERTYTEXTCOLOR, vTextColor));

    *pResult = true;

    return hr;
}


template <>
IAction* CCreateable<CSetTextColor, IAction>::NewInstance()
{
    return new CSetTextColor;
}


template <>
void CCreateable<CSetTextColor, IAction>::ReleaseInstance(IAction *pAction)
{
    delete pAction;
}


DEFINE_ACTION_CREATE_FXN(SetTextColor)
DEFINE_ACTION_RELEASE_FXN(SetTextColor)
