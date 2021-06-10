//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmMediaStatusBase.CPP
//
// TITLE: Class implementation for Store Mode Media Status state
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMState.h"				// Base state
#include "SMSmAuthorization.h"
#include "SMSmMediaStatus.h"
#include "DMProcedures.h"
#include "SMSmCardManagement.h" //4.2 - Card Dispenser
#include "SMSmCashierPassword.h" //generic cash management
#include "GloryOpos.h"			//SR742  Need CHAN_STATUS_JAM definition.
#include "SMTakeChange.h" 
#include "SMLaneClosed.h"
//SR672+
#include "OPOSPtr.h"
#include "DeviceErrorLookUpAPI.h" 
//SR672-

#include "SMSmCashManagementEx.h" //SR700
#include "CMInterface.h" //SR700
#include <assert.h>                 // Use CRT assert.  It supports UNICODE.

#else
#include "SMSmMediaStatusBase.h"
#include "FakeSMStoreModeMenuBase.h"
#include "TestMacros.h"
#endif

#define COMP_ID ID_SM				// base state component
#define T_ID _T("SmMediaStatusBase")
#ifndef _CPPUNIT
#include "MethodTrace.h"
#endif
#ifdef _DEBUG
#define MSGFMT _T("%s (%d)")
#else
#define MSGFMT _T("%s")
#endif

#ifdef _DEBUG
#define ERRFMT _T("%s (%d,%d)")
#else
#define ERRFMT _T("%s")
#endif

