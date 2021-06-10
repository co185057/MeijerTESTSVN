//////////////////////////////////////////////////////////////////////
//
// SolutionMHTender.cpp: implementation of the CSolutionMHTender class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMHTender.h"
#include "CustomerCurrentTender.h"
#include "customerTBConnectPOS.h"
#include "customerMHOptions.h"
#include "customerTrxBroker.h"
#include "CustomerTBProperties.h"
#include "traceSystem.h"
#include "customerTBConnectFL.h"
#include "tbutilities.h"
#include "BaseMessages.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentTender.h"

#define T_ID    _T("CSolutionMHTender")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionMHTender::CSolutionMHTender()
{
}

CSolutionMHTender::~CSolutionMHTender()
{
}

TBSTATE CSolutionMHTender::HandleTenderEntry(LPARAM lparam)
{
	tbtraceIO(_T("HandleTenderEntry"));

    // don't reset
    //pCurrTender->Reset();
	return TB_IGNORE;
}

// This is required functionality for all MH classes
TBSTATE CSolutionMHTender::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    CCustomerCurrentTender* pCurr = CCustomerCurrentTender::instance();

    CBaseMHTender::ProcessMessage(pMsgObj);

    pCurr->TenderAccepted(true);

    int nTenderType= CCustomerMHOptions::instance()->IntOptionValue(pCurr->TenderDescription());
    if (nTenderType== 0)
        nTenderType=1;
    pCurr->TenderType(nTenderType);   

    if (pCurr->BalanceLong() <= 0)
        pCurr->ChangeDue((long)abs(pCurr->BalanceLong()));

    tbtrace( TM_ENTRY, _T("+-ProcessMessage"));
    return rc;
}

