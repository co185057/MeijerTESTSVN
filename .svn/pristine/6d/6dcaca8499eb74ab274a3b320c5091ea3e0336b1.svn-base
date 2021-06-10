// DeviceErrorLookUp.cpp : Defines the entry point for the DLL application.
//

#pragma warning(disable: 4786)

#include "stdafx.h"
#include "DeviceErrorLookUp.h"
#include "FLDeviceList.h"
#include "FLDeviceErrorMapping.h"
#include "FastlaneUtils.h"
#include "Log.h"

#define DEFAULT_DEVERRORHTMLFILE _T("%APP_DRIVE%\\scot\\data\\deviceerror\\DeviceErrorUnknown.htm")
#define DEFAULT_DEVERROR_ENHANCEDXML _T("%APP_DRIVE%\\scot\\data\\deviceerror\\DeviceErrorGeneralEnhanced.xml")
#define DEFAULT_DEVERRORPATH _T("%APP_DRIVE%\\scot\\data\\deviceerror")
static const LPCTSTR REGLOGBASE = _T("SOFTWARE\\NCR\\DeviceErrorLookUp");

const TCHAR XML_HTML_DIV[] = _T("//HTML//BODY//div");
const TCHAR XML_HTML_BODY[] = _T("//HTML//BODY");
const TCHAR XML_ID[] = _T("id");
const TCHAR XML_HTML_TEXT[] = _T("//HTML//BODY//div//p");
const TCHAR XML_HTML_TITLE2[] = _T("//HTML//BODY//div//div");


const TCHAR XML_CLASS[] = _T("class");

TCHAR   sCurrentTitle[_MAX_PATH];
TCHAR   sCurrentTitle2[_MAX_PATH];
TCHAR sCurrentText[3*_MAX_PATH];
TCHAR   sCurrentButton1Text[_MAX_PATH];


#pragma warning(disable: 4786)

//+SSCOP-3334

/*------------------------------------------------------------------------------------*
 * RemoveAllChildrenNode is a recursive static function that will traverse and remove *
 *               the children of the aNodeParam then finally remove aNodeParam.       *
 * Parameter:                                                                         *
 *   aParentNode - Parent Node of aNodeParam                                          *
 *   aNodeParam - Node whose children will be deleted (including itself)              *
 *------------------------------------------------------------------------------------*/
void RemoveChildNode(MSXML2::IXMLDOMNodePtr aParentNode, MSXML2::IXMLDOMNodePtr aNodeParam)
{
    MSXML2::IXMLDOMNodeListPtr spNodeList;
    MSXML2::IXMLDOMNodePtr spNode;

    if (aParentNode != NULL && aNodeParam != NULL)
    {
        spNodeList = aNodeParam->childNodes;
        if (spNodeList != NULL)
        {
            while(spNode = spNodeList->nextNode())
            {
                if (spNode != NULL)
                {
                    RemoveChildNode(aNodeParam, spNode);
                }
            }
        }
        aParentNode->removeChild(aNodeParam);
    }
}

/*------------------------------------------------------------------------------------*
 * RemoveAllChildrenNode is a static function that remove all children nodes. The     *
 *               function will get all children nodes and will call a recursive       *
 *               function to remove the grandchildren as well as the child itself.    *
 * Parameter:                                                                         *
 *   aNode - Node whose children will be deleted                                      *
 *------------------------------------------------------------------------------------*/
void RemoveAllChildrenNode(MSXML2::IXMLDOMNodePtr aNode)
{
    MSXML2::IXMLDOMNodeListPtr spNodeList;
    MSXML2::IXMLDOMNodePtr spNode;

    if (aNode != NULL)
    {
        spNodeList = aNode->childNodes;
        if (spNodeList != NULL)
        {
            while(spNode = spNodeList->nextNode())
            {
                if (spNode != NULL)
                {
                    RemoveChildNode(aNode, spNode);
                }
            }
        }
    }
}

/*------------------------------------------------------------------------------------*
 * AppendTextNode is a static function that will append the aStrToAdd to aParentNode. *
 *               An Element Node of <br> is appended to aParentNode first if the      *
 *               bCreateBRElement is set to true.                                     *
 * Parameters:                                                                        *
 *   anXMLDoc - Object pointer for the XML where elements will be added.              *
 *   aParentNode - Node where the text and element nodes will be added                *
 *   aStrToAdd - String that will be added                                            *
 *   csToReplace - Text to search that separates the token                            *
 *------------------------------------------------------------------------------------*/
void AppendTextNode(MSXML2::IXMLDOMDocumentPtr anXMLDoc, MSXML2::IXMLDOMNodePtr aParentNode, 
                       TCHAR *aStrToAdd, bool bCreateBRElement)
{
    MSXML2::IXMLDOMTextPtr txtNode;    
    MSXML2::IXMLDOMElementPtr elemNode;

    if (anXMLDoc != NULL && aParentNode != NULL && aStrToAdd != NULL)
    {
        if (bCreateBRElement == true)
        {
            elemNode = anXMLDoc->createElement(_T("br"));
            aParentNode->appendChild(elemNode);
        }
        txtNode = anXMLDoc->createTextNode(_bstr_t(aStrToAdd));
        aParentNode->appendChild(txtNode);            
    }
}