#define CASE_MSG(a)				    \
case DM_##a:                        \
  csWork.Format(MSGFMT,             \
	ps.GetPSText(MSG_##a),			\
	lCode);							\
	break;

#ifndef _CPPUNIT
bool SMSmMediaStatusBase::m_bIsButtonFourPressed = false; //4.2 - Card Dispenser
#endif
IMPLEMENT_DYNCREATE(SMSmMediaStatusBase, CObject)// MFC Runtime class/object information

//IMPLEMENT_STATE(SmMediaStatus)


DEFINE_TIMESTAMP

//===================================================================
// constructor
//===================================================================
SMSmMediaStatusBase::SMSmMediaStatusBase()
{
#ifndef _CPPUNIT
  IMPLEMENT_TIMESTAMP
  bReadDataFromRegistry = false;
  bCashDispenserOnly = false;
  bCashDeviceErrorToDispaly = true;  //415172
#endif
}
#ifndef _CPPUNIT
//===================================================================
// Initialize
///===================================================================
SMStateBase  *SMSmMediaStatusBase::Initialize(void)
{
    //+tar415390
    if(CString(_T("SMTakeCard")) == this->csLastNonHelpStateName)
    {
        TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmAuthorization") );	
    }
    //-tar415390
	 TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmMediaStatus") );	// RFC 330050

    dm.CashRecyclerPauseCounts(true);	// SSCOP-2197
    printerUtils.ResetReceiptPrinter();
	 //+SR742  
    if ( co.IsDegradedModeOn() && dm.GetDegradedModeType() == DEGRADEDMODE_CARD_ONLY )
    {
		//tar409422 and 410364
		ExitDegradedMode(); 
		
		SMStateBase  * pRetState = DisplayCashDeviceError();
		if (bCashDeviceErrorToDispaly)  //415172
			return pRetState;
    }
	//Tar 450827
    if ( co.IsDegradedModeOn() && 
         dm.GetDegradedModeType() == DEGRADEDMODE_OFF )
    {
        // SSCOP-4169:  Generate a SystemMessage if reload of cash
        //              devices fails.
        SMStateBase *pRetState = ReloadCashDevices();

        if(pRetState != STATE_NULL)
        {
            return pRetState;
        }
    }
    //-SR742 
	bIsButtonOnePressed = false; //TAR225069 CHU/DALE
	m_bIsButtonFourPressed = false; //4.2 - Card Dispenser
    //TAR200557 if media errors, set bToldRAPMediaLow to false  so will tell RAP about the problem
    if (dm.GetCoinsOrBillsVeryLow() || dm.GetPrinterPaperLow()) 
        bToldRAPMediaLow = true;	
    else 
        bToldRAPMediaLow = false;
  	  	
    UpdateDeviceStatus();

    if(fPutSCOTinLC)
    {
      RETURNSTATE(LaneClosed)
    }

   // NewUI
   ps.SetFrame(_T("SmMediaStatus"));
   // End NewUI
   ps.SetLeadthruText(PS_BLANK, PS_BLANK);

   ps.Echo( _T("") ); //dp185016 tar 299411

   int i;
   bool bNeedCashBack = false;

   for(i=0; i < MAXBUTTONNUM; i++)
   {
	   if (co.TenderList[ps.m_languageUsed][i].bCashBack)
	   {
		   bNeedCashBack = true;
		   break;
	   }
   }

    // TAR 449890:  The only devices that do not have the coin media low
    //              sensor are on 3x hardware.  4.5 does not support 3x so
    //              there should no longer be a need to show the 
    //              FILLCOINDISPENSER button.
#if 0  
                  
   // TAR 140515 - If Credit/Debit only then don't show the button.
   // TAR 267542 - If the currency map is empty, don't show Refill button.
   if ((co.fTenderCashAllowed 
            || bNeedCashBack) 
            && !bCashDispenserOnly && !dm.CashChangerGetCurrencyMap().IsEmpty()) 	
	
   {
      ps.SetButtonText(_T("SMButton2"), BTT_FILLCOINDISPENSER);
    }
#endif

    // +TAR 449890:  Set state of Cash Management button.
    bool bCMButtonEnable = !(co.fLocaleCashManagementDisabledInTransaction && 
                             m_cmMgr.GetInTransaction()); //TAR 450629

    // If no transaction in progress, then check if CM is available.
    if(bCMButtonEnable)
    {
        // +RFC 445940:
        // If we have arrived here from a startup device error screen, then
        // the CM module may not have been loaded.  Try to do so now.
        if(co.fLocaleCashManagementScreen && !m_cmMgr.IsOpen())
        {
            m_cmMgr.Open();
        }
        // -RFC 445940:

        // Enable if external CM is loaded.
        bCMButtonEnable = m_cmMgr.IsOpen();
    }
    ps.ButtonEnable(_T("SMButton2"), bCMButtonEnable);
    // -TAR 449890
	    
	//TAR 452490
    if (!IsCashDeviceConfigured())
    {
		COleVariant vValue;
	    vValue = VARIANT_FALSE;
        m_pAppPSX->SetConfigProperty(_T("SMButton2"), _T("SmMediaStatus"), UI::PROPERTYVISIBLE, vValue);
    }
	//-TAR 452490

	//+4.2 - Card Dispenser
	//if ManageCardDispenser = Y then display Card Manage Button 
	if ( co.IsManageCardDispenser() )
	{
		ps.SetButtonText(_T("SMButton4"), MSG_CARDMANAGEMENT);
	}else
	{
	    ps.SetButtonText(_T("SMButton4"), PS_NOTEXT);
	}
	//-4.2 - Card Dispenser
	
    ps.ShowSMTBText(csTBMessage2Scot);	//thp
    ps.SMShowMediaList();
    ps.ShowFrame();

    return STATE_NULL;							  
}


//===================================================================
// initializeHopperList
//   Add a line in the media list for each hopper in error (if a hopper is
//   ok, no message will be displayed).
//===================================================================
void SMSmMediaStatusBase::initializeHopperList()
{
  //------------------------------------------------------
  // Cash Changer Status
  //------------------------------------------------------
  CString currencyMap, changerStatusList, hopperStatusLine;
  int     semicolonIndex = 0;
	
  //
  // Get a list of currency denominations in use from the currency map, and
  // the corresponding status for each.
  // Example of a GetCashChangerCurrencyContainerMap string for US currency is:
  //
  // "USD:1,USD:5,USD:5,USD:25;USD:100,USD:500,USD:1000,USD:2000"
  //
  // The corresponding CashChangerLowIndicatorList (should be called
  // GetCashChangerStatusString) might look like:
  //
  // "0,0,0,1;0,1,2, 0" - this indicates quarters and fives are low, tens are out.
  // "3,2,2,0;9,9,9,9" - this indicates the penny hopper is jammed, nickels are out and
  //                     the currency dispenser has a transport jam.
  //
  currencyMap = dm.CashChangerGetCurrencyMap();
  changerStatusList = dm.CashChangerGetLowIndicators();
	
  bool bHeaderDisplayed = false;

  if ((0 == currencyMap.Find(_T(';')) ) && ( 0 == changerStatusList.Find(_T(';')) ))
  {
     // This is a cash dispenser only system with no coins
     bCashDispenserOnly = true;
  }  
  // The currencyStatusLine will get filled in. currencyMap and
  // changerStatusList will get truncated from the front as we go
  // through the list, until they're empty.
  while ( getNextHopperStatusLine(currencyMap, changerStatusList, hopperStatusLine) )
  {
      if (!hopperStatusLine.IsEmpty()) //TAR#134928 Don't display a line if its OK
      {
          if (!bHeaderDisplayed) //TAR#121132 Only display header line if cash changer units are found
          {
              ps.AddMediaList(ps.GetPSText(TXT_UNITS_STATUS));      
              bHeaderDisplayed = true;
          }
          ps.AddMediaList(hopperStatusLine);
      }
  }
}


//===================================================================
// initializeCashAcceptorStatusLine
//   Add a line in the media list for the cash acceptor's current status.
// Moved the implementation to base class. awk.11.20.2
//===================================================================
void SMSmMediaStatusBase::initializeCashAcceptorStatusLine()
{
  CString acceptorStatusLine;
  CString csWork;
  long lCode;

  if (co.fTenderCashAllowed)
  {
    lCode = dm.GetCashAcceptorDeviceStatus();

    // Sure, it SAYS it's OK, but it's not to be trusted!
    // (i.e. not all errors reported by above function are correct at this time.)

    if (lCode == BILL_STATUS_OK) //SR672
	{
		lCode = lastDMCashAcceptorCode;
	}
  }
  getCashAcceptorStatusLine(lCode, acceptorStatusLine);
  ps.AddMediaList(acceptorStatusLine);

}

//===================================================================
// initializeCoinAcceptorStatusLine
//   Add a line in the media list for the coin acceptor's current status.
// Moved the implementation to base class. awk.11.20.2
//===================================================================
void SMSmMediaStatusBase::initializeCoinAcceptorStatusLine()
{
  //------------------------------------------------------
  // Coin Acceptor Status
  //------------------------------------------------------
  CString acceptorStatusLine;
  CString csWork;
  long lCode;

  if (co.fTenderCashAllowed) // check if coin acceptor installed
  {
    lCode = dm.GetCoinAcceptorDeviceStatus();

    // If the current status is inoperable (according to the DM property)
    // then it could be for a lot of reasons.  The Coin Acceptor spec says
    // it will set the property to inoperable for lots of things, but the last
    // status event will tell us what the real reason is.  So we will get the
    // last event.
    //
	
	if (lCode == COIN_STATUS_INOPERABLE) //SR672
    {
      lCode = lastDMCoinAcceptorCode;
    }
  }

  getCoinAcceptorStatusLine(lCode, acceptorStatusLine);
  ps.AddMediaList(acceptorStatusLine);
}



//===================================================================
// Button 1 - Update
//===================================================================
SMStateBase  *SMSmMediaStatusBase::PSButton1(void)
{
  //sscoadk-6282: disable other buttons during checkhealth
  ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYENABLECONTROLS, VARIANT_FALSE);
  bIsButtonOnePressed = true; //TAR225069 CHU/DALE
  //TAR# 144557 green light blinking at the wrong time LPM061600
    if(! dm.m_csCashRecyclerModel.IsEmpty() )    // If this is a cash recycler.
    {
        DMCheckHealth(DMCLASS_CASHRECYCLER);
    }
    else
    {
        DMCheckHealth(DMCLASS_CASHCHANGER);
        DMCheckHealth(DMCLASS_CASHACCEPTOR);
        DMCheckHealth(DMCLASS_COINACCEPTOR);
    }

  //TAR# 180401 if the printer is busy, don't perform checkhealth on it
  /******cm150000: Removed In Merged Code******/
	//if (! rq.nJobsInQueue())
	/******cm150000: End Removed Merged Code*/
  DMCheckHealth(DMCLASS_RECEIPT);
  DMCheckHealth(DMCLASS_JOURNAL);
  UpdateDeviceStatus();
  //sscoadk-6282: reenable buttons after checkhealth
  ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYENABLECONTROLS , VARIANT_TRUE);
  return STATE_NULL;
}


