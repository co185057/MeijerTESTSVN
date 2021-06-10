    //////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // FILE:  PSProcedures.CPP
    //
    // TITLE:   Display Presentation Services Interface Procedures (High Level)
    //
    // This module handles all DPS calls for the SCOT application so that no other
    // component need know the details of DPS calls and all other components are
    // insulated from changes to the DPS API.
    //
    // From here we call the PSInterface routines
    //
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    #include <stdafx.h>                 // MS MFC always needed first
    #include <afxtempl.h>               // MS MFC template header
	  
    #include "PSProcedures.h"

  ////////////////////////////////////////////////
PSProcedures::PSProcedures()
{
  
}
#ifdef _TESTSSF
#define COMP_ID ID_PS				// Base state component
#define T_ID	_T("PSProcedures")
///////////////////////////////////////////////////////////////////////////////
// Function:    ShowCardAmountList
// Description: layout the UI of Shopping Card Amount button lists
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::ShowCardAmountList()
{
	//+set the CashBackList buttonlist to 11 buttons
	long nButtonCount = 11;
    COleVariant vReturn, v = (long) nButtonCount ;
    m_pPSX->SetConfigProperty( _T("CashBackList"), _T("SelectPayment"), UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(_T("CashBackList"), UI::PROPERTYBUTTONCOUNT, v );

	CString csImage, csImagePath, csTemp;
	GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
    COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
	//saTextArray is for the text for each button of the buttonlist
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saImageArray is for the image for each button of the button list
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saItemCodeArray is for the code for each button of the button list.
	//the code will be used by PSListLookup as a parameter.
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

	// setup an array that will be used as text and code for each button of the button lists
	int nAmt[] = {5, 10, 15, 20, 25, 30, 35, 40, 50, 100, 250};

	// setup an array that will be used as image for each button of the button lists
	CStringArray aImage;
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_five.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_ten.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_fifteen.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_twenty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_twentyfive.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_thirty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_thirtyfive.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_forty.jpg"));
	aImage.Add(csImage);
	csImage.Format(_T("%s%s"), csImagePath, _T("\\10x7_USD_fifty.jpg"));
	aImage.Add(csImage);
	aImage.Add(_T(""));
	aImage.Add(_T(""));
	for (int nIndex = 0;  nIndex  < 11; nIndex++)
	{
		csTemp.Format(_T("$%d\n"), nAmt[nIndex]);
		PutToSafeArray(nIndex, saTextArray, csTemp);
		PutToSafeArray(nIndex, saImageArray, aImage[nIndex]);				
		v = (long)nAmt[nIndex];
		saItemCodeArray.PutElement((long *)&nIndex, v );	
	}

	//tell psx to layout CashBacklist
	v = saTextArray;
    m_pPSX->SendCommand(_T("CashBackList"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saImageArray;
    m_pPSX->SendCommand( _T("CashBackList"), UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand( _T("CashBackList"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
}

///////////////////////////////////////////////////////////////////////////////
// Function:    ShowGiftCardActivityList
// Description: layout the UI of Gift Card Activity lists
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::ShowGiftCardActivityList()
{
	//+set the TenderImage buttonlist to 3 buttons
	long nButtonCount = 3;
	int nIndex = 0;
	COleVariant vReturn, v = (long) nButtonCount ;
	m_pPSX->SetConfigProperty( _T("TenderImage"), _T("Finish"), UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(_T("TenderImage"), UI::PROPERTYBUTTONCOUNT, v );
	//-set the TenderImage buttonlist to 3 buttons
	CString csImage, csImagePath;
	GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
	csImage.Format(_T("%s%s"), csImagePath ,_T("\\10x7_0413giftcard.jpg"));
	COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
	//saTextArray is for the text for each button of the buttonlist
	saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);  
	//saImageArray is for the image for each button of the button list
	saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
	//saItemCodeArray is for the code for each button of the button list.
	//the code will be used by PSListLookup as a parameter.
	saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

	//Setup Purchase New Shopping Card button
	PutToSafeArray(nIndex, saTextArray, _T("Purchase New Shopping Card"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//Setup Add Money Shopping Card button
	nIndex++;
	PutToSafeArray(nIndex, saTextArray, _T("Add Money Shopping Card"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//Setup Shopping Card Balance
	nIndex++;
	PutToSafeArray(nIndex, saTextArray, _T("Shopping Card Balance"));
	PutToSafeArray(nIndex, saImageArray, csImage);
	v = (long) (nIndex+1);
	saItemCodeArray.PutElement((long *)&nIndex, v );

	//tell psx to layout the TenderImage buttonlist
	v = saTextArray;
	m_pPSX->SendCommand(_T("TenderImage"), UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
	v = saImageArray;
	m_pPSX->SendCommand( _T("TenderImage"), UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
	v = saItemCodeArray;
	m_pPSX->SendCommand( _T("TenderImage"), UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

}
///////////////////////////////////////////////////////////////////////////////
// Function:    PutToSafeArray
// Description: Put values to safe array
// Parameters : [in] int nIndex - index to safe array
//				[in] COleSafeArray& saArray - the safe array 
//				[in]  CString csString - value to put in the safe array 
		
///////////////////////////////////////////////////////////////////////////////////
void PSProcedures::PutToSafeArray(int nIndex, COleSafeArray& saArray, CString csString)
{
	USES_CONVERSION;
#ifndef UNICODE
        saArray.PutElement((long*)&nIndex, SysAllocString( T2OLE( csString ) ) );
#else
        saArray.PutElement((long*)&nIndex, SysAllocString(  csString ) );
#endif
}


#endif
