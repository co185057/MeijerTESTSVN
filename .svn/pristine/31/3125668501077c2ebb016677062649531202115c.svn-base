//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnterBirthdate.CPP
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

#include "transbrokerssf.h"
#include "SMSmAuthorization.h"
#include "SMInProgress.h"
#include "SMSmAssistMenu.h"
#include "SMSmVoidAgeRestrictedItem.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmEnterBirthdate")

IMPLEMENT_DYNCREATE(SMSmEnterBirthdate, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmEnterBirthdate::SMSmEnterBirthdate()
{
  IMPLEMENT_TIMESTAMP
}

//*********************************************************************************
// Notes: 	Single partial assist mode screen used for Age Restricted Approval in Meijer DG30.
//		Integrate SMSmRestrictedItems functionality in SMSmEnterBirthdate
//********************************************************************************* 
SMStateBase * SMSmEnterBirthdate::Initialize(void)
{
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmEnterBirthdate::Initialize"));
	ra.RequestEnableAssistMode(false);  	// disable assist mode in rap
	
	SMStateBase *pRetState = SMSmEnterBirthdateBase::Initialize();
	
	fStateAssist = true;
	
	CString strPropValue = GetHookProperty(_T("AgeRequiredLeadThru"));
	
	if(strPropValue.Find(_T("SCAN")) == -1 )
	{
		DMScannerDisable();
		m_pAppPSX->SetConfigProperty(_T("LeadthruText"), 
			_T("SmRestrictedItems"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
	else
		DMScannerEnable();
	
    //sotf 6226
    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION) == false)
    {
		strPropValue = GetHookProperty(_T("RestrictedAge_Prop"));
    }else
    {
        strPropValue.Empty();
        strPropValue.Format(_T("%d"), FindRequiredAge());
    }
    //-sotf
    
	CString csTitle2;
	csTitle2.Format(_T("Age %s Restricted Item"), strPropValue);
	COleVariant v;
	v = csTitle2;
	
	m_pAppPSX->SetConfigProperty(_T("Title2"), _T("SmEnterBirthdate"), UI::PROPERTYTEXTFORMAT, v);
	
	// TAR 362597 - make sure Button 2 is enabled (DOB Not Needed) 
	m_pAppPSX->SetConfigProperty(_T("SMButton2"), _T("SmEnterBirthdate"), UI::PROPERTYSTATE, (long) UI::Normal);
	
	return pRetState;
}

void SMSmEnterBirthdate::UnInitialize(void)
{
    SMSmEnterBirthdateBase::UnInitialize();
}

//************************************************************************
// 
// Function:	PSButton2()
// 
// Purpose:		Handles when date not needed button is pressed
// 
// Returns:		SMStateBase *
// 
//************************************************************************ 

SMStateBase  *SMSmEnterBirthdate::PSButton2(void)
{
    //sotf 6226
    SMStateBase *returnState;
    returnState = SMSmRestrictedItemsBase::PSButton2();

    delete getRealStatePointer(returnState);	//TAR 376302 delete this pointer to free the memory

    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {
        lUnapprovedRestrictedItems =0;
        lRegularYellowNeedingApproval = 0;
        bInApprovalNeeded = false; 
        fApprovedForRestrictedItems = true;
        fAgeObtained = true;
        nCustomerAge = FindRequiredAge();

        SetAgeRestrictedItemStatus();
        while (m_lAgerestrictQueuedtoRAP > 0)
        {
            trace(L7, _T("+ra.OnAgeApproval"));
            ra.OnAgeApproval();
            trace(L7, _T("-ra.OnAgeApproval"));
            m_lAgerestrictQueuedtoRAP--;
        }
        m_lAgerestrictQueuedtoRAP = 0;

        
        
        ra.OnNoRAPDataNeeded();

        
        TBEnterDOB(0, 0, 0,nCustomerAge);

        SendTBCommand(TBCMD_SET_DOB_ENTERED_STATUS, 2);        
        SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 1);
        SendTBCommand(TBCMD_SEND_AGE_VERIFY_DATA);

        fStateAssist = false;
        //SOTF 7384
        m_bGoBackAfterAgeRestrictedItem = true;
        RETURNSTATE(SmAuthorization)
    }
    //-sotf

    
	fStateAssist = false;
    // send No DOB required to POS
    trace(L3, _T("SMSmRestrictedItems::PSButton2 Send No DOB Needed to POS"));
    CString csTest(_T("{F1}"));
    SendTBCommand(TBCMD_SENDKEYEDDATA, csTest);
   
    //TAR306722, preventing from going into SMSmAuthorization that would cause
    //to neglect the added bagging scale weight    
    SAClearSecurityCondition();

	if( GetHookProperty(_T("PropIsPrcVerify")) == _T("0") )
	{
		if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
		{
			setAnchorState(BES_FINISH);
			RETURNSTATE(InProgress)
		}
		else
        {   
            // +SOTF6481
            setAnchorState(BES_SCANANDBAG);
            RETURNSTATE(InProgress);
			//return setAndCreateAnchorState(BES_BAGANDEAS);
            // -SOTF6481
        }
	}
	else
	{
		return STATE_NULL;
	}

}

//************************************************************************
// 
// Function:	PSButton8()
// 
// Purpose:		Handles when reject/cancel button is pressed
// 
// Returns:		SMStateBase *
// 
//************************************************************************ 
SMStateBase * SMSmEnterBirthdate::PSButton8(void)
{
	//sotf 6226
    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {
        fStateAssist = false;
        bInApprovalNeeded = false; 
        bStoreModeGoBack = true;
        bForceApprovalBeforeSuspend = false;
		
        if(!isRestrictedItemApprovalRequired())
        {     
            
            ra.OnNoRAPDataNeeded();
			
        }else
        {
            if(ps.RemoteMode())
            {
                bForceApprovalBeforeSuspend = true; 
            }
        }
		
        setAnchorState(BES_SCANANDBAG);
		
        if ((co.IsInAssistMenus()) || (ps.RemoteMode()))
        {
            ra.RequestEnableAssistMode(false);	//TAR356813
            RETURNSTATE(SmAssistMenu)
        }
        
        RETURNSTATE(SmAuthorization)
			
    }
    //-sotf
	fStateAssist = false;
	
	SendTBCommand(TBCMD_SET_TB_STATE,_T("CANCEL_AGE_RESTRICTED_ITEM"));
	
	lUnapprovedRestrictedItems =0;
	lRegularYellowNeedingApproval = 0;
	bInApprovalNeeded = false; //TAR 279092
	fApprovedForRestrictedItems = true;
	fAgeObtained = true;
	nCustomerAge = 99;
	
	CString csAgeCheck = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
	csAgeCheck +=  _T("\n") + ps.GetPSText(BTT_PRODUCE /*age check canceled*/, SCOT_LANGUAGE_PRIMARY);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csAgeCheck);
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csAgeCheck);
	trace(L7, _T("-ra.OnNormalItem"));  
	
	//TAR 196910 start
	while (m_lAgerestrictQueuedtoRAP > 0)
	{
		trace(L7, _T("+ra.OnAgeApproval"));
		ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
	}
	m_lAgerestrictQueuedtoRAP = 0;
	//TAR 196910 end
	
	SAClearSecurityCondition();
	
	if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
	{
		setAnchorState(BES_FINISH);
		RETURNSTATE(InProgress)
	}
	else
		return setAndCreateAnchorState(BES_SCANANDBAG);
}