//===================================================================
// Button 2 - Refill Coin Dispenser OR Cash Management
//===================================================================
SMStateBase  *SMSmMediaStatusBase::PSButton2(void)
{
    TRACE_METHOD(_T("SMSmMediaStatusBase::PSButton2"));

    // +TAR 449890
    if( co.fLocaleCashManagementScreen )
    { 
        assert(m_cmMgr.IsOpen());   // Otherwise button should be disabled.
        RETURNSTATE(SmCashManagementEx); //SR700
    }

    // TAR 449890:  Removed coin refill logic.  3x hardware is not supported
    //              in this release.
    assert(false);        // Button should have been disabled!
    // -TAR 449890
    
    return STATE_NULL;
}


/////////////////////////////////////////////
// Button 4 Card Management
/////////////////////////////////////////////
SMStateBase  *SMSmMediaStatusBase::PSButton4(void)
{
	m_bIsButtonFourPressed = true;	
    RETURNSTATE(SmCashierPassword);
    
    return STATE_NULL;
}

/////////////////////////////////////////////
// Button 5 Blank
/////////////////////////////////////////////
SMStateBase  *SMSmMediaStatusBase::PSButton5(void)
{
    return STATE_NULL;
}

/////////////////////////////////////////////
// Button 6 Blank
/////////////////////////////////////////////
SMStateBase  *SMSmMediaStatusBase::PSButton6(void) // 
{	
    return STATE_NULL;
}

