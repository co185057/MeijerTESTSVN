#include <stdafx.h>
//#include <afxpriv.h>                // for USES_CONVERSION

#include "DeviceCtxStoreXML.h"
#include "DevicePropertyChanges.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_SM
#define T_ID	_T("CDeviceCtxStoreXml")

CDeviceCtxStoreXml::CDeviceCtxStoreXml(const CString& csFile)
	: m_csFileName(csFile)
{
	GET_PARTITION_PATH(m_csFileName, m_csFileName.GetBuffer(_MAX_PATH));
	m_csFileName.ReleaseBuffer();

	m_spList = NULL;
	m_spAttr = NULL;
	m_spAttrList = NULL;
	m_spNode = NULL;
	m_spDoc = NULL;
	// Load();  //Do not load on creation, may not be CoInitialized yet
}

CDeviceCtxStoreXml::CDeviceCtxStoreXml()
	: m_csFileName(DEVICECONTEXTS_FILE_DEFAULT)
{
	GET_PARTITION_PATH(m_csFileName, m_csFileName.GetBuffer(_MAX_PATH));
	m_csFileName.ReleaseBuffer();

	m_spList = NULL;
	m_spAttr = NULL;
	m_spAttrList = NULL;
	m_spNode = NULL;
	m_spDoc = NULL;
}

bool CDeviceCtxStoreXml::SetFileName(const CString& csFile)
{
	bool bRetValue = false;
	
	if ((m_csFileName != csFile) || (m_spDoc == NULL))
	{
		m_csFileName = csFile;
		
		GET_PARTITION_PATH(m_csFileName, m_csFileName.GetBuffer(_MAX_PATH));
		m_csFileName.ReleaseBuffer();
		
		// check whether the file exists, if does not exist, return false
		bRetValue = Load();
	}
	return bRetValue;
}


bool CDeviceCtxStoreXml::Load()
{
	bool bRetValue = false;
	try
	{
		// get the filepath from scotopts.dat
		HRESULT hr = m_spDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
		if( FAILED(hr) || m_csFileName.IsEmpty() )
		{
			// throw exception - in constructor a little dangerous though :(
			return bRetValue;
		}

		// load the xml into the m_spDoc
		_variant_t varOut((bool)TRUE);
		varOut =  m_spDoc->load((LPCTSTR)m_csFileName);

		if ((bool)varOut == FALSE)
		{// show error description - IXMLDOMParseError sample
			IXMLDOMParseErrorPtr errPtr = m_spDoc->GetparseError();
			// print error details
		}
		else
		{
			IXMLDOMDocumentTypePtr docTypPtr = m_spDoc->doctype;
			if (docTypPtr)
			{
				_bstr_t bstrTyp(docTypPtr->name);
				_tprintf(_T("Document type name = %s\n"), (TCHAR*)bstrTyp);
			}
			bRetValue = true;
		}
		// initialize the context to nothing
		m_csContextName.Empty();
	}
	catch (_com_error &e)
	{
		_tprintf(_T("Error:\n"));
		_tprintf(_T("Code = %08lx\n"), e.Error());
		_tprintf(_T("Code meaning = %s\n"), (TCHAR*) e.ErrorMessage());
		_tprintf(_T("Source = %s\n"), (TCHAR*) e.Source());
		_tprintf(_T("Error Description = %s\n"), (TCHAR*) e.Description());
	}
	catch(...)
	{
		_tprintf(_T("Unknown error!"));
	}
	return bRetValue;
}


bool CDeviceCtxStoreXml::SetCurrentContext(LPCTSTR pszCtxName)
{

	m_csContextName = pszCtxName;
	bool bRetValue = false;
	if (!m_csContextName.GetLength())
	{
		return bRetValue;
	}
	m_spList = NULL;
	m_spAttr = NULL;
	m_spAttrList = NULL;
	m_spNode = NULL;

	MSXML2::IXMLDOMNodePtr spNode = NULL;

	_bstr_t bstrQuery = OLESTR("DeviceContexts/DeviceContext[@Name='");
	bstrQuery += pszCtxName;
	bstrQuery += OLESTR("']");

	spNode = m_spDoc->selectSingleNode(bstrQuery);
	if( spNode != NULL )
	{
		m_bstrCtx = pszCtxName;
		bstrQuery += OLESTR("/DeviceProperty");
		m_spList = m_spDoc->selectNodes(bstrQuery);
		if (m_spList != NULL)
		{
			m_spList->reset();
			bRetValue = true;	
		}
	}
	return bRetValue;
}

bool CDeviceCtxStoreXml::GetAllDevicePropertyNames(CStringArray& csPropList)
{
	bool bRetValue = false;
	if (m_spList == NULL)
	{
		return bRetValue;	
	}
	long nLen = m_spList->Getlength();
	for (int i = 0; (i < nLen) ; ++i)
	{
		_bstr_t tmp = m_spList->item[i]->attributes->getNamedItem("Name")->text;
		csPropList.Add((LPCTSTR)tmp);
	}
	int nLen2 = csPropList.GetSize();
	if (nLen2 == nLen)
	{
		bRetValue = true;
	}
	else
	{
		csPropList.RemoveAll();
	}
	return bRetValue;
}

