// FLDeviceErrorMapping.cpp: implementation of the CFLDeviceErrorMapping class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FLDeviceErrorMapping.h"
#include "FastlaneUtils.h"
#include "log.h"
#include <direct.h>


const TCHAR DEFAULT_HTML_TEMPLATE_IMAGE[] =  _T("%APP_DRIVE%\\scot\\data\\deviceerror\\FastLaneDeviceErrorTemplateBase.htm");
const TCHAR DEFAULT_HTML_TEMPLATE_VIDEO[] =  _T("%APP_DRIVE%\\scot\\data\\deviceerror\\FastLaneDeviceErrorTemplateBaseVideo.htm");
const TCHAR DEFAULT_HTML_TEMPLATE[] =  _T("%APP_DRIVE%\\scot\\data\\deviceerror\\FastLaneDeviceErrorTemplateBaseNoImage.htm");

const TCHAR XML_DEVICE_MODEL[] = _T("//Device-Lookup-Table//DeviceModels//Device-Model");
const TCHAR XML_DMID[] = _T("id");
const TCHAR XML_DMNAME[] = _T("name");
const TCHAR XML_DMSUBMDOEL[] = _T("SubModel");

const TCHAR XML_DEVICE_ERROR[] = _T("//Device-Lookup-Table//DeviceErrors//Device-Error");
const TCHAR XML_DEMODEL[] = _T("Model");
const TCHAR XML_DESUBMODEL[] = _T("SubModel");
const TCHAR XML_DERETCODE[] = _T("ResultCode");
const TCHAR XML_DEEXRETCODE[] = _T("ExtendedResultCode");
const TCHAR XML_DEDISPLAY[] = _T("ErrorDisplay");
const TCHAR XML_CHECKHEALTH[] = _T("CheckHealth");
const TCHAR XML_CONTEXT[] = _T("Context");
const TCHAR XML_TITLE1[] = _T("Title1");
const TCHAR XML_TITLE2[] = _T("Title2");
const TCHAR XML_TEXT[] = _T("Text");
const TCHAR XML_IMAGE[] = _T("Image");
const TCHAR XML_VIDEO[] = _T("Video");
const TCHAR XML_BUTTON1TEXT[] = _T("Button1Text");

const TCHAR XML_HTML_DIV[] = _T("//HTML//BODY//div");
const TCHAR XML_ID[] = _T("id");
const TCHAR XML_HTML_TEXT[] = _T("//HTML//BODY//div//p");
const TCHAR XML_HTML_TITLE2[] = _T("//HTML//BODY//div//div");
const TCHAR XML_HTML_IMG[] = _T("//HTML//BODY//div//img");
const TCHAR XML_HTML_VIDEO[] = _T("//HTML//BODY//div//OBJECT");
const TCHAR XML_HTML_VIDEO_PARAM[] = _T("//HTML//BODY//div//OBJECT//PARAM");
const TCHAR XML_HTML_VIDEO_BTNPLAY[] = _T("//HTML//BODY//div//a");
const TCHAR XML_CLASS[] = _T("class");
const TCHAR XML_IMGSRC[] = _T("src");
const TCHAR XML_VIDEO_NAME[] = _T("NAME");
const TCHAR XML_VIDEO_VALUE[] = _T("VALUE");
const TCHAR XML_VIDEO_ONCLICK[] = _T("OnClick");


const TCHAR XML_MSG_TABLE[] = _T("//DeviceError//Languages//Language//StringTable//");
extern CLog* g_logger;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFLDeviceErrorMapping::CFLDeviceErrorMapping()
{
	//CoInitialize(NULL);
	m_sDefaultPath = _T("");
	m_sImagePath = _T("");
	m_sConfigPath = _T("");
	m_sVideoPath = _T("");;
    m_sLanguageID = _T("0409");
	m_sClassName = _T("");
	m_sHTMLPath = _T("");
	m_sMsgFile = _T("");
	m_sMsgFile0=_T("");
	m_sTerminalMsgFile = _T("");
	
	m_nDeviceClass = -1;
	m_nCurrModel=0;
	m_nCurrSubModel=0;
}

CFLDeviceErrorMapping::~CFLDeviceErrorMapping()
{	
	m_aDeviceErrorInfo.clear();
	m_aModels.clear();
}


//************************************************************************ 
//
// Function:		LoadDeviceErrorMapping
// Purpose:			Load DeviceErrorXXXXEnhanced.xml for error mappings		
// Parameters:		[in] sFileName - DeviceErrorXXXXEnhanced.xml to open
//					[in] nDeviceClass - class id
//					[in] sClassName - class description, used in created html file name
//					[in] sDefaultPath - default device error dir: %APP_DRIVE%\scot\data\deviceerror, configured in FLDeviceList.xml
//					[in] sImagePath - image file path, default %APP_DRIVE%\scot\image, configured in FLDeviceList.xml
//					[in] sConfigPath - configure file dir, default %APP_DRIVE%\scot\config, configured in FLDeviceList.xml
//					[in] sLanguageID - primary language code, default 0409
//					[in] nTermNum - the terminal number,  be set by SetTerminalNumber()
// Returns:			true or false
// Calls:			LoadDeviceErrorMappingToList()
// Side Effects:
// Notes:
//
//************************************************************************

