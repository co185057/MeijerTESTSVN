//////////////////////////////////////////////////////////////////////
//
// CustomerMHTender.cpp: implementation of the CCustomerMHTender class.
//
// POS148973 Work Request:68237 Name: Robert Susanto Date: March 25, 2015
// POS138176 Work Request:66559 Name: Robert Susanto Date: February 17, 2015
// POS99115 Work Request:61232 Name: Robert Susanto Date: June 18, 2014
// POS72562 Work Request:56285 Name: Matt Condra Date: August 1, 2013
// POS69877 Work Request:56285 Name:Robert Susanto Date:July 5, 2013
// POS36905/POS42532 Work Request:52635 Name:Robert Susanto Date:October 22, 2012
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerMHTender.h"
#include "traceSystem.h"
#include "customerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "customerTBConnectPOS.h"
#include "CustomerTBProperties.h"
#include "CustomerCurrentTender.h"
#include "BaseMessages.h"
#include "customerTBConnectFL.h"
#include "customerMHOptions.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerCurrentTotals.h"



#define T_ID    _T("CCustomerMHTender")

#undef super
#define super CSolutionMHTender

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCustomerMHTender::CCustomerMHTender()
{
}

CCustomerMHTender::~CCustomerMHTender()
{
}


TBSTATE CCustomerMHTender::ProcessMessage(CTBMsgObj *pMsgObj)
{
    TBSTATE rc = TB_IGNORE;
    CCustomerCurrentTender* pCurr = CCustomerCurrentTender::instance();

    CBaseMHTender::ProcessMessage(pMsgObj);

    pCurr->TenderAccepted(true);

    int nTenderType= CCustomerMHOptions::instance()->IntOptionValue(pCurr->TenderDescription());
    
    //if the tender type is unknown assume it some other type of tender
    if (nTenderType== 0)
        nTenderType = -1;  //tender type other

    pCurr->TenderType(nTenderType);   


    // POS99115 Reset the variable for echeck - RRS
    CString csECheckTender = CCustomerTrxBroker::instance()->TBGetProperty(_T("IsECheckTender"),false);
    tbtrace( TM_INFO, _T("Reset the ECheck:(%s) so that the cash drawer will be opened for CASH(%d)"), csECheckTender,nTenderType );
    if( csECheckTender == _T("1") && nTenderType == TB_TENDER_CASH)
    {
        CCustomerTrxBroker::instance()->TBSetProperty(_T("IsECheckTender"),_T("0"),false);
        tbtrace( TM_INFO, _T("ECheck:(%s) is resetted so that the cash drawer will be opened for CASH(%d)"), csECheckTender,nTenderType );
    }
    //e POS99115
    

    if (pCurr->BalanceLong() <= 0)
        pCurr->ChangeDue((long)abs(pCurr->BalanceLong()));

	CCustomerTrxBroker::instance()->SetTenderInProgress(false); //SOTF 6524		
		
    tbtrace( TM_ENTRY, _T("+-ProcessMessage"));
    return rc;
}

