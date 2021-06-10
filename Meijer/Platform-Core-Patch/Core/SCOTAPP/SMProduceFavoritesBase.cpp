//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMLookUpItemBase.cpp
//
// TITLE: Class implementation for Select Item state
//
//
// AUTHOR:    Ehrhardt, Dale
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state
#include "SMProduceFavorites.h"       // This state
#include "SMEnterQuantity.h"
#include "PSProceduresbase.h"
#include "SMBagAndEAS.h"
#include "SMVoidItem.h"
#include "SMLookUpItem.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecMisMatchWeight.h"


#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ProduceFavorites")

IMPLEMENT_DYNCREATE(SMProduceFavoritesBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ProduceFavorites)

DEFINE_TIMESTAMP

static bool bKeyInCodeButtonPressed = false;
////////////////////////////
SMProduceFavoritesBase::SMProduceFavoritesBase()
{
  IMPLEMENT_TIMESTAMP
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
}

///////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::Initialize(void)
{
	if(fInMultiSelectPickList)
	{
		bKeyInCodeButtonPressed = false;
		if(ps.GetPicklistItemsIncludeVoidItems() == 0)
		{
			g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
			TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("MultiPicklist") );
		}
	}
	else
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ProduceFavorites") );	// RFC 330050
	}
	// Update RAP window with SCOT state //
	UpdateStateName( ps.GetPSText( LTT_PRODUCEFAVORITES, SCOT_LANGUAGE_PRIMARY ) );
	BEST prevAnchorState = getAnchorState();
	setAnchorState( BES_PRODUCEFAVORITES );

	CString csProduceFavoritesContext;
	if ( co.fOperationsAllowMultiPick )
		csProduceFavoritesContext= _T("MultiSelectProduceFavorites") ;
	else
		csProduceFavoritesContext = _T("ProduceFavorites") ;

	if ( !co.fOperationsAllowPickListSearchButton )
    {
		m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), csProduceFavoritesContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	   COleVariant vReturn;
	   m_pAppPSX->GetConfigProperty( _T("CMButton2Lg"), csProduceFavoritesContext, UI::PROPERTYPOSITION, vReturn);
		m_pAppPSX->SetConfigProperty(_T("CMButton1Global"), csProduceFavoritesContext, UI::PROPERTYPOSITION, vReturn);
    }

	ps.SetCMFrame(csProduceFavoritesContext);
	ps.m_bFirstTimeNextButtonEnabled = TRUE;

	if(co.fOperationsAllowPickListSearchButton && ps.GetFoundPickListItemsFromSearch())
   {
      ps.SetCategoryListText(_T("TabSelected"));

      COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );
      m_pAppPSX->GetControlProperty( _T("TabSelected"), UI::PROPERTYBUTTONCOUNT, vIndex );
      long nButtonCount = vIndex.lVal;
      if( nButtonCount > 0 )
      {
         for( long i = 0; i < nButtonCount; i++ )
         {
            vIndex = i;
            vParam = (long) UI::Normal;
            m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
         }
      }  
   }
   else
   {
      ps.SetCategoryListText(_T("TabSelected"));

	   COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );
	   m_pAppPSX->GetControlProperty( _T("TabSelected"), UI::PROPERTYBUTTONCOUNT, vIndex );
	   long nButtonCount = vIndex.lVal;
	   CString csTemp( _T("") );
	   long nSelectIndex = 0;
	   bool bSubCategory = false;

	   if( nButtonCount > 0 )
	   {
		   for( long i = 0; i < nButtonCount; i++ )
		   {
			   vIndex = i;

			   if( PSX_SUCCESS != m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex ) )
			   {
				   continue;
			   }

			   if( VT_BSTR == vReturn.vt )
			   {
				   csTemp = vReturn.bstrVal;
			   }
			   else
			   {
				   continue;
			   }
			   
			   // if favorites option is set then show the Favorites tab //
			   if( co.fStateProduceFavorites && ps.ItemsInFavoritesList() > 0 )
			   {
    			   if( csTemp == TAB_FAV )
				   {
					   vParam = VARIANT_TRUE;
					   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, vParam );
					   nSelectIndex = i;
					   break;
				   }
			   }
			   else
			   {
				   // Otherwise hide the Favorites tab //
				   if( csTemp == TAB_FAV )
				   {
					   vParam = VARIANT_FALSE;
					   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, vParam );
				   }
			   }
		       //Display selected category from quick pick item screen
			   if ( (co.fOperationsDisplayQuickPickItems && csTemp == ps.GetLastProduceLookupList()) && 
				    (csItemSent == ps.GetLastProduceLookupList() || csItemSent.Find(_T("SUBCATEGORY")) != -1) && 
				    (prevAnchorState == BES_SCANANDBAG) )
			   {
				   nSelectIndex = i;
				   break;
			   }	
		   }
		   
		   if ( csItemSent.Find(_T("SUBCATEGORY")) != -1 )
		   {
			   vIndex = nSelectIndex;
			   vParam = (long) UI::Pushed;
			   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
			   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );
			   
			   int nLen = csItemSent.GetLength();
			   csItemSent = csItemSent.Right( nLen - _tcslen(_T("SUBCATEGORY")) );
			   ps.DisplaySelectedSubItem(csItemSent);
		   }
		   else if ( !co.fOperationsAllowMultiPick ||
			    (co.fOperationsAllowMultiPick && prevAnchorState == BES_SCANANDBAG) )
		   {
			   vIndex = nSelectIndex;
			   vParam = (long) UI::Pushed;
			   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
			   m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );
			   
			   if( VT_BSTR == vReturn.vt )
			   {
               ps.DisplaySelectedItem(vReturn.bstrVal);
			   }
			   DMSayPhrase( SELECTFROMPOPULAR ); // TAR [396117]: ji/40g339 - picklist screen is playing wrong audio.
		   }
		   csItemSent = _T("");
	   }
   }
  
	// allow the EchoInput() func to accept up to max digits //
	ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );

	SAWLDBScannerDisable(); // Disable the scanner //

	if ( co.fOperationsAllowMultiPick )
	{
		CString csLookupItems;
		long lLookupItems = ps.GetPicklistItemsSold();
		csLookupItems.Format(_T("%d"), lLookupItems);

		COleVariant v(csLookupItems);
		m_pAppPSX->SetConfigProperty(_T("LookupItemNumber"), csProduceFavoritesContext,UI::PROPERTYTEXTFORMAT, v);
		
		ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
	}
	
	ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
	ps.Message12(ps.GetInstructions(), true);
	
	if (co.fOperationsAllowMultiPick) 
	{
		if (ps.GetPicklistItemsSold() > 0)
		{
			ps.Button(_T("CMButton3Global"), LTT_CANCELITEMS, true);
		}
		else
		{
			ps.Button(_T("CMButton3Global"), LTT_CANCELITEMS, false);
		}
	}

	ps.ShowCMFrame();
	return STATE_NULL;       
}

