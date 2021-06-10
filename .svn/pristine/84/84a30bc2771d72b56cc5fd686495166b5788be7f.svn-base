#include "stdafx.h"
#include "ConfigData.h"
#include <string.h>

CConfigData::CConfigData(LPCTSTR sSource):
   m_dLayoutFactor(1)
	{
	if (sSource != NULL)
		SetSource(sSource);
	}

CConfigData::CConfigData(HKEY hkParent, LPCTSTR sSubKey):
   m_dLayoutFactor(1)
   {
   // This ASSERTION means a registry key was passed in that we cannot open.
   VERIFY(SetSource(hkParent, sSubKey));
   }


// Copy constructor (It's not safe to make binary copies of HKEYs!!!)

CConfigData::CConfigData(const CConfigData &ref):
   m_sDataSource(ref.m_sDataSource),
   m_dLayoutFactor(ref.m_dLayoutFactor)
   {
      HKEY hkDuplicate = NULL;
   if (0 != m_rkDataSource.m_hKey)
      {
      if (DuplicateHandle(
         GetCurrentProcess(), 
         ref.m_rkDataSource.m_hKey, 
         GetCurrentProcess(), 
         (HANDLE *)&hkDuplicate, 
         0, 
         FALSE, 
         DUPLICATE_SAME_ACCESS))
         {m_rkDataSource.Attach(hkDuplicate);}
         else
            {ASSERT(0);}   // Failed to duplicate handle - bad copy.
      }
   }

// Assignment operator. (It's not safe to make binary copies of HKEYs!!!)
CConfigData &CConfigData::operator = (const CConfigData &ref)
   {
   if (&ref != this)
      {
      if (0 != m_rkDataSource.m_hKey)
         VERIFY(ERROR_SUCCESS == m_rkDataSource.Close());

      m_dLayoutFactor = ref.m_dLayoutFactor;

      if (0 != ref.m_rkDataSource.m_hKey)
         {
         HKEY hkDuplicate = NULL;
         if (DuplicateHandle(
            GetCurrentProcess(), 
            ref.m_rkDataSource.m_hKey, 
            GetCurrentProcess(), 
            (HANDLE *)&hkDuplicate, 
            0, 
            FALSE, 
            DUPLICATE_SAME_ACCESS))
            {m_rkDataSource.Attach(hkDuplicate);}
         else
            {ASSERT(0);}   // Failed to duplicate handle - bad copy.
         }
      else
         {
         m_sDataSource = ref.m_sDataSource;
         }
      }
   return *this;
   }

CConfigData::~CConfigData()
   {
   }

BOOL CConfigData::EnumKeys(CString &sKey, DWORD dwIndex) const
	{
   BOOL bStatus = FALSE;
	TCHAR		szKeys[CD_ENUM_KEYS_BUFFER_SIZE];
   DWORD dwSize = CD_ENUM_KEYS_BUFFER_SIZE;

   if (0 != m_rkDataSource.m_hKey)
      {
      FILETIME ft;

      if (RegEnumKeyEx(
         m_rkDataSource.m_hKey, 
         dwIndex, 
         szKeys, 
         &dwSize, 
         NULL, 
         NULL, 
         NULL, 
         &ft) == ERROR_SUCCESS)
         {
         bStatus = TRUE;
         sKey = szKeys;
         }
      }
   else
      {
	   DWORD dwCount;
	   LPCTSTR	lpKey = szKeys;
   	DWORD dwStatus = GetPrivateProfileString(
		   NULL, 
		   NULL, 
		   NULL, 
		   szKeys, 
		   sizeof(szKeys), 
		   m_sDataSource);

      if (dwStatus)
         {
         bStatus = TRUE;
	      for (dwCount = 0; dwCount < dwIndex; dwCount++)
		      {
		      if (_tcslen(lpKey) + lpKey + 1 >= szKeys + dwStatus)
               {
			      // The index requested is greater than the number of keys.
               bStatus = FALSE;
			      break;
               }

		      lpKey += _tcslen(lpKey) + 1;
		      }

         if (bStatus)
            {sKey = lpKey;}
         }
      }

   return bStatus;
	}
// Begin RFQ 2605
BOOL CConfigData::EnumSectionValues(CString sSection, CString &sKey, DWORD dwIndex) const
{
	BOOL bStatus = FALSE;
	TCHAR		szKeys[CD_ENUM_KEYS_BUFFER_SIZE];
	DWORD dwSize = CD_ENUM_KEYS_BUFFER_SIZE;
	
	   DWORD dwCount;
	   LPCTSTR	lpKey = szKeys;
	   DWORD dwStatus = GetPrivateProfileString(
		   sSection, 
		   NULL, 
		   NULL, 
		   szKeys, 
		   sizeof(szKeys), 
		   m_sDataSource);
	   
	   if (dwStatus)
	   {
		   bStatus = TRUE;
		   for (dwCount = 0; dwCount < dwIndex; dwCount++)
		   {
			   if (_tcslen(lpKey) + lpKey + 1 >= szKeys + dwStatus)
               {
				   // The index requested is greater than the number of keys.
				   bStatus = FALSE;
				   break;
               }
			   
			   lpKey += _tcslen(lpKey) + 1;
		   }
		   
		   if (bStatus)
		   {sKey = lpKey;}
	   }
	   
	   
	   return bStatus;
}
// End RFQ 2605
void CConfigData::GetString(LPCTSTR sKey, LPCTSTR sValueName, CString &sValue, LPCTSTR sDefault) const
	{
	DWORD	dwTemp = CD_GET_STRING_BUFFER_SIZE;
	TCHAR	szValue[CD_GET_STRING_BUFFER_SIZE];

   if (0 != m_rkDataSource.m_hKey)
      {
      CRegKey rkSubKey;
      if (ERROR_SUCCESS == rkSubKey.Open(m_rkDataSource, sKey))
         {
         sValue = 
            (ERROR_SUCCESS == rkSubKey.QueryValue(szValue, sValueName, &dwTemp)) 
            ? szValue : sDefault;

         VERIFY(ERROR_SUCCESS == rkSubKey.Close());
         }
      }
   else
      {
	   dwTemp = GetPrivateProfileString(
		   (LPCTSTR)sKey,
		   (LPCTSTR)sValueName,
		   (LPCTSTR)sDefault,
		   szValue,
		   sizeof(szValue),
		   m_sDataSource);

	   sValue = szValue;
      }
   }

