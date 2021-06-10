//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmEnhancedAssistMode.CPP
//
// TITLE: Class implementation for Enhanced Assist Mode keyboard
//
//
// AUTHOR:    Robert Susanto
//
// CHANGE HISTORY:
//
// POS78133          Work Request:58657 Name:Matt Condra    Date:October 7, 2013
// POS50102          Work Request:52635 Name:Matt Condra    Date:December 6, 2012
// POS30981/POS40698 Work Request:50864 Name:Robert Susanto Date:August 15, 2012
// POS30981/POS31054 Work Request:50864 Name:Robert Susanto Date:August 1, 2012
// POS30981/POS31053 Work Request:50864 Name:Robert Susanto Date:July 31, 2012
// POS30981/POS31051 Work Request:50864 Name:Robert Susanto Date:July 18, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:July 17, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:July 16, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:July 12, 2012
// POS30981/POS36096 Work Request:50864 Name:Robert Susanto Date:June 7, 2012
// POS30981/POS35097 Work Request:50864 Name:Robert Susanto Date:June 7, 2012
// POS30981/POS31051 Work Request:50864 Name:Robert Susanto Date:May 18, 2012
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:April 23, 2012
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // MGV common includes
#include "SMSmEnhancedAssistMode.h"
#include "SMRapDataNeeded.h"

#include "TransBrokerSSF.h"

//POS31050
#include "SMInProgress.h"

#define COMP_ID ID_SM               // base state component
#define T_ID _T("SMSmEnhancedAssistMode")

#define TB_DEV_TOTAL_DETAILS 10006