//************************************************************************
// 
// Function:	PSEnterKey()
// 
// Purpose:		Handles when enter key is pressed
// 
// Returns:		SMStateBase *
// 
//************************************************************************ 
SMStateBase * SMSmEnterBirthdate::PSEnterKey(void)
{
    //sotf 6226
    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {

        long lAgeVerification = VerifyDOB();
        PSClearAllInput();
        
        fStateAssist = false;
        switch(lAgeVerification)
        {
            case AGE_NOT_APPROVED:
                {

                    SetAgeRestrictedItemStatus();
                    RETURNSTATE(SmVoidAgeRestrictedItem);
                }

            case AGE_INVALID:
                {
                    return STATE_NULL;
                }

            case AGE_APPROVED:
                {
                    lUnapprovedRestrictedItems =0;
                    lRegularYellowNeedingApproval = 0;
                    bInApprovalNeeded = false; 
                    fApprovedForRestrictedItems = true;
                    fAgeObtained = true;
                    ra.OnNoRAPDataNeeded();

                    SetAgeRestrictedItemStatus();

                    if(m_bReEnterDOB)
                    {
                         m_bReEnterDOB = false;
                         SendTBCommand(TBCMD_SET_DOB_ENTERED_STATUS, 3);
                    }

                    SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 1);
                    SendTBCommand(TBCMD_SEND_AGE_VERIFY_DATA);
                    
                    if ((co.IsInAssistMenus()) || (ps.RemoteMode()))
                    {
                        ra.RequestEnableAssistMode(false);	//TAR356813
                        RETURNSTATE(SmAssistMenu)
                    }

                    //SOTF 7384
                    m_bGoBackAfterAgeRestrictedItem = true;

                    RETURNSTATE(SmAuthorization)
                }

        }

       

    }
    // -sotf

	CString csEntryDate;
	CString csWork;
	fStateAssist = false;

	int nEntryDateLength = ps.GetInput(csEntryDate);
	SMStateBase  *returnState  = SMSmEnterBirthdateBase::PSEnterKey();

	// +TAR 362535
    CString strPropValue = GetHookProperty(_T("RestrictedAge_Prop"));

	int nRestrictedAge = _ttoi(strPropValue);
	int nInputAge = GetAge(csEntryDate);
	
	CString csToday;
	COleDateTime dtToday = COleDateTime::GetCurrentTime();
	csToday.Format(ps.DateFormat(dtToday.GetMonth(), dtToday.GetDay(), dtToday.GetYear()));
	int nYearToday = _ttoi(csToday.Right(2));
	int nInputBirthYear = _ttoi(csEntryDate.Right(2));
	// -TAR 362535

	//tar306723
	bAlreadyPlayedWelcome = true;

	trace(L3, _T("SMSmEnterBirthdate::PSEnterKey birthdate input %s"), csEntryDate);
	trace(L3, _T("SMSmEnterBirthdate::PSEnterKey approvals needed %d"), lUnapprovedRestrictedItems);

	// TAR 362535 - Check the date input if valid (equal to 6 digits)  and check if input age is not zero or year today is equal to year inputed
	if ( (0 == lUnapprovedRestrictedItems) && (6 == nEntryDateLength) && 
		((nInputAge != 0 ) || (nYearToday == nInputBirthYear)) )
	{
        //sotf 6226
        if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION) == false)
        {
      
		    CString csTest = csEntryDate + _T("{ENTER}");
		    SendTBCommand(TBCMD_SENDKEYEDDATA, csTest);
        }
        //-sotf

		// lAge1 is the minimum age
		// lAge2 is the input age
		long lAge1 = (long) nRestrictedAge;
		long lAge2 = (long) nInputAge;
		if ( lAge2 < lAge1)
		{
			// log message to RAP
			CString csBirthDateEnter;
			csBirthDateEnter.Format(ps.GetPSText(SSF_TXT_GUEST_AGE, SCOT_LANGUAGE_PRIMARY), lAge1);
			ps.AddMessageToReceipt(_T("CMSMReceipt"), csBirthDateEnter);
			DMSayPhrase(SETITEMASIDECONTINUEWITHPURCHASES);
			
			SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);

			delete getRealStatePointer(returnState);     
			if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
			{
				setAnchorState(BES_FINISH);
				RETURNSTATE(InProgress)
			}
			else
				return setAndCreateAnchorState(BES_SCANANDBAG);
		}
		//TAR306722, preventing from going into SMSmAuthorization that would cause
		//to neglect the added bagging scale weight 
		else  
		{
			delete getRealStatePointer(returnState);
			SAClearSecurityCondition();   

			Sleep(200);		// TAR 365482	Wait for POS so that FL will not go to Assistance Needed Screen again.
			
			if( GetHookProperty(_T("PropIsPrcVerify")) == _T("0") )
			{
				if( GetHookProperty(_T("IsResumeTrxInProgress")) == _T("1") )
				{
					setAnchorState(BES_FINISH);
					RETURNSTATE(InProgress)
				}
				else
                {
                    // +SOTF6481
                    setAnchorState(BES_SCANANDBAG);
                    RETURNSTATE(InProgress);
					//return setAndCreateAnchorState(BES_BAGANDEAS);
                    // -SOTF6481
                }
			}
			else
			{
				return STATE_NULL;
			}
		}
	}
	return returnState;
}