/////////////////////////////////////////////
// Button 7 Blank
/////////////////////////////////////////////
SMStateBase  *SMSmMediaStatusBase::PSButton7(void)
{
    return STATE_NULL;
}
#endif
//===================================================================
// Button 8 - Exit
//===================================================================
SMStateBase  *SMSmMediaStatusBase::PSButton8(void)
{
#ifndef _CPPUNIT
    //sscoadk-6282: disable other buttons during checkhealth
    ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYENABLECONTROLS, VARIANT_FALSE);
  // Tar 160002 Reset trilight
    if(! dm.m_csCashRecyclerModel.IsEmpty() )    // If this is a cash recycler.
    {
        DMCheckHealth(DMCLASS_CASHRECYCLER);
    }
    else
    {
        DMCheckHealth(DMCLASS_CASHCHANGER);
        DMCheckHealth(DMCLASS_CASHACCEPTOR);
        DMCheckHealth(DMCLASS_COINACCEPTOR);
    }

  //TAR# 180401 if the printer is busy, don't perform checkhealth on it
	/******cm150000: Removed In Merged Code******/
  //if (! rq.nJobsInQueue())
	/******cm150000: End Merged Code*************/
  DMCheckHealth(DMCLASS_RECEIPT);
  DMCheckHealth(DMCLASS_JOURNAL);
  UpdateDeviceStatus();
#endif
  if (m_bCancellingCashTender && co.IsInAssistMenus())
  {
      trace(L6,_T("SmMediaStatusBase::PSButton8 - In assistmenu, cancelling cash tender. Go to TakeChange"));
      RETURNSTATE(TakeChange) 
  }
//#ifndef _CPPUNIT
  //Return to current store mode menu state instead of SmAuthorization
  return STATE(StoreModeMenu)::CreateStoreModeMenu();
//#endif

}

