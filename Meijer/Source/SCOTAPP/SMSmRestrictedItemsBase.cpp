//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmRestrictedItemsBase.CPP
//
// TITLE: Class implementation for store mode Restricted Items state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmRestrictedItems.h"    // This state
#include "SMSmVoidedItems.h"        // Voided items auth
#include "SMSmAuthorization.h"      // Auth state
#include "SMSmEnterBirthdate.h"     // 
#include "SMSmAssistMenu.h"      // MGV auth state
#include "SMSmAssistMode.h"         // TAR 391593

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmRestrictedItemsBase")


IMPLEMENT_DYNCREATE(SMSmRestrictedItemsBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmRestrictedItems)

DEFINE_TIMESTAMP

//////////////////////////////////////////
// Constructor
//////////////////////////////////////////
SMSmRestrictedItemsBase::SMSmRestrictedItemsBase()
: csCurContext(EMPTY_STRING)
{
  IMPLEMENT_TIMESTAMP
}

//////////////////////////////////////////////
// Initialize
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::Initialize(void)
{   
   //sscoadk-1859+
   //one special case: the item that has a linked item which is an age restricted, even it is one item, but 
   //UnapprovedRestrictedItems is 2
   //this code will search the SMReceiptRestricted receipt to see if there is any item which hasn't been voided.
   //If all items are voided, then we are leaving this state
   CStringArray csItemEntryIdListRestricted;
   CString csItemEntryId, csParentItemId;
   m_pAppPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdListRestricted);
   int nTmpCountListRestricted = csItemEntryIdListRestricted.GetSize();
   trace(L6, _T("There are %d restricted items on the receipt"), nTmpCountListRestricted);

   if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETING_APPROVALS)
   {
       //If processing during generic delayed,  set this so SMAssistMenu will reprocess if AgeRestricted interrupted
       m_GenericDelayedInterventionStatus=GENERIC_DELAYED_COMPLETE;  
   }
   //are they all voided (don't care about linked items)?
   
   COleVariant vValue;
   bool bAllItemsVoided = true;
   for (int k=0; k<nTmpCountListRestricted; k++)
   {
        csItemEntryId = csItemEntryIdListRestricted.GetAt(k);
        m_pAppPSX->GetReceiptItemParent( _T("SMReceiptRestricted"), csItemEntryId, csParentItemId );

        vValue.Clear();
        if (!csParentItemId.GetLength())
        {
            PSXRC rc = m_pAppPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_VOID"), vValue);
        }
        else if (csParentItemId.GetLength() && (csItemEntryId != csParentItemId ))
        {
         
            PSXRC rc = m_pAppPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csParentItemId, _T("ITEM_VOID"), vValue);
        }

         if (VARIANT_TRUE != vValue.boolVal)
         {
             bAllItemsVoided = false;
             trace(L6, _T("Found item on SmReceiptRestricted that has not been voided, staying at SmSmRestrictedItems"));
             break;
         }
         csParentItemId = "";
   }
     
   if (bAllItemsVoided == true )
       return TimedOut();	//SSCOP-4920 leaving this state
   else
       nTimeOut=0;

   //sscoadk-1859-
  bStoreModeApprovals = true;
  csBirthDate = "";                 // Birthdate input by user
  csCurContext = _T("SmRestrictedItems");  // Tar 238987
  ps.SetFrame(csCurContext);

  //sscob-2854+
  //If DOB has been entered, remove the approved items from SMReceiptRestriced receipt. 
   if (nCustomerAge > 0 )
   {
       CStringArray csItemEntryIdList, csParentItemEntryIdList;
       CString csItemEntryId, csParentItemId;
       m_pAppPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdList);
       int nTmpCountList = csItemEntryIdList.GetSize();
       trace(L6, _T("There are %d restricted items on the receipt"), nTmpCountList);

       for (int k=0; k<nTmpCountList; k++)
       {
            csItemEntryId = csItemEntryIdList.GetAt(k);
            m_pAppPSX->GetReceiptItemParent( _T("SMReceiptRestricted"), csItemEntryId, csParentItemId );
            if (!csParentItemId.GetLength())
            {
             csParentItemEntryIdList.Add(csItemEntryId);
            }
            csParentItemId =""; 
       }

       long ltmpUnapprRestItems = csParentItemEntryIdList.GetSize();
       long i = ltmpUnapprRestItems - 1;
       long lAge = 0;
   

       COleVariant vValue;
       while (i>=0)
       {
        vValue.Clear();
        csItemEntryId = csParentItemEntryIdList.GetAt(i);
        m_pAppPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
        vValue.ChangeType(VT_INT);
        lAge = vValue.lVal;

        if (nCustomerAge >= lAge && lAge != -1 && lAge !=0)    
        { 
           //remove approved item from the receipt. 
            m_pAppPSX->RemoveReceiptItem(_T("SMReceiptRestricted"), csItemEntryId); //remove items which meet age requirement
            m_pAppPSX->UpdateReceiptControls(_T("SMReceiptRestricted"));
        }
        i--;
       }
       //SSCOADK-756+
       //since age has been entered, button 1 (key in birthdate) and button 2 (Approve) should be disabled
       ps.ButtonState( _T("SMButton1"), true, false);
       ps.ButtonState( _T("SMButton2"), true, false);
       //SSCOADK-756-
   }
   //sscob-2854-

  ps.SetLeadthruText(TXT_SMM_EDITADD_INSTR, PS_BLANK); // SSCOP-1074 

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmRestrictedItems") );	// RFC 330050
  COleVariant v = VARIANT_TRUE;

  //SSCOADK-2074+
  if (co.IsInAssistMenus() || fStateAssist ) 
  {
      ps.ButtonState( _T("SMButton1"), true, ((nCustomerAge == 0) && co.GetfOperationsAllowSaleofRestrictedItemsInAssist()));
  }
  else if ( co.IsAttendantModeOn() )
  {
      ps.ButtonState( _T("SMButton1"), true, co.GetfOperationsAllowSaleofRestrictedItemsInAssist());  
  }
  else
  {
	  if (!co.GetfOperationsAllowSaleofRestrictedItems())
	  {
          ps.ButtonState( _T("SMButton1"), true, false);
	  }
	  else if (nCustomerAge == 0) //SSCOP-5820
	  {
          ps.ButtonState( _T("SMButton1"), true, true);
	  }
  }  

  //AssistMode/AssistMenu: based on option: DOBNotNeededButton
  if ( co.IsInAssistMenus() || fStateAssist ) 
  {
       ps.ButtonState( _T("SMButton2"), true, (co.fStateDOBNotNeededButton && co.GetfOperationsAllowSaleofRestrictedItemsInAssist()));
	   //SSCOADK-2075+
	  //since age has been entered, button 1 (key in birthdate) and button 2 (Date not needed) should be disabled
	  if(nCustomerAge > 0)
          ps.ButtonState( _T("SMButton2"), true, false);
	  //SSCOADK-2075-
  }
  else
  {
      if (!co.fStateDOBNotNeededButton || !co.GetfOperationsAllowSaleofRestrictedItems() || ( co.IsAttendantModeOn() && !co.GetfOperationsAllowSaleofRestrictedItemsInAssist() )) //sscob-2485
      {
          ps.ButtonState( _T("SMButton2"), true, false);
      }
      else if (nCustomerAge == 0) //SSCOADK-2075
      {
        ps.ButtonState( _T("SMButton1"), true, true);
        ps.ButtonState( _T("SMButton2"), true, true);
      }
  }
  //SSCOADK-2074-
   
  ps.ShowSMTotal(true);
  ps.ShowSMTBText(csTBMessage2Scot);	//thp
  
  // BeginCodeLocale
  int nNoTotalDigit = 4; // By definition: 2 digits for month and for days
  if (co.nLocaleNoCenturyDigit==0)
	 nNoTotalDigit += 2;
  if (co.nLocaleNoCenturyDigit==1) 
	 nNoTotalDigit += 4;  
  ps.AllowInput(nNoTotalDigit,false,false);
  ps.ButtonState( _T("SMButton3"), true, true); //SSCOP-1481


  //TAR 408485+
  if (ps.RemoteMode())
  {
      ps.Button(_T("SMButton8"), BTT_GOBACK, false);
  }
  else
  {
      ps.Button(_T("SMButton8"), TTL_STOREAUTH, false);
  }
