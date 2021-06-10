#include "StdAfx.h"
#include "ACSContext.h"
#include "DataKit.h"


using namespace std;


CACSContext::CACSContext(LPCSTR szContext, LPCVOID pvContextData) : 
   pContext(pvContextData),
   contextName(CA2T(szContext))
{
}


CACSContext::~CACSContext(void)
{
}

// returning a pointer is okay in this case because it will 
// only be used while the object is alive.
LPCTSTR CACSContext::GetName()
{
   return contextName;
}


CString CACSContext::GetFieldAsString(const CString& fieldName)
{
   return DataKit::GetContextField(contextName, fieldName, pContext);
}

