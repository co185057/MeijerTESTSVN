// FLDeviceErrorMapping.h: interface for the CFLDeviceErrorMapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLDEVICEERRORMAPPING_H__3DAC0135_67D9_4E4A_8130_0974A4F043B1__INCLUDED_)
#define AFX_FLDEVICEERRORMAPPING_H__3DAC0135_67D9_4E4A_8130_0974A4F043B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
typedef struct
{
	_bstr_t sModel;
	_bstr_t sSubModel;
	int nResultCode;
	long lExtendedResultCode;
	_bstr_t sErrorHTML;
	bool bCheckHealth;
	int nContext;
	_bstr_t sTitle1;
	_bstr_t sTitle2;
	_bstr_t sText;
	_bstr_t sImage;
	_bstr_t sVideo;
} FLDeviceErrorInfo;
*/

class FLDeviceErrorInfo
{
public:
	FLDeviceErrorInfo() {
		m_nResultCode=-1;
		m_lExtendedResultCode=-1;	
		m_bCheckHealth=false;
		m_nContext=-1;
		m_sModel=_T("");
		m_sSubModel=_T("");
		m_sErrorHTML=_T("");
		m_sTitle1=_T("");
		m_sTitle2=_T("");
		m_sText=_T("");
		m_sImage=_T("");
		m_sVideo=_T("");
		m_sButton1Text=_T("");
	}
	_bstr_t &GetTitle1() {return m_sTitle1;}
	void SetTitle1(_bstr_t & sTitle1) { m_sTitle1=sTitle1;}

	_bstr_t &GetTitle2() {return m_sTitle2;}
	void SetTitle2(_bstr_t & sTitle2) { m_sTitle2=sTitle2;}

	_bstr_t &GetText() {return m_sText;}
	void SetText(_bstr_t & sText) { m_sText=sText;}

	_bstr_t &GetDeviceModel() {return m_sModel;}
	void SetDeviceModel(_bstr_t & sModel) { m_sModel=sModel;}

	_bstr_t &GetDeviceSubModel() {return m_sSubModel;}
	void SetDeviceSubModel(_bstr_t & sSubModel) { m_sSubModel=sSubModel;}

	_bstr_t &GetErrorHTML() {return m_sErrorHTML;}
	void SetErrorHTML(_bstr_t & sErrorHTML) { m_sErrorHTML=sErrorHTML;}

	_bstr_t &GetImage() {return m_sImage;}
	void SetImage(_bstr_t & sImage) { m_sImage=sImage;}

	_bstr_t &GetVideo() {return m_sVideo;}
	void SetVideo(_bstr_t & sVideo) { m_sVideo=sVideo;}

	_bstr_t &GetButton1Text() {return m_sButton1Text;}
	void SetButton1Text(_bstr_t & sButton1Text) { m_sButton1Text=sButton1Text;}

	int GetResultCode() { return m_nResultCode;}
	void SetResultCode(int nResultCode) { m_nResultCode = nResultCode;}

	long GetExtendedResultCode() { return m_lExtendedResultCode;}
	void SetExtendedResultCode(long lExtendedResultCode) { m_lExtendedResultCode = lExtendedResultCode;}

	int GetContext() { return m_nContext;}
	void SetContext(int nContext) { m_nContext = nContext;}


	bool GetCheckHealth() {return m_bCheckHealth;}
	void SetCheckHealth(bool bCheckHealth) {m_bCheckHealth = bCheckHealth;}

	void Reset() 
	{
		m_nResultCode=-1;
		m_lExtendedResultCode=-1;	
		m_bCheckHealth=false;
		m_nContext=-1;
		m_sModel=_T("");
		m_sSubModel=_T("");
		m_sErrorHTML=_T("");
		m_sTitle1=_T("");
		m_sTitle2=_T("");
		m_sText=_T("");
		m_sImage=_T("");
		m_sVideo=_T("");
		m_sButton1Text=_T("");
	}


private:	
	int m_nResultCode;
	long m_lExtendedResultCode;	
	bool m_bCheckHealth;
	int m_nContext;
	_bstr_t m_sModel;
	_bstr_t m_sSubModel;
	_bstr_t m_sErrorHTML;
	_bstr_t m_sTitle1;
	_bstr_t m_sTitle2;
	_bstr_t m_sText;
	_bstr_t m_sImage;
	_bstr_t m_sVideo;
	_bstr_t m_sButton1Text; // SSCOADK-2896
};

/*
typedef struct
{
	_bstr_t sModel;
	int nModelID;
	_bstr_t sSubModel;
	int nSubModelID;
} FLDeviceModelInfo;
*/
class FLDeviceModelInfo
{
public:
	FLDeviceModelInfo() {
		m_sModel=_T("");
		m_sSubModel = _T("");
		m_nModelID = 0;
		m_nSubModelID = 0;
	}

