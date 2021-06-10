//*************************************************************************************
//  SolutionTrxBroker.cpp	:	Definition file for the CSolutionTrxBroker class
//*************************************************************************************

#include "stdafx.h"

#include "SolutionTrxBroker.h"
#include "CustomerTrxBroker.h"
#include "traceSystem.h"
#define SOLUTION_TBSTATES
#include "CustomerTBSharedDefs.h"
#include "TBUtilities.h"
#include "TBInProgress.h"
#include "customerTBProperties.h"
#include "CustomerTBConnectFL.h"
#include "CustomerMHOptions.h"
#include "CustomerMHItem.h"
#include "CustomerMHCashDrawer.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerCurrentSigCap.h"
#include "CustomerTBScotMsg.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentItem.h"
#include "ScannerMsg.h"
#include "CustomerMHReceipt.h"
#include "CustomerMHOperator.h"

#define T_ID    _T("CSolutionTrxBroker")

const CString ACS_POS(_T("c:\\acs\\bin\\salesw2k.exe")); 
bool CSolutionTrxBroker::bLaunchedPOS = false;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSolutionTrxBroker::CSolutionTrxBroker() :
	m_bRemoteAssist( false ),
	m_bLimitedAssistMode( false ),
    m_csFLNextState(_T("")),
    m_bTenderInProgress(false),
    m_bRewardItem(false),
    m_bCheckPaymentAtLaneAvailable(false)
{
	SetCurrentInProgressState(_T("TERM_INIT"));
    m_oleMsgFilter.EnableNotRespondingDialog(FALSE);
    m_oleMsgFilter.EnableBusyDialog(FALSE);
    m_oleMsgFilter.SetRetryReply(250);
    m_oleMsgFilter.SetMessagePendingDelay(15000);
    if( !m_oleMsgFilter.Register() )
    {
        tbtrace(TM_ERROR, _T("SolutionTrxBroker(): COleMsgFilter failed to register"));
    }
}

CSolutionTrxBroker::~CSolutionTrxBroker()
{
}

// Note that this function may be called more than once without uninitialize 
TBRC CSolutionTrxBroker::TBInitialize(HWND hWnd, LPCTSTR szFilePath)
{
    TBRC rc = TB_SUCCESS;
    tbtraceIO(_T("TBInitialize"));

    CSolutionTBConnectPOS::IsTerminating(FALSE); // static call

    try {
        CBaseTrxBroker::TBInitialize(hWnd, szFilePath);
    }
    catch(...) {
        tbtrace(TM_ERROR,_T("Initialize Failed"));
        return TB_E_NOTINITIALIZED;
    }
    

    return rc;
}


TBRC CSolutionTrxBroker::TBUninitialize(void)
{
    TBRC rc = TB_SUCCESS;
    tbtraceIO(_T("TBUninitialize"));

    try
    {
        CSolutionTBConnectPOS::IsTerminating(TRUE); // static call
        CCustomerTBConnectPOS::instance()->StopHookThread();
    }
    catch(...)
    {
        tbtrace(TM_ERROR,_T("Exception occured while attempting to stop the hook thread."));
        rc = TB_E_UNEXPECTED;
    }
    
	try 
	{
		rc = CBaseTrxBroker::TBUninitialize();
    }
    catch(...) 
	{
        tbtrace(TM_ERROR,_T("Uninitialize Failed"));
        rc = TB_E_UNEXPECTED;
    }

    return rc;
}

TBRC CSolutionTrxBroker::TBLaneClosed(const bool bEnter)
{
    tbtraceIO(_T("TBLaneClosed()"));	
    
	if(bEnter)
	{
		
		//change the current TB state and let table drive a open
		SetCurrentInProgressState(_T("TB_CLOSELANE"));
		TriggerMsgMap();

	}else
	{
		SetCurrentInProgressState(_T("TB_OPENLANE"));
		TriggerMsgMap();
	}

	return TB_SUCCESS;
}


TBRC CSolutionTrxBroker::SendKeyedData(va_list pArgs)
{
    tbtraceIO( _T("SendKeyedData()"));
	CString csSequence;
    TBRC retCode = TB_SUCCESS;
    try
    {
        csSequence = va_arg( pArgs, CString);
		if (csSequence.GetLength())
		{
			CCustomerTBConnectPOS::instance()->SendSequence(csSequence);
		}
    }
    catch (...)
    {
        retCode = TB_E_UNEXPECTED;
    }

    return retCode;
}

