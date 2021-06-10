///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   CustomerInfoBase.CPP
//          Retrieves customer data from Customer web service and collects/sends
//          customer metrics
//
// AUTHOR:  Barbara Dawkins
// 
/////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerInfoBase.h"          // Track events 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CCustomerInfoBase")

#define REQUEST_UNINITIALIZED 0
#define REQUEST_LOADING 1
#define REQUEST_LOADED 2
#define REQUEST_INTERACTIVE 3
#define REQUEST_COMPLETE 4

#define PERSONALIZATIONKEY        _T("SOFTWARE\\NCR\\SCOT\\Installation\\CorePersonalizationServer")                            
#define PERSONALIZATIONSERVERNAME    _T("PersonalizationServer")

const TCHAR XML_CUSTINFOCFGFILE[] = _T("CustomerInfoService");

bool CCustomerInfoBase::m_bInitialized = false;

IMPLEMENT_DYNCREATE(CCustomerInfoBase, CObject)// MFC Runtime class/object information
// Constructor
//////////////////////////////////////////////////////////////////////////////////
CCustomerInfoBase::CCustomerInfoBase():
m_pWebRequest(NULL),
m_pXMLHTTPEvent(NULL),
m_bstrCustomerInfo(_T("")),
m_bOnline(false),
m_csServerName(_T(""))
{
	m_bInitialized = false;
}
///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////
CCustomerInfoBase::~CCustomerInfoBase()
{
	if	( m_bInitialized )
	{
		UnInitialize();
	}
}


///////////////////////////////////////////////////////////////////////////////////
// Unitialize for shutdown
///////////////////////////////////////////////////////////////////////////////////

void CCustomerInfoBase::UnInitialize()
{
    try
    {
		m_bInitialized = false;
		if (m_pWebRequest != NULL)
		{
		  m_pWebRequest->abort();
		  if (m_pXMLHTTPEvent != NULL)
		  {
			 delete m_pXMLHTTPEvent;
			 m_pXMLHTTPEvent = NULL;
		  }
		  m_pWebRequest.Release();
		  m_pWebRequest = NULL;
		}
	}
    catch(...)
	{
		// Can't trace or ScotError, they may not exist
		// because UnInitialize may be during shutdown
	}
}