TBSTATE CSolutionMHTender::HandleMessage300(bool bInItemization)
{
    TBSTATE rc= TB_IGNORE;
    // get the hook property response code
    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    CCustomerTBScotMsg *pScotMsg = CCustomerTBScotMsg::instance();
    CCustomerMsgMap *pMsgMap = CCustomerMsgMap::instance();
    CBaseMessages *pMessages = CBaseMessages::instance();

    CString csResponseCode= Trx.TBGetProperty(_T("EftCardResponseCode"), true);
    CString csGuidance= _T("\n") + Trx.TBGetProperty(_T("OG_Guidance_Prop"), true);
    CString csMsgLevel= Trx.TBGetProperty(_T("OG_MsgLevel_Prop"), true);
    
    int iMsgLevel = _ttoi((LPCTSTR)csMsgLevel);

    CCustomerCurrentTender  &CurrTender = *CCustomerCurrentTender::instance();
    CCustomerTBConnectPOS &pos= *CCustomerTBConnectPOS::instance();

    int nRespCode= FindResponseCode(csResponseCode, (TBTENDERTYPE)CurrTender.TenderType());

    tbtrace(TM_INFO, _T("EftCardResponseCode:%s, Guidance:%s, msg_lvl:%s, handled as Code:%d"), 
        (LPCTSTR)csResponseCode, (LPCTSTR)csGuidance, (LPCTSTR)csMsgLevel, nRespCode);

    CurrTender.ErrorDescription( _T("") );
    
   
    CCustomerMHOptions *pCurrOpts = CCustomerMHOptions::instance();
    CString csAckOkKey = pCurrOpts->OptionValue(_T("Acknowledge"));
    CString csCancelKey = pCurrOpts->OptionValue(_T("Cancel"));

    switch (nRespCode)
    {
    
    case 4: // gift card delayed activation
        {   
            rc = TB_IGNORE;
            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
      
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);
        
            if(iMsgLevel == 2)
            {
                pos.SendSequence(csCancelKey);  
            }else
            {
                pos.SendSequence(csAckOkKey);  // Acknowledge
            }
            
            Trx.SetCurrentInProgressState(_T("ITEMIZATION")); 
        
            break;
        }
    case 7: //approved 
        {
            rc = TB_IGNORE;
            pos.SendSequence(csAckOkKey);  // Acknowledge
            
            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
      
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

            if(bInItemization )
            {
                Trx.SetCurrentInProgressState(_T("ITEMIZATION")); 
            }
            break;
        }
    case 5: // EBT card declined
        {
            rc = TB_TENDERDECLINED;

            CurrTender.ErrorDescription( _T("") );
            

            if(iMsgLevel >= 2)
            {
                pos.SendSequence(csCancelKey);  
            }else
            {
                pos.SendSequence(csAckOkKey);  // Acknowledge
            }
            

            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
          
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;

            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

            break;
        }
    case 3: // EBT card remaining balance or any response code that needs operator intervention
        {
            TBSTATE rc1 = TB_NEEDMOREDATA;

            rc = TB_IGNORE;
            
            
            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
          
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

            CString csCustomerMsg = _T("NeedAssist;9078"); 
        
            //Must remember to add balance
            //CString csRemainingBalance;
            //csRemainingBalance.Format("%d", pCurrTender->GetEBTBalance());
            //csCustomerMsg += _T(" ") + csRemainingBalance;

            pMsgMap->msgDataNeededWithCustomerScreen(rc1, (LPARAM) (LPCTSTR)csCustomerMsg);
            //go ahead and cancel the tender before we issue the dataneeded
            //CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_TENDERCANCELLED, NULL, NULL);
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL);
            Trx.SetCurrentInProgressState(_T("TENDER_PAYMENT")); 
            

            break;
        }
    case 2: // gift card balance
        {
            
            TBSTATE rc1 = TB_NEEDMOREDATA;
           
            if(iMsgLevel == 2)
            {
                pos.SendSequence(csCancelKey);  
            }else if ( iMsgLevel != 3 )
            {
                pos.SendSequence(csAckOkKey);  // Acknowledge
            }
            
            if(bInItemization == false)
            {
                if(iMsgLevel == 2)  //there is no balance must decline
                {
                    rc = TB_TENDERDECLINED;

                    Trx.SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
                    CurrTender.ErrorDescription( csGuidance );
                    
                        
                }else
                {
                    //message map should handle the tenderaccepted when the screen arrives
                    rc = TB_IGNORE;                    
            
                    PSMessageInfo pMsg = new SMessageInfo;
                    ZeroMemory(pMsg, sizeof(SMessageInfo));
          
                    CopyString(pMsg->szRAP_MessageText, csGuidance);
                    pMsg->bHiliteRAPText = true;
                    CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
                    delete [] pMsg->szRAP_MessageText;
                    delete pMsg;
                    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

                    CString csCustomerMsg = _T("NeedAssist;Gift/Return Card Tender"); 
        

                    pMsgMap->msgDataNeeded(rc1, (LPARAM) (LPCTSTR)csCustomerMsg);
                    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL);
                }

            }else
            {
                //we think we're sellin a card with balance problems
                rc = TB_IGNORE;
                
            

	            PSMessageInfo pMsg = new SMessageInfo;
	            ZeroMemory(pMsg, sizeof(SMessageInfo));
	            
                CopyString(pMsg->szScreenText, csGuidance );
	            CopyString(pMsg->szLeadThruText, pMessages->GetStringValueFromID(9075));
	            CopyString(pMsg->szRAP_MessageText, csGuidance);
                pMsg->bHiliteRAPText = true;
				
                //if it was a hard cancel then the item has to be canceled
	            if(iMsgLevel == 2)
                {
                    pMsg->bNeedItemCancel = true;                
                }
                pMsg->bGoBackScanAndBag = true;
               

	            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
                delete [] pMsg->szScreenText;
                delete [] pMsg->szLeadThruText;
                delete [] pMsg->szRAP_MessageText;
                delete pMsg;
                CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);
            }
            break;
        }
     case 6: // serious  need special handling for this one
        if(bInItemization == false)
        {
            TBSTATE rc1 = TB_NEEDMOREDATA;
            rc = TB_IGNORE;

            

            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
          
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);


            CString csCustomerMsg = _T("NeedAssist;9078");     

            pMsgMap->msgDataNeededWithCustomerScreen(rc1, (LPARAM) (LPCTSTR)csCustomerMsg);
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_NEEDMOREDATA, NULL, NULL);
            
            Trx.SetCurrentInProgressState(_T("TENDER_PAYMENT"));           
            break; 
        }
    case 1: // suspend
    default: // all others declined
       
        if(iMsgLevel >= 2)
        {
            pos.SendSequence(csCancelKey);
        }else
        {
            pos.SendSequence(csAckOkKey);  // Acknowledge
        }

        if(bInItemization == false)
        {
            rc = TB_TENDERDECLINED;
            Trx.SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
            PSMessageInfo pMsg = new SMessageInfo;
            ZeroMemory(pMsg, sizeof(SMessageInfo));
          
            CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
            CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

            CurrTender.ErrorDescription( _T("") );
            
            
        }else
        {
            rc = TB_IGNORE;
                       

	        PSMessageInfo pMsg = new SMessageInfo;
	        ZeroMemory(pMsg, sizeof(SMessageInfo));
	        
            CopyString(pMsg->szScreenText, pMessages->GetStringValueFromID(9023));
	        CopyString(pMsg->szLeadThruText, pMessages->GetStringValueFromID(9026));
	        CopyString(pMsg->szRAP_MessageText, csGuidance);
            pMsg->bHiliteRAPText = true;
				        
	        if(iMsgLevel >= 2)
            {
                pMsg->bNeedItemCancel = true; 
            }
            pMsg->bGoBackScanAndBag = true;
         

	        CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
            delete [] pMsg->szScreenText;
            delete [] pMsg->szLeadThruText;
            delete [] pMsg->szRAP_MessageText;
            delete pMsg;
            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);  
        }
        break;

    }

    return rc;
}