TBRC CSolutionTrxBroker::GetDisplayXML(va_list pArgs)
{
    tbtraceIO( _T("GetDisplayXML()"));
	TCHAR* strDisplay;
	TCHAR** strDisplayPtr;
	CString csDisplay = TBGetProperty(_T("POSDisplay"));
	tbtrace(TM_ERROR, _T("XML=%s"), (LPCTSTR)csDisplay);
	try
	{
		strDisplay = new TCHAR[csDisplay.GetLength() + 1];
		_tcscpy(strDisplay, csDisplay);
		strDisplayPtr = va_arg(pArgs, TCHAR**);
		*strDisplayPtr = strDisplay;
		return TB_SUCCESS;
	}
	catch (...)
	{
		return TB_E_UNEXPECTED;
	}
}

/* -- Deprecated. Use SetSelectedItem(LPCTSTR szEntryID instead --*
TBRC CSolutionTrxBroker::SetSelectedItem(va_list pArgs)
{
    tbtraceIO( _T("SetSelectedItem()"));
	CString csEntryID;
    TBRC retCode = TB_SUCCESS;
    try
    {
        csEntryID = va_arg( pArgs, CString);
		if (csEntryID.GetLength())
		{
			tbtrace(TM_INFO, _T("SetSelectedItem to %s"), csEntryID);
			CCustomerTBConnectPOS::instance()->SetSelectedItem(csEntryID);
		}
		else
		{
			tbtrace(TM_INFO, _T("Invalid zero-length Item ID"));
		}
    }
    catch (...)
    {
        retCode = TB_E_UNEXPECTED;
    }
	return retCode;
}
* -- Deprecated. Use SetSelectedItem(LPCTSTR szEntryID instead --*/

TBRC CSolutionTrxBroker::SetSelectedItem(LPCTSTR szEntryID)
{
    tbtraceIO( _T("SetSelectedItem()"));
    TBRC retCode = TB_SUCCESS;
    try
    {
		if (szEntryID && szEntryID[0])
		{
			tbtrace(TM_INFO, _T("SetSelectedItem to %s"), szEntryID);
			CCustomerTBConnectPOS::instance()->SetSelectedItem(szEntryID);
		}
		else
		{
			tbtrace(TM_INFO, _T("Invalid zero-length Item ID"));
		}
    }
    catch (...)
    {
        retCode = TB_E_UNEXPECTED;
    }
	return retCode;
}