TBSTATE CCustomerMHTender::HandleMessage300(bool bInItemization)
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
    CString csWICTransaction = CCustomerTBProperties::instance()->GetHookProperty(_T("WICTransaction"));

    BOOL bHandled = false;

	CCustomerMHOptions *pCurrOpts = CCustomerMHOptions::instance();
    CString csAckOkKey = pCurrOpts->OptionValue(_T("Acknowledge"));
    CString csCancelKey = pCurrOpts->OptionValue(_T("Cancel"));

    switch (nRespCode)
    {
      /* SOTF8022 - special handling for "Please scan long barcode" */
      case 6: // Specific responses that require operator intervention
         {
            if (csResponseCode == _T("RM87"))
			   {
               bHandled = true;
               rc = TB_IGNORE;
               CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_NEEDASSISTMODE, NULL, NULL);
            }

            break;
         }
      /* END SOTF8022 */

      case 3: // EBT card remaining balance or any response code that needs operator intervention
        {
            if(csResponseCode == _T("RM51"))
			{
              bHandled = true;
              rc = TB_IGNORE;
            
            
              PSMessageInfo pMsg = new SMessageInfo;
              ZeroMemory(pMsg, sizeof(SMessageInfo));
          
              CopyString(pMsg->szRAP_MessageText, csGuidance);
              pMsg->bHiliteRAPText = true;
              CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
			  delete [] pMsg->szRAP_MessageText;
              delete pMsg;
              CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

              Trx.TBSetProperty(_T("IsEBTTenderInsufficient"), _T("1"), true);
			  
			  if(iMsgLevel >= 2)
			  {
                  pos.SendSequence(csCancelKey);  
			  }else
			  {
                  pos.SendSequence(csAckOkKey);  // Acknowledge
			  }

			  Trx.SetCurrentInProgressState(_T("TENDER_PAYMENT_EBT_BALANCE"));
            }
			else if(csResponseCode == _T("RM74"))
			{
				bHandled = true;
                rc = TB_IGNORE;

				PSMessageInfo pMsg = new SMessageInfo;
                ZeroMemory(pMsg, sizeof(SMessageInfo));
          
                CopyString(pMsg->szRAP_MessageText, csGuidance);
                pMsg->bHiliteRAPText = true;

				//CString csLeadThruText = pMessages->GetStringValueFromID(9112);
				CString csInstructionText = pMessages->GetStringValueFromID(9378);

				tbtrace(TM_INFO, _T("csInstructionText = %s"), csInstructionText);

				if (csInstructionText == _T("") || csInstructionText.GetLength() == 0)
				{
			        //assigning default texts.
					tbtrace(TM_INFO, _T("csInstructionText = %s"), _T("using default."));
			        csInstructionText = _T("Transaction amount is greater than card balance. Card balance is $ %s, \n\nApply Balance?");
				}
			
				int index = csGuidance.Find(_T("$"));

				CString csAmount = csGuidance.Mid(index + 1);
                csAmount.TrimLeft();
				int index2 = csAmount.Find(_T(" ")); //use to get all chars until it found space.

    			csAmount = csAmount.Mid(0, index2);


				if(iMsgLevel == 2) //it means the value is 0 and only Cancel button is enable. remove the Apply Balance text.
				{
				    index = csInstructionText.Find(_T("%s"));
                    csInstructionText = csInstructionText.Mid(0, index);

					csInstructionText += csAmount;

                    //tbtrace(TM_INFO, _T("csInstructionText for 0 amount: %s"), csInstructionText);
				}
                else
                    csInstructionText.Format(csInstructionText, csAmount);
				
				tbtrace(TM_INFO, _T("Final csInstructionText = %s"),csInstructionText);

				CString strAudio = _T("blank.wav");
				pMsg->szLeadThruText    = (LPTSTR)(LPCTSTR)csInstructionText; //(LPTSTR)(LPCTSTR)csLeadThruText;
                pMsg->szScreenText      = _T(""); //(LPTSTR)(LPCTSTR)csInstructionText;
                pMsg->szPlayWave        = (LPTSTR)(LPCTSTR)strAudio;
                pMsg->bNeedItemCancel   = false;
                pMsg->bGoBackScanAndBag = false;

                CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
			    delete [] pMsg->szRAP_MessageText;
                delete pMsg;
	
	            CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);

			} 
            break;
        }

		case 7: //approved 
        {
			bHandled = true;

            rc = TB_IGNORE;
            pos.SendSequence(csAckOkKey);  // Acknowledge

		    // for non wic and NL35 display the message on the screen
			if (!((csResponseCode == _T("NL35"))  && 
			    (csWICTransaction == _T("1"))))  
			{
				PSMessageInfo pMsg = new SMessageInfo;
				ZeroMemory(pMsg, sizeof(SMessageInfo));
      
				CopyString(pMsg->szRAP_MessageText, csGuidance);
				pMsg->bHiliteRAPText = true;
				CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
				delete [] pMsg->szRAP_MessageText;
				delete pMsg;
				CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL);
			}

			 
            if(bInItemization )
            {
                Trx.SetCurrentInProgressState(_T("ITEMIZATION")); 
            }
            break;
        }
    }

    // If we're in a WIC transactiona and receive an RM message that's not RM00, prompt for assistance.
    if( 
        (csWICTransaction == _T("1")) && 
        (-1 != csResponseCode.Find(_T("RM"))) &&
        (csResponseCode != _T("RM00") &&
        ( bInItemization == false) )
      )
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
        bHandled = true;
    }

    //if we are not itemizing and the balance is zero or negative
    //then this RM code does not go with the tender send the operator into AM
    //to handle it
	CCustomerCurrentTotals* pCurr = CCustomerCurrentTotals::instance();    
    if((!bInItemization) && (pCurr->TotalDueLong() <=0))
    {
		tbtrace(TM_INFO, _T("CCustomerMHTender::HandleMessage300() Trxn has been tendered and a new EFT response code arrive.  Calling for assistance"));
    
		PSMessageInfo pMsg = new SMessageInfo;
		ZeroMemory(pMsg, sizeof(SMessageInfo));

		CString csGuidance= _T("\n") + CCustomerTrxBroker::instance()->TBGetProperty(_T("OG_Guidance_Prop"), true);

		CopyString(pMsg->szRAP_MessageText, csGuidance);
		pMsg->bHiliteRAPText = true;
		CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
		delete pMsg->szRAP_MessageText;
		delete pMsg;

		CCustomerTBScotMsg *pScotMsg = CCustomerTBScotMsg::instance();
		pScotMsg->SendReturnStateMsg(TB_MESSAGESCREEN, NULL, NULL);

		pScotMsg->SendReturnStateMsg(TB_NEEDASSISTMODE, NULL, NULL);

		rc = TB_IGNORE;
		bHandled = true;
    }

	if(!bHandled)
	{
		return CSolutionMHTender::HandleMessage300(bInItemization);
	}
	else
	{
		return rc;
	}
}

