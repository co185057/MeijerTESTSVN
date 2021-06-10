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
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"                // Base state       // This state
#include "SMEnterQuantity.h"
#include "PSProceduresbase.h"
#include "SMBagAndEAS.h"
#include "SMVoidItem.h"
#include "SMLookUpItem.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecMisMatchWeight.h"
#include "SMKeyInCode.h"
#include "SMPLAEnterWeight.h"          //	enter wgt state
#include "SMCustomMessage.h"
#else  // _CPPUNIT
#endif // _CPPUNIT
#include "SMProduceFavorites.h"
#include "LaneStateData.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID    _T("ProduceFavorites")

IMPLEMENT_DYNCREATE(SMProduceFavoritesBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(ProduceFavorites)

DEFINE_TIMESTAMP

CString SMProduceFavoritesBase::m_csCategory=_T("");
bool SMProduceFavoritesBase::bKeyInCodeButtonPressed = false;
bool SMProduceFavoritesBase::m_bSymbolOn = false;
////////////////////////////
SMProduceFavoritesBase::SMProduceFavoritesBase()
:m_HKStatus(IPSProcedures::HK_NORMAL),
 m_bShiftOn(false),
 m_bAltGrOn(false)
{
  IMPLEMENT_TIMESTAMP
#ifndef _CPPUNIT
  saMsgFuncPtr = PSProceduresBase::SecurityMessageNonmodal;
#endif // _CPPUNIT
}

///////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::Initialize(void)
{
    if (fInMultiSelectPickList)
    {
#ifndef _CPPUNIT
        SMStateBase *pRetState = ProcessQueuedActions();
        if (pRetState != STATE_NULL)
        {
            return pRetState;
        }
#endif //_CPPUNIT

        if ((short(lTotalPaid)==(nDMCashInDrawer+nDMCashInserted))) //tar 376458 & 376815
        {
            /* TAR 346592 begin */
            if (tb.IsAllowed(TB_FT_SUSPEND) && isAnySoldVoidedItems() & !bSuspendRequested){//TB allows Suspend Button to be enabled so App can too 
                trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=true"));
                ra.OnCanSuspendNow(true) ;  // If it is not partially paid with other(credit, ATM or EBT payment)
                trace(L7, _T("-ra.OnCanSuspendNow"));
            }
            else{//TB diabled Suspend Button so App can not enable it
                trace(L7, _T("+ra.OnCanSuspendNow, CanSuspend=false"));
                ra.OnCanSuspendNow(false) ;
                trace(L7, _T("-ra.OnCanSuspendNow"));
            }
        }
    }
	
	ra.RequestEnableAssistMode(true);	//SSCOADK-5473
    LoadPersonalizedPicklist();
    SetAppStateToSecurity();

    BEST prevAnchorState = getAnchorState();
    UpdateDisplayTextsAndButtons();

	ps.m_bFirstTimeNextButtonEnabled = TRUE;

    COleVariant v;
    m_pAppPSX->GetTransactionVariable( _T("IsQuickPick"), v );
    bool bIsFromQuickPick = v.boolVal;
    if(prevAnchorState == BES_SCANANDBAG)
    {
        m_bSymbolOn = false;//init to false
        SetupTabDisplayFromScanAndBag(prevAnchorState,bIsFromQuickPick);
    }
    else
    {
        SetupTabDisplayFromOtherStates();
    }

    SAWLDBScannerDisable();

	UpdateTextsAndButtonsAfterTabSetup(bIsFromQuickPick, prevAnchorState);
    SetKeyboardSearch();
	ps.ShowCMFrame();
#ifndef _CPPUNIT
    //+ Personalization picklist
    m_EnterPicklistTime = CTime::GetCurrentTime();
    // item was not sold, remove it from the tail of the list
    if ((!g_bHaveSoldFromPicklist) && (g_bHavePicklist) && (g_csPicklistItemSoldInfo.GetSize()))
    {
        g_csPicklistItemSoldInfo.RemoveAt(g_csPicklistItemSoldInfo.GetSize()-1);
    }
    g_bHavePicklist = false;      
    //- Personalization picklist
    //sscop-3361+
    if (co.getTimeOutItemSaleInProgress() && GetISIPTimeOut()) 
    { 
      // Item Sale In Progress (ISIP) timer is running; disable all buttons 
      ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYUSERINPUT , VARIANT_FALSE);

    } 
    //sscop-3361-
#endif
	return STATE_NULL;       
}

#ifndef _CPPUNIT
/////////////////////////////////////
void SMProduceFavoritesBase::UnInitialize(void)
{
	if (co.IsPicklistAssistEnable()) 
	{
		ps.ClearPicklistAssistItemList();
	}
	ps.HideVideoProduce();

	//RFC236357
	DMSaySecurity( SEC_NONE, _T("blank.wav") );	//to interrupt the wave file if it's playing 
	if(!fInMultiSelectPickList)//SR679
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("ProduceFavorites") );	// RFC 330050
	}
}
#endif
//////////////////////////////////////
/* @@@INFOPOINT*/
SMStateBase  *SMProduceFavoritesBase::PSButton1(void)// cancel
{
#ifndef _CPPUNIT
	ps.SetFoundPickListItemsFromSearch(false);
	//TAR 407200 Move this code here, need to wait for replied message from SM before sending ItemSold to SM
	//SR773 only send Skip Bagging message when the previous item is not bag and not void

	if (fInMultiSelectPickList && (ps.GetPicklistItemsSold() > 0) 
        && !g_bOnItemOK && !io.d.fVoid && !io.d.fCoupon && !g_bMultiPickSkipBaggingAllowed) 
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
        g_lNumberOfItemsNotBagInMultiPick = 0;
		return STATE_NULL;
	}

	g_bMultiPickSkipBaggingAllowed = false; //TAR417235
