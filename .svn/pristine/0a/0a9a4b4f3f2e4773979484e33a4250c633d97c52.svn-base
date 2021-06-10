//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmVoidAgeRestrictedItem.cpp
//
// TITLE: Implementation for SOTF 6226
// DATE:  11/11/09
//
// AUTHOR:    Shawn Marks
// 
//POS216842 Work Request:68941 Name:Veerasekhara D, Date: February 8, 2016
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "TransBrokerSSF.h"
#include "SMSmVoidAgeRestrictedItem.h"
#include "SMSmAssistMenu.h"  
#include "SMSmAuthorization.h" 
#include "SMSmEnterBirthdate.h"



#define COMP_ID ID_SM               // base state component
#define T_ID _T("SMSmVoidAgeRestrictedItem")

IMPLEMENT_DYNCREATE(SMSmVoidAgeRestrictedItem, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP



////////////////////////////////
SMSmVoidAgeRestrictedItem::SMSmVoidAgeRestrictedItem()
:m_csVoidCurrentItemKey(_T("")),
m_csReceiptSrc(_T("SMReceiptVoidRestrictedItem")),
m_csReceiptRestrictedSrc(_T("SMReceiptRestricted"))
{
	IMPLEMENT_TIMESTAMP

	m_csScreenContext = _T("SmVoidAgeRestrictedItem");
	ps.SetFrame(m_csScreenContext); 
		

} 


void SMSmVoidAgeRestrictedItem::UnInitialize(void)
{
	
   SendTBCommand(TBCMD_EXIT_OPERATOR_VOID_RESTRICTED_ITEM, NULL); 
    
}

////////////////////////////////////
SMStateBase  *SMSmVoidAgeRestrictedItem::Initialize()
{
    trace(L0, _T("+SMSmVoidAgeRestrictedItem::Initialize()")); 
    fStateAssist = true;
    nTimeOut =0;

    SendTBCommand(TBCMD_ENTER_OPERATOR_VOID_RESTRICTED_ITEM);
       

    CString csOptionName(_T("VoidCurrentItem"));

    SendTBCommand(TBCMD_GET_OPTIONS, &csOptionName, &m_csVoidCurrentItemKey);
    m_csVoidCurrentItemKey = m_csVoidCurrentItemKey;

    BuildEReceipt();

    ps.ShowFrame();

	//fix for POS216842 
    CString csLastEntryID = SMState::GetTBProperty(_T("LastEntryID"));

    CString csVoidRestrictedItemEntryID = ps.GetSelection(m_csReceiptSrc); 
    CString csCMReciptEntryID = ps.GetSelection(_T("CMReceipt"));

    if((csVoidRestrictedItemEntryID == csLastEntryID) && (csCMReciptEntryID != csVoidRestrictedItemEntryID))
    {
        SMState::SetTBProperty(_T("LastEntryID"),csCMReciptEntryID);  
    }//fix for POS216842 
    
	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));
   

    if(m_bReEnterDOB)
    {
         m_bReEnterDOB = false;
         SendTBCommand(TBCMD_SET_DOB_ENTERED_STATUS, 3);
    }

    SendTBCommand(TBCMD_SEND_AGE_VERIFY_DATA);

    ps.ButtonState(_T("SMButton1"), true, true); 
   
    ps.ButtonState(_T("SMButton2"), true, true);
    return STATE_NULL;
    
}