	_bstr_t &GetModel() {return m_sModel;}
	void SetModel(_bstr_t & sModel) { m_sModel=sModel;}

	_bstr_t &GetSubModel() {return m_sSubModel;}
	void SetSubModel(_bstr_t & sSubModel) { m_sModel=sSubModel;}

	int GetSubModelID() { return m_nSubModelID;}
	void SetSubModelID(int nSubModelID) { m_nSubModelID = nSubModelID;}

	int GetModelID() { return m_nModelID;}
	void SetModelID(int nModelID) { m_nModelID = nModelID;}
	void Reset() {
		m_sModel=_T("");
		m_sSubModel = _T("");
		m_nModelID = 0;
		m_nSubModelID = 0;
	}

private:
	_bstr_t m_sModel;	
	_bstr_t m_sSubModel;
	int m_nModelID;
	int m_nSubModelID;

};

typedef std::list<FLDeviceErrorInfo> DEVICEERRORMAP;
typedef std::list<FLDeviceModelInfo> DEVICEMODELMAP;

class CFLDeviceErrorMapping  
{
public:
	CFLDeviceErrorMapping() ;

	virtual ~CFLDeviceErrorMapping();

	bool LoadDeviceErrorMapping(LPTSTR sFileName,int nDeviceClass, _bstr_t & sClassName,_bstr_t & sDefaultPath, _bstr_t & sImagePath, _bstr_t & sConfigPath, _bstr_t & sLanguageID, long nTermNum=-1);
	bool LoadDeviceErrorMappingOverride(LPTSTR sFileName);


	bool IgnoreDeviceError( LPTSTR sDeviceModel, LPTSTR sDeviceSubModel,int StatusEventId, long ExtendedResultCode);
	bool GetDeviceErrorHTMLFile(LPTSTR sHTML, LPTSTR sTitle1, LPTSTR sTitle2,LPTSTR sCurrText, LPTSTR DeviceModel, LPTSTR DeviceSubModel,
						int StatusEventId, long ExtendedResultCode, bool* bCheckHealth, int nContext, LPTSTR sButton1Text = _T(""));

	bool ParseErrorCode(LPTSTR sHTMLFile, LPTSTR sErrCode, int nDeviceClass, int nStatusId, long nExtCode, LPTSTR sToken);
        
    bool ParseTextNewLine(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csText, bool bLastNode=false);

    bool ParseDeviceErrorHTMLTitle(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csTitle); //SSCOB-517

    bool ParseDeviceErrorHTMLSubTitle(MSXML2::IXMLDOMNodePtr spParagraph, LPTSTR csSubTitle);   ////SSCOB-517


protected:
    bool GetDMMsgTag(LPTSTR sT1, LPTSTR sTag1);
    _bstr_t GetString(MSXML2::IXMLDOMDocumentPtr spDoc, MSXML2::IXMLDOMDocumentPtr spDoc0, bool bMsg0, MSXML2::IXMLDOMDocumentPtr spDocTerm, bool bTermMsg, LPTSTR sInput);
    bool  FileExist(LPTSTR csFileName);
	bool  SetModelIDs(LPTSTR DeviceModel, LPTSTR DeviceSubModel);
	bool  CreateDefaultDeviceErrorHTML(LPTSTR sHTMLTemplate, LPTSTR csHTMLFile,  LPTSTR csTitle1, LPTSTR csTitle2,
					LPTSTR csText, LPTSTR csImage , LPTSTR csVideo , LPTSTR sLCID);

	bool  GetDMString3( LPTSTR sT1, _bstr_t& sResult1,  LPTSTR sT2, _bstr_t& sResult2,  LPTSTR sTxt, _bstr_t& sResult3, LPTSTR sErrorCode, _bstr_t&sErrCode, LPTSTR sBT1, _bstr_t& sBT1Result);
	
	bool LoadDeviceErrorMappingToList(LPTSTR sFileName, DEVICEERRORMAP &aDevErrInfo,DEVICEMODELMAP &aModels );


	DEVICEERRORMAP m_aDeviceErrorInfo;
	DEVICEMODELMAP m_aModels;
	
	
	_bstr_t m_sDefaultPath;
	_bstr_t m_sVideoPath;
	_bstr_t m_sImagePath;	
	_bstr_t m_sConfigPath;
	_bstr_t m_sLanguageID;
	_bstr_t m_sHTMLPath;
	_bstr_t m_sClassName;
	_bstr_t m_sMsgFile;
	_bstr_t m_sMsgFile0;
	_bstr_t m_sTerminalMsgFile;
	int m_nDeviceClass;
	int m_nCurrModel;
	int m_nCurrSubModel;



};

#endif // !defined(AFX_FLDEVICEERRORMAPPING_H__3DAC0135_67D9_4E4A_8130_0974A4F043B1__INCLUDED_)