int CSolutionMHTender::FindResponseCode(const CString &csResponseCode, TBTENDERTYPE tendertype)
{    
    CCustomerMHOptions *mhOpts= CCustomerMHOptions::instance();
    
    //if in the approval list the no need to check
    if (-1 != mhOpts->OptionValue(_T("ResponseCodeApprove")).Find( csResponseCode ) )  
        return 7;
    // suspend 
    else if ( -1 != mhOpts->OptionValue(_T("RCodeSuspend")).Find( csResponseCode ) )
        return 1;    
    // gift card balance    
    else if (( -1 != mhOpts->OptionValue(_T("RCodeGiftCardRemainingBalance")).Find( csResponseCode )) &&             
        ( tendertype == TB_TENDER_GIFTCARD ))        
        return 2;    
    // Card tendering remaining balance    
    else if (( -1 != mhOpts->OptionValue(_T("RCodeCardRemainingBalance")).Find( csResponseCode )) &&             
        ((tendertype == TB_TENDER_EBT) || (tendertype == TB_TENDER_FOODSTAMPS)))        
        return 3;    
    // gift card delayed activation    
    else if ( -1 != mhOpts->OptionValue(_T("RCodePhoneGiftDelayedActivation")).Find( csResponseCode ) )        
        return 4; 
    // Determine if response code has been configured as serious (e.g., stolen or lost card) (TAR 211937)    
    else if ( -1 != mhOpts->OptionValue(_T("RCodeSerious")).Find( csResponseCode ) )        
        return 6;
    // EBT Card declined decline any EBT tender
    else if ((tendertype == TB_TENDER_EBT) || (tendertype == TB_TENDER_FOODSTAMPS))        
        return 5;     
    // response code not found  all other declined by default handler
    else        
        return 0;
}

//display tender at e-receipt
void CSolutionMHTender::HandleTenderAccepted(void)
{
    CCustomerCurrentTender* pCurr = CCustomerCurrentTender::instance();

	PSMessageInfo pMsg = new SMessageInfo;
    // reset all LPTSTR to NULL
    pMsg->szLeadThruText = pMsg->szScreenText = pMsg->szPlayWave = NULL;
	pMsg->szRAP_MessageText = pMsg->szE_ReceiptText = NULL;
               
	CString szTenderDesc = pCurr->TenderDescription();
	szTenderDesc += _T("\t");

	double dTenderAmt = pCurr->TenderAmountDbl();
	dTenderAmt = dTenderAmt / 100.00;
	CString csTemp; 
	csTemp.Format(_T("%0.2f"), dTenderAmt);
	szTenderDesc += csTemp; 

	CopyString(pMsg->szE_ReceiptText, szTenderDesc);
	CopyString(pMsg->szRAP_MessageText, szTenderDesc);
				
	pMsg->bHiliteReceiptText = false;
    pMsg->bNeedItemCancel = false;
    pMsg->bApprovalNeeded = false;
    pMsg->bGoBackScanAndBag = false;
    pMsg->bHiliteRAPText = false;

	CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
    delete [] pMsg->szE_ReceiptText;
    delete [] pMsg->szRAP_MessageText;
    delete pMsg;

	
    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);
    
}

TBRC CSolutionMHTender::GetTenderDetails(PSTenderDetails psTenderDetails)
{
    TBRC rc = CBaseMHTender::GetTenderDetails(psTenderDetails);

    CCustomerCurrentTender* pCurrTender = CCustomerCurrentTender::instance();
 
    //Reset the tender details after giving them the FL
    //this prevents a hang where the TB may send an extra
    //TB_TENDERACCEPTED because of the message map.  
    //if the tender is null, the tenderaccepted will be
    //ignored on the TB side
    pCurrTender->Reset();

    pCurrTender->TenderDescription(_T(""));

    return rc;


}