/*------------------------------------------------------------------------------------*
 * PutLineBreaks is a static function that will attempt to tokenize csStr and append  *
 *               to aParentNode. The function will search for instances of            *
 *               csToReplace and append the token to aParentNode. For 2nd child       *
 *               onwards, an Element Node of <br> is appended to aParentNode first    *
 *               before the token is added as text node. Appending is done by calling *
 *               the AppendTextNode function.                                         *
 * Parameters:                                                                        *
 *   anXMLDoc - Object pointer for the XML where elements will be added.              *
 *   aParentNode - Node where the text and element nodes will be added                *
 *   csStr - Text that will be tokenized                                              *
 *   csToReplace - Text to search that separates the token                            *
 *------------------------------------------------------------------------------------*/
 void PutLineBreaks(MSXML2::IXMLDOMDocumentPtr anXMLDoc, MSXML2::IXMLDOMNodePtr aParentNode, 
                   TCHAR *csStr, TCHAR *csToReplace)
{
    int iTextPartitionCount = 0;
    TCHAR *tmp=NULL;
    TCHAR *strToAdd=NULL;

    if (csStr != NULL && csToReplace != NULL)
    {
        while (tmp = _tcsstr(csStr, csToReplace))         //while the search str can still be found
        {
            strToAdd = new TCHAR[tmp-csStr+1];       //Allocate enough characters from start of the string
            strToAdd[0] = _T('\0');                  //to the loc of search string. Make sure to make the str NULL 
            _tcsncat(strToAdd, csStr, tmp-csStr);    //terminated before concatenating part of csStr to strToAdd
            AppendTextNode(anXMLDoc, aParentNode, strToAdd, 
                           (iTextPartitionCount > 0 && aParentNode->hasChildNodes()));
            delete[] strToAdd;                        //Deallocate memory allocated
            iTextPartitionCount++;
            csStr = tmp + _tcslen(csToReplace);
        }
        if (_tcslen(csStr) > 0)                       //Check if there are still characters to be added
        {
            strToAdd = new TCHAR[_tcslen(csStr)+1];   //Must follow same as above, only that the entire
            strToAdd[0] = _T('\0');                   //csStr will be added as a text node
            _tcsncat(strToAdd, csStr, _tcslen(csStr));
            AppendTextNode(anXMLDoc, aParentNode, strToAdd, 
                           (iTextPartitionCount > 0 && aParentNode->hasChildNodes()));
            delete[] strToAdd;
            iTextPartitionCount++;
        }
    }
}
 //-SSCOP-3334

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


CLog *g_logger = NULL;
CFLDeviceList *g_DeviceList=NULL;
long g_nTerminalNumber=-1;

//************************************************************************ 
//
// Function:		DeviceErrorInitialize
// Purpose:			Read devicielist config file and the Load FLDeviceList.xml		
// Parameters:
// Returns:			true or false
// Calls:			CFLDeviceList::GetDeviceListFile()
//					CDeviceList::LoadDeviceList()
//					Initialize log object
// Side Effects:
// Notes:
//
//************************************************************************

DEVICEERRORLOOKUP_API bool DeviceErrorInitialize()
{
	CoInitialize(NULL);
	bool bRet = false;
	
	_bstr_t sRegLog=_T(""),sDevListFile=_T("");	
	sRegLog = REGLOGBASE;


#ifdef UNICODE
#ifdef _DEBUG    
	sRegLog += _T("\\DeviceErrorLookUpUD.dll");
#else    
	sRegLog += _T("\\DeviceErrorLookUpU.dll");
#endif
#else
#ifdef _DEBUG    
	sRegLog += _T("\\DeviceErrorLookUpD.dll");
#else
	sRegLog += _T("\\DeviceErrorLookUp.dll");
#endif
#endif

    g_logger = CLog::GetLog();
    g_logger->GetConfig((LPCTSTR)sRegLog);
    g_logger->SetPrefix(_T("DevErrorLookUp"));
    g_logger->SetEnabled(true);

    g_logger->Log(TRACE_INFO, _T("************** BEGIN TRACE **************"));
    g_logger->LogFileVersion(TRACE_INFO);
    g_logger->Log(TRACE_API, _T("+DeviceErrorInitialize"));	

	//get file name from registry
	g_DeviceList = new CFLDeviceList();
	g_DeviceList->GetDeviceListFile(sDevListFile );  
	
	g_logger->Log(TRACE_INFO, _T("Loading device list: %s"), (LPTSTR)sDevListFile);
	//Load device list
	bRet = g_DeviceList->LoadDeviceList(sDevListFile);

    g_logger->Log(TRACE_API, _T("-DeviceErrorInitialize"));

	return bRet;
	
}