#ifndef _CPPUNIT
void SMSmMediaStatusBase::initializeCurrencyList()
{
    CString csHighCurrencies, csCoinDispenserAlert, csCashDispenserAlert;
    CString csLowCurrencies = DMCheckCashCounts(false, csHighCurrencies, csCoinDispenserAlert, csCashDispenserAlert);
    int nStrIndex;
    bool bDisplayedHeader = false;

    trace(L7, _T("Low Currencies : %s. High Currencies : %s."), csLowCurrencies, csHighCurrencies);
    trace(L7, _T("Coin Dispenser: %s. Cash Dispenser: %s."), csCoinDispenserAlert, csCashDispenserAlert);
	
    // (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_ON,
                       DM_TRICOLORLIGHT_NORMALREQUEST);

	//TAR200557 if media errors and recently told RAP about them, set bToldRAPMediaLow to false and tell RAP normal media
	if (!dm.GetCoinsOrBillsVeryLow() && !dm.GetPrinterPaperLow() && bToldRAPMediaLow)
	{
		bToldRAPMediaLow = false;
		// tell rap
		trace(L7, _T("+ra.OnMediaLow, Message=%s"), ps.GetPSText(RA_MEDIA_NORMAL_ID));
		ra.OnMediaLow(ps.GetPSText(RA_MEDIA_NORMAL_ID, SCOT_LANGUAGE_PRIMARY), false); 
		trace(L7, _T("-ra.OnMediaLow"));
	}

    bool bCurrencyNormal = true; //dp185016 support glory device - support currency is high


    CString *csFirstGroup, *csSecondGroup;
    // grouped by low, and high currencies, OR
    // grouped by coin dispenser and cash dispenser
    PSTEXT psFirstMsg, psSecondMsg;
    
    if( dm.m_csCashRecyclerModel.IsEmpty() == FALSE || 
        co.fLocaleCashManagementScreen  // SSCOP-896
      )
    {
      csFirstGroup = &csCoinDispenserAlert;
      csSecondGroup = &csCashDispenserAlert;
      psFirstMsg = MSG_COIN_DISPENSER_STATUS;
      psSecondMsg = MSG_CASH_DISPENSER_STATUS;
    }
    else
    {
      csFirstGroup = &csLowCurrencies;
      csSecondGroup = &csHighCurrencies;
      psFirstMsg = MSG_CURRENCYLOWSTATUS;
      psSecondMsg = MSG_CURRENCYHIGHSTATUS;

    }
    if (!csFirstGroup->IsEmpty())
      { 
          while (!csFirstGroup->IsEmpty())
          { 
              if (!bDisplayedHeader)
              {
                  ps.AddMediaList(ps.GetPSText(psFirstMsg));
                  bDisplayedHeader = true;
              }

              nStrIndex = csFirstGroup->Find(_T('\n'));
              if (nStrIndex == -1)
              {
                  ps.AddMediaList(*csFirstGroup);
                  csFirstGroup->Empty();
              }
              else
              {
                  ps.AddMediaList(csFirstGroup->Left(nStrIndex));
                  *csFirstGroup = csFirstGroup->Mid(nStrIndex+1);
              }
          }
          bCurrencyNormal = false;
      }

      bDisplayedHeader = false;
      if (!csSecondGroup->IsEmpty())
      { 
          while (!csSecondGroup->IsEmpty())
          { 
              if (!bDisplayedHeader)
              {
                  ps.AddMediaList(ps.GetPSText(psSecondMsg));
                  bDisplayedHeader = true;
              }

              nStrIndex = csSecondGroup->Find(_T('\n'));
              if (nStrIndex == -1)
              {
                  ps.AddMediaList(*csSecondGroup);
                  csSecondGroup->Empty();
              }
              else
              {
                  ps.AddMediaList(csSecondGroup->Left(nStrIndex));
                  *csSecondGroup = csSecondGroup->Mid(nStrIndex+1);
              }
          }
          bCurrencyNormal = false;
      }


   if( bCurrencyNormal )
      {ps.AddMediaList(ps.GetPSText(MSG_CURRENCYALLNORMAL));}
    //-dp185016
}