///////////////////////////////////////////////////////////////////////////////////
// Function:    Initialize                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
long CCustomerInfoBase::Initialize()
{
    trace( L6, _T("+Initialize"));
    long lReturn = 0;

    try
    {
        LONG     lRC;
        HRESULT  hr;
        HKEY     hKey;
        TCHAR  szServer [_MAX_PATH+1];
        CString csRegKey;
        DWORD dwServerSize=sizeof(long);
        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, PERSONALIZATIONKEY,
            0, KEY_QUERY_VALUE, &hKey);
        if (ERROR_SUCCESS == lRC)
        {
            
            dwServerSize=_MAX_PATH;
            csRegKey = PERSONALIZATIONSERVERNAME;
            lRC = ::RegQueryValueEx(hKey, csRegKey, NULL, NULL,
                (LPBYTE)szServer, &dwServerSize);
            ::RegCloseKey(hKey);
            if (lRC != ERROR_SUCCESS)
            {
                ScotError(RETURN, SM_CAT_CONFIGURATION, SCOTAPP_REG_ACCESS_ERROR, _T("%d|%s|%s"), 
                    lRC, _T("opening"), PERSONALIZATIONKEY);
            }
            else
            {
                m_csServerName=szServer;
            }
        }
        else
        {
            trace(L6, _T("Error opening Personalization registry key"));
            lReturn = -1;
        }
        if (m_csServerName.IsEmpty() || m_csServerName == _T("") || lReturn == -1)
        {
            trace(L6, _T("No Personalization Server configured"));
            m_bInitialized = false;
            lReturn = -1;
            return lReturn;
        }


        hr=m_pWebRequest.CreateInstance("Msxml2.XMLHTTP");
        if (FAILED(hr))
        {
           ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_CREATE_INSTANCE_FAILURE,
           (_bstr_t)_T("Initialize - HRESULT=%x, Unable to create XMLHTTP object"), hr);
           lReturn = -1;
        }
        
        if (!FAILED(hr))
        {           
            CString csXMLFileName = co.csConfigFilePath + _T("\\") + XML_CUSTINFOCFGFILE + _T(".xml");
            
            if (!GetConfigInfo(csXMLFileName))
            {
                trace(L1, _T("Unable to load %s configuration file"), csXMLFileName);
                ScotError(RETURN, SM_CAT_CONFIGURATION, CUSTINFO_CONFIG_LOAD_FAILURE,
                    (_bstr_t)_T("Initialize - HRESULT=%x, Unable load %s"), hr, _bstr_t(csXMLFileName));            
                lReturn = -1;
            }
            else
            {
                csXMLFileName = co.csConfigFilePath + _T("\\") + XML_CUSTINFOCFGFILE + _T(".000");
                
                if (!GetConfigInfo(csXMLFileName))
                {
                    trace(L6, _T("Unable to load %s configuration file"), csXMLFileName);
                }
                
                m_pXMLHTTPEvent = new CXMLHTTPEvent(m_pWebRequest, this);
                if (m_pXMLHTTPEvent == NULL)
                {
                    lReturn = -1;
                }
            }
        }

        
        if (lReturn != 0)
        {
            if (m_pWebRequest != NULL)
            {
                m_pWebRequest->abort();
                m_pWebRequest.Release();
                m_pWebRequest = NULL;
            }
            if (m_pXMLHTTPEvent != NULL)
            {
                delete m_pXMLHTTPEvent;
                m_pXMLHTTPEvent = NULL;

            }
        }
        else
        {
            m_bInitialized = true;
        }
    }
    catch (_com_error ce) 
    {
        HRESULT hr = ce.Error();
        CString csMesg = ce.ErrorMessage();
        CString csErrorCode;
        csErrorCode.Format(_T("%x"),hr);
        BSTR bstrDesc = NULL;
        ce.ErrorInfo()->GetDescription(&bstrDesc);
        CString csErrorDesc = bstrDesc;
        
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_COM_ERROR,
            (_bstr_t)_T("Initialize - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
            hr,
            csMesg,
            csErrorDesc);
        
        if(bstrDesc)
        {
            ::SysFreeString(bstrDesc);
            bstrDesc = NULL;
        }
        trace( L6, _T("-Initialize returning %d"), -1);
        return -1;
    }
    catch(...)
    {
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_GENERAL_EXCEPTION,
            (_bstr_t)_T("Initialize"));
        trace( L6, _T("-Initialize Caught Unknown Exception, returning %d"), -1);
        return -1;
    }
    
    trace( L6, _T("+Initialize returning %d"), lReturn);
    return lReturn;
}


///////////////////////////////////////////////////////////////////////////////////
// Function:    GetConfigInfo                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
bool CCustomerInfoBase::GetConfigInfo(CString csFileName)
{
    trace(L6, _T("+GetConfigInfo(%s)"), csFileName);
    MSXML2::IXMLDOMDocumentPtr pCustomerDOM = NULL;
    bool bReturn = false;
    try
    {
        HRESULT hr = pCustomerDOM.CreateInstance(__uuidof(DOMDocument40));
        //Make sure that the DOM document object was successfully created
        if (FAILED(hr))
        {
           ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_CREATE_INSTANCE_FAILURE,
           (_bstr_t)_T("Initialize - HRESULT=%x, Unable to create DOMDocument object"), hr);
           bReturn = false;
        }
        else
        {
        pCustomerDOM->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
        //Load PickList XML config file
        VARIANT_BOOL bResult = pCustomerDOM->load( (LPCTSTR)csFileName );
        if( bResult != VARIANT_TRUE )
        {
            if( csFileName.Find(_T(".xml")) != -1)  // only complain for base files, not overrides
            {
            trace(L1, _T("ERROR: Load CustomerInfo config file %s failed"), csFileName);
            ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_LOAD_XML_FAILURE,
                (_bstr_t)_T("GetConfigInfo - Unable to load config file %s, bResult=%x"), csFileName, bResult);
            }
        } 
        else
        {
            IXMLDOMNodeListPtr ConfigValues;
            IXMLDOMNodePtr ConfigNode;
            
            ConfigValues = pCustomerDOM->selectNodes(_T("//CustomerInfoService/*"));
            for (int i=0; i<ConfigValues->length; i++) 
            {
                ConfigNode = ConfigValues->nextNode();
                _bstr_t bstrName = ConfigNode->GetnodeName();
                if (bstrName != _bstr_t(_T("#comment")))
                {
                    _bstr_t bstrValue = ConfigNode->Gettext();
                    ConfigOpts.SetAt(bstrName, bstrValue);
                }
            }
            bReturn = true;
        }
        }
    }
   catch (_com_error ce) 
   {
       HRESULT hr = ce.Error();
       CString csMesg = ce.ErrorMessage();
       CString csErrorCode;
       csErrorCode.Format(_T("%x"),hr);
       BSTR bstrDesc = NULL;
       ce.ErrorInfo()->GetDescription(&bstrDesc);
       CString csErrorDesc = bstrDesc;

       ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_COM_ERROR,
           (_bstr_t)_T("GetConfigInfo - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
           hr,
           csMesg,
           csErrorDesc);

       if(bstrDesc)
       {
           ::SysFreeString(bstrDesc);
           bstrDesc = NULL;
       }
       bReturn = false;
   }
   catch (...) 
   {  
       ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_GENERAL_EXCEPTION,
           (_bstr_t)_T("GetConfigInfo"));
       bReturn = false;
   }

   trace(L1, _T("-GetConfigInfo returning %d"), bReturn);
   if (pCustomerDOM != NULL)
   {
       pCustomerDOM.Release();
       pCustomerDOM = NULL;
   }
   return bReturn;
}