bool CFLDeviceErrorMapping::LoadDeviceErrorMapping(LPTSTR sFileName,int nDeviceClass,_bstr_t & sClassName,_bstr_t & sDefaultPath, _bstr_t & sImagePath, _bstr_t & sConfigPath, _bstr_t & sLanguageID,long nTermNum)
{
	bool bRet=true;
    g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::LoadDeviceErrorMapping() with file=%s"), sFileName);
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() ClassName=%s, language=%s, terminal=%d"), (LPTSTR)sClassName,(LPTSTR)sLanguageID, nTermNum);
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() DefaultPath=%s"), (LPTSTR)sDefaultPath );
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() ImagePath=%s"),  (LPTSTR)sImagePath );
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() ConfigPath=%s"), (LPTSTR)sConfigPath );
	
	m_aDeviceErrorInfo.clear();
	m_aModels.clear();

	m_sDefaultPath = sDefaultPath;
	m_sImagePath = sImagePath;
	m_sConfigPath = sConfigPath;
	m_sLanguageID = sLanguageID;
	m_sClassName = sClassName;
	m_nDeviceClass = nDeviceClass;

	//default to English
	if (_tcslen(m_sLanguageID) == 0)
		m_sLanguageID = _T("0409");
		

	m_sHTMLPath=m_sDefaultPath+m_sLanguageID+_T("\\");
	m_sMsgFile=m_sConfigPath + _T("DMMsg") + m_sLanguageID +_T(".xml");
	m_sMsgFile0 = m_sConfigPath + _T("DMMsg") + m_sLanguageID +_T(".000");
	m_sVideoPath = m_sDefaultPath + _T("Videos\\");

	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() HTML path=%s"), (LPTSTR)m_sHTMLPath );
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() Video path=%s"), (LPTSTR)m_sVideoPath );
	g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() message file=%s"),(LPTSTR)m_sMsgFile);

    int n = _tmkdir(m_sHTMLPath);
	
	if (nTermNum !=-1)
	{
		TCHAR sTerminal[25];
		_stprintf(sTerminal, _T("DMMsg%s.%03d"), (LPTSTR)m_sLanguageID,nTermNum);
		m_sTerminalMsgFile = m_sConfigPath + sTerminal;
		g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() terminal message file=%s"), (LPTSTR)m_sTerminalMsgFile);

	}

	 //construct the DeviceErrorXXXXenhanced.000 or .Terminal file names
	  TCHAR *s1 = _tcsstr(sFileName, _T(".xml"));
	  int nLength = _tcslen(sFileName) - _tcslen(s1);
	  _bstr_t sOverride0=_T(""),sOverrideN=_T("");

	  TCHAR * sLeft=new TCHAR[nLength+1];
      if (s1)
		_tcsncpy(sLeft, sFileName, nLength);		
	  sLeft[nLength]=_T('\0');

	  if (_tcslen(sLeft) !=0)
	  {
		sOverride0 = m_sDefaultPath + sLeft + _T(".000");

		TCHAR sTermSuffix[20];
		_stprintf(sTermSuffix, _T(".%03d"), nTermNum);

		sOverrideN=m_sDefaultPath + sLeft + sTermSuffix;
	  }

      // +TAR 447073
      delete[] sLeft;
        sLeft=NULL;
      // -TAR 447073

	_bstr_t sEnhancedFile=_T("");
	sEnhancedFile = m_sDefaultPath + sFileName;

	bRet= LoadDeviceErrorMappingToList(sEnhancedFile, m_aDeviceErrorInfo, m_aModels );
	

	//Load DeviceErrorXXXXenhanced.000 and/or DeviceErrorXXXXenhanced.XXX
	if (FileExist((LPTSTR)sOverride0))
	{
		g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() Device Error Mapping Override=%s"), sOverride0);
		LoadDeviceErrorMappingOverride((LPTSTR)sOverride0);
	}
	if (FileExist((LPTSTR)sOverrideN))
	{
		g_logger->Log(TRACE_INFO, _T("CFLDeviceErrorMapping::LoadDeviceErrorMapping() Device Error Mapping terminal Override=%s"), sOverrideN);
		LoadDeviceErrorMappingOverride((LPTSTR)sOverrideN);
	}


    g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::LoadDeviceErrorMapping() with ret=%d"), bRet);
	
	return bRet;
}

//************************************************************************ 
//
// Function:		LoadDeviceErrorMappingToList
// Purpose:			Load error mappings in DeviceErrorXXXXEnhanced.xml into lists		
// Parameters:
// Returns:			true or false
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::LoadDeviceErrorMappingToList(LPTSTR sFileName, DEVICEERRORMAP &aDevErrInfo, DEVICEMODELMAP &aModels )
{
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::LoadDeviceErrorMappingToList() %s"), sFileName);

	aDevErrInfo.clear();
	aModels.clear();

	MSXML2::IXMLDOMDocumentPtr spDoc;
    HRESULT hr;
	

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??") );
        g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::LoadDeviceErrorMapping()"));
        return false;
    }

	FLDeviceErrorInfo objDeviceErrInfo ;
    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load

	if( spDoc->load( sFileName ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("Failed to load device error mapping file %s"), sFileName );
        g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::LoadDeviceErrorMapping()"));
        spDoc.Release();
		return false;
	}

	//load device error mappings
	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList;
	MSXML2::IXMLDOMNodePtr spNode, spSubNode, spItem, spItem2, spSubItem, spSubItem2;
	

	//build error mapping list
	spNodeList = spDoc->selectNodes(XML_DEVICE_ERROR);

	while(  spNode = spNodeList->nextNode())
	{
		objDeviceErrInfo.Reset();		

		spItem = spNode->selectSingleNode( XML_DEMODEL );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetDeviceModel(spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_DESUBMODEL );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetDeviceSubModel(spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_DERETCODE );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetResultCode( _ttoi(LPCTSTR(spItem->text)));
		}
		spItem = spNode->selectSingleNode( XML_DEEXRETCODE );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetExtendedResultCode( _ttol(LPCTSTR(spItem->text)));
			
		}
		spItem = spNode->selectSingleNode( XML_DEDISPLAY );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetErrorHTML( spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_CHECKHEALTH );
		if (spItem != NULL)
		{
			if (_tcsicmp(LPCTSTR(spItem->text), _T("true")) == 0 ||
				_tcsicmp(LPCTSTR(spItem->text), _T("True")) == 0 ||
				_tcsicmp(LPCTSTR(spItem->text), _T("TRUE")) == 0)
					objDeviceErrInfo.SetCheckHealth(true);
		}
		spItem = spNode->selectSingleNode( XML_CONTEXT );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetContext( _ttoi(LPCTSTR(spItem->text)));
		}

		spItem = spNode->selectSingleNode( XML_TITLE1 );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetTitle1(spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_TITLE2 );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetTitle2(spItem->text);
		}

		spItem = spNode->selectSingleNode( XML_TEXT );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetText(spItem->text);
		}
		spItem = spNode->selectSingleNode( XML_IMAGE );
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetImage(spItem->text);
		
		}

		spItem = spNode->selectSingleNode( XML_VIDEO);
		if (spItem != NULL)
		{
			objDeviceErrInfo.SetVideo(spItem->text);
		
		}

        spItem = spNode->selectSingleNode( XML_BUTTON1TEXT );
        if (spItem != NULL)
        {
            objDeviceErrInfo.SetButton1Text(spItem->text);
        
        }

		aDevErrInfo.push_back(objDeviceErrInfo);

	}
	// load model info
	spNodeList = spDoc->selectNodes(XML_DEVICE_MODEL);
	FLDeviceModelInfo objDeviceModel;
	bool bSubModel = false;

	while(  spNode = spNodeList->nextNode())
	{

		objDeviceModel.Reset();
		spItem = spNode->attributes->getNamedItem( XML_DMNAME );
		spItem2 = spNode->attributes->getNamedItem( XML_DMID );
		if( spItem != NULL && spItem->text.length() && 
			spItem2 != NULL && spItem2->text.length())
		{
			 objDeviceModel.SetModel(spItem->text);
			objDeviceModel.SetModelID( _ttoi((LPCTSTR)spItem2->text));

			 spSubNodeList = spNode->selectNodes(XML_DMSUBMDOEL);
			 bSubModel = false;
			 while(  spSubNode = spSubNodeList->nextNode())
			 {
				 spSubItem = spSubNode->attributes->getNamedItem( XML_DMNAME);
				 spSubItem2 = spSubNode->attributes->getNamedItem( XML_DMID);

				 if (spSubItem != NULL && spSubItem->text.length() &&
					 spSubItem2 != NULL && spSubItem2->text.length())
				 {
					 objDeviceModel.SetSubModel(spSubItem->text);
					 objDeviceModel.SetSubModelID( _ttoi((LPCTSTR)spSubItem2->text));
					 bSubModel = true;
					 aModels.push_back(objDeviceModel);
				 } 

			 }	
			 if (bSubModel == false)
				aModels.push_back(objDeviceModel);

			 
		}
	}
	spDoc.Release();
	g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::LoadDeviceErrorMappingToList()with ret=true"));
	return true;

}