//************************************************************************ 
//
// Function:		DeviceErrorUnInitialize
// Purpose:			Uninitialize devie list		
// Parameters:
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool DeviceErrorUnInitialize()
{
	g_DeviceList->UnLoadDeviceList();
	delete g_DeviceList;
	g_DeviceList = NULL;
	g_logger->Log(TRACE_API, _T("DeviceErrorUnInitialize"));
    CLog::DestroyLog();
    g_logger = NULL;	
	CoUninitialize();
	return true;
}



//************************************************************************ 
//
// Function:		GetDeviceErrorHTML
// Purpose:			Get/create device error html based on error codes configured in 
//					DeviceErrorXXXXEnhanced.xml		
// Parameters:
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool GetDeviceErrorHTML(LPTSTR sHTML,  int DeviceClass, LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,
					int StatusEventId, long ExtendedResultCode, int nDataLength, void* sData, bool* bCheckHealth, int nContext, LPTSTR sLCID)
{
	
	bool bRet=false;
	g_logger->Log(TRACE_API, _T("+GetDeviceErrorHTML() with DevClass=%d, DevModel=%s, DevSubModel=%s, Code=%d, ExtCode=%d, context=%d, LCID=%s"),
       DeviceClass, sDeviceModel, sDeviceSubModel, StatusEventId, ExtendedResultCode, nContext, sLCID );

	TCHAR   sHTMLFile[_MAX_PATH];
	//_bstr_t sHTMLFile=_T("");

	_bstr_t sErrPath=_T(""), sClassName=_T(""), sXMLFile=_T(""), sImagePath=_T(""), sConfigPath=_T("");
	   

	bRet = g_DeviceList->GetDeviceErrorXML(DeviceClass, sXMLFile,  sClassName);
	if (!bRet)
	{
		GET_PARTITION_PATH(DEFAULT_DEVERRORHTMLFILE, sHTML);
		g_logger->Log(TRACE_ERROR, _T("Failed to find DeviceErrorXXXXenhanced.xml, use default %s"),sHTML );
		return false;
	}


	//get directories
	sErrPath = g_DeviceList->GetDeviceErrorPath();
	sImagePath = g_DeviceList->GetImagePath();
	sConfigPath = g_DeviceList->GetConfigPath();
	_bstr_t sLCIDTmp = sLCID;

	//Load the DeviceErrorXXXXEnhanced.xml
	CFLDeviceErrorMapping g_DeviceErrorMap;	
	bRet = g_DeviceErrorMap.LoadDeviceErrorMapping((LPTSTR)sXMLFile, DeviceClass,sClassName, sErrPath, sImagePath, sConfigPath,sLCIDTmp, g_nTerminalNumber );



	if (!bRet)
	{
		GET_PARTITION_PATH(DEFAULT_DEVERRORHTMLFILE, sHTML);
		g_logger->Log(TRACE_ERROR, _T("Failed to LoadDeviceErrorMapping, use default %s"), sHTML);
		return false;
	}
	
	_tcscpy(sCurrentTitle, _T(""));
	_tcscpy(sCurrentText, _T(""));
    _tcscpy(sCurrentButton1Text, _T(""));
    
	//search the error mapping to find html file
    bRet = g_DeviceErrorMap.GetDeviceErrorHTMLFile((LPTSTR)sHTMLFile, sCurrentTitle,sCurrentTitle2,
        sCurrentText, sDeviceModel, sDeviceSubModel, StatusEventId, ExtendedResultCode,  bCheckHealth,
        nContext, sCurrentButton1Text);
	

	if (_tcslen(sCurrentTitle) != 0)
		g_logger->Log(TRACE_INFO, _T("CurrentTitle=%s"),sCurrentTitle );

	if (_tcslen(sCurrentTitle2) != 0)
		g_logger->Log(TRACE_INFO, _T("CurrentTitle2=%s"),sCurrentTitle2 );
	
	if (bRet && _tcslen(sHTMLFile) < _MAX_PATH ) //found the html
	{
		_tcscpy(sHTML, sHTMLFile); 
        g_logger->Log(TRACE_API, _T("-GetDeviceErrorHTML() returns %s"),sHTMLFile );
		
		
	}
	else  //default html
	{
		GET_PARTITION_PATH(DEFAULT_DEVERRORHTMLFILE, sHTML);
        g_logger->Log(TRACE_ERROR, _T("Failed to find the html file defined for this error, using default %s"), sHTML); 
        g_logger->Log(TRACE_ERROR, _T("-GetDeviceErrorHTML()with ret=false")); 
		bRet = false;
	}
	
	return bRet;

	
}

//************************************************************************ 
//
// Function:		GetCurrentTitleText
// Purpose:			Get current title1 text 		
// Parameters:
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool GetCurrentTitleText(LPTSTR sTitle1)
{
	if (_tcslen(sCurrentTitle) < _MAX_PATH)
		_tcscpy(sTitle1, sCurrentTitle); 
	return true;

}

DEVICEERRORLOOKUP_API bool GetCurrentButton1Text(LPTSTR sButtonText)
{
    if (_tcslen(sCurrentButton1Text) < _MAX_PATH)
        _tcscpy(sButtonText, sCurrentButton1Text); 
    return true;

}