//TAR 408485-


  // EndCodeLocale
  ps.ShowFrame();

  //Tar 229455 Show green light
  
  // (*) SR898 ====================
  DMTurnOffTriColorLight();
  
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                     DM_TRICOLORLIGHT_ON,
                     DM_TRICOLORLIGHT_NORMALREQUEST);

  return STATE_NULL;                // stay in SmRestrictedItemsBase state
}

/////////////////////////////////////////////
// PSButton1 - birth date needed
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton1(void)
{
  PSClearInput(); // SSCOP-1074
  bInApprovalNeeded = false; //TAR 279092
  RETURNSTATE(SmEnterBirthdate)
}

//SR81 begin
/////////////////////////////////////////////
// PSButton3 - "Do Not Approve" button - SR803                 
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton3(void)
{	  
	PSClearInput(); // SSCOP-1074
	bForceApprovalBeforeSuspend = true;
    
    if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETE)
    {
        //Age Intervention has progressed
        m_GenericDelayedInterventionStatus=GENERIC_DELAYED_COMPLETING_APPROVALS;  
    }
    
    //+SR803
    /* 
	lUnapprovedRestrictedItems = 1;

	CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
	csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
	trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
	ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
	trace(L7, _T("-ra.OnNormalItem"));  

	// remote approval
	CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);

	//TAR 196910 start
    //+SR803 - don't clear if going to VoidItem or Full Assist to still show ApproveAgeCheck in Remote Mode 
    //         when returning from VoidItem or Full Assist without voiding the restricted items    
    if( !(lUnapprovedRestrictedItems > 1) ) 
    {
        while (m_lAgerestrictQueuedtoRAP > 0)
        {
            trace(L7, _T("+ra.OnAgeApproval"));
            ra.OnAgeApproval();
            trace(L7, _T("-ra.OnAgeApproval"));
            m_lAgerestrictQueuedtoRAP--;
        }
        m_lAgerestrictQueuedtoRAP = 0;
    }
    */
    //-SR803

    //SSCOADK-7224 +
    if ((lRestrictedItems > 1) && !co.fOperationsImmediateInterventionForRestrictedItems)
    {
      bReturnToRestrictedItems = true;
    }
    else
    {
      bReturnToRestrictedItems = false;
    }
    //SSCOADK-7224 -
    if (co.fStateAssistMenuFromStoreModeAllowed || (co.fStateAssistMenuFromRAPAllowed && ps.RemoteMode())) //SR803 - SSCOP-681
	{
        RETURNSTATE(SmAssistMenu)
	}

    rp.m_bIsInterventionApproved = false; //report that the intervention was not approved 

    if(ps.RemoteMode())
    {
        CString csRAP;
        csRAP = _T("RAP");
        rp.SetInterventionTerminalNumber(csRAP);
    }

	RETURNSTATE(SmAssistMode)  // TAR 391593
}
//SR81 end