//************************************************************************ 
//
// Function:		LoadDeviceErrorMappingOverride
// Purpose:			Load error mappings in DeviceErrorXXXXEnhanced.xxx(override)
//					Compare with existing mappings and override if matched		
// Parameters:
// Returns:			true or false
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::LoadDeviceErrorMappingOverride(LPTSTR sFileName)
{
	bool bRet=true;
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::LoadDeviceErrorMappingOverride() %s"), sFileName);

	DEVICEERRORMAP aDeviceErrorInfo000;
	aDeviceErrorInfo000.clear();
	DEVICEMODELMAP aModels000;	
	aModels000.clear();

	bRet=LoadDeviceErrorMappingToList(sFileName, aDeviceErrorInfo000, aModels000 );
	
	//go though original list to see if any match. If match, override, otherwise, add.
	std::list <FLDeviceErrorInfo>::iterator IterOverride;
	for ( IterOverride = aDeviceErrorInfo000.begin( ); IterOverride != aDeviceErrorInfo000.end( ); IterOverride++ )
	{
		FLDeviceErrorInfo objDeviceErrorInfoOverride;

		objDeviceErrorInfoOverride = (FLDeviceErrorInfo)(*IterOverride);	

		FLDeviceErrorInfo objDeviceErrorInfoExist;
		

		std::list <FLDeviceErrorInfo>::iterator Iter;
		bool bFoundMatch = false;

		for ( Iter = m_aDeviceErrorInfo.begin( ); Iter != m_aDeviceErrorInfo.end( ); Iter++ )
		{
			objDeviceErrorInfoExist = (FLDeviceErrorInfo)(*Iter);	
			

			if ( (_tcsstr((LPTSTR)objDeviceErrorInfoExist.GetDeviceModel(), (LPTSTR)objDeviceErrorInfoOverride.GetDeviceModel()) != NULL  ) &&
				 (_tcsstr((LPTSTR)objDeviceErrorInfoExist.GetDeviceSubModel(), (LPTSTR)objDeviceErrorInfoOverride.GetDeviceSubModel()) != NULL || (_tcsicmp((LPTSTR)objDeviceErrorInfoExist.GetDeviceSubModel(), _T("")) == 0 &&_tcsicmp((LPTSTR)objDeviceErrorInfoOverride.GetDeviceSubModel(), _T("")) == 0)  ) &&
					(objDeviceErrorInfoExist.GetResultCode() == objDeviceErrorInfoOverride.GetResultCode() ) && 
					(objDeviceErrorInfoExist.GetExtendedResultCode() == objDeviceErrorInfoOverride.GetExtendedResultCode() ) &&
					(objDeviceErrorInfoExist.GetContext() == objDeviceErrorInfoOverride.GetContext() ))
			{
				    g_logger->Log(TRACE_INFO, _T("LoadDeviceErrorMappingOverride() found a match with %s and Err=%d"), objDeviceErrorInfoOverride.GetDeviceModel(), objDeviceErrorInfoOverride.GetResultCode()  );
					bFoundMatch = true;
					break;
			}
		}
			
		if (bFoundMatch)
		{
			g_logger->Log(TRACE_INFO, _T("LoadDeviceErrorMappingOverride() match found")  );
			m_aDeviceErrorInfo.insert(Iter,objDeviceErrorInfoOverride);
		}
		else
		{
			g_logger->Log(TRACE_INFO, _T("LoadDeviceErrorMappingOverride() No match found")  );
					
			if (objDeviceErrorInfoExist.GetDeviceModel() ==_bstr_t( _T("Any")))
				m_aDeviceErrorInfo.push_front( objDeviceErrorInfoOverride);
			else
				m_aDeviceErrorInfo.push_back( objDeviceErrorInfoOverride);

		}
	}

	//go through model list in override file
	std::list <FLDeviceModelInfo>::iterator IterOverride2;
    for ( IterOverride2 = aModels000.begin( ); IterOverride2 != aModels000.end( ); IterOverride2++ )
	{
		    FLDeviceModelInfo objDeviceModelInfoOverride;
			objDeviceModelInfoOverride = (FLDeviceModelInfo)(*IterOverride2);

			std::list <FLDeviceModelInfo>::iterator Iter;

			FLDeviceModelInfo objDeviceModelInfoExist;
			bool bMatched = false;

			for ( Iter = m_aModels.begin( ); Iter != m_aModels.end( ); Iter++ )
			{
				objDeviceModelInfoExist = (FLDeviceModelInfo)(*Iter);
				

				if( objDeviceModelInfoOverride.GetSubModelID() != -1 && objDeviceModelInfoExist.GetSubModelID() == objDeviceModelInfoOverride.GetSubModelID() )
				{
					 objDeviceModelInfoExist.SetSubModel(objDeviceModelInfoOverride.GetSubModel());
					
				}
				if (objDeviceModelInfoExist.GetModelID() == objDeviceModelInfoOverride.GetModelID()  )
				{
					objDeviceModelInfoExist.SetModel(objDeviceModelInfoOverride.GetModel());
					bMatched=true;
					m_aModels.insert(Iter,objDeviceModelInfoOverride);
					break;
				}

			}
			if (!bMatched)
				m_aModels.push_back(objDeviceModelInfoOverride);
	}
	
	
	g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::LoadDeviceErrorMappingOverride() with ret=%d"), bRet);
	return bRet;
}