//===================================================================
// initializeCardDispenserStatus()
//   Add a line in the media list for card dispenser status
//===================================================================
void SMSmMediaStatusBase::initializeCardDispenserStatus(void)
{
	//lCountBin - number of cards in the bin
	//lCountRect - number of cards in the reject bin
	long lCountBin = 0,lCountReject = 0;
	CString csWork, csPerCentage;
	double nPercentage = 0;

	//show percentage if ManageCardDispenser = Y 
	if ( co.IsManageCardDispenser() )
	{
		dm.GetAdjustCardDispenserBinCounts(lCountBin, lCountReject, 0);
		nPercentage = ((double)lCountBin/co.getCardDispenserBinCapacity()) * 100;
		csPerCentage.Format( _T("%.1f%%"), nPercentage );
	}
	ps.AddMediaList( ps.GetPSText( MSG_CARDDISPENSER_STATUS ) );

	if ( dm.GetCardDispenserEmpty() ) 
	{
		ps.AddMediaList(ps.GetPSText( MSG_CARDDISPENSER_BIN_EMPTY));
	}
	else
	{
		if ( dm.GetCardDispenserVeryLow() )
		{
			
			if ( co.IsManageCardDispenser() ) 
			{
				csWork.Format(ps.GetPSText( MSG_CARDDISPENSER_BIN_LOW_PERCENTAGE ), csPerCentage );
				ps.AddMediaList(csWork );
			}else
			{
				ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_BIN_LOW));
			}
		}else
		{
			if ( co.IsManageCardDispenser() ) 
			{
				csWork.Format( ps.GetPSText( MSG_CARDDISPENSER_BIN_NORMAL_PERCENTAGE ), csPerCentage );
				ps.AddMediaList(csWork );
			}else
			{
				ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_BIN_NORMAL));
			}
		}
	}
	//-dispnser bin
	//+reject bin
	csPerCentage = _T("");
	nPercentage = 0;
	//show percentage if ManageCardDispenser = Y
	if ( co.IsManageCardDispenser() )
	{
		nPercentage = ((double)lCountReject/co.getDispenserRejectBinFull()) * 100;	
		csPerCentage.Format( _T("%.1f%%"), nPercentage );
	}
	if ( 0 == lCountReject )
	{
		ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_REJECT_EMPTY));
	}else
	{
		if ( lCountReject == co.getDispenserRejectBinFull() )
		{
			ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_REJECT_FULL) );
		}else
		{
			if ( lCountReject >= co.getDispenserRejectBinNearFull() )
			{
				if ( co.IsManageCardDispenser() ) 
				{
					csWork.Format(ps.GetPSText(MSG_CARDDISPENSER_REJECT_NEAR_FULL_PERCENTAGE), csPerCentage);
					ps.AddMediaList(csWork );
				}else
				{
					ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_REJECT_NEAR_FULL) );
				}					
			}else
			{
				if ( co.IsManageCardDispenser() ) 
				{
					csWork.Format(ps.GetPSText(MSG_CARDDISPENSER_REJECT_NORMAL_PERCENTAGE), csPerCentage);
					ps.AddMediaList(csWork );
				}else
				{
					ps.AddMediaList(ps.GetPSText(MSG_CARDDISPENSER_REJECT_NORMAL) );
				}					
			}
		}
	}
	//-reject bin
}
//===================================================================
// initializePrinterList()
//   Add a line in the media list for the each printer's current status
//   (receipt & journal).
//===================================================================
void SMSmMediaStatusBase::initializePrinterList()
{
  //------------------------------------------------------
  // Printer Status
  //------------------------------------------------------
  CString csWork1,csWork2;
  if (!dm.GetPrinterPaperLow()) //tar 251305
  {
	  getPrinterStatusLine(lastDMReceiptCode,csWork1);
	  csWork2 = ps.GetPSText(TXT_RECEIPT_PRINTER);
	  csWork2 += _T(": ");
	  csWork2 += csWork1;
	  ps.AddMediaList(csWork2);
  }
  else //tar 251305
  {
    // 206331 LPM lane light not blinking after going into media status even if printer is low
    
	// (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);  
                       
	getPrinterStatusLine(PTR_SUE_REC_NEAREMPTY,csWork1); //SR672
    csWork2 = ps.GetPSText(TXT_RECEIPT_PRINTER);
    csWork2 += _T(": ");
    csWork2 += csWork1;
    ps.AddMediaList(csWork2);
  }

  //TAR199516
  if (dm.IsDeviceConfigured( DMCLASS_JOURNAL, 0) )
  {
		getPrinterStatusLine(lastDMJournalCode,csWork1);
		csWork2 = ps.GetPSText(TXT_JOURNAL_PRINTER);
		csWork2 += _T(": ");
		csWork2 += csWork1;
	  ps.AddMediaList(csWork2);
  }
}