TBRC CSolutionTrxBroker::SendTBCommand(long lCommandID, va_list pArgs)
{
    tbtraceIO( _T("SendTBCommand()"));
	
	CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
	TBRC rcRet = TB_SUCCESS;
	
    try
    {
        switch(lCommandID)
        {
        case TBCMD_POS_READY:
			{   
                tbtraceIO(_T("TBCMD_POS_READY"));
				// FastLane now ready to communicate with POS, make sure we have started it
				if (!bLaunchedPOS)
				{
                    tbtrace(TM_INFO, _T("CCustomerTBConnectPOS::instance()->Connect()"));
					BOOL bConnect = CCustomerTBConnectPOS::instance()->Connect();
					ASSERT(bConnect);
					if ( bConnect )
					{
                        tbtrace(TM_INFO, _T("LaunchApp"));
						LaunchApp(ACS_POS);
						bLaunchedPOS = true;
					}
				}
                if((csCurTBState == _T("TERM_INIT_READY"))  || 
                   (csCurTBState == _T("SIGNOUT_COMPLETE")) ||  
                   (csCurTBState == _T("TB_CLOSELANE")))
				{
					return 1;  // 1 true
				}else
				{
					return 0;  // 0 false
				}
			}
			break;

		case TBCMD_POS_SIGNEDIN:
			if((csCurTBState == _T("SIGNON_COMPLETE")) || 
			   (csCurTBState.Left(11) == _T("ITEMIZATION")) )
			{
				return TB_SUCCESS_IMMEDIATE;
			}else
			{
				return TB_SUCCESS;
			}
			break;
		case TBCMD_SENDKEYEDDATA:
			return SendKeyedData(pArgs);
		case TBCMD_GETDISPLAYXML:
			return GetDisplayXML(pArgs);
		case TBCMD_SETSELECTEDITEM:
            {
            CString csEntryID = va_arg(pArgs, CString);
			return SetSelectedItem(csEntryID);
            }
        case TBCMD_GET_CUSTOMER_SELECTED_TENDER:
            {
                CString *csRetStr;
                csRetStr = va_arg(pArgs, CString* );
            
                csRetStr ->Empty();
                csRetStr->Insert(0, TBGetProperty(_T("TenderType"), true));
                csRetStr->TrimLeft();
                csRetStr->TrimRight();

                if((csRetStr->IsEmpty()) || (csRetStr->Find(_T("Dept")) != -1 ))
                {
                    return 0;
                }else
                {
                    //don't tell FL we have a tender until we're in the right state
                    CString csCurrTBState = GetCurrentInProgressStateName();
                    if((csCurrTBState != _T("TB_ENTERTENDERSTATE_IN")) && (csCurrTBState != _T("ITEMIZATION")))
                        return 1;
                    return 0;
                }
            } 
        case TBCMD_GET_POS_ERROR_NUM:            
     		if((csCurTBState == _T("RESTART_FASTLANE")))
			{
				TriggerMsgMap();
                return TB_SUCCESS;
			}
            else if(csCurTBState == _T("CRITICAL_ERROR") || (csCurTBState == _T("TERM_DISABLED")))
            {
               CCustomerTBConnectFL &tbConnectFL = *CCustomerTBConnectFL::instance();

                SMessageInfo msgMessageObj;
                memset(&msgMessageObj,0,sizeof(SMessageInfo));
                tbConnectFL.GetMessageInfo(&msgMessageObj);

               
                CString *csRetStr;
                csRetStr = va_arg(pArgs, CString* );

                csRetStr->Insert(0, msgMessageObj.szLeadThruText);                        
                                       
                va_end( pArgs ); 

                if(csRetStr->IsEmpty())
                    return TB_SUCCESS;
                else
				{
					if(csCurTBState == _T("TERM_DISABLED"))
					{
						SetCurrentInProgressState(_T("RESTART_FASTLANE"));
					}
                    return TB_SUCCESS_IMMEDIATE;
				}
            }
            return 0;              
        case TBCMD_ENTER_ASSISTMODE:
            {
                bool bRemote = va_arg(pArgs, bool );
                SetInRemoteAssistMode( bRemote );
                //check if we were about to finish a trxn before we blindly change states
                if(csCurTBState == _T("EXIT_TRAINING_IP"))
                {
                    SetCurrentInProgressState(_T("ASSISTMODE_EXIT_TRAINING_IP"));

                }
				else if(csCurTBState == _T("ENTER_TRAINING_IP"))
                {   
                    SetCurrentInProgressState(_T("ASSISTMODE_ENTER_TRAINING_IP"));

                }
				else if(csCurTBState == _T("TB_FINISH_FLAPI"))
                {
                    SetCurrentInProgressState(_T("ASSISTMODE_FINISH_FLAPI"));
                }
				else if ( csCurTBState == _T("ASSISTMODE_TRXEND") )
				{
					rcRet = TB_SUCCESS_IMMEDIATE;
				}
				else if ( csCurTBState == _T("ASSISTMODE_TRXEND_SIGSLIP") ) 
				{
					TBSTATE tbRet = TB_IGNORE;
					CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbRet, NULL );
					if ( tbRet != TB_IGNORE )
					{
						CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
					}
				}
                else if(csCurTBState == _T("TENDER_CASH"))
                {
                        SetCurrentInProgressState(_T("ASSISTMODE_TENDER_CASH"));
                }
				else
                {
                    SetCurrentInProgressState(_T("ASSISTMODE_ENTER"));
                }
                TriggerMsgMap();
            }
            return rcRet;
        case TBCMD_EXIT_ASSISTMODE:
	    {
            SetInRemoteAssistMode( false );
            SetLimitedAssistMode( false );
			//TAR #303018
			// 
			//You either do the tender or you don't while in assist mode.
			//Either way, you exit to where you came from.
			//The tender is not on-the-fly as we leave assist mode.
			//If the tender finished the transaction, that is covered below.
			//Otherwise, go back to where we came from!
            //if(csCurTBState == _T("ASSISTMODE_TENDER"))
            //{   
            //    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
            //    TriggerMsgMap();
            //    return 1;
            //}
			//TAR #303076
			//BUT, if we came from FL tendering, that is where we will return.
			CString csFLState = TBGetProperty(_T("FLState"));
			if (csFLState == _T("TENDER"))
			{
				if(csCurTBState == _T("ASSISTMODE_TENDER"))
				{   
				    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
				    TriggerMsgMap();
				    return 1;
				}
			}
            if( (csCurTBState != _T("ASSISTMODE_TRXEND")) &&
				(csCurTBState != _T("ASSISTMODE_TRXEND_SIGSLIP")) &&
				(csCurTBState != _T("TB_FINISH_FLAPI")) && 
				(csCurTBState != _T("ITEMIZATION")) )
            {
                SetCurrentInProgressState(_T("ASSISTMODE_EXIT"));
                TriggerMsgMap();
                return 1;
            }
            return 0;
	    }
        case TBCMD_ASSISTMODE_BUTTON_ALLOWED:
            {
                CString csTmp = va_arg(pArgs, CString );
                csTmp.TrimLeft();
                csTmp.TrimRight();

                CString csAssistModeBtn = _T(";") + csTmp + _T(";");
                va_end( pArgs );
                
                CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
                CString csDisallowedBtn;
                if ( IsLimitedAssistMode() )
                {
                    csDisallowedBtn = pOpts->OptionValue(_T("AssistModeDisallowedKeys_Limited"));
                }
                else
                {
                    if ( InRemoteAssistMode() )
                    {
                         csDisallowedBtn = pOpts->OptionValue(_T("AssistModeDisallowedKeys_Remote"));
                    }
                    else
                    {
                        csDisallowedBtn = pOpts->OptionValue(_T("AssistModeDisallowedKeys"));
                    }
                }
                       

                if(csDisallowedBtn[0] != _T(';'))
                {
                    csDisallowedBtn = _T(';') + csDisallowedBtn;
                }

                if(csDisallowedBtn[csDisallowedBtn.GetLength() - 1] != _T(';'))
                {
                    csDisallowedBtn += _T(';');
                }            

                if(csDisallowedBtn.Find(csAssistModeBtn) != -1)
                {
                    return 1;
                }else
                {
                    return 0;
                }

                return 0;
                
            }
        case TBCMD_SET_TB_STATE:
            {
                CString csTmp = va_arg(pArgs, LPCTSTR );
                va_end( pArgs );
                csTmp.TrimLeft();
                csTmp.TrimRight();
                SetCurrentInProgressState(csTmp);
                TriggerMsgMap();
            }

            return 1;
		case TBCMD_CASHDRAWER_FAILED:
			tbtrace( TM_ERROR, _T("Cash Drawer failure at RAP"));
            // Let FastLane control the POS cash drawer
            CCustomerMHCashDrawer::instance()->SetCloseImmediate( true );
			return TB_SUCCESS;
        
        case TBCMD_HIDEINPUT:
            {
                CString csProp = TBGetProperty(_T("HideInput"), true); 
                csProp.TrimLeft();
                csProp.TrimRight();

                if(csProp == _T("1"))
                    return 1;

                return 0;
            }
        case TBCMD_CHECK_FORCE_LOAD:
            {
                //check if we have a pending Force Load or Store Close
                CString csProp = TBGetProperty(_T("ForceLoadRequested"), true); 
                csProp.TrimLeft();
                csProp.TrimRight();

                if(csProp == _T("1"))
                {
                    SetCurrentInProgressState(_T("FORCE_LOAD"));
                    TriggerMsgMap();
                    return 1;
                }
                
				//+CRD185924
				CString csPosState = TBGetProperty(_T("SavedLastPosState"), false);
				tbtrace( TM_INFO, _T("csPosState = %s"), csPosState);

				//CString csState = GetCurrentInProgressStateName();

				CString csNewPrompt = TBGetProperty(_T("IsNewPOSSoftwareAvailable"), true);
				bool bIsNeedNewSoftwarePrompt(csNewPrompt == _T("1") && csPosState == _T("NEW_POS_SOFTWARE_AVAILABLE"));

				csProp = TBGetProperty(_T("StoreCloseRequested"), true);
				csProp.TrimLeft();
				csProp.TrimRight();
				if (csProp == _T("1") || bIsNeedNewSoftwarePrompt)
				{
					if (bIsNeedNewSoftwarePrompt)
					{
						TBSetProperty(_T("IsNewPOSSoftwareAvailable"), _T("0"), true);
						tbtrace(TM_INFO, _T("Just re-using the STORE_CLOSE_REQUESTED for POS quick retart."));
					}

					SetCurrentInProgressState(_T("STORE_CLOSE_REQUESTED"));
					TriggerMsgMap();
					return 1;
				}
                //-CRD185924
                
                csProp = TBGetProperty(_T("SeparateMedia"), true); 
                csProp.TrimLeft();
                csProp.TrimRight();
                if(csProp == _T("1"))
                {
                    SetCurrentInProgressState(_T("SEPARATE_MEDIA"));
                    TBSetProperty(_T("SeparateMedia"), _T(""),true);
                    TriggerMsgMap();
                    return 1;
                }
                csProp = TBGetProperty(_T("SeparateMediaExceptLastTrxn"), true); 
                csProp.TrimLeft();
                csProp.TrimRight();
                if(csProp == _T("1"))
                {
                    SetCurrentInProgressState(_T("SEPARATE_MEDIA_ALL_BUT_LAST_TRXN"));
                    TBSetProperty(_T("SeparateMediaExceptLastTrxn"), _T(""),true);
                    TriggerMsgMap();
                    return 1;
                }
                

                return 1;
            }
		case TBCMD_GET_BITMAP:
			{
				BSTR* pData = va_arg(pArgs, BSTR* );
				CCustomerCurrentSigCap::instance()->GetSlipBitmap( pData );
				return 1;
			}
        case TBCMD_TENDER_IN_PROGRESS:
            {
                bool bRet = IsTenderInProgress();
                tbtrace( TM_INFO, _T("IsTenderInProgress returns %d"), bRet );
                return bRet;
            }
        case TBCMD_ANY_REWARD_CARDS:
            {
                return m_bRewardItem;

            }
        case TBCMD_SEND_ANYRESPONSE_TO_POS:
            {
                CString csResponse = TBGetProperty(_T("ResponseCodeToSendToPOS"), true); 
                csResponse.TrimLeft();
                csResponse.TrimRight();
                if (csResponse != _T(""))
                {
           			CCustomerTBConnectPOS::instance()->SendSequence(csResponse);
                    TBSetProperty(_T("ResponseCodeToSendToPOS"), _T(""),true);
                    return 1;
                }
                return 0;
            }
        case TBCMD_IS_CUST_REQUEST_CREDIT_ENABLED:
            {
                CString csProp = TBGetProperty(_T("CustRequestCreditBtn"), true); 
                csProp.TrimLeft();
                csProp.TrimRight();
                
                //reset the tender type. we no longer need it
                TBSetProperty(_T("TenderType"), _T(""),true);

                if(csProp != _T("0"))
                {
             
                
                    return 0;
                }
                
                return 1;
            }
        case TBCMD_ENTERING_FINISH_AND_PAY:
            {
                CString csCurrentTBState= GetCurrentInProgressStateName();               
                //CString csLastPosState =  CCustomerMsgMap::instance()->GetLastPosState();

                if(csCurrentTBState == _T("ITEMIZATION"))
                {
                    tbtrace( TM_ERROR, _T("Fastlane is at Finish and Pay while TB is in itemization"));
                    SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));                    
                    //TriggerMsgMap();
                    return true;
                }

                return 0;
            }
        case TBCMD_DUPLICATE_RECEIPT_PRNT:
			{
			    SetCurrentInProgressState(_T("DUPLICATE_RECEIPT_PRNT"));
				SendKeyedData(pArgs);				
				return 1;
			}
        case TBCMD_GET_OPTIONS:
	     {
    
            CString * pstrOption = va_arg(pArgs, CString*);
		    CString * pstrValue = va_arg(pArgs, CString*);

		    CCustomerMHOptions* pOpts = CCustomerMHOptions::instance();
            *pstrValue = pOpts->OptionValue(*pstrOption);

		    return 1;
	     }

        case TBCMD_GET_AGEREQUIRED:
         {

             CString *csRetStr;
             csRetStr = va_arg(pArgs, CString* );

             csRetStr ->Empty();
             csRetStr->Insert(0, TBGetProperty(_T("RestrictedAge_Prop"), true));
             csRetStr->TrimLeft();
             csRetStr->TrimRight();

             if(csRetStr->IsEmpty())
             {
                 return 0;
             }   
             else
             {
                 return 1;
             }

         }

        case TBCMD_MSGVOIDABLE:
		  {
             if (_ttol(TBGetProperty(_T("PropertyAllowMSGVoid"), true)) != 0)
			 {
                 return 1;
			 }
			 else
			 {
                 return 0;
			 }
		  }
        case TBCMD_SENDSCANDATA:
			{
			
				return (SendScanData(pArgs));
				
			}

        case TBCMD_MBMP_ENABLED:
            {
                bool * pbEnabled = va_arg(pArgs, bool*);
                *pbEnabled = CCustomerMHReceipt::instance()->GetMultiBitmap().IsEnabled();
                return TB_SUCCESS;
            }
        case TBCMD_GET_MBMP_SYNCRAP:
            {
                bool * pbSyncRAP = va_arg(pArgs, bool*);
                *pbSyncRAP = CCustomerMHReceipt::instance()->GetMultiBitmap().SyncRAP();
                return TB_SUCCESS;
            }
        case TBCMD_MBMP_GET_FLREQ:
        case TBCMD_MBMP_GET_RAPREQ:
            {
                CSolutionMultiBitmap::MBMP_REQ req;
                bool bRet;
                CSolutionMultiBitmap & mbmp = 
                    CCustomerMHReceipt::instance()->GetMultiBitmap();
                
                if( lCommandID == TBCMD_MBMP_GET_FLREQ )
                {
                    bRet = mbmp.GetFastLaneRequest(req);
                }
                else
                {
                    bRet = mbmp.GetRAPRequest(req);
                }
                
                if( bRet )
                {
                    long * plIndex = va_arg(pArgs, long*);
                    long * plStation = va_arg(pArgs, long*);
                    CString * pcsFile = va_arg(pArgs, CString*);
                    long * plWidth = va_arg(pArgs, long*);
                    long * plAlign = va_arg(pArgs, long*);
                    
                    *plIndex = req.lIndex;
                    *plStation = req.lStation;
                    *pcsFile = req.csFilename;
                    *plWidth = req.lWidth;
                    *plAlign = req.lAlign;
                    return TB_SUCCESS_IMMEDIATE;
                }
                return TB_SUCCESS;
            }
            
        case TBCMD_MBMP_UPD_FLREQ:
        case TBCMD_MBMP_UPD_RAPREQ:
            {
                long lIndex = va_arg(pArgs, long);
                long lStation = va_arg(pArgs, long);
                bool bSucceeded = va_arg(pArgs, bool);
                CString csResult = va_arg(pArgs, CString);
                CSolutionMultiBitmap & mbmp = 
                    CCustomerMHReceipt::instance()->GetMultiBitmap();
                bool bRet = false;
                if( lCommandID == TBCMD_MBMP_UPD_FLREQ )
                {
                    bRet = mbmp.UpdateFastLaneRequest(lIndex, lStation, bSucceeded, csResult);
                }
                else
                {
                    bRet = mbmp.UpdateRAPRequest(lIndex, lStation, bSucceeded, csResult);
                }
                return bRet ? TB_SUCCESS : TB_E_UNEXPECTED;
            }
        case TBCMD_GET_TB_STATE:
            {
                CString * pcsTBState = va_arg(pArgs, CString*);
                *pcsTBState = csCurTBState;
                return TB_SUCCESS;
            }
        // Support for check tendering with second printer at FL (FL 4.0 Patch F+)
        case TBCMD_SET_CHECK_AT_LANE_AVAILABLE:
            {
            bool bCheckAtLane = va_arg(pArgs, bool);
            
            SetCheckPaymentAtLaneAvailable(bCheckAtLane);
            return TB_SUCCESS;
            }
        default:
			{
            return CBaseTrxBroker::SendTBCommand(lCommandID, pArgs);
			}
        }
    }
    catch(...)
    {
        tbtrace(TM_ERROR, _T("An exception occured."));
        return TB_E_UNEXPECTED;
    }
}