bool CFLDeviceErrorMapping::IgnoreDeviceError( LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,int StatusEventId, long ExtendedResultCode)
{
	bool bRet=false;
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::IgnoreDeviceError (%s,%s,%d,%d)"), sDeviceModel,sDeviceSubModel,StatusEventId,ExtendedResultCode   );


	FLDeviceErrorInfo objDeviceErrorInfo;

	
	int nCurrEventId = -99;
	long lCurrExRetCode = -99;
	int lCurrContext = -1;
	
	
	_bstr_t sCurrDeviceModel= _T(""), sCurrDeviceSubModel = _T("");
	_bstr_t sHTMLName=_T("");
	_bstr_t sHTMLFile = _T("");


	//match error and find the html 
	std::list <FLDeviceErrorInfo>::iterator Iter;

	for ( Iter = m_aDeviceErrorInfo.begin( ); Iter != m_aDeviceErrorInfo.end( ); Iter++ )
	{
		objDeviceErrorInfo = (FLDeviceErrorInfo)(*Iter);
	
		sCurrDeviceModel = objDeviceErrorInfo.GetDeviceModel();
		sCurrDeviceSubModel= objDeviceErrorInfo.GetDeviceSubModel();
		nCurrEventId = objDeviceErrorInfo.GetResultCode();
		lCurrExRetCode = objDeviceErrorInfo.GetExtendedResultCode();		
		
		
		if ( (_tcsstr((LPTSTR)sCurrDeviceModel, sDeviceModel) != NULL || _tcsicmp(sCurrDeviceModel, _T("Any")) == 0) &&
			 (_tcsstr((LPTSTR)sCurrDeviceSubModel, sDeviceSubModel) != NULL || _tcsicmp((LPTSTR)sCurrDeviceSubModel, _T("") ) == 0 || _tcsicmp(sCurrDeviceSubModel, _T("Any")) == 0) &&
				(nCurrEventId == StatusEventId || nCurrEventId == -99) && 
				(lCurrExRetCode == ExtendedResultCode || lCurrExRetCode == -99))
		{
			    sHTMLName = objDeviceErrorInfo.GetErrorHTML();
				if (_tcsstr((LPTSTR)sHTMLName, _T("Ignore") ) !=NULL)
					bRet = true;
				break;
		}

	}

	g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::IgnoreDeviceError() with ret=%d"), bRet);
	return bRet;
}
//************************************************************************ 
//
// Function:		GetDeviceErrorHTMLFile
// Purpose:			Find/create html based on error codes		
// Parameters:
// Returns:			true or false
// Calls:			CreateDefaultDeviceErrorHTML()	
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::GetDeviceErrorHTMLFile(LPTSTR sHTML,LPTSTR sTitle1,LPTSTR sTitle2, LPTSTR sCurrentText,LPTSTR DeviceModel, LPTSTR DeviceSubModel,
						int StatusEventId, long ExtendedResultCode,   bool* bCheckHealth, int nContext, LPTSTR sButton1Text)
{
     g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::GetDeviceErrorHTMLFile()with DevModel=%s, DevSubModel=%s, Code=%d, ExtCode=%d, context=%d")
         , DeviceModel, DeviceSubModel, StatusEventId, ExtendedResultCode, nContext );
	bool bRet=false;
	
	FLDeviceErrorInfo objDeviceErrorInfo;
	_tcscpy(sHTML, _T(""));
	_tcscpy(sTitle1, _T(""));	
    _tcscpy(sButton1Text, _T(""));

	
	int nCurrEventId = -99;
	long lCurrExRetCode = -99;
	int lCurrContext = -1;
	
	
	_bstr_t sCurrDeviceModel= _T(""), sCurrDeviceSubModel = _T(""), sCurrTitle1= _T(""), sCurrTitle2= _T(""),sCurrText= _T("");
	_bstr_t sCurrImage= _T(""),sCurrVideo= _T(""),sHTMLName=_T("");
	_bstr_t sHTMLFile = _T("");
	_bstr_t sCurrButton1Text = _T("");
	TCHAR sCurrDeviceModel2[25];
	TCHAR sDeviceModel2[25];

	if (_tcslen(DeviceModel) ==0) //Tar 449891
		_tcscpy(DeviceModel, _T("Any"));

	//Tar 450302
	_tcscpy(sDeviceModel2, (LPTSTR)DeviceModel);
	for (int j=0; j<_tcslen(sDeviceModel2); j++)
	{
			
			sDeviceModel2[j] = toupper(sDeviceModel2[j]);
	}


	//match error and find the html 
	std::list <FLDeviceErrorInfo>::iterator Iter;

	for ( Iter = m_aDeviceErrorInfo.begin( ); Iter != m_aDeviceErrorInfo.end( ); Iter++ )
	{
		objDeviceErrorInfo = (FLDeviceErrorInfo)(*Iter);
	
		sCurrDeviceModel = objDeviceErrorInfo.GetDeviceModel();
		sCurrDeviceSubModel= objDeviceErrorInfo.GetDeviceSubModel();
		nCurrEventId = objDeviceErrorInfo.GetResultCode();
		lCurrExRetCode = objDeviceErrorInfo.GetExtendedResultCode();
		
		lCurrContext = objDeviceErrorInfo.GetContext();
		sCurrTitle1= objDeviceErrorInfo.GetTitle1();		
		sCurrTitle2= objDeviceErrorInfo.GetTitle2();		
		sCurrText = objDeviceErrorInfo.GetText();		
		sCurrImage=objDeviceErrorInfo.GetImage();		
		sCurrVideo = objDeviceErrorInfo.GetVideo();			
        sCurrButton1Text = objDeviceErrorInfo.GetButton1Text();

		//Tar 450302
		_tcscpy(sCurrDeviceModel2, (LPTSTR)sCurrDeviceModel);		
		for (int i=0; i<_tcslen(sCurrDeviceModel2); i++)
		{
			
			sCurrDeviceModel2[i] = toupper(sCurrDeviceModel2[i]);
		}
		
		
		if ( (_tcsstr(sCurrDeviceModel2, sDeviceModel2) != NULL || _tcsicmp(sCurrDeviceModel2, _T("ANY")) == 0) &&
			 (_tcsstr((LPTSTR)sCurrDeviceSubModel, DeviceSubModel) != NULL || _tcsicmp((LPTSTR)sCurrDeviceSubModel, _T("") ) == 0 || _tcsicmp(sCurrDeviceSubModel, _T("Any")) == 0) &&
				(nCurrEventId == StatusEventId || nCurrEventId == -99) && 
				(lCurrExRetCode == ExtendedResultCode || lCurrExRetCode == -99) &&
				(lCurrContext == nContext || lCurrContext == -1))
		{
			    sHTMLName = objDeviceErrorInfo.GetErrorHTML();
				*bCheckHealth = objDeviceErrorInfo.GetCheckHealth();                
				bRet = true;
				break;
		}

	}
 
	
	if (bRet) 
	{
		
		if (_tcslen(sHTMLName) != 0)
		{
			
			sHTMLFile = m_sHTMLPath + sHTMLName;

			//if <ErrorDisplay> file exists, do not create default html
			if (FileExist((LPTSTR)sHTMLFile) && _tcsstr((LPTSTR)sHTMLFile, _T("Ignore")) == NULL )  
			{
				    g_logger->Log(TRACE_INFO, _T("GetDeviceErrorHTMLFile() <ErrorDisplay> configured and file exists:%s "),sHTMLFile);
					_tcsncpy(sHTML, (LPTSTR)sHTMLFile, _tcslen(sHTMLFile));	
					sHTML[_tcslen(sHTMLFile)] = _T('\0');
					g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::GetDeviceErrorHTMLFile() with ret=%d"), bRet);
					return bRet;
			}
		}
		else
		{
			//create default html file name 
			TCHAR sName[_MAX_PATH], sLeft[100];
			TCHAR *s1 = NULL;

			//handle glory model name issue such as GloryRAD-75/RT-75 			
			_bstr_t sTmpModel = DeviceModel;
			s1=_tcsstr((LPTSTR)sTmpModel, _T("/"));			
			if ( s1 != NULL)
			{
				 _tcsncpy(sLeft, (LPTSTR)sTmpModel, _tcslen(sTmpModel) - _tcslen(s1));
				 sLeft[_tcslen(sTmpModel) - _tcslen(s1)]=_T('\0');

			}
			else
			{
				_tcsncpy(sLeft, (LPTSTR)sTmpModel, _tcslen(sTmpModel) );
				sLeft[_tcslen(sTmpModel)]=_T('\0');
			}


			//construct file name
			if (!_tcslen(sCurrDeviceSubModel))
				_stprintf( sName, _T("%s_%s_%d_%d_%d.htm"),(LPTSTR)m_sClassName,(LPTSTR) sLeft,nCurrEventId, lCurrExRetCode, lCurrContext);
			else
				_stprintf( sName, _T("%s_%s_%s_%d_%d_%d.htm"),(LPTSTR)m_sClassName, (LPTSTR)sLeft,(LPTSTR)sCurrDeviceSubModel,nCurrEventId, lCurrExRetCode, lCurrContext);
			
			
			sHTMLFile = m_sHTMLPath + sName; 

			g_logger->Log(TRACE_INFO, _T("GetDeviceErrorHTMLFile() creating html:%s "),(LPTSTR)sHTMLFile);

			
		}
		
	
		//if <ErrorDisplay> is not specified or specified file not exists, create the html.  
		TCHAR  *sTemplate=NULL;

		_bstr_t sImage=_T(""), sVideo=_T("");
		_bstr_t sT1=_T(""), sT2=_T(""), sText=_T(""), sErr=_T(""), sBT1=_T("");

	
		//get the strings
		bRet=GetDMString3( (LPTSTR)sCurrTitle1, sT1,(LPTSTR)sCurrTitle2, sT2, (LPTSTR)sCurrText,  sText, _T("$DMERROR_CODE$"), sErr , sCurrButton1Text, sBT1);

		if (_tcslen(sT1) != 0)
		{
			_tcsncpy(sTitle1, sT1, _tcslen(sT1));
			sTitle1[_tcslen(sT1)] = _T('\0');
		}

		if (_tcslen(sT2) != 0)
		{
			_tcsncpy(sTitle2, sT2, _tcslen(sT2));
			sTitle2[_tcslen(sT2)] = _T('\0');
		}

		if (_tcslen(sText)!=0)
		{
			_tcsncpy(sCurrentText, sText, _tcslen(sText));
			sCurrentText[_tcslen(sText)] = _T('\0');
		}
        
		if (_tcslen(sBT1) != 0)
		{
			_tcsncpy(sButton1Text, sBT1, _tcslen(sBT1));
			sButton1Text[_tcslen(sBT1)] = _T('\0');
		}
	
		if (_tcslen(sCurrImage) != 0)
		{
			sImage=m_sImagePath + sCurrImage;

			sVideo=_T("");

			sTemplate = new TCHAR[_tcslen(DEFAULT_HTML_TEMPLATE)+1];
			GET_PARTITION_PATH(DEFAULT_HTML_TEMPLATE_IMAGE, sTemplate);

		}
		else if (_tcslen(sCurrVideo) != 0)
		{

			sVideo=m_sVideoPath + sCurrVideo;
			sImage = _T("");

			sTemplate = new TCHAR[_tcslen(DEFAULT_HTML_TEMPLATE_VIDEO)+1];
			GET_PARTITION_PATH(DEFAULT_HTML_TEMPLATE_VIDEO, sTemplate);
		}
		else
		{
			sImage = _T("");
			sVideo = _T("");
			sTemplate = new TCHAR[_tcslen(DEFAULT_HTML_TEMPLATE)+1];
			GET_PARTITION_PATH(DEFAULT_HTML_TEMPLATE, sTemplate);
		}	
	
		TCHAR *s1=NULL;

		s1 = _tcsstr( (LPTSTR)sHTMLFile, _T("Ignore") );  //frist sentences

		if (s1 != NULL)
			bRet = true;
		else	
			bRet=CreateDefaultDeviceErrorHTML(sTemplate, sHTMLFile,(LPTSTR)sT1,(LPTSTR) sT2, (LPTSTR)sText, (LPTSTR)sImage,(LPTSTR)sVideo, (LPTSTR)m_sLanguageID );
		delete[] sTemplate;
		sTemplate=NULL;
			
							
		if (bRet)
		{
			_tcsncpy(sHTML, sHTMLFile, _tcslen(sHTMLFile));
			sHTML[_tcslen(sHTMLFile)] = _T('\0');
			if (s1 == NULL) {
				SetModelIDs(DeviceModel, DeviceSubModel);
				bRet = ParseErrorCode(sHTMLFile, sErr, m_nDeviceClass, StatusEventId, ExtendedResultCode, _T("%s"));
			}
		}
		//else will be use default DeviceErrorUnknown.htm	        
		
	}
	//else will be use default DeviceErrorUnknown.htm
	

    g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::GetDeviceErrorHTMLFile() with ret=%d"), bRet);

	return bRet;
}
//************************************************************************ 
//
// Function:		FileExist
// Purpose:			check html file exist or not		
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::FileExist(LPTSTR csFileName)
{
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::FileExist() %s"), csFileName);

	MSXML2::IXMLDOMDocumentPtr spDoc;
	HRESULT hr;

	hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??") );
        return false;
    }
	spDoc->async = VARIANT_FALSE; 
	if( spDoc->load( csFileName ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("File %s does not exist"), csFileName);
		g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::FileExist() with ret=false"));
        spDoc.Release();
		return false;
	}
	else
	{
		g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::FileExist() with ret=true"));
		spDoc.Release();
		return true;
	}
}