//===================================================================
// UpdateDeviceStatus()
//===================================================================
void SMSmMediaStatusBase::UpdateDeviceStatus()
{
   // First delete all list entries.
   ps.ResetMediaList();

   if (IsCashDeviceConfigured()) //TAR 452490
   {
	  initializeCurrencyList();
   }
   initializeHopperList();
   initializePrinterList();
   //initializeCardDispenserStatus();  //4.2 - Card Dispenser
   //jrparro TAR#321829 - check if ManageCardDispenser = Y 
   if(co.IsManageCardDispenser())
   {
	  initializeCardDispenserStatus();  //4.2 - Card Dispenser
   }

   //dp185016 tar 299298
   if(dm.m_csCashRecyclerModel.IsEmpty() && IsCashDeviceConfigured())//TAR 452490
   {
	   initializeCashAcceptorStatusLine();
     if (dm.IsCoinAcceptorAvailable())
	 {
		 initializeCoinAcceptorStatusLine();
	 }
   }
	//If enhanced coin low sensor is not available, display Refill information
   if(m_nCoinDispenserWithLowSensor != 1 
      && !co.fLocaleCashManagementScreen && IsCashDeviceConfigured() && dm.IsCoinAcceptorAvailable() ) //TAR431544, 452490
   {
	   InitializeCoinDispenserStatus();
   }

   //dp185016 tar 299310
   COleVariant vReturn, vScrollMode;
   vScrollMode = (long) UI::ScrollTop;// SSCOP-699: Set receiptlist's view from top to bottom by default.
   m_pAppPSX->SendCommand(_T("MediaList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);

}

//===================================================================
// DisplayCoinDispenserStatus() - TAR 140515 
//===================================================================
CString SMSmMediaStatusBase::DisplayCoinDispenserStatus(void)
{
  // Get current time
  COleDateTime dtNow;
  dtNow = COleDateTime::GetCurrentTime();

  // Format string to display date, time.
  // No room on screen for the year, so display "mm/dd" for date
  CString csDate, csTime, csWork;
  csDate = dtNow.Format( _T("%m/%d") );	
  csTime = dtNow.Format( _T("%H:%M") );	
  
  //Tar 228718
  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  csWork.Format(ps.GetPSText(MSG_FILLCOINDISPENSER, SCOT_LANGUAGE_PRIMARY), csDate, csTime);

  // If there is not an entry for coin dispenser, then add it. Otherwise, update it.
  if (csDateCoinDispenserRefilled.GetLength() == 0 && !bReadDataFromRegistry)
  {
    ps.AddMediaList(csWork);
	// Display status, then update values saved.
    csDateCoinDispenserRefilled = csDate;	
    csTimeCoinDispenserRefilled = csTime;
  }
  else
  {
    csDateCoinDispenserRefilled = csDate;
    csTimeCoinDispenserRefilled = csTime;
    // Update values saved before display status.   
    UpdateDeviceStatus();
  }

  // Saved data to registry for next time when SCOTAPP is restarted.
  HKEY  hKey;

  // If the entry does not exist, it will create one.
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP"), 
       0, NULL,REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&hKey, NULL) == ERROR_SUCCESS)
  {
	  // Write a string value to the registry.
     RegSetValue(hKey,
	              TEXT("LastCoinRefilled"), 
                   REG_SZ,
				   csWork,
                   csWork.GetLength() + 1);
  }
  RegCloseKey(hKey);
  return csWork;
}

//===================================================================
// InitializeCoinDispenserStatus() - TAR 140515 
//===================================================================
void SMSmMediaStatusBase::InitializeCoinDispenserStatus(void)
{
  // If we have data that coin dispenser was refilled, then display it.
  // Otherwise, get data from the registry.
  if (csDateCoinDispenserRefilled.GetLength() == 0)
  {
    HKEY  hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\LastCoinRefilled"), 
        0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
	{
	   _TCHAR csDataSaved[100];
       DWORD dwDataSize = sizeof (csDataSaved), dwDataType;
       if(RegQueryValueEx(hKey, NULL, 0, &dwDataType, 
          (LPBYTE)csDataSaved, &dwDataSize) == ERROR_SUCCESS)
       {
	     ps.AddMediaList(csDataSaved);
         bReadDataFromRegistry = true;
       }
	}
    RegCloseKey(hKey);
  }
  else
  {
    CString csWork;
	//Tar 228718
    csWork.Format(ps.GetPSText(MSG_FILLCOINDISPENSER, SCOT_LANGUAGE_PRIMARY), csDateCoinDispenserRefilled,
           		  csTimeCoinDispenserRefilled);
	ps.AddMediaList(csWork);
  }
  return;
}


