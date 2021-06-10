#include <stdafx.h>
#include <afxtempl.h>
#include <queue>

#ifndef _CPPUNIT
#include "Common.h"
#include "SMState.h"
#include "SMContinueTrans.h"
#include "SMScanAndBag.h"
#include "SMHelpOnWay.h"
#include "SMRapDataNeeded.h"
#include "SMSecUnExpectedIncrease.h"
#include "SMSecMisMatchWeight.h"
#include "MPProcedures.h"
#include "SMWaitForApproval.h"
#include <math.h>
#else
#include "FakeCommon.h"
#endif

#include "SMRecallingItemsBase.h"

#define COMP_ID ID_SM
#define T_ID    _T("RecallingItemsBase")



DLLEXPORT extern HWND hw;           
#ifndef _CPPUNIT
DLLEXPORT extern MPProcedures mp;   // RFC 276235
#endif //CPPUNIT

CString SMRecallingItemsBase::m_Context = _T("");
bool SMRecallingItemsBase::g_bWaitingForScale = false;
bool SMRecallingItemsBase::g_bPressedDone = false;
long SMRecallingItemsBase::g_nTimeoutRecallingWaitForDone = 0;


IMPLEMENT_DYNCREATE(SMRecallingItemsBase, CObject)

DEFINE_TIMESTAMP

SMRecallingItemsBase::SMRecallingItemsBase()
{
    STATE(InProgress)(false, false, 100);
    g_bWaitingForScale = false;
    g_bPressedDone = false;

    IMPLEMENT_TIMESTAMP
}

SMStateBase * SMRecallingItemsBase::Initialize()
{
    trace(L6, _T("+SMRecallingItemsBase::Initialize()"));
    trace (L6, _T("GetPreferredLanguage() = [%d]"), GetPreferredLanguage());
    trace (L6, _T("ps.GetLanguage() = [%d]"), ps.GetLanguage());
    trace (L6, _T("m_LanguageUsedPreviously = [%d]"), m_LanguageUsedPreviously);
    trace (L6, _T("m_customerLanguage = [%d]"), m_customerLanguage);
    trace (L6, _T("fStateFreeze = [%d]"), fStateFreeze);

	if(ImmediateInterventionStack.GetCount()>0) // Clear the security intervention if item is placed on scale at Attract .
    { 
       OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR, KEY_EXCEPTION_CATEGORY_IMMEDIATE_WT_CHANGES); 
       trace (L6, _T("clear all weight exceptions if any")); 
    }

    InitLanguageInfo();
      
    SMStateBase * retState = STATE_NULL;
    ShowRecallingScreen();
    //SMStateBase * retState = STATE(InProgress)::Initialize();
    if ( retState != STATE_NULL ) {
        delete getRealStatePointer( retState );
        retState = STATE_NULL;
    }
    
    if(g_bInRecallMode)
    {
        nTimeOut = 0;
        trace(L6, _T("In recall, set timeout to %d"), nTimeOut);
    }
    else
    {
        nTimeOut = g_nTimeoutRecallingWaitForDone;
        trace(L6, _T("Out of recall, set timeout to %d"), nTimeOut);
    }
    
    if(g_bInRecallMode && !fSAInItemSale)
    {
        if ( !recallList.ParseNextItem(true))  //No More Items
        { 
            FinishRecall();
            return STATE_NULL;
            
        }
        
        // Sell the first item in the recall item queue or the next one if
        // we've just answered an immediate intervention.
        CString LastNonHelpStateName = this->csLastNonHelpStateName;
        if ( (LastNonHelpStateName == _T("SMScanAndBag") ||
            LastNonHelpStateName == _T("SMAttract") ) )//TAR427016
        {
            retState = SellRecalledItem();
        }
        
        // Throw the SMInProgress state away and keep us in SMRecallingItemsBase.
        // Otherwise, return the state TBItemSale() returned to handle
        // exceptions.
        if ( retState != STATE_NULL ) {
            CString csNextStateName =
                retState->GetRuntimeClass()->m_lpszClassName;
            if ( csNextStateName.Find(_T("InProgress")) != -1 ) {
                delete getRealStatePointer( retState );
                retState = STATE_NULL;
            }
        }
    }
    
    if (co.fOperationsDisplayQuickPickItems)
    {
        ps.DisplayQuickPickItem();
    }
    return retState;
}

