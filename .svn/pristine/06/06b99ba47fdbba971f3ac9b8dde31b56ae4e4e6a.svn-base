// SolutionTBConnectPOS.cpp: implementation of the CSolutionTBConnectPOS class.
//
// Change history:
//
// CRD 537490 Name: CJ Estrada Date: Mar 17, 2021
// CRD 564271 Name: CJ Estrada Date: Mar 9, 2021
// POS300122 Work Request:79417 Name:Aparna Tunuguntla  Date:April 12, 2017
// POS311584 Work Request:76936 Name:Alex Bronola  Date:January 23, 2017
// POS293815 Work request:76936 Name:Jangala Sunny Date:December 16,2016
// POS277809 Work Request:73594 Name:Robert Susanto Date:July 12, 2016
// POS248467 Work Request:68942 Name:Robert Susanto Date:April 14, 2016
// POS97215 Work Request:61233 Name:Matt Condra Date:May 20, 2014
// POS95744 Work Request:61232 Name:Robert Susanto Date:May 1, 2014
// POS83168 Work Request:61232 Name:Matt Condra Date:February 11, 2014
// POS81632 Work Request:59313 Name:Matt Condra Date:December 10, 2013
// POS30981/POS31050 Work Request:50864 Name:Robert Susanto Date:July 12, 2012
// POS25385 Work Request:16656 Name:Matt Condra Date:Dec 9, 2011
// POS18041 Work Request:16656 Name:Matt Condra Date: October 3, 2011
// POS23514 Work Request:16632 Name: Sandeep Shenoy Date: Nov 9, 2011
// POS15617 Work Request:15196 Name:Matt Condra Date:July 20, 2011
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionTBConnectPOS.h"
#include "CustomerTBConnectPOS.h"
#include "TBGenMsg.h"
#include "CustomerTBProperties.h"
#include "TraceSystem.h"
#include "safearrayvariant.h"
#include "PrinterMsg.h"
#include "CustomerMHPrinter.h"
#include "CustomerMHItem.h"
#include "CashDrawerMsg.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentItem.h"
#include "CustomerTBSharedDefs.h"
#include "CustomerMHOptions.h"
#include "CustomerCurrentTransaction.h"
#include "CustomerMsgMap.h"

#define T_ID  _T("CSolutionTBConnectPOS")

bool CSolutionTBConnectPOS::ms_bIsItemsNeedTobeBuffered = false;

CString CSolutionTBConnectPOS::csPrev_bstr1 = _T(""); // POS15617
static CString csPrevTotalMsg = _T("");
CArray <CString, CString> CSolutionTBConnectPOS::csEASEntryIDList;
long CSolutionTBConnectPOS::lMobileAssistanceNeeded = 0;

// POS248467 rrs
// Retrieve a SAFE interface for the foreign thread
// Repackage an object reference in a stream object
// return the newly extracted interface
extern ACS::IACSHookPtr spHook = NULL;
// e - POS248467 rrs

DWORD WINAPI ACSHookThreadProc(PVOID pvParam)
{	
	_ASSERT(pvParam);
	CSolutionTBConnectPOS & conPos = *(CSolutionTBConnectPOS*)pvParam;
	return conPos.ACSHookThread();
}