int CCustomerMHTender::FindResponseCode(const CString &csResponseCode, TBTENDERTYPE tendertype)
{
	 
	 CString csWICTransaction = CCustomerTBProperties::instance()->GetHookProperty(_T("WICTransaction"));

	 //if this is an NL35 and the resp. code is in the RCodeCardRemainingBalance section and the 
	 //tender is a WIC then treat it as an approved
	 if ((csResponseCode == _T("NL35"))  && 
		 (csWICTransaction == _T("1")))  
	 {
		tbtrace(TM_INFO, _T("Michigan WIC Trxn Response Code:%s treating as approved"), csResponseCode);
    
        return 7;
	 }

	 return CSolutionMHTender::FindResponseCode(csResponseCode, tendertype);

}

//do not display zero cash tender <TAR369970>
void CCustomerMHTender::HandleTenderAccepted(void)
{
    CCustomerCurrentTender* pCurr = CCustomerCurrentTender::instance();
	if(!(pCurr->TenderDescription() == _T("CASH") && pCurr->TenderAmountDbl() == 0))
		CSolutionMHTender::HandleTenderAccepted();
}

//Madz - TAR377139
TBRC CCustomerMHTender::EnterTenderState( const bool bEnter)
{
	CCustomerTrxBroker *pTrxBroker = CCustomerTrxBroker::instance();

	//377139-To prevent from changing to TB_ENTERTENDERSTATE_OUT state
	if(!bEnter && pTrxBroker->GetCurrentInProgressStateName() == _T("ITEMIZATION_GET_COUPON_IP"))
	{
		 return TB_SUCCESS;
	}
	return CSolutionMHTender::EnterTenderState(bEnter);

}
//End - TAR377139