/////////////////////////////////////
void SMProduceFavoritesBase::UnInitialize(void)
{
	ps.HideVideoProduce();
	if ( !co.fOperationsAllowMultiPick )
	{
		ps.DeleteAllLevels();
	}
	//RFC236357
	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ProduceFavorites") );	// RFC 330050
	}
}

//////////////////////////////////////
/* @@@INFOPOINT*/
SMStateBase  *SMProduceFavoritesBase::PSButton1(void)// cancel
{
	ps.SetFoundPickListItemsFromSearch(false);
	//TAR 407200 Move this code here, need to wait for replied message from SM before sending ItemSold to SM
	//SR773 only send Skip Bagging message when the previous item is not bag and not void
	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid && !io.d.fCoupon && !g_bMultiPickSkipBaggingAllowed) 
	{
		bKeyInCodeButtonPressed = true;
		//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
		nSAPrevSCOTInputID = 0;
		return STATE_NULL;
	}

	g_bMultiPickSkipBaggingAllowed = false; //TAR417235

	return setAndCreateAnchorState(BES_KEYINCODE);
}

//////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::PSButtonGoBack(void)// cancel
{
  	ps.SetFoundPickListItemsFromSearch(false);
	return setAndCreateAnchorState(BES_SCANANDBAG);
}

