//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMSmPrintReceiptBase.CPP
//
// TITLE:   Class implementation for Print Receipt state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes
#include "SMSmPrintReceipt.h"
#include "SMSmReportsMenu.h"
#include "SMInProgress.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SMSmPrintReceiptBase")

static BEST nPrevState;

IMPLEMENT_DYNCREATE(SMSmPrintReceiptBase, CObject)// MFC Runtime class/object information
DEFINE_TIMESTAMP
///////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////
SMSmPrintReceiptBase::SMSmPrintReceiptBase()
{
	IMPLEMENT_TIMESTAMP
}
///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////
SMSmPrintReceiptBase::~SMSmPrintReceiptBase()
{
}            
///////////////////////////////////////////////////////////////////////////////
// Initialize                                                                           
///////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmPrintReceiptBase::Initialize(void)
{ 
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmPrintReceipt") );	// RFC 330050
  if (getAnchorState() != BES_SMPRINTRECEIPT)
  {
	  nPrevState = getAnchorState();
  }
  trace(L6, _T("+SMSmPrintReceiptBase::Initialize(), Previous Anchor State: %d"), nPrevState);
  setAnchorState(BES_SMPRINTRECEIPT);
  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  ps.SetFrame(_T("SmPrintReceipt"));
  ps.SetTitle(BTT_PRINT_RECEIPT, PS_BLANK);

  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  
  // TAR 236945 - Needed for Remote, this api will check option whether to display or not.
  ps.Weight(lDMScaleWeight);

  //ps.SetLeadthruText(MSG_SELECT_PRINT_FUNCTION, PS_BLANK);
  if (bReprintReceiptFailed)
  {
	  ps.SetLeadthruText(MSG_UNABLE_PRINT_RECEIPT, PS_BLANK);
	  bReprintReceiptFailed = false;
  }
  else
  {
	  ps.SetLeadthruText(MSG_SELECT_PRINT_FUNCTION, PS_BLANK);
  }

  ps.Button(_T("SMButton1"), BTT_PRINTLAST, true);
  ps.Button(_T("SMButton2"), BTT_PRINT_OTHERRECEIPT, true);

  ps.ShowFrame();
  if (dm.GetPrinterPaperLow())
  {
	// (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);  
  }
  ps.Echo(PS_BLANK);
  ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());  //Tar 228689, While lane in training, RAP store mode did not show in training

  ps.SMButton(_T("SMButton8"), BTT_GOBACK, true); //enable go back button always

  trace(L6, _T("-SMSmPrintReceiptBase::Initialize()") );   

  return STATE_NULL;  // stay in SMSmPrintReceiptBase state
}

//////////////////////////////////////////////////////
// Print last receipt
//////////////////////////////////////////////////////
SMStateBase  *SMSmPrintReceiptBase::PSButton1(void) 
{
  trace(L6, _T("SMSmPrintReceiptBase::PSButton1() - Print Last Receipt") ) ;

  // I know -- these should be member variables.
  CString csSlipFile = SLIP_FILE;
  CString csLReceiptFile = LRECEIPT_FILE;
  CString csLReceiptTmpFile = LRECEIPT_TMP_FILE;

  GET_PARTITION_PATH(SLIP_FILE, csSlipFile.GetBuffer(_MAX_PATH));
  GET_PARTITION_PATH(LRECEIPT_FILE, csLReceiptFile.GetBuffer(_MAX_PATH));
  GET_PARTITION_PATH(LRECEIPT_TMP_FILE, csLReceiptTmpFile.GetBuffer(_MAX_PATH));

  csSlipFile.ReleaseBuffer();
  csLReceiptFile.ReleaseBuffer();
  csLReceiptTmpFile.ReleaseBuffer();
  
  PrintLastReceipt(csSlipFile);      //print slip receipt
  if (FileExist(csLReceiptTmpFile))
  {
	  PrintLastReceipt(csLReceiptTmpFile);  //print receipt
  }
  else
  {
	  PrintLastReceipt(csLReceiptFile);  //print receipt
  }
 
  return STATE_NULL;
}

//////////////////////////////////////////////////////
// Print other receipt
//////////////////////////////////////////////////////
SMStateBase  *SMSmPrintReceiptBase::PSButton2(void) 
{
  trace(L6, _T("SMSmPrintReceiptBase::PSButton2() - Print Other Receipt") ) ;

  SendTBCommand(TBCMD_REPRINTRECEIPT, true);
  
  CREATE_AND_DISPENSE(InProgress)(storeMode(), helpMode(), 0);
}

//////////////////////////////////////////////////////
// Go back to reports menu
//////////////////////////////////////////////////////
SMStateBase  *SMSmPrintReceiptBase::PSButton8(void)
{
   setAnchorState(nPrevState);    
   RETURNSTATE(SmReportsMenu);
}

///////////////////////////////////////////////////////////////////////////////
// UnInitialize                                                                           
///////////////////////////////////////////////////////////////////////////////
void SMSmPrintReceiptBase::UnInitialize()
{
   TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmPrintReceipt") );	// RFC 330050
   trace(L6, _T("SMSmPrintReceiptBase::UnInitialize()") );
}