TBRC CCustomerMHTender::GetTenderDetails(PSTenderDetails psTenderDetails)
{
    TBRC rc = CSolutionMHTender::GetTenderDetails(psTenderDetails);

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


// POS42532 - Reassign the tender selection based from the customer selection on MSR - RRS
TBRC CCustomerMHTender::EnterTender( TBTENDERTYPE nTenderType, LONG lAmount, LPCTSTR szCardData,
                              int nPINLength, LPCTSTR szEncryptedPIN, BOOL fOverride, BOOL fVoid) 
{
    TBRC rc = TB_SUCCESS;

    tbtrace( TM_INFO, _T("EnterTender::(nTenderType= %d, lAmount= %d, nPINLength= %d, szEncryptedPIN= %s, fOverride= %d, fVoid= %d)"), 
		  nTenderType, lAmount, nPINLength, szEncryptedPIN, fOverride, fVoid);

    CCustomerTrxBroker &Trx= *CCustomerTrxBroker::instance();
    CBaseCurrentTender* pCurrTender = CCustomerCurrentTender::instance();

    //POS138176 - prevent multiple CASH key being sent
    if( pCurrTender->TenderType() == nTenderType && lAmount ==0 && pCurrTender->TenderAmountLong() == 0 )
    {
        tbtrace( TM_INFO, _T("Duplicate tender sequence is detected for zero cash tender, TenderType(%d), Amount(%d)"), pCurrTender->TenderType(), pCurrTender->TenderAmountLong());
        return rc;
    }

    if ( nTenderType == 10 )
    {
        CString csTenderType = Trx.TBGetProperty(_T("TenderTypeSelected"), true);
        
        //POS148973 - can't get the TenderSelected desc
        if( csTenderType == _T("") )
        {
            csTenderType = Trx.TBGetProperty(_T("TenderType"), true);
            tbtrace( TM_INFO, _T("TenderTypeSelected is still empty, get desc from TenderType(%s))"), csTenderType);
        }
        //e POS148973
        
        
        if(csTenderType.Find(_T("ATM")) != -1)
        {   
            nTenderType = TB_TENDER_DEBIT;            

        }
        if(csTenderType.Find(_T("CREDIT"))!= -1)
        {  
            nTenderType = TB_TENDER_CREDIT;
        }
        if(csTenderType.Find(_T("GIFT"))!= -1)
        {
            nTenderType = TB_TENDER_GIFTCARD;            
        }

        if(csTenderType.Find(_T("EBT CASH"))!= -1) //EBT CASH          
		{
            nTenderType = TB_TENDER_EBT;            
		}

        // POS42533 - Handle new Tender Description OTC CARD - rrs 
        if(csTenderType.Find(_T("OTC CARD"))!= -1)
        {
            nTenderType = TB_TENDER_CREDIT;            		
		}
        // POS42533

        if(csTenderType.Find(_T("Starbucks"))!= -1)
        {
            // POS72562: Handle Starbucks gift card tender.
            tbtrace( TM_INFO, _T("EnterTender::Starbucks gift card tender"));
            nTenderType = TB_TENDER_CREDIT;
        }

        //POS69877 - tender descsription shouldn't contain the GIFT text on it
        if(csTenderType.Find(_T("FOOD"))!= -1 && csTenderType.Find(_T("GIFT")) == -1 )
        {
            nTenderType = TB_TENDER_FOODSTAMPS;                
		}
    }
    tbtrace(TM_INFO, _T("Enter Tender, Type = %d"), nTenderType);
    return CSolutionMHTender::EnterTender(nTenderType, lAmount, szCardData, nPINLength, szEncryptedPIN, fOverride, fVoid);
}
// e - POS42532 - Reassigne the tender selection based from the customer selection on MSR - RRS