void WINAPI ACSHookCallbackProc(long lEvent, long lData, BSTR bstrData1, BSTR bstrData2)
{
	tbtrace(TM_INFO, _T("+ACSHookCallbackProc"));
    CCustomerTBConnectPOS::CallbackLock();
    tbtrace( TM_INFO, _T("ACSHookCallbackProc Inside"));

    
	if( CCustomerTBConnectPOS::IsTerminating() ) // static call don't use instance
	{
		tbtrace(TM_INFO, _T("Ignoring ACSHookCallbackProc, TB is terminating."));
    	tbtrace(TM_INFO, _T("-ACSHookCallbackProc"));
        CCustomerTBConnectPOS::CallbackUnlock();
		return;
	}

	// CAUTION: we're in a foreign thread when this is called, so don't do anything funny, 
    // and don't take long doing whatever you wanna do in here either!
    _bstr_t bstr1(bstrData1,FALSE);
    _bstr_t bstr2(bstrData2, FALSE);
    
    switch(lEvent)
    {
    case CSolutionTBConnectPOS::ACSHookEvent::OnConnectEvent:
        {
			try
			{
				// implement handler if necessary
				// ideally the hook layer will send an appropriate state change
				tbtrace(TM_INFO, _T("Received a Connect Event from the POS."));
			
			
			}
			catch(_com_error & cex)
			{
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("A COM exception as occured. Error [0x%08X]: %s."), cex.Error(), cex.ErrorMessage());
			}
			catch(...)
			{
				//
				// No bogus TB code is allowed to cause exceptions in this foreign thread.
				//
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("GENERAL EXCEPTION HAS OCCURED!!!"));
			}
        }
        break;
        
        
    case CSolutionTBConnectPOS::ACSHookEvent::OnDisconnectEvent:
        {
			try
			{
				// implement handler if necessary
				// ideally the hook layer will send an appropriate state change
				tbtrace(TM_INFO, _T("Received a Disconnect event from the POS."));
				

			}
			catch(_com_error & cex)
			{
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("A COM exception has occured. Error [0x%08X]: %s."), cex.Error(), cex.ErrorMessage());
			}
			catch(...)
			{
				//
				// No bogus TB code is allowed to cause exceptions in this foreign thread.
				//
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("GENERAL EXCEPTION HAS OCCURED!!!"));
			}
        }
        break;
       
        
    case CSolutionTBConnectPOS::ACSHookEvent::OnGenMsgEvent:
        {
            /*
            if( CCustomerTBConnectPOS::IsTerminating() ) // static call don't use instance
	        {
		        tbtrace(TM_INFO, _T("Ignoring OnGenMsgEvent, TB is terminating."));
                break;
            }*/

			try
			{
				//static CString csPrev_bstr1 = _T(""); //POS15617: Use CSolutionTBConnectPOS::csPrev_bstr1 instead
				tbtrace(TM_INFO, _T("Received a GenMsg event from the POS.  lData:%d"), lData); // POS15617
				if ( lData == 0 )
                {
					CString csCurr = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
				    CTBGenMsg * pMsg = new CTBGenMsg();
				    pMsg->setXML(bstr1);
					bool bIsIgnoreMsg = false;

					tbtrace(TM_INFO, _T("GenMsg name: '%s'."), pMsg->GetName()); // POS15617

                    // Update the current display property
                    // BEFORE posting.  This display is updated BEFORE
                    // the CCurrentCustomerDisplay.

                    if (pMsg->GetName() == _T("Display"))
				    {
                        CCustomerTBProperties::instance()->SetProperty(_T("POSDisplay"), (LPCTSTR) pMsg->getXML());                        
				    }
					
					else if (pMsg->GetName() == _T("Total"))
				    {

						//added checking of itemsold cause the first total message is going to be ignored by TB.
						if(csPrevTotalMsg == (LPCTSTR)bstr1 && CCustomerMHItem::instance()->GetNumberOfItemsSold() > 1) 
						{
							tbtrace(TM_INFO, _T("Ignoring Total message...Totals is equal to the previous one."));
							
							bIsIgnoreMsg = true;
						}
						else
						{
							tbtrace(TM_INFO, _T("Total message...Totals don't equal previous one.")); // POS15617
							csPrevTotalMsg = (LPCTSTR)bstr1;
							CSolutionTBConnectPOS::csPrev_bstr1 = _T("");
							//tbtrace(TM_INFO, _T("GenMsg Dump:\r\n%s\r\n"), (LPCTSTR)bstr1); // POS248467 rrs 
						}                        
					}
					#ifdef _MEIJER
					else if ( pMsg->GetName() == _T("PosState"))
					{
						_bstr_t szDesc = pMsg->GetValue2(_T("PosState"));
						CString csPosState = (TCHAR *)szDesc;

						if( csPosState.Find(_T("RESUME_STARTED")) != -1)
						{
							tbtrace(TM_INFO, _T("Resume started.")); // POS15617
							csPrevTotalMsg = _T(""); //SSCOI-46483 Need reset here, it shouldn't remember the total from previously suspended transaction.
							bIsIgnoreMsg = true;
							CCustomerCurrentItem *pCurItem = CCustomerCurrentItem::instance();
							CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
							//CString csFLConfig = pOpts->OptionValue(CString(_T("FastLaneConfiguration")));

							CString csResumeConfig = pOpts->OptionValue(CString(_T("IsResumeLikePayStation")));
							
							tbtrace(TM_INFO, _T("m_SItemArray cleared")); // POS18041
							pCurItem->m_SItemArray.clear();
							
							if ( csCurr.Find(_T("ASSISTMODE_")) == -1 && csResumeConfig != _T("N") )
							{
								tbtrace(TM_INFO, _T("Resume while assist mode not active.")); // POS25385/POS15617
								CCustomerCurrentTransaction::instance()->DirtyTrx(TRUE);
								CCustomerTrxBroker::instance()->TBSetProperty(_T("IsDirtyTrxWithNoItems"), _T("1"), false);
								CSolutionTBConnectPOS::ms_bIsItemsNeedTobeBuffered = true;
							}
							if (csCurr == _T("ITEMIZATION"))
							{	// POS25385: Need left and right braces to prevent sending TB_RESUME_STARTED during assist mode.
								tbtrace(TM_INFO, _T("Resume in itemization.")); // POS15617
								CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_RESUME_STARTED, NULL, NULL);
							}
						}
						else if ( csPosState == _T("RESUME_COMPLETE") )
						{
							tbtrace(TM_INFO, _T("Resume complete.")); // POS15617
							//tbtrace(TM_INFO, _T("GenMsg Dump:\r\n%s\r\n"), (LPCTSTR)bstr1); // POS248467 rrs
							bIsIgnoreMsg = true;
							CSolutionTBConnectPOS::ms_bIsItemsNeedTobeBuffered = false;

							//CRD 289150 - include ASSISTMODE_ENTER TB State for Generic Handler Mobile Resume
							//CRD 291952 - include DIGITAL_COUPON_PIN_ENTRY for regular resumes and cancelled mPerks PIN entry on the PIN pad
							if( csCurr.Find(_T("ITEMIZATION")) != -1 || csCurr.Find(_T("ASSISTMODE_ENTER")) != -1 || csCurr.Find(_T("TB_DIGITAL_COUPON_PIN_ENTRY")) != -1)
							{
								//Start CRD 177093
								if(CCustomerMHItem::instance()->isFLMSTransferring)
									CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_RESUME_COMPLETE, NULL, NULL);
								//End CRD 177093
								else
									CCustomerTrxBroker::instance()->SetFLState((LPCTSTR) _T("SMScanAndBag"));
							}
						}
						else if (csPosState.Find(_T("EAS_ITEM")) != -1)	//CRD 232268
						{
								CCustomerMHItem *pMHItem = CCustomerMHItem::instance();
								pMHItem->msgEASItem();
						}
						else if (csPosState.Find(_T("MOBILE_GENERIC_DELAYED")) != -1)	//CRD 232268
						{

								CSolutionTBConnectPOS::lMobileAssistanceNeeded++;
								tbtrace(TM_INFO, _T("is an mobile generic delayed item and is mobile") );													
						}
					}
               //POS23514:Mobile Shopper-Display CM reward desc at resume time
					else if ( (pMsg->GetName() == _T("ItemDetails") || pMsg->GetName() == _T("ACSReward")) 
                          && CSolutionTBConnectPOS::ms_bIsItemsNeedTobeBuffered == true 
                          && csCurr.Find(_T("ASSISTMODE_")) == -1
						  && CCustomerMHItem::instance()->isFLMSTransferring == false)	//CRD 241464
					{	
						tbtrace(TM_INFO, _T("ItemDetails while assist mode not active.")); // POS25385/POS15617
						CCustomerTrxBroker::instance()->TBSetProperty(_T("IsThereItemNeedToDisplayed"), _T("1"), false);
						CCustomerCurrentItem *pCurItem = CCustomerCurrentItem::instance();				

						bIsIgnoreMsg = true;

						CTBGenMsg * pResumeMsg = new CTBGenMsg();
						*pResumeMsg = *pMsg;
                        
                        //6226
                  //POS23514:Mobile Shopper-Skip this block if this is a CM reward
                  if(pMsg->GetName() != _T("ACSReward"))
                  {
                     
                     CCustomerMHItem *pMHItem = CCustomerMHItem::instance();
                     if(pMHItem->GetACSDelayedAgeRestricted()) 
                     {
                         int nLength = 0;
                         CItemDetailsMsg pItemMsg;
			             BYTE* pBuf = pMsg->Serialize( nLength );
                         pItemMsg.Parse( pBuf);
                         delete[] pBuf;

                         if (&pItemMsg) 
                         {
                             //if we're dealing with an age restricted item
                             //save off the max age required
                             long lTmpAge = pItemMsg.RestrictedAge();
                             if((pItemMsg.IsRestricted()) || (lTmpAge))
                             {
                                 pItemMsg.IsRestricted(true);            
                                 if(lTmpAge > pMHItem->GetHighestAgeRequired())
                                 {
                                      pMHItem->SetHighestAgeRequired(lTmpAge);
         
                                 }

                                 //save off the item code of the very first
                                 //age rest. item for tlogging
                                 if(pMHItem->GetUPCAgeRequired() == _T(""))
                                 {
                                     pMHItem->SetUPCAgeRequired(pItemMsg.ItemCode());
                                 }
                             }
                         }
                     }
                  }     
                  //POS23514:Mobile Shopper-End
                        // -sotf
						tbtrace(TM_INFO, _T("Saving ItemDetails to m_SItemArray buffer.")); // POS18041
						pCurItem->m_SItemArray.push_back(pResumeMsg);
					}
					#endif
					//CRD 313027 save off TB properties for Everseen properties
					else if( pMsg->GetName() == _T("EverseenProperties"))
					{
						_bstr_t szDesc = pMsg->GetValue2(_T("EverseenVideoPath"));
						CString videoPath = (TCHAR *)szDesc;

						tbtrace(TM_INFO, _T("video path - %s"), videoPath);
						CCustomerTrxBroker::instance()->TBSetProperty(_T("EverseenVideoPathProp"), videoPath , false);
					}
					// Start CRD 564271
					else if( pMsg->GetName() == _T("ApplyMeijerCardProperties"))
					{
						_bstr_t szEnabled = pMsg->GetValue2(_T("ApplyMeijerCardEnabled"));
						CString applyMeijerCardEnabled = (TCHAR *)szEnabled;
						CCustomerTrxBroker::instance()->TBSetProperty(_T("ApplyMeijerCardEnabled"), applyMeijerCardEnabled , false);
						// CRD 537490
						_bstr_t szRedisplay = pMsg->GetValue2(_T("ApplyMeijerCardRedisplay"));
						CString applyMeijerCardRedisplay = (TCHAR *)szRedisplay;
						CCustomerTrxBroker::instance()->TBSetProperty(_T("ApplyMeijerCardRedisplay"), applyMeijerCardRedisplay , false);

						_bstr_t szAttract = pMsg->GetValue2(_T("ApplyMeijerCardOnAttract"));
						CString applyMeijerCardAttract = (TCHAR *)szAttract;
						CCustomerTrxBroker::instance()->TBSetProperty(_T("ApplyMeijerCardOnAttract"), applyMeijerCardAttract , false);

						_bstr_t szSNB = pMsg->GetValue2(_T("ApplyMeijerCardOnSNB"));
						CString applyMeijerCardSNB = (TCHAR *)szSNB;
						CCustomerTrxBroker::instance()->TBSetProperty(_T("ApplyMeijerCardOnSNB"), applyMeijerCardSNB , false);

						_bstr_t szPayment = pMsg->GetValue2(_T("ApplyMeijerCardOnPayment"));
						CString applyMeijerCardPayment = (TCHAR *)szPayment;
						CCustomerTrxBroker::instance()->TBSetProperty(_T("ApplyMeijerCardOnPayment"), applyMeijerCardPayment , false);
					}
					// End CRD 564271
				    else
				    {
					    // don't trace the display control
						if ((CSolutionTBConnectPOS::csPrev_bstr1 != (LPCTSTR)bstr1) && !bIsIgnoreMsg)
						{
							tbtrace(TM_INFO, _T("GenMsg different than previous.")); // POS15617
							//tbtrace(TM_INFO, _T("GenMsg Dump:\r\n%s\r\n"), (LPCTSTR)bstr1);   // POS248467 rrs
						}
				    }
					 
                    if ( pMsg->GetTBMsgId() == TBMSG_PRINTER )
                    {
                        tbtrace(TM_INFO, _T("%s message has a printer ID."),pMsg->GetName()); // POS15617
                        TBSTATE rc = TB_IGNORE;
                        CPrinterMsg PrtMsg;
                        pMsg->SetMsgVersion(VERSION_GENOBJ_BIN);
                        int nLength = 0;
                        BYTE* pBuf = pMsg->Serialize( nLength );
                        PrtMsg.Parse( pBuf);
                        delete[] pBuf;
                        rc = CCustomerMHPrinter::instance()->ProcessMessage( &PrtMsg );
                        if ( TB_IGNORE != rc )
                        {
                            tbtrace( TM_WARNING, _T("Printer ProcessMessage returned %d"), rc );
                            CCustomerTBScotMsg::instance()->SendReturnStateMsg( rc, NULL, NULL);
                        }
                        delete pMsg;
                    }
                    else if(pMsg->GetName() == _T("PosDynaframeMode"))
                    {
                        // POS248467 rrs
                        // POS83168: Check if the POS sales app is ready to receive scanned input.

                        /* _bstr_t bstrPosScanScaleImage = pMsg->GetValue2(_T("PosScanScaleImage"));
                        CString csPosScanScaleImage = (TCHAR *)bstrPosScanScaleImage;
                        if ( (csPosScanScaleImage.Find(_T("scaninpt.bmp")) != -1) &&
                             (CCustomerTrxBroker::instance()->TBGetProperty(_T("WaitForPosScannerReady"), true) == _T("1"))
                           )
                        {
                            // The sales app is ready to receive scanned input, so tell the FastLane app
                            // so that it can enable the scanner (if it is in the proper state).
                            // Multiple scaninpt.bmp can be received so bump the WaitForPosScannerReady property
                            // so that TB_POS_SCANNER_READY is only sent once.
                            CCustomerTrxBroker::instance()->TBSetProperty(_T("WaitForPosScannerReady"), _T("2"), true);
                            tbtrace(TM_INFO, _T("WaitForPosScannerReady=2"));
                            CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_POS_SCANNER_READY, NULL, NULL);
                        }*/
                        // e - POS248467 rrs
						
						// start RFQ 9545 Mobile shopper
						bstr_t bstrPosScanScaleImage = pMsg->GetValue2(_T("PosScanScaleImage"));
                        CString csPosScanScaleImage = (TCHAR *)bstrPosScanScaleImage;
					
						//CCustomerTrxBroker::instance()->TBSetProperty(_T("WaitForPosScannerReady"), _T("1"), false);
						tbtrace(TM_INFO, _T("current POS scanner state csPosScanScaleImage = %s"), csPosScanScaleImage);
                       
						if ((csPosScanScaleImage.Find(_T("|BITMAP|:scaninpt.bmp")) == 0) &&
							(CCustomerTrxBroker::instance()->TBGetProperty(_T("xWaitForPosScannerReady")) == _T("1")))
						{
                            // The sales app is ready to receive scanned input, so tell the FastLane app
                            // so that it can enable the scanner (if it is in the proper state).
                            CCustomerTrxBroker::instance()->TBSetProperty(_T("xWaitForPosScannerReady"), _T("0"), false);
                            tbtrace(TM_INFO, _T("ok to process the next item. itemmode. xWaitForPosScannerReady = 0 csPosScanScaleImage = %s"), csPosScanScaleImage);
							CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_POS_SCANNER_READY, NULL, NULL);
                        }
						else if ((csPosScanScaleImage.Find(_T("|BITMAP|:scaninpt_refresh.bmp")) == 0) && 
							     (CCustomerTrxBroker::instance()->TBGetProperty(_T("xWaitForPosScannerReady")) == _T("1")))
						{
                            // The sales app is not ready to receive scanned input, so tell the FastLane app
                            // so that it can enable the scanner (if it is in the proper state).
                            CCustomerTrxBroker::instance()->TBSetProperty(_T("xWaitForPosScannerReady"), _T("0"), false);
                            tbtrace(TM_INFO, _T("ok to process the next item. itemmode. xWaitForPosScannerReady = 01 csPosScanScaleImage = %s"), csPosScanScaleImage);
							//CCustomerTBScotMsg::instance()->SendReturnStateMsg(TB_POS_SCANNER_READY, NULL, NULL);
						}
						else if ((csPosScanScaleImage.Find(_T("")) == 0) && 
							(CCustomerTrxBroker::instance()->TBGetProperty(_T("xWaitForPosScannerReady")) == _T("0")))
						{
                            // The sales app is not ready to receive scanned input, so tell the FastLane app
                            // so that it can enable the scanner (if it is in the proper state).
                            CCustomerTrxBroker::instance()->TBSetProperty(_T("xWaitForPosScannerReady"), _T("1"), false);
                            tbtrace(TM_INFO, _T("not ok to process the next item. waitingmode. xWaitForPosScannerReady = 1 csPosScanScaleImage = %s"), csPosScanScaleImage);
						}

						// end RFQ 9545 Mobile shopper

                        //POS95744 - delete the pMsg to avoid memory leak
                        delete pMsg;
                    }
                    else if ((CSolutionTBConnectPOS::csPrev_bstr1 != (LPCTSTR)bstr1) && !bIsIgnoreMsg)
                    {
						tbtrace(TM_INFO, _T("Post %s message."),pMsg->GetName()); // POS15617
						CCustomerTBScotMsg::instance()->SendScotMsg(pMsg);
						CSolutionTBConnectPOS::csPrev_bstr1 = (LPCTSTR)bstr1;	
                    }
                    else if( pMsg->GetName() == _T("ACSList") )  //POS31050 - Keep posting ACSList even the contents are exactly the same
                    {
                        tbtrace(TM_INFO, _T("Same ACSList received but still need to Post %s message."),pMsg->GetName()); // POS31050
						CCustomerTBScotMsg::instance()->SendScotMsg(pMsg);
						CSolutionTBConnectPOS::csPrev_bstr1 = (LPCTSTR)bstr1;
                    }
					else
					{
						static bool bIsIgnore = true;
						
						//if(bIsIgnore)//escape only alternately // POS248467 rrs
                        if(bIsIgnoreMsg)
						{
							tbtrace(TM_INFO, _T("Ignoring %s message."),pMsg->GetName());
							delete pMsg; //TAR345284
							bIsIgnore = false;                            
						}
						else
						{
							tbtrace(TM_INFO, _T("Don't ignore %s message."),pMsg->GetName()); // POS15617
							CCustomerTBScotMsg::instance()->SendScotMsg(pMsg);
							bIsIgnore = true;
						}
					}
                }
                else
                {
                    tbtrace( TM_INFO, _T("GenMsg binary control %d"), lData );

                    CString csData = (LPCTSTR)bstr1;
                    CString csTemp;
                    int i=0;
                    BYTE* pBuf = new BYTE[csData.GetLength()/2];
                    while ( i < csData.GetLength() )
                    {
                        csTemp = csData.Mid( i, 2 );
                        int iVal=0; 
                        _stscanf( csTemp, _T("%x"), &iVal ); 
                        pBuf[i/2] = iVal;
                        i += 2;
                    }

                    switch( lData )
                    {
                    case 2:   //Cash Drawer
                        {
                            tbtrace( TM_INFO, _T("Cash Drawer Msg") ); // POS15617
                            CCashDrawerMsg* pCashDrawer = new CCashDrawerMsg();
                            pCashDrawer->Parse( pBuf );
                            CCustomerTBScotMsg::instance()->SendScotMsg(pCashDrawer);
                        }
                        break;
                    case 11:  //Printer
                        {
                            tbtrace( TM_INFO, _T("PrinterBIN:%s"), csData );
                            CPrinterMsg pPrinter;
                            pPrinter.Parse( pBuf );
                            TBSTATE rc = TB_IGNORE;
                            rc = CCustomerMHPrinter::instance()->ProcessMessage( &pPrinter );
                            if ( TB_IGNORE != rc )
                            {
                                tbtrace( TM_WARNING, _T("Printer ProcessMessage returned %d"), rc );
                                CCustomerTBScotMsg::instance()->SendReturnStateMsg( rc, NULL, NULL);
                            }
                        }
                        break;
                    default:  // Other Devices
                        tbtrace( TM_WARNING, _T("  Message from Unsupported device %d"), lData );
                        break;
                    }

                    delete[] pBuf;
                }
			}
			catch(_com_error & cex)
			{
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("A COM exception as occured. Error [0x%08X]: %s."), cex.Error(), cex.ErrorMessage());
			}
			catch(...)
			{
				//
				// No bogus TB code is allowed to cause exceptions in this foreign thread.
				//
				_ASSERT(FALSE);
				tbtrace(TM_ERROR, _T("GENERAL EXCEPTION HAS OCCURED!!!"));
			}

        }
        break;
        
    case CSolutionTBConnectPOS::ACSHookEvent::OnTraceEvent:
        {
            switch(lData)
            {
            case CSolutionTBConnectPOS::ACSHookTraceType::ttInfo:  // info
                tbtrace(TM_INFO, _T("ACSHook - %s"), (LPCTSTR)bstr1);
                break;
                
            case CSolutionTBConnectPOS::ACSHookTraceType::ttWarning: // warning
                tbtrace(TM_WARNING, _T("ACSHook - %s"), (LPCTSTR)bstr1);
                break;
                
                
            case CSolutionTBConnectPOS::ACSHookTraceType::ttError: // error
                tbtrace(TM_ERROR, _T("ACSHook - %s"), (LPCTSTR)bstr1);
                break;
            
            case CSolutionTBConnectPOS::ACSHookTraceType::ttDebug: // debug
                tbtrace(TM_DEBUG, _T("ACSHook - %s"), (LPCTSTR)bstr1);
                break;                

            default:  // unknown
                tbtrace(TM_ERROR, _T("ACSHook - %s"), (LPCTSTR)bstr1);
                break;
                
            };
        }
        break;
    default:
        
        _ASSERT(FALSE);
    };

  	tbtrace(TM_INFO, _T("-ACSHookCallbackProc"));
    CCustomerTBConnectPOS::CallbackUnlock();
}