SMStateBase * SMSmEnterBirthdate::PSClearKey(void)
{
    return SMSmEnterBirthdateBase::PSClearKey();
}

SMStateBase * SMSmEnterBirthdate::PSNumericKey(const long lKey)
{
    return SMSmEnterBirthdateBase::PSNumericKey(lKey);
}

//************************************************************************
// 
// Function:	DMScanner()
// 
// Purpose:		Handles when there is scanned item
// 
// Returns:		SMStateBase *
// 
//************************************************************************ 

SMStateBase *SMSmEnterBirthdate::DMScanner(void)
{	
    if(SendTBCommand(TBCMD_ACS_DELAYED_AGE_RESTRICTION))
    {
        return STATE_NULL;
    }
	fStateAssist = false;
	csItemSent = csDMLastBarCode;

	lUnapprovedRestrictedItems =0;
	lRegularYellowNeedingApproval = 0;
	bInApprovalNeeded = false; //TAR 279092
	fApprovedForRestrictedItems = true;
	fAgeObtained = true;
	nCustomerAge = 99;
    
	//TAR 196910 start
	while (m_lAgerestrictQueuedtoRAP > 0)
	{
		trace(L7, _T("+ra.OnAgeApproval"));
		ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
	}
	m_lAgerestrictQueuedtoRAP = 0;

	SAClearSecurityCondition(); 
	
	bEASReEnableScanner = true;

	return TBItemSale(false,0,true);	
}


