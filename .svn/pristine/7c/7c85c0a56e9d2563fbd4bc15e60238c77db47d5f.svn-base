//////////////////////////////////////////////////////////////////////
//
// ConfigDynamic.cpp: implementation of Dynamic configuration options
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define COMP_ID ID_SM
#define T_ID	_T("ConfigDynamic")


void ConfigObject::InitializeDynamicOptions()
{
    m_bAttendantMode = false;
    m_csAttendantID = EMPTY_STRING;
	m_csContextPrefixCustom = EMPTY_STRING;
	m_csContextPrefixAM = EMPTY_STRING;
    m_bRemoteScannerConnected = false;
	m_bSecurityDisable = false; // SR683
	m_bSecurityDisableSubCheck = false;
}

void ConfigObject::SetAttendantMode(bool bAttendantMode) 
{ 
    trace(L6,_T("+-Set AttendantMode flag to %d"), bAttendantMode);
    m_bAttendantMode = bAttendantMode; 
}
bool ConfigObject::IsAttendantModeOn() 
{ 
    trace(L7,_T("+-IsAttendantModeOn m_bAttendantMode=%d,IsRemoteScannerConnected=%d, returning %d"), 
        m_bAttendantMode,
        IsRemoteScannerConnected(),
        m_bAttendantMode || IsRemoteScannerConnected());
    return (m_bAttendantMode || IsRemoteScannerConnected()); 
}

bool ConfigObject::IsLoginBypassOn()
{
    bool bRet = ((IsAttendantModeOn() && fStateAttndModeLoginBypass) ||
		         IsInAssistMenus());
    trace(L7,_T("+-IsLoginBypassOn: returning %d"), bRet);
    return (bRet);
}

bool ConfigObject::IsSecurityBypassOn()
{
    //Security bypass is currently only enabled when in attendant mode.
    trace(L7,_T("+-IsAttendantModeOn returning %d"), m_bAttendantMode);
    //return (IsAttendantModeOn() || IsInAssistMenus() || IsSecurityDisableOn());
    return (IsAttendantModeOn() || IsInAssistMenus());
}

// +SR683
void ConfigObject::SetSecurityDisable(bool bValue)
{
    trace(L6,_T("Setting SecurityDisable to %d"), bValue);
    m_bSecurityDisable = bValue;
}

bool ConfigObject::IsSecurityDisableOn()
{
    trace(L7,_T("+-IsSecurityDisableOn returning %d"), m_bSecurityDisable);
    return m_bSecurityDisable;
}
// -SR683

//+SR814.4
void ConfigObject::SetSecurityDisableSubCheck(bool bValue)
{
    trace(L6,_T("Setting SecurityDisableSubCheck to %d"), bValue);
    m_bSecurityDisableSubCheck = bValue;
}

bool ConfigObject::GetSecurityDisableSubCheckOn()
{
    trace(L7,_T("+-GetSecurityDisableSubCheckOn returning %d"), m_bSecurityDisableSubCheck);
    return m_bSecurityDisableSubCheck;
}
//-SR814.4
bool ConfigObject::IsImmedInterventionOn()
{
    bool bRet = ((IsAttendantModeOn() && fStateAttndModeImmedInterventions) ||
		          IsInAssistMenus());
    trace(L7,_T("+-IsImmedInterventionOn:    returning %d"), bRet);
    return bRet;
}

void ConfigObject::SetAttendantID(CString csAttendantID)
{
    trace(L6,_T("+-Set AttendantID to %s"), csAttendantID);
    m_csAttendantID = csAttendantID;
}

CString ConfigObject::GetAttendantID()
{
    trace(L7, _T("+-GetAttendantID returning %s"), m_csAttendantID);
    return m_csAttendantID;
}
    
bool ConfigObject::IsInAssistMenus()
{
	return m_bAssistMenus;
}
   
void ConfigObject::SetInAssistMenus(bool bAssistMenus)
{
	m_bAssistMenus = bAssistMenus;
}

void ConfigObject::SetContextPrefixAM(CString csContextPrefixAM)
{
	m_csContextPrefixAM = csContextPrefixAM;
}

void ConfigObject::SetContextPrefixCustom(CString csContextPrefixCustom)
{
	m_csContextPrefixCustom = csContextPrefixCustom;
}

CString ConfigObject::GetContextPrefix()
{
	if (m_csContextPrefixAM != EMPTY_STRING && IsInAssistMenus())
	{
		return m_csContextPrefixAM;
	}
	else
	{
		return m_csContextPrefixCustom;
	}
}
bool ConfigObject::IsRemoteScannerConnected()
{
    return m_bRemoteScannerConnected;
}

void ConfigObject::SetRemoteScannerConnected(bool bRemoteScannerConnected)
{
    m_bRemoteScannerConnected = bRemoteScannerConnected;
}