DEVICEERRORLOOKUP_API bool GetCurrentTitle2Text(LPTSTR sTitle2)
{
	if (_tcslen(sCurrentTitle2) < _MAX_PATH)
		_tcscpy(sTitle2, sCurrentTitle2); 
	return true;

}


DEVICEERRORLOOKUP_API int GetCurrentMessageTextLength()
{ 

	return _tcslen(sCurrentText);

}
//************************************************************************ 
//
// Function:		GetCurrentTitleText
// Purpose:			Get current title1 text 		
// Parameters:
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool GetCurrentMessageText(LPTSTR sMsgText)
{
	if (sMsgText != NULL && _tcslen(sCurrentText) > 0)
	{
		_tcsncpy(sMsgText, sCurrentText, _tcslen(sCurrentText) ); 
		sMsgText[_tcslen(sCurrentText)]= _T('\0');

	}
	return true;

}

//************************************************************************ 
//
// Function:		SetTerminalNumber
// Purpose:			Set Terminal Number		
// Parameters:
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool SetTerminalNumber(long nTermNumber)
{
    g_nTerminalNumber = nTermNumber;
	g_logger->Log(TRACE_INFO, _T("SetTerminalNumber() = %d"),g_nTerminalNumber );
    
   return true;
}


DEVICEERRORLOOKUP_API bool IsDeviceErrorIgnored(int DeviceClass, LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,int StatusEventId, long ExtendedResultCode)
{
  bool bRet=false;


  _bstr_t sErrPath=_T(""), sClassName=_T(""), sXMLFile=_T(""), sImagePath=_T(""), sConfigPath=_T("");
	   

    bRet = g_DeviceList->GetDeviceErrorXML(DeviceClass, sXMLFile,  sClassName);
    if (!bRet)
    {
        LPTSTR sDefaultXML = new _TCHAR[_MAX_PATH];
        GET_PARTITION_PATH(DEFAULT_DEVERROR_ENHANCEDXML, sDefaultXML);
        g_logger->Log(TRACE_ERROR, _T("Failed to find DeviceErrorXXXXenhanced.xml, use default %s"), sDefaultXML);
        delete [] sDefaultXML;
        return false;
    }


	//get directories
	sErrPath = g_DeviceList->GetDeviceErrorPath();
	sImagePath = g_DeviceList->GetImagePath();
	sConfigPath = g_DeviceList->GetConfigPath();

	//Load the DeviceErrorXXXXEnhanced.xml, do not care about locale
	CFLDeviceErrorMapping g_DeviceErrorMap;	
	bRet = g_DeviceErrorMap.LoadDeviceErrorMapping((LPTSTR)sXMLFile, DeviceClass,sClassName, sErrPath, sImagePath, sConfigPath,_bstr_t(_T("0409")), g_nTerminalNumber );

    if (bRet)
	 bRet = g_DeviceErrorMap.IgnoreDeviceError(sDeviceModel, sDeviceSubModel, StatusEventId ,ExtendedResultCode);

 
  return bRet;
}

//************************************************************************ 
//
// Function:        UpdateDeviceErrorHTMLTitle
// Purpose:         Replace or updates device error html title      
// Parameters:      spParagraph - XML Node pointer
//                  csTitle  - String that holds the new title message.
// Returns:         true or false
// Calls:               
// Side Effects:
// Notes:           Added for SSCOB-517
//
//************************************************************************
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLTitle(LPTSTR sHTMLFile, LPTSTR csTitle, bool bAppend)
{
    if (_tcslen(csTitle) == 0) 
        return false;

    g_logger->Log(TRACE_API, _T("+UpdateDeviceErrorHTMLTitle() %s TO %s"), csTitle, sHTMLFile);

    HRESULT hr;
    bool bRet=false;

    MSXML2::IXMLDOMDocumentPtr spDoc;
    MSXML2::IXMLDOMNodeListPtr spNodeList, spNodeListInput, spSubNodeList;
    MSXML2::IXMLDOMNodePtr spParagraph, spVideo, spNodeBody, spBtnPlay, spNode, spNodeLast, spBtnPause, spBtnStop, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem, spImgParent, spImg;
    

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
        g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLTitle() with ret=false"));
        return false;
    }

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load    
    if( spDoc->load( sHTMLFile ) != VARIANT_TRUE )
    {
        g_logger->Log(TRACE_ERROR, _T("Failed to load HTML file %s"), sHTMLFile);
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLTitle() with ret=false"));
        spDoc.Release();
        return false;
    }

    spNodeList = spDoc->selectNodes(XML_HTML_DIV);
    while(  spNode = spNodeList->nextNode())
    {
        spItem = spNode->attributes->getNamedItem( XML_ID );
        if( spItem != NULL && spItem->text.length() )
        {
              _bstr_t sTmp=spItem->text;

             if (_tcscmp((LPTSTR)sTmp, _T("TopBanner")) == 0)
             {
                 spParagraph = spNode;
                 spSubNodeList=spNode->selectNodes(XML_HTML_TITLE2);                
             }
        }
    }

    //update title
    CFLDeviceErrorMapping g_DeviceErrorMap;
    if (spParagraph != NULL)
    {
        bRet = g_DeviceErrorMap.ParseDeviceErrorHTMLTitle(spParagraph, csTitle);
    }


    //Construct the parse html file name
    TCHAR *s1 = _tcsstr(sHTMLFile, _T(".htm"));
    TCHAR sLeft[_MAX_PATH];
    _bstr_t sSavedFile=_T("");
    int nLength = 0;

    if (bAppend == false)
    {
        if (s1)
        {
            nLength = _tcslen(sHTMLFile) - _tcslen(s1);  
            _tcsncpy(sLeft, sHTMLFile, nLength);
            sLeft[nLength]=_T('\0');
            sSavedFile = _bstr_t(sLeft)+ _bstr_t(_T("_NotFixed.htm"));      
        }
    }
    else
    {
        sSavedFile=sHTMLFile;
    }

    
    _tcsncpy(sHTMLFile, (LPTSTR)sSavedFile, _tcslen(sSavedFile));
    sHTMLFile[_tcslen(sSavedFile)]=_T('\0');

    try
    {
        spDoc->save(sSavedFile);
    }
    catch(_com_error& e)
    {
        bRet=false;
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
        g_logger->Log(TRACE_ERROR, _T("Exception caught! The change to the HTML error text for file %s failed to save."), (LPTSTR)sSavedFile);
        g_logger->Log(
            TRACE_ERROR,
            _T("Error\n\tCode = 0x%08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
            e.Error(),
            e.ErrorMessage(),
            (LPCTSTR) bstrSource,
            (LPCTSTR) bstrDescription
        );
    }
    g_logger->Log(TRACE_INFO, _T("The parsed html file is %s"),(LPTSTR)sSavedFile );
    

    spDoc.Release();    
    g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLTitle()") );
    return bRet;
}

