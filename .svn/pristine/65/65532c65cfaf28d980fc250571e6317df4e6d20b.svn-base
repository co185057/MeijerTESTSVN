//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateCUBase.cpp
//
// TITLE: Customer Info Parse Functions
//
//
// AUTHOR:Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV abstract base state
#include "VolumeControl.h"          // SR815.5 Personalized Volume
#include "SMRapDataNeeded.h"        // SSCOADK-260
#include "SMProduceFavorites.h"
#include "SMPLAHandInView.h"
#else
#include "FakeSMStateBase.h"
#endif //_CPPUNIT

#include "SMStateCUBase.h"

#ifndef _CPPUNIT
#define COMP_ID ID_CU               // base state component
#define T_ID    _T("CUBase")

CString SMStateBase::csPersonalThemeUsed = _T(""); //SR814.5 SR815.6 SR851 Personalization and Security

///////////////////////////////////////////////////////////////////////////////
// Function:    OnLookupComplete                                 
// Class:       SMStateBase                                            
// Description: 
// Parameters:  long byteCount        
//              
//              
//              
//              
// Returns:      STATE_NULL                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnLookupComplete (long byteCount)
{
    trace(L6, _T("+SMStateBase::OnLookupComplete byteCount = %d"), byteCount);
    if (byteCount >= 0 && bIsTransactionStarted) //Lookup completed
    {
        SCOT_LANGUAGE_TYPE scotLangTemp = m_customerLanguage;
        if ((custInfo.GetOption(_T("PersonalizeLanguage")) == "Y") &&
			(scotLangTemp != GetPreferredLanguage()))  //SSCOP-2495
        {
            m_customerLanguage = GetPreferredLanguage(); //SSCOP-2495
            ps.SetLanguage(m_customerLanguage);
            co.csCMCustomerMessage = ps.GetCustomerMessageDefault();
            CustomerMessage = co.csCMCustomerMessage;
        }

		// +SSCOP-2657
		if ( co.IsShowVolumeControl() )
		{
			try
			{
				CVolumeControl *pVolumeControl = new CVolumeControl();
				int nCurrentVolume = pVolumeControl->GetVolumeLevel();
				int nDefaultVolume = pVolumeControl->GetDefaultVolume();
				CString csVolume = custInfo.GetPreference(_T("VolumeLevel")); // SSCOP-2657
				if ( !IsPersonalizedVolumeChange() ) // SSCOP-2657
				{
					if (!csVolume.IsEmpty()) // SSCOP-2657
					{
						int nPersonalizedVolume = _ttoi( csVolume ); // SSCOP-2657
						if ( nCurrentVolume != nPersonalizedVolume ) // SSCOP-2657
						{
							CVolumeControl VolumeControl(SETVOLUMELEVEL, 0, nPersonalizedVolume);
							trace(L6,_T("SMScanAndBagBase::OnLookupComplete - Personalized volume level is set successfully. Personalized Volume Level: %d"), nPersonalizedVolume);
						}
						else
						{
							trace(L6,_T("SMScanAndBagBase::OnLookupComplete - Personalized Volume level is the same as the current level. Current Volume Level: %d, Personalized Volume Level: %d"), nCurrentVolume, nPersonalizedVolume);
						}
					}
					else
					{
						trace(L6,_T("SMScanAndBagBase::OnLookupComplete - Personalized Volume was not set, using the current volume level. Current Volume Level: %d, Default Volume Level: %d"), nCurrentVolume, nDefaultVolume);
					}
				}
				else
				{
					trace(L6,_T("SMScanAndBagBase::OnLookupComplete - Personalized Volume level not used, customer has already selected a volume level. Current Volume Level: %d, Default Volume Level: %d"), nCurrentVolume, nDefaultVolume);
				}
				delete pVolumeControl;
				pVolumeControl = NULL;
			}
			catch( CException *e )
			{
				_TCHAR szError[255] = {NULL};
				e->GetErrorMessage(szError, 255);
				trace(L6, _T("SMScanAndBagBase::OnLookupComplete - Got an exception! Error text: %s"), szError);
				e->Delete();
			}
		}
		else
		{
			trace(L6,_T("SMScanAndBagBase::OnLookupComplete - Store volume setting overrides personalize volume setting."));
		}
		// -SSCOP-2657

        //+SR814.5 SR815.6 SR851 Personalization and Security
        SetPersonalTheme(_T("SMStateBase::OnLookupComplete"));
        /*
        CString csTheme = custInfo.GetPreference(_T("Theme"));
        if (csTheme.GetLength())
        {
			CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMStateBase::OnLookupComplete"));
            m_pAppPSX->SetTheme(csTheme);
			CString csTestTheme;
            m_pAppPSX->GetTheme(csTestTheme);
			if (csTestTheme != csTheme)
			{
				m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
			}
			
        }
		else
		{
			m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
		}
        */
        //-SR814.5 SR815.6 SR851 Personalization and Security

        bAlreadyInSnB = false;
		
		//+ Personalization age prompt
		if(custInfo.GetOption(_T("PersonalizedAgePrompt")).CompareNoCase(_T("y"))==0)
		{
			COleVariant v = custInfo.GetPreference(_T("Birthdate"));
			CString csDOB = v.bstrVal;
			trace(L6, _T("GetPreference-Birthdate = %s"), csDOB);
			if (csDOB.GetLength() > 0)
			{
				ps.GetPSXObject()->SetTransactionVariable( _T("AgePrompt"), v );
			}
		}
		//- Personalization age prompt
        
    }
  trace(L6,_T("-OnLookupComplete"));
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    OnPersonalizationOptionsRecieved                                 
// Class:       SMStateBase                                            
// Description: 
// Parameters:  long byteCount        
//              
//              
//              
//              
// Returns:      STATE_NULL                                        
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::OnPersonalizationOptionsReceived()
{
    trace(L6, _T("+SMStateBase::OnPersonalizationOptionsReceived"));	
	if (getStateName(this) == _T("SMAttract"))
	{
        m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme"))); 
	}
	trace(L6,_T("-OnPersonalizationOptionsReceived"));
	return STATE_NULL;
}
#endif //_CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    CUParse                                  
// Class:       SMStateBase::CUParse                                             
// Description: 
// Parameters:     
//              
//              
//              
//              
// Returns:     retState                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMStateBase::CUParse(MessageElement *me)
{
  int nEvent = me->cuInfo.Msg;
  CString csStateName;
  SMStateBase *retState = STATE_NULL;

  LPTSTR szParms = NULL;
  if( me->cuInfo.szParms )
  {
      szParms = new TCHAR [_tcslen(me->cuInfo.szParms) + sizeof(TCHAR)];
      _tcscpy( szParms, me->cuInfo.szParms );
      delete [] me->cuInfo.szParms;
  }

  trace(L7,_T("+CUParse"));
  switch(nEvent)
    {
#ifndef _CPPUNIT
        case LOOKUP_COMPLETE:
            retState = OnLookupComplete(0);  
            break;
        case LOOKUP_TIMEOUT:
            retState = OnLookupComplete(-1);
			break;
		case LOOKUP_OPTIONS_RECIEVED:
			retState = OnPersonalizationOptionsReceived();
			break;
#endif //CPPUNIT
        //+SSCOADK-260
        case PLA_VALIDATION:
            retState = OnPLAValidation();
            break;
        //-SSCOADK-260
        case PLA_LOOKUP:
            retState = OnPLALookUp();
            break;
        case PLA_HANDINVIEW:
            retState = OnPLAHandInView();
            break;
        case PLA_TRANSITION:
            retState = OnPLATransition();
            break;
        case PLA_HANDINVIEW_AFTERRETRY:
            retState = OnPLAHandInViewAfterRetry();
            break;    
        default:
            trace(L6,_T("CUParse unhandled message(%d)"), nEvent );
            break;
    }
	
  if (szParms != NULL)
  {
      delete [] szParms;
  }
  trace(L7,_T("-CUParse"));
  return retState;
}

SMStateBase *SMStateBase::OnPLAHandInViewAfterRetry()
{
    trace(L6, _T("+SMStateBase::OnPLAHandInViewAfterRetry"));
    if (m_bWaitForPicklist)
    {
        m_bWaitForPicklist = false;
        RETURNSTATE(ProduceFavorites)
    }
    else
    {
        return OnPLAValidation();
    }
}

SMStateBase *SMStateBase::OnPLATransition()
{
    if (m_bWaitForPicklist)
    {
        m_bWaitForPicklist = false;
    }
    return STATE_NULL;
}

SMStateBase *SMStateBase::OnPLALookUp()
{

	trace(L6, _T("+SMStateBase::OnPLALookUp"));
	if (m_bWaitForPicklist)
	{
		m_bWaitForPicklist = false;
		RETURNSTATE(ProduceFavorites)
	}
	trace(L6, _T("-SMStateBase::OnPLALookUp"));

	return STATE_NULL;
}

SMStateBase *SMStateBase::OnPLAHandInView()
{
    RETURNSTATE(PLAHandInView)
}

//+SSCOADK-260
//this method will generate PLA Validation intervention
SMStateBase *SMStateBase::OnPLAValidation()
{
    trace(L6, _T("+SMStateBase::OnPLAValidation"));
#ifndef _CPPUNIT

    ra.RequestEnableAssistMode(true);   
    CString sInstructionText = ps.GetPSText(LTT_WEIGHTITEM_VALIDATION, SCOT_LANGUAGE_PRIMARY);   
    CSCOTDataEntry cDataEntry;
    cDataEntry.FromConfigData(co.csDataEntryINIFile, _T("SMDataEntryPLAValidation"), true);      
    CString csViewString = cDataEntry.ToString();
    CREATE_AND_DISPENSE(RAPDataNeeded)(sInstructionText,DATANEEDED_MOREDATA, csViewString, storeMode());
    
#else
    return STATE_NULL;
#endif //_CPPUNIT
}
//-SSCOADK-260


#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    GetPreferredLanguage                                 
// Class:       SMStateBase                                          
// Description: Gets the preferred language string from the custInfo object,
//              returns a corresponding SCOT_LANGUAGE_TYPE.
// Parameters:  None       
//             
// Comments:    If the language has been deliberately selected via button press,
//              the current customer language will be returned instead of the
//              value returned from the web service. 
//              
//              
// Returns:     SCOT_LANGUAGE_TYPE                                         
///////////////////////////////////////////////////////////////////////////////////
SCOT_LANGUAGE_TYPE SMStateBase::GetPreferredLanguage(bool bOrigValue)
{
    trace(L6,_T("+GetPreferredLanguage()"));
    SCOT_LANGUAGE_TYPE langPref = m_customerLanguage;

    CString csLangPref = custInfo.GetPreference(_T("Language"), bOrigValue);
    if (csLangPref == _T("default") || csLangPref == _T(""))
    {
        trace(L6,_T("-GetPreferredLanguage returning currently selected language"));
        if (bOrigValue)
            return SCOT_LANGUAGE_PRIMARY; //TAR 446885 reworked 
        else
            return m_customerLanguage;
    }
    if (csLangPref == custInfo.GetOption(_T("language1")))
    {
        langPref = SCOT_LANGUAGE_PRIMARY;
    }
    else if (csLangPref == custInfo.GetOption(_T("language2")))
    {
        langPref = SCOT_LANGUAGE_SECONDARY;
    }
    else if (csLangPref == custInfo.GetOption(_T("language3")))
    {
        langPref = SCOT_LANGUAGE_THIRD;
    }
    else if (csLangPref == custInfo.GetOption(_T("language4")))
    {
        langPref = SCOT_LANGUAGE_FOURTH;
    }
    else if (csLangPref == custInfo.GetOption(_T("language5")))
    {
        langPref = SCOT_LANGUAGE_FIFTH;
    }
    else if (csLangPref == custInfo.GetOption(_T("language6")))
    {
        langPref = SCOT_LANGUAGE_SIXTH;
    }
    else
    {
        trace(L6,_T("-GetPreferredLanguage returning currently selected language for unmatched \"%s\""), langPref);
        langPref = m_customerLanguage;
		custInfo.SetMetricLong(_T("LanguageNotConfigured"),1);
    }

    trace(L6,_T("-GetPreferredLanguage returning %d"), langPref);
    return langPref;
}


///////////////////////////////////////////////////////////////////////////////
// Function:    SetPreferredLanguage                                 
// Class:       SMStateBase                                          
// Description: Sets an updated preferred language type in the custInfo object.
//              The customer will later be asked whether save the preference
//              to the web service.  This method must map a SCOT_LANGUAGE_TYPE
//              to a string recognized by the personalization web service.
// Parameters:  SCOT_LANGUAGE_TYPE langChosen  -  the language chosen.
//              CString csSelectionMethod - how the language was chosen
//                  "Scan"  by scanning at the attract and getting default
//                  "Button" chosen by language button press
//              
// Comments:    CustomerInfoService.xml defines language strings for
//              each defined SCOT_LANGUAGE_TYPE as follows:
//                 SCOT_PRIMARY_LANGUAGE is mapped to language1,
//                 SCOT_LANGUAGE_SECONDARY mapped to language2,  etc.
//              
//              
// Returns:     SCOT_LANGUAGE_TYPE                                         
///////////////////////////////////////////////////////////////////////////////////
void SMStateBase::SetPreferredLanguage(SCOT_LANGUAGE_TYPE langChosen,
                                       CString csSelectionMethod)
{
    trace(L6,_T("+SetPreferredLanguage LangType= %d, SelectionMethod=%s"), 
                 langChosen, csSelectionMethod);

    CString csLangOption, csLang;

    csLangOption.Format(_T("language%d"), langChosen+1); 
    csLang = custInfo.GetOption(csLangOption);

	// +TAR 357892.  Unconditionally set preference options in case 
	//               personalization is misconfigured.

    //Don't immediately save language for scans, wait till SavePreference.
    custInfo.SetPreference(_T("Language"), csLang);
    custInfo.SetDataString(_T("LangSelectionMethod"), csSelectionMethod);

    if (csLang == _T(""))
    {
        trace(L6,_T("+Set Preferred language did not find a map for language%d"), langChosen+1);
    }

	// -TAR 357892

    trace(L6,_T("-SetPreferredLanguage"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PreferencesRecieved                                 
// Class:       SMStateBase                                          
// Description: Actions to be performed when preference information is recieved from
//              the web service.  
// Parameters:  None      
// Comments:    Writes a RAP receipt line.
//                    
// Returns:     Nothing                                     
///////////////////////////////////////////////////////////////////////////////////
void SMStateBase::PreferencesReceived ()
{
    
    
    CString csReceiptSrc = _T("CMSMReceipt");
    CString csItemEntryId = ps.GetSelection(_T("CMReceipt")); // Get the item's entry id
    CString csMsgItemEntryRapId = csItemEntryId + _T("CustomerInfoRecievedRap");
    COleVariant	vValue;
    
    CString csDescPrimary = ps.GetPSText(MSG_PERSONALIZATION_IN_EFFECT, SCOT_LANGUAGE_PRIMARY); 

    m_pAppPSX->CreateReceiptItem(csReceiptSrc, csMsgItemEntryRapId);
    
    trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDescPrimary);

    ra.OnNormalItem(RAP_RECEIPT_LINE_HIGHLIGHT_SUBITEM + csDescPrimary);
    trace(L7, _T("-ra.OnNormalItem"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    NeedPreferenceUpdatePermission                                
// Class:       SMStateBase                                          
// Description: Check to determine whether user should be asked whether tell
//              personalization web service to update user preference information.
// Parameters:  None      
// Comments:    Determines whether to ask user whether to update preferences. 
//              Called in SMFinish state.
//                    
// Returns:     Nothing                                     
///////////////////////////////////////////////////////////////////////////////////

bool SMStateBase::NeedPreferenceUpdatePermission()
{
    bool bReturn = false;
    if ( fLoyaltyCardTrx)
    {
        CString csSave = custInfo.GetPreference(_T("SavePreferences"), true);
        if (!csSave.IsEmpty())
        {
            custInfo.SetDataString(_T("SavePreferences"), csSave);
            bReturn = false;
        }
        else 
        {
            csSave = custInfo.GetOption(_T("SavePreferences"));
            if (!csSave.IsEmpty())
            {
                custInfo.SetDataString(_T("SavePreferences"), csSave);
                bReturn = false;
            }
            else if ( fLoyaltyCardTrx && 
                (custInfo.GetDataString(_T("SavePreferences")) == _T("")) )
            {
                if ( IsPersonalizedLanguageChange() || IsPersonalizedVolumeChange() )
                {
                    bReturn = true;
                }
            }
        }
    }
    
    return bReturn;
}

//+SR815.5 Personalized Volume
bool SMStateBase::IsPersonalizedLanguageChange()
{
    bool bReturn = false; 
    if ((custInfo.GetOption(_T("PersonalizeLanguage")) == _T("Y")) && 
        (custInfo.GetDataString(_T("LangSelectionMethod")) == _T("Button")) &&
        (m_customerLanguage != GetPreferredLanguage(true)))
    {
        bReturn = true;
    }
    
    return bReturn;
}

bool SMStateBase::IsPersonalizedVolumeChange()
{
    bool bReturn = false;
    
    if ( co.IsShowVolumeControl() )
    {
        try
        {
            CVolumeControl *pVolumeControl = new CVolumeControl();        
            int nCurrentVolume = pVolumeControl->GetVolumeLevel();
            int nDefaultVolume = pVolumeControl->GetDefaultVolume();
            CString csVolume = custInfo.GetPreference(_T("VolumeLevel"));
            CString csVolumeChangeMethod = custInfo.GetDataString(_T("VolumeChangeMethod"));
            bool bVolumeButtonUsed = (csVolumeChangeMethod.Compare(_T("Button")) == 0);
            if (!csVolume.IsEmpty())
            {
                int nPersonalizedVolume = _ttoi( custInfo.GetPreference(_T("VolumeLevel")) );
                if (bVolumeButtonUsed &&
                    //(nCurrentVolume != nDefaultVolume) && // SSCOP-2657
                    (nPersonalizedVolume != nCurrentVolume))
                {
                    bReturn = true;
                }
            }
            else  if ( (nCurrentVolume != nDefaultVolume) && bVolumeButtonUsed)
            {
                bReturn = true;
            }

            delete pVolumeControl;
            pVolumeControl = NULL;
        }
        catch( CException *e )
        {
            _TCHAR szError[255] = {NULL};
		    e->GetErrorMessage(szError, 255);
		    trace(L6, _T("SMStateBase::IsPersonalizedVolumeChange - Got an exception! Error text: %s"), szError);
		    e->Delete();
        }
    }    

    return bReturn;    
}

void SMStateBase::PreferenceUpdate()
{  
	if (fLoyaltyCardTrx)   //Only update if TB has confirmed this is a loyalty card
	{
		if ( (custInfo.GetDataString( _T("SavePreferences") ) == _T("Y") ))
		{
			// Update Personalized Volume
			try
			{
				CVolumeControl *pVolumeControl = new CVolumeControl();        
				int nCurrentVolume = pVolumeControl->GetVolumeLevel();
				
				CString csVolumeLevel;
				csVolumeLevel.Format(_T("%d"), nCurrentVolume);
				
                if(IsPersonalizedVolumeChange())
                {
                    custInfo.SetPreference(_T("VolumeLevel"), csVolumeLevel);
                    trace(L6,_T("SMStateBase::PreferenceUpdate - New personalized volume level: %d"), nCurrentVolume);
                }
				delete pVolumeControl;
				pVolumeControl = NULL;
			}
			catch( CException *e )
			{
				_TCHAR szError[255] = {NULL};
				e->GetErrorMessage(szError, 255);
				trace(L6, _T("SMStateBase::IsPersonalizedVolumeChange - Got an exception! Error text: %s"), szError);
				e->Delete();
			}
			
			// Update Personalized Language
			if (custInfo.GetPreference(_T("LangSelectionMethod")) == _T("None"))
			{ //Restore access to preference value sent by server
				custInfo.SetPreference(_T("LangSelectionMethod"), custInfo.GetPreference(_T("LangSelectionMethod"), true));
			}
		}
		//+SSCOP-3426
	    else
		{
			if( fInVoidTrans == true || co.IsAttendantModeOn())
			{
				custInfo.SetDataString(_T("SavePreferences"),_T("N"));
			}
		}
		//-SSCOP-3426
		
        //custInfo.UpdatePreferences();  //transferred below - SR814.5 SR815.6 SR851 Personalization and Security  
	}
    
    //+SR814.5 SR815.6 SR851 Personalization and Security  
    custInfo.SetDataString(_T("ThemeUsed"), GetPersonalThemeUsed());
    
    CString csLCID = ps.GetLCID(m_customerLanguage);
    CString language = ps.GetISOLanguageCode(csLCID); 
    custInfo.SetDataString(_T("LanguageUsed"), language);
    custInfo.SetDataString( _T("UpdateMetrics"), _T("Y")); //Signal PersonalizationServer to do Trust level update
    trace(L5, _T("PersonalizationMetrics: setting Theme = %s and Language = %s."), GetPersonalThemeUsed(), language );

    //always send updates for loyalty and non-loyalty transactions
    custInfo.UpdatePreferences(); 
    //+SR814.5 SR815.6 SR851 Personalization and Security

}
//-SR815.5 Personalized Volume
//+SR815 - Personalization
///////////////////////////////////////////////////////////////////////////////
// Function:    LoyaltyCheck                                
// Class:       SMStateBase                                          
// Description: Call CustInfo method to filter possible loyalty data and
//              perform lookup on data meeting filter criteria
// Parameters:  csLookupType - Swipe, Scan, or Loyalty    
// Comments:    Filters are configured in NCRPersonalization.xml file
//                    
// Returns:     Nothing                                     
///////////////////////////////////////////////////////////////////////////////////
bool SMStateBase::LoyaltyCheck(CString csLookupType, CString csLookupData)
{
    bool bLookupStarted = false;
    long lLoyaltyCheckTimer = custInfo.VerifyAndGetLookupTimer(csLookupType, csLookupData);
    if (lLoyaltyCheckTimer > 0)
    {
        bLookupStarted = true;
    }
    return bLookupStarted;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    GetAccountDataFromMSRSwipe                              
// Class:       SMStateBase                                          
// Description: Isolate the portion of a card swipe to be pattern-matched for loyalty
// Parameters:  csLookupType - Swipe, Scan, or Loyalty    
// Comments:    Filters are configured in NCRPersonalization.xml file
//                    
// Returns:     Nothing                                     
///////////////////////////////////////////////////////////////////////////////////
CString SMStateBase::GetAccountDataFromMSRSwipe(CString csAllTrackData)
{
            //+codes here are copied from SMStateBase::GetAccountNum
            //use LPTSTR SMStateBase::cDMLastTrack to get the track data
            //get track and validate
            CString csCardData;
            short int nT1, nT2, nT3;
            short int nShortLen = sizeof(short);
            
            nT1 = nT2 = nT3 = 0;
            nT1 = (short ) *(  (const char *)cDMLastTrack);						//length of Track 1
            nT2 = (short ) *( ((const char *)cDMLastTrack) + nShortLen);		//length of Track 2
            nT3 = (short ) *( ((const char *)cDMLastTrack) + (nShortLen * 2));	//length of Track 3
            
            _TCHAR szTrack1[300];
            _TCHAR szTrack2[300];
            _TCHAR szTrack3[300];
            
            int nPosition = 3*nShortLen;
            memcpy(szTrack1, ((const char *)cDMLastTrack + nPosition), nT1*sizeof(_TCHAR));
            szTrack1[nT1] = '\0';
            
            nPosition += nT1 * sizeof(_TCHAR);
            memcpy(szTrack2, ((const char *)cDMLastTrack + nPosition), nT2*sizeof(_TCHAR));
            szTrack2[nT2] = '\0';
            
            nPosition += nT2 * sizeof(_TCHAR);;
            memcpy(szTrack3, ((const char *)cDMLastTrack + nPosition), nT3*sizeof(_TCHAR));
            szTrack3[nT3] = '\0';

            //Base implementation will compare against track 2 data
            csCardData = szTrack2;
            int nPosEqualSign = csCardData.Find(_T("="));
         
            if (nPosEqualSign > 0)
                return csCardData.Mid(0,nPosEqualSign);
            else
                return csCardData;
            
}
//-SR815 - Personalization

//+SR814.5 SR815.6 SR851 Personalization and Security
void SMStateBase::SetPersonalTheme(CString csForRedrawLock)
{
    CString csTheme = custInfo.GetPreference(_T("Theme"));

    if (csTheme.GetLength())
    {
        if(csForRedrawLock.GetLength())
        {
            CPSXRedrawLock csRedrawLock(_T("Display"), csForRedrawLock);
        }

        m_pAppPSX->SetTheme(csTheme);
        CString csTestTheme;
        m_pAppPSX->GetTheme(csTestTheme);
        if (csTestTheme != csTheme)
        {
            m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
        }
            
    }
    else
    {
        m_pAppPSX->SetTheme(custInfo.GetOption(_T("LaneTheme")));
    }
    
    CString csThemeUsed;
    m_pAppPSX->GetTheme(csThemeUsed);

    SetPersonalThemeUsed(csThemeUsed);  
}

void SMStateBase::SetPersonalThemeUsed(CString csThemeUsed)
{
    csPersonalThemeUsed = csThemeUsed;
}

CString SMStateBase::GetPersonalThemeUsed()
{
    return csPersonalThemeUsed;
}
//-SR814.5 SR815.6 SR851 Personalization and Security

//////////////////////////////////////////////////////////////////////
//End of SMStateCUbase

#endif //_CPPUNIT
SMStateBase *SMStateBase::OnMobileTransactionStart(CString csTransactionInfo)
{
    trace(L6, _T("+SMStateBase::OnMobileTransactionStart csTransactionInfo = %s"), csTransactionInfo);
    return STATE_NULL;
}