CSolutionTBConnectPOS::CSolutionTBConnectPOS()
	: m_spHook(NULL), m_pHookStream(NULL), m_dwHookThreadId(0), m_hExitHookSignal(NULL),  
	  m_hHookThread(NULL), m_hInitHookSignal(NULL)

{
    IsTerminating(FALSE);
	m_hInitHookSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
	_ASSERT(m_hInitHookSignal);

	m_hExitHookSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
	_ASSERT(m_hExitHookSignal);


	m_hHookThread = CreateThread(NULL, 0, ACSHookThreadProc, this, 0, NULL);
	_ASSERT(m_hHookThread);
}

CSolutionTBConnectPOS::~CSolutionTBConnectPOS()
{
    StopHookThread();
}

// can't seem to define a static member in these classes so 
// wrapping global with Callback[Un]Lock() static method instead below
static CCriticalSection g_critSecCallback;
void CSolutionTBConnectPOS::CallbackLock()
{
    g_critSecCallback.Lock();
}

void CSolutionTBConnectPOS::CallbackUnlock()
{
    g_critSecCallback.Unlock();
}

    
// can't seem to define a static member in these classes so 
// wrapping global with IsTerminating() static method instead below
static BOOL g_bTerminating = FALSE;
void CSolutionTBConnectPOS::IsTerminating(BOOL bTerminating)
{ 
    g_bTerminating = bTerminating; 
}