void SMSmVoidAgeRestrictedItem::BuildEReceipt(void)
{
    CStringArray csItemEntryIdList, csParentItemEntryIdList;
    CString csItemEntryId, csParentItemId;

    m_pAppPSX->ClearReceipt(m_csReceiptSrc);

    m_pAppPSX->FindReceiptItems(m_csReceiptRestrictedSrc, csItemEntryIdList);
    long lTotalAgeItems = csItemEntryIdList.GetSize();

    for (int k=0; k < lTotalAgeItems; k++)
    {
      csItemEntryId = csItemEntryIdList.GetAt(k);
      m_pAppPSX->GetReceiptItemParent( m_csReceiptRestrictedSrc, csItemEntryId, csParentItemId );
      
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
    long lTotalAgeRestrictedItems = 0; 
    while (i >= 0)
    {
        vValue.Clear();
        csItemEntryId = csParentItemEntryIdList.GetAt(i);
        m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
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
       

        if((lAge > 0) && (!bItemVoided))
        {
            //SOTF 6968
            lTotalAgeRestrictedItems++;
        }

        if ((lAge > nCustomerAge)&&
            (!bItemVoided) && 
            (lAge != -1) &&
            (lAge != 0) &&
            (lAge != -2))    
        {
            m_pAppPSX->CreateReceiptItem(m_csReceiptSrc, csItemEntryId);       

            vValue.Clear();         
	        m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
            m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);


	        vValue.Clear();
	        m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
            m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);

	        vValue.Clear();
            m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
            m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
            
  
	        vValue.Clear();
	        m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_PRICE"), vValue);
            m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_PRICE"), vValue);
	        

            vValue.Clear();            
	        m_pAppPSX->GetReceiptItemVariable(m_csReceiptRestrictedSrc, csItemEntryId, _T("ITEM_QUANTITY"), vValue);
            m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_QUANTITY"), vValue); 
            

            vValue.Clear();
            vValue = VARIANT_TRUE;
	        m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
 

            vValue = VARIANT_TRUE;
		    m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED"), vValue);
		
            vValue.Clear();
            vValue = (long)lAge;
			m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);

            
            AddAgeDescription(csItemEntryId, lAge);
            
            
        }

        i--;
    }

    //SOTF 6968
    if(lTotalAgeRestrictedItems > 12)
    {
        m_pAppPSX->SetConfigProperty( _T("SMReceiptScrollUp"), _T("SmVoidAgeRestrictedItem"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pAppPSX->SetConfigProperty( _T("SMReceiptScrollDown"), _T("SmVoidAgeRestrictedItem"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    }else
    {
        m_pAppPSX->SetConfigProperty( _T("SMReceiptScrollUp"), _T("SmVoidAgeRestrictedItem"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pAppPSX->SetConfigProperty( _T("SMReceiptScrollDown"), _T("SmVoidAgeRestrictedItem"), UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    m_pAppPSX->UpdateReceiptControls(m_csReceiptSrc);
  
    

}

//Reenter DOB or Return to shopping
SMStateBase  *SMSmVoidAgeRestrictedItem::PSButton1(void)
{
	trace(L0,_T("+SMSmVoidAgeRestrictedItem::PSButton1"));
    fStateAssist = false;
  
    //operator press re-enter DOB
    //set a flag to denote that age was entered and now re-entered
    if(isRestrictedItemApprovalRequired())
    {
       
        m_bReEnterDOB = true;
        if ((co.IsInAssistMenus()) || (ps.RemoteMode()))
        {
            RETURNSTATE(SmEnterBirthdate)
        }

    }else
    {
       
        ra.OnNoRAPDataNeeded();
    }



    if ((co.IsInAssistMenus()) || (ps.RemoteMode()))
    {
      RETURNSTATE(SmAssistMenu)
    }

    RETURNSTATE(SmAuthorization)	
	          
}

//dynakey F3 softkey void item
SMStateBase  *SMSmVoidAgeRestrictedItem::PSButton2(void)
{
	trace(L0,_T("+SMSmVoidAgeRestrictedItem::PSButton2"));
    SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));

    ps.ButtonState(_T("SMButton2"), false, true);    

    nTimeOut = 30;
    SendTBCommand(TBCMD_SENDKEYEDDATA, m_csVoidCurrentItemKey);
    

	return STATE_NULL;          
}


//return to shopping
SMStateBase  *SMSmVoidAgeRestrictedItem::PSButton8(void)
{
    trace(L0,_T("+SMSmVoidAgeRestrictedItem::PSButton8"));
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
    
    if ((co.IsInAssistMenus()) || (ps.RemoteMode()))
    {
      RETURNSTATE(SmAssistMenu)
    }

    
    RETURNSTATE(SmAuthorization)
           
}

/////////////////////////////////////////////
SMStateBase *SMSmVoidAgeRestrictedItem::PSReceiptDown(void)
{ 

	ps.ScrollDown(_T("SMReceiptVoidRestrictedItem"));

	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));
	
	return STATE_NULL;
}

/////////////////////////////////////////////
SMStateBase *SMSmVoidAgeRestrictedItem::PSReceiptUp(void)
{

	ps.ScrollUp(_T("SMReceiptVoidRestrictedItem"));

	SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));
	return STATE_NULL;
}

  
void SMSmVoidAgeRestrictedItem::AddAgeDescription(CString csID, long lAge)
{

    CString csDesc, csItemEntryId, csMsgItemEntryId, csMsgItemEntryRapId, csDescPrimary;
    csDesc.Format(ps.GetPSText(SSF_AGE_REQUIRED, ps.m_languageUsed), lAge); 

    //add spaces to make the line up the "item not bagged text"
    csDesc =_T("  ") + csDesc;


    csItemEntryId =  csID;
    csMsgItemEntryId = csItemEntryId + _T("AgeRequired");

    COleVariant	vValue;
    vValue= csDesc;
    m_pAppPSX->CreateReceiptItem(m_csReceiptSrc, csMsgItemEntryId, csItemEntryId);        
    m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csMsgItemEntryId, _T("ITEM_MSG"), vValue);



}


SMStateBase *SMSmVoidAgeRestrictedItem::TBItemDetails(void)
{
    nTimeOut = 0;

    

    SMStateBase *pNextState = SMState::TBItemDetails();

    //if there are no more items to void then turn of the btn
    if(!isRestrictedItemApprovalRequired())
    {
        ps.ButtonState(_T("SMButton1"), false, true); 
    }else
    {
        ps.ButtonState(_T("SMButton2"), true, true);
    }

    
    if (io.fVoid)
    {
        COleVariant vValue;

        CString csItemEntryId = io.d.csItemID;

        vValue = VARIANT_TRUE;
	    m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue, true);

        vValue = VARIANT_FALSE;
	    m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);

        vValue = (long)-1;
		m_pAppPSX->SetReceiptItemVariable(m_csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);


        m_pAppPSX->UpdateReceiptControls(m_csReceiptSrc); 
    }

    delete getRealStatePointer( pNextState); 

    SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));
    return STATE_NULL;

}


SMStateBase *SMSmVoidAgeRestrictedItem::TimedOut(void)
{
    nTimeOut = 0;
    
    if(isRestrictedItemApprovalRequired())
    {
        ps.ButtonState(_T("SMButton2"), true, true);
    }
    SendTBCommand(TBCMD_SETSELECTEDITEM, ps.GetSelection(m_csReceiptSrc));
    return STATE_NULL;
}