//+SR803 - Remove method handling "Store Authorization"
/*
/////////////////////////////////////////////
// PSButton8 - Cancel
/////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton8(void)
{
  bForceApprovalBeforeSuspend = false; //TAR 364905
  bInApprovalNeeded = false; //TAR 279092
  trace(L7, _T("Cashier selected Store Authorization button instead of approving age restricted item(s)"));
  bStoreModeGoBack = true;
  // They are cancelling - nothing entered here will be
  // saved.  Note that if they have entered valid information
  // to authorized all unapproved items, they will have already
  // left via PSEnterKey and won't have a chance to get here.
  //TAR 290433
  if (co.IsInAssistMenus())
  {
	  RETURNSTATE(SmAssistMenu)
  }
  RETURNSTATE(SmAuthorization)
}
*/
//-SR803

///////////////////////////////////////////////////////////
// PSNumericKey
///////////////////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSNumericKey(const long lKey)
{
  PSEchoInput(lKey);
  return STATE_NULL;
}

///////////////////////////////////////////////////////////
// PSButton2 - birth date not needed
///////////////////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSButton2(void)
{
  PSClearInput(); // SSCOP-1074
  // On the cashier's authority, consider all tobacco and alcohol items
  // approved now, and proceed as if a valid legal age for any restricted
  // item has been obtained.
  long lSavedlUnapprovedRestrictedItems = lUnapprovedRestrictedItems;

  lUnapprovedRestrictedItems =0;
  lRegularYellowNeedingApproval = 0;
  bInApprovalNeeded = false; //TAR 279092
  fApprovedForRestrictedItems = true;
  fAgeObtained = true;
  nCustomerAge = 99;
  // update ITEM_AUTOAPPROVAL_FLAG for age-restricted items for NGUI
  ps.RestrictedItemNotAllowed(nCustomerAge, lSavedlUnapprovedRestrictedItems);

  ps.SetContextWithKeyPressed(); //SSCOADK-1180

  if(fUnapprovedRestrictedItemFromCustomerMode)
  {
      fUnapprovedRestrictedItemFromCustomerMode = false;
      co.SetInAssistMenus(false);
  }
  
  if (m_GenericDelayedInterventionStatus == GENERIC_DELAYED_COMPLETE)
  {
      //Age Intervention has progressed
      m_GenericDelayedInterventionStatus=GENERIC_DELAYED_COMPLETING_APPROVALS;  
  }

  rp.m_bIsInterventionApproved = true;  //SR803 - report that the intervention was approved 
  
  CString csDateNotNeeded = ps.GetPSText(RA_AGE_RESTRICTED_ID, SCOT_LANGUAGE_PRIMARY);
  csDateNotNeeded +=  _T("\n") + ps.GetPSText(BTT_BIRTHDATENOTREQUIRED, SCOT_LANGUAGE_PRIMARY);
  trace(L7, _T("+ra.OnNormalItem, Message=%s"), csDateNotNeeded);
  ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + csDateNotNeeded); // Display valid birthday on RAP
  trace(L7, _T("-ra.OnNormalItem"));  
  
  // remote approval
  CString csDesc = ps.GetPSText(RA_AGE_RESTRICTED_ID);

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

  bReturnToRestrictedItems = false; //SSCOADK-7224
  RETURNSTATE(SmAuthorization)
}

