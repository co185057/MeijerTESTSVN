//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMUSSFManager.CPP 
//
// TITLE: Class implementation for this state
//
// AUTHOR:	Christopher Swift
//  
//            
//////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"                 // MS MFC always needed first
//                     // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION

#include "Common.h"                       // MGV common includes
#include "PSIntList.h"                    // MGV common includes
#include "SMUSSFManager.h"    // MGV this state
#include "SMCardMisRead.h"
#include "SMFinish.h"
#include "SMTakeCard.h"
#include "USSFCommon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif


#define COMP_ID ID_SM               // base state component
#define T_ID    _T("SMUSSFManager")

IMPLEMENT_DYNCREATE(SMUSSFManager, CObject)// MFC Runtime class/object information

DEFINE_TIMESTAMP

//////////////////////////////////////////
// CONTEXTS THAT USE THIS STATE:
//
//  SMUSSFManager...
//
//////////////////////////////////////////
SMUSSFManager::SMUSSFManager()
{
  IMPLEMENT_TIMESTAMP
}

/////////////////////////////////////////////////
SMStateBase *SMUSSFManager::Initialize(void)
{
  trace(L7, _T("SMUSSFManager::Initialize(void)"));
  CString tempstr = _T("");
  long rc = 0;

  nTimeOut = 30;
  
  SMStateBase *returnState = SMUSSFManagerBase::Initialize();
  if (returnState == STATE_NULL)
  {
    // Context Dependent Stuff

    CString currentContext = GetDisplayContext();
    returnState = ShowDetails(currentContext);
    if (returnState == STATE_NULL)
    {
      returnState = ShowContext();
    }
  }
  
  return returnState;
}

void SMUSSFManager::UnInitialize(void)
{
    SMUSSFManagerBase::UnInitialize();
}

long SMUSSFManager::isHandled(CString currentEvent, CString altContext, bool bGAMode)
{
    return SMUSSFManagerBase::isHandled(currentEvent, altContext, bGAMode);
}

SMStateBase *SMUSSFManager::MakeNextMove(CString altContext)
{
  trace(L7,_T("SMUSSFManager::MakeNextMove(CString altContext)"));

  //bool rc = TBStateIsReadyToSell();
  //if (rc == true)
  //{
    //  SetTransactionVariable(_T("GA_ITEM"), _T("TRUE"));
  //}

  return SMUSSFManagerBase::MakeNextMove(altContext);
}

SMStateBase* SMUSSFManager::ProcessEventPartial(CString thisEvent, CString thisContext )
{
    return SMUSSFManagerBase::ProcessEventPartial(thisEvent , thisContext );
}

SMStateBase* SMUSSFManager::ProcessEventComplete(CString thisEvent, CString thisContext )
{
    return SMUSSFManagerBase::ProcessEventComplete(thisEvent , thisContext );
}

SMStateBase* SMUSSFManager::ProcessEvent(CString thisEvent, CString thisContext )
{
    return SMUSSFManagerBase::ProcessEvent(thisEvent , thisContext );
}

bool SMUSSFManager::PSRemoteLegalForState()
{
    return SMUSSFManagerBase::PSRemoteLegalForState();
}