long SMSmEnterBirthdate::VerifyDOB(void)
{
  CString csEntryDate;
  CString csWork;

  DMSayPhrase(KEYPADVALID);

  int nEntryDateLength = ps.GetInput(csEntryDate);

  if (!nEntryDateLength)
	return AGE_INVALID;

  COleDateTime currentDate = COleDateTime::GetCurrentTime();

  int nNoCenturyDigit=0;
  int nNoTotalDigit=0;

  if (co.nLocaleNoCenturyDigit==0) nNoCenturyDigit = 2;
  if (co.nLocaleNoCenturyDigit==1) nNoCenturyDigit = 4;

  nNoTotalDigit = nNoCenturyDigit + 4;

  // Not physically allowed to enter more, only less
  if (nEntryDateLength == nNoTotalDigit)
  {
    int birthMonth=0;
    int birthDay=0;
    int birthYear=0;

    switch (co.nLocaleShortDateOrdering)
    {
      case 0: // Month-Day-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthMonth  = _ttoi(csEntryDate.Left(2));
          birthDay    = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 1: // Day-Month-Year
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthDay    = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthYear   = _ttoi(csEntryDate.Right(4));
        }
        break;
      case 2: // Year-Month-Day
        if ((csEntryDate.GetLength()==6) && (nNoCenturyDigit==2))
        {
          birthYear   = _ttoi(csEntryDate.Left(2));
          birthMonth  = _ttoi(csEntryDate.Mid(2,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        if ((csEntryDate.GetLength()==8) && (nNoCenturyDigit==4))
        {
          birthYear   = _ttoi(csEntryDate.Left(4));
          birthMonth  = _ttoi(csEntryDate.Mid(4,2));
          birthDay    = _ttoi(csEntryDate.Right(2));
        }
        break;
      default:
        break;
    }


    if (nNoCenturyDigit==2)
    {
		// +SSCOI-44480: The old codes assumes that the year entered was 19xx.
		// it did not check for the the year 20xx.
       //int yy = _ttoi(currentDate.Format(_T("%y")));
       //if ( birthYear >= yy )
		   
	   if ( currentDate.GetYear() < 2000 || birthYear > _ttoi(currentDate.Format(_T("%y"))) ) // SSCOI-44880
       {
		     birthYear += 1900;
       }
       else
       {
		     birthYear += 2000;
       }
    }

    COleDateTime birthDate(birthYear, birthMonth, birthDay, 0, 0, 0);

    // If the date isn't valid (e.g. 11/35/62 or 02/29/98 (not leap yr.)),
    // drop outta here.
    if (birthDate.GetStatus() == COleDateTime::valid)
    {
	  nCustomerAge = ps.ageFromDateTime(birthDate);
      CString msgText;

      csBirthDate = csEntryDate;  // mmddyy for restricted items needed in exit code MGG


	  CString csBirthDateEnter;
	  csBirthDateEnter.Format(ps.GetPSText(RA_BIRTHDATEENTER, SCOT_LANGUAGE_PRIMARY), ps.DateFormat(birthMonth, birthDay, birthYear), nCustomerAge);
	  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csBirthDateEnter);
	  ra.OnNormalItem(_T("[Summary]") + csBirthDateEnter);
	  trace(L7, _T("-ra.OnNormalItem"));

	  static long lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;	// Need to save flag value
	  
      // Check for under age limits with respect to the types of items
      // purchased that require approval.  Set error msg text as appropriate.
      if ( long ltmpUnapprRestItems = ps.RestrictedItemNotAllowed(nCustomerAge,  lSavedlUnapprovedRestrictedItems) ) //TAR249400
      {
        // both item types need approval - under age
        msgText = ps.GetPSText(MSG_UNDER_RESTRICTED_AGE_LIMIT);

	    
	    TBEnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("TBEnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;

		//If entered DOB has approved some items, we need to decrement the flags
		if ( ltmpUnapprRestItems < lUnapprovedRestrictedItems )
		{
			lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;
			lUnapprovedRestrictedItems = ltmpUnapprRestItems;
		 	lRestrictedItems = lRestrictedItems - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
		 	lRegularYellowNeedingApproval = lRegularYellowNeedingApproval - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
			m_lAgerestrictQueuedtoRAP = m_lAgerestrictQueuedtoRAP - lSavedlUnapprovedRestrictedItems + ltmpUnapprRestItems;
		}
		

      }
      else
      {
		// All is well.  One or both types of restricted items
		// are now authorized.  We should never come here again
		// during this transaction.  Even if we have just authorized
		// only one type of restricted item, we have obtained a valid
		// age and can set approved flags for any future use.

		TBEnterDOB(birthYear, birthMonth, birthDay,nCustomerAge); // inform TB about customer age with date info
	    CString strFmt ;
	    strFmt =_T("TBEnterDOB--birthYear:%d; birthMonth:%d; birthDay:%d; nCustomerAge:%d") ;
	    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, strFmt, birthYear, birthMonth, birthDay,nCustomerAge) ;

		// TAR 167991 - DMSayPhrase(KEYPADVALID);
		fAgeObtained = true;
		fApprovedForRestrictedItems = true;
		lUnapprovedRestrictedItems = 0;
		lRegularYellowNeedingApproval = 0;

        // remote approval
        CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);
		
		if (m_lAgerestrictQueuedtoRAP)
		{
		trace(L7, _T("+ra.OnAgeApproval"));
        ra.OnAgeApproval();
		trace(L7, _T("-ra.OnAgeApproval"));
		m_lAgerestrictQueuedtoRAP--;
		}
		

	    return AGE_APPROVED;
      }

      // All is not well - the customer does not meet one or both of the age limits.
      DMSayPhrase(SECURITYMED);	// Issue error audio msg.
      
      ps.SetLeadthruText(csWork, _T(""));
      return AGE_NOT_APPROVED;
      

    }
    else
    {
      // If we get to here, the date entered is invalid, but the right length
      DMSayPhrase(SECURITYMED);

      // format with slashes to look like a date
      switch (co.nLocaleShortDateOrdering)
      {
          case 0: // Month-Day-Year
          case 1: // Day-Month-Year
              csWork.Format(_T("%s: %s%s%s%s%s"),
		                    ps.GetPSText(MSG_DATEINVALID),
                            csEntryDate.Left(2),
                            co.csLocaleDateSeparator,

		                    csEntryDate.Mid(2,2),
		                    co.csLocaleDateSeparator,
		                    csEntryDate.Right(nNoCenturyDigit));
              break;
          case 2: // Year-Month-Day
              csWork.Format(_T("%s: %s%s%s%s%s"),
		                    ps.GetPSText(MSG_DATEINVALID),
                            csEntryDate.Left(nNoCenturyDigit),
                            co.csLocaleDateSeparator,
		                    csEntryDate.Mid(2,2),
		                    co.csLocaleDateSeparator,
		                    csEntryDate.Right(2));
              break;
          default:
              break;
      }
      
      
      ps.SetLeadthruText(csWork, _T(""));	
      return AGE_INVALID;
      
    }
  }

  // if we get here the date entered was an incorrect length
  DMSayPhrase(SECURITYMED);
  csWork.Format(ps.GetPSText(MSG_DATE_FORMAT_INVALID), ps.DateFormat(), csEntryDate);
  
  
  ps.SetLeadthruText(csWork, _T(""));
  return AGE_INVALID;
  

}