BOOL CConfigData::WriteString(LPCTSTR sKey, LPCTSTR sValueName, LPCTSTR sValue) const
	{
   BOOL bStatus = FALSE;

   if (0 != m_rkDataSource.m_hKey)
      {
      CRegKey rkSubKey;
      if (ERROR_SUCCESS == rkSubKey.Open(m_rkDataSource, sKey))
         {
         bStatus = (ERROR_SUCCESS == rkSubKey.SetValue(sValue, sValueName));
         VERIFY(ERROR_SUCCESS == rkSubKey.Close());
         }
      }
   else
      {
	   bStatus = WritePrivateProfileString(
		   (LPCTSTR)sKey,
		   (LPCTSTR)sValueName,
		   sValue,
		   m_sDataSource);
      }
   return bStatus;
	}

BOOL CConfigData::WriteInt(LPCTSTR sKey, LPCTSTR sValueName, DWORD dwValue) const
	{
   BOOL bStatus = FALSE;

   if (0 != m_rkDataSource.m_hKey)
      {
      CRegKey rkSubKey;
      if (ERROR_SUCCESS == rkSubKey.Open(m_rkDataSource, sKey))
         {
         bStatus = (ERROR_SUCCESS == rkSubKey.SetValue(dwValue, sValueName));
         VERIFY(ERROR_SUCCESS == rkSubKey.Close());
         }
      }
   else
      {
      CString sValue;
      sValue.Format(_T("%d"), dwValue);
	   bStatus = WritePrivateProfileString(
		   (LPCTSTR)sKey,
		   (LPCTSTR)sValueName,
		   sValue,
		   m_sDataSource);
      }
   return bStatus;
	}

ULONG CConfigData::GetInt(LPCTSTR sKey, LPCTSTR sValueName,ULONG ulDefault) const
	{
   ULONG ulValue = ulDefault;
   if (0 != m_rkDataSource.m_hKey)
      {
      CRegKey rkSubKey;
      if (ERROR_SUCCESS == rkSubKey.Open(m_rkDataSource, sKey))
         {
         if (ERROR_SUCCESS != rkSubKey.QueryValue(ulValue, sValueName))
            {ulValue = ulDefault;}
         VERIFY(rkSubKey.Close());
         }
      }
   else
      {
	   ulValue = (ULONG)GetPrivateProfileInt(
		   (LPCTSTR)sKey,
		   (LPCTSTR)sValueName,
		   (INT)ulDefault,
		   m_sDataSource);
      }
   return ulValue;
	}

	
CRect CConfigData::GetRect(LPCTSTR sKey, LPCTSTR sRect) const
	{
	ASSERT (sKey != NULL);
	ASSERT (sRect != NULL);

	CString sx(_T("X")), sy(_T("Y")), sPos(_T("Pos")), sSize(_T("Size"));
	CRect rc(0,0,0,0);

	rc.left = (long)(GetInt(sKey, sx + sRect + sPos) * m_dLayoutFactor);
	rc.top = (long)(GetInt(sKey, sy + sRect + sPos) * m_dLayoutFactor);
	rc.right = (long)((GetInt(sKey, sx + sRect + sSize)  * m_dLayoutFactor) + rc.left);
	rc.bottom = (long)((GetInt(sKey, sy + sRect + sSize) * m_dLayoutFactor) + rc.top);

	return rc;
	}

void CConfigData::SetSource(LPCTSTR sSource)
	{
	ASSERT (sSource != NULL);
	m_sDataSource = sSource;
   if (0 != m_rkDataSource.m_hKey)
      {
      VERIFY(m_rkDataSource.Close());
      }
   }

BOOL CConfigData::SetSource(HKEY hkParent, LPCTSTR sSubKey)
   {
   ASSERT (hkParent != NULL);
   ASSERT (sSubKey != NULL);

   if (0 != m_rkDataSource.m_hKey)
      {
      VERIFY(m_rkDataSource.Close());
      }

   return (ERROR_SUCCESS == m_rkDataSource.Open(hkParent, sSubKey));
   }

void CConfigData::Flush() const
   {
   if (0 != m_rkDataSource.m_hKey)
      VERIFY(ERROR_SUCCESS == RegFlushKey(m_rkDataSource));
   }

void CConfigData::SetLayoutFactor(double dLayoutFactor)
   {
   m_dLayoutFactor = dLayoutFactor;
   }