#endif
    RETURNSTATE(KeyInCode)
}

//////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::PSButtonGoBack(void)// cancel
{
    m_csCategory.Empty();
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
  
  if (co.GetfOperationsAllowMultiPick()) 
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
#ifndef _CPPUNIT
    m_csCategory.Empty();
   //TAR429915: don't set this flag here
   //set it when FL receives SmItemPickFinish event from SecurityManager. 
   //fInMultiSelectPickList = false;
     if (co.IsAttendantModeOn()) 
   {
		fInMultiSelectPickList = false;
   }
   ps.DeleteAllLevels();
    

   //SR679
	if (ps.GetPicklistItemsSold() > 0 && !co.IsSecurityBypassOn())
	{
		TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );

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
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );

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
    else if(co.IsSecurityDisableOn())
    {
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") );

		CKeyValueParmList parmList;
		CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_MULTIPICK_INSIDE_DISABLE_SECURITY_MODE);
		_bstr_t bstrParmList((const _TCHAR *)parmList);
		BSTR bstrResult = NULL;
		bstrResult = NULL;
		SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
		SysFreeString(bstrResult);	
        return STATE_NULL;

    }
    //-TAR 412758
    else
    {
        //SSCOADK-1549 SM is still at WaitScale state, need to tell SM moving on 
        CKeyValueParmList parmList;
        CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_SKIP_BAGGING);
        _bstr_t bstrParmList((const _TCHAR *)parmList);
        BSTR bstrResult;
        bstrResult = NULL;
        SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
        SysFreeString(bstrResult);
        nSAPrevSCOTInputID = 0;
       ps.SetFoundPickListItemsFromSearch(false);

	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("MultiPicklist") ); //must call after SKIP_BAGGING is sent for the SM flag to have correct value (true) when SKIP_BAGGING is processed by SM

       return STATE_NULL; // TAR407109
    }
#else
    return STATE_NULL;
#endif
}


//////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::PSListLookup(CString csItemInfo) // list or lookup selection
{
    if(!co.GetfOperationsAllowMultiPick())
    {
        m_csCategory.Empty();
    }
	//g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
	//071904 TAR 273325 BMediano - BEGIN
	//Retrieves information for the selected item.
	//The format is in ItemCode/CategoryId@@Description@@IsSubCategory@@InCategories
	//see PSProceduresBase::DisplaySelectedItem(..) on the formatting.
    trace(L7,_T("PSListLookup iteminfo[%s]"), csItemInfo);
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

    if (csItemSent.Find(TAB_FAV) != -1)  //bwdFav
    {
        ps.PushLevels(csInCategory, csItemSent);
        ps.DisplaySelectedItem(csItemSent, false, true);
        ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
        return STATE_NULL;
    }
#ifndef _CPPUNIT
	else if (!csSubCategory.CompareNoCase(_T("True")))
	{
        //determine the index of the currentcategory through its main category
        long lTabIndex = ps.GetTabSelectedIndexofSubCategory(ps.GetFirstLevelCategoryID(), ps.GetSecondLevelCategoryID());
        CString csIndex;
        csIndex.Format(_T("%d"),lTabIndex + 1);

        //if tab menu are subcategories, set TabSelected correctly
        if(!ps.GetDisplayTabIsMainCategories())
        {
            ps.SetProduceLookupCategoryNumber(csIndex);
        }

		ps.PushLevels(csInCategory, csItemSent);
		
		ps.DisplaySelectedSubItem(csItemSent);

		ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
		//ps.Message12(ps.GetInstructions(), true);

        m_csCategory.Empty();
        
		return STATE_NULL;
	}

    //+ Personalization picklist
    CTimeSpan tTimeElapsed = CTime::GetCurrentTime() - m_EnterPicklistTime;
    long lPicklistTimeSpent = tTimeElapsed.GetTotalSeconds();
    CString csPicklistTimeSpent;
    csPicklistTimeSpent.Format(_T("%ld000"), lPicklistTimeSpent);
    CString csPicklistSoldItemMetrics = csPicklistTimeSpent + STR_SEPARATOR + csItemSent + 
       STR_SEPARATOR + csInCategory;
    g_csPicklistItemSoldInfo.Add(csPicklistSoldItemMetrics);
    trace(L7, _T("PicklistSoldItemMetrics = %s"), csPicklistSoldItemMetrics);
    g_bHavePicklist = true;
    g_bHaveSoldFromPicklist = false;
    //- Personalization picklist

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
		if(g_bAllowLateBagging)
		{
			CKeyValueParmList parmList;
			CKeyValue<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_CLEAR_ALLOW_LATE_BAGGING);
			_bstr_t bstrParmList((const _TCHAR *)parmList);
			BSTR bstrResult;
			bstrResult = NULL;
			SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
			SysFreeString(bstrResult);
		}
		else
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
		}

		return STATE_NULL;
	}
	
	g_bMultiPickSkipBaggingAllowed = false; //TAR417235