/*************************************************************************
* GetNextState
************************************************************************/
TBRC CSolutionTrxBroker::GetNextState(LPCTSTR lpLastStateName, LPCTSTR lpNextStateName,
                              bool *pbOverride, LPTSTR *lpOverrideStateName, CDescriptorList &dlParameterList)
{
    TBRC rc= CBaseTrxBroker::GetNextState(lpLastStateName,  lpNextStateName,
                               pbOverride, lpOverrideStateName, dlParameterList);
    
    //preserve the FL state so we can use it in the TB if needed
    CString csTemp = *lpOverrideStateName;
    if((*pbOverride) && (!csTemp.IsEmpty()))
    {
        m_csFLNextState = csTemp;
    }else
    {
        m_csFLNextState = lpNextStateName;
    }

    //if we are trying to exit FL at a lane close then allow it    
    if((CString(lpLastStateName) == _T("SMLaneClosed"))      && 
       (CString(lpNextStateName) == _T("SMOutOfService")))  
    {
        
        *pbOverride = false;
        return rc;
    }

    //if we are going to or comming out of laneclose put FL in outservices temp. until POS is ready
    if((CString(lpLastStateName) == _T("SMLaneClosed"))      && 
       (CString(lpNextStateName) != _T("SMOutOfService2")))  
    {
        CCustomerTBProperties &tbProp= *CCustomerTBProperties::instance();
        CString csOverrideStateName= _T("SMOutOfService2");

        *pbOverride = true;
        *lpOverrideStateName = new TCHAR[csOverrideStateName.GetLength() +1];
        _tcscpy(*lpOverrideStateName, (LPCTSTR)csOverrideStateName);
        tbProp.SetProperty(TBPROP_NEWFLSTATE, _T(""));
        
        m_csFLNextState = csOverrideStateName;
        return rc;
    }
    CTBInProgress *pTBState = CTBInProgress::instance();
    if (CString(lpNextStateName) == _T("SMScanAndBag")) 
    {
        
        CString csTBStateType= pTBState->GetType(GetCurrentInProgressState());
        if (csTBStateType == _T("ITEMIZATION_IP"))
        {
            CString csCurrentTBState= GetCurrentInProgressStateName();
            tbtrace(TM_WARNING, _T("FastLane leaving TB in an ITEMIZATION_IP. Resync"));
            SetCurrentInProgressState(csCurrentTBState + CString(_T("_")) + lpNextStateName);
            TriggerMsgMap();
        }
    }

    
    if ((CString(lpNextStateName) == _T("SMFinish")) && (TBGetProperty(_T("SignatureRequested"), true)==_T("1")))
    {
        
        //we have the situation where the pinpad is at the sign screen and FL timedout waiting for the POS
        //alert the cashier
        SetCurrentInProgressState(_T("TENDER_SIG_TIMEOUT"));
        TriggerMsgMap();
    }

	//
	// G. Doss  Commented Out Start 
	//    The following lines were causing problems with the exit of AM after
	//    the end of a transaction.
    //if (CString(lpNextStateName) == _T("SMFinish")) 
    //{
    //    CString csTBStateType= pTBState->GetType(GetCurrentInProgressState());
	//	
    //    if (csTBStateType == _T("ITEMIZATION"))
    //    {
    //        tbtrace(TM_WARNING, _T("FastLane returning to SMFinish and TB set for Itemization-- adjusting to tender state"));
    //        SetCurrentInProgressState(_T("TB_ENTERTENDERSTATE_IN"));
    //        TriggerMsgMap();
    //    }
    //}
	// G. Doss  Commented Out End
    return rc;
}