//************************************************************************ 
//
// Function:        GetString
// Purpose:         Get the corresponding string from the string table 
//                  with StringID input
// Parameters:
// Returns:         String from string table
// Calls:               
// Side Effects:
// Notes:
//
//************************************************************************
_bstr_t CFLDeviceErrorMapping::GetString(MSXML2::IXMLDOMDocumentPtr spDoc, MSXML2::IXMLDOMDocumentPtr spDoc0,
                                         bool bMsg0, MSXML2::IXMLDOMDocumentPtr spDocTerm, bool bTermMsg, LPTSTR sInput)
{
    MSXML2::IXMLDOMNodePtr spNode, spNode0, spNodeTerm;
    TCHAR sTag[_MAX_PATH];
    GetDMMsgTag(sInput, sTag);
    _bstr_t sResult = _T("");

    if (spDoc)
       spNode = spDoc->selectSingleNode(sTag);
    
    if (spDoc0 != NULL && bMsg0 == true)
       spNode0 = spDoc0->selectSingleNode(sTag);
    
    if (spDocTerm != NULL && bTermMsg == true)
       spNodeTerm = spDocTerm->selectSingleNode(sTag);

    if (spNode)
        sResult = spNode->text;

    if (spNode0)
        sResult = spNode0->text;

    if (spNodeTerm)
        sResult = spNodeTerm->text;

    return sResult;
}
//************************************************************************ 
//
// Function:		GetDMMsgTag
// Purpose:			Parse the msg tag such as 	<Title1>$DMCashAcceptor_1130$</Title1>	
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************
 bool CFLDeviceErrorMapping::GetDMMsgTag(LPTSTR sT1, LPTSTR sTag1)
 {
	 
	 TCHAR* s1=NULL, *s2=NULL;
	_tcscpy(sTag1, XML_MSG_TABLE);
	if (_tcslen(sT1) != 0)
	{
		
		s1=_tcsstr((LPTSTR)sT1, _T("$"));	
		if (s1)
		{
			if (_tcslen(s1) > 1)
			{
				s2 = _tcsstr((LPTSTR)s1+1, _T("$"));
				if (s2)			
					_tcsncat((LPTSTR)sTag1, s1+1, s2-s1-1); //parse $DMCashAcceptor_1130$			
				else
					_tcsncat((LPTSTR)sTag1, s1+1, _tcslen(s1)-1); //parse $DMCashAcceptor_1130
			}
			else
				_tcsncat((LPTSTR)sTag1, sT1, _tcslen(sT1)-1); //parse DMCashAcceptor_1130$


		}
		else
			_tcsncat((LPTSTR)sTag1, sT1, _tcslen(sT1)); //parse no $ in DMCashAcceptor_1130
	}
	else
		_tcscpy(sTag1, _T(""));
		return true;


 }
//************************************************************************ 
//
// Function:		GetDMString3
// Purpose:			Get DM msgs based on msg tag in DMMsg*.xml		
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::GetDMString3( LPTSTR sT1, _bstr_t& sResult1,  LPTSTR sT2, _bstr_t& sResult2,  LPTSTR sTxt, _bstr_t& sResult3 , LPTSTR sErrorCode, _bstr_t&sErrCode, LPTSTR sBT1, _bstr_t& sBT1Result)
	