#endif // _CPPUNIT

	if (VerifyQtyItem(csItemSent))     
	{
		io.Reset();
		
		// Saved for dynamic picklist purposes //
		SMStateBase::ms_bIsCurrentItemFromPickList = true;
		
		io.csOrigCode = csItemSent;	   // item code keyed
		io.csItemCode = csItemSent;	   // assumed to be the same
		io.d.bPickListItem = true;//SR679
        if (!co.IsPicklistAssistEnable()) // dont Send SASendScot when PLA is enable, Let TBItemSale called by PLAEnterWeight do
        {
            SASendSCOTInput( UPC_MANUAL, csItemSent);
        }
        
        SetNextGenUICurrentItem(io.csOrigCode);
		
		ps.SetFoundPickListItemsFromSearch(false);
		RETURNSTATE( EnterQuantity )
	}


	ps.SetFoundPickListItemsFromSearch(false);

    if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csItemSent))
    {
        io.Reset();
        io.csOrigCode = csItemSent;
        io.csItemCode = csItemSent;
        RETURNSTATE(PLAEnterWeight)
    }
    else
    {
        return TBItemSale( FALSE, 0, false, true ); //Sell item
    }
}

#ifndef _CPPUNIT

SMStateBase  *SMProduceFavoritesBase::PSListLookupTab(LPCTSTR lpszTabValue) // list or lookup selection
{
    if(!CheckSubCategoryDisplay(lpszTabValue))
    {
        ps.DisplaySelectedItem(lpszTabValue);
    }

	ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
	//ps.Message12(ps.GetInstructions(), true);
	
    return STATE_NULL;
}

SMStateBase  *SMProduceFavoritesBase::PSListScroll(const long lScroll) // list scroll
{
    m_csCategory.Empty();
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

            int nIndex;
			bool bIsCategory = ps.FindCategory(csPreviousLevel, csRetCategory, csCategoryId, nIndex);

			if (bIsCategory)
			{
                ps.SetCategoryListText(_T("TabSelected"), _T(""));

                COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );

                vIndex = SelectedTabIndex(bIsCategory,csRetCategory, getAnchorState(), true);
                vParam = (long) UI::Pushed;
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );


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
	//ps.Message12(ps.GetInstructions(), true);
	

	return STATE_NULL;
}

#endif //_CPPUNIT
/////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::DMScanner(void)// scanned item
{
  csItemSent = csDMLastBarCode;
  ps.SetFoundPickListItemsFromSearch(false);
  if (co.IsPicklistAssistEnable() && ps.IsPickListItem(csDMLastBarLabel))
  {
      io.Reset();
      csItemSent = csDMLastBarLabel;
      io.csOrigCode = csItemSent;
      io.csItemCode = csItemSent;
      RETURNSTATE(PLAEnterWeight)
  }
  else
  {
      return TBItemSale(false,0,true);
  }
}
#ifndef _CPPUNIT

//////////////////////////////////
// TimedOut
//////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::TimedOut(void)
{
    ps.SetFoundPickListItemsFromSearch(false);
    return STATE(State)::TimedOut();
    
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
#endif //_CPPUNIT
SMStateBase *SMProduceFavoritesBase::OnSkipBaggingAllowed()	 
{
	g_bNoBaggingRequiredForTheCurrentItem = false;
	if(g_bAllowLateBagging)
	{
		g_bAllowLateBagging = false;
	}
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
			
            SetNextGenUICurrentItem(io.csOrigCode);
			ps.SetFoundPickListItemsFromSearch(false);
			RETURNSTATE( EnterQuantity )
		}
			

		ps.SetFoundPickListItemsFromSearch(false);
		return TBItemSale( FALSE, 0, false, true );			  //Sell item
	}
	else if (bKeyInCodeButtonPressed)
	{
		bKeyInCodeButtonPressed = false;
        RETURNSTATE(KeyInCode)
	}
	else
	{
		return STATE_NULL;
	}
}

#ifndef _CPPUNIT
//430778 no need to do createAnchorState() as base does.
SMStateBase *SMProduceFavoritesBase::OnBackToLGW(void)
{
	SMStateBase  *pRetState = STATE_NULL;

    pRetState = STATE(State)::OnBackToLGW();

	delete getRealStatePointer( pRetState);
	return STATE_NULL;
}

