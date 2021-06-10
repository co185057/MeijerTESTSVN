#ifndef __CONFIGDATA_H__
#define __CONFIGDATA_H__

#include "stdafx.h"
#ifndef __ATLBASE_H__
   #include <atlbase.h>
#endif

#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif 
 CConfigData
	{
public:
	CConfigData(LPCTSTR sSource = NULL);
   CConfigData(HKEY hkParent, LPCTSTR sSubKey = NULL);
   CConfigData(const CConfigData &ref);
   CConfigData &operator = (const CConfigData &ref);

	virtual ~CConfigData();
	BOOL EnumKeys(CString &sKey, DWORD iIndex) const;
	BOOL EnumSectionValues(CString sSection, CString &sKey, DWORD iIndex) const;  // RFQ 2605
	CString GetSource() const {return m_sDataSource;}
   void Flush() const;
	void SetSource(LPCTSTR sSource = NULL);
	BOOL SetSource(HKEY hkParent, LPCTSTR sSubKey = NULL);
   void SetLayoutFactor(double dLayoutFactor);
	void GetString(LPCTSTR sKey, LPCTSTR sValueName, CString&sValue, LPCTSTR sDefault = _T("")) const;
	BOOL WriteString(LPCTSTR sKey, LPCTSTR sValueName, LPCTSTR sValue) const;
   BOOL WriteInt(LPCTSTR sKey, LPCTSTR sValueName, DWORD dwValue) const;
	ULONG GetInt(LPCTSTR sKey, LPCTSTR sValueName, ULONG ulDefault = 0) const;
	CRect GetRect(LPCTSTR sKey, LPCTSTR sRect) const;

protected:
	CString m_sDataSource;
   CRegKey m_rkDataSource;
   double m_dLayoutFactor;
	};

#define CD_KEY_DEFAULT							_T("No Keys Defined")
#define CD_ENUM_KEYS_BUFFER_SIZE				1024
#define CD_GET_STRING_BUFFER_SIZE			32767

#endif