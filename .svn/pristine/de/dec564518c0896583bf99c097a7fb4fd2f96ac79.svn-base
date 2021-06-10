//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCardManagementBase.CPP
//
// TITLE: Class implementation for store mode CardManagement state
//
////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"                 // Common includes

#include "SMState.h"                // Base state
#include "SMSmCardManagement.h"      
#include "SmSmCashierPassword.h"
#include "SMSmMediaStatus.h"



#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmCardManagement")

CString  SMSmCardManagementBase::m_csRectCylinder8;
CString  SMSmCardManagementBase::m_csRectCylinder9;

IMPLEMENT_DYNCREATE(SMSmCardManagementBase, CObject)// MFC Runtime class/object information


DEFINE_TIMESTAMP

//////////////////////////////////
SMSmCardManagementBase::SMSmCardManagementBase():
	m_lRejectBinCount(0),
	m_lDispenserBinCount(0),
	m_lRejectBinChangeCount(0),
	m_lDispenserBinChangeCount(0)
{
  IMPLEMENT_TIMESTAMP

}

SMStateBase  *SMSmCardManagementBase::Initialize(void)
{
  trace(L6,_T("+Initialize"));
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("SmCardManagement") );	// RFC 330050
  dm.GetAdjustCardDispenserBinCounts(m_lDispenserBinCount, m_lRejectBinCount, 0);
  //save the position of SmCashManagementGenericCylinderArea8 and SmCashManagementGenericCylinderArea9
  if ( 0 == m_csRectCylinder8.GetLength() || 0 == m_csRectCylinder9.GetLength() )
  {
	COleVariant vReturn;
	m_pAppPSX->GetConfigProperty( _T("SmCashManagementGenericCylinderArea8"), _T("SmCardManagement"), UI::PROPERTYPOSITION, vReturn);
	m_csRectCylinder8 = vReturn.bstrVal;
	m_pAppPSX->GetConfigProperty( _T("SmCashManagementGenericCylinderArea9"), _T("SmCardManagement"), UI::PROPERTYPOSITION, vReturn);
	m_csRectCylinder9 = vReturn.bstrVal;
  }
  showScreen();
  trace(L6,_T("-Initialize"));
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    showScreen()
// Description: This is the main function that sets the UI layout. The context is SmCardManagement
///////////////////////////////////////////////////////////////////////////////////
void SMSmCardManagementBase::showScreen()
{
  trace(L6,_T("+showScreen "));
  ps.SetCMFrame(_T("SmCardManagement"));
  ps.SetTitle(ps.GetPSText(MSG_CARDMANAGEMENT));
  ps.SetLeadthruText(ps.GetPSText(MSG_CARDMANAGEMENT_LEADTHRUTEXT));
  ps.SetButtonText(_T("SMButton1"),MSG_DISPENSER_PICKUP);
  ShowTextData();
  ShowImgData();
  ps.ShowFrame();
  trace(L6,_T("-showScreen"));
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton1
// Description: Dispenser PickUp button
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCardManagementBase::PSButton1(void)
{
  trace(L6,_T("+PSButton1"));
  LoadAmt( m_lDispenserBinCount * -1 );
  showScreen();
  trace(L6,_T("-PSButton1"));
  return STATE_NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton8
// Description: Go Back button
///////////////////////////////////////////////////////////////////////////////////
SMStateBase *SMSmCardManagementBase::PSButton8() 
{
  trace(L6,_T("+PSButton8"));
  //if the current context is SmLoadLift then go back to SMCardManagement else to SMMediaStatus
  if( ps.GetCurrentContext() == _T("SmLoadLift") )  
  {
	  showScreen();
	  return STATE_NULL;
  }
  trace(L6,_T("-PSButton8"));
  RETURNSTATE(SmMediaStatus);
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowTextData
// Description: Display the value of "Count" and "Change in Count" of Reject and Dispenser Bin
///////////////////////////////////////////////////////////////////////////////////
void SMSmCardManagementBase::ShowTextData(void)
{
	COleVariant vValue;

	vValue = (long)m_lRejectBinCount;
	m_pAppPSX->SetTransactionVariable(_T("SmCashManagementAcceptorCountOrValue9"), vValue );
	vValue = (long)m_lRejectBinChangeCount;
	m_pAppPSX->SetTransactionVariable(_T("SmCashManagementAcceptorChangeInCount"), vValue );
	vValue = (long)m_lDispenserBinCount;
	m_pAppPSX->SetTransactionVariable(_T("SmCashManagementDispenserCountOrValue8"), vValue );
	vValue = (long)m_lDispenserBinChangeCount;
	m_pAppPSX->SetTransactionVariable(_T("SmCashManagementChangeInCount8"), vValue );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowImgData
// Description: Display the cylinder graph
///////////////////////////////////////////////////////////////////////////////////
void SMSmCardManagementBase::ShowImgData(void)
{
	CString csNewPosition = GetNewCylinderPosition(m_csRectCylinder8, m_lDispenserBinCount, co.getCardDispenserBinCapacity());
	m_pAppPSX->SetConfigProperty( _T("SmCashManagementGenericCylinderArea8"), _T("SmCardManagement"), UI::PROPERTYPOSITION, csNewPosition );

	csNewPosition = GetNewCylinderPosition(m_csRectCylinder9, m_lRejectBinCount, co.getDispenserRejectBinFull());
	m_pAppPSX->SetConfigProperty( _T("SmCashManagementGenericCylinderArea9"), _T("SmCardManagement"), UI::PROPERTYPOSITION, csNewPosition );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    GetNewCylinderPosition
// Description: Display the cylinder graph
// Parameters : [in] CString csDefRect - property position
//				[in] long lCount - number of cards in the dispenser
//				[in] long lCapacity - capacity of dispenser
// Return Values : CString - the new position
///////////////////////////////////////////////////////////////////////////////
CString SMSmCardManagementBase::GetNewCylinderPosition(const CString csDefRect , long lCount, long lCapacity)
{

    CString csRet;
    int nX[2] = {0}, nY[2] = {0};

#ifdef _UNICODE
    swscanf(	csDefRect , _T("%d,%d,%d,%d"), &nX[0], &nY[0], &nX[1], &nY[1] );
#else
    _stscanf(	csDefRect , _T("%d,%d,%d,%d"), &nX[0], &nY[0], &nX[1], &nY[1] );

#endif

	const int HEIGHTOFBOTTOM = 17; //bottom height of the cylinder
    int nBottomPosition = nY[0]+nY[1] - HEIGHTOFBOTTOM; //compute our limit
    int nLengthOfCylinder = nY[1];

    //+assign new data to cylinder
    CString csNewCylinderPosition = _T("%d,%d,%d,%d");
          double dPercentage = ((double)lCount / lCapacity); //percentage compute, count / capacity
		  if ( dPercentage > 1) 
		  {
			dPercentage = 1;	
		  }
		
        trace(L9,_T("Count Percentage %f"), dPercentage);

        int nSupposedLengthOfCylinder = ( nLengthOfCylinder - HEIGHTOFBOTTOM) * dPercentage; // ok to disregard to decimals. 

        int nBottomLimit = nBottomPosition ;

        int nNewYStartingPosition = nBottomPosition - nSupposedLengthOfCylinder; // the new vertical position starting point.

        trace(L9,_T("Bottom Limit : %d, new starting point : %d"), nBottomLimit, nNewYStartingPosition);

        if( nNewYStartingPosition > nBottomLimit )
        { //if the new starting position is greater than the limit, reset the new position to the limit
          nNewYStartingPosition = nBottomLimit;
        }
        trace(L9,_T("length of cylinder : %d, new starting point : %d"), nLengthOfCylinder, nNewYStartingPosition);


            csNewCylinderPosition.Format( (CString) csNewCylinderPosition, nX[0], nNewYStartingPosition, nX[1], nLengthOfCylinder );
//  trace(L6,_T("-GetNewCylinderPosition() %s, %s"), csNewCylinderPosition, csImage);

  return csNewCylinderPosition;
}
	
/////////////////////////////////////////////
// PSClearKey - Clear all input (on the pinpad)
//////////////////////////////////////////////
SMStateBase * SMSmCardManagementBase::PSClearKey()
{
  trace( L6, _T("+PSClearKey"));
  PSClearAllInput();
  trace( L6, _T("-PSClearKey"));
  return STATE_NULL;
}

//////////////////////////////////////////////
// PSEnterKey
//////////////////////////////////////////////
SMStateBase  *SMSmCardManagementBase::PSEnterKey(void)
{
  trace( L6, _T("+PSEnterKey"));

  CString csInput;
  long lEntryDataLength = ps.GetInput(csInput);
  if (lEntryDataLength > 0 )
  {
	  //if the LoadAmt is true then go back to SMCardManagement else retain SMLoadLift
	  if (LoadAmt( _ttol(csInput)))
	  {
		showScreen();
	  }else
	  {
		CString csWork;
		csWork.Format(ps.GetPSText(MSG_REENTER_LOAD), co.getCardDispenserBinCapacity());
		ps.SetLeadthruText( csWork );						
		PSClearAllInput();
	  }
  }
  trace( L6, _T("-PSEnterKey"));
  return STATE_NULL;
}
// PSNumericKey
//////////////////////////////////////////////
SMStateBase  *SMSmCardManagementBase::PSNumericKey(const long lKey)
{
  trace( L6, _T("+PSNumericKey %d"), lKey );

  PSEchoInput(lKey);

  CString csVal;
  long lLen = ps.GetInput(csVal);
  long lVal = _ttol(csVal);

  if (lVal==0)
  {
    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.
  }


  trace( L6, _T("-PSNumericKey"));

  return STATE_NULL;
}

//////////////////////////////////////////////
// PSOtherSpecialKeys - the minus sign
//////////////////////////////////////////////
SMStateBase *SMSmCardManagementBase::PSOtherSpecialKeys(const long lKey)
{
  trace( L6, _T("+PSOtherSpecialKeys()"));

  PSEchoInput(lKey);

  CString csVal;
  ps.GetInput(csVal);
  long lVal = _ttol(csVal); // if csVal is '-', then lVal will be zero

  if (lVal==0)
  {
    ps.EnterClearKeys(true, false);  // disable Enter, enable Clear key.  
  }

  trace( L6, _T("-PSOtherSpecialKeys"));
  return STATE_NULL;
}

SMStateBase *SMSmCardManagementBase::PSKeystroke(const long wParam, const long lParam) 
{ 
	 trace(L6, _T("+PSKeyStroke %d"), wParam);
	if( ps.GetCurrentContext() == _T("SmLoadLift") )
    { 
      return SMStateBase::PSKeystroke( wParam, lParam );
    }
	ChangeAndShowContext();
	 
	trace(L6,_T("-PSKeyStroke") );

  return STATE_NULL; 
} 

///////////////////////////////////////////////////////////////////////////////
// Function:    ChangeAndShowContext()
// Description: This function layout the SmLoadLift context
///////////////////////////////////////////////////////////////////////////////////
void SMSmCardManagementBase::ChangeAndShowContext()
{
  trace(L6, _T("+ChangeAndShowContext()") );
  CString csWork;
  ps.SetCMFrame(_T("SmLoadLift"));
  ps.SetTitle(ps.GetPSText( MSG_LOAD ), _T("") );
  csWork.Format(ps.GetPSText(MSG_ENTER_LOAD), co.getCardDispenserBinCapacity());
  ps.SetLeadthruText( csWork );
  ps.SetButtonText(_T("SMButton1"), PS_NOTEXT);
  ps.SetButtonText(_T("SMButton8"), BTT_GOBACK);
  ps.AllowInput(20, false, false);
  ps.ShowFrame();
  trace(L6, _T("-ChangeAndShowContext()") );
}

void SMSmCardManagementBase::UnInitialize()
{
  trace(L6, _T("+Uninitialize"));
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmCardManagement") );	// RFC 330050
  trace(L6, _T("-Uninitialize"));	
}

///////////////////////////////////////////////////////////////////////////////
// Function:    LoadAmt
// Description: Determine if the enter value is less than or equal to bin capacity.
//			    If true, then call GetAdjustCardDispenserBinCount to the modify card dispenser bin count
// Parameters : [in]const long lValue - value to load
// Return Values : bool - true if the value entered is less than or equal to bin capacity.
///////////////////////////////////////////////////////////////////////////////
bool SMSmCardManagementBase::LoadAmt(const long lValue)
{
	bool bRet = false;
	trace(L6, _T("+LoadAmt"));
	if ( (m_lDispenserBinCount + lValue ) <= co.getCardDispenserBinCapacity() )
	{
		dm.GetAdjustCardDispenserBinCounts(m_lDispenserBinCount, m_lRejectBinCount, lValue);
		m_lDispenserBinChangeCount += lValue;	
		bRet = true;
	}
	trace(L6, _T("+LoadAmt"));
	return bRet;
}
