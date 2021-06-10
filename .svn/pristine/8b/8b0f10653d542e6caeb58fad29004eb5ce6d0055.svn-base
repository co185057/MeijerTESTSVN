// CurrentPosState.cpp: implementation of the CCurrentPosState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurrentPosState.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCurrentPosState::CCurrentPosState()
:CTBGenMsg(TBMSG_POSSTATE, true, false) // xml output, no validate
{

}

CCurrentPosState::~CCurrentPosState()
{

}

_variant_t CCurrentPosState::GetValue(LPCTSTR szName)
{
	MSXML2::IXMLDOMNodePtr pField;
	CString csxPathString;
   _variant_t v;
	csxPathString.Format(_T("field[@name = \"%s\"]"), szName); 
	if (m_pFields) {
		pField= m_pFields->selectSingleNode(_bstr_t(csxPathString));
		if (pField) {
			v= pField->nodeTypedValue;
			return v;
		}
		else {
			CString cs;
			cs.Format(_T("field [%s] not found"), szName);
			_bstr_t bstrString(cs);
			v = bstrString;
			return v;
			//throw v;
		}
	}
	else{
		_bstr_t bstrString(_T("Message Not Loaded"));
		v = bstrString;
		return v;
	//	throw CString(_T("Message Not Loaded"));
	}
   v.vt=VT_EMPTY;
   return v;
}