// ACSHookProperties.cpp : Implementation of CACSHook

#include "stdafx.h"
#include "ACSHook.h"
#include "Utils.h"
#include "TraceImpl.h"

STDMETHODIMP CACSHook::get_States(VARIANT* pVal)
{
   Trace trace(_T("CACSHook::get_States"));
   trace(DCM_INFO, _T("CACSHook::get_States"));
      
   States const& states = m_appModel.GetStates();

   SAFEARRAY * psa = NULL;
   SAFEARRAYBOUND sab;
   sab.lLbound = 0;
   sab.cElements = (ULONG)states.size();
   psa = SafeArrayCreate(VT_BSTR, 1, &sab); 
   _ASSERT(psa);

   _bstr_t stateName;
   long lPos = 0;

   for (States::const_iterator it = states.begin(); it != states.end(); ++it)
   {
      stateName = it->Name();
      HRESULT hr = SafeArrayPutElement(psa, &lPos, stateName.copy());

      if( FAILED(hr) )
      {
         SafeArrayDestroy(psa);
         return hr;
      }

      lPos++;
   }

   VariantInit(pVal);
   V_VT(pVal) = VT_ARRAY | VT_BSTR;
   V_ARRAY(pVal) = psa;

   return S_OK;
}




STDMETHODIMP CACSHook::get_Inputs(VARIANT* pVal)
{
   Trace trace(_T("CACSHook::get_Inputs"));
   trace(DCM_INFO, _T("CACSHook::get_Inputs"));

   SAFEARRAY * psa = NULL;

   SequenceMap const& sequenceMap = m_appModel.GetSequenceMap();
   InputTextMap const& inputTextMap = m_appModel.GetInputTextMap();

   SAFEARRAYBOUND sab;
   sab.lLbound = 0;
   sab.cElements = (ULONG)(sequenceMap.size() + inputTextMap.size());
   psa = SafeArrayCreate(VT_BSTR, 1, &sab); 
   _ASSERT(psa);
   long lPos = 0;

   _bstr_t inputName;

   for (SequenceMap::const_iterator it = sequenceMap.begin(); it != sequenceMap.end(); ++it)
   {
      inputName = it->first.c_str();
      HRESULT hr = SafeArrayPutElement(psa, &lPos, inputName.copy());
      if( FAILED(hr) )
      {
         SafeArrayDestroy(psa);
         return hr;
      }
   }

   for (InputTextMap::const_iterator it = inputTextMap.begin(); it != inputTextMap.end(); ++it)
   {
      inputName = it->first.c_str();
      HRESULT hr = SafeArrayPutElement(psa, &lPos, inputName.copy());
      if( FAILED(hr) )
      {
         SafeArrayDestroy(psa);
         return hr;
      }
   }

   VariantInit(pVal);
   V_VT(pVal) = VT_ARRAY | VT_BSTR;
   V_ARRAY(pVal) = psa;

   return S_OK;
}


STDMETHODIMP CACSHook::get_Properties(VARIANT* pVal)
{
   Trace trace(_T("CACSHook::get_Properties"));

   trace(DCM_INFO, _T("CACSHook::get_Properties"));

   MSXML2::IXMLDOMNodeListPtr spProperties = m_appModel.GetProperties();
   MSXML2::IXMLDOMNodePtr spProperty = NULL;

   SAFEARRAY * psa = NULL;

   SAFEARRAYBOUND sab;
   sab.lLbound = 0;
   sab.cElements = spProperties->length;
   psa = SafeArrayCreate(VT_BSTR, 1, &sab); 
   _ASSERT(psa);
   long lPos = 0;
   while( (spProperty = spProperties->nextNode()) != NULL )
   {
      HRESULT hr = SafeArrayPutElement(psa, &lPos, m_appModel.GetPropertyName(spProperty).copy());
      if( FAILED(hr) )
      {
         SafeArrayDestroy(psa);
         return hr;
      }
      lPos++;
   }

   VariantInit(pVal);
   V_VT(pVal) = VT_ARRAY | VT_BSTR;
   V_ARRAY(pVal) = psa;

   return S_OK;
}

STDMETHODIMP CACSHook::get_Contexts(VARIANT* pVal)
{
   Trace trace(_T("CACSHook::get_Contexts"));

    trace(DCM_INFO, _T("CACSHook::get_Contexts"));
	
	APPCONTEXTMAP & ctxMap = m_Contexts.GetContextMap();

	SAFEARRAY * psa = NULL;
    SAFEARRAYBOUND sab;
    sab.lLbound = 0;
    sab.cElements = (ULONG)ctxMap.GetCount();
    psa = SafeArrayCreate(VT_BSTR, 1, &sab); 
    _ASSERT(psa);
    long lPos = 0;
	
	
	POSITION pos = ctxMap.GetStartPosition();
	//PAPPCONTEXTMAPITEM pcmi = NULL;
	APPCONTEXTMAPITEM const * pcmi = NULL;
	while( pos && (pcmi = ctxMap.GetNext(pos)) )
	{
		HRESULT hr = SafeArrayPutElement(psa, &lPos, _bstr_t(pcmi->m_key.GetString()).copy());
        if( FAILED(hr) )
        {
            SafeArrayDestroy(psa);
            return hr;
        }
        lPos++;
	}

    VariantInit(pVal);
    V_VT(pVal) = VT_ARRAY | VT_BSTR;
    V_ARRAY(pVal) = psa;

    return S_OK;
}

STDMETHODIMP CACSHook::get_Controls(VARIANT* pVal)
{
   Trace trace(_T("CACSHook::get_Controls"));

    trace(DCM_INFO, _T("CACSHook::get_Contexts"));
	
	const CONTROLMAP & ctrlMap = m_Controls.GetControlMap();

	SAFEARRAY * psa = NULL;
    SAFEARRAYBOUND sab;
    sab.lLbound = 0;
    sab.cElements = (ULONG)ctrlMap.GetCount();
    psa = SafeArrayCreate(VT_BSTR, 1, &sab); 
    _ASSERT(psa);
    long lPos = 0;
	
	
	POSITION pos = ctrlMap.GetStartPosition();
	CONTROLMAPITEM const * pcmi = NULL;
	while( pos && ( pcmi = ctrlMap.GetNext(pos)) )
	{
		HRESULT hr = SafeArrayPutElement(psa, &lPos, _bstr_t(pcmi->m_key.GetString()).copy());
        if( FAILED(hr) )
        {
            SafeArrayDestroy(psa);
            return hr;
        }
        lPos++;
	}

    VariantInit(pVal);
    V_VT(pVal) = VT_ARRAY | VT_BSTR;
    V_ARRAY(pVal) = psa;

    return S_OK;
}