TBRC CSolutionTrxBroker::TBFinish()
{
	CCustomerMHItem::instance()->ResetNumberOfItemsSold();
	return CBaseTrxBroker::TBFinish();
}


bool  CSolutionTrxBroker::InRemoteAssistMode( void)
{
    return m_bRemoteAssist;
}

void CSolutionTrxBroker::SetInRemoteAssistMode( bool bRemote )
{
    m_bRemoteAssist = bRemote;
 
    if ( CCustomerMHOptions::instance()->BoolOptionValue( _T("TB_FT_SUPPORT_RAPTENDER") ) )
    {
        if ( bRemote )
        {
            // Let AssistMode control the POS cash drawer
            CCustomerMHCashDrawer::instance()->SetCloseImmediate( false );
        }
        else
        {
            // Let FastLane control the POS cash drawer
            CCustomerMHCashDrawer::instance()->SetCloseImmediate( true );
        }
    }
}

bool CSolutionTrxBroker::IsLimitedAssistMode()
{
    return m_bLimitedAssistMode;
}

void CSolutionTrxBroker::SetLimitedAssistMode( bool bVal )
{
    m_bLimitedAssistMode = bVal;
}


/*************************************************************************
* PreprocessSignatureData - 
* Parameters: none
*
* Returns: void
*************************************************************************/
TBRC CSolutionTrxBroker::PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
	tbtraceIO( _T("PreprocessSignatureData") );
	TBRC nRetCode;
	nRetCode = CCustomerCurrentSigCap::instance()->PreprocessSignatureData( lpFileName, bstrSignatureData, lpReceiptType, bProcessData );
	return nRetCode;   
}