{
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::GetDMString3()"));
	bool bRet=true;
	MSXML2::IXMLDOMDocumentPtr spDoc, spDocTerm, spDoc0;
	HRESULT hr;

	sResult1=_T("");
	sResult2=_T("");
	sResult3=_T("");
	sErrCode=_T("");
    sBT1Result=_T("");

	bool bMsg0=false;
	bool bTermMsg=false;

	hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("GetDMString3() failed to create XML DOM Object. IS MS XML INSTALLED??") );
        return false;
    }
	spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
	if( spDoc->load( m_sMsgFile ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("GetDMString3() failed to load %s"), (LPTSTR)m_sMsgFile );
        spDoc.Release();
		return false;
	}

	hr = spDoc0.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("GetDMString3() failed to create XML DOM Object. IS MS XML INSTALLED??") );
		spDoc0.Release();
        return false;
    }
	spDoc0->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
	if( spDoc0->load( m_sMsgFile0 ) == VARIANT_TRUE )
	{
		g_logger->Log(TRACE_INFO, _T("GetDMString3() override file %s exists"), m_sMsgFile0 );
		bMsg0 = true;
	}

	hr = spDocTerm.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("GetDMString3() failed to create XML DOM Object. IS MS XML INSTALLED??") );
		spDocTerm.Release();
        return false;
    }
	spDocTerm->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
	if( spDocTerm->load( m_sTerminalMsgFile ) == VARIANT_TRUE )
	{
		g_logger->Log(TRACE_INFO, _T("GetDMString3() override file %s exists"), m_sTerminalMsgFile );
		bTermMsg = true;
	}

    try {
    //Get title1
    if (_tcslen(sT1) != 0)
        sResult1 = GetString(spDoc, spDoc0, bMsg0, spDocTerm, bTermMsg, sT1);

    //Get title2
    if (_tcslen(sT2) != 0)
        sResult2 = GetString(spDoc, spDoc0, bMsg0, spDocTerm, bTermMsg, sT2);
        
    //Get Text
    if (_tcslen(sTxt) != 0)
        sResult3 = GetString(spDoc, spDoc0, bMsg0, spDocTerm, bTermMsg, sTxt);
        
    //Get Button1Text
    if (_tcslen(sBT1) != 0)
        sBT1Result = GetString(spDoc, spDoc0, bMsg0, spDocTerm, bTermMsg, sBT1);

    if (_tcslen(sErrorCode) != 0)
        sErrCode = GetString(spDoc, spDoc0, bMsg0, spDocTerm, bTermMsg, sErrorCode);

	}
	catch(...)
	{
		bRet = false;
		g_logger->Log(TRACE_ERROR, _T("GetDMString3() Exception in getting messages") );
		
	}

	//if all T1, T2 and sText all empty
	if (_tcslen(sResult1) == 0 && _tcslen(sResult2) == 0 && _tcslen(sResult3) == 0)
		{
			g_logger->Log(TRACE_ERROR, _T("GetDMString3() T1, T2 and Text are all empty") );
			bRet = false;
		}

		spDoc.Release();
		spDoc0.Release();
	    spDocTerm.Release();

  g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::GetDMString3() with ret=%d"), bRet);
  
  return bRet;
}

//************************************************************************ 
//
// Function:        ParseDeviceErrorHTMLTitle
// Purpose:         Updates device error html title     
// Parameters:      spParagraph - XML Node pointer
//                  csTitle  - String that holds the new title message.
// Returns:         true or false
// Calls:               
// Side Effects:
// Notes:           Added SSCOB-517
//
//************************************************************************
bool CFLDeviceErrorMapping::ParseDeviceErrorHTMLTitle(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csTitle)
{
    if (_tcslen(csTitle) == 0) 
        return false;

    g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::ParseDeviceErrorHTMLTitle() with title1=%s"), csTitle);

    MSXML2::IXMLDOMNodeListPtr spSubNodeList;
    MSXML2::IXMLDOMNodePtr spNodeText, spNode,spItem, spNodeLast;
    bool bRet = false;

    spSubNodeList=spParagraph->selectNodes(XML_HTML_TITLE2); 

    if (spSubNodeList != NULL  && _tcslen(csTitle) != 0)
    {
        spNodeLast = spNode = spSubNodeList->nextNode();
    }


    if (spNodeLast != NULL)
    {
            spItem = spNodeLast->attributes->getNamedItem( XML_CLASS );
            if( spItem != NULL && spItem->text.length() )
            {
                 _bstr_t sTmp=spItem->text;
                 if ( _tcscmp((LPTSTR)sTmp, _T("Title")) == 0)
                 {
                    _bstr_t sRight2 = csTitle;
                    spNodeLast->put_text(sRight2);
                    spNodeText = spNodeLast;
                    bRet = true;
                 }
            }
    }

    g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::ParseDeviceErrorHTMLTitle() with ret=%d"), bRet);
    return bRet;
}


//************************************************************************ 
//
// Function:        ParseDeviceErrorHTMLSubTitle
// Purpose:         Update device error html sub title      
// Parameters:      spParagraph - XML Node pointer
//                  csSubTitle  - String that holds the new sub title message.
// Returns:         true or false
// Calls:               
// Side Effects:
// Notes:           Added SSCOB-517
//  
//************************************************************************
bool CFLDeviceErrorMapping::ParseDeviceErrorHTMLSubTitle(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csSubTitle)
{
    if (_tcslen(csSubTitle) == 0) 
        return false;

    g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::ParseDeviceErrorHTMLSubTitle() with title2=%s"), csSubTitle);
    
    MSXML2::IXMLDOMNodeListPtr spSubNodeList;
    MSXML2::IXMLDOMNodePtr spNodeText, spNode,spItem, spNodeLast;
    bool bRet = false;

    spSubNodeList=spParagraph->selectNodes(XML_HTML_TITLE2); 

    if (spSubNodeList != NULL  && _tcslen(csSubTitle) != 0)
    {
        while (spNode = spSubNodeList->nextNode())
		{
			spItem = spNode->attributes->getNamedItem( XML_CLASS );
            if( spItem != NULL && spItem->text.length() )
            {
                 _bstr_t sTmp = spItem->text;
                 if ( _tcscmp((LPTSTR)sTmp, _T("SubTitle")) == 0)
                 {
                    _bstr_t sRight2 = csSubTitle;
                    spNode->put_text(sRight2);
                    spNodeText = spNode;
                    bRet = true;
                 }
            }
		}
    }

    g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::ParseDeviceErrorHTMLSubTitle()  with ret=%d"), bRet);
    return bRet;
}