//************************************************************************ 
//
// Function:        UpdateDeviceErrorHTMLSubTitle
// Purpose:         Replace or updates device error html sub title      
// Parameters:      spParagraph - XML Node pointer
//                  csSubTitle  - String that holds the new sub title message.
// Returns:         true or false
// Calls:               
// Side Effects:
// Notes:           Added for SSCOB-517
//
//************************************************************************
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLSubTitle(LPTSTR sHTMLFile, LPTSTR csSubTitle, bool bAppend)
{
    if (_tcslen(csSubTitle) == 0) 
        return false;

    g_logger->Log(TRACE_API, _T("+UpdateDeviceErrorHTMLSubTitle() %s TO %s"), csSubTitle, sHTMLFile);
    

    HRESULT hr;
    bool bRet=false;

    MSXML2::IXMLDOMDocumentPtr spDoc;
    MSXML2::IXMLDOMNodeListPtr spNodeList, spNodeListInput, spSubNodeList;
    MSXML2::IXMLDOMNodePtr spParagraph, spVideo, spNodeBody, spBtnPlay, spNode, spNodeLast, spBtnPause, spBtnStop, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem, spImgParent, spImg;
    

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
        g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLSubTitle() with ret=false"));
        return false;
    }

    spDoc->async = VARIANT_FALSE; 
    if( spDoc->load( sHTMLFile ) != VARIANT_TRUE )
    {
        g_logger->Log(TRACE_ERROR, _T("Failed to load HTML file %s"), sHTMLFile);
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLSubTitle() with ret=false"));
        spDoc.Release();
        return false;
    }


    spNodeList = spDoc->selectNodes(XML_HTML_DIV);
    while(  spNode = spNodeList->nextNode())
    {
        spItem = spNode->attributes->getNamedItem( XML_ID );
        if( spItem != NULL && spItem->text.length() )
        {
              _bstr_t sTmp=spItem->text;

             if (_tcscmp((LPTSTR)sTmp, _T("SubTitleMid")) == 0)
             {
                 spParagraph = spNode;
             }
        }
    }

    //update sub title
    CFLDeviceErrorMapping g_DeviceErrorMap;
    if (spParagraph != NULL)
    {
        bRet = g_DeviceErrorMap.ParseDeviceErrorHTMLSubTitle(spParagraph, csSubTitle);
    }

    //Construct the parse html file name
    TCHAR *s1 = _tcsstr(sHTMLFile, _T(".htm"));
    TCHAR sLeft[_MAX_PATH];
    _bstr_t sSavedFile=_T("");
    int nLength = 0;

    if (bAppend == false)
    {
        if (s1)
        {
            nLength = _tcslen(sHTMLFile) - _tcslen(s1);  
            _tcsncpy(sLeft, sHTMLFile, nLength);
            sLeft[nLength]=_T('\0');
            sSavedFile = _bstr_t(sLeft)+ _bstr_t(_T("_NotFixed.htm"));      
        }
    }
    else
    {
        sSavedFile=sHTMLFile;
    }

    _tcsncpy(sHTMLFile, (LPTSTR)sSavedFile, _tcslen(sSavedFile));
    sHTMLFile[_tcslen(sSavedFile)]=_T('\0');

    try
    {
        spDoc->save(sSavedFile);
    }
    catch(_com_error& e)
    {
        bRet=false;
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
        g_logger->Log(TRACE_ERROR, _T("Exception caught! The change to the HTML error text for file %s failed to save."), (LPTSTR)sSavedFile);
        g_logger->Log(
            TRACE_ERROR,
            _T("Error\n\tCode = 0x%08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
            e.Error(),
            e.ErrorMessage(),
            (LPCTSTR) bstrSource,
            (LPCTSTR) bstrDescription
        );
    }
    g_logger->Log(TRACE_INFO, _T("The parsed html file is %s"),(LPTSTR)sSavedFile );


    spDoc.Release();    
    g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLSubTitle()") );
    return bRet;
}