//////////////////////////////////////////////
// PSClearKey
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSClearKey(void)
{
  PSClearInput();
  return STATE_NULL;
}

//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmRestrictedItemsBase::PSEnterKey(void)
{
  return VerifyBirthDate();
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmRestrictedItemsBase::PSReceiptUp(void)
{
  ps.ScrollUp(_T("SMReceiptRestricted"));
  return STATE_NULL;

}
SMStateBase  *SMSmRestrictedItemsBase::PSReceiptDown(void)
{
  ps.ScrollDown(_T("SMReceiptRestricted"));
  return STATE_NULL;

}
// NewUI
//////////////////////////////////////////////
SMStateBase *SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived(LPCTSTR sData) 
   {
   trace (L6, _T("+SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived(%s)."), sData);
   trace (L6, _T("-SMSmRestrictedItemsBase::OnRAPUnsolicitedDataReceived() -- Ignore remote scanned input"));
   return STATE_NULL;
   }
// + RFC 330050
void SMSmRestrictedItemsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRestrictedItems") );	
}
// - RFC 330050

//SSCOP-4920
SMStateBase  *SMSmRestrictedItemsBase::TimedOut(void) 
{
   //sscoadk-1859+
   //if (isRestrictedItemApprovalRequired() && 
   //    (co.IsInAssistMenus() || (!co.IsInAssistMenus() && !isAnyNonImmediateApprovalRequired() )))
   //{
   //   return STATE_NULL;
   //}

  
   if (isRestrictedItemApprovalRequired() )
   {
      return STATE_NULL;
   }
   else
   {
       trace (L6, _T("SMSmRestrictedItemsBase::TimedOut()"));
       if (co.IsInAssistMenus())
       {
           RETURNSTATE(SmAssistMenu)
       }

       //sscoadk-1859+
       co.SetInAssistMenus(false); 
    
        if (dm.fStateTakeawayBelt && ps.RemoteMode())
	    {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
	    }
        else
	    {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
	    }  	
     
        SendTBCommand(TBCMD_ASSISTMENUSET, false); 
        //sscoadk-1859-

       RETURNSTATE(SmAuthorization)
   }
}

SMStateBase *SMSmRestrictedItemsBase::OnWtIncreaseNotAllowed()
{ 
	OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR); 
	return STATE_NULL; 
}

SMStateBase *SMSmRestrictedItemsBase::OnWtDecrease()
{ 
	OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR); 
	return STATE_NULL; 
}
