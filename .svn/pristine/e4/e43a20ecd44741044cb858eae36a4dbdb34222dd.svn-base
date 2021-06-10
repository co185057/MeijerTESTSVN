// FLDeviceList.cpp: implementation of the CFLDeviceList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FLDeviceList.h"
#include "log.h"
#include <string>
#include <list>
#include "FastlaneUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DEFAULT_DEVLISTXMLFILE _T("%APP_DRIVE%\\scot\\config\\FLDeviceList.xml")
#define DEFAULT_DEVICERRPATH _T("%APP_DRIVE%\\scot\\data\\deviceerror\\")
#define DEFAULT_IMAGEPATH _T("%APP_DRIVE%\\scot\\image\\")
#define DEFAULT_CONFIGPATH _T("%APP_DRIVE%\\scot\\config\\")
#define DEFAULT_VALIDATE_SCHEMA _T("%APP_DRIVE%\\scot\\data\\deviceerror\\DeviceErrorMapping.xsd")

const TCHAR XML_DEVICELIST[] = _T("//DeviceListConfiguration//DeviceList//Device-Class");
const TCHAR XML_DEVICELISTLCID[] = _T("//DeviceListConfiguration//Languages//Language");
const TCHAR XML_DEVICERRPATH[] = _T("//DeviceListConfiguration//DefaultPath");
const TCHAR XML_IMAGEPATH[] = _T("//DeviceListConfiguration//ImagePath");
const TCHAR XML_CONFIGPATH[] = _T("//DeviceListConfiguration//ConfigPath");
const TCHAR XML_VALIDATE_XML[] = _T("//DeviceListConfiguration//ValidateXML");
const TCHAR XML_VALIDATE_SCHEMA[] = _T("//DeviceListConfiguration//SchemaForErrMapping");
const TCHAR XML_ID[] = _T("id");
const TCHAR XML_CLASSNAME[] = _T("Class-Name");
const TCHAR XML_ERROR_CONFIG_FILE[] = _T("Device-Error-File");
const TCHAR XML_LCID[] = _T("LCID");
const TCHAR XML_HTMLPATH[] = _T("Html-Path");
const TCHAR XML_CONFIGURE_LANG[]=_T("//Display");
const TCHAR CONFIGURE_LANG[]=_T("Language");

extern CLog* g_logger;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFLDeviceList::CFLDeviceList()
{
	m_bValidateXML = false;		

}

CFLDeviceList::~CFLDeviceList()
{
	m_aDeviceList.clear();
}

//************************************************************************ 
//
// Function:		GetDeviceListFile
// Purpose:			Read FLDeviceList.xml from registry	
// Parameters:		[out]  sConfigFile holds device list xml file
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceList::GetDeviceListFile(_bstr_t & sConfigFile)
{

	HKEY hKey;
	TCHAR sDeviceListConfigFile[_MAX_PATH];

	if( ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\DeviceErrorLookUp"),
				 0, KEY_QUERY_VALUE, &hKey))
	{
		// Get the configuration value
		DWORD dwValueSize=100 * sizeof(TCHAR);
		_TCHAR  szValue[101];

		memset(szValue, 0, 101);
		if( ERROR_SUCCESS == RegQueryValueEx(hKey, _T("DeviceListFile"), NULL, NULL, (LPBYTE)szValue, &dwValueSize))
		{
		   GET_PARTITION_PATH(szValue, sDeviceListConfigFile);
		}
		else
		{
			GET_PARTITION_PATH(DEFAULT_DEVLISTXMLFILE,sDeviceListConfigFile);
		}

		::RegCloseKey(hKey);

	}
	else
	{
		GET_PARTITION_PATH(DEFAULT_DEVLISTXMLFILE,sDeviceListConfigFile);
	}

	sConfigFile = sDeviceListConfigFile;

	return true;
	
}