//SR078+
SMStateBase *SMProduceFavoritesBase::SASecMgrParse( const MessageElement *me)
{

    int nEvent = me->secmgrInfo.nEvt;
	trace(L6, _T("SMProduceFavoritesBase::SASecMgrParse nEvent = %d"), nEvent);
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    
	SecurityControlEventParms secControlEventParms;
	if (_tcslen(szParms) != 0)
	{
		secControlEventParms.ParseKeys(szParms);
	}
	delete [] szParms;

    if ( (nEvent == SmOnSecurityControl) && 
		 (secControlEventParms.m_csOperation == KEY_OSC_OPERATION_GOTO_SCANANDBAG ) &&
		 (nTBLastRealState == TB_ITEMSOLD ) &&
		 fInMultiSelectPickList)
    {
		trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
        delete [] me->secmgrInfo.szParms;
		g_bNoBaggingRequiredForTheCurrentItem = true;
		return STATE_NULL;
	} 
	if( (nEvent == SmOnSecurityControl) && 
		 (secControlEventParms.m_csOperation == KEY_OSC_OPERATION_ALLOWLATEBAGGING_GOTO_SCANANDBAG ))
	{
		trace(L6, _T("KEY_OSC_OPERATION_ALLOWLATEBAGGING_GOTO_SCANANDBAG is received"));
		delete [] me->secmgrInfo.szParms;
		g_bAllowLateBagging = true;
		return STATE_NULL;
	}

    return STATE(State)::SASecMgrParse(me);
}
//SR078-

void SMProduceFavoritesBase::SetPSContext()
{
    if ( co.GetfOperationsAllowMultiPick() )
        m_csProduceFavoritesContext= _T("MultiSelectProduceFavorites") ;
    else
        m_csProduceFavoritesContext = _T("ProduceFavorites") ;

    m_csProduceFavoritesContext = ps.AppendContextWithLCID(m_csProduceFavoritesContext);

    ps.SetCMFrame(m_csProduceFavoritesContext);
    
}

