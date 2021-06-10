// TBInProgress.cpp: implementation of the TBInProgress class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TBInProgress.h"
#include "customerMHOptions.h"
#include "tbutilities.h"
#include "CustomerCurrentTransaction.h"

#define T_ID _T("TBInProgress")
//////////////////////////////////////////////////////////////////////
// CTBInProgress Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTBInProgress::CTBInProgress()
:m_pTBProgressList(NULL), m_nStatesCount(0)
{
    HRESULT hr = m_pTBStatesDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40));
    _ASSERT(SUCCEEDED(hr));
    m_pTBStatesDoc->async = VARIANT_FALSE;
    m_pTBStatesDoc->validateOnParse = VARIANT_TRUE;
}

CTBInProgress::~CTBInProgress()
{
   tbtraceIO(_T("~TBInProgress()"));
   delete [] m_pTBProgressList; // this has a distructor that deletes the internal objs.
   m_pTBStatesDoc = NULL;
}

void CTBInProgress::Initialize()
{
   tbtraceIO(_T("LoadTBProgressStates"));
	MSXML2::IXMLDOMParseErrorPtr pError;
	MSXML2::IXMLDOMElementPtr ElementPtr;
	MSXML2::IXMLDOMElementPtr subElementPtr;

   _bstr_t strFile = CCustomerMHOptions::instance()->OptionValue(_T("TBInProgressXML"));
   _bstr_t strResult = OLESTR("");

   m_pTBStatesDoc->load(strFile);
   pError = m_pTBStatesDoc->parseError;
   ASSERT(pError->errorCode == S_OK);
   if (pError->errorCode != S_OK)
   {
      strResult = _bstr_t("Validation failed on ") + strFile +
         _bstr_t("\n=====================") +
         _bstr_t("\nReason: ") + _bstr_t(pError->Getreason()) +
         _bstr_t("\nSource: ") + _bstr_t(pError->GetsrcText()) +
         _bstr_t("\nLine: ") + _bstr_t(pError->Getline());
		tbtrace(TM_ERROR, strResult);
		return;
   }

   const _bstr_t cTBProgressState= _bstr_t(_T("/TBProgressStates/TBProgressState"));
   MSXML2::IXMLDOMNodeListPtr spList= m_pTBStatesDoc->selectNodes(cTBProgressState);

	int count= spList->length;
   int indx=0;
   m_pTBProgressList= new tTBInProgressList[count];
   _bstr_t bstrType;
	while ((ElementPtr= spList->nextNode()) != NULL) {
      _variant_t v=ElementPtr->getAttribute(OLESTR("type"));
      if (v.vt != VT_NULL)
         bstrType = v;
      else
         bstrType = OLESTR("");

		_bstr_t bstrName=ElementPtr->getAttribute(OLESTR("name"));
      ASSERT(indx < count);
      CopyString(m_pTBProgressList[indx].szname, (LPCTSTR)bstrName);
      CopyString(m_pTBProgressList[indx].szType, (LPCTSTR)bstrType);
      indx++;
	}
    m_nStatesCount = count;

}

long CTBInProgress::GetKey(LPCTSTR szStateName)
{
   ASSERT(m_nStatesCount);
   // if statename is empty return no state change
   if ((szStateName == NULL) || (szStateName[0] == '\0')) {
      CCustomerCurrentTransaction *pCurrTrx = CCustomerCurrentTransaction::instance();
      return(pCurrTrx->GetCurrentInProgressState());
   }
   if ((_tcscmp(szStateName, _T("cSTATE_ANY")) == 0) ||
       (_tcscmp(szStateName, _T("NO_STATE_CHANGE")) == 0) )
       return -1;
   for (int i=0; i < m_nStatesCount; i++) {
      if (_tcscmp(m_pTBProgressList[i].szname, szStateName) == 0) {
         return i;
      }
   }
   TBASSERT(false);
   tbtrace(TM_ERROR, _T("InProgressState Name:%s not found!"), szStateName);
   return 0;
}

const TCHAR *CTBInProgress::GetName(long key)
{
   ASSERT(m_nStatesCount > key); 
   if ((m_nStatesCount > key) && (key >= 0))
      return m_pTBProgressList[key].szname;

   return _T("UNKNOWN");
 
}

const TCHAR *CTBInProgress::GetType(long key)
{
   ASSERT((m_nStatesCount > key));
   if ((m_nStatesCount > key) && (key >=0))
      return m_pTBProgressList[key].szType;
   return _T("UNKNOWN");
}