//************************************************************************ 
//
// Function:		ParseTextNewLine
// Purpose:			parse new line into html nodes		
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::ParseTextNewLine(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csText, bool bLastNode)
{
	if (_tcslen(csText) == 0) //Tar 449502
		return false;

	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::ParseTextNewLine() with text=%s"), csText);
	MSXML2::IXMLDOMNodeListPtr spSubNodeList;
	MSXML2::IXMLDOMNodePtr spNodeText, spNode,spItem, spNodeLast;

	if (bLastNode == false)
	{
		spSubNodeList=spParagraph->selectNodes(XML_HTML_TEXT); 

		if (spSubNodeList != NULL  && _tcslen(csText) != 0)
		{
			while (spNode = spSubNodeList->nextNode())
			{
				spNodeLast = spNode;
				 _bstr_t sTmp2=spNodeLast->text;
			}
		}
	}
	else
		spNodeLast = spParagraph;

	//Text
	if (spNodeLast != NULL)
	{
			spItem = spNodeLast->attributes->getNamedItem( XML_CLASS );
			if( spItem != NULL && spItem->text.length() )
			{

				 _bstr_t sTmp=spItem->text;

				 if ( _tcscmp((LPTSTR)sTmp, _T("ParagraphBold")) == 0)
				 {
					 TCHAR *s1=NULL,*sLeft=NULL;

					s1 = _tcsstr( csText, _T("\\n") );  //frist sentences

					_bstr_t sRight2 = csText;
					TCHAR * sRight=csText;
				
					if (s1 == NULL )
					{
						spNodeLast->put_text(sRight2);
					}
					spNodeText = spNodeLast;
					while (s1 != NULL)
					{
						if (_tcslen(sRight) - _tcslen(s1) !=0)
						{
							int nLen = (_tcslen(sRight) - _tcslen(s1));
							sLeft = new TCHAR[nLen+1];
							_tcsncpy((LPTSTR)sLeft, sRight, nLen);
							sLeft[nLen]=_T('\0');
							_bstr_t bstrLeft=sLeft;
							spNodeText->put_text(bstrLeft);
							spNodeText = spNodeLast->cloneNode(false);
							spParagraph->appendChild(spNodeText);
							delete[] sLeft;
						}
						sRight = s1+ _tcslen( _T("\\n")) ;
						s1=_tcsstr( (LPTSTR)sRight, _T("\\n"));						
						if (s1 == NULL)
						{
							sRight2=sRight;
							spNodeText->put_text(sRight2);
							break;
						}

					}		
				
					
				 }
				
			}
		
		
	}
	g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::ParseTextNewLine()  with ret=true"));
	return true;
}
//************************************************************************ 
//
// Function:		CreateDefaultDeviceErrorHTML
// Purpose:			create html using html template and text		
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************

bool CFLDeviceErrorMapping::CreateDefaultDeviceErrorHTML(LPTSTR sHTMLTemplate, LPTSTR csHTMLFile,  LPTSTR csTitle1, LPTSTR csTitle2,
					LPTSTR csText, LPTSTR csImage , LPTSTR sVideo, LPTSTR sLCID)
{
	 
	g_logger->Log(TRACE_API, _T("+CFLDeviceErrorMapping::CreateDefaultDeviceErrorHTML()"));

	
	MSXML2::IXMLDOMDocumentPtr spDoc;
	MSXML2::IXMLDOMNodeListPtr spNodeList, spNodeListInput, spSubNodeList,spSubNodeList1, spSubNodeList2,spSubNodeList3, spSubNodeVideoParms, spVideoPlayBtn;
	MSXML2::IXMLDOMNodePtr spParagraph, spNodeText, spNode, spNodeInput, spNode2, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem, spImgParent, spImg, spVideoParent;
	


	HRESULT hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("CreateDefaultDeviceErrorHTML() failed to create XML DOM Object. IS MS XML INSTALLED??") );
        return false;
    }
	spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
	if( hr=spDoc->load( sHTMLTemplate ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("CreateDefaultDeviceErrorHTML() failed to load %s"), sHTMLTemplate );
        spDoc.Release();
		return false;
	}

	spNodeList = spDoc->selectNodes(XML_HTML_DIV);

	while(  spNode = spNodeList->nextNode())
	{
		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			  _bstr_t sTmp = spItem->text;

			 if (_tcscmp((LPTSTR)sTmp, _T("Layer1")) == 0)
			 {
				 spParagraph = spNode;
				 spSubNodeList=spNode->selectNodes(XML_HTML_TEXT);  //text
				
			 }
			 if (_tcscmp((LPTSTR)sTmp, _T("SubTitleMid")) == 0)
			 {
				 spSubNodeList2 = spNode->selectNodes(XML_HTML_TITLE2);  //Title2
			 }
			 if (_tcscmp((LPTSTR)sTmp, _T("TopBanner")) == 0)
			 {
				 spSubNodeList3 = spNode->selectNodes(XML_HTML_TITLE2);  //title1
			 }
			 if (_tcscmp((LPTSTR)sTmp, _T("DVbitmap")) == 0)
			 {
				 spImgParent = spNode;
				 spSubNodeList1 = spNode->selectNodes(XML_HTML_IMG);  //image
			 }
			  if (_tcscmp(sTmp, _T("VideoWindow")) == 0)
			 {
				 spVideoParent = spNode;
				 spSubNodeVideoParms = spNode->selectNodes(XML_HTML_VIDEO_PARAM);  //video
			 }
			  if (_tcscmp((LPTSTR)sTmp, _T("BtnPlayLayer")) == 0)
			  {
				  spVideoPlayBtn = spNode->selectNodes(XML_HTML_VIDEO_BTNPLAY); //video play button

			  }
			 
		}
	}

	//Title1
	if (spSubNodeList3 != NULL && _tcslen(csTitle1) != 0)
	{
		while (spNode3 = spSubNodeList3->nextNode())
		{
						
			spItem = spNode3->attributes->getNamedItem( XML_CLASS );
			if( spItem != NULL && spItem->text.length() )
			{
				 _bstr_t sTmp=spItem->text;

				 if (_tcscmp((LPTSTR)sTmp, _T("Title")) == 0 )
				 {
					 
					 _bstr_t bstrTitle = csTitle1;	
					 spNode3->put_text(bstrTitle);
				 }
			}
			
		}
	}

	//Title2
	if (spSubNodeList2 != NULL && _tcslen(csTitle2) != 0  )
	{
		while (spNode2 = spSubNodeList2->nextNode())
		{
			
			spItem = spNode2->attributes->getNamedItem( XML_CLASS );
			if( spItem != NULL && spItem->text.length() )
			{
				  _bstr_t sTmp=spItem->text;

				 if (_tcscmp((LPTSTR)sTmp, _T("SubTitle")) == 0 )
				 {
					 _bstr_t bstrTitle2=csTitle2;
					 spNode2->put_text(bstrTitle2);			
				 }
			}
			
		}
	}
	//Text
	ParseTextNewLine(spParagraph,  csText);


		//image	
		if (spSubNodeList1 != NULL )
		{
			while (spNode = spSubNodeList1->nextNode())
			{
				
				spItem = spNode->attributes->getNamedItem( XML_IMGSRC);
				if( spItem != NULL && _tcslen(csImage) != 0) 
				{
					_bstr_t bstrImage=csImage;
					spItem->put_text(bstrImage);
				}
				else if (spImgParent != NULL &&  _tcslen(csImage)==0 )
				{
					spImgParent->removeChild(spNode);
				}
			}
			
		}

		//video
		_bstr_t bstrVideo=_T("");
		if (spSubNodeVideoParms != NULL )
		{
			while (spNode = spSubNodeVideoParms->nextNode())
			{
				
				spItem = spNode->attributes->getNamedItem(XML_VIDEO_NAME );
				if( spItem != NULL) 
				{

					_bstr_t sTmp = spItem->text;

					if (_tcscmp((LPTSTR)sTmp, _T("URL")) == 0)
					{
						if (_tcscmp(sVideo, _T("")) != 0) 
						{
							spItem = spNode->attributes->getNamedItem(XML_VIDEO_VALUE );
							bstrVideo = sVideo;
							if (spItem != NULL)
								spItem->put_text(bstrVideo);

						}
						//else remove all video nodes

					}
					
				}
				
			}
			
		}
		//video play button
		if (spVideoPlayBtn != NULL )
		{
			while (spNode = spVideoPlayBtn->nextNode())
			{
				
				spItem = spNode->attributes->getNamedItem(XML_VIDEO_ONCLICK );
				if( spItem != NULL) 
				{
				
					_bstr_t sTmp = spItem->text;

					if (_tcscmp((LPTSTR)sTmp, _T("StartMeUp()")) == 0)
					{
					
							_bstr_t sStartVideo=_T("StartMeUp('");

							sStartVideo += bstrVideo;
							sStartVideo +=_T("')");
							spItem->put_text(sStartVideo);


					}
				
				}
				
			}
			
		}
	


	//save to html file
	if (_tcscmp(csHTMLFile, _T("")) !=0)
	{
        try
        {
            spDoc->save(csHTMLFile);
        }
        catch(_com_error& e)
        {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
            g_logger->Log(TRACE_ERROR, _T("Exception caught! The change to the HTML error text for file %s failed to save."), (LPTSTR)csHTMLFile);
            g_logger->Log(
                TRACE_ERROR,
                _T("Error\n\tCode = 0x%08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
                e.Error(),
                e.ErrorMessage(),
                (LPCTSTR) bstrSource,
                (LPCTSTR) bstrDescription
            );
            spDoc.Release();
            return false;
        }
	}
	
	spDoc.Release();
  	
	g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::CreateDefaultDeviceErrorHTML() with ret=true"));

    return true;

}
//************************************************************************ 
//
// Function:		SetModelIDs
// Purpose:			create html using html template and text		
// Parameters:
// Returns:			true or false
// Calls:				
// Side Effects:
// Notes:
//
//************************************************************************
bool  CFLDeviceErrorMapping::SetModelIDs(LPTSTR DeviceModel,LPTSTR DeviceSubModel)
{
    g_logger->Log(TRACE_INFO, _T("+CFLDeviceErrorMapping::SetModelIDs()with DevModel=%s, DevSubModel=%s"), DeviceModel,DeviceSubModel );
	bool bRet = false;
	std::list <FLDeviceModelInfo>::iterator Iter;

	FLDeviceModelInfo objDeviceModelInfo;
	int k = -1;
    m_nCurrModel = 0;
    m_nCurrSubModel = 0;

	for ( Iter = m_aModels.begin( ); Iter != m_aModels.end( ); Iter++ )
	{
		k = k+1;  //index of the model, 0 based
		objDeviceModelInfo = (FLDeviceModelInfo)(*Iter);
		if (_tcsicmp(objDeviceModelInfo.GetModel(), DeviceModel) == 0 && 
			 (_tcsicmp(objDeviceModelInfo.GetSubModel(), DeviceSubModel) == 0 || _tcsicmp(objDeviceModelInfo.GetSubModel(), _T("")) == 0) )
		{
			m_nCurrModel = objDeviceModelInfo.GetModelID();
			m_nCurrSubModel = objDeviceModelInfo.GetSubModelID();
			bRet=true;
			break;
		}


	}
    g_logger->Log(TRACE_INFO, _T("-CFLDeviceErrorMapping::SetModelIDs()  with ret=%d"), bRet);
	return bRet;
	
	

}