//************************************************************************ 
//
// Function:		UpdateDeviceErrorHTMLText
// Purpose:			Replace the text area of html with csText	
// Parameters:		sHTMLFile - html file name
//					csText - new text to replace old one
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool UpdateDeviceErrorHTMLText(LPTSTR sHTMLFile, LPTSTR csText , bool bAppend)
{
    g_logger->Log(TRACE_API, _T("+UpdateDeviceErrorHTMLText() %s TO %s"), csText, sHTMLFile);
	

    HRESULT hr;
	bool bRet=false;

	MSXML2::IXMLDOMDocumentPtr spDoc;
	MSXML2::IXMLDOMNodeListPtr spNodeList, spNodeListInput, spSubNodeList;
	MSXML2::IXMLDOMNodePtr spParagraph, spVideo, spNodeBody, spBtnPlay, spNode, spNodeLast, spBtnPause, spBtnStop, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem, spImgParent, spImg;
	

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLText() with ret=false"));
        return false;
    }

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load

	
	if( spDoc->load( sHTMLFile ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("Failed to load HTML file %s"), sHTMLFile);
        g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLText() with ret=false"));
        spDoc.Release();
		return false;
	}

	spNodeList = spDoc->selectNodes(XML_HTML_DIV);
	bool bParsed=false;

	while(  spNode = spNodeList->nextNode())
	{
		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			  _bstr_t sTmp=spItem->text;

			 if (_tcscmp((LPTSTR)sTmp, _T("Layer1")) == 0)
			 {
				 spParagraph = spNode;
				 spSubNodeList=spNode->selectNodes(XML_HTML_TEXT);  //text

				if (bAppend == false)
				{
					//remove all children except the first one to serve as a place hold for new text
					if (spSubNodeList != NULL )
					{
						while (spNode = spSubNodeList->nextNode())
						{
							if (bParsed)
							{
								spParagraph->removeChild(spNode);
							}							
							else
							{
								bParsed = true;
							}
						}
					}
				}
				else
				{
					//add an extra node at the end for append in g_DeviceErrorMap.ParseTextNewLine(spParagraph, csText);
					if (spSubNodeList != NULL )
					{
						while (spNode = spSubNodeList->nextNode())
						{
							    spNodeLast = spNode;						
								bParsed = true;							
						}
						spNode = spNodeLast->cloneNode(false);
						spParagraph->appendChild(spNode);
						_bstr_t x = spParagraph->Getxml();
					}
				}
				
			 }
			 else if (_tcscmp((LPTSTR)sTmp, _T("VideoWindow")) == 0)
			 {
				 spVideo = spNode;
			 }
			 else if (_tcscmp((LPTSTR)sTmp, _T("BtnPlayLayer")) == 0)
			 {
				 spBtnPlay = spNode;
			 }
			else if (_tcscmp((LPTSTR)sTmp, _T("BtnStopLayer")) == 0)
			 {
				 spBtnStop = spNode;
			 }
			else if (_tcscmp((LPTSTR)sTmp, _T("BtnPauseLayer")) == 0)
			 {
				 spBtnPause = spNode;
			 }
			 
		}
	}

	//replace with new text
	CFLDeviceErrorMapping g_DeviceErrorMap;
	if (spParagraph != NULL)
		g_DeviceErrorMap.ParseTextNewLine(spParagraph, csText);


	//remove all the video related controls in html
	spNodeBody = spDoc->selectSingleNode(XML_HTML_BODY);
	if (spNodeBody != NULL) 
	{   
		if (spVideo != NULL )
			spNodeBody->removeChild(spVideo);
		if (spBtnPlay != NULL)
			spNodeBody->removeChild(spBtnPlay);
		if (spBtnPause != NULL)
			spNodeBody->removeChild(spBtnPause);
		if (spBtnStop != NULL)
			spNodeBody->removeChild(spBtnStop);
	}

	//Save the parsed html to *_NotFixed.htm such as CashAcceptor_CashFlow66_10_-99_-1_NotFixed.htm
	//Construct the parse html file name
	TCHAR *s1 = _tcsstr(sHTMLFile, _T(".htm"));
	TCHAR sLeft[_MAX_PATH];
	_bstr_t sSavedFile=_T("");
	int nLength = 0;

	if (bAppend == false)
	{
		if (s1)
		{
			nLength = _tcslen(sHTMLFile) - _tcslen(s1);	 
			_tcsncpy(sLeft, sHTMLFile, nLength);
			sLeft[nLength]=_T('\0');
			sSavedFile = _bstr_t(sLeft)+ _bstr_t(_T("_NotFixed.htm"));		
		}
	}
	else
	{
		sSavedFile=sHTMLFile;
	}

	
	if (bParsed == true)
	{
		_tcsncpy(sHTMLFile, (LPTSTR)sSavedFile, _tcslen(sSavedFile));
		sHTMLFile[_tcslen(sSavedFile)]=_T('\0');

        try
        {
            spDoc->save(sSavedFile);
        }
        catch(_com_error& e)
        {
            bRet=false;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
            g_logger->Log(TRACE_ERROR, _T("Exception caught! The change to the HTML error text for file %s failed to save."), (LPTSTR)sSavedFile);
            g_logger->Log(
                TRACE_ERROR,
                _T("Error\n\tCode = 0x%08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                e.Error(),
                e.ErrorMessage(),
                (LPCTSTR) bstrSource,
                (LPCTSTR) bstrDescription
            );
        }
		g_logger->Log(TRACE_INFO, _T("The parsed html file is %s"),(LPTSTR)sSavedFile );
		bRet=true;
	}
	else
	{
			g_logger->Log(TRACE_ERROR, _T("%s text is not updated."), sHTMLFile);
			bRet = false;
	}


	spDoc.Release();    
	g_logger->Log(TRACE_API, _T("-UpdateDeviceErrorHTMLText()") );
	return bRet;

}