void SMRecallingItemsBase::InitLanguageInfo(void)
{
    SCOT_LANGUAGE_TYPE customerLanguage; 
    customerLanguage = ps.GetLanguage();

    if (co.GetfStateDualLanguage())
    {
        m_customerLanguage = customerLanguage;
        ps.SetLanguage(customerLanguage);
        co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
        CustomerMessage = co.csCMCustomerMessage;
        
        ps.SetTareListToCurrentLanguage();
        
        if(m_LanguageUsedPreviously != m_customerLanguage)
        {
            m_LanguageUsedPreviously=m_customerLanguage;        
        }
    }
}

void SMRecallingItemsBase::ShowRecallingScreen()
{
    CString csContext = ps.GetCurrentContext();
    
    trace(L6, _T("ShowRecallingScreen %d, %s"), g_bEnteringRecallMode, csContext );
    
    if (g_bEnteringRecallMode || (csContext.Compare(_T("Processing")) != 0) )
    {
        g_bEnteringRecallMode = false;
        ps.SetCMFrame(_T("Processing"));
        ps.SetCMLeadthruText(TTL_LOADING_TRANSACTION);
        ps.Message12(MSG_LOADING_TRANSACTION, true);
        ps.ShowCMTBText(csTBMessage2Scot);  
        ps.CMButton(_T("ButtonHelp"),BTT_HELP,false);
        UpdateStateName(ps.GetPSText(MSG_LOADING_TRANSACTION, SCOT_LANGUAGE_PRIMARY));

        ps.ShowFrame();
    }
}


/// FinishRecall
/**
Once the recall list is empty,  set all necessary flags to exit recalling.
The state then waits for the "Done" button to be pressed.
*/
void SMRecallingItemsBase::FinishRecall()
{

    trace(L6, _T("+SMRecallingItemsBase::FinishRecall"));
    nTimeOut = g_nTimeoutRecallingWaitForDone;
    
    g_bInRecallMode = false;

    SMStateBase *tempState = TBGetTotalDetails();  //Woodmans item processing needs Total between item sales
    delete getRealStatePointer(tempState);
    UpdateTotal();
    rp.ModeChange(rp.m_lAttendantEntryID, _T("RECALLMODE"), EXIT);
    ps.SetCMFrame(_T("CmDataEntry1"));
    trace(L7, _T("+ra.OnNormalItem, Message=%s"), ps.GetPSText(MSG_RECALLMODE_END, SCOT_LANGUAGE_PRIMARY));
    CString csItemsSold;
    csItemsSold.Format(ps.GetPSText(MSG_NUMBER_ITEMS_RECALLED, SCOT_LANGUAGE_PRIMARY), lItems);
    ra.OnNormalItem(RAP_RECEIPT_LINE_SUMMARY + ps.GetPSText(MSG_RECALLMODE_END, SCOT_LANGUAGE_PRIMARY) +
        _T("\t\t  ") + csItemsSold); // inform RAP about entering/leaving recall mode
    trace(L7, _T("-ra.OnNormalItem"));
    trace(L6, _T("+SMRecallingItemsBase::FinishRecall, Finished recall, waiting for Done button"));
    ps.SetCMLeadthruText(TTL_RECALL_COMPLETE);
	ps.Message12(MSG_RECALL_PLEASE_SCAN_MORE_ITEMS, true);
    ps.ShowCMTBText(csTBMessage2Scot);  
    UpdateStateName(ps.GetPSText(MSG_LOADING_TRANSACTION, SCOT_LANGUAGE_PRIMARY));
    ps.Button(_T("CMButton1MidiList"), BTT_OK, true);
    ps.ShowFrame();
    SendTBCommand(TBCMD_CMDSTRING,CString(_T("SetRecallingModeFalse")));

}