long SMSmEnterBirthdate::FindRequiredAge(void) 
{
      CStringArray csItemEntryIdList;
	  m_pAppPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdList);
	  int nCountList = csItemEntryIdList.GetSize();

	  int i = nCountList - 1;

      int lAge = 0;
      int iTempAge = -1;
      bool bAutoApprovedItem = false;
	  
	  CString csItemEntryId;
	  COleVariant vValue;
	  while (i>=0)
	  {
		csItemEntryId = csItemEntryIdList.GetAt(i);

		vValue.Clear();
		m_pAppPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
        vValue.ChangeType(VT_INT);
		lAge = vValue.lVal;

        vValue.Clear(); 
	    PSXRC rc = m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOID"), vValue);

        if (rc != PSX_SUCCESS)
        {
			rc = m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
        }

	
        bool bItemVoided = false;
        if(rc == PSX_SUCCESS)
        {
            bItemVoided = true;
        }
	

        if((lAge > iTempAge) && 
           (lAge != -1) && 
           (lAge !=0) &&
           (!bItemVoided))
        {

            iTempAge = lAge;
        }

		i--;
	  }
	  return iTempAge;
}
void SMSmEnterBirthdate::SetAgeRestrictedItemStatus(void)
{
    CStringArray csItemEntryIdList, csParentItemEntryIdList;
    CString csItemEntryId, csParentItemId;


    m_pAppPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdList);
    long lTotalAgeItems = csItemEntryIdList.GetSize();

    for (int k=0; k < lTotalAgeItems; k++)
    {
      csItemEntryId = csItemEntryIdList.GetAt(k);
      m_pAppPSX->GetReceiptItemParent( _T("SMReceiptRestricted"), csItemEntryId, csParentItemId );
      
      if (!csParentItemId.GetLength())
      {
	      csParentItemEntryIdList.Add(csItemEntryId);
      }
      csParentItemId =""; 
    }


    long lAge = 0;
    COleVariant vValue;


	long i = csParentItemEntryIdList.GetSize() - 1;
    
    //SOTF 6968
    long lTotalAgeRestrictedItems =0, lTotalAgeItemsNotPassed =0;

    while (i >= 0)
    {
        vValue.Clear();
        csItemEntryId = csParentItemEntryIdList.GetAt(i);
        m_pAppPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
        vValue.ChangeType(VT_INT);
        lAge = vValue.lVal;

        
        vValue.Clear(); 
	    PSXRC rc = m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOID"), vValue);

        if (rc != PSX_SUCCESS)
        {
			rc = m_pAppPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
        }

	
        bool bItemVoided = false;
        if(rc == PSX_SUCCESS)
        {
            bItemVoided = true;
        }
       
        //SOTF 6968
        if((lAge > 0) && (!bItemVoided))
        {
            lTotalAgeRestrictedItems++;
        }

        if ((lAge > nCustomerAge)&&
            (!bItemVoided) && 
            (lAge != -1) &&
            (lAge != 0) &&
            (lAge != -2))    
        {
            lTotalAgeItemsNotPassed++;
        }
        //sotf 6968
        
        if ((!bItemVoided) &&
            (lAge != -1) &&
            (lAge != 0) &&
            (lAge != -2) &&
            (nCustomerAge >= lAge))    
        {   
            vValue = (long)-2;            
			m_pAppPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
	
        }

        i--;
    }

    //SOTF 6968
    if(lTotalAgeRestrictedItems == lTotalAgeItemsNotPassed)
    {
        SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 2);

    }

    if(lTotalAgeRestrictedItems > lTotalAgeItemsNotPassed)
    {
        SendTBCommand(TBCMD_SET_AGE_RESTRICTION_ITEM_ALLOWED_STATUS, 3);

    }
    //sotf 6968

}