SMStateBase  *SMProduceFavoritesBase::PSButton2(void) //search
{
	ps.SetFoundPickListItemsFromSearch(false);
	return setAndCreateAnchorState(BES_LOOKUPITEM);
}

SMStateBase  *SMProduceFavoritesBase::PSButton3(void) //cancel items
{
   // Tell security that we are exiting form ProduceFavorites state
  //TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ProduceFavorites") );
  
  if (co.fOperationsAllowMultiPick) 
  {
	if (ps.GetPicklistItemsSold() > 0)
	{
		ps.Button(_T("CMButton3Global"), LTT_CANCELITEMS, true);

		RETURNSTATE(VoidItem)	
	}
	else
	{
		ps.Button(_T("CMButton3Global"), LTT_CANCELITEMS, false);
	}
  }

  return STATE_NULL;
}

SMStateBase  *SMProduceFavoritesBase::PSButton4(void) //finished with picklist
{ 
   fInMultiSelectPickList = false;
   ps.DeleteAllLevels();

   //SR679
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );
	if (ps.GetPicklistItemsSold() > 0 && !co.IsSecurityBypassOn())
	{
/*	   if (((ps.GetPicklistItemsIncludeVoidItems() > 1) && (io.d.lWgtSold > 0)) ||
		   ((ps.GetPicklistItemsIncludeVoidItems() >= 1) && (io.d.lWgtSold <= 0)))
	   {
		   io.d.lRequireSecSubstChk = TB_OVERRIDE_SECURITY_WITH_NO;
	   }
	   
	   if(!io.bBizerbaScale)
	   {
		   SASendSCOTInput(	ITEM_SOLD_MANUAL,
								io.csItemCode,	//TAR123822  LPM101399 SMStateSABase now determines if it is price embedded
								( io.xd.ExceptionCode == XAPPROVALNONE ) ? 0 : io.lTotalWeightEntered,	// No weight for this exception type												
								io.d.lExtPrice,
								io.lQuantityEntered,
								io.d.lDepartment,
								io.b_gIsThisItemPriceEmbedded	);
	   }
	   else
	   {
		   SASendSCOTInput(	ITEM_SOLD_MANUAL,
								io.csItemCode,	//TAR123822  LPM101399 SMStateSABase now determines if it is price embedded
								io.lWeightEntered,
								io.d.lExtPrice,
								io.lQuantityEntered,
								io.d.lDepartment,
								io.b_gIsThisItemPriceEmbedded	);	
	   }*/

	   if(g_bOnItemOK)
	   {
		   g_lNumberOfItemsNotBagInMultiPick = 0;
	   }

	   return STATE_NULL;
	}
    //TAR 412758
    else if(co.IsSecurityBypassOn())
    {
        g_lNumberOfItemsNotBagInMultiPick = 0;  // TAR 413892
        ps.SetPicklistItemsIncludeVoidItems(0); // TAR 413892
        if((ps.GetPicklistItemsSold() > 0))
        {
            ps.SetPicklistItemsSold(0);         // TAR 413892
            return setAndCreateAnchorState(BES_BAGANDEAS);
        }
        else
        {
            return setAndCreateAnchorState(BES_SCANANDBAG);
        }
    }
    //-TAR 412758
	else
	{
	   ps.SetFoundPickListItemsFromSearch(false);
	   return STATE_NULL; // TAR407109
	}
}