SMStateBase* SMUSSFManager::USSF_Show2x20_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Show2x20_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SendScanData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SendScanData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SellDepartmentItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SellDepartmentItem_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SellScannedItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SellScannedItem_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SellKeyedItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SellKeyedItem_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IgnoreUnknownAction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IgnoreUnknownAction_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_LoadDefinition_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_LoadDefinition_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearDefinition_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearDefinition_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_StoreSymbols_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_StoreSymbols_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Resume_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Resume_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Mark_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Mark_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Return_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Return_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Exit_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Exit_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_AmtDueAmtPaid_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_AmtDueAmtPaid_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EnableMSR_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EnableMSR_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisableMSR_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisableMSR_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CancelItem_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CancelItem_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_FormatData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_FormatData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EnableScanner_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EnableScanner_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisableScanner_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisableScanner_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IfTxStart_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IfTxStart_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SayAmount_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SayAmount_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SayPhrase_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SayPhrase_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SayWave_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SayWave_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SetDeclineMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SetDeclineMessage_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Intervention_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Intervention_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_FireTBDisplay_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_FireTBDisplay_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Alert_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Alert_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_AssistAlert_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_AssistAlert_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SetTimeOut_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SetTimeOut_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Trace_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Trace_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IdentifyApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IdentifyApproval_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Sleep_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Sleep_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearUSSFApproval_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_TBFinish_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_TBFinish_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ResetTenderButtons_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ResetTenderButtons_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SetAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SetAnchorState_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GoAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GoAnchorState_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IfApprovalPending_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IfApprovalPending_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_FullReset_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_FullReset_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_PartialReset_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_PartialReset_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearTransactionData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearTransactionData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearItemData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearItemData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearMetaData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearMetaData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Clear_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Clear_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Append_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Append_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Store_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Store_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Parse_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Parse_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SendPOSMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SendPOSMessage_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisplayKeyedElement_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisplayKeyedElement_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisplayKeyedBuffer_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisplayKeyedBuffer_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisplayKeyedData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisplayKeyedData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Display_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Display_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearDisplay_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearDisplay_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_InitAssist_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_InitAssist_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ExitAssist_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ExitAssist_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_NOP_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_NOP_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Set_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Set_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_PressButtonX_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_PressButtonX_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_PressGoBack_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_PressGoBack_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GetDiagFiles_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GetDiagFiles_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Shutdown_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Shutdown_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisplayRAP_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisplayRAP_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisplayReceipt_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisplayReceipt_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GetText_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GetText_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EchoInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EchoInput_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EchoData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EchoData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ShowLeadthru_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ShowLeadthru_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EchoAmount_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EchoAmount_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearInput_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EnableButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EnableButton_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DisableButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DisableButton_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SetButton_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SetButton_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ClearControlButtons_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ClearControlButtons_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_AllowInput_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_AllowInput_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SendRAPMessage_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SendRAPMessage_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GoUSSF_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GoUSSF_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IF_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IF_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GoFLState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GoFLState_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_WaitForApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_WaitForApproval_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IfTxBalance_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IfTxBalance_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GetPSXVariable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SetPSXVariable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_FreezeState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_FreezeState_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_UnFreezeState_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_UnFreezeState_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_IfFrankingPending_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_IfFrankingPending_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SuspendTransaction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SuspendTransaction_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_HandleUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_HandleUSSFApproval_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GetTotalDetails_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GetTotalDetails_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_GoTo_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_GoTo_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_EndScript_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_EndScript_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CancelTransaction_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CancelTransaction_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SAWLDBScannerDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SAWLDBScannerDisable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SAWLDBScannerEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SAWLDBScannerEnable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CashAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CashAcceptorEnable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CashAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CashAcceptorDisable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CoinAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CoinAcceptorEnable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_CoinAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_CoinAcceptorDisable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ScaleDisable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ScaleDisable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ScaleEnable_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ScaleEnable_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DumpUSSFSymbols_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DumpUSSFSymbols_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DumpUSSFSymbolsBySerial_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DumpUSSFSymbolsBySerial_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_DumpUSSFSymbolsByElementID_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_DumpUSSFSymbolsByElementID_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_SaveActivationData_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_SaveActivationData_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ScrollUp_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ScrollUp_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ScrollDown_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ScrollDown_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Scroll_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Scroll_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_ScrollList_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_ScrollList_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::USSF_Initialize_Action(SMStateBase *pOpState, vector<CString> vParms)
{
    return SMUSSFManagerBase::USSF_Initialize_Action(pOpState, vParms);
}

SMStateBase* SMUSSFManager::TBParse(MessageElement* me)
{
    return SMUSSFManagerBase::TBParse(me);
}