//************************************************************************ 
//
// Function:		LoadDeviceList
// Purpose:			Read FLDeviceList.xml and build the device list	m_aDeviceList
// Parameters:		[in]  sFileName -  device list xml file to read
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceList::LoadDeviceList(_bstr_t & sFileName)
{
	bool bRet=false;
	g_logger->Log(TRACE_API, _T("+CFLDeviceList::LoadDeviceList() with file=%s"), (LPTSTR)sFileName);


	MSXML2::IXMLDOMDocumentPtr spDoc;
    HRESULT hr;

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??") );
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::LoadDeviceList()"));
        return false;
    }

	FL_DeviceClass flDeviceClass;

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
    	
	
	if( spDoc->load( sFileName ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("Failed to load file %s"), sFileName);
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::LoadDeviceList("));
        spDoc.Release();
		return false;
	}

	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList;
	MSXML2::IXMLDOMNodePtr spNode, spItem, spPath, spImage, spConfig, spValidateXML,spSchema;

	_bstr_t sValidateXML=_T("");
	

	//DeviceListConfiguration//DefaultPath
	spPath=spDoc->selectSingleNode(XML_DEVICERRPATH);
	if (spPath != NULL)
	{
		 GET_PARTITION_PATH((LPCTSTR)spPath->text, m_sFLDevErrPath);	
	}
	else
		GET_PARTITION_PATH(DEFAULT_DEVICERRPATH, m_sFLDevErrPath);
		


	//DeviceListConfiguration//ImagePath
	spImage=spDoc->selectSingleNode(XML_IMAGEPATH);
	if (spImage != NULL)
	{
	
		GET_PARTITION_PATH((LPCTSTR)spImage->text, m_sFLImagePath);

	}
	else
		GET_PARTITION_PATH(DEFAULT_IMAGEPATH, m_sFLImagePath);


	//DeviceListConfiguration//ConfigPath
	spConfig=spDoc->selectSingleNode(XML_CONFIGPATH);
	if (spConfig != NULL)
	{
		GET_PARTITION_PATH((LPCTSTR)spConfig->text, m_sFLConfigPath);
	}
	else
		GET_PARTITION_PATH(DEFAULT_CONFIGPATH, m_sFLConfigPath);

	
	//DeviceListConfiguration//ValidateXML
	spValidateXML=spDoc->selectSingleNode(XML_VALIDATE_XML);
	if (spValidateXML != NULL)
	{
		 sValidateXML = spValidateXML->text;
		 if (_tcscmp((LPTSTR)sValidateXML, _T("true")) == 0 ||
			 _tcscmp((LPTSTR)sValidateXML, _T("True")) == 0 ||
			 _tcscmp((LPTSTR)sValidateXML, _T("TRUE")) == 0)
			 m_bValidateXML = true;
		 
	}
	
	//DeviceListConfiguration//SchemaForErrMapping
	spSchema=spDoc->selectSingleNode(XML_VALIDATE_SCHEMA);
	if (spSchema != NULL)
	{
		GET_PARTITION_PATH((LPCTSTR)spSchema->text, m_sXSDFile);
	}
	else
		GET_PARTITION_PATH(DEFAULT_VALIDATE_SCHEMA, m_sXSDFile);

	

	//DeviceListConfiguration//DeviceList//Device-Class
	spNodeList = spDoc->selectNodes(XML_DEVICELIST);	
	
	while(  spNode = spNodeList->nextNode())
	{
		flDeviceClass.reset();

		bstr_t bstrAttName = spNode->GetnodeName();
		

		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			flDeviceClass.SetClassID(_ttoi((LPCTSTR)spItem->text));
		}
		
		spItem = spNode->selectSingleNode( XML_CLASSNAME );
		if (spItem != NULL)
		{
			flDeviceClass.SetClassName(spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_ERROR_CONFIG_FILE );
		if (spItem != NULL)
		{
			flDeviceClass.SetErrorXML(spItem->text);

			if (m_bValidateXML == true)
			{
				_bstr_t sDevMappingXML=m_sFLDevErrPath + flDeviceClass.GetErrorXML();

				bRet = ValidateDeviceErrorMappingXML(sDevMappingXML, m_sXSDFile);
				if (bRet == false)
				{
					break;	
				}
					
			}
		}
		g_logger->Log(TRACE_INFO, _T("CFLDeviceList::LoadDeviceList()- Add device class %d "),flDeviceClass.GetClassID() );
		m_aDeviceList.push_back(flDeviceClass);
	}

	spDoc.Release();
	
	if (m_aDeviceList.size() != 0)
		bRet = true;

	g_logger->Log(TRACE_API, _T("-CFLDeviceList::LoadDeviceList() "));
    return bRet;

}