//////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::PSListLookup(CString csItemInfo) // list or lookup selection
{
	//g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	//071904 TAR 273325 BMediano - BEGIN
	//Retrieves information for the selected item.
	//The format is in ItemCode/CategoryId@@Description@@IsSubCategory@@InCategories
	//see PSProceduresBase::DisplaySelectedItem(..) on the formatting.
	csItemSent = csItemInfo.Left( csItemInfo.Find(STR_SEPARATOR));
	int nLen = csItemInfo.GetLength()-(csItemSent.GetLength()-_tcsclen(STR_SEPARATOR));
	CString csDescWithRemainder = csItemInfo.Mid(csItemSent.GetLength()+_tcsclen(STR_SEPARATOR),nLen );
	int nIndex = csDescWithRemainder.Find(STR_SEPARATOR);
	CString csDesc, csSubCatWithRemainder, csSubCategory, csInCategory; 
	if (nIndex != -1)
	{
		csDesc = csDescWithRemainder.Left(nIndex);
		csSubCatWithRemainder= csDescWithRemainder.Mid(nIndex + 2);
		csSubCategory = csSubCatWithRemainder.Left(csSubCatWithRemainder.Find(STR_SEPARATOR));
		csInCategory = csSubCatWithRemainder.Mid(csSubCatWithRemainder.Find(STR_SEPARATOR)+_tcsclen(STR_SEPARATOR));
	}

	if (!csSubCategory.CompareNoCase(_T("True")))
	{
		ps.PushLevels(csInCategory, csItemSent);
		
		ps.DisplaySelectedSubItem(csItemSent);

		ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
		ps.Message12(ps.GetInstructions(), true);

		return STATE_NULL;
	}

	ps.SetItemDesc(csDesc);

	if( csItemSent==_T("") ) //fail in picking list
	{
		DMSayPhrase(PICKLISTINVALID);
		return STATE_NULL;
	}

	//TAR 407200 Move this code here, need to wait for replied message from SM before sending ItemSold to SM
	//SR773 only send Skip Bagging message when the previous item is not bag and not void
	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid && !io.d.fCoupon && !g_bMultiPickSkipBaggingAllowed) 
	{
		//SASendSCOTInput(CANCEL_ITEM, io.csItemCode, 0, 0, 1);
		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);
		nSAPrevSCOTInputID = 0;
		return STATE_NULL;
	}
	
	g_bMultiPickSkipBaggingAllowed = false; //TAR417235


	if (VerifyQtyItem(csItemSent))     
	{
		io.Reset();
		
		// Saved for dynamic picklist purposes //
		SMStateBase::ms_bIsCurrentItemFromPickList = true;
		
		io.csOrigCode = csItemSent;	   // item code keyed
		io.csItemCode = csItemSent;	   // assumed to be the same
		io.d.bPickListItem = true;//SR679
		SASendSCOTInput( UPC_MANUAL, csItemSent);
		
		ps.SetFoundPickListItemsFromSearch(false);
		RETURNSTATE( EnterQuantity )
	}
		

	ps.SetFoundPickListItemsFromSearch(false);
	return TBItemSale( FALSE, 0, false, true );			  //Sell item
}


SMStateBase  *SMProduceFavoritesBase::PSListLookupTab(LPCTSTR lpszTabValue) // list or lookup selection
{
	ps.DeleteAllLevels();
	ps.DisplaySelectedItem(lpszTabValue);

	ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
	ps.Message12(ps.GetInstructions(), true);
	
    return STATE_NULL;
}

