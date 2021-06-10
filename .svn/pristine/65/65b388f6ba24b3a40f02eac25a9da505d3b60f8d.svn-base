//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementExBase.CPP
//
// TITLE: Class implementation for store mode CashManagement state
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"                 // MS MFC always needed first
#include <assert.h>

#include "Common.h"                 // Common includes
#include "SMSmCashManagementEx.h"      
#include "CMInterface.h"
#include "SMSmMediaStatus.h"
#include "OposChan.h"
#include "GloryOpos.h" // SSCOP-2132
#include "Base64EncodeDecode.h"
#include "DMCashCount.h"
DLLEXPORT extern DMCashCount  dmcc;    // Global DM object

BEST SMSmCashManagementExBase::bestSaved = BES_STATERETURN;
IPSProcedures::HKStatus SMSmCashManagementExBase::m_HKStatus = IPSProcedures::HK_NORMAL;
bool SMSmCashManagementExBase::m_bShiftOn = false;

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmCashManagementEx")

#include "MethodTrace.h"

IMPLEMENT_DYNCREATE(SMSmCashManagementExBase, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP


//////////////////////////////////
SMSmCashManagementExBase::SMSmCashManagementExBase()
: m_pSMAccessor(new SMStoreModeStateBase())
{
    IMPLEMENT_TIMESTAMP
}

SMSmCashManagementExBase::~SMSmCashManagementExBase()
{
    delete m_pSMAccessor;
    m_pSMAccessor = NULL;
} 
SMStateBase  *SMSmCashManagementExBase::Initialize(void)
{
	//+SSCOADK-898
	//Cash Device Error must be corrected first before proceeding to Cash Management
    SMStateBase *sReturnState = CkStateStackAndReturn();
    if (sReturnState != STATE_NULL )
    {
        trace(L6, _T("Device Error must be corrected first before proceeding to Cash Management"));
        return sReturnState;
    }
    nTimeOut = 0; // needed to disable state from timing out.
	//-SSCOADK-898

    TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmCashManagementEx") );    // RFC 330050

    if (getAnchorState() != BES_CASHMANAGEMENTEX)
        bestSaved = getAnchorState();

    setAnchorState(BES_CASHMANAGEMENTEX);

    rp.SetCashManagementInProgress(true);  // TAR 450658

	dm.CashRecyclerPauseCounts(true);	// SSCOP-2197

    inputMode = OPERATOR_PASSWORD_ACCEPTED; 
    m_csOperatorLoggedIn = csOperator;
    m_csPasswordLoggedIn = csPassword;

    CString csTempId = csOperator;
    CString csTempPw = csPassword;
    if(co.GetfOperationsEncodeOperatorIDPW())
    {
        CString csPrefix(_T("BASE64:"));
        CBase64EncodeDecode base64;
        csTempId = base64.GetBase64EncodeWithPrefix(csPrefix, csTempId);
        csTempPw = base64.GetBase64EncodeWithPrefix(csPrefix, csTempPw);
    } 

    //TAR 448439 : Pass credentials first before entering state.
    // Pass credentials down to CM.
    m_cmMgr.SetCredentials(csTempId, csTempPw);

    m_cmMgr.EnterState();
	
    ps.SetInXM(true);	//SSCOADK-1021
    //m_cmMgr.SetInTransaction(fSAInTransaction);	//SSCOP-436

    return STATE_NULL;
}

SMStateBase *SMSmCashManagementExBase::PSParse( const MessageElement* pMsg)
{
    TRACE_METHOD(_T("SMSmCashManagementBaseEx::PSParse"));

    CString csContext = _T("");
    switch(pMsg->psInfo.nEvt)
    {
    case PS_CMDATA:
        trace(L6, _T("Got CM data!  CTX: [%s]"), pMsg->psInfo.pszContext);

        // Pass event down to CM DLL to be handled.
        m_cmMgr.HandleEvent(pMsg);

        return STATE_NULL;
        break;
	// +TAR 449235 / 450969:  If the current PSX context is different from what the app
    // thinks the context is, then set the app to the PSX   context.
    //
    // The CM DLL sets the context for this screen.  If SCOTApp changes the 
    // context (SystemMessage, etc.), then the SCOTApp context will be out
    // of sync from the PSX context that is set by the CM DLL (from the 
    // m_cmMgr.EnterState() call above.
    case PS_SWITCHCONTEXT:
    case PS_SWITCHCONTEXTOTHER:
        trace(L6, _T("Context has changed.  Synchronize with current PSX context."));
        csContext = pMsg->psInfo.pszContext;
        
        ps.SetCMFrame(pMsg->psInfo.pszContext);
        
        if (csContext.Find(_T("XMCashierPasswordKeyboard")) != -1 ||
            csContext.Find(_T("XMCashierIDKeyboard")) != -1)
        {
            HandleCashierIDContext(csContext);
        }
        break;
	// +TAR 449235 / 450969
    default:
        break;
    }

    return STATE(CMOperatorPWState)::PSParse(pMsg);
}

void SMSmCashManagementExBase::HandleCashierIDContext(CString csContext)
{
    m_HKStatus = IPSProcedures::HK_NORMAL;
    m_bShiftOn = false;
    ps.PopulateHotKeys(csContext);
    ps.PopulateHKSensitiveControls(csContext, m_HKStatus, m_bShiftOn, false);
}

SMStateBase  *SMSmCashManagementExBase::PSHotKey(CString & csHotKey) 
{
  PSProceduresBase::HKStatus newStatus = PSProceduresBase::HK_NORMAL;
  if(csHotKey==_T("HKAcute"))
  {
    newStatus = PSProceduresBase::HK_ACUTEON;
  }
  else if(csHotKey==_T("HKGrave"))
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
  
  if(m_HKStatus==newStatus)
  {
    m_HKStatus = PSProceduresBase::HK_NORMAL;
  }
  else
  {
    m_HKStatus = newStatus;
  }


   PSXRC rc=PSX_SUCCESS;
   rc=m_pAppPSX->GetControlProperty( _T("HotKeys"), UI::PROPERTYBUTTONCOUNT, vIndex );
   if(rc==PSX_SUCCESS)
   {
      long nButtonCount = vIndex.lVal;
      if(nButtonCount)
      {
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), ps.GetCurrentContext(), UI::PROPERTYREDRAW, VARIANT_FALSE);
         if(m_HKStatus == PSProceduresBase::HK_NORMAL)
         {
            for( long i = 0L; i < nButtonCount; i++ )
            {
               COleVariant pvParams[2];
               pvParams[0]=i;
               pvParams[1]=(long) UI::Normal;
               m_pAppPSX->SendCommand( _T("HotKeys"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
            }
         }
         m_pAppPSX->SetConfigProperty(_T("HotKeys"), ps.GetCurrentContext(), UI::PROPERTYREDRAW, VARIANT_TRUE);
      }

      ps.PopulateHKSensitiveControls(ps.GetCurrentContext(), m_HKStatus, m_bShiftOn, false);
   }


   return STATE_NULL;//remain in same state

}

SMStateBase  *SMSmCashManagementExBase::PSShiftKey(void)
{
  
  m_bShiftOn =! m_bShiftOn;
  SetAlphaShiftState(ps.GetCurrentContext(), m_bShiftOn);

  ps.PopulateHKSensitiveControls(ps.GetCurrentContext(), m_HKStatus, m_bShiftOn, false);

  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    DMAcceptor                                        
// Description: resets the timeout
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementExBase::DMAcceptor()
{
    trace( L6, _T("SMSmCashManagementExBase::DMAcceptor()") );

    return m_pSMAccessor->DMAcceptor();

}

void SMSmCashManagementExBase::UnInitialize()
{
    trace(L6, _T("+Uninitialize"));

    csOperator = m_csOperatorLoggedIn;
    csPassword = m_csPasswordLoggedIn;

    m_cmMgr.ExitState();
    DMCoinAcceptorDisable();//JIRA-SSCOP-897

	dm.CashRecyclerPauseCounts(false);	// SSCOP-2197

    ps.SetInXM(false);	//SSCOADK-1021

    trace(L6, _T("-Uninitialize"));
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton8
////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCashManagementExBase::PSButton8() // Go Back button
{
    trace(L6,_T("+PSButton8"));

    dmcc.UpdateCashArrays(); //TAR 448434

	if(fPutSCOTinLC)
    {
       m_cmMgr.ExitToApp(); 
    }
    else
    {
   	   m_cmMgr.GoToBaseState();	// SSCOP-2212
    }

	// SR752+: If the TB supports this feature (TBIsAllowed) then we have to wait 
    // for the TB response before changing state.
    if(m_cmMgr.WaitForTBResponse()) 
    {
        trace(L6,_T("Waiting on a response from the TB before exiting."));
        return STATE_NULL;
    }
    // SR752-

    setAnchorState(bestSaved);
	// SSCOP-2212
    m_cmMgr.ExitCM();
    rp.SetCashManagementInProgress(false);  // TAR 450658

    ps.SetInXM(false);	//SSCOADK-1021

    trace(L6,_T("-PSButton8"));

    RETURNSTATE(SmMediaStatus);
}

/**
 * Pass device status events for the cash devices down to cash management.
 * Added to address TAR 448430.
 */
SMStateBase *SMSmCashManagementExBase::DMParse(const MessageElement* pMe)
{
    TRACE_METHOD(_T("SMSmCashManagementExBase::DMParse"));

	int nDMMsg = pMe->dmInfo.Msg;
    int nDMDevClass = pMe->dmInfo.Class;
	int nEvent     = pMe->dmInfo.lCodeOrLength;
    // If this is a DM_DATA event or if the event is not from one of the 
    // cash devices, then just return the base parse.
    if(nDMMsg == DM_DATA)
    {
        return STATE(CMOperatorPWState)::DMParse(pMe);
    }

    // This will be set to true by CM if it handled the event.
    bool bHandled = false;      

    switch(nDMDevClass)
    {
    case DMCLASS_COINACCEPTOR:
    case DMCLASS_CASHACCEPTOR:
    case DMCLASS_CASHCHANGER:
    case DMCLASS_CASHRECYCLER:  // Add Glory CM Support
        // Pass the event down to the CM module.  If CM handles the event then
        // bHandled will be set to true.
        m_cmMgr.DeviceEvent(const_cast<MessageElement &>(*pMe), bHandled);
        break;
    default:
        break;
    }

    if(bHandled)
    {
        trace(L6, _T("Event was handled by CM module."));
        return STATE_NULL;
    }

    // Show dispense jam error while in CM
    CString csWork;
    switch(nDMDevClass)
    {
        case DMCLASS_CASHCHANGER:
        {
            switch(nEvent)
            {
                case CHAN_STATUS_JAM:
                {
                    csWork = GetCashChangerError();
                    return  createLevel1Message(csWork,PS_NOTEXT, PS_NOTEXT,0, 
                        DMCLASS_CASHCHANGER,true,true);   
                }
                break;
            default:
                break;
            }
		    break;
		}
		// +SSCOP-2132
		case DMCLASS_CASHRECYCLER:
		{
            switch(nEvent)
			{
                case CHAN_STATUS_EMPTY: 
                case CHAN_STATUS_FULL:
                    trace(L6, _T("Ignoring event [%d] dev class [%d] while in CM."),
						nEvent, nDMDevClass);
                    return STATE_NULL;

                // Note:  Did not move OPOS_E_EXTENDED and OPOS_E_DISABLED here 
                //        because these errors were added for the SafePay
                //        (looked in VSS).
                case CHAN_STATUS_JAM:   //  31

                case CHAN_DIEVT_DEPOSITERROR:  //3
                case CHAN_DIEVT_DEPOSITCASSETTEFULL:   //5
                case CHAN_DIEVT_DEPOSITSETERROR:   //6
                    trace(L4, _T("Event [%d] received in CM.  Exiting CM."), nEvent);
                    PSButton8();
			}
            break;
		}
		// -SSCOP-2132
 
        default:
            break;
    }
	
    // If not handled, then pass it on up to the base class handler.
    return STATE(CMOperatorPWState)::DMParse(pMe);
}

//TAR 451616
//Override this function to prevent disable the CashAcceptor
///////////////////////////////////
long SMSmCashManagementExBase::DMCashAcceptorAccept(void)            
{
  long rc = 0;
  trace(L6,_T("+DMCashAcceptorAccept"));                    
    
  if (co.fTenderCashAllowed && dm.IsCashAcceptorAvailable())
  {
    if (nDMLastAcceptor)
    {
      nDMLastAcceptor = 0;
	  return rc;
    }
  }
    
  trace(L6,_T("-DMCashAcceptorAccept %d"),rc);              
  return rc;
}

//TAR 448426
SMStateBase * SMSmCashManagementExBase::PSEnterKey(void)
{
    TRACE_METHOD(_T("SMSmCashManagementExBase::PSEnterKey"));

    return STATE(CMOperatorPWState)::PSEnterKey();
}

//TAR 448426
SMStateBase *SMSmCashManagementExBase::HandleScannerData(const MessageElement* pMe)
{
    TRACE_METHOD(_T("SMSmCashManagementExBase::HandleScannerData"));

	DMScannerDisable();
    
    bool bIsValidOperator = isBarcodeValidOperatorPassword(cDMLastData);
    
    if (bIsValidOperator)      
    {
        bool bMakeXMLNow = false;
        if (!rp.IsTransactionInProgress() )
        {
            bMakeXMLNow = true;
            rp.TransactionStart(rp.m_csBusinessDate);
        }
        rp.SetOperator(csOperator);
        rp.OperatorLogin(rp.m_lOperatorLoginID, ATTIN, SCOTAPP);
        rp.m_lOperatorLoginID += 1;

        if (bMakeXMLNow)
        {
            rp.TransactionEnd();
        }
    }
    else
    {
        //SSCOADK-5801
        //csOperator = pMe->raInfo.operatorID;
        //csPassword = pMe->raInfo.password;
        return displayInvalidMessage();
    }

    CString csTempId = csOperator;
    CString csTempPw = csPassword;
    if(co.GetfOperationsEncodeOperatorIDPW())
    {
        CString csPrefix(_T("BASE64:"));
        CBase64EncodeDecode base64;
        csTempId = base64.GetBase64EncodeWithPrefix(csPrefix, csTempId);
        csTempPw = base64.GetBase64EncodeWithPrefix(csPrefix, csTempPw);
    }    
    //pass down credentials to CM
    m_cmMgr.SetCredentials(csTempId, csTempPw);

    return STATE_NULL;
}

// Disable copies. 
SMSmCashManagementExBase::SMSmCashManagementExBase(const SMSmCashManagementExBase &rhs)
{
    throw "ERROR";
}

SMStateBase * SMSmCashManagementExBase::createLevel1Message(LPCTSTR szMsg,
                                                          PSTEXT pstMsg,
                                                          PSTEXT prompt,
                                                          long   value,
                                                          long   device,
                                                          bool   checkHealth, 
                                                          bool displayHTML) //SR672
{
#ifndef _CPPUNIT
    return m_pSMAccessor->createLevel1Message(szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
#endif //CPPUNIT
}


///////////////////////////////////////////////////////////
SMStateBase * SMSmCashManagementExBase::createLevel2Message(LPCTSTR szMsg,
                                                          PSTEXT pstMsg,
                                                          PSTEXT prompt,
                                                          long   value,
                                                          long   device,
                                                          bool   checkHealth,
                                                          bool displayHTML) //SR672
{
#ifndef _CPPUNIT
    return m_pSMAccessor->createLevel2Message(szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
#endif //CPPUNIT
}


///////////////////////////////////////////////////////////
SMStateBase * SMSmCashManagementExBase::createLevel3Message(LPCTSTR szMsg,
                                                          PSTEXT pstMsg,
                                                          PSTEXT prompt,
                                                          long   value,
                                                          long   device,
                                                          bool   checkHealth,
                                                          bool displayHTML) //SR672
{
#ifndef _CPPUNIT
    return m_pSMAccessor->createLevel3Message(szMsg, pstMsg, prompt, value, device, checkHealth, displayHTML);
#endif //CPPUNIT
}