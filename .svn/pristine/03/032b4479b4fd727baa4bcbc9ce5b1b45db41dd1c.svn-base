// Change history:
//
// POS14165 Work Request:13752 Name:Robert Susanto Date:May 23, 2011

#include "StdAfx.h"
#include "Controls.h"
#include "ACSContext.h"
#include "Contexts.h"
#include "Properties.h"
#include "Utils.h"
#include "TraceImpl.h"
#include "ACSIODataKit.h"

CControls::CControls(void)
{
	CCriticalSectionLock lock(m_cs, TRUE);
	m_controlMap.RemoveAll();
}

CControls::~CControls(void)
{

}

CONTROL & CControls::GetAddControl(LPCTSTR pszControl)
{
   Trace trace(_T("CControls::GetAddControl"), false);

	//
	// Fetch control entry, if it doesn't exist, create it.
	//
	PCONTROLMAPITEM pcmi = m_controlMap.Lookup(pszControl);
	if( !pcmi )
	{
		trace(DCM_INFO)
			<< _T("Adding Control '")
			<< pszControl
			<< _T("' to map.") << NCR::Util::endl;

		m_controlMap[pszControl];
		pcmi = m_controlMap.Lookup(pszControl);
		_ASSERT(pcmi);
		pcmi->m_value.dwLastSavedTime = 0;
		pcmi->m_value.dwLastSentTime = 0;
		pcmi->m_value.fieldMap.RemoveAll();
		pcmi->m_value.spGenCtrl = NULL;
		HRESULT hr = pcmi->m_value.spGenCtrl.CreateInstance(__uuidof(TBC::CTBGenObj));
		if( SUCCEEDED(hr) )
		{
			try
			{
				pcmi->m_value.spGenCtrl->Name = pszControl;
				pcmi->m_value.spGenCtrl->IsXMLOutput = (BYTE)FALSE;
				pcmi->m_value.spGenCtrl->Validate = (BYTE)TRUE;
			}
			catch(_com_error & cex)
			{
				trace(DCM_ERROR)
					<< _T("An exception occured while setting TBGenObj object members for '") << pszControl
					<< _T("' control. Error [0x") << std::hex << cex.Error() << std::dec 
					<< _T("]: ") << cex.ErrorMessage() << NCR::Util::endl;
			}
		}
		else
		{
			trace(DCM_ERROR)
				<< _T("Failed to create TBGenObj object for '") << pszControl
				<< _T("' control. Error [0x") << std::hex << hr << std::dec
				<< _T("]") << NCR::Util::endl;
		}
	}
	
	return pcmi->m_value;
}

CONTROLFIELD & CControls::GetAddControlField(LPCTSTR pszControl, LPCTSTR pszField)
{
   Trace trace(_T("CControls::GetAddControlField"), false);

	CONTROL & ctrl = GetAddControl(pszControl);
	//
	// Fetch control field entry, if it doesn't exist, create it.
	//
	PCONTROLFIELDMAPITEM pcfmi = ctrl.fieldMap.Lookup(pszField);
	if( !pcfmi )
	{
		trace(DCM_INFO)
			<< _T("Adding ControlField '") << pszControl
			<< _T("/") << pszField
			<< _T("' to map.") << NCR::Util::endl;

		ctrl.fieldMap[pszField];
		pcfmi = ctrl.fieldMap.Lookup(pszField);
		_ASSERT(pcfmi);
		pcfmi->m_value.strLastSavedValue = _T("");
		// pcfmi->m_value.bindingMap.RemoveAll();
	}
	
	return pcfmi->m_value;
}


const PCONTROLFIELD CControls::GetControlField(LPCTSTR pszControl, LPCTSTR pszControlField)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	PCONTROLMAPITEM pC = m_controlMap.Lookup(pszControl);
	if( pC )
	{
		PCONTROLFIELDMAPITEM pF = pC->m_value.fieldMap.Lookup(pszControlField);
		
		return pF ? &pF->m_value : NULL;  
	}

	return NULL;
}

const PCONTROL CControls::GetControl(LPCTSTR pszControl)
{
	CCriticalSectionLock lock(m_cs, TRUE);	
	PCONTROLMAPITEM pC = m_controlMap.Lookup(pszControl);
	
	return pC ? &pC->m_value : NULL;
}


void CControls::AddContextControlField(LPCTSTR pszControl, LPCTSTR pszField, LPCTSTR pszMapCtx, LPCTSTR pszMapCtxField)
{
	CCriticalSectionLock lock(m_cs, TRUE);

	CONTROLFIELD & field = GetAddControlField(pszControl, pszField);
   Utils.Contexts().AddMappedControl(pszMapCtx, pszMapCtxField, pszControl, pszField);
}


void CControls::AddPropertyControlField(LPCTSTR pszControl, LPCTSTR pszField, LPCTSTR pszMapProperty)
{
	CCriticalSectionLock lock(m_cs, TRUE);
	
	CONTROLFIELD & field = GetAddControlField(pszControl, pszField);
   Utils.Properties().AddMappedControl(pszMapProperty, pszControl, pszField);
}