LPCTSTR CSolutionTrxBroker::GetFLNextStateName(void)
{
    return (LPCTSTR)m_csFLNextState;
}


void CSolutionTrxBroker::TBSayPhrase(LPCTSTR szWaveFileName)
{
    CString csWaveFileName = szWaveFileName;
    PSMessageInfo pMsg = new SMessageInfo;
	ZeroMemory(pMsg, sizeof(SMessageInfo));
	
    CopyString(pMsg->szPlayWave, csWaveFileName);  

	CCustomerTBConnectFL::instance()->SetMessageInfo(pMsg);
    delete [] pMsg->szPlayWave;
    delete pMsg;

    CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_MESSAGESCREEN, NULL, NULL); 
}

TBRC CSolutionTrxBroker::SendScanData(va_list pArgs)
{
    tbtraceIO( _T("SendScanData()"));
	//CString csItemCode;
	//long lScanCodeType;
    TBRC retCode = TB_SUCCESS;
    try
    {
		CCustomerCurrentItem *pCurItem= CCustomerCurrentItem::instance();
		CScannerMsg scan;
		scan.BarcodeType((CScannerMsg::BarcodeTypes)va_arg(pArgs, long));
		scan.Data(va_arg(pArgs, CString));

		CCustomerTBConnectPOS::instance()->SendMsgToPOS(scan);
        //csSequence = va_arg( pArgs, CString);
		//if (csSequence.GetLength())
		//{
		//	CCustomerTBConnectPOS::instance()->SendSequence(csSequence);
		//}
    }
    catch (...)
    {
        retCode = TB_E_UNEXPECTED;
    }

    return retCode;
}