void SMProduceFavoritesBase::SetKeyboardSearch(void)
{
    m_HKStatus=IPSProcedures::HK_NORMAL;
    m_bShiftOn=false;
    m_bAltGrOn=false;

    SetAlphaShiftState(m_csProduceFavoritesContext, m_bShiftOn);

    ps.PopulateHotKeys(m_csProduceFavoritesContext);
    ps.PopulateHKSensitiveControls(m_csProduceFavoritesContext, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
    ps.SetShiftAndSymbolState(m_csProduceFavoritesContext, m_bSymbolOn);
}

SMStateBase  *SMProduceFavoritesBase::PSHotKey(CString & csHotKey) 
{
    PSProceduresBase::HKStatus newStatus = PSProceduresBase::HK_NORMAL;
    if(csHotKey == _T("HKAcute"))
    {
        newStatus = PSProceduresBase::HK_ACUTEON;
    }
    else if(csHotKey == _T("HKGrave"))
    {
        newStatus = PSProceduresBase::HK_GRAVEON;
    }
    else if(csHotKey == _T("HKCircumflex"))
    {
        newStatus = PSProceduresBase::HK_CIRCUMFLEXON;
    }
    else if(csHotKey == _T("HKUmlaut"))
    {
        newStatus = PSProceduresBase::HK_UMLAUTON;
    }
    else if(csHotKey == _T("HKTilde"))
    {
        newStatus = PSProceduresBase::HK_TILDEON;
    }

    COleVariant vIndex(0L), vReturn;

    if(m_HKStatus == newStatus)
    {
        m_HKStatus = PSProceduresBase::HK_NORMAL;
    }
    else
    {
        m_HKStatus = newStatus;
    }


    PSXRC rc = PSX_SUCCESS;
    rc = m_pAppPSX->GetControlProperty( _T("PFKBHotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
    if(rc == PSX_SUCCESS)
    {
        long nButtonCount = vIndex.lVal;
        if(nButtonCount)
        {
            m_pAppPSX->SetConfigProperty(_T("PFKBHotKeys"), ps.GetCurrentContext(), UI::PROPERTYREDRAW, VARIANT_FALSE);
            if(m_HKStatus == PSProceduresBase::HK_NORMAL)
            {
                for( long i = 0L; i < nButtonCount; i++ )
                {
                    COleVariant pvParams[2];
                    pvParams[0]=i;
                    pvParams[1]=(long) UI::Normal;
                    m_pAppPSX->SendCommand( _T("PFKBHotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
                }
             }
             m_pAppPSX->SetConfigProperty(_T("PFKBHotKeys"), ps.GetCurrentContext(), UI::PROPERTYREDRAW, VARIANT_TRUE);
        }

        ps.PopulateHKSensitiveControls(ps.GetCurrentContext(), m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
    }


    return STATE_NULL;//remain in same state

}

SMStateBase  *SMProduceFavoritesBase::PSShiftKey(void)
{
  
  m_bShiftOn=!m_bShiftOn;
  SetAlphaShiftState(m_csProduceFavoritesContext, m_bShiftOn);

  ps.PopulateHKSensitiveControls(m_csProduceFavoritesContext, m_HKStatus, m_bShiftOn, m_bAltGrOn,m_bSymbolOn);

  return STATE_NULL;
}

void SMProduceFavoritesBase::HideFavoritesButton(const long nSelectIndex,
                                                 const long nButtonCount, 
                                                 const bool bShow)
{
   COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );

   for( long x = nSelectIndex; x < nButtonCount; x++ )
   {
       vIndex = x;
       CString csTemp;
       
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
       if( csTemp == TAB_FAV )
       {
           trace(L6,_T("ProduceFavoritesBase::HideFavoritesButton - Hide/Show[%d] favorites tab."), bShow);
           vParam = bShow ? VARIANT_TRUE : VARIANT_FALSE;
           m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, vParam );
       }
   }
}
#endif

long SMProduceFavoritesBase::SelectedTabIndex(bool bCategory, CString csCategory, BEST previousAnchorState, bool bScroll)
{
    trace(L7,_T("SMProduceFavoritesBase::SelectedTabIndex csCategory[%s], prevAnchor[%d], scroll[%d]"), 
                            csCategory, previousAnchorState, bScroll);
   long lSelectIndex = 0;
#ifndef _CPPUNIT
   CString csTemp( _T("") );
   COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );

   m_pAppPSX->GetControlProperty( _T("TabSelected"), UI::PROPERTYBUTTONCOUNT, vIndex );
   long lButtonCount = vIndex.lVal;

   bool bFavButtonEnabled = IsFavButtonEnabled();


   for( long i = 0; i < lButtonCount; i++ )
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
       
       // if favorites option is set or personalization picklist is set then show the Favorites tab //
       if(bFavButtonEnabled) // SSCOP-2575 - moved the condition outside the for loop
       {
           if( csTemp == TAB_FAV )
           {
               vParam = VARIANT_TRUE;
               m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, vIndex, vParam );
               lSelectIndex = i;
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
            (csItemSent == ps.GetLastProduceLookupList() 
            || csItemSent.Find(_T("SUBCATEGORY")) != -1) && (previousAnchorState == BES_SCANANDBAG) 
            || previousAnchorState == BES_PRODUCEFAVORITES && bScroll) //using scroll so recheck fave button
       {
           lSelectIndex = i;

           if(lSelectIndex < lButtonCount)
           {
                HideFavoritesButton(lSelectIndex + 1, lButtonCount, bFavButtonEnabled);
           }
           break;
       }

       //add here condition if we are scrolling and DisplaySubCategoriesOnTop is enabled
       if(co.fOperationsDisplayPickListSubCategoriesInTab)
       {
           if(csTemp == csCategory)
           {
               lSelectIndex = i;
               break;
           }
           else
           {
               lSelectIndex = 0;

           }
       }
   }
#endif
   return lSelectIndex;
}

bool SMProduceFavoritesBase::IsFavButtonEnabled(void)
{
    // +SSCOP-2575
    CString csPicklistPref;
    bool bIsFavButtonEnabled;

    if (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0)
    {
        csPicklistPref = custInfo.GetPreference(_T("PicklistFavorites"));
    }

   // if favorites option is set or personalization picklist is set then show the Favorites tab //
   if( co.IsPicklistAssistEnable() || ( co.fStateProduceFavorites && ps.ItemsInFavoritesList() > 0 ) ||
       ((_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0) && 
       csPicklistPref.GetLength())) //Personalization picklist
   {
       bIsFavButtonEnabled = true;
   }
   else
   {
       bIsFavButtonEnabled = false;
   }
   // -SSCOP-2575

   return bIsFavButtonEnabled;
}

SMStateBase  *SMProduceFavoritesBase::PSCharKey(TCHAR c)
{
   ps.EchoInput(c, true, true);
   ps.GetInput(csItemSent);
   
   return SearchStringFromList();
}

///////////////////////////////////////
SMStateBase  *SMProduceFavoritesBase::SearchStringFromList(void)
{
   m_csCategory.Empty();
   ResetNextGenUIPicklistDisplayLevelsData();
   if(csItemSent.IsEmpty())
   {
       trace(L7,_T("SearchStringFromList Empty keyword -- clear or empty from backspace"));
#ifndef _CPPUNIT
       CPSXRedrawLock csRedrawLock( _T("ProductImage"), _T("SMProduceFavoritesBase::SearchStringFromList"));   
#endif
       m_pAppPSX->SetControlProperty( _T("ProductImage"), UI::PROPERTYBUTTONCOUNT, 0L );
   }
   else
   {
       trace(L6,_T("SearchStringFromList-- keyword[%s]"), csItemSent);
       ps.SetFoundPickListItemsFromSearch(true);
       int nCount=ps.DisplaySelectedItem(csItemSent, true);
       if(nCount<=0)
       {
          CustomMessageObj.csScreenText = ps.GetPSText(MSG_NOMATCHTRYAGAIN);
          CustomMessageObj.csLeadThruText = ps.GetPSText(MSG_ITEMNOTFOUND);

          RETURNSTATE(CustomMessage)
       }
   }
   nTimeIdleMsec = 0;
   ResetTabSelectedTabFromSearch();
   return STATE_NULL;
}

void SMProduceFavoritesBase::ResetNextGenUIPicklistDisplayLevelsData(bool bResetSearch)
{
    int nTabSelected = -1;
    CString csSearchKey, csLevels;

    if(!bResetSearch)
    {
        csSearchKey.Format(_T("SearchKey=%s;"),csItemSent);
    }

    csLevels.Format(_T("TabSelected=%d;%s"), nTabSelected, csSearchKey);
    CLaneStateData laneStateData(m_pAppPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUIPicklistDisplayLevels"), csLevels);
    trace(L6,_T("NextGenUIPicklistDisplayLevels [%s]"), csLevels);
}

void SMProduceFavoritesBase::ResetTabSelectedTabFromSearch(void)
{
    m_csCategory.Empty();
    ps.SetCategoryListText(_T("TabSelected"), _T(""));//csItemSent);

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

SMStateBase  *SMProduceFavoritesBase::PSClearKey(void)
{
  csItemSent=EMPTY_STRING;
  ps.ClearInput();
  return SearchStringFromList();
}

SMStateBase  *SMProduceFavoritesBase::PSBackSpaceKey(void)
{
  long lLen = ps.GetInput(csItemSent);
  if (lLen)
  {
    ps.EchoInput(0x08, false, true);	// clear or correction key should clear only one character
    lLen = ps.GetInput(csItemSent);
    if(lLen == 0)
    {
        ps.ClearInput();
    }
  }
  return SearchStringFromList();
}

SMStateBase  *SMProduceFavoritesBase::PSAltGrKey(void)
{
  m_bAltGrOn=!m_bAltGrOn;

  ps.PopulateHKSensitiveControls(m_csProduceFavoritesContext, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);

  return STATE_NULL;
}

SMStateBase  *SMProduceFavoritesBase::PSSymbolKey(void)
{
  m_bSymbolOn=!m_bSymbolOn;
  if(m_bSymbolOn)
  {
      m_bShiftOn = false;
  }

  ps.PopulateHKSensitiveControls(m_csProduceFavoritesContext, m_HKStatus, m_bShiftOn, m_bAltGrOn, m_bSymbolOn);
  ps.SetShiftAndSymbolState(m_csProduceFavoritesContext, m_bSymbolOn);

  return STATE_NULL;
}

SMStateBase  *SMProduceFavoritesBase::PSSpaceKey(void)
{
  return PSCharKey(_T(' '));
}


void SMProduceFavoritesBase::LoadPersonalizedPicklist(void)
{
#ifndef _CPPUNIT
    //+ Personalization picklist
    CString csPicklistPref;
    if (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0)
    {
        csPicklistPref = custInfo.GetPreference(_T("PicklistFavorites"));
        trace(L6, _T("PersonalizeItems = %s"), csPicklistPref);

        if (csPicklistPref.GetLength() && (g_csPreviousPicklist != csPicklistPref))
        {
            ps.LoadPersonalizedPopularList(csPicklistPref);
            g_csPreviousPicklist = csPicklistPref;
        }
    }
    //- Personalization picklist
#endif
}

bool SMProduceFavoritesBase::CheckSubCategoryDisplay(const CString csTabValue)
{
    bool bReturn(false);
#ifndef _CPPUNIT
    trace(L7,_T("CheckSubCategoryDisplay --tab [%s]"), csTabValue);
    CString csMainCategory = ps.GetFirstLevelCategoryID();

    ps.DeleteAllLevels();
    ps.ClearInput();

    CString csItem(csTabValue);

    CString csCategory(_T(""));
    CString csSubCategoryID( _T(""));
    CString csCatAlphabetic;
    bool bIsMainCategory = false;
    bool bIsSubCategory = false;
    bool bMainCategoryHasSubCategory = false;
    int nTabIndex(-1);

    if(!csItem.IsEmpty())
    {
        bIsMainCategory = ps.FindCategory(csItem, csCatAlphabetic, csCategory, nTabIndex);

        if(bIsMainCategory)
        {
            bMainCategoryHasSubCategory = ps.HasSubCategory(csCategory, csSubCategoryID);
        }
        else
        {
            bIsSubCategory = ps.FindSubCategory(csItem);
        }
    }

    if(co.fOperationsDisplayPickListSubCategoriesInTab && ((bIsMainCategory && bMainCategoryHasSubCategory) || bIsSubCategory ))
    {
        m_csCategory = csTabValue;
        if(bIsSubCategory)
        {
            csCategory = csMainCategory;
            int nLen = csItem.GetLength();
            csSubCategoryID = csItem.Right( nLen - _tcslen(TAB_CATEGORY));
        }

        //determine the index of the currentcategory through its main category
        long lTabIndex = ps.GetTabSelectedIndexofSubCategory(csCategory, csSubCategoryID);
        ps.SetSubCategorySelectedButton(lTabIndex);
        CString csIndex;
        csIndex.Format(_T("%d"),lTabIndex + 1);

        ps.SetProduceLookupCategoryNumber(csIndex);

        ps.PushLevels(csCategory, csSubCategoryID);

        ps.SetCategoryListText(_T("TabSelected"), csItem);

        ps.DisplaySelectedSubItem(csSubCategoryID);

        bReturn = true;
    }
#endif
    return bReturn;
}

void SMProduceFavoritesBase::SetDisplayToNoTabSelected(const int nButtonCount)
{
    trace(L7,_T("SMProduceFavoritesBase::SetDisplayToNoTabSelected"));
    COleVariant vIndex(0L), vParam(0L), vReturn(0L);
    //nothing gets highlighted / selected
    vParam = (long) UI::Normal;

    for( long i = 0; i < nButtonCount; i++ )
    {
        vIndex = i;
        m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
    }

    ps.DisplaySelectedItem(_T("   "));

    ResetNextGenUIPicklistDisplayLevelsData(true);
}

void SMProduceFavoritesBase::SetAppStateToSecurity(void)
{
#ifndef _CPPUNIT
    if(fInMultiSelectPickList)
    {
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("MultiPicklist") ); //tar430757
        bKeyInCodeButtonPressed = false;
        if(ps.GetPicklistItemsIncludeVoidItems() == 0)
        {
            g_bMultiPickSkipBaggingAllowed = false;// TAR 398289
        //  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("MultiPicklist") );
        }
    }
    else
    {
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("ProduceFavorites") );    // RFC 330050
    }
#endif
}

void SMProduceFavoritesBase::UpdateDisplayTextsAndButtons(void)
{
#ifndef _CPPUNIT
	setAnchorState( BES_PRODUCEFAVORITES );

    // Update RAP window with SCOT state //
    UpdateStateName( ps.GetPSText( LTT_PRODUCEFAVORITES, SCOT_LANGUAGE_PRIMARY ) );

    SetPSContext();

    /*if ( !co.GetfOperationsAllowPickListSearchButton() )
    {
        m_pAppPSX->SetConfigProperty(_T("CMButton2Lg"), m_csProduceFavoritesContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
       COleVariant vReturn;
       m_pAppPSX->GetConfigProperty( _T("CMButton2Lg"), m_csProduceFavoritesContext, UI::PROPERTYPOSITION, vReturn);
        m_pAppPSX->SetConfigProperty(_T("CMButton1Global"), m_csProduceFavoritesContext, UI::PROPERTYPOSITION, vReturn);
    }*/

    // allow the EchoInput() func to accept up to max digits //
	ps.AllowInput( MAX_DIGITS_ITEMCODE, false, false );
#endif
}

void SMProduceFavoritesBase::UpdateTextsAndButtonsAfterTabSetup(bool bIsFromQuickPick, BEST prevAnchorState)
{
#ifndef _CPPUNIT
    if ( co.GetfOperationsAllowMultiPick() )
    {
        CString csLookupItems;
        long lLookupItems = ps.GetPicklistItemsSold();
        csLookupItems.Format(_T("%d"), lLookupItems);

        COleVariant v(csLookupItems);
        m_pAppPSX->SetConfigProperty(_T("LookupItemNumber"), m_csProduceFavoritesContext,UI::PROPERTYTEXTFORMAT, v);
        
        ps.CMButtonFlash(_T("CMButton4LgFinish"),true);
    }
    
    if(!co.fOperationsHighlightFirstCategoryFromSearch && !bIsFromQuickPick && prevAnchorState == BES_SCANANDBAG)
    {
        ps.SetLeadthruText(_T(""), _T("")); 
        ps.Message12(_T(""), true);
    }
    else
    {
        ps.SetLeadthruText(ps.GetScreenTitle(), _T("")); 
    }
    //ps.Message12(ps.GetInstructions(), true); // SSCOP-2575   
    
    if (co.GetfOperationsAllowMultiPick()) 
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
   
#endif
}
#ifndef _CPPUNIT
SMStateBase *SMProduceFavoritesBase::PSButtonHelp(void)
{
    ps.SetFoundPickListItemsFromSearch(false);
    return STATE(State)::PSButtonHelp();
}
#endif
bool SMProduceFavoritesBase::GetSearchKey(CString &csSearchKey)
{
    bool bFind(false);
    COleVariant v;
    m_pAppPSX->GetTransactionVariable( _T("NextGenUIPicklistDisplayLevels"), v );
    CString csDisplayLevels(v.bstrVal);

    CString csKey(_T("SearchKey"));
    int nIndex = csDisplayLevels.Find(csKey);
    if (nIndex != -1)
    {
        int nCount = csDisplayLevels.GetLength()-(nIndex+1);
        csSearchKey = csDisplayLevels.Mid(nIndex, nCount);
        nIndex = csSearchKey.Find(_T("="));
        if(nIndex > 0)
        {
            csSearchKey = csSearchKey.Mid(nIndex + 1);
        }
    }
    if(!csSearchKey.IsEmpty())
    {
        bFind = true;
    }
    return bFind;
}

void SMProduceFavoritesBase::SendSearchString(const CString csSearch)
{
    trace(L6,_T("SMProduceFavorites::Initialize--back while searching [%s]"), csItemSent);
    csItemSent = csSearch;
    ps.SetInputText(csItemSent);
    ps.DisplaySelectedItem(csItemSent, true);
    ResetTabSelectedTabFromSearch();
}

void SMProduceFavoritesBase::SetupTabDisplayFromScanAndBag(BEST prevAnchorState,bool bIsFromQuickPick)
{

    ps.SetCategoryListText(_T("TabSelected"), csItemSent);

    COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );
    m_pAppPSX->GetControlProperty( _T("TabSelected"), UI::PROPERTYBUTTONCOUNT, vIndex );
    long nButtonCount = vIndex.lVal;

    long nSelectIndex = 0;
    bool bSubCategory = false;

    CString csCategory(_T(""));
    CString csSubCategoryID( _T(""));
    CString csCatAlphabetic;
    bool bIsMainCategory = false;
    bool bIsSubCategory = false;
    bool bMainCategoryHasSubCategory = false;
    int nTabIndex(-1);
#ifndef _CPPUNIT
    //from ScanAndBag
    if(!csItemSent.IsEmpty())
    {
        bIsMainCategory = ps.FindCategory(csItemSent, csCatAlphabetic, csCategory, nTabIndex);

        if(bIsMainCategory)
        {
            bMainCategoryHasSubCategory = ps.HasSubCategory(csCategory, csSubCategoryID);
        }
        else
        {
            bIsSubCategory = ps.FindSubCategory(csItemSent);
        }
    }
#endif
    trace(L6,_T("SetupTabDisplayFromScanAndBag buttoncount[%d]"), nButtonCount);
    if( nButtonCount > 0 )
    {
        nSelectIndex = SelectedTabIndex(bIsMainCategory,csCategory, prevAnchorState);

        if(!co.fOperationsHighlightFirstCategoryFromSearch && !bIsFromQuickPick)
        {
            SetDisplayToNoTabSelected(nButtonCount);
        }
        else
        {
            vParam = (long) UI::Pushed;
            if( csItemSent.Find(_T("HIDDENCATEGORY_SUBCATEGORY")) != -1)
            {
                int nLen = csItemSent.GetLength();
                csItemSent = csItemSent.Right( nLen - _tcslen(_T("HIDDENCATEGORY_SUBCATEGORY")) );

                ps.SetProduceLookupCategoryNumber(_T("-1"));//no tab selection

                ps.PushLevels(_T(""), csItemSent);

                ps.DisplaySelectedSubItem(csItemSent);
                
            }
            else if ( csItemSent.Find(_T("SUBCATEGORY")) != -1 )
            {
#ifndef _CPPUNIT
                vIndex = nSelectIndex;
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );
                
                int nLen = csItemSent.GetLength();
                csItemSent = csItemSent.Right( nLen - _tcslen(_T("SUBCATEGORY")) );
                ps.DisplaySelectedSubItem(csItemSent);
#endif
            }
            else if(co.fOperationsDisplayPickListSubCategoriesInTab && ((bIsMainCategory && bMainCategoryHasSubCategory) || bIsSubCategory ))
            {
#ifndef _CPPUNIT
                vIndex = nSelectIndex;
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );

                //determine the index of the currentcategory through its main category
                long lTabIndex = ps.GetTabSelectedIndexofSubCategory(csCategory, csSubCategoryID);
                ps.SetSubCategorySelectedButton(lTabIndex);
                CString csIndex;
                csIndex.Format(_T("%d"),lTabIndex + 1);

                ps.SetProduceLookupCategoryNumber(csIndex);

                ps.PushLevels(csCategory, csSubCategoryID);

                ps.DisplaySelectedSubItem(csSubCategoryID);
#endif
            }
            else 
            {
#ifndef _CPPUNIT
                vIndex = nSelectIndex;
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
                m_pAppPSX->SendCommand( _T("TabSelected"), UI::COMMANDGETBUTTONDATA, vReturn, 1, vIndex );

                if( VT_BSTR == vReturn.vt )
                {
                    ps.DisplaySelectedItem(vReturn.bstrVal);
                }
#endif
            }
            
            int nPicklistMsg = MSG_LOOKUP_TITLE2;
            bool b_IsFavButtonEnabled = IsFavButtonEnabled();
            if(b_IsFavButtonEnabled && nSelectIndex == 8)
            {
                DMSayPhrase( SELECTFROMPOPULAR );
                ps.Message12(ps.GetPSText( nPicklistMsg ), true);
            }
            else
            {
                DMSayPhrase( SELECTFROMLIST );
                ps.Message12(ps.GetPSText( MSG_LOOKUP_TITLE1 ), true);
            }
        }
        csItemSent = _T("");
    }
}