TBSTATE SMUSSFManager::TBProcessMessage(MessageElement* me)
{
    return SMUSSFManagerBase::TBProcessMessage(me);
}

SMStateBase* SMUSSFManager::PSButtonHelp(void)
{
    return SMUSSFManagerBase::PSButtonHelp();
}

SMStateBase* SMUSSFManager::PSButtonGoBack(void)
{
    return SMUSSFManagerBase::PSButtonGoBack();
}

SMStateBase* SMUSSFManager::PSButton1(void)
{
    return SMUSSFManagerBase::PSButton1();
}

SMStateBase* SMUSSFManager::PSButton2(void)
{
    return SMUSSFManagerBase::PSButton2();
}

SMStateBase* SMUSSFManager::PSButton3(void)
{
    return SMUSSFManagerBase::PSButton3();
}

SMStateBase* SMUSSFManager::PSButton4(void)
{
    return SMUSSFManagerBase::PSButton4();
}

SMStateBase* SMUSSFManager::PSButton5(void)
{
    return SMUSSFManagerBase::PSButton5();
}

SMStateBase* SMUSSFManager::PSButton6(void)
{
    return SMUSSFManagerBase::PSButton6();
}

SMStateBase* SMUSSFManager::PSButton7(void)
{
    return SMUSSFManagerBase::PSButton7();
}

SMStateBase* SMUSSFManager::PSButton8(void)
{
    return SMUSSFManagerBase::PSButton8();
}

SMStateBase* SMUSSFManager::PSReceiptUp(void)
{
    return SMUSSFManagerBase::PSReceiptUp();
}

SMStateBase* SMUSSFManager::PSReceiptDown(void)
{
    return SMUSSFManagerBase::PSReceiptDown();
}

SMStateBase* SMUSSFManager::PSKeystroke(const long wParam, const long lParam)
{
    return SMUSSFManagerBase::PSKeystroke(wParam, lParam);;
}

SMStateBase* SMUSSFManager::PSParse(const MessageElement *me)
{
    return SMUSSFManagerBase::PSParse(me);
}

SMStateBase* SMUSSFManager::PSListLookup(CString csItemInfo)
{
    return SMUSSFManagerBase::PSListLookup(csItemInfo);
}

SMStateBase* SMUSSFManager::NumericInput(long value)
{
    return SMUSSFManagerBase::NumericInput(value);
}

SMStateBase* SMUSSFManager::EnterInput(void)
{
    return SMUSSFManagerBase::EnterInput();
}

SMStateBase* SMUSSFManager::ClearInput(void)
{
    return SMUSSFManagerBase::ClearInput();
}

SMStateBase* SMUSSFManager::TimedOut(void)
{
    return SMUSSFManagerBase::TimedOut();
}

SMStateBase *SMUSSFManager::TBIdleTimer(void)
{
    return SMUSSFManagerBase::TBIdleTimer();
}

SMStateBase *SMUSSFManager::DMScanner()
{
    return SMUSSFManagerBase::DMScanner();
}

SMStateBase *SMUSSFManager::DMScale()
{
    return SMUSSFManagerBase::DMScale();
}

SMStateBase *SMUSSFManager::PureMethod(void)
{
    return SMUSSFManagerBase::PureMethod();
}

void SMUSSFManager::SetActiveState(CString thisState)
{
    SMUSSFManagerBase::SetActiveState(thisState);
}

void SMUSSFManager::SetActiveContext(CString thisState)
{
    SMUSSFManagerBase::SetActiveContext(thisState);
}

void SMUSSFManager::SetPendingPreAction(CString action)
{
    SMUSSFManagerBase::SetPendingPreAction(action);
}

CString SMUSSFManager::GetPendingPreAction(void)
{
    return SMUSSFManagerBase::GetPendingPreAction();
}

void SMUSSFManager::SetPendingPostAction(CString action)
{
    SMUSSFManagerBase::SetPendingPostAction(action);
}