IMPLEMENT_DYNCREATE(SMSmEnhancedAssistMode, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP



////////////////////////////////
SMSmEnhancedAssistMode::SMSmEnhancedAssistMode()
: SMSmAssistMode()
{
	IMPLEMENT_TIMESTAMP	

    m_bEnhancedAssistMode = false; //POS30981 rrs

    m_bDisplayKepayd = false;

    m_bAuditDone = false; // POS31053
} 


void SMSmEnhancedAssistMode::UnInitialize(void)
{
    SMSmAssistMode::UnInitialize();   

    m_pAppPSX->SetConfigProperty(_T("Key12"), _T("SmAssistMode"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("Key13"), _T("SmAssistMode"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("Key22"), _T("SmAssistMode"), UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("Key23"), _T("SmAssistMode"), UI::PROPERTYVISIBLE, VARIANT_TRUE);   
    
    // POS31053 - display the total information and hide the weight display
    m_pAppPSX->SetConfigProperty(_T("AssistTotalText"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("EnhancedAssistTotalAmount"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("AssistTotalItemText"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("AssistTotalItemAmount"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
    m_pAppPSX->SetConfigProperty(_T("CMWeight"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
}

////////////////////////////////////
SMStateBase  *SMSmEnhancedAssistMode::Initialize()
{
    trace(L0, _T("+SMSmEnhancedAssistMode::Initialize()"));    
	
    CString csOriginalAssistModeContext = g_csAssistModeStateName;
    g_csAssistModeStateName = _T("MeijerEnhancedAssistMode");

	SMStateBase *retState = SMSmAssistMode::Initialize();

    g_csAssistModeStateName = csOriginalAssistModeContext;

    // POS30981 - Go to Enhanced AssistMode for any intervention during Mobile SSCO resume rrs
    csCurContext = _T("MeijerEnhancedAssistMode");  
    ps.SetFrame( _T("MeijerEnhancedAssistMode") );
    
    //m_bEnhancedAssistMode = true;
        trace(L6, _T("Activate EnhancedAssistMode") );
    
    //SOTF 6657
    SendTBCommand(TBCMD_NOTIFY_ACS_REMOTE_ASSISTMODE_STATUS, (ps.RemoteMode() ? true : false));

    //POS31051 - clear the receipt area
    m_pAppPSX->ClearReceipt( _T("EnhancedAMList") );

    m_pAppPSX->SetConfigProperty( _T("AuditRemovedItemLabel"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );  //POS31051 hide the No Of Removed Item text area - rrs

    m_pAppPSX->SetConfigProperty(_T("CMWeight"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);  // POS31053 hide the weight display

	TBShowDisplay(true);

    //POS31051- avoid flashing of logout by hiding it and let the TBShowDisplay function controls the logout button state - rrs
    //ps.Button(_T("SMButton1"), SSF_TXT_ENHANCEDAM_LOGOUT, false);     
    COleVariant v;
	v=ps.GetPSText(SSF_TXT_ENHANCEDAM_LOGOUT);
    m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYTEXTFORMAT, v);

    //POS31054 - Set the yellow light during Mobile Transaction
    if( m_bNeedYellowLightForMobile )
    {
        dm.setTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                                        DM_TRICOLORLIGHT_ON,
                                        DM_TRICOLORLIGHT_NORMALREQUEST);
    }   
    
    return retState;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Method:		SMStateBase  *SMSmEnhancedAssistMode::TBShowDisplay
//
// Purpose:		Retrieve latest POS screen info and display it
//
// Parameters:	N/A
//
// Returns:		N/A
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SMSmEnhancedAssistMode::TBShowDisplay(bool bRefresh, bool bRepaintLastScreen)
{
   trace( L4, _T("+SMSmEnhancedAssistMode::TBShowDisplay()"));     

   //POS 35907 - don't process anything if it's waiting to exit rrs
   if( fWaitingToExit )
   {
       return;
   }

   // POS31051 - prevents flickering screen rrs
   CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmEnhancedAssistMode::TBShowDisplay"));

   CString csPosState = GetHookProperty(_T("__POSState"));        

   SMSmAssistMode::TBShowDisplay(bRefresh, bRepaintLastScreen);
   

   // force to hide the Log Out button if not in the MOBILE_ASSISTANCE dynaframe
   // POS31050 - make sure the Log Out button is enabled for the PriceRequired screen and the keypad is displayed

   //POS31050 - Use the DF_ID to identify the Price Required state as POS doesn't send the DF_DYNAKEY_LABEL when there is a back to back price reqd - rrs
   _variant_t nDFID = GetFieldValue(_T("DF_ID"));  
	
   if( csPosState.Find(_T("MOBILE_TRANSACTION_ASSISTANCE")) == -1 && nDFID.intVal != 90 )  //|| csPosState.Find(_T("PRICE_REQUIRED")) == -1 ) )
   {
       m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
   }
   else
   {
       m_pAppPSX->SetConfigProperty(_T("SMButton1"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
   }

   if( GetHookProperty(_T("HHRapVirtualKeyboardNeeded")) == _T("0") && nDFID.intVal != 90 ) 
   {
       m_pAppPSX->SetConfigProperty(_T("Key12"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("Key13"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("Key22"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("Key23"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("Key32"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
   }
   else
   {
       m_pAppPSX->SetConfigProperty(_T("Key12"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
       m_pAppPSX->SetConfigProperty(_T("Key13"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
       m_pAppPSX->SetConfigProperty(_T("Key22"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
       m_pAppPSX->SetConfigProperty(_T("Key23"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
       m_pAppPSX->SetConfigProperty(_T("Key32"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);       
   }

   //m_pAppPSX->SetTransactionVariable( _T("SMTotalAmount"), (long)9999 );

   // POS31053 - if audit is done, remove the total information and display the weight scale
   if( m_bAuditDone )
   {
       m_pAppPSX->SetConfigProperty(_T("AssistTotalText"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("EnhancedAssistTotalAmount"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("AssistTotalItemText"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       m_pAppPSX->SetConfigProperty(_T("AssistTotalItemAmount"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);

       m_pAppPSX->SetConfigProperty( _T("AuditRemovedItemLabel"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE );  //POS31053 hide the No Of Removed Item text area - rrs


       //m_pAppPSX->SetTransactionVariable( _T("SMEnhancedTotalAmount"), (long)lTotalSale );

       if( csPosState.Find(_T("WEIGHT_REQUIRED")) != -1 )
       {
           m_pAppPSX->SetConfigProperty(_T("CMWeight"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
       }
       else
       {
           m_pAppPSX->SetConfigProperty(_T("CMWeight"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       }
   }
   else
   {
       CString csNumOfItem = GetHookProperty(_T("number_of_sold_item"));
       if(csNumOfItem != _T("") )
       {
           long lNumOfItem = _ttol(csNumOfItem);
           m_pAppPSX->SetTransactionVariable( _T("SMTotalItemAmount"), (long)lNumOfItem );

	       // POS31050 - use subtotal information from acs_tb_int=1543 instead of from total context
           CString csRunningTotal = GetHookProperty(_T("transaction_subtotal_mobile_item"));
           long lSubTotal = _ttol(csRunningTotal);
           
           m_pAppPSX->SetTransactionVariable( _T("SMEnhancedTotalAmount"), (long)lSubTotal );
        
           /*if( lTotalSale == 0 )
           {
               CString csRunningTotal = GetHookProperty(_T("transaction_subtotal_mobile_item"));
               long lSubTotal = _ttol(csRunningTotal);
               lTotalSale = lSubTotal;  
           
               ps.Total(lTotalSale);
           
               ps.ShowSMTotal(true);
           }*/
       }
       else
       {
           m_pAppPSX->SetTransactionVariable( _T("SMTotalItemAmount"), (long)0 );
       }  
   }

  
    
   // only displays the EnhancedAMList if there is no information from POS in DF_BuildRainCk
   CString csVoidPrepaidCardsProp = _T("VoidPrepaidCards");
   CString csIsVoidPrepaidCards;
   SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csVoidPrepaidCardsProp, &csIsVoidPrepaidCards);

   PSXRC rc;
   bool bRainCkList = false;
   if (csIsVoidPrepaidCards == _T("1")) 
   {
       bRainCkList = true;
   }

   if ( bRainCkList ) 
   {
       rc = m_pAppPSX->ClearReceipt( _T("EnhancedAMList") );
       
       // Make ACSList visible here
       m_pAppPSX->SetConfigProperty( _T("EnhancedAMList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );
       
       CString csPrepaidCardProp = _T("PrepaidCardList");
       CString csPrepaidCard;
       SendTBCommand(TBCMD_GET_HOOKPROPERTY, &csPrepaidCardProp, &csPrepaidCard);
       
       if( csPosState.Find(_T("MOB_SHOP_SCAN_AND_COLLECT")) != -1 )
       {
           COleVariant vValue;
           bool bIsScanCollecttemExist = true;
           CString csScanCollectItemDesc, csReceiptID;
           int iToken=0;
           int iReceiptID=0;
                
           // parse the tax list item from the cstring buffer
           // the cstring would have the format of "Tax1 Off   |Tax2 Off    |Tax3 Off"
           while( bIsScanCollecttemExist )
           {
               iToken = csPrepaidCard.Find(_T("\t"));
               if( iToken > -1 )
               {
                   // create the tax list description
                   csScanCollectItemDesc = csPrepaidCard.Left(iToken);
                   csPrepaidCard = csPrepaidCard.Mid(iToken+1);

                   // create the receipt item entry id
                   csReceiptID.Format(_T("item%d"), iReceiptID);
                   vValue = csScanCollectItemDesc;                   

                   rc = m_pAppPSX->CreateReceiptItem( _T("EnhancedAMList"), csReceiptID );

                   if( iReceiptID%2 == 0)
                   {
                       rc = m_pAppPSX->SetReceiptItemVariable( _T("EnhancedAMList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                   }
                   else
                   {
                       rc = m_pAppPSX->SetReceiptItemVariable( _T("EnhancedAMList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                       rc = m_pAppPSX->SetReceiptItemVariable(_T("EnhancedAMList"), csReceiptID, _T("ITEM_LINKED_FMT"), VARIANT_TRUE);
                   }

                   iReceiptID++;
               }
               else
               {
                   csReceiptID.Format(_T("item%d"), iReceiptID);
                   vValue = csPrepaidCard;
                    
                   rc = m_pAppPSX->CreateReceiptItem( _T("EnhancedAMList"), csReceiptID );
                   rc = m_pAppPSX->SetReceiptItemVariable( _T("EnhancedAMList"), csReceiptID, _T("ITEM_DESCRIPTION"), vValue );
                   rc = m_pAppPSX->SetReceiptItemVariable(_T("EnhancedAMList"), csReceiptID, _T("ITEM_LINKED_FMT"), VARIANT_TRUE);
                   
                   bIsScanCollecttemExist = false;
               }
          }
          // update the receipt control
          rc = m_pAppPSX->UpdateReceiptControls( _T("EnhancedAMList") );
       }
       else
       {
           rc = m_pAppPSX->CreateReceiptItem( _T("EnhancedAMList"), _T("item1") );
           if ( PSX_SUCCESS != rc )
           {
                // kaboom!
           }
		   COleVariant vValue;
		   vValue = csPrepaidCard;

           rc = m_pAppPSX->SetReceiptItemVariable( _T("EnhancedAMList"), _T("item1"), _T("ITEM_DESCRIPTION"), vValue );
           // create more
           rc = m_pAppPSX->UpdateReceiptControls( _T("EnhancedAMList") );
       }		
   }
      
   m_pAppPSX->SetConfigProperty( _T("EnhancedAMList"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );    
   
   CString csItemDesc = GetHookProperty(_T("tb_char"));
   if( (csItemDesc.GetLength() > 1) &&
       (csPosState.Find(_T("MOBILE_AUDIT")) == -1) &&  //POS31051 - Don't modify the LTT2 in Mobile Audit screen - rrs
       (GetHookProperty(_T("__POSState")).Find(_T("HIGHVALUE_PROMO_CHOICE")) == -1) //POS50102: Don't modify the LTT2 when high value mPerks promo choice is active which can occur when exiting Enhanced AM.
     )                                                                              //          Need to use GetHookProperty here due to timing issue with ACSHookManager thread.
   {
       if( GetDisplayText(_T("LTT2")) != _T("") )
       {
           csItemDesc = GetDisplayText(_T("LTT2")) + _T(" - ") + csItemDesc + _T(" - ") + GetHookProperty(_T("item_code"));
       }
       else
       {
           csItemDesc = csItemDesc + _T(" - ") + GetHookProperty(_T("item_code"));
       }

       ps.SetLeadthruText(GetDisplayText(_T("LTT1")), csItemDesc);
   }
   
   //POS31050 - display the item code and description on the CHANGE UNIT PRICE, CHANGE_PACKAGE_QTY and CHANGE PACKAGE PRICE rrs
   if( csPosState.Find(_T("PRICE_VISUAL_VALIDATE")) != -1 || csPosState.Find( _T("CHANGE_UNIT_PRICE") ) != -1 || csPosState.Find( _T("CHANGE_PACKAGE_PRICE") ) != -1 || csPosState.Find( _T("CHANGE_PACKAGE_QTY") ) != -1 )
   {
       CString csItemPriceVerifyUPC = GetHookProperty(_T("PriceVerifyItemUPC"));
       CString csTitle2 = GetDisplayText(_T("Title2")) + _T(" - ") + csItemPriceVerifyUPC;
       ps.SetTitle(GetDisplayText(_T("Title1")), _T(""));
       ps.SetLeadthruText(GetDisplayText(_T("LTT1")), csTitle2);
   }
   //POS31051 - Use ereceipt for Audit information - rrs
   else if( csPosState.Find(_T("MOBILE_AUDIT")) != -1 ) 
   {       
       rc = m_pAppPSX->ClearReceipt( _T("EnhancedAMList") );
       
       rc = m_pAppPSX->CreateReceiptItem( _T("EnhancedAMList"), _T("item1") );
       
       if ( PSX_SUCCESS != rc )
       {
            // kaboom!
       }
	   COleVariant vValue;
	   vValue = GetDisplayText(_T("LTT2"));

       rc = m_pAppPSX->SetReceiptItemVariable( _T("EnhancedAMList"), _T("item1"), _T("ITEM_DESCRIPTION"), vValue );

       CString csItem2( ps.GetPSText(SSF_TXT_ENHANCEDAM_TOTALREMOVEDITEM) ); //_T("No Of Removed Items:"));
       csItem2 += _T("  ") + GetHookProperty(_T("tb_char"));  // POS31050 - provide the space after the text
       vValue = csItem2;
       m_pAppPSX->SetConfigProperty( _T("AuditRemovedItemLabel"), csCurContext, UI::PROPERTYVISIBLE, VARIANT_TRUE );  //POS31051 hide the No Of Removed Item text area - rrs
       rc = m_pAppPSX->SetConfigProperty(_T("AuditRemovedItemLabel"), csCurContext, UI::PROPERTYTEXTFORMAT, vValue);

       rc = m_pAppPSX->UpdateReceiptControls( _T("EnhancedAMList") );   
       
       /*CString csTitle2 = GetDisplayText(_T("Title2"));
       if( csTitle2.Find(_T("Please")) != -1 )
       {
           ps.SetTitle(GetDisplayText(_T("Title1")), _T(""));
           ps.SetLeadthruText(GetDisplayText(_T("LTT1")), csTitle2);       
       }
       else
       {
           ps.SetLeadthruText(GetDisplayText(_T("LTT1")), _T(""));       
       }*/
       ps.SetLeadthruText(GetDisplayText(_T("LTT1")), _T(""));       
   }
   //e POS31051 - Use ereceipt for Audit information - rrs 
   

   ps.ShowFrame();

   trace( L4, _T("-SMSmEnhancedAssistMode::TBShowDisplay()"));
}

void SMSmEnhancedAssistMode::SetDynakeys()
{
    trace(L0, _T("SMSmEnhancedAssistMode::SetDynakeys") );

    // POS31051 - prevents flickering screen rrs
    CPSXRedrawLock csRedrawLock(_T("Display"), _T("SMSmEnhancedAssistMode::SetDynakeys"));

    SMSmAssistMode::SetDynakeys();

    int nButtonCount = 8;
    int nKeyOffset = 0;
    COleVariant vIndex = (long)0;

    CString csLabel, csEnabled, csText, csGrayed, csIcon;
    COleVariant vReturn;

    for (int nKey=1; nKey <= nButtonCount; nKey++)
	{
		nKeyOffset=nKey-1;  
		
		csLabel.Format(_T("DK%d_Label"),nKey);
		csEnabled.Format(_T("DK%d_Enabled"), nKey);
		csGrayed.Format(_T("DK%d_Grayed"), nKey);
		csIcon.Format(_T("DK%d_Icon"), nKey);
		
		csText = GetDisplayText(csLabel);  //Dynakey1
		if (csText == _T(""))
		{
			csLabel.Format(_T("DK%d_Label_Alt"), nKey);
			csText = GetDisplayText(csLabel);
			if (csText == _T("")) 
            {
				csText = _T(" ");           
            
                trace(L7, _T("Remove Softkey button%d"), nKeyOffset);
                vIndex = (long)nKeyOffset;
                m_pAppPSX->SendCommand(_T("Key24"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, VARIANT_FALSE );                                        
            }
		}
    }

}

SMStateBase  *SMSmEnhancedAssistMode::PSAssistKey(const long nKey)
{
    trace(L0,_T("+Enhanced Assist Mode %d"),nKey); 

    //clear the rainck buffer when the softkey is presses
    SetHookProperty(_T("PrepaidCardList"), _T(""));  
    
    //POS31053 - make sure the tb_char is empty after did something with it to preven the item description displayed on the next prompt
    SetHookProperty(_T("tb_char"), _T(""));  

    // POS31053
    CString csPOSString = TBGetKeyPOSString(nKey);
    if( csPOSString == _T("{F1}") && GetHookProperty(_T("__POSState")).Find(_T("MOBILE_AUDIT")) != -1 )
    {
        m_bAuditDone = true;
        trace(L7, _T("Audit is done"));
    }

    // POS78133: Save POS state before sending key press to sales app to prevent a race condition when POS state is
    // accessed after the send of the key press.
    CString csPOSState = GetHookProperty(_T("__POSState"));

    // Send key press to sales app.
    SMStateBase *returnState = SMSmAssistMode::PSAssistKey(nKey);

    if( (csPOSString == _T("{F2}")) && (csPOSState.Find(_T("MOB_SHOP_ORDER_NOT_FOUND_AT_NONMOBILEPAYSTATION")) != -1) )
    {
        // POS78133: Exit enhanced assist mode when 'No' is tapped on the "Unable to retrieve mobile order.Do you wish to cancel this transaction?" message screen.
        delete getRealStatePointer(returnState);
        return SMSmAssistModeBase::PSButton1();        
    }

    return returnState;
}

SMStateBase  *SMSmEnhancedAssistMode::DMScanner()// scanned item
{
    trace(L0,_T("+EnhancedAssistMode::DMScanner")); 

    //clear the rainck buffer when the softkey is presses
    SetHookProperty(_T("PrepaidCardList"), _T(""));

    //POS31053 - make sure the tb_char is empty after did something with it to preven the item description displayed on the next prompt
    SetHookProperty(_T("tb_char"), _T(""));  

    return SMSmAssistMode::DMScanner();
}

SMStateBase *SMSmEnhancedAssistMode::TBParse(MessageElement *me)
{
    trace(L0,_T("+EnhancedAssistMode TBParse")); 

    SMStateBase *returnState = SMSmAssistMode::TBParse(me);	

    //POS36096 - stop the belt during the EnhancedAssistMode
    /*if( m_TBLastState == TB_ITEMSOLD )
    {        
        trace(L6, _T("TB_ITEMSOLD during enhancedAM, Stop the belt"));
        if( dm.fStateTakeawayBelt )
        {
            DMTakeawayBeltNotify(tabEventAssistModeStop);            
        }
    }*/
    //e POS36906

    CString csPosState = GetHookProperty(_T("__POSState"));

    // POS35907 - Smooth exit from enhancedassistmode at the end of the mobile intervention, it needs to exit immediately - rrs
    trace(L6, _T("POSState (%s)"), csPosState);
    
    // POS40698 - perform the auto exit when POS is at PAYMENTMENU also rrs
    if( (csPosState.Find(_T("RESUME_READY")) != -1) ||
        (csPosState.Find(_T("PAYMENTMENU_DK1")) != -1) || 
        (csPosState.Find(_T("HIGHVALUE_PROMO_CHOICE")) != -1) // POS50102: Exit Enhanced AM when high value choice is active.
      )
    {
        SetHookProperty(_T("VoidPrepaidCards"), _T("0"));
        SetHookProperty(_T("PropMobShopOrderPrompt"), _T("0"));
        delete getRealStatePointer( returnState);
        return SMSmAssistModeBase::PSButton1();        
        //return TimedOut();
    }

    if( lTotalSale == 0 )
    {
        CString csRunningTotal = GetHookProperty(_T("transaction_subtotal_mobile_item"));
        long lSubTotal = _ttol(csRunningTotal);
        lTotalSale = lSubTotal; 
        ps.Total(lTotalSale);

        ps.ShowSMTotal(true);

		// POS31050 - use subtotal information from acs_tb_int=1543 instead of from total context
        m_pAppPSX->SetTransactionVariable( _T("SMEnhancedTotalAmount"), (long)lSubTotal );
    }
    
    return returnState;
}

SMStateBase* SMSmEnhancedAssistMode::PSButton1()
{
    trace(L0,_T("+EnhancedAssistMode::PSButton1 for Log Out")); 

    SMStateBase *returnState = SMSmAssistMode::PSButton1();

    //if( GetHookProperty( _T("__POSState") ) == _T("MOBILE_TRANSACTION_ASSISTANCE") )
    if( GetHookProperty(_T("PropMobShopOrderPrompt")) == _T("1") )
    {
        delete getRealStatePointer( returnState);
        CREATE_AND_DISPENSE(RAPDataNeeded)(ps.GetPSText(SSF_TXT_ENHANCEDAM_MOBILE_ASSISTANCE, SCOT_LANGUAGE_PRIMARY), DATANEEDED_ASSISTMODE, _T(""), false);        
    }

    return returnState;

    
}