void SMProduceFavoritesBase::SetupTabDisplayFromOtherStates(void)
{
    if(ps.GetFoundPickListItemsFromSearch())
    {//back from item not found
      ResetNextGenUIPicklistDisplayLevelsData(true);
      ResetTabSelectedTabFromSearch();
    }
    else
    {
        COleVariant vIndex( 0L );
        m_pAppPSX->GetControlProperty( _T("TabSelected"), UI::PROPERTYBUTTONCOUNT, vIndex );
        long nButtonCount = vIndex.lVal;

        trace(L6,_T("SetupTabDisplayFromOtherStates buttoncount[%d]"), nButtonCount);
        if( nButtonCount > 0 )
        {
            CString csSearch;
            if(GetSearchKey(csSearch))
            {
                SendSearchString(csSearch);
            }
            else if(!m_csCategory.IsEmpty())
            {
                trace(L6,_T("SMProduceFavorites::Initialize--back while in tab [%s]"), m_csCategory);
                (void)CheckSubCategoryDisplay(m_csCategory);
            }
            else
            {
                trace(L7,_T("SetupTabDisplayFromOtherStates -- do nothing"));
            }
            csItemSent = _T("");
        }
    }
}

SMStateBase *SMProduceFavoritesBase::DMAcceptor(void)
{
    trace(L6,_T("SMProduceFavoritesBase::DMAcceptor -- Returning to ScanAndBag."));
    if(co.GetfOperationsAllowMultiPick())
    {
        return PSButton4();
    }
    else
    {
        return PSButtonGoBack();      
    }
}
