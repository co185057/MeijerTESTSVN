// BaseTBProperties.cpp: implementation of the CBaseTBProperties class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BaseTBProperties.h"
#include "autolock.h"
#include "CustomerMHOptions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseTBProperties::CBaseTBProperties()
{
   InitializeCriticalSection(&m_CritSec);
}

CBaseTBProperties::~CBaseTBProperties()
{
	DeleteCriticalSection(&m_CritSec);
}

void CBaseTBProperties::SetProperty(LPCTSTR szPropertyName, LPCTSTR szValue)
{
   CAutoLock lock(m_CritSec); // make this thread safe. locks crit section in this function.  
   m_stringMap[szPropertyName] = szValue;
}

CString CBaseTBProperties::GetProperty(LPCTSTR szPropertyName)
{
   CAutoLock lock(m_CritSec); // make this thread safe. locks crit section in this function.  
	m_MapIterator= m_stringMap.find(szPropertyName);
   if (m_MapIterator != m_stringMap.end())
      return (*m_MapIterator).second;

   // we did not find it in our local properties.
   // get it from MHOptions if it's there!

   CString csPropertyName(szPropertyName);
   return CCustomerMHOptions::instance()->OptionValue(csPropertyName);

}

void CBaseTBProperties::SetHookProperty(LPCTSTR szPropertyName, LPCTSTR szValue)
{
   // default is local properties
   SetProperty(szPropertyName, szValue);
}

CString CBaseTBProperties::GetHookProperty(LPCTSTR szPropertyName)
{
   // default is local properties
   return GetProperty(szPropertyName);
}
