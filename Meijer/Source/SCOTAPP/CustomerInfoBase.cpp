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

#include "Common.h"
#include "CustomerInfoBase.h"          // Track events 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CCustomerInfoBase")

#define PERSONALIZATIONKEY        _T("SOFTWARE\\NCR\\SCOT\\Installation\\CorePersonalizationServer")                            
#define PERSONALIZATIONSERVERNAME    _T("PersonalizationServer")
#define PERSONALIZATIONCONFIG  _T("\\NCRPersonalization.xml")
#define PERSONALIZATIONCMSCONFIG _T("\\CmsPersonalizationConfig.xml")

bool CCustomerInfoBase::m_bInitialized = false;

PERSN::IPersonalizationPtr CCustomerInfoBase::m_pPersonalizationObj = NULL;
PERSN::_IPersonalizationEventsPtr CCustomerInfoBase::m_pPersonalizationEventObj = NULL;
PersonalizationEventHandler *CCustomerInfoBase::m_pPersonalizationEventHandler = NULL;
time_t CCustomerInfoBase::m_tLookupStartTime;
time_t CCustomerInfoBase::m_tLookupCompleteTime;
IMPLEMENT_DYNCREATE(CCustomerInfoBase, CObject)// MFC Runtime class/object information
// Constructor
//////////////////////////////////////////////////////////////////////////////////
CCustomerInfoBase::CCustomerInfoBase():
m_bstrCustomerInfo(_T(""))
{
    m_csLookupID=_T("");
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
// UnInitialize for shutdown
///////////////////////////////////////////////////////////////////////////////////

void CCustomerInfoBase::UnInitialize()
{
    try
    {      
        m_bInitialized = false;
        trace( L6, _T("+ %s()"), _T("CCustomerInfoBase::UnInitialize"));
        if (m_pPersonalizationEventHandler)
        {
            m_pPersonalizationEventHandler -> ShutdownConnectionPoint();
            m_pPersonalizationEventHandler -> Release();
            m_pPersonalizationEventHandler = NULL;
        }
        if (m_pPersonalizationObj)
		{
			m_pPersonalizationObj->Uninitialize();
			m_pPersonalizationObj->Release();
			m_pPersonalizationObj = NULL;
		}
    }
    catch(...)
    {
        trace( L6, _T("+ %s()"), _T("CCustomerInfoBase::UnInitialize exception"));
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
    trace( L6, _T("+ %s()"), _T("CCustomerInfoBase::Initialize"));
    long lReturn = 0;

    // +TAR 446826:  Handle successive Initialize calls.  Don't initialize
    //               again if already initialized.
    if(m_bInitialized)
    {
        trace(L6, 
          _T("CCustomerInfoBase::Initialize: WARNING - ALREADY INITIALIZED."));
        return 0;    // Return success since not really a problem here.
    }
    // -TAR 446826

    m_bInitialized = false;

    try
    {
        HRESULT hr= m_pPersonalizationObj.CreateInstance(__uuidof(PERSN::Personalization));
        _ASSERT(SUCCEEDED(hr));
        if (hr == S_OK)
        {
            CString strConfig = co.csConfigFilePath + PERSONALIZATIONCONFIG;
            CString strCMSConfig = co.csConfigFilePath + PERSONALIZATIONCMSCONFIG;
            m_pPersonalizationEventHandler = new PersonalizationEventHandler(*this, m_pPersonalizationObj, &CCustomerInfo::OnEventPersonalizationObj);
            m_pPersonalizationObj->Initialize((_bstr_t) strConfig,(_bstr_t) strCMSConfig, (_bstr_t) rp.GetTerminalNumber());
            
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
        if (ce.ErrorInfo())
        {
            ce.ErrorInfo()->GetDescription(&bstrDesc);
            CString csErrorDesc = bstrDesc;
        }
        
        if(bstrDesc)
        {
            ::SysFreeString(bstrDesc);
            bstrDesc = NULL;
        }
        trace( L6, _T("+ %s() Caught COM exception %x: description: %s"), _T("CCustomerInfoBase::Initialize"), hr, csMesg );
        lReturn = -1;
    }
    catch (CException e)
    {
        //CString test =e.GetErrorMessage;
    }
    catch(...)
    {
        ScotError(RETURN, SM_CAT_INTERNAL_ERROR, CUSTINFO_GENERAL_EXCEPTION,
            (_bstr_t)_T("Initialize"));
        trace( L6, _T("%s() Caught Unknown Exception, returning %d"), _T("CCustomerInfoBase::Initialize"), -1);
        lReturn = -1;
    }


    try
    {
        if (!m_bInitialized)
        {
            if (m_pPersonalizationObj)
            {
                m_pPersonalizationObj->Uninitialize();
                m_pPersonalizationObj->Release();
            }
            
            delete m_pPersonalizationEventHandler;
            trace( L6, _T("Cleaned up Personalization Objects because of initialize exception"));
        }
    }
    catch(...)
    {
        trace( L6, _T("Caught exception cleaning up Personalization object"), lReturn);
    }

    //ReloadOptions();
    trace( L6, _T("+ %s() returning %d"), _T("CCustomerInfoBase::Initialize"), lReturn);

    return lReturn;
   
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
        CString csLookupID = lookupParms.GetLoyaltyID();
        if (m_csLookupID != lookupParms.GetLoyaltyID())
        {
            m_tLookupStartTime = GetTickCount();
            m_csLookupID = lookupParms.GetLoyaltyID();
            m_pPersonalizationObj->Lookup((bstr_t) lookupParms.GetOperation(), (_bstr_t) lookupParms.GetLoyaltyID());
        }
        else
        {
            trace( L6, _T("Lookup already started, ignoring"));
        }
    }
    trace( L6, _T("-Lookup"));
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    VerifyAndGetLookupTimer                                  
// Class:       CustomerInfo                                         
// Description: Performs a pattern match on the lookupID based on the
//              the lookup type.  If a match occurs, a lookup to the
//              personalization server is performed.
// Parameters:  csLookupType - the type of lookup.  Base types are:
//                Scan - csLookupID came from a scan
//                Swipe - csLookupID came from a swipe
//                Loyalty - csLookupID came from the TB
//             
// Comments:    N/A
//              
//              
// Returns:     A timer value in milleseconds. SMAttract uses the value to
//              to wait for a lookup response.  Other states usually ignore the
//              value.                                       
///////////////////////////////////////////////////////////////////////////////////

long CCustomerInfoBase::VerifyAndGetLookupTimer(CString csLookupType, CString csLookupID)
{
    CString csLoyaltyPattern;
    COleSafeArray csaPatternArray;
    int nPatterns = 0;
    bool bMatched = false;
    bool bVariableLength = false;
    long lTimerValue = 0;
    _bstr_t bstrTimerValue = _T("");
    _variant_t vPatterns;        
    long index;
    CString csLoyaltyType;
    BSTR bstrPattern;
    
    trace( L6, _T("+VerifyAndGetLookupTimer - type='%s', m_bInitialized=%d"),
           csLookupType, m_bInitialized);

    if (m_bInitialized)
    {
        csLoyaltyType = _T("LoyaltyPattern");
        csLoyaltyType += csLookupType;
        vPatterns = m_pPersonalizationObj->GetOptionList((_bstr_t) csLoyaltyType);
        
        
        if (vPatterns.vt != VT_EMPTY)
        {
            csaPatternArray.Attach(vPatterns);
            nPatterns = csaPatternArray.GetOneDimSize();
            trace( L7, _T("VerifyAndGetLookupTimer found %d patterns"), nPatterns);
        }
        
        for (index=0; (index < nPatterns) && !bMatched; index++)
        {	
            csaPatternArray.GetElement(&index, &bstrPattern);
            csLoyaltyPattern = bstrPattern;
            int nPatternLen = csLoyaltyPattern.GetLength();
            
            if ( csLoyaltyPattern.GetAt(nPatternLen - 1) == _T('+') )
                bVariableLength = true;
            
            if ( (nPatternLen == 1) && csLoyaltyPattern.GetAt(0) == _T('*'))
            {
                bMatched = true;
                break;
            }
            
            if ((csLookupID.GetLength() == csLoyaltyPattern.GetLength())||
                (bVariableLength && (csLookupID.GetLength() >= csLoyaltyPattern.GetLength())) )
                
            {	// first check if lengths match
                int pos = 0;
                while(pos < csLoyaltyPattern.GetLength())
                {	// compare each character
                    _TCHAR strChar = csLoyaltyPattern.GetAt(pos);
                    // ignore x and c
                    if((strChar != _T('x')) && (strChar != _T('c')) && (strChar != _T('+')))
                    {
                        if(strChar != csLookupID.GetAt(pos))
                        {	// doesn't match so go to next coupon format
                            break;
                        }
                    }
                    pos++;
                } 
                if(pos == csLoyaltyPattern.GetLength())
                    bMatched = true;
            }
			::SysFreeString(bstrPattern); // cs185088 mem leak 2012.09.11
        }
        if (bMatched)
        {
            
            bstrTimerValue = m_pPersonalizationObj->GetOption(_T("LoyaltyLookupTimer"));
            if(bstrTimerValue != (_bstr_t) _T(""))
            {
                lTimerValue = _ttol(bstrTimerValue);
                
                CKeyValueParmList parmList;
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_LOYALTY_LOOKUP);
                CKeyValue<const _TCHAR*>::Put(parmList, KEY_LOYALTY_ID, csLookupID);
                Lookup((const _TCHAR *)parmList);
            }
            
        }
        csaPatternArray.Detach();
        
    }
    trace( L6, _T("-VerifyAndGetLookupTimer returning %d"), lTimerValue);
    return lTimerValue;

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
    //restore the store defaults
    m_tLookupStartTime = 0;
    m_tLookupCompleteTime = 0;

    if (m_bInitialized)
    {
        m_csLookupID.Empty();
        m_pPersonalizationObj->ClearCustomerInfo();
    } 
}


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
        _bstr_t bstrPref = m_pPersonalizationObj->GetPreference((_bstr_t) csSetting, (VARIANT_BOOL) bOrigValue);
        csPreference = (_TCHAR*) bstrPref;
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
        m_pPersonalizationObj->SetPreference((_bstr_t) csSetting, (_bstr_t) csValue);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    GetDataString                                 
// Class:       CustomerInfo                                         
// Description: Gets the value of a data string.  
// Parameters:  csSetting - The name of the data string to retrieve       
//             
// Comments:    Data strings contain information that is sent to the server but
//              does not directly update the server database as a preference.
//              The data can be processed and used to indirectly update 
//              a preference, or can be used for information gathering.
//              The personalization server is responsible for processing the data.
//              
//              
// Returns:     CString containing the DataString value. Returns the null string
//              if the DataString does not exist.                                    
///////////////////////////////////////////////////////////////////////////////////
CString CCustomerInfoBase::GetDataString(CString csSetting)
{
    CString csData = _T("");
    
    if (PersonalizationActive())
    {
        _bstr_t bstrData =  m_pPersonalizationObj->GetDataString((_bstr_t) csSetting);
        csData = (_TCHAR*) bstrData;
    }
    return csData;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    SetDataString                                 
// Class:       CustomerInfo                                         
// Description: Create or update a data string value.  
// Parameters:  csSetting - the name of the data string
//              csValue - the value of the data string       
//             
// Comments:    Data strings contain information that is sent to the server but
//              does not directly update the server database as a preference.
//              The data can be processed and used to indirectly update 
//              a preference, or can be used for information gathering.
//              The personalization server is responsible for processing the data.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::SetDataString(CString csSetting, CString csValue)
{
    if (PersonalizationActive())
    {
        m_pPersonalizationObj->SetDataString((_bstr_t) csSetting, (_bstr_t) csValue);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    GetMetricLong                                 
// Class:       CustomerInfo                                         
// Description: Get the numeric value of a metric
// Parameters:  csSetting - the name of the value to retrieve       
//             
// Comments:    A metric is a numeric value sent to the personalization server,
//              most likely used for tallies or timing values.  The 
//              The personalization server is responsible for processing the data.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
long CCustomerInfoBase::GetMetricLong(CString csSetting)
{
    long retValue = 0;
    
    if (PersonalizationActive())
    {
        retValue =  m_pPersonalizationObj->GetMetricLong((_bstr_t) csSetting);
    }
    return retValue;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    SetMetricLong                                 
// Class:       CustomerInfo                                         
// Description: Set the numeric value of a metric
// Parameters:  None       
//             
// Comments:    A metric is a numeric value sent to the personalization server,
//              most likely used for tallies or timing values.  The 
//              The personalization server is responsible for processing the data.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::SetMetricLong(CString csSetting, long lValue)
{
    if (PersonalizationActive())
    {
        m_pPersonalizationObj->SetMetricLong((_bstr_t) csSetting, lValue);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    IncrementMetricLong                                 
// Class:       CustomerInfo                                         
// Description: Add to the current value of a metric. A Negative number can be
//              be added.
// Parameters:  lValue the amount to add to the current value of a metric       
//             
// Comments:    A metric is a numeric value sent to the personalization server,
//              most likely used for tallies or timing values.  The 
//              The personalization server is responsible for processing the data.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::IncrementMetricLong(CString csSetting, long lValue)
{
    if (PersonalizationActive())
    {
        long lNewValue = m_pPersonalizationObj->GetMetricLong((_bstr_t) csSetting);
        m_pPersonalizationObj->SetMetricLong((_bstr_t) csSetting, lValue + lNewValue);
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
       csValue = (TCHAR*) m_pPersonalizationObj->GetOption((_bstr_t) csOption);
    }
    return csValue;
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
    if (PersonalizationActive())
    {
        if (m_tLookupCompleteTime != 0)
        {
            m_pPersonalizationObj->SetMetricLong(_T("LookupTime"), m_tLookupCompleteTime);
        }
        m_pPersonalizationObj->SendUpdateToServer();
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    RequestOptions                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    Initiate an option request to the Personalization Server.
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CCustomerInfoBase::RequestOptions()
{
    if (PersonalizationActive())
    {
        m_pPersonalizationObj->RequestOptions();
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    OnEventPersonalizationObj                                 
// Class:       CustomerInfoBase                                         
// Description: 
// Parameters:  None       
//             
// Comments:    Preferences object recieved response to lookup
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////// ***** invoked by the TEventHandler based class object *****
// ***** when an event is fired from the COM object *****
HRESULT CCustomerInfoBase::OnEventPersonalizationObj
(
  PersonalizationEventHandler* pEventHandler,
  DISPID dispidMember, 
  REFIID riid,
  LCID lcid, 
  WORD wFlags, 
  DISPPARAMS* pdispparams, 
  VARIANT* pvarResult,
  EXCEPINFO* pexcepinfo, 
  UINT* puArgErr
)
{
  if (dispidMember == 0x01)  // LookupComplete event.
  {

   bool bSuccess;
   m_tLookupCompleteTime = GetTickCount() - m_tLookupStartTime;
   m_tLookupStartTime = 0;

   bSuccess= (pdispparams -> rgvarg)[0].boolVal ;
   mo.PostCU(LOOKUP_COMPLETE, _T("")); //Post message, no params as yet
  }
  if (dispidMember == 0x02) //Recieved configuration options
  {
      mo.PostCU(LOOKUP_OPTIONS_RECIEVED, _T("")); //Post message, no params as yet
  }

  return S_OK;
}

