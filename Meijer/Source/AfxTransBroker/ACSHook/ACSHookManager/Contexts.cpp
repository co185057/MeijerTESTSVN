// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#include "StdAfx.h"
#include "Contexts.h"
#include "ACSContext.h"
#include "Utils.h"
#include "TraceImpl.h"
#include "ACSIODataKit.h"

CContexts::CContexts(void)
{
	CCriticalSectionLock lock(m_cs, TRUE);
	m_contextMap.RemoveAll();
}

CContexts::~CContexts(void)
{
}


APPCONTEXT & CContexts::GetAddContext(LPCTSTR pszContext)
{
   Trace trace(_T("CContexts::GetAddContext"), false);
	CCriticalSectionLock lock(m_cs, TRUE);
	PAPPCONTEXTMAPITEM pC = m_contextMap.Lookup(pszContext);
	if( !pC )
	{
		trace(DCM_INFO) << _T("Adding Context '") << pszContext << _T("' to map.") << NCR::Util::endl;
		m_contextMap[pszContext].dwLastSavedTime = 0;
		pC = m_contextMap.Lookup(pszContext);
		pC->m_value.bValidContents = FALSE;
	}
	return pC->m_value;
}

APPCONTEXTFIELD & CContexts::GetAddContextField(LPCTSTR pszContext, LPCTSTR pszContextField)
{
   Trace trace(_T("CContexts::GetAddContextField"), false);
	CCriticalSectionLock lock(m_cs, TRUE);	// we won't block if we're coming from the same thread
	APPCONTEXT & ac = GetAddContext(pszContext);

	//
	// See if the context/field pair it's been added. If it is - don't add it again
	//
	PAPPCONTEXTFIELDMAPITEM pF = ac.fieldMap.Lookup(pszContextField);

	if( !pF )
	{
		// create the field parameter and populate it
		trace(DCM_INFO) 
			<< _T("Adding ContextField '") << pszContext
			<< _T("/") << pszContextField
			<< _T("' to map.") << NCR::Util::endl;

		APPCONTEXTFIELD & field = ac.fieldMap[pszContextField];
		pF = ac.fieldMap.Lookup(pszContextField);

		field.fieldName = pszContextField;
		field.strLastSavedValue = _T("");
	}
	return pF->m_value;
}



bool CContexts::IsContextApplicable(BSTR contextName)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	return m_contextMap.Lookup(contextName) != NULL;
}

//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
//instead use the VARIANT pointer directly
bool CContexts::SaveContext(const BSTR contextName, const VARIANT& contextData)
{
	Trace trace(_T("CContexts::SaveContext"));
	CCriticalSectionLock lock(m_cs, TRUE);	
	// Get the saved context info
	PAPPCONTEXTMAPITEM pC = NULL;
	PAPPCONTEXTFIELDMAP * pF = NULL;
	bool bRet = false;
	CT2A aContextName(contextName);

	pC = m_contextMap.Lookup(contextName);

	if( pC )
	{
		// loop through the required fields and store their new values.
		APPCONTEXT& saveCtx = pC->m_value;

		// set last save timestamp
		saveCtx.dwLastSavedTime = GetTickCount();
		saveCtx.bValidContents = TRUE;

		SAFEARRAY* pSA = contextData.parray;
		SAFEARRAYBOUND indices[1] = {0};
		indices[0].lLbound = 0;
		indices[0].cElements = DkGetContextFieldCount(aContextName);
		long aLong[1] = {0};

		VARIANT var;
		VariantInit(&var);

		// save the field values if there are any
		PAPPCONTEXTFIELDMAPITEM pF = NULL;
		POSITION pos = saveCtx.fieldMap.GetStartPosition();

		while( pos && (pF = saveCtx.fieldMap.GetNext(pos)) )
		{
			APPCONTEXTFIELD& field = pF->m_value;
			CT2A aFieldName(field.fieldName);
			
			VARTYPE type = VT_EMPTY;
			DWORD index = UINT_MAX;
			
			if (DkGetContextFieldInfoEx(aContextName, aFieldName, &type, NULL, NULL, &index))
			{
				// field.strLastSavedValue = ctx.GetFieldAsString(field.fieldName);
            aLong[0] = index;
            SafeArrayGetElement(pSA, aLong, &var);

				_bstr_t bValue = var;
				field.strLastSavedValue = (LPCWSTR)bValue;
			}

         if (trace.IsLevel(DCM_DEBUG))
         {
			   trace(DCM_DEBUG)
					<< _T("[Context:") << contextName
					<< _T("][Field:") << field.fieldName.GetString()
					<< _T("] Stored value '") << field.strLastSavedValue.GetString()
					<< _T("'.") << NCR::Util::endl;
         }

			VariantClear(&var);
		}

		bRet = true;
	}
	else
	{
		trace(DCM_ERROR)
			<< _T("The ") << contextName
			<< _T(" context was not found in the Context Field map.") << NCR::Util::endl;
	}

	return bRet;
}

const PAPPCONTEXTFIELD CContexts::GetContextField(LPCTSTR pszContext, LPCTSTR pszContextField)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	PAPPCONTEXTMAPITEM pC = m_contextMap.Lookup(pszContext);
	if( pC )
	{
		PAPPCONTEXTFIELDMAPITEM pF = pC->m_value.fieldMap.Lookup(pszContextField);
		
		return pF ? &pF->m_value : NULL;  
	}

	return NULL;
}

const PAPPCONTEXT CContexts::GetContext(LPCTSTR pszContext)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	PAPPCONTEXTMAPITEM pC = m_contextMap.Lookup(pszContext);
	
	return pC ? &pC->m_value : NULL;
}

bool CContexts::ResetContext(LPCTSTR pszContext)
{
   Trace trace(_T("CContexts::ResetContext"), false);
	trace(DCM_INFO) 
		<< _T("Resetting context '") << pszContext
		<< _T("'.") << NCR::Util::endl;

	CCriticalSectionLock lock(m_cs, TRUE);	
	PAPPCONTEXTMAPITEM pC = m_contextMap.Lookup(pszContext);
	if( pC )
	{
		pC->m_value.bValidContents = FALSE;
	}
	return (pC != NULL);
}

bool CContexts::ResetContexts(CSimpleArray<CString> & strContextNames)
{
   Trace trace(_T("CContexts::ResetContexts"), false);
	trace(DCM_INFO, _T("Resetting all contexts (except exclusions)."));
	CCriticalSectionLock lock(m_cs, TRUE);	

	bool bRet = true;
	for(int i = 0; i < strContextNames.GetSize(); i++)
	{
		PAPPCONTEXTMAPITEM pC = m_contextMap.Lookup(strContextNames[i]);
		if( pC )
		{
         if (trace.IsLevel(DCM_DEBUG))
         {
			   trace(DCM_DEBUG)
					<< _T("Resetting context: '") << strContextNames[i].GetString()
					<< _T("'") << NCR::Util::endl;
         }

			pC->m_value.bValidContents = FALSE;
		}
		else
		{
			trace(DCM_WARNING)
				<< _T("ResetContexts() Context '") << strContextNames[i].GetString()
				<< _T("' not found for resetting.") << NCR::Util::endl;
			bRet = false;
		}
	}
	return bRet;
}

void CContexts::ResetAllContexts(void)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	
	POSITION pos = m_contextMap.GetStartPosition();
	APPCONTEXTMAPITEM * pcmi = NULL;
	while( pos && (pcmi = m_contextMap.GetNext(pos)) )
	{
		pcmi->m_value.bValidContents = FALSE;
	}
}