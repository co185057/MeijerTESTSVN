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
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV abstract base state
#include "SMStateCUBase.h"

#define COMP_ID ID_CU               // base state component
#define T_ID    _T("CUBase")


///////////////////////////////////////////////////////////////////////////////
// Function:    OnLookupComplete                                 
// Class:       SMStateBase::OnLookupComplete                                             
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
  trace(L6,_T("+OnLookupComplete "));	
  trace(L6,_T("-OnLookupComplete  %d"),byteCount);
  return STATE_NULL;
}

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
        case LOOKUP_COMPLETE:
            retState = OnLookupComplete(0);  
            break;
        default:
            trace(L6,_T("CUParse unhandled message(%d)"), nEvent );

    }
	
  if (szParms != NULL)
  {
      delete [] szParms;
  }
  trace(L7,_T("-CUParse"));
  return retState;
}

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
        return SCOT_LANGUAGE_PRIMARY;
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
    custInfo.SetPreference(_T("LangSelectionMethod"), csSelectionMethod);

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
    if  (fLoyaltyCardTrx && 
        (custInfo.GetOption(_T("PersonalizeLanguage")) == "Y") && 
        (custInfo.GetPreference(_T("SavePreferences")) == _T("?")) &&
        (custInfo.GetPreference(_T("LangSelectionMethod")) == _T("Button")) &&
        (m_customerLanguage != GetPreferredLanguage(true)))
    {
        bReturn = true;
    }
    
    return bReturn;
    
}

//////////////////////////////////////////////////////////////////////
//End of SMStateCUbase