///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       LookupParmsBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
BEGIN_KV_HANDLER_CLASS(LookupParmsBase)
    DEFINE_KV_HANDLER_MAP_ENTRY(LookupParmsBase, KEY_OPERATION, Operation)
    DEFINE_KV_HANDLER_MAP_ENTRY(LookupParmsBase, KEY_LOYALTY_ID, LoyaltyID)
END_KV_HANDLER_CLASS()
DEFINE_KV_CSTRING(LookupParmsBase, Operation)
DEFINE_KV_CSTRING(LookupParmsBase, LoyaltyID)


///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       LookupParmsBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void LookupParmsBase::Clear()
{
    m_Operation.Empty();
    m_LoyaltyID.Empty();
}


///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::Lookup(_bstr_t bstrLookupParms)
{
    LookupParmsBase lookupParms;
    trace( L6, _T("+Lookup"));
    if (m_bInitialized)
    {
        lookupParms.ParseKeys(bstrLookupParms);
        
        CString csAddress = _T("");
        CString csAddressFormat = _T("");
        CString csRequestFormat = _T("");
        CString csRequest=_T("");
        CString csRetailerID=_T("");
        CString csAction=_T("");
        if (lookupParms.GetOperation() == _T("LoyaltyLookup"))
        {
            ConfigOpts.Lookup(_T("LoyaltyLookupAction"), csAction);
            ConfigOpts.Lookup(_T("LoyaltyLookupAddress"), csAddressFormat);
            ConfigOpts.Lookup(_T("LoyaltyLookupRequest"), csRequestFormat);
            ConfigOpts.Lookup(_T("RetailerID"), csRetailerID);
            
            if (!csAddressFormat.IsEmpty() && !csRequestFormat.IsEmpty())
            {
                csAddress.Format(csAddressFormat, m_csServerName);
                csRequest.Format(csRequestFormat, lookupParms.GetLoyaltyID(), csRetailerID);
                SendRequest(_bstr_t(csAddress), _bstr_t(csAction), bstr_t(csRequest));
            }
        }
    }
    trace( L6, _T("-Lookup"));
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    SendRequest                                  
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  bstrWebAddress - Web address for HTTP request
//              bstrAction - SOAPAction string for header
//              bstrRequest - text of the web request to be posted.
//                     
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////

long CCustomerInfoBase::SendRequest(_bstr_t bstrWebAddress,
                                    _bstr_t bstrAction, 
                                    _bstr_t bstrRequest )
{
   BSTR bstrResponse = NULL;
   HRESULT hr;
   trace( L6, _T("+SendRequest"));
   try 
   {
       CSingleLock lockRequest(&m_CriticalRequest);
       lockRequest.Lock();

       AbortRequest(); //Clean up any previous request
       
       m_pXMLHTTPEvent = new CXMLHTTPEvent(m_pWebRequest, this);
       if (m_pXMLHTTPEvent == NULL)
       {
           return -1;
       }

       hr=m_pWebRequest->open(_T("POST"), bstrWebAddress, true);
       hr=m_pWebRequest->setRequestHeader(_T("SOAPAction"),bstrAction);  
       SUCCEEDED(hr) ? 0 : throw hr;
       
       hr=m_pWebRequest->setRequestHeader(_T("Content-Type"), _T("text/xml"));
       hr=m_pWebRequest->send(bstrRequest);
       SUCCEEDED(hr) ? 0 : throw hr;
          
       
   }
   catch (_com_error ce) 
   {
       HRESULT hr = ce.Error();
       CString csMesg = ce.ErrorMessage();
       CString csErrorCode;
       csErrorCode.Format(_T("%x"),hr);
       BSTR bstrDesc = NULL;
       ce.ErrorInfo()->GetDescription(&bstrDesc);
       CString csErrorDesc = bstrDesc;

       m_bOnline = false;
       ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_COM_ERROR,
           (_bstr_t)_T("SendRequest - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
           hr,
           csMesg,
           csErrorDesc);

       if(bstrResponse)
       {
           ::SysFreeString(bstrResponse);
           bstrResponse = NULL;
       }
       if(bstrDesc)
       {
           ::SysFreeString(bstrDesc);
           bstrDesc = NULL;
       }
   }
   catch (...) 
   {
       m_bOnline = false;
       if(bstrResponse)
       {
           ::SysFreeString(bstrResponse);
           bstrResponse = NULL;
       }
   }
   trace( L6, _T("-SendRequest"));
   return 0;
   //   return bstrResponse;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    AbortRequest                                     
// Class:       CCustomerInfoBase                                              
// Description: Aborts request
// Parameters:  None
// Returns:     N/A                                      
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::AbortRequest()
{
    if (m_bInitialized && 
        (m_pWebRequest != NULL))
    {
        m_pWebRequest->abort();
    }
    if (m_pXMLHTTPEvent != NULL)
    {
        delete m_pXMLHTTPEvent;
        m_pXMLHTTPEvent = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ClearCustomerInfo                                      
// Class:       CCustomerInfoBase                                              
// Description: Clears all current customer info and restores the default settings
// Parameters:  None
// Returns:     N/A                                      
///////////////////////////////////////////////////////////////////////////////////
CCustomerInfoBase::ClearCustomerInfo()
{
    
    CString csDefault;
    //Delete settings from previous lookup
    CSingleLock lockPreferences(&m_CriticalPreferences);
    lockPreferences.Lock();

    CustomerPref.RemoveAll();
    UpdatedPref.RemoveAll();
    lockPreferences.Unlock();
    
    //restore the store defaults
    if (m_bInitialized)
    {
		ConfigOpts.Lookup(_T("DefaultCustomerInfo"), csDefault);
		GetCustomerSettings( _bstr_t(csDefault));
    }

    
}
//CustomerInfoBase End 

///////////////////////////////////////////////////////////////////////////////////
// Function:    GetPreference                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  csSetting - string containing the name of the preference to 
//                          retrieve.
//              bUpdatePref - bool indicating whether to retrieve the current
//                            value of the preference or the original setting.    
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
CString CCustomerInfoBase::GetPreference(CString csSetting, bool bOrigValue)
{
    CString csPreference = _T("");
    
    if (PersonalizationActive())
    {
        CSingleLock lockPreferences(&m_CriticalPreferences);
        lockPreferences.Lock();
        if (m_bInitialized)
        {
            if (!bOrigValue)
            {
                UpdatedPref.Lookup(csSetting, csPreference);
            }
            if (csPreference.IsEmpty())
            {
                CustomerPref.Lookup(csSetting, csPreference);
            }
        }
        
        lockPreferences.Unlock();
    }
    return csPreference;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::SetPreference(CString csSetting, CString csValue)
{
    if (PersonalizationActive())
    {
        UpdatedPref.SetAt(csSetting, csValue);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
CString CCustomerInfoBase::GetOption(CString csOption)
{
    CString csValue = _T("");
    if (PersonalizationActive())
    {
        ConfigOpts.Lookup(csOption, csValue);
    }
    return csValue;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    GetCustomerSettings                                 
// Class:       CustomerInfoBase                                         
// Description: Extracts preference settings from an XML string and saves them
//              in a CMap object.
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::GetCustomerSettings(_bstr_t bstrCustomerXML)
{
    MSXML2::IXMLDOMNodePtr spNode = NULL;
    MSXML2::IXMLDOMDocumentPtr pCustomerDOM;
    try
    {
        HRESULT hr = pCustomerDOM.CreateInstance(__uuidof(DOMDocument40));
        //Make sure that the DOM document object was successfully created
        if (FAILED(hr))
        {
            ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_CREATE_INSTANCE_FAILURE,
                (_bstr_t)_T("GetCustomerSettings - HRESULT=%x, Unable to create DOMDocument object"), hr);
            return;
        }
        trace(L7, _T("GetCustomerSettings processing xml string: %s"), (LPCSTR) bstrCustomerXML);
        VARIANT_BOOL bResult = pCustomerDOM->loadXML(bstrCustomerXML);
        
        if( bResult != VARIANT_TRUE )
        {
            trace(L1, _T("ERROR: Load Customer Data failed"));
            ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_LOAD_XML_FAILURE,
                (_bstr_t)_T("GetCustomerSettings - Unable to load xml, bResult=%x"),  bResult);
        } 
        else
        {
            IXMLDOMNodeListPtr pIDOMNodeList = NULL;
            IXMLDOMNodePtr pCurrSetting = NULL;
            IXMLDOMNodePtr pSetting = NULL;
            IXMLDOMNodePtr pSettingValue = NULL;
            IXMLDOMNodePtr pRoot = NULL;
            
            CSingleLock lockPreferences(&m_CriticalPreferences);
            lockPreferences.Lock();

            if (pCustomerDOM != NULL)
            {
                pIDOMNodeList = pCustomerDOM->getElementsByTagName(_T("customer")); 
                
                if (pIDOMNodeList != NULL)
                {              
                    //  pIDOMNodeList->reset();
                    
                    CustomerPref.RemoveAll();
                    
                    long NodeListLen = pIDOMNodeList->Getlength();

                    if (pIDOMNodeList->Getlength() > 0)
                    {
                        pRoot = pIDOMNodeList->nextNode();

                    if (pRoot != NULL)
                    {
                        pCurrSetting = pRoot->GetfirstChild();
                        while (pCurrSetting != NULL)
                        {
                            pSetting = pCurrSetting->selectSingleNode(_T("setting"));
                            if (pSetting)
                            {
                                pSettingValue = pCurrSetting->selectSingleNode(_T("value"));
                                _bstr_t bstrName = pSetting->Gettext();
                                _bstr_t bstrValue = pSettingValue->Gettext();
                                
                                if (bstrName.length != 0)
                                {
                                    CustomerPref.SetAt(bstrName, bstrValue);
                                }
                            }
                            pCurrSetting = pCurrSetting->nextSibling;
                            
                        }
                    }
                    }
                }
            }
        }
    }
    catch (_com_error ce) 
    {
        HRESULT hr = ce.Error();
        CString csMesg = ce.ErrorMessage();
        CString csErrorCode;
        csErrorCode.Format(_T("%x"),hr);
        BSTR bstrDesc = NULL;
        ce.ErrorInfo()->GetDescription(&bstrDesc);
        CString csErrorDesc = bstrDesc;
        
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_COM_ERROR,
            (_bstr_t)_T("GetCustomerSettings - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
            hr,
            csMesg,
            csErrorDesc);
        
        if(bstrDesc)
        {
            ::SysFreeString(bstrDesc);
            bstrDesc = NULL;
        }
    }
    catch(...)
    {
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_GENERAL_EXCEPTION,
            (_bstr_t)_T("GetCustomerSettings"));
        trace( L6, _T("-GetCustomerSettings Caught Unknown Exception"), -1);
    }
    return;

}

///////////////////////////////////////////////////////////////////////////////////
// Function:    PersonalizationActive                                 
// Class:       CustomerInfoBase                                         
// Description: Is personalization active?
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     bool                                        
///////////////////////////////////////////////////////////////////////////////////
bool CCustomerInfoBase::PersonalizationActive()
{
    return m_bInitialized;
}
///////////////////////////////////////////////////////////////////////////////////
// Function:    UpdatePreferences                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    Merge updated selection with original preferences and send to
//              Web service.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::UpdatePreferences()
{

    CString  csName;
    CString  csValue;
    
    if( !PersonalizationActive())
    {
        return;
    }

    CSingleLock lockPreferences(&m_CriticalPreferences);
    lockPreferences.Lock();

    if (GetPreference(_T("LookupSuccessful")) == _T("true"))
    {
        UpdatedPref.RemoveKey(_T("LookupSuccessful"));
    }
    else
    {
        return;
    }

    POSITION pos = UpdatedPref.GetStartPosition();
    while (pos != NULL)
    {
        UpdatedPref.GetNextAssoc( pos, csName, csValue );
        CustomerPref.SetAt(csName, csValue);
    }

    CString csSettingFormat;
    //CString csSettingFormat = _T("<userSetting><setting>%s</setting><value>%s</value></userSetting>");  
    ConfigOpts.Lookup(_T("SettingFormat"), csSettingFormat);
    CString csSetting;
    CString csPrefs = _T("");
    pos = CustomerPref.GetStartPosition();
    while (pos != NULL)
    {
        CustomerPref.GetNextAssoc( pos, csName, csValue);
        csSetting.Format(csSettingFormat, csName, csValue);
        csPrefs = csPrefs + csSetting;
    }

    CString csAddress, csAddressFormat, csAction, csRequest, csRequestFormat;
    
    ConfigOpts.Lookup(_T("UpdatePreferencesAction"), csAction);
    ConfigOpts.Lookup(_T("UpdatePreferencesAddress"), csAddressFormat);
    ConfigOpts.Lookup(_T("UpdatePreferencesRequest"), csRequestFormat);

    if (!csAddressFormat.IsEmpty() && !csRequestFormat.IsEmpty())
    {
        csAddress.Format(csAddressFormat, m_csServerName);
        csRequest.Format(csRequestFormat, csPrefs);
        SendRequest(_bstr_t(csAddress), _bstr_t(csAction), bstr_t(csRequest));
    }

}


///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::OnReadyStateChange(long lReadyState)
{
	if	( !m_bInitialized )
	{
		// We can't even trace if not initialized  
		// error and trace may have been destructed
		return;
	}

    trace( L6, _T("+OnReadyStateChange(%d)"), lReadyState);
    if (lReadyState == REQUEST_COMPLETE)
    {
        try
        {
            CSingleLock lockRequest(&m_CriticalRequest);
            lockRequest.Lock();
            m_bstrCustomerInfo = m_pWebRequest->responseText;
            lockRequest.Unlock();

            if (m_bstrCustomerInfo.length() == 0)
            {
                trace(L4, _T("No data returned for lookup request"));
                return;
            }
            
            CString csResult = (TCHAR*) m_bstrCustomerInfo;
            if (csResult.Find(_T("getUserSettingsResponse")) != -1)
            {
                GetCustomerSettings((_bstr_t) csResult);
                
                trace(L6, _T("Posting LOOKUP_COMPLETE message"));
                SetPreference(_T("LookupSuccessful"), _T("true"));
                mo.PostCU(LOOKUP_COMPLETE, _T("")); //Post message, no params as yet 
            }

            if (!m_bOnline)
            {
                m_bOnline = true;
                ScotError(INFO, SM_CAT_OPERATIONAL, CUSTINFO_ONLINE,
                    (_bstr_t)_T("OnReadyStateChange"));
            }
                
            trace(L6, _T("HTTPResult = %s"), csResult);     

            
        }
        catch (_com_error ce) 
        {
            HRESULT hr = ce.Error();
            CString csMesg = ce.ErrorMessage();
            CString csErrorCode;
            csErrorCode.Format(_T("%x"),hr);
            BSTR bstrDesc = NULL;
            ce.ErrorInfo()->GetDescription(&bstrDesc);
            CString csErrorDesc = bstrDesc;
            m_bOnline = false;
            ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_COM_ERROR,
                (_bstr_t)_T("OnReadyStateChange - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
                hr,
                csMesg,
                csErrorDesc);
            
            if(bstrDesc)
            {
                ::SysFreeString(bstrDesc);
                bstrDesc = NULL;
            }
        }
        catch(...)
        {
            m_bOnline=false;
            ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_GENERAL_EXCEPTION,
                (_bstr_t)_T("OnReadyStateChange"));
            trace(L6, _T("OnReadyStateChange caught unknown exception"));
        }
      
    }
    trace( L6, _T("-OnReadyStateChange"));
}