SMStateBase  *SMProduceFavoritesBase::PSListScroll(const long lScroll) // list scroll
{
	COleVariant vReturn, v = lScroll; // 0 goes up 1 row and 1 goes down one row
	CPSXUserInputLock userInputLock(_T("SMProduceFavoritesBase::PSListScroll"));  //TAR 371980 fix 

    ps.EnableProduceScrollButtons();
	
	if (!ps.IsLevelEmpty())
	{
		if ( ps.CanScroll(lScroll) )
		{
			v = lScroll;
			m_pAppPSX->SendCommand( _T("ProductImage"), UI::COMMANDSCROLL, vReturn, 1, v );
		}
		else
		{
			CString csCurrentLevel(_T(""));
			CString csPreviousLevel(_T(""));
			CString csRetCategory(_T(""));
			CString csCategoryId(_T(""));

			ps.PopLevels(csPreviousLevel, csCurrentLevel);

			bool bIsCategory = ps.FindCategory(csPreviousLevel, csRetCategory, csCategoryId);

			if (bIsCategory)
			{
				ps.DisplaySelectedItem(csRetCategory);
			}
			else
			{
				ps.DisplaySelectedSubItem(csPreviousLevel);
			}
		}
	}
	else
	{
		ps.DisplayNextCategoryItem(lScroll);
	}
		
	ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
	ps.Message12(ps.GetInstructions(), true);
	

	return STATE_NULL;
}

/////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::DMScanner(void)// scanned item
{
  csItemSent = csDMLastBarCode;
  ps.SetFoundPickListItemsFromSearch(false);
  return TBItemSale(false,0,true);
}

//////////////////////////////////
// TimedOut
//////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::TimedOut(void)
{
	ps.SetFoundPickListItemsFromSearch(false);
	return SMStateBase::TimedOut();
}

SMStateBase *SMProduceFavoritesBase::OnWtIncreaseNotAllowed()	 
{
	DMSaySecurity( SEC_NONE, _T("blank.wav") );

	// 218558 EAS motion sensor received in the wrong order must behave the same as 
	// other violations. Therefore removing the following code LPM103002
	// Stay in the Bag Item screen if have matched wt but not EAS motion
	//if( (!dm.fStateTakeawayBelt) && (g_bMatchedWtWaitingOnEAS && dm.EASDeactivatorPresent() ) )
	//	return STATE_NULL;

	//TAR221282 chu 112702
	//for TAB, if we are at this state that means we are waiting for good weight, so always return to
	//Mismatch state. We added "&& !dm.fStateTakeawayBelt" in the following line:
	//if (g_bMatchedWtWaitingOnEAS)  
	if (g_bMatchedWtWaitingOnEAS && !dm.fStateTakeawayBelt) //TAR221282 chu 112702
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_PRODUCEFAVORITES, getNextStateForGoodItem());
	}

	if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
	{
		CREATE_AND_DISPENSE(SecUnExpectedIncrease)(BES_PRODUCEFAVORITES, getNextStateForGoodItem());
	}

	//unmatched weights while on the BES_PRODUCEFAVORITES mean a mismatched wt so return here, else if cleared, return to s&b
	CREATE_AND_DISPENSE(SecMisMatchWeight)(BES_PRODUCEFAVORITES, getNextStateForGoodItem());
}

//TAR 407200 Add OnSkipBaggingAllowed to make FL to wait until the response from skip bagging message.
//Then process the item
SMStateBase *SMProduceFavoritesBase::OnSkipBaggingAllowed()	 
{
	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) && !g_bOnItemOK && !io.d.fVoid && !bKeyInCodeButtonPressed)
	{
		if (VerifyQtyItem(csItemSent))     
		{
			io.Reset();
			
			// Saved for dynamic picklist purposes //
			SMStateBase::ms_bIsCurrentItemFromPickList = true;
			
			io.csOrigCode = csItemSent;	   // item code keyed
			io.csItemCode = csItemSent;	   // assumed to be the same
			io.d.bPickListItem = true;//SR679
			SASendSCOTInput( UPC_MANUAL, csItemSent);
			
			ps.SetFoundPickListItemsFromSearch(false);
			RETURNSTATE( EnterQuantity )
		}
			

		ps.SetFoundPickListItemsFromSearch(false);
		return TBItemSale( FALSE, 0, false, true );			  //Sell item
	}
	else if (bKeyInCodeButtonPressed)
	{
		bKeyInCodeButtonPressed = false;
		return setAndCreateAnchorState(BES_KEYINCODE);
	}
	else
	{
		return STATE_NULL;
	}
}