//************************************************************************ 
//
// Function:		ValidateDeviceErrorMappingXML
// Purpose:			validate all error mapping xml against schema	
// Parameters:		[in]  sXML - file to be validated
//					[in]  sXSDFile - schema file
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceList::ValidateDeviceErrorMappingXML(LPTSTR sXML, LPTSTR sXSDFile)
{
	bool bRet=false;
	g_logger->Log(TRACE_API, _T("+CFLDeviceList::ValidateDeviceErrorMappingXML() with sXML=%s"), sXML);


	IXMLDOMDocument2Ptr pIXMLDOMDocument2;
    IXMLDOMSchemaCollection2Ptr pIXMLDOMSchemaCollection2Ptr;

    try
    {

	MSXML2::IXMLDOMDocument2Ptr spDoc;
	MSXML2::IXMLDOMSchemaCollection2Ptr spCollection;
    HRESULT hr;

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??") );
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML()"));
		spDoc.Release();
        return false;
    }

    // Create the Schema Collections
     hr = spCollection.CreateInstance(__uuidof(MSXML2::XMLSchemaCache60));
	 if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create schema collection instance") );
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML()"));
		spCollection.Release();
        return false;
    }
      
    // Add the schema to the collection
     hr = spCollection->add(_T(""), sXSDFile);
	if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to add schema to shema collection") );
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML()"));
		spCollection->Release();
        return false;
    }    

    // Attach schemas
     spDoc->schemas = spCollection.GetInterfacePtr();
     spDoc->async = false;
     spDoc->validateOnParse = true;


    // Load the document into the DOM
     hr  = spDoc->load(sXML);
    
	 if(hr != -1)
	 {
			g_logger->Log(TRACE_ERROR, _T("Failed to validate xml") );
			g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML()"));
			return false;
	 }
	 else
		 bRet = true;

	 spDoc.Release();

       
    } catch(...)
    {
        g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML() exception"));
    }

	g_logger->Log(TRACE_API, _T("-CFLDeviceList::ValidateDeviceErrorMappingXML()"));
	
	return bRet;
}


//************************************************************************ 
//
// Function:		GetDeviceErrorXML
// Purpose:			Get the DeviceErrorXXXXEnhanced.xml file name and class name		
// Parameters:		device class 	
// Returns:			true or false
// Calls:						
// Side Effects:
// Notes: Go through m_aDeviceList to find the class name and enhanced xml name
//
//************************************************************************

 bool CFLDeviceList::GetDeviceErrorXML(int DeviceClass, _bstr_t& sXML,  _bstr_t& sClassName)
{
    g_logger->Log(TRACE_API, _T("+CFLDeviceList::GetDeviceErrorXML() with DevClass=%d"), DeviceClass);
	bool bRet=false;
	FL_DeviceClass flItem;

	std::list <FL_DeviceClass>::iterator Iter;

	for ( Iter = m_aDeviceList.begin( ); Iter != m_aDeviceList.end( ); Iter++ )
	{
		flItem = (FL_DeviceClass)(*Iter);
		if (DeviceClass == flItem.GetClassID())
		{			
			sXML =  flItem.GetErrorXML();			
			sClassName=flItem.GetClassName();
			if (_tcslen(sXML) != 0)
				bRet=true;
			break;
		}
	}

	g_logger->Log(TRACE_API, _T("-CFLDeviceList::GetDeviceErrorXML() return %s"), (LPTSTR)sXML);
	return bRet;
}

 //************************************************************************ 
//
// Function:		UnLoadDeviceList
// Purpose:					
// Parameters:		 	
// Returns:			true 
// Calls:						
// Side Effects:
// Notes: Go through m_aDeviceList to find the class name and enhanced xml name
//
//************************************************************************
bool CFLDeviceList::UnLoadDeviceList()
{
//	m_aDeviceList.clear();
	return true;
}