BOOL CSolutionTBConnectPOS::IsTerminating(void)
{ 
    return g_bTerminating; 
}

void CSolutionTBConnectPOS::StopHookThread()
{
    tbtraceIO(_T("StopHookThread"));
    if( m_hExitHookSignal )
    {
        SetEvent(m_hExitHookSignal);


        // Wait up to a X seconds for thread completion
        // if it does not complete gracefully in 5 seconds then
        // kill it by force and continue
        //
        DWORD dwCode = 0;
        const DWORD dwTimeout = INFINITE;
	    WaitForSingleObject( m_hHookThread, dwTimeout );
        tbtrace(TM_INFO, _T("Waiting for ACSHookThread to complete..."));
        GetExitCodeThread(m_hHookThread, &dwCode);

        // If thread is still active, kill it
        if( STILL_ACTIVE == dwCode )
        {
            tbtrace(TM_WARNING, _T("ACS Hook Thread did not die within %d ms as expected. TerminateThread is being called."), dwTimeout);
            TerminateThread(m_hHookThread,-1);
        }
        else
        {
            tbtrace(TM_INFO, _T("ACS Hook Thread terminated gracefully."));
        }
        m_hHookThread = NULL;
	    CloseHandle(m_hExitHookSignal);
        m_hExitHookSignal = NULL;
    }    

    if( m_hInitHookSignal )
    {
	    CloseHandle(m_hInitHookSignal);
	    m_hInitHookSignal = NULL;
    }
}

