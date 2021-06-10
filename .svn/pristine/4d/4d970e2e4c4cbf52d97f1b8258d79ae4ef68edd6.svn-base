// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#include "StdAfx.h"
#include "Properties.h"
#include "ACSContext.h"
#include "Contexts.h"
#include "Controls.h"
#include "Utils.h"
#include "TraceImpl.h"
#include "ACSIODataKit.h"

CProperties::CProperties()
{
	CCriticalSectionLock lock(m_cs, TRUE);
	m_map.RemoveAll();
}

CProperties::~CProperties(void)
{
}


CString CProperties::Get(LPCTSTR pszName)
{
	CCriticalSectionLock lock(m_cs, TRUE);

	PPROPERTYMAPITEM ppmi = m_map.Lookup(pszName);

	return ppmi ? ppmi->m_value.strValue : _T("");
}

bool CProperties::Set(LPCTSTR pszName, LPCTSTR pszValue, LPCTSTR pszMapContext, LPCTSTR pszMapContextField)
{
    Trace trace(_T("CProperties::Set"), false);

	CCriticalSectionLock lock(m_cs, TRUE);

	CString strValue = DereferenceProperty(pszValue);

	PPROPERTYMAPITEM ppmi = m_map.Lookup(pszName);
	if( !ppmi )
	{
		m_map[pszName].strValue = (LPCTSTR)strValue;
		ppmi = m_map.Lookup(pszName);
		
      if (pszMapContext && pszMapContextField)
      {
         Utils.Contexts().AddMappedProperty(pszMapContext, pszMapContextField, pszName);
      }
	}
	else
	{
		ppmi->m_value.strValue = (LPCTSTR)strValue;

		// bindings should not be specified if the property had already been created.
		if( pszMapContext || pszMapContextField )
		{
			trace(DCM_WARNING) << _T("[Property:'") << pszName << _T("'] Context bindings have been specified for a Property that was previously created.") << NCR::Util::endl;
			_ASSERT(FALSE);
		}
	}
	Utils.Controls().UpdatePropertyBoundControlFields(pszName, strValue);
	return true;
}

//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
//instead use the VARIANT pointer directly
void CProperties::UpdateContextBoundProperties(const BSTR contextName, const VARIANT& contextData)
{
   Trace trace(_T("CProperties::UpdateContextBoundProperties"), false);

	/* ACSIODataKit requires an ACSII context name */
	CT2A aContextName(contextName);

	/* Obtain a pointer to the safe-array stored in contextData */
	SAFEARRAY* pSA = contextData.parray;
	SAFEARRAYBOUND indices[1] = {0};
	indices[0].lLbound = 0;
	indices[0].cElements = DkGetContextFieldCount(aContextName);
	long aLong[1] = {0};

	/* Initialize a VARIANT that will hold the field values */
	VARIANT var;
	VariantInit(&var);

   CContexts::FieldPropListRange propRange = Utils.Contexts().GetMappedProperties((LPCTSTR)contextName);
   CContexts::FieldPropList::const_iterator iter = propRange.first;
   CContexts::FieldPropList::const_iterator end = propRange.second;

	/* Loop through the mapped properties to see if any context fields are relevant. */
   while (iter != end)
   {
      CString const& field = iter->first;
      CString const& property = iter->second;

	   PPROPERTYMAPITEM ppmi = m_map.Lookup(property);

	   if (ppmi)
	   {
		   PROPERTY& prop = ppmi->m_value;
         PAPPCONTEXTFIELD pCtxField = Utils.Contexts().GetContextField(contextName, field);
   		
			/* If the current property field is mapped to a field in this context, extract the 
			value of the context field and assign it to the property. */
         if (pCtxField)
		   {
				CT2A aFieldName(field);
				
				VARTYPE type = VT_EMPTY;
				DWORD index = UINT_MAX;
				
				if (DkGetContextFieldInfoEx(aContextName, aFieldName, &type, NULL, NULL, &index))
				{
					aLong[0] = index;
					HRESULT getResult = SafeArrayGetElement(pSA, aLong, &var);

					if (SUCCEEDED(getResult))
					{
						/* Convert the VARIANT to a wide string and assign it to the property */
						_bstr_t bValue = var;
						prop.strValue = (LPCWSTR)bValue;

						if (trace.IsLevel(DCM_DEBUG))
						{
							trace(DCM_DEBUG)  
								<< _T("Updating Property '") << ppmi->m_key.GetString() 
								<< _T("' from ContextField '") << contextName 
								<< _T("/") << field.GetString() 
								<< _T("' to value '") << prop.strValue.GetString() << _T("'.") << NCR::Util::endl;
						}

						/* Since a property was changed - see if it affects any of the property bound control fields.*/
						Utils.Controls().UpdatePropertyBoundControlFields(ppmi->m_key, ppmi->m_value.strValue);
					}
					else
					{
						trace(DCM_ERROR) << L"Promperty " << (PCWSTR)property 
							<< L" was not updated because SafeArrayGetElement failed with return code " 
							<< std::hex << getResult << std::dec << NCR::Util::endl;
					}
				}
		   }
			else
			{
				trace(DCM_ERROR)
					<< _T("The ContextField '") << contextName << _T("/") << (LPCTSTR)field
					<< _T("' specified in the Property was not found in the ContextMap") << NCR::Util::endl;
			}
      }
		else
		{
			trace(DCM_ERROR)
				<< _T("The Property '") << (LPCTSTR)property
				<< _T("' was not found in the Property map") << NCR::Util::endl;
		}

      ++iter;
   }
}

CString CProperties::DereferenceProperty(const CString & strPropVal)
{
	// Used to dereference values from encapsulated properties
	// For example, assuming PropTest1 = "X", executing:
	// <SetProperty Property="PropTest2" Action="Set" Value="[[PropTest1]]" />
	// PropTest2's value would now also be X
	// This dereferencing applies to SetProperty supporting Actions "Set", "ConcatLeft" & "ConcatRight"
	Trace trace(_T("CProperties::DereferenceProperty"), false);
	// Dereference property value if value is within tokens. 
	if( (strPropVal.Left(XML_PROPREF_START_LEN) == XML_PROPREF_START) &&
		(strPropVal.Right(XML_PROPREF_END_LEN) == XML_PROPREF_END) )
	{
		int iPropLen = strPropVal.GetLength() - XML_PROPREF_START_LEN - XML_PROPREF_END_LEN;
		return Get(strPropVal.Mid(XML_PROPREF_START_LEN, iPropLen));
	}
	return strPropVal;
}


LPCTSTR CProperties::XML_PROPREF_START = _T("[[");
LPCTSTR CProperties::XML_PROPREF_END = _T("]]");
const long CProperties::XML_PROPREF_START_LEN = (long)_tcslen(CProperties::XML_PROPREF_START);
const long CProperties::XML_PROPREF_END_LEN = (long)_tcslen(CProperties::XML_PROPREF_END);
