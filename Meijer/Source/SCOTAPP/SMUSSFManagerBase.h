//////////////////////////////////////////////////////////////////////////////////////////////////
//  This module was added for TTW620 to handle Generic Restrictions
// 
#ifndef _SMUSSFMANAGERBASE
#define _SMUSSFMANAGERBASE

#include "USSFProcedures.h"
#include "SMState.h"
//#include <vector>
//using namespace std;

#ifndef STATE_USSF_FALSE
#define STATE_USSF_FALSE	((SMStateBase *)(999))
#endif

extern const CString csUSSFDefaultContext;
extern const CString csUSSFManagerContext;

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif
 SMUSSFManagerBase : public STATE(State)
{
public:
	SMUSSFManagerBase();
	virtual void UnInitialize(void);
	
  DECLARE_DYNCREATE(SMUSSFManagerBase)  // MFC Runtime class/object information

public:
  virtual SMStateBase *Initialize(void);
  static long GetTimeOut(CString context);
 
  void SetChoiceID(CString);
  static void GetActivationStatus(CString &state, CString &context);
  virtual long isHandled(CString, CString altContext = _T(""), bool bUSSFMode = false);
  virtual SMStateBase *MakeNextMove(CString thisContext = _T(""));
  virtual SMStateBase *ProcessEventPartial(CString thisEvent = _T(""), CString thisContext = _T(""));
  virtual SMStateBase *ProcessEventComplete(CString thisEvent = _T(""), CString thisContext = _T(""));
  virtual SMStateBase *ProcessEvent(CString thisEvent = _T(""), CString thisContext = _T(""));

  // Utilities

  virtual bool PSRemoteLegalForState();

  // Local Actions
  virtual SMStateBase *USSF_Show2x20_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SendScanData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SellDepartmentItem_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SellScannedItem_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SellKeyedItem_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IgnoreUnknownAction_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_LoadDefinition_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearDefinition_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_StoreSymbols_Action(SMStateBase *pOpState, vector<CString> vParms);

  // System Actions (local)
  virtual SMStateBase *USSF_Resume_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Mark_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Return_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Exit_Action(SMStateBase *pOpState, vector<CString> vParms);

  // Actions Pushed Down From Boots Development.
  virtual SMStateBase *USSF_AmtDueAmtPaid_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EnableMSR_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisableMSR_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CancelItem_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_FormatData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EnableScanner_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisableScanner_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IfTxStart_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SayAmount_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SayPhrase_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SayWave_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SetDeclineMessage_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Intervention_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_FireTBDisplay_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Alert_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_AssistAlert_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SetTimeOut_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Trace_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IdentifyApproval_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Sleep_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_TBFinish_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ResetTenderButtons_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SetAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GoAnchorState_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IfApprovalPending_Action(SMStateBase *pOpState, vector<CString> vParms);

  // Actions Previously in USSFUtility.
  virtual SMStateBase *USSF_FullReset_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_PartialReset_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearTransactionData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearItemData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearMetaData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Clear_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Append_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Store_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Parse_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SendPOSMessage_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisplayKeyedElement_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisplayKeyedBuffer_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisplayKeyedData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Display_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearDisplay_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_InitAssist_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ExitAssist_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_NOP_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Set_Action(SMStateBase *pOpState, vector<CString> vParms);
  //virtual SMStateBase *USSF_EnableButton_Action(SMStateBase *pOpState, vector<LPCTSTR> vParms);
  //virtual SMStateBase *USSF_DisableButton_Action(SMStateBase *pOpState, vector<LPCTSTR> vParms);
  virtual SMStateBase *USSF_PressButtonX_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_PressGoBack_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GetDiagFiles_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Shutdown_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisplayRAP_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisplayReceipt_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GetText_Action(SMStateBase *pOpState, vector<CString> vParms);

  // Pushed Down From Boots Development.
  virtual SMStateBase *USSF_EchoInput_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EchoData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ShowLeadthru_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EchoAmount_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearInput_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EnableButton_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DisableButton_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SetButton_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ClearControlButtons_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_AllowInput_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SendRAPMessage_Action(SMStateBase *pOpState, vector<CString> vParms);

  virtual SMStateBase *USSF_GoUSSF_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IF_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GoFLState_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_WaitForApproval_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IfTxBalance_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SetPSXVariable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_FreezeState_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_UnFreezeState_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_IfFrankingPending_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SuspendTransaction_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_HandleUSSFApproval_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GetTotalDetails_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_GoTo_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_EndScript_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CancelTransaction_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SAWLDBScannerDisable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_SAWLDBScannerEnable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CashAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CashAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CoinAcceptorEnable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_CoinAcceptorDisable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ScaleDisable_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ScaleEnable_Action(SMStateBase *pOpState, vector<CString> vParms);

  //Begin SWR740
  virtual SMStateBase *USSF_DumpUSSFSymbols_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DumpUSSFSymbolsBySerial_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_DumpUSSFSymbolsByElementID_Action(SMStateBase *pOpState, vector<CString> vParms);
  //End SWR740

  // TAR #429050 START
  virtual SMStateBase *USSF_SaveActivationData_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ScrollUp_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ScrollDown_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Scroll_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_ScrollList_Action(SMStateBase *pOpState, vector<CString> vParms);
  virtual SMStateBase *USSF_Initialize_Action(SMStateBase *pOpState, vector<CString> vParms);
  // TAR #429050 STOP
protected:
  virtual SMStateBase *TBParse(MessageElement*);
  virtual TBSTATE TBProcessMessage(MessageElement* me);
  // Event Handlers
  virtual SMStateBase *PSButtonHelp(void);
  virtual SMStateBase *PSButtonGoBack(void);
  virtual SMStateBase *PSButton1(void);
  virtual SMStateBase *PSButton2(void);
  virtual SMStateBase *PSButton3(void);
  virtual SMStateBase *PSButton4(void);
  virtual SMStateBase *PSButton5(void);
  virtual SMStateBase *PSButton6(void);
  virtual SMStateBase *PSButton7(void);
  virtual SMStateBase *PSButton8(void);
  virtual SMStateBase *PSReceiptUp(void);
  virtual SMStateBase *PSReceiptDown(void);
  virtual SMStateBase *PSKeystroke(const long wParam, const long lParam);
  virtual SMStateBase *PSParse(const MessageElement *me);
  virtual SMStateBase *PSListLookup(CString); // Ingredients

  virtual SMStateBase *NumericInput(long value) { return STATE_NULL; }
  virtual SMStateBase *EnterInput(void) { return STATE_NULL; }
  virtual SMStateBase *ClearInput(void) { return STATE_NULL; }
  virtual SMStateBase *TimedOut(void);
  virtual SMStateBase *TBIdleTimer(void);
  virtual SMStateBase *DMScanner();	// scanned item  
  virtual SMStateBase *DMScale();	// scale weight 

  virtual SMStateBase *PureMethod(void){return STATE_NULL;}

  // State Managers
  virtual void SetActiveState(CString);
  virtual void SetActiveContext(CString);
  virtual void SetPendingPreAction(CString);
  //virtual void SetPendingNextStep(CString);
  virtual CString GetPendingPreAction(void);
  //virtual CString GetPendingNextStep(void);
  virtual void SetPendingPostAction(CString);
  virtual CString GetPendingPostAction(void);
  virtual CString GetPendingEvent(void);

  virtual SMStateBase *MakeAnchorState(CString nextState);
  virtual SMStateBase *MakeNewState(CString nextState);
  //virtual SMStateBase *MakeNextMove(CString thisContext = _T(""));
  virtual SMStateBase *PrepareContext(CString &currentContext);
  virtual SMStateBase *ShowContext(void);
  virtual SMStateBase *RunScript(CString thisScript);
  virtual SMStateBase *ManageAction(CString thisAction, bool &bActionTaken);
  virtual long ParseAction(CString thisAction);
  virtual void ClearParameters(void);
  virtual SMStateBase *PerformAction(CString thisAction, bool &bActionTaken);
  virtual CString GetHostState(CString nextContext);
  virtual CString SetDisplayContext(CString nextContext);
  virtual CString GetDisplayContext(void);
  virtual SMStateBase *MakePreviousMove(void);
  virtual SMStateBase *MakeReferenceMove(CString reference = _T(""));
  virtual SMStateBase *DoTransition(CString nextState, CString nextContext);
  virtual SMStateBase *Resume(void);

  // Utilities
  virtual bool isUSSFMode(void){return true;}
  virtual SMStateBase *ShowDetails(CString);
  virtual void DoContextConfig(CString currentContext, CString defaultContext = _T(""));
  virtual SMStateBase *ConfigurationCheck(long lProgress = -1);
  static long lConfigurationCheckProgress;
  static long lConfigurationCheckHWM;

  // where to proceed from here

  bool	bGoingToInProgress;   // going to this state next?
  bool  m_bLangChange;         //Last language was not the system default - TAR 262131

  // Used by Derived States.
  // TAR #429050 START
  CString csDisplayGroup;
  // TAR #429050 STOP
  CString csDisplayType;
  CString csHelpAVI;
  CString csHelpImage;
  CString csDataEntryGroup;
  CString csDataEntryBuffer;
  CString csScanner;
  CString csScale;
  CString csTitle;
  CString csTitle2;
  CString csLeadthru;
  CString csHelpText;
  CString csInstructionText;
  CString csPOSDisplay;

  CString csPOSData;
  CString csMetaData;

  //static CString csQuantity;
  //static CString csPrice;

  long lCurrentWeight;

  bool bVoid;
  CString lastItemSold;
  bool bSecurityWaitingForItemSold;

  // TAR #429050 START
  vector<CString> myArgs;
  // TAR #429050 STOP
  CString myCommand;
  CString myCommandUpper;
  CString myCommandLower;
  CStringArray myParms;
  CStringArray myParmsUpper;
  CStringArray myParmsLower;

  //static CCriticalSection USSFEventCriticalSection;

private:

  static LPTSTR SwipeData;
  static int SwipeDataLen;
};

#endif