DWORD CSolutionTBConnectPOS::ACSHookThread()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	try
	{
		_ASSERT(SUCCEEDED(hr));
		hr = m_spHook.CreateInstance(__uuidof(ACS::CACSHook));

		// Set the callback for events
		_variant_t vtCallback;
        vtCallback.ulVal= (ULONG)ACSHookCallbackProc;
        vtCallback.vt = VT_UI4;
        m_spHook->SetCallback(vtCallback);


		if( SUCCEEDED(hr) )
		{
			m_dwHookThreadId = GetCurrentThreadId();
			hr = CoMarshalInterThreadInterfaceInStream(__uuidof(ACS::IACSHook), m_spHook, &m_pHookStream);
			if( SUCCEEDED(hr) )
			{
				//
				// Hook layer has been initialized so enable access to hook object
				//
				SetEvent(m_hInitHookSignal);

				//
				// Implement thread pump for COM apartment calls
				// 
				DWORD dwWait = 0;
				HANDLE hHandles[] = { m_hExitHookSignal };
				while( (dwWait = MsgWaitForMultipleObjects(1, hHandles, FALSE, 5000, QS_ALLINPUT)) )
				{
					if( dwWait == (WAIT_OBJECT_0 + 1) )
					{
						MSG msg = { 0 };
						while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
						{ 
							if( msg.message == WM_QUIT )
							{
                                tbtrace(TM_INFO, _T("Received WM_QUIT message to end ACS Hook Thread."));
								break;
							}
							
							TranslateMessage(&msg);
							DispatchMessage(&msg);			
						}
					}
					else if( dwWait == WAIT_TIMEOUT )
					{
						// perform any housekeeping over here.

					}
					else
					{
						tbtrace(TM_INFO, _T("Received unknown wait return code [%d] to end ACS Hook Thread."));
						break;
					}
                }

                if( dwWait == WAIT_OBJECT_0 )
                {
                    tbtrace(TM_INFO, _T("Received signal to end ACS Hook Thread."));
                }
			}
			else
			{
				tbtrace(TM_ERROR, _T("Failed to package the ACS Hook Manager interface into a stream. Error: 0x%08X"), hr);
			}

            try
            {
                tbtrace(TM_INFO, _T("Disconnecting ACS Hook..."));
			    m_spHook->Disconnect();
                tbtrace(TM_INFO, _T("ACS Hook disconnected."));
            }
            catch(...)
            {
                tbtrace(TM_ERROR, _T("Exception occured while disconnecting ACS Hook."));
            }

            try
            {
			    ACS::IACSHook * pHook = NULL;
			    if ( NULL != m_pHookStream )
			    {
                    tbtrace(TM_INFO, _T("Releasing Hook Stream..."));
				    HRESULT hr = CoGetInterfaceAndReleaseStream(m_pHookStream, __uuidof(ACS::IACSHook), (void**)&pHook);
				    m_pHookStream = NULL;
                    pHook->Release();
                    pHook = NULL;
                    tbtrace(TM_INFO, _T("Hook Stream Released."));
			    }
            }
            catch(...)
            {
                tbtrace(TM_ERROR, _T("Exception occured while releasing Hook Stream."));
            }
		}
		else
		{
			tbtrace(TM_ERROR, _T("Failed to create an instance of the ACS Hook Manager. Error: 0x%08X"), hr);
		}

	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("COM Exception [0x%08X]: %s"), cex.Error(), cex.ErrorMessage());
	}
    catch(...)
    {
        tbtrace(TM_ERROR, _T("General Exception occured in ACSHookThread"));
    }
    m_spHook = NULL;
                
    tbtrace(TM_INFO, _T("Calling CoUninitialize() in ACSHookThread and returning..."));
    CoUninitialize();
    return 0;
}


