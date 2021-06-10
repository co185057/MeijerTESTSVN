// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#pragma once
#include <vector>
#include <map>

class CACSContext;

class CControls
{
	friend class CAppModel;

public:
	CControls(void);
	~CControls(void);

protected:
	CONTROLMAP m_controlMap;
	CCriticalSection m_cs;

private:
	CONTROL & GetAddControl(LPCTSTR pszControl);
	CONTROLFIELD & GetAddControlField(LPCTSTR pszControl, LPCTSTR pszField);

protected:
	void AddContextControlField(LPCTSTR pszControl, LPCTSTR pszField, LPCTSTR pszMapCtx, LPCTSTR pszMapCtxField);
	void AddPropertyControlField(LPCTSTR pszControl, LPCTSTR pszField, LPCTSTR pszMapProperty);

public:
	const PCONTROLFIELD GetControlField(LPCTSTR pszControl, LPCTSTR pszControlField);
	const PCONTROL GetControl(LPCTSTR pszControl);
	//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
	//instead use the VARIANT pointer directly
	void UpdateContextBoundControlFields(const BSTR contextName, const VARIANT& contextData);
	void UpdatePropertyBoundControlFields(LPCTSTR szPropName, LPCTSTR szPropValue);
	inline const CONTROLMAP & GetControlMap(void) { return m_controlMap; };
	bool ResetControl(LPCTSTR pszControl);
};