//POS14165 don't use the _bstr_t and _variant_t as the _variant_t will make a variantcopy of the ContextData that is sensitive to memory corruption
//instead use the VARIANT pointer directly
void CControls::UpdateContextBoundControlFields(const BSTR contextName, const VARIANT& contextData)
{
   Trace trace(_T("CControls::UpdateContextBoundControlFields"), false);
	CCriticalSectionLock lock(m_cs, TRUE);

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

   CContexts::FieldControlListRange controlRange = Utils.Contexts().GetMappedControls((LPCTSTR)contextName);
   CContexts::FieldControlList::const_iterator iter = controlRange.first;
   CContexts::FieldControlList::const_iterator end = controlRange.second;

	/* Loop through the mapped controls to see if any context fields are relevant. */
   while (iter != end)
   {
      CString const& field = iter->first;
      CString const& control = iter->second.first;
      CString const& controlField = iter->second.second;

	   PCONTROLMAPITEM pcmi = m_controlMap.Lookup(control);

	   if (pcmi)
	   {
         CONTROL& ctrl = pcmi->m_value;
         PCONTROLFIELDMAPITEM pcfmi = ctrl.fieldMap.Lookup(controlField);

         if (pcfmi)
         {
			   CONTROLFIELD& cfield = pcfmi->m_value;
            PAPPCONTEXTFIELD pCtxField = Utils.Contexts().GetContextField(contextName, field);
      		
				/* If the current control field is mapped to a field in this context, extract the 
				value of the context field and assign it to the control. */
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
							/* Convert the VARIANT to a wide string and assign it to the control */
							_bstr_t bValue = var;
							cfield.strLastSavedValue = (LPCWSTR)bValue;

							/* update the last saved time */
							ctrl.dwLastSavedTime = GetTickCount();

							if (trace.IsLevel(DCM_DEBUG))
							{
								trace(DCM_DEBUG) 
									<< _T("[Control:'") << pcmi->m_key.GetString()
									<< _T("'][ControlField:'") << pcfmi->m_key.GetString()
									<< _T("'] Updating control from ContextField '") << contextName << _T("/") << field.GetString()
									<< _T("' to value '") << cfield.strLastSavedValue.GetString()
									<< _T("'.") << NCR::Util::endl;
							}
						}
						else
						{
							trace(DCM_ERROR) << L"control " << (PCWSTR)control 
								<< L" was not updated because SafeArrayGetElement failed with return code " 
								<< std::hex << getResult << std::dec << NCR::Util::endl;
						}
					}
		      }
			   else
			   {
				   trace(DCM_ERROR)
						<< _T("The ContextField '") << contextName << _T("/") << (LPCTSTR)field
						<< _T("' specified in the control was not found in the ContextMap") << NCR::Util::endl;
			   }
         }
			else
			{
				trace(DCM_ERROR)
					<< _T("The Control field '") << (LPCTSTR)control <<_T("/") << (LPCTSTR)controlField
					<< _T("' was not found in the control field map") << NCR::Util::endl;
			}
      }
		else
		{
			trace(DCM_ERROR)
				<< _T("The control '") << (LPCTSTR)control
				<< _T("' was not found in the control map") << NCR::Util::endl;
		}

      ++iter;
		VariantClear(&var);
   }
}

void CControls::UpdatePropertyBoundControlFields(LPCTSTR szPropName, LPCTSTR szPropValue)
{
   Trace trace(_T("CControls::UpdatePropertyBoundControlFields"), false);
	CCriticalSectionLock lock(m_cs, TRUE);

   CProperties::ControlFieldListRange contRange = Utils.Properties().GetMappedControls(szPropName);
   CProperties::ControlFieldList::const_iterator iter = contRange.first;
   CProperties::ControlFieldList::const_iterator end = contRange.second;

   while (iter != end)
   {
      CString const& control = iter->first;
      CString const& field = iter->second;

	   PCONTROLMAPITEM pcmi = m_controlMap.Lookup(control);

	   if (pcmi)
	   {
         CONTROL& ctrl = pcmi->m_value;
         PCONTROLFIELDMAPITEM pcfmi = ctrl.fieldMap.Lookup(field);

         if (pcfmi)
         {
			   CONTROLFIELD& cfield = pcfmi->m_value;

				// we found a match - so update it
				cfield.strLastSavedValue = szPropValue;
				// update the last saved time
				ctrl.dwLastSavedTime = GetTickCount();
				
            if (trace.IsLevel(DCM_DEBUG))
            {
					trace(DCM_DEBUG)
						<< _T("[Control:") << pcmi->m_key.GetString()
						<< _T("][ControlField:") << pcfmi->m_key.GetString()
						<< _T("] Updating control from Property '") << szPropName
						<< _T("' to value '") << cfield.strLastSavedValue.GetString()
						<< _T("'.") << NCR::Util::endl;
            }
         }
			else
			{
				trace(DCM_ERROR)
					<< _T("The Control field '") << (LPCTSTR)control
					<< _T("/") << (LPCTSTR)field
					<< _T("' was not found in the control field map") << NCR::Util::endl;
			}
      }
		else
		{
			trace(DCM_ERROR)
				<< _T("The Control '") << (LPCTSTR)control
				<< _T("' was not found in the control map") << NCR::Util::endl;
		}

      ++iter;
   }
}

bool CControls::ResetControl(LPCTSTR pszControl)
{
   Trace trace(_T("CControls::ResetControl"), false);
	trace(DCM_INFO) 
		<< _T("Resetting Control '") << pszControl
		<< _T("' field values.") << NCR::Util::endl;

	CCriticalSectionLock lock(m_cs, TRUE);	
	PCONTROLMAPITEM pcmi = m_controlMap.Lookup(pszControl);
	if( pcmi )
	{
		// iterate through each control field values and reset them to blank.
		CONTROL & ctrl = pcmi->m_value;
		PCONTROLFIELDMAPITEM pcfmi = NULL;
		POSITION posField = ctrl.fieldMap.GetStartPosition();
		while( posField && (pcfmi = ctrl.fieldMap.GetNext(posField)) )
		{
			CONTROLFIELD & field = pcfmi->m_value;
			field.strLastSavedValue = _T("");
		}
		return true;
	}
	
	return (pcmi != NULL);
}