SMStateBase * SMRecallingItemsBase::PSButton1(void)
{
    g_bPressedDone = true;
    g_bWaitingForScale = true;
    trace(L6, _T("SMRecallingItemsBase::PSButton1, Done pressed, g_bWaitingForScale = %d"), g_bWaitingForScale);
    SetTimer(hw, RAP_ESCALATION, co.getTimeOutBagItemEscalation(), &EscalationTimedOut);
    nTimeOut = 1;

    CKEYVALUEPARMLIST parmList;
    CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_EXIT_RECALLMODE);
    _bstr_t bstrParmList((const _TCHAR *)parmList);
    BSTR bstrResult;
    bstrResult = NULL;
    SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
    SysFreeString(bstrResult);
    trace(L6, _T("-SMRecallingItemsBase::PSButton1"));
    //   RestoreSBButtonStates();
    ra.RequestEnableAssistMode(true);
    mp.LogTransition((CString)_T("SMEndMobileReplay"), nTBLastRealState); 
    return STATE_NULL;
}



SMStateBase *SMRecallingItemsBase::TimedOut(void)
{
    trace(L6, _T("SMRecallingItemsBase::TimedOut, g_bInRecallMode = %d, g_bWaitingForScale = %d"), g_bInRecallMode, g_bWaitingForScale);
    if ( !g_bInRecallMode ) 
    {
        if (g_bPressedDone && !g_bWaitingForScale)
        {
            //If we've given up on waiting for the customer to something on the scale after "Done",
            // send SecurityManager a "Item Expired" message to trigger processing.
            CKEYVALUEPARMLIST parmList;
            setAnchorState(BES_SCANANDBAG);
            CKEYVALUE<const _TCHAR*>::Put(parmList, KEY_OPERATION, KEY_AC_ITEM_EXPIRED);
            g_bMatchedWtWaitingOnEAS = false;
            g_bWtExpected = false;  //We should never expect wt if button is pressed on RAP
            _bstr_t bstrParmList((const _TCHAR *)parmList);
            BSTR bstrResult;
            bstrResult = NULL;
            SecMgr.AppControl(lSecMgrCookie, bstrParmList, &bstrResult);
            SysFreeString(bstrResult);

            return STATE_NULL;
        }
        else if (!g_bPressedDone)
        {
            return PSButton4();  //Automatically press the "Done" button
        }
        else
        {
            return STATE_NULL;
        }
    } else {
        // Oops, must be out of sync. Panic!
        return STATE(InProgress)::TimedOut();
    }

}


/**
* \brief Override to handle KEY_OSC_OPERATION_GOTO_SCANANDBAG and SmItemPickFinish message.
* 
*  SASecMgrParse() override will make Scotapp transit to Scan and Bag after SM allows it.
*  
*/
SMStateBase *SMRecallingItemsBase::SASecMgrParse( const MessageElement *me) 
{

    int nEvent = me->secmgrInfo.nEvt;
    trace(L6, _T("SMRecallingItemsBase::SASecMgrParse nEvent = %d"), nEvent);
    LPTSTR szParms;
    szParms = new TCHAR [_tcslen(me->secmgrInfo.szParms) + sizeof(TCHAR)];
    _tcscpy( szParms, me->secmgrInfo.szParms );
    
    CString csParms;
    csParms = szParms;
    delete [] szParms;
    switch (nEvent) {

		case SmOnSecurityException:   
        if ( secEventParms.m_csExceptionType == KEY_OSE_EXC_TYPE_UNEX_INCREASE )
        { 
           trace(L6, _T("At RecallingItem, clear the exception; returning STATE_NULL")); 
           OverrideSecurityException(_T("Lane"), KEY_EXCEPTION_OPERATION_CLEAR); return STATE_NULL; 
        }
    case SmOnSecurityControl:
        if (csParms.Find(KEY_OSC_OPERATION_GOTO_SCANANDBAG) > -1)
        {
            trace(L6, _T("KEY_OSC_OPERATION_GOTO_SCANANDBAG is received"));
            delete [] me->secmgrInfo.szParms;
            if (g_bInRecallMode || g_bPressedDone)
            {
                RETURNSTATE(ScanAndBag);
            }
            else
            {
                return STATE_NULL;
            }

        }
        else if (csParms.Find(KEY_OSE_EXC_TYPE_MISMATCHED) > -1)
        {
            return STATE_NULL;  //Ignore this for now.
        }
        break;
    case SmItemPickFinish:
        delete [] me->secmgrInfo.szParms;
        RETURNSTATE(ScanAndBag);
        break;

    }

    return STATE(InProgress)::SASecMgrParse(me);
    
}