ACS::IACSHookPtr CSolutionTBConnectPOS::GetHook(bool bForceMarshalled) 
{
    tbtrace( TM_INFO, _T("+CSolutionTBConnectPOS::GetHook()"));
    m_Critical.Lock(INFINITE);
    //tbtrace( TM_INFO, _T("GetHook Inside"));
    //tbtrace( TM_INFO, _T("HookThreadID (%d), CurrentThreadID(%d)"), m_dwHookThreadId, GetCurrentThreadId() );
	//
	// Ensure the hook layer is initialized before completing this call
	//
	WaitForSingleObject(m_hInitHookSignal, INFINITE);
    
    //tbtrace( TM_INFO, _T("Done waiting !!!"));

    // Provide a safe pointer to the caller!
    if( m_dwHookThreadId == GetCurrentThreadId() )
    {
        tbtrace( TM_DEBUG, _T("-CSolutionTBConnectPOS::GetHook()"));
        m_Critical.Unlock();
        return m_spHook;
    }
    else
    {
        // Retrieve a SAFE interface for the foreign thread
        // Repackage an object reference in a stream object
        // return the newly extracted interface
        // POS248467 rrs
        //ACS::IACSHookPtr spHook = NULL;
        if( spHook == NULL || bForceMarshalled )	// POS277809
        {
            ASSERT(m_pHookStream);
            HRESULT hr = CoGetInterfaceAndReleaseStream(m_pHookStream, __uuidof(ACS::IACSHook), (void**)&spHook);
            if ( !SUCCEEDED(hr))
            {
                tbtrace( TM_ERROR, _T("Failed in CoGetInterfaceAndReleaseStream 0x%0X"), hr );
            }
            tbtrace( TM_INFO, _T("CSolutionTBConnectPOS::GetHook() - Get the Interface"));
            ASSERT(SUCCEEDED(hr) && !(spHook == NULL));
            m_pHookStream = NULL;
            hr = CoMarshalInterThreadInterfaceInStream(__uuidof(ACS::IACSHook), spHook, &m_pHookStream);
            if ( !SUCCEEDED(hr))
            {
                tbtrace( TM_ERROR, _T("Failed in CoGetInterfaceAndReleaseStream 0x%0X"), hr );
            }
            ASSERT(SUCCEEDED(hr) && m_pHookStream);
            tbtrace( TM_INFO, _T("CSolutionTBConnectPOS::GetHook() - Marshalled the Interface"));
            tbtrace( TM_DEBUG, _T("-CSolutionTBConnectPOS::GetHook()"));
            m_Critical.Unlock();
        }
        else
        {			
            m_Critical.Unlock();			
            tbtrace( TM_INFO, _T("Use existing spHook"));
        }
        // e - POS248467 rrs

        return spHook;
    }    
}

