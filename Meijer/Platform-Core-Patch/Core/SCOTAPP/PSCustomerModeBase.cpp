//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSCustomerModeBase.CPP
//
// TITLE:   Display Presentation Services Customer Mode funcs.
//
// This module shows all store mode screens
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// @@@INFOPOINT
// AUTHOR:  Jason Neylon
// Altered Customer mode screens functions so that they are now more objected-oriented
// and hopefully more easy to use.
// The ps object now hold a pointer to the current customer frame.
// All the calls to that frame and it children are now made using that pointer.
// The current customer frame needs to be set from the state classes by calling setCMFrame
// Most of the CMShow arguements are now set by set function.
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
#include <afxtempl.h>               // MS MFC template header

#include "Common.h"					// MGV common includes
#include "CommonNCR.h"              // NCR common includes

#include "PSProcedures.h"           // MGV this header


// local defines
#define COMP_ID   ID_PS
#define T_ID      _T("PScmBase")

///////////////////////////////////////////////////////////////////////

#include "PSIntList.h"

// AKR extern PSProcedures ps;

DLLEXPORT extern LPTSTR cPSBmps[];
DLLEXPORT extern CString csPSTare[];

DLLEXPORT extern HDEVCONTEXT hDC;             // handle to the device context from PSThread()


//************************************************************************ 
//
// Function:		SetCMGobackButtonText
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetCMGobackButtonText(const PSTEXT nText) 
{  

	Button(_T("ButtonGoBack"), GetPSText(nText), true);
}
//************************************************************************ 
//
// Function:		ShowCMTotal
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowCMTotal(const bool fTotal,// show current total as a total T/F
                                   const bool fReTot)// show current total as retracted total T/F
{
  COleVariant v;
  v = lTotal;
  static bool fTotalShown = true;
 // show the current total on the total or the retracted total window? not both
  if (fTotal)
  {
    // show the current tax and total on the bottom of the receipt
    m_pPSX->SetTransactionVariable( _T("CMTotalAmount"), v );
	if (co.fLocaleIsTaxLine)
    {
		m_pPSX->SetConfigProperty(_T("CMTaxAmount"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		m_pPSX->SetConfigProperty(_T("CMTaxText"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);

        v = lTaxes;
		m_pPSX->SetTransactionVariable( _T("CMTaxAmount"), v );
	}
	else
	{
		m_pPSX->SetConfigProperty(_T("CMTaxAmount"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		m_pPSX->SetConfigProperty(_T("CMTaxText"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
  }
}

//************************************************************************ 
//
// Function:		ShowCMTBText
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowCMTBText(LPCTSTR csMessage)
{
	COleVariant v;
	v = csMessage;
	m_pPSX->SetTransactionVariable(_T("CMTBText"), v);
}

//************************************************************************ 
//
// Function:		ShowSMTBTextText
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowSMTBText(LPCTSTR csMessage)
{
	COleVariant v;
	v = csMessage;
	m_pPSX->SetTransactionVariable(_T("SMTBText"), v);
}

//************************************************************************ 
//
// Function:		RefreshCMTotal
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::RefreshCMTotal()
{
	ShowCMTotal(true, false);
	ShowAmountDueAmountPaid(lPSAmountDue, lPSPaid);
}