/**
* \brief Override OnSkipBaggingAllowed()
* 
*  OnSkipBaggingAllowed() override will make Scotapp transit to Scan and Bag after SM allows it.
*  
*/
SMStateBase *SMRecallingItemsBase::OnSkipBaggingAllowed()    
{
    trace(L6, _T("+SMRecallingItemsBase::OnSkipBaggingAllowed, g_bInRecallMode = %d, nTimeOut set to %d"), g_bInRecallMode, nTimeOut);
    
    SMStateBase* returnState = STATE_NULL;
    //if (!g_bRecallItemSold)
        //return returnState;
    
//    g_bRecallItemSold = false;
    
    if ( !recallList.ParseNextItem(true) )
    {
        FinishRecall();
    } 
    else
    { // Recall queue not empty. Sell another item.
        
        returnState = SellRecalledItem();
        // Throw the SMInProgress state away and keep us in
        // SMRecallingItemsBase. Otherwise, return the state
        // TBItemSale() returned to handle exceptions.
        if ( returnState != STATE_NULL )
        {
            CString csNextStateName =
                returnState->GetRuntimeClass()->m_lpszClassName;
            if ( csNextStateName.Find(_T("InProgress")) != -1 )
            {
                delete getRealStatePointer( returnState );
                returnState = STATE_NULL;
            }
        }
    }
    trace(L6, _T("-SMRecallingItemsBase::OnSkipBaggingAllowed, g_bInRecallMode = %d, nTimeOut set to %d"), g_bInRecallMode, nTimeOut);
    return returnState;
}

/**
* \brief Override OnBackToLGW.
* 
*  OnBackToLGW() will return STATE_NULL instead of transitioning to ScanAndBag.
*  
*/
SMStateBase *SMRecallingItemsBase::OnBackToLGW(void)
{
    SMStateBase  *pRetState = STATE_NULL;
    
    pRetState = STATE(InProgress)::OnBackToLGW();
    
    delete getRealStatePointer( pRetState);
    return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////
SMStateBase *SMRecallingItemsBase::OnMatchedWt()
{
    
    // +203255 Clear the RAP Intervention created for UnMatched/increase/decrease weight
    if(m_View > DATANEEDED_NOVIEW) //tar 252987
    {
        ClearSecurityViolationLight();
        
        trace(L7, _T("+ra.OnNoRAPDataNeeded for SMStateBase::OnMatchedWt"));
        ra.OnNoRAPDataNeeded();
        trace(L7, _T("-ra.OnNoRAPDataNeeded"));
    }// -203255 End
    
    g_bWtExpected = false;  //We should never expect wt if we got a matched wt.
    g_lNumberOfWLDBEntriesThisItem = -1;    // reset the number of WLDB entries
    
    trace(L2, _T("bEASReEnableScanner is set to true."));
    bEASReEnableScanner = true;
    //-218558 End
    
    
    return setAndCreateAnchorState(BES_SCANANDBAG);
}

///////////////////////////////////////
VOID CALLBACK SMRecallingItemsBase::EscalationTimedOut( HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    g_bWaitingForScale = false;
    KillTimer(hw,RAP_ESCALATION);
}

void SMRecallingItemsBase::UnInitialize(void)
{
    KillTimer(hw,RAP_ESCALATION);
    g_bPressedDone = false;
    g_bWaitingForScale = false;
}