//************************************************************************ 
//
// Function:		ParseDeviceErrorHTML
// Purpose:			Fill in %s in the title or text area of html file	
// Parameters:		sHTMLFile - html file name
//					sParms - new text to be inserted
//					sToken - such as %s
// Returns:			true or false
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
DEVICEERRORLOOKUP_API bool ParseDeviceErrorHTML(LPTSTR sHTMLFile, LPTSTR sParms, LPTSTR sToken)
{
	
    g_logger->Log(TRACE_API, _T("+ParseDeviceErrorHTML() with sHTML=%s, In Parms=%s"), sHTMLFile, sParms);

	MSXML2::IXMLDOMDocumentPtr spDoc;
    HRESULT hr;
	bool bRet=false;

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
        g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??"));
        g_logger->Log(TRACE_API, _T("-ParseDeviceErrorHTML() with ret=false"));
        return false;
    }

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load

	
	if( spDoc->load( sHTMLFile ) != VARIANT_TRUE )
	{
        g_logger->Log(TRACE_ERROR, _T("Failed to load HTML file %s"), sHTMLFile);
        g_logger->Log(TRACE_API, _T("-ParseDeviceErrorHTML() with ret=false"));
        spDoc.Release();
		return false;
	}


	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList, spSubNodeList2, spSubNodeList3;
	MSXML2::IXMLDOMNodePtr spNode, spNode2, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem;
	

	spNodeList = spDoc->selectNodes(XML_HTML_DIV);

	while(  spNode = spNodeList->nextNode())
	{
		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			  
			  _bstr_t sTmp = spItem->text;

			 if (_tcscmp((LPTSTR)sTmp, _T("Layer1")) == 0)
			 {
				 spSubNodeList=spNode->selectNodes(XML_HTML_TEXT);  ////Text field
				
			 }
			 if (_tcscmp((LPTSTR)sTmp, _T("SubTitleMid")) == 0)
			 {
				 spSubNodeList2 = spNode->selectNodes(XML_HTML_TITLE2);     //title2
			 }
			 
			 
		}
	}

	_bstr_t sTmp4Parm=sParms;
	TCHAR *sTmp4=NULL;
	sTmp4=(LPTSTR)sTmp4Parm;

	bool bParsed = false;
	

	//Title2
	if (spSubNodeList2 != NULL)
	{
		while (spNode2 = spSubNodeList2->nextNode())
		{
			bRet = false;
			TCHAR   *sTmp3=NULL;
			
			
			spItem = spNode2->attributes->getNamedItem( XML_CLASS );
			if( spItem != NULL && spItem->text.length() )
			{
				 								 
				 _bstr_t sTmp = spItem->text;

				 if (_tcscmp((LPTSTR)sTmp, _T("SubTitle")) == 0 )
				 {
					  _bstr_t sTmp2Title = spNode2->text;
					  TCHAR *sTmp2=NULL;
					  sTmp2=(LPTSTR)sTmp2Title;
					  
					
					 sTmp3 = new TCHAR[_tcslen(sTmp2) + _tcslen(sTmp4)+1];
					 sTmp3[0]=_T('\0');

					 TCHAR *s1=NULL, *s2=NULL;					 

					 s1 = _tcsstr((LPTSTR)sTmp2, sToken );
					 while( s1 != NULL )
						{
						 bParsed = true;
						 bRet = true;
						 _tcsncat(sTmp3, (LPTSTR)sTmp2, s1-sTmp2);
						 sTmp3[_tcslen(sTmp3)+1]=NULL;
						 s2=_tcsstr((LPTSTR)sTmp4, _T("&"));
						 if (s2)
						 {
							_tcsncat(sTmp3, sTmp4, (int)(s2-sTmp4) );
							sTmp3[_tcslen(sTmp3)+1]=NULL;

							sTmp4=s2+_tcslen(_T("&"));
							
						 }
						 else
							 _tcscat(sTmp3, (LPTSTR)sTmp4);
						 sTmp2=s1+_tcslen(sToken);
						 if (_tcslen(sTmp2) != 0)
						 {
							s1 = _tcsstr( sTmp2, sToken );
							if (s1==NULL)
								_tcscat(sTmp3, (LPTSTR)sTmp2);
						 }
						 else
							 s1=NULL;
						}

				 }
				 	
				
				 
			}
			if (bRet == true && sTmp3 != NULL)
			{
				spNode2->put_text(_bstr_t(sTmp3));
			}
			
			delete[] sTmp3;
			sTmp3=NULL;
		}
	}

	//text
	if (spSubNodeList != NULL)
	{
		while (spNode = spSubNodeList->nextNode())
		{
			bRet = false;
			TCHAR   *sTmp3=NULL;
			
			spItem = spNode->attributes->getNamedItem( XML_CLASS );
			if( spItem != NULL && spItem->text.length() )
			{
				 
				 _bstr_t sTmp = spItem->text;

				 if (_tcscmp((LPTSTR)sTmp, _T("ContentHead")) == 0 ||
					 _tcscmp((LPTSTR)sTmp, _T("ParagraphBold")) == 0)
				 {

					 _bstr_t sTmp2Text = spNode->text;
					 TCHAR *sTmp2=NULL;
					 sTmp2=(LPTSTR)sTmp2Text;
					  

					 TCHAR *s1 = NULL, *s2 = NULL;
					 sTmp3=new TCHAR[_tcslen(sTmp2)+_tcslen(sTmp4)+1];
					 sTmp3[0]=_T('\0');

					 s1 = _tcsstr( (LPTSTR)sTmp2, sToken );
					 
					 while( s1 != NULL )
						{
						 bParsed = true;
						 bRet = true;
						 
						 _tcsncat(sTmp3, (LPTSTR)sTmp2, s1-sTmp2);
						 sTmp3[_tcslen(sTmp3)+1]=NULL;

						 s2=_tcsstr((LPTSTR)sTmp4, _T("&"));
						 if (s2)
						 {
							_tcsncat(sTmp3, (LPTSTR)sTmp4, s2-sTmp4 );
							sTmp3[_tcslen(sTmp3)+1]=NULL;
							sTmp4=s2+_tcslen(_T("&"));
							
							
						 }
						 else
						 {
							 _tcscat(sTmp3,(LPTSTR) sTmp4);
							 
						 }
						 sTmp2 = s1+_tcslen(sToken);
						 if (_tcslen(sTmp2) != 0)
						 {
							s1 = _tcsstr((LPTSTR) sTmp2, sToken );
							if (s1==NULL)
								_tcscat(sTmp3, (LPTSTR)sTmp2);
						 }
						 else
							 s1=NULL;
						}

				 }
				 
				 if (bRet == true && sTmp3 != NULL)
				{

					_tcsncpy(sCurrentText, sTmp3, _tcslen(sTmp3));
					sCurrentText[_tcslen(sTmp3)] = _T('\0');

					 TCHAR* s1= NULL;					
					  s1 = _tcsstr( sTmp3, _T("\\n") );
					if (s1 == NULL)
						spNode->put_text(_bstr_t(sTmp3));
					else {
						//+SSCOP-3334
						//Removed the following 3 lines for this JIRA
						//CFLDeviceErrorMapping g_DeviceErrorMap;
						//if (spNode != NULL)
						//	g_DeviceErrorMap.ParseTextNewLine(spNode, sTmp3, true);

						RemoveAllChildrenNode(spNode);
						PutLineBreaks(spDoc, spNode, sTmp3, _T("\\n"));
						//-SSCOP-3334
					}
					
				}

			}
		
			delete[] sTmp3;
			sTmp3=NULL;
		}
		
	}
	

	if (bParsed == true)
	{
        try
        {
            spDoc->save(sHTMLFile);
        }
        catch(_com_error& e)
        {
            bRet=false;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
            g_logger->Log(TRACE_ERROR, _T("Exception caught! The change to the HTML error text for file %s failed to save."), (LPTSTR)sHTMLFile);
            g_logger->Log(
                TRACE_ERROR,
                _T("Error\n\tCode = 0x%08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                e.Error(),
                e.ErrorMessage(),
                (LPCTSTR) bstrSource,
                (LPCTSTR) bstrDescription
            );
        }
        g_logger->Log(TRACE_INFO, _T("The parsed html file is %s"),sHTMLFile );
	}

	
	

	spDoc.Release();
	
	
    g_logger->Log(TRACE_API, _T("-ParseDeviceErrorHTML()"));
	return bRet;
}


// This is the constructor of a class that has been exported.
// see DeviceErrorLookUp.h for the class definition
CDeviceErrorLookUp::CDeviceErrorLookUp()
{ 
	return; 
}