LPCTSTR CSolutionTrxBroker::GetTBSTATEName(long key)
{

    for (int i=0; SolutionTBStates[i].key != TB_MAXSOLUTIONSTATEID; i++) {
        if (SolutionTBStates[i].key == key)
            return SolutionTBStates[i].szname;
    }
    
    return CBaseTrxBroker::GetTBSTATEName(key);
}

long CSolutionTrxBroker::GetTBSTATEFromName(LPCTSTR szTBState)
{
    
    for (int i=0; SolutionTBStates[i].key != TB_MAXSOLUTIONSTATEID; i++) {
        if (_tcscmp(SolutionTBStates[i].szname, szTBState) == 0)
            return SolutionTBStates[i].key;
    }

    return CBaseTrxBroker::GetTBSTATEFromName(szTBState);
}


bool CSolutionTrxBroker::IsCheckPaymentAtLaneAvailable()
{
    return m_bCheckPaymentAtLaneAvailable;
}

void CSolutionTrxBroker::SetCheckPaymentAtLaneAvailable(bool bAvailable)
{
    m_bCheckPaymentAtLaneAvailable = bAvailable;
}

BOOL CSolutionTrxBroker::TBValidateHeadCashier( LPCTSTR szCashierID, LPCTSTR szCashierPw )
{
   tbtraceIO(_T("TBValidateHeadCashier()"));
   CString csPrefixBase64(_T("BASE64:"));
   
   CString csID(szCashierID);
   CString csPW(szCashierPw);
   
   if(csID.Find(csPrefixBase64) != -1)
   {
	   //tbtrace(L0, _T("ValidateOperator encoded(%s,%s)"), szCashierID,szCashierPw);
	   
	   csID.Replace(csPrefixBase64,_T(""));
	   csPW.Replace(csPrefixBase64,_T(""));
	   
	   csID = DecodeStringBase64(csID);
	   csPW = DecodeStringBase64(csPW);
    }

   return CCustomerMHOperator::instance()->ValidateSupervisor(csID, csPW);

}