BOOL CSolutionTBConnectPOS::Connect()
{
    try
    {
        GetHook()->Connect();
        return TRUE;
    }
    catch(_com_error & cex)
    {
        tbtrace(TM_ERROR, _T("Failed to Connect to the POS. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
    }
}



BOOL CSolutionTBConnectPOS::Disconnect()
{
    try
    {
        
        GetHook()->Disconnect();
        return TRUE;
        
    }
    catch(_com_error & cex)
    {
        tbtrace(TM_ERROR, _T("Failed to disconnect from the POS. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
    }
}

CString CSolutionTBConnectPOS::GetProperty(LPCTSTR szName)
{
	try
	{
		return (LPCTSTR)GetHook()->GetProperty(szName);
	}	
	catch(_com_error & cex)
	{			
		tbtrace(TM_INFO, _T("GetProperty: get property '%s' requires hook reference to be marshaled. Marshaling the hook."), szName);
		//POS277809 - force to get the hook marshaled if it has the error as the called is from different thread application (launchpad)
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{			
			try
			{
				return (LPCTSTR)GetHook(true)->GetProperty(szName);
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("Failed to retrieve property '%s' from the POS. HRESULT = 0x%08X:%s"), szName, cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
        return _T("");
	}
}

BOOL CSolutionTBConnectPOS::SetProperty(LPCTSTR szName, LPCTSTR szValue)
{
	try
	{
		GetHook()->SetProperty(szName, szValue);
		return TRUE;
	}
	catch(_com_error & cex)
	{	
		tbtrace(TM_INFO, _T("SetProperty: set property '%s' to value '%s' requires hook reference to be marshaled. Marshaling the hook."), szName, szValue);
		//POS277809 - force to get the hook marshaled if it has the error as the called is from different thread application (launchpad)
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{
			try
			{
				GetHook(true)->SetProperty(szName, szValue);
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("Failed to set property '%s' to value '%s' from the POS. HRESULT = 0x%08X:%s"), szName, szValue, cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::SendInput(LPARAM szInput)
{
	try
	{
		GetHook()->SendInput((LPCTSTR)szInput);
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_INFO, _T("SendInput: '%s' requires hook reference to be marshaled. Marshaling the hook."), (LPCTSTR)szInput);
		//CRD 237507 - force to get the hook marshaled if it has the error as the called is from different thread application (launchpad)
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{			
			try
			{
				GetHook(true)->SendInput((LPCTSTR)szInput);
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("SendInput failed to send '%s' to the POS. HRESULT = 0x%08X:%s"), (LPCTSTR)szInput, cex.Error(), (LPCTSTR)cex.ErrorMessage());
			}
		}
		
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::SendData(LPARAM szData)
{
	try
	{
		GetHook()->SendRawData((LPCTSTR)szData);
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("SendInput failed to send '%s' to the POS. HRESULT = 0x%08X:%s"), (LPCTSTR)szData, cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

bool CSolutionTBConnectPOS::InitEmulation()
{
    return true;
}

void CSolutionTBConnectPOS::SendMsgToPOS(CTBMsgObj &obj)
{
    tbtraceIO(_T("SendTBMsgObj"));

    int len=0;
    BYTE *pBuf= obj.Serialize(len);
    VARIANT v= CSafeArrayVariant::GetVariantFromBuffer(pBuf, len);
    delete [] pBuf;    
    
    SendControlMessage(v);
	SafeArrayDestroy( v.parray );
}

BOOL CSolutionTBConnectPOS::SetSelectedItem(LPCTSTR szEntryID)
{
	CCustomerTrxBroker::instance()->TBSetProperty(_T("LastEntryID"), szEntryID, false);
	SetProperty(_T("PropEntryId"), szEntryID);
	SendInput((LPARAM)_T("ENTRY_ID_ENTERED"));
	return TRUE;
}

BOOL CSolutionTBConnectPOS::SendControlMessage(VARIANT vArray)
{
	try
	{
		GetHook()->SendControlMessage(vArray);
		return TRUE;
	}	
	catch(_com_error & cex)
	{	
		tbtrace(TM_INFO, _T("SendControlMessage: requires hook reference to be marshaled. Marshaling the hook."));		
		//POS277809 - force to get the hook marshaled if it has the error as the called is from different thread application (launchpad)
		if( cex.Error() == RPC_E_WRONG_THREAD)
		{
			try
			{
				GetHook(true)->SendControlMessage(vArray);				
				return TRUE;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("SendControlMessage failed to send to the POS. HRESULT = 0x%08X:%s"), cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
			}
		}
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSValidateOperator(LPCTSTR szOperatorID, LPCTSTR szOperatorPw)
{
	try
	{
		VARIANT_BOOL vb = GetHook(true)->ValidateOperator(_bstr_t(szOperatorID), _bstr_t(szOperatorPw));
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSValidateOperator failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSValidateSupervisor(LPCTSTR szSupervisorID, LPCTSTR szSupervisorPw)
{
    
	try
	{
		VARIANT_BOOL vb = GetHook(true)->ValidateSupervisor(_bstr_t(szSupervisorID), _bstr_t(szSupervisorPw));
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSValidateSupervisor failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSExValidateEmployeeOperatorString(LPCTSTR szID, LPCTSTR szOperatorPw, ACS::ExEntryFlag entryFlag)
{
    try
	{
		VARIANT_BOOL vb = GetHook(true)->ExValidateEmployeeOperatorString(_bstr_t(szID), _bstr_t(szOperatorPw), entryFlag);
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{	
		tbtrace(TM_ERROR, _T("ACSExValidateEmployeeOperatorString failed, will retry with FORCE. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());	
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSExValidateEmployeeSupervisorString(LPCTSTR szID, LPCTSTR szSupervisorPw, ACS::ExEntryFlag entryFlag)
{
    try
	{
		VARIANT_BOOL vb = GetHook(true)->ExValidateEmployeeSupervisorString(_bstr_t(szID), _bstr_t(szSupervisorPw), entryFlag);
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSExValidateEmployeeSupervisorString failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());		
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSExValidateEmployeeOperatorData(LPCTSTR szID, LPCTSTR szData, ACS::ExEntryFlag entryFlag)
{
    try
	{
        _variant_t vData;

        // Decode Base64 encoded string and package into a byte array
        MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof(MSXML2::DOMDocument40));
        
        MSXML2::IXMLDOMElementPtr spElem = spDoc->createElement(_T("b64"));
        spDoc->appendChild(spElem);
        spElem->PutdataType(_T("bin.base64"));
        spElem->text = szData;
        vData = spElem->nodeTypedValue;
		VARIANT_BOOL vb = GetHook(true)->ExValidateEmployeeOperatorData(_bstr_t(szID), vData, entryFlag);
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSExValidateEmployeeOperatorData failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::ACSExValidateEmployeeSupervisorData(LPCTSTR szID, LPCTSTR szData, ACS::ExEntryFlag entryFlag)
{
    try
	{
        _variant_t vData;

        // Decode Base64 encoded string and package into a byte array
        MSXML2::IXMLDOMDocumentPtr spDoc(__uuidof(MSXML2::DOMDocument40));
        
        MSXML2::IXMLDOMElementPtr spElem = spDoc->createElement(_T("b64"));
        spDoc->appendChild(spElem);
        spElem->PutdataType(_T("bin.base64"));
        spElem->text = szData;
        vData = spElem->nodeTypedValue;
		VARIANT_BOOL vb = GetHook(true)->ExValidateEmployeeSupervisorData(_bstr_t(szID), vData, entryFlag);
		spHook = NULL;
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSExValidateEmployeeSupervisorData failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

BOOL CSolutionTBConnectPOS::SendAuxMsg(LPCTSTR pszAuxMsg, long lNotifyId)
{
    if( !pszAuxMsg ) return FALSE;

    tbtrace(TM_INFO, _T("SendAuxMsg('%s',%d)"), pszAuxMsg, lNotifyId);
    try
	{
        GetHook()->SendAuxMsg(pszAuxMsg, lNotifyId);
		return TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, 
            _T("SendAuxMsg failed to send to the POS. HRESULT = 0x%08X:%s"), 
            cex.Error(), 
            (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}


BOOL CSolutionTBConnectPOS::ACSExGetBusinessDate(long terminal, long * currentBusinessDate)
{
	try
	{
		VARIANT_BOOL vb = GetHook()->ExGetTerminalBusinessDay(terminal, currentBusinessDate);
		return vb == VARIANT_TRUE;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSValidateOperator failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
        return FALSE;
	}
}

// POS81632
int CSolutionTBConnectPOS::ACSCheckInOutOperator(int nFunc, LPCTSTR szOperatorID, LPCTSTR szOperatorPw, int nNewRAPid, int * currentRAPid, int * offlineStatus)
{
	try
	{
		int nRet = GetHook(true)->CheckInOutOperator(nFunc, _bstr_t(szOperatorID), _bstr_t(szOperatorPw), nNewRAPid, currentRAPid, offlineStatus);
		spHook = NULL; // POS293815
		return nRet;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSCheckInOutOperator failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
		//(+) POS300122
        if( cex.Error() == RPC_E_WRONG_THREAD)
		{
			try
			{
                int nRet = GetHook(true)->CheckInOutOperator(nFunc, _bstr_t(szOperatorID), _bstr_t(szOperatorPw), nNewRAPid, currentRAPid, offlineStatus);
				spHook = NULL;
				return nRet;
			}		
			catch (_com_error & cex2)
			{
				tbtrace(TM_ERROR, _T("ACSCheckInOutOperator failed. HRESULT = 0x%08X:%s"), cex2.Error(), (LPCTSTR)cex2.ErrorMessage());
	            return -1;
			}
		}
	   //(-) POS300122
	}
}

// POS97215
int CSolutionTBConnectPOS::ACSGetOperatorLastPwdChgTime(LPCTSTR szOperatorID, int * pYear, int * pMonth, int * pDay, int * pHour, int * pMinute, int * pSecond)
{
	try
	{
		int nRet = GetHook(true)->GetOperatorLastPwdChgTime(_bstr_t(szOperatorID), pYear, pMonth, pDay, pHour, pMinute, pSecond);
		spHook = NULL;
		return nRet;
	}
	catch(_com_error & cex)
	{
		tbtrace(TM_ERROR, _T("ACSGetOperatorLastPwdChgTime failed. HRESULT = 0x%08X:%s"), cex.Error(), (LPCTSTR)cex.ErrorMessage());
		return -1;
	}
}