bool CDeviceCtxStoreXml::SetCurrentDeviceProperty(LPCTSTR pszDevicePropName)
{
	bool bRetValue = false;
	if ( (m_spList == NULL) || (!pszDevicePropName) || (!_tcslen(pszDevicePropName)))
	{
		return bRetValue;	
	}
	long nLen = m_spList->Getlength();
	for (int i = 0; ((i < nLen) && !bRetValue)  ; ++i)
	{
		_bstr_t tmp = m_spList->item[i]->attributes->getNamedItem("Name")->text;
		if (CString((LPCTSTR) tmp) == pszDevicePropName)
		{
			bRetValue = true;
			m_spAttrList = m_spList->item[i]->attributes;
		}
	}


	return bRetValue;
}


bool CDeviceCtxStoreXml::GetAllItemsForCurrentDevicePropetry(key_value_pair_t& ItemPairs)
{
	bool bRetValue = false;
	if (m_spAttrList == NULL)
	{
		return bRetValue;
	}
	long nLen = m_spAttrList->Getlength();
	for (int i = 0; (i < nLen) ; ++i)
	{
		_bstr_t tmpName;
		_bstr_t tmpValue;
		try
		{
			tmpName = m_spAttrList->item[i]->nodeName;
		}
		catch(...)
		{
			ASSERT(0);
		}
		try
		{
			tmpValue = m_spAttrList->item[i]->text;
		}
		catch(...)
		{
			ASSERT(0);
		}
		if (CString((LPCTSTR) tmpName) != _T("Name"))
		{
			ItemPairs.SetAt((LPCTSTR) tmpName, (LPCTSTR) tmpValue);
		}
	}
	if (ItemPairs.GetCount())
	{
		bRetValue = true;
	}
	return bRetValue;
}

// private member
bool CDeviceCtxStoreXml::GetAllItemsForThisDevicePropetry(LPCTSTR pszDevicePropName, key_value_pair_t& ItemPairs)
{
	bool bRetValue = false;
	if ((!pszDevicePropName) || (!_tcslen(pszDevicePropName)))
	{
		return bRetValue;	
	}
	bRetValue = SetCurrentDeviceProperty(pszDevicePropName);
	bRetValue = GetAllItemsForCurrentDevicePropetry(ItemPairs);

	return bRetValue;
}


bool CDeviceCtxStoreXml::GetDevicePropertyItemValue(LPCTSTR pszDevicePropItemName, CString& csDevicePropItemValue)
{
	bool bRetValue = false;
	if (m_spAttrList == NULL)
	{
		return bRetValue;
	}
	long nLen = m_spAttrList->Getlength();
	for (int i = 0; ((i < nLen) && !bRetValue)  ; ++i)
	{
		_bstr_t tmpName;
		_bstr_t tmpValue;
		try
		{
			tmpName = m_spAttrList->item[i]->nodeName;
		}
		catch(...)
		{
			ASSERT(0);
		}
		if (CString((LPCTSTR) tmpName) == pszDevicePropItemName)
		{
			try
			{
				tmpValue = m_spAttrList->item[i]->text;
			}
			catch(...)
			{
				ASSERT(0);
			}
			csDevicePropItemValue = (LPCTSTR) tmpValue;
			bRetValue = true;
		}
	}
	return bRetValue;
}


bool CDeviceCtxStoreXml::SetDevicePropertyItemValue(LPCTSTR pszDevicePropItemName, LPCTSTR pszDevicePropItemValue)
{
	bool bRetValue = false;
	if (m_spAttrList == NULL)
	{
		return bRetValue;
	}
	long nLen = m_spAttrList->Getlength();
	for (int i = 0; ((i < nLen) && !bRetValue)  ; ++i)
	{
		_bstr_t tmpName;
		_bstr_t tmpValue;
		try
		{
			tmpName = m_spAttrList->item[i]->nodeName;
		}
		catch(...)
		{
			ASSERT(0);
		}
		if (CString((LPCTSTR) tmpName) == pszDevicePropItemName)
		{
			try
			{
				m_spAttrList->item[i]->text = (LPCTSTR) pszDevicePropItemValue;
			}
			catch(...)
			{
				ASSERT(0);
			}
			bRetValue = true;
		}
	}
	return bRetValue;
}




bool CDeviceCtxStoreXml::UnLoad()
{
	// delete current context if any
	m_csFileName = DEVICECONTEXTS_FILE_DEFAULT;
	GET_PARTITION_PATH(m_csFileName, m_csFileName.GetBuffer(_MAX_PATH));
	m_csFileName.ReleaseBuffer();
	
	bool bRetValue = false;

	return bRetValue;
}






CDeviceCtxStore::CDeviceCtxStore()
{

}

CDeviceCtxStore::~CDeviceCtxStore()
{

}

CDeviceCtxStoreXml::~CDeviceCtxStoreXml()
{
}