////////////////////////////////////////
// NewUI
SMStateBase  *SMSmMediaStatusBase::PSReceiptUp(void)
{
//  COleVariant vReturn, vScrollMode;
//  vScrollMode = (long) UI::PageUp;
//  m_pAppPSX->SendCommand(_T("MediaList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);
  return STATE_NULL;
}
SMStateBase  *SMSmMediaStatusBase::PSReceiptDown(void)
{
//  COleVariant vReturn, vScrollMode;
//  vScrollMode = (long) UI::PageDown;
//  m_pAppPSX->SendCommand(_T("MediaList"), UI::COMMANDSCROLL, vReturn, 1, vScrollMode);  return STATE_NULL;
  return STATE_NULL;
}
// NewUI

//TAR225069+ CHU/DALE
SMStateBase *SMSmMediaStatusBase::DMCashChangerInOperable()
{ 

   if ( !bIsButtonOnePressed )
	   return STATE(StoreModeState)::DMCashChangerInOperable();

   //Add the message to receipt instead of going to SystemMessage
   CString csWork;

   //SR672
   csWork.Format(ERRFMT, ps.GetPSText(MSG_CC_STATUS_INOPERABLE_EX,SCOT_LANGUAGE_PRIMARY),                     \
                  DMCLASS_CASHCHANGER, CASH_STATUS_INOPERABLE);      

   ps.AddMediaList(csWork);

   return STATE_NULL;

}





// + RFC 330050
void SMSmMediaStatusBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmMediaStatus") );	

	dm.CashRecyclerPauseCounts(false);	// SSCOP-2197
}
// - RFC 330050

long SMSmMediaStatusBase::DMCheckHealth(long lDevClass)
{
	if(lDevClass <= DMCLASS_PRINTER)
	{
        trace(L6,_T("+SMSmMediaStatusBase::DMCheckHealth() - DevClass: %d"),lDevClass);
		long rc = dm.CheckHealth(lDevClass);

		if (!rc) // rc is zero on success
		{
			// clear the Intervention related to this device error
			// Begin TAR 192405, 192461
			trace(L6, _T("ra.OnNoRAPDataNeeded()"));
			ra.OnNoRAPDataNeeded();	
			// End TAR 192405, 192461
		}
		trace(L6,_T("-SMSmMediaStatusBase::DMCheckHealth() - rc: %d"), rc);
		return rc;
	}
	else
	{
		return STATE(StoreModeState)::DMCheckHealth(lDevClass);
	}
}

void SMSmMediaStatusBase::getHopperStatus(const CString denominationCode,
                                          CString& denominationStatus)
{
    // SSCOP-896:
    // If this is not an EMPTY condition OR if cash management is not enabled
    // OR if the system has a TQ HOPPER dispenser, then use base version.
    //
    // We can't change the base version because it is also used in 
    // SystemMessage error display.
    if(*denominationCode != _T('2') ||
       !co.fLocaleCashManagementScreen ||
       ( dm.m_csCoinDispenserModel.Find(_T("HOPPER")) != -1 && 
        !dm.m_csCoinDispenserStage.IsEmpty()) )
    {
        STATE(StoreModeState)::getHopperStatus(denominationCode,
                                               denominationStatus);
        return;
    }

    // For the media status screen with CM enabled, we don't want to include
    // EMPTY info because this will be provided by DMCheckCashCounts().
    assert(*denominationCode == _T('2'));
    assert(m_cmMgr.IsOpen());

    denominationStatus.Empty();
}

/**
 * SSCOP-4169:  Consolidate reload of cash devices into one function.
 *              Pay attention to success or failure of the reload.
 *              Return an error state if the reload fails.
 */
SMStateBase * SMSmMediaStatusBase::ReloadCashDevices(void)
{
    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHACCEPTOR,0))
    {
        if(dm.ReloadCashAcceptor() != OPOS_SUCCESS)
        {
            return GetUnloadedCashAcceptorError();
        }
    }

    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_COINACCEPTOR,0))		
    {
        if(dm.ReloadCoinAcceptor() != OPOS_SUCCESS)	
        {
            return GetUnloadedCoinAcceptorError();
        }
    }

    if (dm.IsDeviceConfiguredNotLoaded(DMCLASS_CASHCHANGER,0))
    {
        if(dm.ReloadCashChanger() != OPOS_SUCCESS)
        {
            return GetUnloadedCashChangerError();
        }
    }

    return STATE_NULL;      // Everything reloaded OK.
}
#endif