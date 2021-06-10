//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAndRemoveItemBase.CPP
//
// TITLE: Class implementation for store mode void and remove voided item state
//
//
// AUTHOR:    Virgilio Esquilla, Jr.
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // MGV common includes
#include "SMState.h"                // MGV base state
#include "SMSmAuthorization.h"
#endif //_CPPUNIT

#include "SMSmVoidAndRemoveItem.h"  // MGV remove voided item state

#ifdef _CPPUNIT
#include "FakeSMStateBase.h"
#endif //CPPUNIT

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SMSmVoidAndRemoveItemBase")


IMPLEMENT_DYNCREATE(SMSmVoidAndRemoveItemBase, CObject)// MFC Runtime class/object information



//IMPLEMENT_STATE(SMSmVoidAndRemoveItemBase)


DEFINE_TIMESTAMP

//////////////////////////
SMSmVoidAndRemoveItemBase::SMSmVoidAndRemoveItemBase()
{

  IMPLEMENT_TIMESTAMP


}

/////////////////////////////////////
SMStateBase  *SMSmVoidAndRemoveItemBase::Initialize(void)
{

  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmVoidAndRemoveItem") ); 

  lUnapprovedRestrictedItems = 0;
  bInApprovalNeeded = false;
  lRegularYellowNeedingApproval = 0;
  bStoreModeApprovals = false; 
  SAClearSecurityCondition();

#ifndef _CPPUNIT
          trace(L7, _T("+ra.OnAgeApproval"));
	      ra.OnAgeApproval();
	      trace(L7, _T("-ra.OnAgeApproval"));
            
      //(+)SSCOP-4153
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
       }
      //(-)SSCOP-4153

      //void the restricted item
  
      CString csDesc,csCode;
      long lQty,lWgt,lPrc,lUPrc,lTcd, lBarcodeType;
      if (ps.GetItem(csDesc,csCode,lQty,lWgt,lPrc,lUPrc,lTcd,lBarcodeType, _T("CMSMReceipt"), _T("SMReceiptRestricted"))>0)// get selected item info MGG3
      {
          csItemSent = csCode;					  // save the item code
          csDesc     = csDesc;					  // save the item desc
          lVoidQty   = lQty;					  // save the qty
          lVoidWgt   = lWgt;					  // save the weight
          lVoidTcd   = lTcd;					  // save the tare for later void
          lVoidPrc	 = lPrc;					  // save the price for later void
	      lVoidUPrc  = lUPrc;					  // save the unit price for later void
	      lVoidBarcodeType = lBarcodeType;
      }

    TBItemSale(true,lVoidTcd,false); 

#endif //_CCPUNIT


  // show the context which shows the message to remind the operator to remove the cancelled item
  ps.SetFrame(_T("SmVoidAndRemoveItem"));
  ps.ShowAmountDueAmountPaid(lBalanceDue,lTotalPaid);

  ps.ShowFrame();

  //sscoadk-321
  ps.ButtonState(_T("SMButton1"), false, true);

  return STATE_NULL;    // stay in SMSmVoidAndRemoveItem state    
}
#ifndef _CPPUNIT
////////////////////////////////////
SMStateBase  *SMSmVoidAndRemoveItemBase::PSButton1(void) // OK button
{

    if(fUnapprovedRestrictedItemFromCustomerMode)
    {
        fUnapprovedRestrictedItemFromCustomerMode = false;
        co.SetInAssistMenus(false);
    }

    bool bRemoteMode = ps.RemoteMode();

    if(bSecurityTrustLevelAlert)
    {
         TurnOnYellowTriColorLight();
    }
    else if(bRemoteMode)
    {
        //DMTurnOffTriColorLight();

        DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                           DM_TRICOLORLIGHT_ON,
                           DM_TRICOLORLIGHT_NORMALREQUEST);
    }

    if(isAnyApprovalRequired(false))
    {
        if(fHasEnteredFinishState)
        {
            setAnchorState(BES_FINISH);
        }

        if(!bRemoteMode)
        {
            RETURNSTATE(SmAuthorization)
        }
    }

    bFromAssistMenu = true; //SSCOADK-6286 additions to SSCOB-18178 
	
	//(+SSCOADK-2461. adding the remvoved code)
    if(!co.IsInAssistMenus())
    {
	    if (dm.fStateTakeawayBelt && ps.RemoteMode())
	    {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRemoteAssistMode") );
	    }
        else
	    {
            TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmAssistMode") );
	    }
    }
	//(-SSCOADK-2461)
	
    if (getAnchorState() == BES_ENTERCOUPONS || getAnchorState() == BES_FINISH)
	{
		return createAnchorState();
	}

    SendTBCommand(TBCMD_ASSISTMENUSET, false); // SSCOP-3342

    //SSCOADK-885+

    if(co.fOperationsImmediateInterventionForRestrictedItems)
    {
        return setAndCreateAnchorState(BES_SCANANDBAG);
    }
    else
    {
        if(fHasEnteredFinishState)
        {
            return setAndCreateAnchorState(BES_FINISH);
        }
        else
        {
            return setAndCreateAnchorState(BES_SCANANDBAG);
        }
    }
    //SSCOADK-885-
}

void SMSmVoidAndRemoveItemBase::UnInitialize()
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmVoidAndRemoveItem") );
}
#endif //_CPPUNIT