//************************************************************************ 
//
// Function:		ParseErrorCode
// Purpose:			Fill in the error code string DevClass-Model[-submodel]-resultcode-extcode-context		
// Parameters:
// Returns:			true or false
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool CFLDeviceErrorMapping::ParseErrorCode(LPTSTR sHTMLFile, LPTSTR sErrCode, int nDeviceClass, int nStatusId, long nExtCode, LPTSTR sToken)
{
	
    g_logger->Log(TRACE_INFO, _T("+CFLDeviceErrorMapping::ParseErrorCode() with html=%s, DevClass=%d, Code=%d, ExtCode=%d, Token=%s"),
       sHTMLFile, nDeviceClass, nStatusId, nExtCode, sToken );

	MSXML2::IXMLDOMDocumentPtr spDoc;
    HRESULT hr;
	bool bRet=false;

    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		g_logger->Log(TRACE_ERROR, _T("Failed to create XML DOM Object. IS MS XML INSTALLED??") );
        g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::ParseErrorCode()"));
		spDoc.Release();
        return false;
    }

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load

	
	if( spDoc->load( sHTMLFile ) != VARIANT_TRUE )
	{
		g_logger->Log(TRACE_ERROR, _T("Failed to load file %s"), sHTMLFile );
        g_logger->Log(TRACE_ERROR, _T("-CFLDeviceErrorMapping::ParseErrorCode()"));
        spDoc.Release();
		return false;
	}


	MSXML2::IXMLDOMNodeListPtr spNodeList, spSubNodeList, spSubNodeList2, spSubNodeList3;
	MSXML2::IXMLDOMNodePtr spNode, spNode2, spNode3, spSubNode, spSubNode2, spItem, spItem2, spSubItem2, spSubItem;
	

	//spNodeList = spDoc->selectNodes(XML_HTML_DIV);
	spNodeList = spDoc->selectNodes(XML_HTML_TITLE2);

	while(  spNode = spNodeList->nextNode())
	{
		spItem = spNode->attributes->getNamedItem( XML_ID );
		if( spItem != NULL && spItem->text.length() )
		{
			_bstr_t sTmp=spItem->text;
			 
			 if (_tcscmp((LPTSTR)sTmp, _T("ErrorCode2")) == 0)
			 {
				 spNode3 = spNode;
			 }
			 
		}
	}

	bool bParsed = false;
	
	
	if (spNode3 != NULL)
	{
			bParsed = true;
			bRet = true;
			TCHAR   sTmp5[_MAX_PATH];
			_bstr_t sTmp2 = spNode3->text;
			
			memset(sTmp5,0,_MAX_PATH * sizeof(_TCHAR));


			TCHAR *s1 = NULL;
			sTmp5[0]=NULL;

			s1 = _tcsstr((LPTSTR) sTmp2, sToken );
			if (s1 != NULL)
			{
				/*int x = _tcslen(sTmp2)-_tcslen(s1);
				sTmp3=new TCHAR[_tcslen(sTmp2)-_tcslen(s1)+1];
				sTmp3[0]=NULL;
				_tcsncat(sTmp3, sTmp2, s1-sTmp2);
				sTmp3[_tcslen(sTmp3)]=_T('\0');
				*/
			

				if (m_nCurrSubModel != -1)
					_stprintf(sTmp5, _T("%s %d-%d-%d-%d-%d."), sErrCode, nDeviceClass,m_nCurrModel, m_nCurrSubModel, nStatusId, nExtCode); 
				else
					_stprintf(sTmp5, _T("%s %d-%d-%d-%d."), sErrCode, nDeviceClass, m_nCurrModel, nStatusId, nExtCode); 
			}	
		
			//delete[] sTmp3;
		    // sTmp3=NULL;
			
			if (bRet == true && _tcslen(sTmp5) != 0)
				spNode3->put_text(_bstr_t(sTmp5));	
		
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
    }


	spDoc.Release();
    g_logger->Log(TRACE_API, _T("-CFLDeviceErrorMapping::ParseErrorCode()  with ret=%d"), bRet);
	return bRet;
}


