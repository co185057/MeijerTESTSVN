#pragma once

class CACSContext
{
public:
	CACSContext(LPCSTR szContext, LPCVOID pvParam);
	~CACSContext(void);

	LPCTSTR GetName();
   CString GetFieldAsString(const CString& fieldName);

	inline BOOL	Equals(LPCTSTR pszCtx)
	{
      return pszCtx && (contextName == pszCtx);
	}

	inline BOOL	Equals(LPCSTR pszCtx)
	{
      return (Equals(CA2T(pszCtx)));
	}


protected:
	LPCVOID pContext;
   CString contextName;
};