CString SMUSSFManager::GetPendingPostAction(void)
{
    return SMUSSFManagerBase::GetPendingPostAction();
}

CString SMUSSFManager::GetPendingEvent(void)
{
    return SMUSSFManagerBase::GetPendingEvent();
}

SMStateBase *SMUSSFManager::MakeAnchorState(CString nextState)
{
    return SMUSSFManagerBase::MakeAnchorState(nextState);
}

SMStateBase *SMUSSFManager::MakeNewState(CString nextState)
{
    return SMUSSFManagerBase::MakeNewState(nextState);
}

SMStateBase *SMUSSFManager::PrepareContext(CString &currentContext)
{
    return SMUSSFManagerBase::PrepareContext(currentContext);
}

SMStateBase *SMUSSFManager::ShowContext(void)
{
    return SMUSSFManagerBase::ShowContext();
}

SMStateBase *SMUSSFManager::RunScript(CString thisScript)
{
    return SMUSSFManagerBase::RunScript(thisScript);
}

SMStateBase *SMUSSFManager::ManageAction(CString thisAction, bool &bActionTaken)
{
    return SMUSSFManagerBase::ManageAction(thisAction, bActionTaken);
}

long SMUSSFManager::ParseAction(CString thisAction)
{
    return SMUSSFManagerBase::ParseAction(thisAction);
}

void SMUSSFManager::ClearParameters(void)
{
    SMUSSFManagerBase::ClearParameters();
}

SMStateBase *SMUSSFManager::PerformAction(CString thisAction, bool &bActionTaken)
{
    trace(L7,_T("SMUSSFManager::PerformAction(CString thisAction)"));

    trace(L6, _T("*** PERFORMING ACTION=<%s>."), thisAction);
    bActionTaken = false;

    CString csTraceStr;
    SMStateBase *returnState = STATE_NULL;

    try
    {
        USSF_LOCAL_ACTION(_T("SAMPLE"), FullReset);
    }
    catch(...)
    {
        trace(L7, _T("Exception Within Customer Action(%s)."), thisAction);
    }

    if (bActionTaken == false)
    {
        return SMUSSFManagerBase::PerformAction(thisAction, bActionTaken);
    }

    return returnState;
}

CString SMUSSFManager::GetHostState(CString nextContext)
{
    return SMUSSFManagerBase::GetHostState(nextContext);
}

CString SMUSSFManager::SetDisplayContext(CString nextContext)
{
    return SMUSSFManagerBase::SetDisplayContext(nextContext);
}

CString SMUSSFManager::GetDisplayContext(void)
{
    return SMUSSFManagerBase::GetDisplayContext();
}

SMStateBase *SMUSSFManager::MakePreviousMove(void)
{
    return SMUSSFManagerBase::MakePreviousMove();
}

SMStateBase *SMUSSFManager::MakeReferenceMove(CString reference)
{
    return SMUSSFManagerBase::MakeReferenceMove(reference);
}

SMStateBase *SMUSSFManager::DoTransition(CString nextState, CString nextContext)
{
    return SMUSSFManagerBase::DoTransition(nextState, nextContext);
}

SMStateBase *SMUSSFManager::Resume(void)
{
    return SMUSSFManagerBase::Resume();
}

bool SMUSSFManager::isUSSFMode(void)
{
    return SMUSSFManagerBase::isUSSFMode();
}

SMStateBase *SMUSSFManager::ShowDetails(CString currentContext)
{
    return SMUSSFManagerBase::ShowDetails(currentContext);
}

void SMUSSFManager::DoContextConfig(CString currentContext, CString defaultContext )
{
    SMUSSFManagerBase::DoContextConfig(currentContext, defaultContext );
}

SMStateBase *SMUSSFManager::ConfigurationCheck(long lProgress)
{
    return SMUSSFManagerBase::ConfigurationCheck(lProgress );
}
