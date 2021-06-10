//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSStoreModeBase.CPP
//
// TITLE:   Display Presentation Services Store Mode funcs.
//
// This module shows all store mode screens
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#include "CommonNCR.h"              // NCR common includes
#include "CommonServices.h"         // MGV common includes

#include "PSProcedures.h"           // MGV this header

// local defines
#define COMP_ID   ID_PS
#define T_ID      _T("PSsmBase")
#define WHOLEVEL    L7

///////////////////////////////////////////////////////////////////////
#include "PSIntList.h"

DLLEXPORT extern HDEVCONTEXT hDC;             // handle to the device context from PSThread()

//************************************************************************ 
//
// Function:		SMShowSMMItemList
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SMShowSMMItemList(void)
{
	//*** NewUI ***//
	COleVariant vValue;
	vValue = VARIANT_TRUE;
    m_pPSX->SetConfigProperty(_T("SMMItemList"), _T(""), UI::PROPERTYVISIBLE, vValue);
	return 0;

}

//************************************************************************ 
//
// Function:		SMInfo
// Purpose:			
// Parameters:
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SMInfo(LPCTSTR szInfo)	// display text in SM info box
{
	COleVariant v;
	v = VARIANT_TRUE;
    m_pPSX->SetConfigProperty(_T("SMInfoBoxField"),m_csCurrentContext, UI::PROPERTYVISIBLE, v);
    m_pPSX->SetConfigProperty(_T("SMInfoBox"), m_csCurrentContext,UI::PROPERTYVISIBLE, v);
    v=	ConvertToEscapeSequence(szInfo, _T("\\n"), _T("\n"));
    m_pPSX->SetTransactionVariable(_T("SMInfoBox"), v);
	
    return 0;
}
//************************************************************************ 
//
// Function:		ShowSMTotal
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowSMTotal(const bool fTotal)
{
  COleVariant v;
  v = lTotal;
 // show the current total on the total or the retracted total window? not both
  if (fTotal)
  {
	 if(RemoteMode())
	 {
		 COleVariant v1;
		 v1 = GetPSText(TXT_TOTAL, SCOT_LANGUAGE_PRIMARY);
		 m_pPSX->SetConfigProperty(_T("SMTotalText"), _T(""),UI::PROPERTYTEXTFORMAT, v1);
	 }
     m_pPSX->SetConfigProperty(_T("SMTotalAmount"), _T(""),UI::PROPERTYVISIBLE, VARIANT_TRUE);
	 m_pPSX->SetConfigProperty(_T("SMTotalText"), _T(""),UI::PROPERTYVISIBLE, VARIANT_TRUE);
     // show the current total on the bottom of the receipt
     m_pPSX->SetTransactionVariable( _T("SMTotalAmount"), v );
  }
  else   // don't show Total
  {
	 m_pPSX->SetConfigProperty(_T("SMTotalAmount"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
	 m_pPSX->SetConfigProperty(_T("SMTotalText"), _T(""),UI::PROPERTYVISIBLE, VARIANT_FALSE);
  }

}
