//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdateBase.CPP 
//
// TITLE: Class implementation for store mode Enter Birthdate state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmEnterBirthdate.h"     // This state
#include "SMSmAuthorization.h"      // MGV auth state
#include "SMSmRestrictedItems.h" 

#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SmEnterBirthdateBase")

IMPLEMENT_DYNCREATE(SMSmEnterBirthdateBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmEnterBirthdate)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmEnterBirthdateBase::SMSmEnterBirthdateBase()
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::Initialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmEnterBirthdate") );	// RFC 330050
    csBirthDate = _T("");                 // Birthdate input by user
    // NewUI
	ps.SetFrame(_T("SmEnterBirthdate"));
    // NewUI
	
	CString csWork; 

	if (co.fLocaleDateSettingNotFromRegionalSetting)
	{
        csWork = ps.GetPSText(LTT_BIRTHDATE_TOUCH_ENTER) +  _T("\n") + ps.GetPSText(LTT_MMDDYY); //SR803
		ps.SetLeadthruText(csWork,_T(""));
	}
	else
	{
        csWork.Format(ps.GetPSText(LTT_BIRTHDATE_TOUCH_ENTER), ps.DateFormat());//Personallization age prompt and SR803
		ps.SetLeadthruText(csWork , _T(""));
	}

    ps.ShowSMMNumericKeypad(true);
    // allow the EchoInput() func to accept up to max digits
    // BeginCodeLocale
    int nNoTotalDigit = 4; // By definition: 2 digits for month and for days
    if (co.nLocaleNoCenturyDigit==0) nNoTotalDigit += 2;
    if (co.nLocaleNoCenturyDigit==1) nNoTotalDigit += 4;  
     ps.AllowInput(nNoTotalDigit,false,false);
    // EndCodeLocale

    //+ Personalization age prompt
    ps.ShowFrame();
    csCurContext = _T("SmEnterBirthdate");

    bShowApproveButton = false; //SSCOP-949
    COleVariant v = custInfo.GetPreference(_T("Birthdate"));
    CString csDOB = v.bstrVal;
    trace(L7, _T("GetPreference-Birthdate = %s"), csDOB);
    if ((custInfo.GetOption(_T("PersonalizedAgePrompt")).CompareNoCase(_T("y"))==0) &&
        (csDOB.GetLength() > 0))
    {
        bShowApproveButton = true; //SSCOP-949
        ps.GetPSXObject()->SetTransactionVariable( _T("Echo"), v );
        ps.SetInputText(csDOB);
        ps.EnterClearKeys(false, false); //Enable Clear and Enter buttons
        if (co.fStateDOBNotNeededButton)
        {
            ps.ButtonEnable(_T("SMButton2"), false);
	        v = VARIANT_TRUE;
	        m_pAppPSX->SetConfigProperty(_T("SMButton2"), csCurContext, UI::PROPERTYVISIBLE, v);
        }
        else
        {   
	        v = VARIANT_FALSE;
	        m_pAppPSX->SetConfigProperty(_T("SMButton2"), csCurContext, UI::PROPERTYVISIBLE, v);
        }

        //+SR803 - When Age Prompt is on and if there's pre-populated birth date
        //show but disable "Do Not Approve" button 
        //ps.ButtonEnable(_T("SMButton3"), false); // SSCOP-3564                           
        v = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("SMButton3"), csCurContext, UI::PROPERTYVISIBLE, v);

        //hide "Go Back" button
        v = VARIANT_FALSE;
        m_pAppPSX->SetConfigProperty(_T("SMButton8"), csCurContext, UI::PROPERTYVISIBLE, v);
        //-SR803
    }
    else
    {   
	    v = VARIANT_FALSE;
	    m_pAppPSX->SetConfigProperty(_T("SMButton2"), csCurContext, UI::PROPERTYVISIBLE, v);
        
        //+SR803
        m_pAppPSX->SetConfigProperty(_T("SMButton3"), csCurContext, UI::PROPERTYVISIBLE, v);

        v = VARIANT_TRUE;
        m_pAppPSX->SetConfigProperty(_T("SMButton8"), csCurContext, UI::PROPERTYVISIBLE, v); 
        //-SR803
    }

    //- Personalization age prompt
    return STATE_NULL;                // stay in SmEnterBirthdateBase state
}

void SMSmEnterBirthdateBase::UnInitialize()
{
  ps.PSHideSMMNumericKeypad();
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmEnterBirthdate") );	// RFC 330050
}

/////////////////////////////////////////////
// PSButton8 - Cancel
/////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::PSButton8(void)
{
  //ps.PSHideSMMNumericKeypad();
  //+ Personalization age prompt
  //+SR803 
  /* 
  CString csDOB = custInfo.GetPreference(_T("Birthdate"));
  COleVariant v;
  if((custInfo.GetOption(_T("PersonalizedAgePrompt")).CompareNoCase(_T("y"))==0) &&
      (csDOB.GetLength() > 0))
  {
      return SMSmRestrictedItemsBase::PSButton8();
  }
  else
  */
  //-SR803
  //{
    RETURNSTATE(SmRestrictedItems)
  //}
  //- Personalization age prompt
}

/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmEnterBirthdateBase::PSClearKey()
{
  PSClearAllInput();
  //+ Personalization age prompt
     CString csDOB = custInfo.GetPreference(_T("Birthdate"));
     COleVariant v;
    if (bShowApproveButton) //SSCOP-949
    {
        if(co.fStateDOBNotNeededButton) //SR803 - moved checking here for DOBNotNeeded button
        {
            ps.ButtonEnable(_T("SMButton2"), true);
        }
        
        ps.ButtonEnable(_T("SMButton3"), true); //SR803: enable "Do Not Approve" button if birthdate entry is cleared                
    }
//- Personalization age prompt
  return STATE_NULL;
}


//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::PSEnterKey(void)
{
  SMStateBase *tempState = VerifyBirthDate();
  bReturnToRestrictedItems = false;  //SSCOADK-7224
  PSClearAllInput();
  return tempState;

}

///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmEnterBirthdateBase::PSNumericKey(const long lKey)
{
    PSEchoInput(lKey);
    
    if (bShowApproveButton) //SSCOP-949 - disable approve when data entered.
    {
        if(co.fStateDOBNotNeededButton) 
        {
            ps.ButtonEnable(_T("SMButton2"), false);
        }
        
        //ps.ButtonEnable(_T("SMButton3"), false); // SSCOP-3564
    }
    return STATE_NULL;
}

