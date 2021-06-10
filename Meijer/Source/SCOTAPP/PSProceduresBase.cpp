//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  PSProceduresBase.CPP
//
// TITLE:   Display Presentation Services Interface Procedures 
//
// This module s all DPS calls for the SCOT application so that no other
// component need know the details of DPS calls and all other components are
// insulated from changes to the DPS API.
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first   


//               // MS MFC template header
#include <afxpriv.h>                // for USES_CONVERSION

#ifndef _CPPUNIT
// BeginCodeLocale
#include "Common.h"					// Common includes



// EndCodeLocale

#include "DllDefine.h"
 
#include "ItemObject.h"    
#include "ScotPriceSound.h"
#include "PSIntList.h"
#include "SMState.h"
#include "PSXInterface.h"           // PS thread header
#include "ScotStringlist.h"

#include <io.h>
#include "windowsx.h"
#include "Scotapputils.h"			//SR629 TAR 334800 - to avoid conflict w/ SecMgr & RCM Utils.h

#include "VolumeControl.h"          // TAR 351654
#include "MPProcedures.h"
#include "PSProcedures.h" 
#else
#include "FakeCommon.h"
#include "FakeWindowCalls.h"
#include "PSProceduresBase.h"  
#endif // _CPPUNIT         // This header
#include "PSItemList.h"             // The header file for SelectFromList screen. 
#include "LaneStateData.h"
#include "PSStringManager.h"
                         
// local defines
#define COMP_ID   ID_PS
#define T_ID      _T("PSBase")
#ifndef _CPPUNIT      
// This include contains the obsolete ps functions
#include "DPSCompatibility.h"

DLLEXPORT extern ItemObject io;
DLLEXPORT extern MPProcedures mp;


static int MAX_LINEBREAKS = 3;
	
#ifndef XQUOTE
#define XQUOTE(y) _T(#y)
#define QUOTE(x) XQUOTE(x)
#endif

#define LOGPSX(func)\
do\
{\
    trace(L7,_T("Verifying:  ") QUOTE(func));\
    PSXRC throwaway_rc = (func);\
    if (throwaway_rc != PSX_SUCCESS)\
    {\
        trace(L6,_T("PSX Error detected on QUOTE(func):  %d"), throwaway_rc);\
    }\
    break;\
} while(1)


#define UPDATE_PSX_BOOL(CONTROLNAME, CONTEXTNAME, PROPERTYNAME, NEWVALUE)\
do\
{\
    LOGPSX(m_pPSX->SetConfigProperty(CONTROLNAME, CONTEXTNAME, PROPERTYNAME, NEWVALUE));\
    break;\
} while(1)


#define UPDATE_PSX_LONG(CONTROLNAME, CONTEXTNAME, PROPERTYNAME, NEWVALUE)\
do\
{\
    LOGPSX(m_pPSX->SetConfigProperty(CONTROLNAME, CONTEXTNAME, PROPERTYNAME, NEWVALUE));\
    break;\
} while(1)

LPCTSTR EMPTY_STRING = _T("");

#endif // _CPPUNIT
BOOL      LoadStringCallbackHandler( UINT nId, CString& strReturnedString );

#ifndef _CPPUNIT
extern bool ProcessDescription(LPTSTR szDesc, int nLineLength, int& nLineBreaks, int nMaxAllowedLineBreaks, LPTSTR pResultBuffer, CString& szTextToDisplay, int nLines=-1);
extern CEvent cPSThreadEvent;
#endif // _CPPUNIT


//************************************************************************ 
//
// Function:		FmtDollar
// Purpose:			This function formats values in cents to dollar.
// Parameters:      [in] lCents - value in cents (1000 for $10)
//              	SPosition -	enum from SCREEN_POSITION
// Returns:	        Returns string value containing the formatted value.
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::FmtDollar(const long lCents, SCREEN_POSITION SPosition, FORMAT_OPTION bReporting)
{

	#define StringSize 20			// Max size for a price	
	CString csValue;				// Reel Value to convert to the current currency
#ifndef _CPPUNIT
	LCID lcidLang = NULL;      // TAR 275983 - locale for currency conversion
	CURRENCYFMT CFormat;			// The CURRENCYFMT structure contains information that defines the format of a currency string	
	CString csYEN(_T("¥"));
    CString csReportingYEN(_T("\\"));   // A backslash is the yen symbol in japanese that can be coerced into a currency variant
	CFormat.NumDigits				=	co.nLocaleNoOfDigitsAfterDecimal;
	CFormat.LeadingZero	    		=	co.nLocaleLeadingZero;
	CFormat.Grouping				= co.nLocaleNoOfDigitsInGroup;
	CFormat.lpDecimalSep			= (LPTSTR)(LPCTSTR) co.csLocaleDecimalSymbol;
	CFormat.lpThousandSep			= (LPTSTR)(LPCTSTR) co.csLocaleDigitGroupingSymbol;
	CFormat.NegativeOrder			= co.nLocaleNegativeCurrencyFormat;
	CFormat.PositiveOrder			= co.nLocalePositiveCurrencyFormat;
/*    if ((bReporting) && (csYEN == co.csLocaleCurrencySymbol))
    {
        CFormat.lpCurrencySymbol = (LPTSTR)(LPCTSTR) csReportingYEN;
    }
    */
    if (REPORTINGFORMAT == bReporting)
    {
        CFormat.lpCurrencySymbol = _T("");
		 // TAR 275893 start - force U.S. style data for report XML
        CFormat.lpThousandSep = _T("");
        CFormat.lpDecimalSep = _T(".");
        CFormat.PositiveOrder = 0;
        CFormat.NegativeOrder = 1;
        lcidLang = MAKELCID(0x0409, SORT_DEFAULT);
        // TAR 275893 end
    }
    else if(CASHMANAGEMENTFORMAT == bReporting)
    { //dp185016 tar 299311
        CFormat.lpCurrencySymbol = _T("");
		    //force U.S. style data for report XML
        CFormat.lpThousandSep = _T("");
        CFormat.lpDecimalSep = _T(".");
        CFormat.PositiveOrder = 0;
        CFormat.NegativeOrder = 1;
        lcidLang = MAKELCID(0x0409, SORT_DEFAULT);
        CFormat.lpCurrencySymbol = (LPTSTR)(LPCTSTR) co.csLocaleCurrencySymbol;	
    }
    else
    {
	    CFormat.lpCurrencySymbol = (LPTSTR)(LPCTSTR) co.csLocaleCurrencySymbol;	
    }
	
	if (  (SPosition == SP_ERECEIPTCOL1) && (!co.fLocaleIsCurrencySymbolOnEReceiptCol1) ||
    (SPosition == SP_ERECEIPTCOL2) && (!co.fLocaleIsCurrencySymbolOnEReceiptCol2) ||
    (SPosition == SP_NONE) )
	{
      if (SPosition == SP_NONE)
      {
         // Formatting for RAP data processing routine with a decimal but no separators
         CFormat.lpThousandSep = _T("");
         CFormat.lpDecimalSep = _T(".");
      }
		// Clear the Currency Symbol
 		CFormat.lpCurrencySymbol = _T("");		
		// Change separation to no separation
		switch (CFormat.PositiveOrder)
		{
    // ; HELP on Positive/Negative CurrencyFormat
	// ; ------------------------------------------------------
	// ; Values for the field PositiveCurrencyFormat
	// ; ------------------------------------------------------
	// ; 0 Prefix, no separation
	// ; 1 Suffix, no separation
	// ; 2 Prefix, 1-char. separation
	// ; 3 Suffix, 1-char. separation
			case 2:	CFormat.PositiveOrder = 0; break;
			case 3: CFormat.PositiveOrder = 1; break;
			default: break;
		}
		switch (CFormat.NegativeOrder)
		{
	// ; ------------------------------------------------------
	// ; Values for the field NegativeCurrencyFormat
	// ; ------------------------------------------------------
	// ; 0 ($1.1)
	// ; 1 -$1.1
	// ; 2 $-1.1
	// ; 3 $1.1-
	// ; 4 (1.1$)
	// ; 5 -1.1$
	// ; 6 1.1-$
	// ; 7 1.1$-
	// ; 8 -1.1 $ (space before $)
	// ; 9 -$ 1.1 (space after $)
	// ; 10 1.1 $- (space before $)
	// ; 11 $ 1.1- (space after $)
	// ; 12 $ -1.1 (space after $)
	// ; 13 1.1- $ (space before $)
	// ; 14 ($ 1.1) (space after $)
	// ; 15 (1.1 $) (space before $)
	// ; ------------------------------------------------------
			case  8:	CFormat.NegativeOrder = 5; break;
			case  9:	CFormat.NegativeOrder = 1; break;
			case 10:	CFormat.NegativeOrder = 7; break;
			case 11:	CFormat.NegativeOrder = 3; break;
			case 12:	CFormat.NegativeOrder = 2; break;
			case 13:	CFormat.NegativeOrder = 6; break;
			case 14:	CFormat.NegativeOrder = 0; break;
			case 15:	CFormat.NegativeOrder = 4; break;
			default: break;		
		}	
	}

	CString cCurrencyStr = _T("");
	int cchCurrency = StringSize;	
	long localprice = lCents; 		

	if (CFormat.NumDigits>0) 	// The number of decimal digits (needed to know if we work cents or not)
	{			
		long unit = co.nLocalePriceCentUnit;
		BOOL bPosSign = localprice >= 0 ? TRUE : FALSE;
	  long left = abs(localprice / unit);
		long right = abs(localprice % unit);

		// Start of TAR 105447, Leading zero within decimal place
		CString sFormatString, sWidth;
		if (bPosSign) 
			sFormatString = _T("%li.%0");
		else
			sFormatString = _T("-%li.%0");
		sWidth.Format(_T("%li"), CFormat.NumDigits);
		sFormatString += sWidth;
		sFormatString += _T("li");
		csValue.Format(sFormatString, left, right);
		// End of TAR 105447, Leading zero within decimal place
	}
	else
	{			
		csValue.Format (_T("%li"), localprice);
	}			
	
	int c = GetCurrencyFormat(lcidLang, // Locale
							  0,		// dwFlags
  							  csValue,	
							  &CFormat,																				
  							  cCurrencyStr.GetBuffer(cchCurrency),
							  cchCurrency);
	cCurrencyStr.ReleaseBuffer();
	if (c)	
	{
		csValue = cCurrencyStr;
		return cCurrencyStr;
	}
	#endif //CPPUNIT
	// Sorry, but we failed.
	return _T("#####");
	// EndCodeLocale

}

//************************************************************************ 
//
// Function:		FmtWeight
// Purpose:			This function formats weight to string 0.00 lb or 0.000 kg.
// Parameters:      [in] lWgt - contains weight value (1000 for 1lbs) for noraml case
//                  if lWgt unstable or under weight nothing displayed in screen's
//                  weight area. If lWgt overweight --.--  is displayed in the screen's
//                  weight area.
// Returns:	        Returns string containing formatted weight
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::FmtWeight(const long lWgt)// format weight to string 0.00 lb or 0.000 kg
{
  static CString csWork;
  trace(L9,_T("PSFmtWeight %d"),lWgt);
  CString csDecimal = co.csLocaleDecimalSymbolForNumber;    // SSCOB-13812
     
  switch (lWgt)
  {
  case PS_SCALE_UNSTABLE:           // unstable
  case PS_SCALE_UNDERZERO:          // under zero
    csWork = _T("");
    break;
  case PS_SCALE_OVERWEIGHT:         // over weight
    // csWork.Format(_T("--.-- %s"),csScaleUnits);
    csWork = _T("--") + csDecimal + _T("-- ") + csScaleUnits;   // SSCOB-13812
    break;
  default:                          // normal weight
    if (co.GetfOperationsScaleMetric())
    {
      csWork.Format(_T("%5.3f %s"),1.0*lWgt/1000.0,csScaleUnits);
    }
    else
    {
      csWork.Format(_T("%4.2f %s"),1.0*lWgt/1000.0,csScaleUnits);
    }
	// SSCOB-13812: Need to consider decimal symbol configured in locale setting. If it is not a "." then fix it.
    if (csDecimal != _T("."))
    {
	   int iFind = csWork.Find(_T("."));
	   if (iFind != -1)
       {
	 	  csWork = csWork.Left(iFind) + csDecimal + csWork.Right(csWork.GetLength() - iFind -1);
       } 
    } 
    break;
  }
  return csWork;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		FmtWeightInfo
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::FmtWeightInfo(void)
{
  static CString csInfo;
    
  if (io.d.lDealQty>1 || io.d.lDealQty<-1)
  {
    csInfo.Format(
      GetPSText(TXT_WGTDEAL),
      FmtWeight(io.d.lWgtSold),
      io.d.lDealQty,
      csScaleUnits,
      FmtDollar(io.d.lUnitPrice,SP_ERECEIPTCOL1));
  }
  else
  {
    csInfo.Format(
      GetPSText(TXT_WGTEACH),
      FmtWeight(io.d.lWgtSold),
      FmtDollar(io.d.lUnitPrice,SP_ERECEIPTCOL1),
      csScaleUnits);
  }
    
  return csInfo;
}

//************************************************************************ 
//
// Function:		FmtQuantityInfo
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::FmtQuantityInfo(void)
{
  static CString csInfo;
    
  if (io.d.lDealQty>1 || io.d.lDealQty<-1)
  {
    csInfo.Format(
      GetPSText(TXT_DEALQTY),//TD 06/12/02 TAR 205460
      io.d.lQtySold,
      io.d.lDealQty,
      FmtDollar(io.d.lUnitPrice,SP_ERECEIPTCOL1));
  }
  else
  {
    csInfo.Format(
      GetPSText(TXT_QTYEACH),//TD 06/12/02 TAR 205460
      io.d.lQtySold,
      FmtDollar(io.d.lUnitPrice,SP_ERECEIPTCOL1));
  }
    
  return csInfo;
}
#endif // _CPPUNIT
//************************************************************************ 
//
// Function:		GetDeviceSpecificBMPFileNames
// Purpose:			This function reads the device specific bitmap information
//                  from the registry.
// Parameters:      None
// Returns:	        None
// Calls:           GetBmpName
//                  CheckIfBMPFileExists
// Side Effects:
// Notes:
//
//************************************************************************ 
void PSProceduresBase::GetDeviceSpecificBMPFileNames(void) 
 {
#ifndef _CPPUNIT
  HKEY hKeyDevices;
  
  csBMPforFINISHPAYSWIPE.Empty();
  csBMPforCARDSWIPE.Empty();
	csBMPforPINENTRY.Empty();
	csBMPforREQUESTSIGNATURE.Empty();

  //Get MSR device bitmaps
	long nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
	 		                    _T("SOFTWARE\\NCR\\Device Manager\\Devices\\MSR"), 
				                 0, 
					              KEY_READ, 
						           &hKeyDevices);
  if( nRC != ERROR_SUCCESS )
	  return;

  DWORD nIndex = 0, dwLength;
  TCHAR sClassName[100],sKeyValue[100];
	//Enumerate Devices Under MSR Key
	while ( RegEnumKey( hKeyDevices, nIndex++, sClassName, sizeof( sClassName ) ) == ERROR_SUCCESS )
	{
    CString sClassKey;
	  sClassKey.Format(_T("SOFTWARE\\NCR\\Device Manager\\Devices\\MSR\\%s"),sClassName);
    HKEY hKeyClass;
    nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, sClassKey, 0, KEY_READ, &hKeyClass);
    if ( nRC != ERROR_SUCCESS )
		  continue;
	  dwLength = sizeof(sKeyValue);
	  if(RegQueryValueEx(hKeyClass, _T("Configure"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
		  if(sKeyValue[0]==_T('Y') || sKeyValue[0]==_T('y'))
		  { 
			  //Don't use registry file if Bitmap is not default file
			  if(GetBmpName(BMP_FINISHPAYSWIPE) == _T("CInfoFinishPaySwipe2x20Dev"))
			  {
				  dwLength = sizeof(sKeyValue);
				  if(RegQueryValueEx(hKeyClass, _T("BMPforFINISHPAYSWIPE"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
					  if(dwLength)
						  csBMPforFINISHPAYSWIPE.Format(_T("%s"),sKeyValue);
			  }
			  if(GetBmpName(BMP_CARDSWIPE) == _T("CInfoFinishSwipe2x20Dev"))
			  {
				  dwLength = sizeof(sKeyValue);
				  if(RegQueryValueEx(hKeyClass, _T("BMPforSWIPECARD"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
					  if(dwLength)
						  csBMPforCARDSWIPE.Format(_T("%s"),sKeyValue);
			  }
			  if(GetBmpName(BMP_PINENTER_NOECHO) == _T("CInfoPINEntry2x20Dev"))
			  {
				  dwLength = sizeof(sKeyValue);
				  if(RegQueryValueEx(hKeyClass, _T("BMPforPINENTRY"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
					  if(dwLength)
						  csBMPforPINENTRY.Format(_T("%s"),sKeyValue);
			  }
		  }
		  ::RegCloseKey( hKeyClass );
	}
  ::RegCloseKey( hKeyDevices );

	CheckIfBMPFileExists(&csBMPforFINISHPAYSWIPE);
	CheckIfBMPFileExists(&csBMPforCARDSWIPE);
	CheckIfBMPFileExists(&csBMPforPINENTRY);

  //Get Signature Capture device bitmaps
	nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
	 		                    _T("SOFTWARE\\NCR\\Device Manager\\Devices\\SignatureCapture"), 
				                 0, 
					              KEY_READ, 
						           &hKeyDevices);
  if( nRC != ERROR_SUCCESS )
	  return;

  nIndex = 0;
  
	//Enumerate Devices SignatureCapture Key
	while ( RegEnumKey( hKeyDevices, nIndex++, sClassName, sizeof( sClassName ) ) == ERROR_SUCCESS )
	{
    CString sClassKey;
	  sClassKey.Format(_T("SOFTWARE\\NCR\\Device Manager\\Devices\\SignatureCapture\\%s"),sClassName);
    HKEY hKeyClass;
    nRC = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, sClassKey, 0, KEY_READ, &hKeyClass);
    if ( nRC != ERROR_SUCCESS )
		  continue;
	  dwLength = sizeof(sKeyValue);
	  if(RegQueryValueEx(hKeyClass, _T("Configure"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
		  if(sKeyValue[0]==_T('Y') || sKeyValue[0]==_T('y'))
		  { 
			  //Don't use registry file if Bitmap is not default file
			  if(GetBmpName(BMP_REQUESTSIGNATURE) == _T("CInfoRequestCapture"))
			  {
				  dwLength = sizeof(sKeyValue);
				  if(RegQueryValueEx(hKeyClass, _T("BMPforREQUESTSIGNATURE"), NULL, NULL, (LPBYTE)sKeyValue, &dwLength) == ERROR_SUCCESS)
					  if(dwLength)
						  csBMPforREQUESTSIGNATURE.Format(_T("%s"),sKeyValue);
			  }
		  }
		  ::RegCloseKey( hKeyClass );
	}
  ::RegCloseKey( hKeyDevices );

	CheckIfBMPFileExists(&csBMPforREQUESTSIGNATURE);
#endif // _CPPUNIT
 }
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		CheckIfBMPFileExists
// Purpose:			This function checks fro existance of a bitmap file.
// Parameters:      [in] pcsFileName - pointer to file name
// Returns:	        None
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
void PSProceduresBase::CheckIfBMPFileExists(CString *pcsFileName)
{
	if(pcsFileName->IsEmpty())
		return;

	_TCHAR szPath[_MAX_PATH];
	HANDLE hFile;

	_tmakepath(szPath,NULL,SCOTIMAGE,*pcsFileName,BMP_EXT);
	GET_PARTITION_PATH(szPath, szPath);

   try {
		hFile = CreateFile(szPath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile == INVALID_HANDLE_VALUE)
			pcsFileName->Empty();
		else
			CloseHandle(hFile);
	}
	catch(CFileException *e) {
		eo.FileError(RETURN,_T("CheckIfBMPFileExists"),*e);
		e->Delete();
		pcsFileName->Empty();
	}
}	 

//************************************************************************ 
//
// Function:		GetBmpName
// Purpose:			This function gets the string value associated with the
//                  passed in message id.
// Parameters:      [in] nMsg - ID for message
// Returns:	        String value associated with message id
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::GetBmpName(const int nMsg)
{
	CString csStrValue = GetStringValueFromID(nMsg, BMP_ID);
	//TRACE("Text nMsg = %d [%s]\n", nMsg, csStrValue);
	if (!csStrValue.GetLength())
	{
		CString csStrFileName = GetFileNameFromID(nMsg, BMP_ID);
		CString csMsg;
		csMsg.Format(_T("Requested Text ID = %d was not found, missing(in) %s.\n"), nMsg, csStrFileName);
		TRACE(_T("%s\n"), csMsg);
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTAPP_MISS_ID, _T("%d|%s"), nMsg, csStrFileName);
	}
	return csStrValue;
}


//************************************************************************ 
//
// Function:		GetSoundName
// Purpose:			This function gets the string value associated with the
//                  passed in message id.
// Parameters:      [in] nMsg - ID for message
// Returns:	        String value associated with message id	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CString PSProceduresBase::GetSoundName(const int nMsg)
{
	CString csStrValue = GetStringValueFromID(nMsg, WAVE_ID);
	//TRACE("BMP  nMsg = %d [%s]\n", nMsg, csStrValue);
	if (!csStrValue.GetLength())
	{
		CString csStrFileName = GetFileNameFromID(nMsg, WAVE_ID);
		CString csMsg;
		csMsg.Format(_T("Requested ID = %d was not found, missing(in) %s.\n"), nMsg, csStrFileName);
		TRACE(_T("%s\n"), csMsg);
		ScotError(RETURN,SM_CAT_CONFIGURATION, SCOTAPP_MISS_ID, _T("%d|%s"), nMsg, csStrFileName);
	}
	return csStrValue;
}
#endif //_CPPUNIT

CString PSProceduresBase::GetPSSavedText(const int nMsg)
{
    CString csStrValue;
    int nIndex = nMsg - PS_MAXTEXT; // within getpstext()
    csStrValue = szPSText[nIndex];
    if( csStrValue.Compare(_T("NOTEXT")) == 0)
    {
      csStrValue = _T("");
    }
    trace(L1,_T("PSSavedText -  text[%d]=[%s]"), nIndex, csStrValue);
    return csStrValue;
}

//************************************************************************ 
//
// Function:		GetPSText
// Purpose:			This function gets the string value associated with the
//                  passed in message id.
// Parameters:      [in] nMsg - ID for message
// Returns:	        String value associated with message id
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetPSText(const int nMsg)
{
    CString csStrValue;
    if (nMsg>=PS_MAXTEXT && nMsg<WORKMSGS_LAST)
    {   //get text from saved ps text
        csStrValue = GetPSSavedText(nMsg);
    }
    else
    {
        csStrValue = GetPSText(nMsg, m_languageUsed);
    }
    return csStrValue;
}

//************************************************************************ 
//
// Function:        GetPSText
// Purpose:         This function gets the string value associated with the
//                  passed in message id for the specified language.
// Parameters:      [in] nMsg - ID for message
//                  [in] language - Specifies the language 
// Returns:         String value associated with message id
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetPSText(const int nMsg, SCOT_LANGUAGE_TYPE language)
{
    CString csStrValue;
    csStrValue.Format(_T("ScotApp_%d"),nMsg);
    csStrValue = GetPSText(csStrValue, language);
    return csStrValue;
}

CString PSProceduresBase::GetPSText(const CString csMsg, SCOT_LANGUAGE_TYPE language)
{
     return m_pStrMgr->GetUIString(csMsg, language);   
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetPSText
// Purpose:			This function sets passed in string value for the specified
//                  message id.
// Parameters:      [in] nMsg - ID for message
//                  szText - Specifies the text 
// Returns:	        zero
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetPSText(const WORKMSGS nMsg,LPCTSTR szText)
{
  if (nMsg<PS_MAXTEXT || nMsg>=WORKMSGS_LAST) return -1;

  int iIndex = nMsg-PS_MAXTEXT;
  szPSText[iIndex] = (LPCTSTR) szText;	// within setpstext()
  return 0;
}

//************************************************************************ 
//
// Function:		AddMessageToReceipt
// Purpose:			Add message to receipt
// Parameters:      [in] csReceiptSrc - receipt data source name
// 					[in] text         - message
// Returns:			0
// Calls:			CreateReceiptItem      - create a receipt item based on
//											 item entry id
//					SetReceiptItemVariable - set value for a specific receipt 
//											 item variable
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddMessageToReceipt(const CString& csReceiptSrc, CString text)
{
    CString csMsgEntryId;
    csMsgEntryId.Format(_T("%d"), m_lMessageEntryId++);
    csMsgEntryId = _T("MSG") + csMsgEntryId;
    m_pPSX->CreateReceiptItem(csReceiptSrc, csMsgEntryId);
	COleVariant vValue;
	vValue = text;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_MSG"), vValue);

	if (csReceiptSrc != _T("CMSmallReceipt"))
	{
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csMsgEntryId, _T("ITEM_SELECTABLE"), vValue);
	}
	
	m_pPSX->UpdateReceiptControls(csReceiptSrc);  //update receipt display

	return 0;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		AddItemToReceipt
// Purpose:			Add item information to receipt
// Parameters:		[in] csReceiptSrc - receipt data source name
// Returns:			0
// Calls:			CreateReceiptItem	   - create a receipt item based on 
//										     item entry id
//					SetReceiptItemVariable - set value for a specific receipt
//											 item variable
//					UpdateReceiptControls  - update controls for display
//					SetConfigProperty      - set specific configuration property 
//											 for a control
//					GetControlProperty     - get a specific property for a 
//											 control
//					SendCommand            - send command to get a control's 
//											 property/method
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddItemToReceipt(const CString& csReceiptSrc)
{
  trace(L7,_T("+AddItemToReceipt(), ReceiptName: %s"), csReceiptSrc);

  CString csDesc = _T("");
  CString csSubItemDesc = _T("");
  CString csItemEntryId = _T("");
  CString csInfo = _T("");
  
  CStringArray csItemEntryIdList;
  COleVariant vValue, vOrigDesc, vOrigExtPrice;
  bool bFoundOldPrice = false;
  static CString csSavedItemEntryId = _T("");
 
  vOrigDesc.Clear();
  vOrigExtPrice.Clear();
 
  // csItemEntryId.Format(_T("%d"), io.d.lEntryID); //PIM
  csItemEntryId = io.d.csItemID;

  // tar381634
  if(( _T("CMSMReceipt") == csReceiptSrc))
  {
	UseCustomerLanguageForLoadString(true);	
  }

  if (!io.d.bLinkedItem)
  {
	//create a receipt item for display
	m_pPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId);
	m_csSavedItemEntryId = csItemEntryId;
  }
  else
  {
	  //create a subitem for linked item
	  //tar 233768
	  CString csNumLinkedItems;
	  csNumLinkedItems.Format(_T("%d"), nNumLinkedItems);
	  csItemEntryId = m_csSavedItemEntryId + _T("LNK") + csNumLinkedItems;
	  m_pPSX->CreateReceiptItem(csReceiptSrc, csItemEntryId, m_csSavedItemEntryId);
  }

  if (io.csOrigCode.Left(1) == LINK_ITEM)
  {
	io.csItemCode = io.csOrigCode;
  } 

  // if item is exception of type XAPPROVALNONE and TBXF_SUPPRESSPRICEDISPLAY 
  // display exception category description and do not show price
  if ((io.xd.ExceptionCode == XAPPROVALNONE) && 
	 (io.xd.dwExceptionFlags & TBXF_SUPPRESSPRICEDISPLAY))
  {
	csDesc = io.xd.csCategory;

	vValue = csDesc;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	vValue = io.csItemCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
	vValue = (long)io.cScanType;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	vValue = VARIANT_FALSE;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
  }
  else 
  {
    CString csSiblingId = csItemEntryId + _T("ORIGPRICE");
    //get original price data for updates
    if (!io.d.bLinkedItem && io.lUpdateFields) //update
    {
        if (m_pPSX->GetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDPRICE"), vValue) != PSX_SUCCESS)
        {  //get original data upon first update.
            bFoundOldPrice = true;
            m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vOrigDesc);
            m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vOrigExtPrice);
        }
    }
    //description of item
    csDesc = io.d.csDescription;
	
	vValue = csDesc;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	vValue = io.csItemCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
	vValue = (long)io.lQuantityEntered;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_QUANTITY"), vValue, true); //true means this applies to all subitems also
	vValue = (long)io.lWeightEntered;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_WEIGHT"), vValue); 
	vValue = (long)io.lTareCode;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_TARECODE"), vValue);
	vValue = (long)io.d.lExtPrice;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
	vValue = (long)io.d.lUnitPrice;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_PRICE"), vValue);
	vValue = (long)io.cScanType;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	CString csExcCategory = (io.xd.ExceptionCode == XVISUALVERIFY) ? io.xd.csCategory : _T("");
	vValue = csExcCategory;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXCEPTION_CATEGORY"), vValue);
	vValue = (long)io.d.bSecurityTag;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SECURITY_TAG"), vValue);
	vValue = VARIANT_TRUE;
	m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
 
    if (io.d.fCoupon)
    {
        vValue = VARIANT_TRUE;
        m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_COUPON"), vValue);
    }

    if (bFoundOldPrice)
    {  //Create original price line for updates if it does not already exist
        m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
        if (vValue != vOrigExtPrice  && !(vOrigDesc.vt == VT_EMPTY || vOrigDesc.vt == VT_NULL))  //SSCOADK-930, if extended price changed
        {
            m_pPSX->CreateReceiptItem(csReceiptSrc, csSiblingId, _T(""), csItemEntryId);
            vValue = VARIANT_FALSE;
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_SELECTABLE"), vValue);
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDDESC"), vOrigDesc);
            m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSiblingId, _T("ITEM_OLDPRICE"), vOrigExtPrice);
			//Tar341239
			ScrollDown(_T("SMReceipt"));
			ScrollDown(_T("CMReceipt"));
        }
    }
    
	if (io.d.bLinkedItem)
	{
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_LINKED"), vValue);
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
		//tar 233768
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			vValue = VARIANT_TRUE;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_LINKED_FMT"), vValue);
		}
	}

	// If item is restricted, by setting ITEM_RESTRICTED variable to true
	// the item will be highlighted with a color configured in XML file
	if (io.d.fRestricted)
	{
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED"), vValue);
		
		//TAR239858+ CHU 063003 If it is voided restricted item, need it to -1
		if (io.fVoid)
		{
			vValue = (long)-1;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
		}//TAR239858-
		else
		{
			vValue = (long)io.d.nRestrictedAge;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
		}
	}
    if (io.d.fVisualVerify)
    {
        vValue = VARIANT_TRUE;
        m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
        
    }
		
    if (io.lUpdateFields & ITEMUPDATE_QTY) //update of quantity information
    {
		//weight info for small receipt is treated as a subitem
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("WGT");
			m_pPSX->RemoveReceiptItem(csReceiptSrc, csSubItemId);
        }
        else
        {
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"));     
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"));
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE")); 
            m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"));
        }
    }
	// add weight info to receipt
	if (io.lWeightEntered && io.d.lUnitPrice!=0)
	{
		//weight info for small receipt is treated as a subitem
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("WGT");
			m_pPSX->CreateReceiptItem(csReceiptSrc, csSubItemId, csItemEntryId);

			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"), vValue);
			}
			
			long lWgtSold = io.d.lWgtSold;
			vValue = FmtWeight(lWgtSold);
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDWEIGHT"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE"), vValue); 
			vValue = csScaleUnits;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SCALEUNITS"), vValue); 

			CString csText = _T("");
			if (io.lWeightEntered<0)
			{
			  csText = GetPSText(TXT_CANCELED);
			}

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"), vValue);
			
			if (io.d.fRestricted)
			{
				vValue = VARIANT_TRUE;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"), vValue);
			}
            if (io.d.fVisualVerify)
            {
                vValue = VARIANT_TRUE;
                m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
            }
		}
		else
		{
			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"), vValue);
			}
			
			vValue = FmtWeight(io.d.lWgtSold);
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDWEIGHT"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE"), vValue); 
			vValue = csScaleUnits;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCALEUNITS"), vValue); 

			CString csText = _T("");
			if (io.lWeightEntered<0)
			{
			  csText = GetPSText(TXT_CANCELED);
			}

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"), vValue);
		}
	}
	// add quantity info to receipt
	else if (io.d.lQtySold > 1 || io.d.fQuantityRequired || io.d.lQtySold < -1 )
	{
		//quantity info is treated as a subitem for small receipt
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			CString csSubItemId = csItemEntryId + _T("QTY");
			m_pPSX->CreateReceiptItem(csReceiptSrc, csSubItemId, csItemEntryId);
			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_DEALQTY"), vValue);
			}
			
			long lQtySold = io.d.lQtySold;		
			vValue = (long)lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_SUFFIXTEXT"), vValue);
			
			if (io.d.fRestricted)
			{
				vValue = VARIANT_TRUE;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csSubItemId, _T("ITEM_RESTRICTED"), vValue);
			}
            if (io.d.fVisualVerify)
            {
                vValue = VARIANT_TRUE;
                m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VISUALVERIFY"), vValue);
            }
		}
		else
		{
			if (io.d.lDealQty>1 || io.d.lDealQty<-1)
			{
				vValue = (long)io.d.lDealQty;
				m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_DEALQTY"), vValue);
			}
			
			vValue = (long)io.d.lQtySold;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SOLDQTY"), vValue);
			vValue = (long)io.d.lUnitPrice;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_UNITPRICE"), vValue); 

			CString csText = _T("");
			if (io.lQuantityEntered<1)
			  csText = GetPSText(TXT_CANCELED);

			vValue = csText;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SUFFIXTEXT"), vValue);
		}
	}
  }
  if (io.fVoid && csReceiptSrc != _T("CMSmallReceipt"))
  {
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue);
  }
  TraceItemVariables(csReceiptSrc, csItemEntryId);
  m_pPSX->UpdateReceiptControls(csReceiptSrc); 

  //check to see if need to show more info on CMSmallReceipt
  if (csReceiptSrc == _T("CMSmallReceipt"))
  {
	  ShowMoreInfoOnReceipt(_T("CMSmallReceipt"), _T("CMSmallReceiptMoreInfo"));
  }

  // tar381634
  if(( _T("CMSMReceipt") == csReceiptSrc))
  {
	UseCustomerLanguageForLoadString(false);
  }

  trace(L7,_T("-AddItemToReceipt()"));

  return 0;
} 

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		RestrictedItemNotAllowed
// Purpose:			Void items which meet age requirement from the restricted item list
// Parameters:		[in] nCustomerAge                - customer's age
//					[out] lUnapprovedRestrictedItems - number of unapproved restricted 
//													   items
// Returns:			Number of unapproved restricted items	
// Calls:			FindReceiptItems       - retrieve receipt items from the receipt
//					GetReceiptItemParent   - get parent item entry id base on item
//										     entry id
//					GetReceiptItemVariable - get the value of the specified receipt item 
//										     variable
//					SetReceiptItemVariable - set value for the specified receipt item 
//										     variable
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::RestrictedItemNotAllowed(const int nCustomerAge, long& lUnapprovedRestrictedItems)
{
	  CStringArray csItemEntryIdList, csParentItemEntryIdList;
	  CString csItemEntryId, csParentItemId;
	  m_pPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdList);
	  int nTmpCountList = csItemEntryIdList.GetSize();

	  for (int k=0; k<nTmpCountList; k++)
	  {
		  csItemEntryId = csItemEntryIdList.GetAt(k);
		  m_pPSX->GetReceiptItemParent( _T("SMReceiptRestricted"), csItemEntryId, csParentItemId );
		  if (!csParentItemId.GetLength())
		  {
			  csParentItemEntryIdList.Add(csItemEntryId);
		  }
		  csParentItemId =""; //TAR383762
	  }

	  long ltmpUnapprRestItems = csParentItemEntryIdList.GetSize();
	  long i = ltmpUnapprRestItems - 1;

      long lAge = 0;
      bool bAutoApprovedItem = false;
	  
	  // TAR209317 DON"T want to muck with original value of this flag!!!
	  // use a tempory local to decrement and return.  This flag is decremented in TBItemDetails() LPM071002
	  //lUnapprovedRestrictedItems=i;		//TAR 193136 Age 21 restricted item was sold to 19 year old if they purchased age 18 item first

	  COleVariant vValue;
	  while (i>=0)
	  {
		vValue.Clear();
		csItemEntryId = csParentItemEntryIdList.GetAt(i);
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
        vValue.ChangeType(VT_INT);
		lAge = vValue.lVal;

		if (nCustomerAge >= lAge && lAge != -1 && lAge !=0)    
		{	
			// Collect all entries to be voided from SMReceiptRestricted 
			vValue = VARIANT_TRUE;
			//TAR 242784    
/*			if(co.fOperationsAutoVoidUnderageRestrict)  // Tar 159241 if AutoVoidUnderageRestrict is false, don't cross out the item
			{
				m_pPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_VOID"), vValue, true);
				vValue = VARIANT_FALSE;
				m_pPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_SELECTABLE"), vValue, true);
	  
			}
*/			//TAR 174877
			bAutoApprovedItem = true;
			vValue = (long)bAutoApprovedItem;
			m_pPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
            //update CMSMReceipt for NGUI
            m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
        
			if (ltmpUnapprRestItems)
                ltmpUnapprRestItems--;
		}
		//*** end of NewUI ***//

		//TAR 193136 NN 013002 if the item has been voided then take it off the list
		if (lAge==-1) 
		{
			if (ltmpUnapprRestItems)		
               ltmpUnapprRestItems--;
		}
		//TAR 195280 Begin  	
		//This "item" is retrieved from PSRcpt->lCountList, but it is the quantity of the 
		//linked restricted item. The lAge of this "item" was returned as zero, therefore, 
		//we need to check for this condition. 
		if (lAge == 0) 			
		{
			if (ltmpUnapprRestItems)		
               ltmpUnapprRestItems--;
		}
		//TAR 195280 End
		i--;
	  }
	  return ltmpUnapprRestItems;
}

//************************************************************************ 
//
// Function:		FindRequiredAge
// Purpose:			Get required age from the restricted item list
// Parameters:		[in] nCustomerAge - customer's age				
// Returns:			Restricted age from the restricted item
// Calls:			FindReceiptItems       - retrieve items from the receipt
//					GetReceiptItemVariable - get the value of the specified 
//										     receipt item variable
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::FindRequiredAge(const long nCustomerAge) 
{
      CStringArray csItemEntryIdList;
	  m_pPSX->FindReceiptItems(_T("SMReceiptRestricted"), csItemEntryIdList);
	  int nCountList = csItemEntryIdList.GetSize();

	  int i = nCountList - 1;

      int lAge = 0;
      bool bAutoApprovedItem = false;
	  
	  CString csItemEntryId;
	  COleVariant vValue;
	  while (i>=0)
	  {
		csItemEntryId = csItemEntryIdList.GetAt(i);

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
        vValue.ChangeType(VT_INT);
		lAge = vValue.lVal;

		if (nCustomerAge < lAge && lAge != -1 && lAge !=0)   //TB//This item is Ok to sell
		{	
			return lAge;
		}

		i--;
	  }
	  return 0;
}

//************************************************************************ 
//
// Function:		ScanReceiptForExceptionCategorys
// Purpose:			Get all the exception categories from the receipt items		
// Parameters:		[out] csArray - array of exception categories
// Returns:			None
// Calls:			FindReceiptItems       - retrieve items from the receipt
//					GetReceiptItemVariable - get the value of the specified 
//										     receipt item variable
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ScanReceiptForExceptionCategorys(CStringArray &csArray)
{
	USES_CONVERSION;

	CStringArray csItemEntryIdList;
	m_pPSX->FindReceiptItems(_T("SMReceiptVisualItem"), csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize();

	CString csCat; 
	long i=0;

	CString csItemEntryId;
	COleVariant vValue;
	while(i<nCountList)    
	{
		vValue.Clear();
		csItemEntryId = csItemEntryIdList.GetAt(i);
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_EXCEPTION_CATEGORY"), vValue);
        vValue.ChangeType( VT_BSTR );
		csCat = OLE2T(vValue.bstrVal);

		vValue.Clear();
		PSXRC rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_VOID"), vValue);
        if (rc != PSX_SUCCESS)
			rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

		if (-1 != csCat.Find(_T("RewardLine")))
		{
			csCat.Empty();
		}
		if (csCat.GetLength() && rc != PSX_SUCCESS)
		{
			csArray.Add(csCat);
		}
		i++;
	}
}

//************************************************************************ 
//
// Function:		ScanReceiptForSecurityTagItems
// Purpose:			Find any security tag item from the receipt
// Parameters:		None
// Returns:			TRUE if find any item
//					FASLE otherwise
// Calls:			FindReceiptItems       - retrieve items from the receipt
//					GetReceiptItemVariable - get the value of the specified 
//										     receipt item variable
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::ScanReceiptForSecurityTagItems(void)
{
	bool bFound = false;
	COleVariant vValue = (long)1;

	CStringArray csItemEntryIdList;
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList, _T(""), _T("ITEM_SECURITY_TAG"), vValue);
	int nCountList = csItemEntryIdList.GetSize();
	
	int i = 0;	
    CString csItemEntryId;
	while(i < nCountList)    
	{
		csItemEntryId = csItemEntryIdList.GetAt(i);
		vValue.Clear();
		PSXRC rc = m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOID"), vValue);
		if (rc != PSX_SUCCESS)
			rc = m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

		if (rc != PSX_SUCCESS)
		{
			bFound = true;
			break;
		}
		i++;
	}
	return bFound;
}

//************************************************************************ 
//
// Function:		ScanReceiptToVoid
// Purpose:			Get the receipt item entry id from the entry id list based
//					on the index
// Parameters:		[in] csReceiptSrc   - receipt data source name
//					[in] csReceipt      - receipt control name
//					[in] nIndex         - index mapped to the item entry id
//					[out] csItemEntryId - item entry id
// Returns:			count of items in the list	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ScanReceiptToVoid(const CString& csReceiptSrc, CString& csItemEntryId, int nIndex)
{
  csItemEntryId = _T("");
  CStringArray csItemEntryIdList;

  m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryIdList, _T(""), _T("ITEM_SELECTABLE"), VARIANT_TRUE);
	
  int nCountList = csItemEntryIdList.GetSize();

  if (nIndex>=0 && nCountList)
	csItemEntryId = csItemEntryIdList.GetAt(nIndex);

  return nCountList;
}

//************************************************************************ 
//
// Function:		VoidItemFromReceipt
// Purpose:			This function voids an item form receipt.
// Parameters:      [in] fAdd - If TRUE indicates to add a new line to receipt
//                              in the case where no exact match on this void
//                              was found
//                  [in] csReceiptSrc   - receipt data source name
// Returns:	        0  - If item was found on the receipt and voided
//                  1  - if Item was not found on the receipt to void
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::VoidItemFromReceipt(const bool fAdd, const CString& csReceiptSrc)
{
  USES_CONVERSION;
	
  CString csItemEntryId = _T("");
  COleVariant vValue;
  CString csItemCode = _T("");
  bool bFound = false;
  long lExtPrice;
  long lBarcodeType; //TAR261289
  PSXRC rc;
  
  if (!io.d.csItemID.IsEmpty())
  {
      csItemEntryId = io.d.csItemID;
  }
  else if ( csReceiptSrc == _T("PickListReceipt") )
  {
	  m_pPSX->GetSelectedReceiptItem(_T("PickListReceiptVoid"), csItemEntryId);
  }
  else
  {
	  m_pPSX->GetSelectedReceiptItem(_T("CMReceiptVoid"), csItemEntryId);
  }
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
  vValue.ChangeType( VT_BSTR );
  csItemCode = OLE2T(vValue.bstrVal);

  vValue.Clear();
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
  vValue.ChangeType( VT_INT );
  lExtPrice = vValue.lVal;

  //TAR261289+
  vValue.Clear();
  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
  vValue.ChangeType( VT_INT );
  lBarcodeType = vValue.lVal;
  //TAR261289-
  
  static CString csSavedEntryId = _T("");
  if (!io.d.bLinkedItem)
  {
	  csSavedEntryId = csItemEntryId;
	  vValue.Clear();
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue);
  }
  else
  {
	  vValue.Clear();
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csSavedEntryId, _T("ITEM_VOID"), vValue);
/*	  if (rc == PSX_SUCCESS)  //TAR 290534
	  {
		  return 0;
	  } */
  }

  vValue.Clear();
  if (rc != PSX_SUCCESS)
	  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);
		
  CString csItemCode1 = csItemCode;
  if (csItemCode1.GetLength())
  {
	  if (csItemCode1.Left(3) == _T("]e0"))  //RFC 412256
				csItemCode1 = csItemCode1.Right(csItemCode1.GetLength() - 3); 
	  else
	  {
		  if (!_istdigit(csItemCode1[0]))
		  {
			csItemCode1.SetAt(0, _T(' '));
		  }
		  csItemCode1.TrimLeft();
	  }
  }
  CString csItemCode2 = io.csItemCode;
  if (csItemCode2.GetLength())
  {
	  if (csItemCode2.Left(3) == _T("]e0"))  //RFC 412256
				csItemCode2 = csItemCode2.Right(csItemCode2.GetLength() - 3); 
	  else
	  {
		  if (!_istdigit(csItemCode2[0]))
		  {
			csItemCode2.SetAt(0, _T(' '));
		  }
		  csItemCode2.TrimLeft();
	  }
  }
  //TAR261289+
  CString csItemCode1WithOutCheckDigit = _T("");
  CString csItemCode2WithOutCheckDigit = _T("");
  
  if (    ( abs (csItemCode1.GetLength() - csItemCode2.GetLength() ) == 1  ) 
	   && ( ( lBarcodeType != 0  && io.cScanType == 0 ) || (lBarcodeType == 0  && io.cScanType != 0) )
	 )
  {
	  if (csItemCode1.GetLength() > csItemCode2.GetLength() )
	  {
		  csItemCode1WithOutCheckDigit = csItemCode1.Left(csItemCode1.GetLength()-1);
	  }
	  else
	  {
		  csItemCode2WithOutCheckDigit = csItemCode2.Left(csItemCode2.GetLength()-1);
	  }

  }

  //if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
  //{
  //	  bFound = true;
  //}
  if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) )
  {
	  if (    (csItemCode1 == csItemCode2) 
		   || (csItemCode1 == csItemCode2WithOutCheckDigit )
		   || (csItemCode2 == csItemCode1WithOutCheckDigit )
		 )
	  {
		  bFound = true;
	  }
  }
  //TAR261289-
  else
  {
	  CStringArray csItemEntryList;
	  vValue = io.csItemCode;
      if (!io.d.bLinkedItem)
      {
          m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryList, _T(""), _T("ITEM_SELECTABLE"), VARIANT_TRUE);
      }
      else
      {
          m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryList, csSavedEntryId);
      }
	  int lCountList = csItemEntryList.GetSize();
	  if (lCountList)
	  {
		  for (int i=lCountList-1; i>=0; i--)
		  {
			  vValue.Clear();
			  csItemEntryId = csItemEntryList.GetAt(i);
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_CODE"), vValue);
			  vValue.ChangeType(VT_BSTR);
			  csItemCode = OLE2T(vValue.bstrVal);

			  vValue.Clear();
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
			  vValue.ChangeType(VT_INT);
			  lExtPrice = vValue.lVal;

			  //TAR261289+
			  vValue.Clear();
			  m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
			  vValue.ChangeType( VT_INT );
			  lBarcodeType = vValue.lVal;
			  //TAR261289-

			  vValue.Clear();
			  rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue);
			  if (rc != PSX_SUCCESS)
				rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

			  csItemCode1 = csItemCode;
			  if (csItemCode1.GetLength())
			  {
				  if (!_istdigit(csItemCode1[0]))
				  {
					csItemCode1.SetAt(0, _T(' '));
				  }
				  csItemCode1.TrimLeft();
			  }
			  csItemCode2 = io.csItemCode;
			  if (csItemCode2.GetLength())
			  {
				  if (!_istdigit(csItemCode2[0]))
				  {
					csItemCode2.SetAt(0, _T(' '));
				  }
				  csItemCode2.TrimLeft();


			  }
			  if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
			  {
				  bFound = true;
				  break;
			  }
			  //TAR261289+   
			  csItemCode1WithOutCheckDigit = _T("");
			  csItemCode2WithOutCheckDigit = _T("");

			  if (    ( abs (csItemCode1.GetLength() - csItemCode2.GetLength() ) == 1  ) 
            	   && ( ( lBarcodeType != 0  && io.cScanType == 0 ) || (lBarcodeType == 0  && io.cScanType != 0) )
	             )
			  {
	              if (csItemCode1.GetLength() > csItemCode2.GetLength() )
				  {
		              csItemCode1WithOutCheckDigit = csItemCode1.Left(csItemCode1.GetLength()-1);
				  }
				  else
				  {
					  csItemCode2WithOutCheckDigit = csItemCode2.Left(csItemCode2.GetLength()-1);
				  }

			  }


			  //if ((csItemCode1 == csItemCode2) && (rc != PSX_SUCCESS) && io.d.lExtPrice == 0-lExtPrice)
			  if ( (rc != PSX_SUCCESS) && (io.d.lExtPrice == 0-lExtPrice) )
			  {
				 if (    (csItemCode1 == csItemCode2) 
					  || (csItemCode1 == csItemCode2WithOutCheckDigit )
		              || (csItemCode2 == csItemCode1WithOutCheckDigit )
		            )
				 {

			        bFound = true;
				    break;
				 }
			  }			  //TAR260736-   
              else if (io.d.bLinkedItem && (io.d.lExtPrice == 0-lExtPrice))
              {
                  bFound = true;
                  break;
              }
			  //TAR261289-
		  }
	  }
  }
  if (bFound)
  {
	  vValue = VARIANT_TRUE;
	  m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_VOID"), vValue, true);
	  vValue = VARIANT_FALSE;
	  m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_SELECTABLE"), vValue, true);

	  //TAR239858+ CHU 063003: if it is voided and restricted, need to set age to -1
	  if (io.d.fRestricted) 
	  {
	  	vValue = (long)-1;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, _T("ITEM_RESTRICTED_AGE"), vValue);
	  }
	  //TAR239858-
      //Remove pre-update price information and skip bagging lines.
      CString csSiblingId = csItemEntryId + _T("ORIGPRICE");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      csSiblingId = csItemEntryId + _T("ShowSkipBagging");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      csSiblingId = csItemEntryId + _T("ShowSkipBaggingRap");
      m_pPSX->RemoveReceiptItem(csReceiptSrc, csSiblingId);
      TraceItemVariables(csReceiptSrc, csItemEntryId);
	  m_pPSX->UpdateReceiptControls(csReceiptSrc); 
	  return 1;
  }

  if (fAdd && !bFound)
  {
		// there was no exact match on this void
		// so we can't find a line to strike out
		// instead we add a new line as a void
		long    lSvQty   = io.d.lQtySold;
		long    lSvWgt   = io.d.lWgtSold;
		io.d.lQtySold *= -1;            // show negative quantity on receipt
		io.d.lWgtSold *= -1;            // show negative weight on receipt
		AddItemToReceipt(csReceiptSrc);
		io.d.lQtySold = lSvQty;
		io.d.lWgtSold = lSvWgt;

		//TAR 397432
		if ( csReceiptSrc == _T("PickListReceipt") && !io.d.fCoupon)
		{
			m_lTotalPicklistItemsSold = m_lTotalPicklistItemsSold + io.d.lQtySold;
		}
  }
  return 0;
}

//************************************************************************ 
//
// Function:		ShowAmountDueAmountPaid
// Purpose:			This function sets values for the amount due and amount paid 
//                  transaction variables on the view.
// Parameters:      [in] lDue  - Amount due value
//                  [in] lPaid - Amount paid value
// Returns:	        None
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowAmountDueAmountPaid(const long lDue, const long lPaid)
{

  lPSAmountDue = lDue;
  lPSPaid = lPaid;
  COleVariant v;
  v = lDue;
  m_pPSX->SetTransactionVariable( _T("Amount1"), v );
  //reseting NextGenUIChangeDue and NextGenUIRefundDue to 0
  CLaneStateData laneStateData(m_pPSX);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountDue"), lDue);
  laneStateData.SendNextGenUIData( _T("NextGenUIChangeDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIRefundDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountPaid"), lPaid);

  v = lPaid;
  m_pPSX->SetTransactionVariable( _T("Amount2"), v );
  v = GetPSText(TXT_AMOUNTDUE, mp.sCurr->m_customerLanguage);	//SSCOADK-7019
  m_pPSX->SetConfigProperty(_T("Amount1Label"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);

}
#endif
//************************************************************************ 
//
// Function:		ShowChangeDueAmountPaid
// Purpose:			This function sets values for the change due and amount paid 
//                  transaction variables on the view.
// Parameters:      [in] lDue  - Amount due value
//                  [in] lPaid - Amount paid value
// Returns:	        None	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowChangeDueAmountPaid(const long lChange, const long lPaid, bool bChangeDueText)
{

  COleVariant v;
  v = lChange;
  m_pPSX->SetTransactionVariable( _T("Amount1"), v );
  CLaneStateData laneStateData(m_pPSX);
  laneStateData.SendNextGenUIData( _T("NextGenUIChangeDue"), lChange);
  //reseting NextGenUIRefundDue to 0
  laneStateData.SendNextGenUIData( _T("NextGenUIRefundDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountPaid"), lPaid);

  v = lPaid;
  m_pPSX->SetTransactionVariable( _T("Amount2"), v );
  v = GetPSText(TXT_CHANGEDUE);
  if(!bChangeDueText)
  {
     v = GetPSText(_T("Change"), m_languageUsed);
  }
  m_pPSX->SetConfigProperty(_T("Amount1Label"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);

}
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		UpdateAmountDueAmountPaid
// Purpose:			This function sets the global variables: lPSAmountDue and
//                  lPSPaid for the amount due and amount paid.
// Parameters:      [in] lDue  - Amount due value
//                  [in] lPaid - Amount paid value
// Returns:	        None	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::UpdateAmountDueAmountPaid(const long lDue, const long lPaid)
{
  lPSAmountDue = lDue;
  lPSPaid = lPaid;

  CLaneStateData laneStateData(m_pPSX);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountDue"), lDue);
  laneStateData.SendNextGenUIData( _T("NextGenUIChangeDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIRefundDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountPaid"), lPaid);
  return;
}

//************************************************************************ 
//
// Function:		ShowAmountRefundedAmountPaid
// Purpose:			This function sets values for the amount refunded and amount 
//                  paid transaction variables on the view.
// Parameters:      [in] lDue  - Amount due value
//                  [in] lPaid - Amount paid value
// Returns:	        None	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowAmountRefundedAmountPaid(const long lRefund, const long lPaid)
{

  COleVariant v;
  v = lRefund;
  m_pPSX->SetTransactionVariable( _T("Amount1"), v );

  CLaneStateData laneStateData(m_pPSX);
  laneStateData.SendNextGenUIData( _T("NextGenUIRefundDue"), lRefund);
  //reseting NextGenUIChangeDue to 0
  laneStateData.SendNextGenUIData( _T("NextGenUIChangeDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountDue"), (long)0);
  laneStateData.SendNextGenUIData( _T("NextGenUIAmountPaid"), lPaid);

  v = lPaid;
  m_pPSX->SetTransactionVariable( _T("Amount2"), v );
  
  v = GetPSText(TXT_AMOUNTREFUNDED);
  m_pPSX->SetConfigProperty(_T("Amount1Label"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
}

//************************************************************************ 
//
// Function:		Total
// Purpose:			This function sets the lTotal global variable.
// Parameters:      [in] lTol - Value for total
// Returns:	        0    
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Total(const long lTot)// show new total
{
    lTotal = lTot;
    return 0;
}

//************************************************************************ 
//
// Function:		Tax
// Purpose:			This function sets the lTaxes global variable.
// Parameters:      [in] lTax - Value for tax
// Returns:	        0	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Tax(const long lTax)// show new tax
{
    lTaxes = lTax;
    return 0;
}

//************************************************************************ 
//
// Function:		Weight
// Purpose:			This function shows scale's weight on the view
// Parameters:      lWeight - Value for weight
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Weight(const long lWeight)   // show new weight
{
	COleVariant v;
	if (!co.fStateDisplayWeightsAndMeasures || !dm.IsDeviceConfigured(DMCLASS_SCALE)) //SSCOP-3714
	{
        UPDATE_PSX_BOOL(_T("CMWeight"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		UPDATE_PSX_BOOL(_T("SMScaleImage"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		UPDATE_PSX_BOOL(_T("ScaleClassLogo"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		UPDATE_PSX_BOOL(_T("CMScaleImage"), _T(""), UI::PROPERTYVISIBLE, VARIANT_FALSE);
		return 0;
	}
	else
	{
        UPDATE_PSX_BOOL(_T("CMWeight"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		UPDATE_PSX_BOOL(_T("SMScaleImage"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		UPDATE_PSX_BOOL(_T("ScaleClassLogo"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
		UPDATE_PSX_BOOL(_T("CMScaleImage"), _T(""), UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}

    trace(L7, _T("Setting weight ..."));
    lScaleWeight = lWeight;  // Set class member var. to new value.
	v=FmtWeight(lWeight);
	m_pPSX->SetTransactionVariable( _T("CMWeight"), v );
    return 0;
}

//************************************************************************ 
//
// Function:		Prompt
// Purpose:			This function displays text in prompt area.
// Parameters:      [in] nText - Identifies message id
// Returns:	        0 
// Calls:           GetPSText
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Prompt(PSTEXT nText)// display text in prompt area
{
  return Prompt(GetPSText(nText));
}



//************************************************************************ 
//
// Function:		Prompt
// Purpose:			This function displays text in prompt area.
// Parameters:      [in] szText - Text string to display
// Returns:	        0 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Prompt(LPCTSTR szText)// display text in prompt area
{
	COleVariant v;
	v=szText;
	m_pPSX->SetTransactionVariable( _T("Prompt"), v );
    return 0;
}
    
//************************************************************************ 
//
// Function:		Echo
// Purpose:			This function displays text in echo area.
// Parameters:      [in] nText - Identifies message id
// Returns:	        0
// Calls:           Echo
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Echo(PSTEXT nText)// display text in echo area
{
   return Echo(1,GetPSText(nText));
}

//************************************************************************ 
//
// Function:		Echo
// Purpose:			This function displays text in echo area.
// Parameters:      [in] szText - Text string to display
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Echo(LPCTSTR szText, LPCTSTR szText2)
{
   long rc = Echo(1, szText);
   if (szText2 != NULL)
	 rc = Echo(2, szText2);  
   return rc;
}
//************************************************************************ 
//
// Function:		Echo
// Purpose:			This function sets the echo transaction variable. There 
//                  can be up to 8 lines((ie. RSC Assist mode) to echo. Each 
//                  identified by Echo through Echo7.
// Parameters:      [in] echoLineNumber - Identifies line number coressponding 
//                                        to Echo controls
//                  [in] szText - Text to display
// Returns:	
// Calls:           ConvertToEscapeSequence
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Echo(short echoLineNumber, LPCTSTR szText)
{
	COleVariant v;
	v = ConvertToEscapeSequence(szText, _T("\\n"), _T("\n"));
    switch (echoLineNumber)
	{
		case 1:
			m_pPSX->SetTransactionVariable( _T("Echo"), v );
			break;
		case 2:
			m_pPSX->SetTransactionVariable( _T("Echo1"), v );
			break;
		case 3:
			m_pPSX->SetTransactionVariable( _T("Echo2"), v );
			break;
		case 4:
			m_pPSX->SetTransactionVariable( _T("Echo3"), v );
			break;
		case 5:
			m_pPSX->SetTransactionVariable( _T("Echo4"), v );
			break;
		case 6:
			m_pPSX->SetTransactionVariable( _T("Echo5"), v );
			break;
		case 7:
			m_pPSX->SetTransactionVariable( _T("Echo6"), v );
			break;
		case 8:
			m_pPSX->SetTransactionVariable( _T("Echo7"), v );
			break;
        default:
            break;
	}
    return 0;
}
//************************************************************************ 
//
// Function:		Message12
// Purpose:			This function displays text in message 12 area
// Parameters:      [in] szText - Text to display 
// Returns:	        0
// Calls:           Message12
// Side Effects:
// Notes:
//
//************************************************************************
///////////////////////////////////////////////////////////////////////
long PSProceduresBase::Message12(LPCTSTR szText)// display text in message 12 area
{
    return Message12(szText, false);
}

//************************************************************************ 
//
// Function:		Message12Modal
// Purpose:			This function displays text in message 12 area
// Parameters:      [in] szText - Text to display
// Returns:	        0
// Calls:           Message12
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Message12Modal(LPCTSTR szText)// display text in message 12 area
{
    return Message12(szText, true);
}

//************************************************************************ 
//
// Function:		Message12
// Purpose:			This function displays text in message 12 area
// Parameters:      [in] nText - Identifies message id
//                  [in] bIsModal - If set to TRUE text displays in InstructionTextArea,
//                  otherwise in MessageBoxNoEcho area
// Returns:	        0
// Calls:           Message12
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Message12(PSTEXT nText, bool bIsModal)
{
    return Message12(GetPSText(nText), bIsModal); // by default bIsModal is false

}

//************************************************************************ 
//
// Function:		Message12
// Purpose:			This function displays text in InstructionTextArea if 
//                  bIsModal is TRUE, otherwise in MessageBoxNoEcho area.
// Parameters:      [in] szText - Text to display
//                  [in] bIsModal - If set to TRUE text displays in InstructionTextArea,
//                  otherwise in MessageBoxNoEcho area
// Returns:	        0
// Calls:           ConvertToEscapeSequence
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Message12(LPCTSTR szText, bool bIsModal)
{
	COleVariant v;
    PSXRC rc;
    CString csPrefix = _T("");
    if (ps.RemoteMode())
    {
        csPrefix = co.GetContextPrefix();
    }

	v = ConvertToEscapeSequence(szText, _T("\\n"), _T("\n"));
	//TAR 394961 -- deleted unnecessary SetTransactionVariable call.
	if (bIsModal)
	{
		//tar407726 & 408135 - check if control is set properly
		rc = m_pPSX->SetConfigProperty( csPrefix + _T("InstructionTextArea"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
		if(PSX_SUCCESS != rc)
		{
		    trace(L4,_T("No %sInstructionTextArea, setting InstructionTextArea"),csPrefix);
            rc = m_pPSX->SetConfigProperty( _T("InstructionTextArea"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
		    if(PSX_SUCCESS != rc)
			{
		        trace(L6,_T("PSMessage12() - Something's wrong. Control not found!"));
			}
		}
	}
	else
	{
		m_pPSX->SetConfigProperty( csPrefix + _T("MessageBoxNoEcho"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
		m_pPSX->SetConfigProperty( _T("AttractMessageBoxNoEcho"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);//TAR 394961		
	}
    return 0;
}
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		ScrollUP
// Purpose:			This function scrolls up the specified receipt by one item
// Parameters:      [in] csReceipt      - receipt control name
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ScrollUp(const CString& csReceipt)
{
  COleVariant vReturn, vScrollMode;
  vScrollMode = (long)UI::StepUp;
  m_pPSX->SendCommand(csReceipt, UI::COMMANDSCROLLSELECTION, vReturn, 1, vScrollMode);
  return 0;
}

//************************************************************************ 
//
// Function:		ScrollDown
// Purpose:			This function scrolls down the specified receipt by one item
// Parameters:      [in] csReceipt      - receipt control name
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ScrollDown(const CString& csReceipt)// scroll this receipt down
{
  COleVariant vReturn, vScrollMode;
  vScrollMode = (long)UI::StepDown;
  m_pPSX->SendCommand(csReceipt, UI::COMMANDSCROLLSELECTION, vReturn, 1, vScrollMode);
  return 0;
}
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		AddItem
// Purpose:			This function adds an item to CMSMReceipt, and CMSmallReceipt,
//                  SMReceiptRestricted, SMReceiptCoupon, & SMReceiptVisualItem
//                  when applicable.
// Parameters:      None
// Returns:	        0
// Calls:           AddItemToReceipt
//                  VoidItemFromReceipt
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddItem(void)
{
  //tar 233768
  if (!io.d.bLinkedItem)
  {
	  nNumLinkedItems = 0;
  }
  else
  {
	  nNumLinkedItems++;
  }
  if (io.fVoid)
  {
	long    lSvQty   = io.d.lQtySold;
	long    lSvWgt   = io.d.lWgtSold;
	io.d.lQtySold *= -1;   // show negative quantity if voided
	io.d.lWgtSold *= -1;   // show negative weight if voided
	if (!io.d.bLinkedItem)
	  m_pPSX->ClearReceipt(_T("CMSmallReceipt"));
	AddItemToReceipt(_T("CMSmallReceipt"));		                                     // void from CMSmallReceipt
	io.d.lQtySold = lSvQty;
	io.d.lWgtSold = lSvWgt;	

    CString csItemEntryId = _T("");
    if ( SMStateBase::fInMultiSelectPickList && !io.d.fCoupon)
    {
        m_pPSX->GetSelectedReceiptItem(_T("PickListReceiptVoid"), csItemEntryId);
        if (!csItemEntryId.IsEmpty())
        {
            ps.SetSelection(_T("CMReceiptVoid"), csItemEntryId);
        }
    }

	VoidItemFromReceipt((io.lCoupCode>0 ? true:false), _T("SMReceiptCoupon"));	     // void from SMReceiptCoupon receipt

   	VoidItemFromReceipt((io.lVisualCode>0 ? true:false), _T("SMReceiptVisualItem")); // void from SMReceiptVisualItem receipt

	VoidItemFromReceipt((io.lRestCode>0 ? true:false), _T("SMReceiptRestricted"));	 // void from SMReceiptRestricted receipt
    	 	
    VoidItemFromReceipt(true, _T("CMSMReceipt"));		                             // void from CMReceipt & SMReceipt
        	   	     
	if ( SMStateBase::fInMultiSelectPickList && !io.d.fCoupon)
	 {
		 VoidItemFromReceipt(true, _T("PickListReceipt"));
		 m_lTotalPicklistItemsSold = m_lTotalPicklistItemsSold - io.d.lQtySold;
	 }

    return 0;
  }
    
  if ( SMStateBase::fInMultiSelectPickList)
  {
	  AddItemToReceipt(_T("PickListReceipt")); 
	  if(!io.d.fCoupon)
	  {
		  m_lTotalPicklistItemsSold = m_lTotalPicklistItemsSold + io.d.lQtySold;
		  m_lTotalPicklistItemsIncludeVoidItems = m_lTotalPicklistItemsIncludeVoidItems + io.d.lQtySold;//SR679  
	  }
  }
 
  AddItemToReceipt(_T("CMSMReceipt"));            //add item to CMReceipt & SMReceipt

  if (!io.d.bLinkedItem)
	  m_pPSX->ClearReceipt(_T("CMSmallReceipt")); //add item to CMSmallReceipt
  AddItemToReceipt(_T("CMSmallReceipt"));
   
  if (io.d.fRestricted)
  {
    AddItemToReceipt(_T("SMReceiptRestricted"));  //add item to SMReceiptRestricted receipt
  }
    
  if (io.d.fCoupon)
  {
    AddItemToReceipt(_T("SMReceiptCoupon"));      //add item to SMReceiptCoupon receipt
  }
    
  if (io.d.fVisualVerify)
  {
    AddItemToReceipt(_T("SMReceiptVisualItem"));  //add item to SMReceiptVisualItem receipt
  }
 
  return 0;
}

//************************************************************************ 
//
// Function:		SalesReceiptEmpty
// Purpose:			This function checks if CMReceipt is empty or not.
// Parameters:      none
// Returns:	        Returs FALSE if receipt is not empty, TRUE if empty
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::SalesReceiptEmpty(void) //Tar 127562 Check for the receipt is empty or not
{
	COleVariant vReturn;	
    m_pPSX->SendCommand(_T("CMReceipt"), UI::COMMANDGETCOUNT, vReturn);
	int nCountList = vReturn.lVal;

	return (nCountList>0 ? false:true);
}
//************************************************************************ 
//
// Function:		GetItem
// Purpose:			This function gets the current receipt item.
//                  
// Parameters:      [out] csDesc - Item description
//                  [out] csCode - Item code
//                  [out] lQuantity - Item quantity
//                  [out] lWeight - Item Weight
//                  [out] lPrice - Item price
//                  [out] lUPrice - Item extended price
//                  [out] lTareCode -Item Tare code
//                  [out] lBardcodeType - Item barcode type
//                  [in] csReceiptSrc - receipt data source name
//                  [in] csReceipt - receipt name
//                  [in] csItemEntryCodeId - unique item entry code
//
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetItem(CString &csDesc,// get the current receipt item
                               CString &csCode,
                               long    &lQuantity,
                               long    &lWeight,
                               long    &lPrice,
                               long    &lUPrice,
                               long    &lTareCode,
							   long    &lBarcodeType,
							   const CString& csReceiptSrc,
							   const CString& csReceipt,
							   const CString& csItemEntryId)
{
	USES_CONVERSION;
	CString csSelItemEntryId = _T("");
	CString csTmpItemEntryId = _T("");
	COleVariant vValue;
	m_pPSX->GetSelectedReceiptItem(csReceipt, csSelItemEntryId);
	if (!csItemEntryId.GetLength())
	{
		csTmpItemEntryId = csSelItemEntryId;
	}
	else
	{
		csTmpItemEntryId = csItemEntryId;
	}
	if (csTmpItemEntryId.GetLength())
	{
		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
		vValue.ChangeType( VT_BSTR );
		csDesc = OLE2T(vValue.bstrVal);

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_CODE"), vValue);
		vValue.ChangeType( VT_BSTR );
		csCode = OLE2T(vValue.bstrVal);

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_QUANTITY"), vValue);
		vValue.ChangeType( VT_INT );
		lQuantity = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_WEIGHT"), vValue);
		vValue.ChangeType( VT_INT );
		lWeight = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_PRICE"), vValue);
		vValue.ChangeType( VT_INT );
		lUPrice = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
		vValue.ChangeType( VT_INT );
		lPrice = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_TARECODE"), vValue);
		vValue.ChangeType( VT_INT );
		lTareCode = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
		vValue.ChangeType( VT_INT );
		lBarcodeType = vValue.lVal;

		vValue.Clear();
		PSXRC rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_VOID"), vValue);

		if (rc != PSX_SUCCESS)
			rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csTmpItemEntryId, _T("ITEM_VOIDADDED"), vValue);
		if (rc != PSX_SUCCESS)
			return 1;
	}
	return 0;
}

//************************************************************************ 
//
// Function:		ResetTransactionLists
// Purpose:			This function clears all receipt controls.
// Parameters:      none
// Returns:	        0 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetTransactionLists(void)// reset all receipts to empty them
{
  m_pPSX->ClearReceipt(_T("CMSMReceipt"));
  m_pPSX->ClearReceipt(_T("CMSmallReceipt"));
  m_pPSX->ClearReceipt(_T("SMReceiptRestricted"));
  m_pPSX->ClearReceipt(_T("SMReceiptCoupon"));
  m_pPSX->ClearReceipt(_T("SMReceiptVisualItem"));
  m_pPSX->ClearReceipt(_T("SMMItemList"));
  m_pPSX->ClearReceipt(_T("MediaList"));
  m_pPSX->ClearReceipt(_T("DetailsList"));
  m_pPSX->ClearReceipt(_T("PickListReceipt"));
  m_pPSX->ClearReceipt(_T("PickListReceiptVoid"));
  //Reset Quickpick 
  m_pPSX->SetControlProperty( s_szQuickPickButtonListName, UI::PROPERTYBUTTONCOUNT, (COleVariant) 0L );
  csPSCustomerRewardMessage = co.csCMCustomerMessage;
  m_lMessageEntryId = 0;
    
  lTotal = 0;
  lTaxes = 0;
  lPSAmountDue = 0;
  lPSPaid = 0;
  lPSPointsTotal = 0;	// total reward points for this trx
  lPSSavingsTotal = 0;	// total savings for this trx
  
  return 0;
}
  

//************************************************************************ 
//
// Function:		GetTareAt
// Purpose:			This function gets the tare code associated with the 
//                  specified list index.
// Parameters:      lIndex - index to the list (1-based)
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetTareAt(const long lIndex)

{
  if (lIndex < 1 || lIndex > MAXTARE)
      return 0;
  return lPSTareCode[m_languageUsed][lIndex-1];
return 0;
}
//************************************************************************ 
//
// Function:		ClearInput
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ClearInput(void)// clear all input from echo area
{
  if (!nInputDigits) return 0;
    
  *szInputDigits = 0;
  *szInputAsterx = 0;
  Echo(PS_BLANK);
//  Echo(_T("_"));
  // Disable Enter and Clear keys. 
  EnterClearKeys(true, true);

  return 0;
}

//************************************************************************ 
//
// Function:		BackSpaceInput
// Purpose:			This functions backs up one echo input area character
// Parameters:      none
// Returns:	        Returns echoed input 
// Calls:           EchoInput
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::BackSpaceInput(void)// back up one echo input area character
{
  return EchoInput(0x08);
}

//************************************************************************ 
//
// Function:		AllowInput
// Purpose:			This function allows echo input digits to the echo area
// Parameters:      [in] lMax - Max number of disgits to echo to the echo area
//                  [in] fHide - Hide input as *'s T/F
//                  [in] fDollar - format inout as dollar amount $0.00 T/F
//                  [in] lMin - Min number of input digits before enabling Enter
// Returns:	        Returns length of echoed input
// Calls:           ClearInput
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AllowInput(const long lMax, // allow echo input digits
                              const bool fHide,
                              const bool fDollar,
                              const long lMin) //RFC 335528
{
  nInputDigits = lMax;
  fInputHide  = fHide;
  fInputDollar = fDollar;


  //RFC 335528
  if (lMin > lMax)  
  {
      nMinInputDigits = lMax; //Don't allow min to exceed max
  }
  else
  {
      nMinInputDigits = lMin;
  }
  //RFC 335528 - end
  ClearInput();
    
  if (lMax<=0) return 0;
  if (nInputDigits>MAX_DIGITS) nInputDigits = MAX_DIGITS;

  CString csNextGenData;
  csNextGenData.Format(_T("MinLen=%d;MaxLen=%d;Private=%s;Currency=%s"),
                       nMinInputDigits, nInputDigits, 
                       fHide ? _T("true") : _T("false"),
                       fDollar ? _T("true") : _T("false"));

  CLaneStateData laneStateData(m_pPSX);
  laneStateData.SendNextGenUIData( _T("NextGenUIEchoField"), csNextGenData);


    
  return _tcsclen(szInputDigits);
}

//************************************************************************ 
//
// Function:		EchoInput
// Purpose:			This function echos input digits to the echo area
// Parameters:      [in] cDigit - character to echo
//                  [in] bAllowAlpha - TRUE allows alpha characters to echo
//                                     FALSE numeric characters to echo(default)
// Returns:	        Returns echoed input
// Calls:           EnterClearKeys
//                  FmtDollar
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::EchoInput(const _TCHAR cDigit, // echo input digits to the echo area
                                 bool bAllowAlpha,	  // default true
								 bool bAsteriskMask)  // default true  
{
  _TCHAR szWork[2];
  CString csOut;
    
  if (!nInputDigits) return 0;

  if (cDigit == 0x08) 
  {//backspace
    int i = _tcsclen(szInputDigits);
    if (i>0)
    {
      i--;
      szInputDigits[i] = 0x00;
      szInputAsterx[i] = 0x00;
    }
  }
  else
  {
    int i = _tcsclen(szInputDigits);
    if (i >= nInputDigits) return -2;	// only allow up to a max of digits

    szWork[0] = cDigit;
    szWork[1] = 0;
    _tcscat(szInputDigits,szWork);
    _tcscat(szInputAsterx,_T("*"));
  }

  bool bDisableEnter = true;
  if (fInputHide)
  {
    csOut = szInputAsterx;
    bDisableEnter = csOut.GetLength() < nMinInputDigits;
    if (csOut.GetLength()<nInputDigits)
	  csOut += _T("_");
    // show Enter and Clear if numerickeypad is visible.
    EnterClearKeys(bDisableEnter, false); //Only enable clear key

  }
  else
  {
    bool bDisableClear = false;	//tar418130
    csOut = szInputDigits;

    bDisableEnter = csOut.GetLength() < nMinInputDigits;
    bDisableClear = csOut.GetLength() == 0;	//tar418130

    if (fInputDollar)
    {
      csOut = FmtDollar(_ttol(szInputDigits));
    }
    else
    { 
	  if (bAsteriskMask)
	  {
		  int nLen = nInputDigits - csOut.GetLength();
		  for (int i=0; i<nLen; i++)
		  {
			  csOut += _T("*");
		  }
	  }
	  else
	  {
		  if (csOut.GetLength()< nInputDigits)
			  csOut += _T("_");
	  }
	  // show Enter and Clear if numerickeypad is visible.
    }

    EnterClearKeys(bDisableEnter, bDisableClear); 
  }

  Echo(csOut);
  return _ttol(szInputDigits);
}

//************************************************************************ 
//
// Function:		EnterClearKeys
// Purpose:			This function enables or disables Enter and Clear keys 
//                  if numeric or alphanumeric keyboards are visible.
// Parameters:      [in] disableEnter - TRUE - disables Enter key
//                                      FALSE - enables Enter key
//                  [in] disableClear - TRUE - disables Clear key
//                                      FALSE - disables Clear key
// Returns:	        none
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::EnterClearKeys(const bool disableEnter,
		                              const bool disableClear)

{
	COleVariant v1, v2;
	CString csClearCtlName, csEnterCtlName, csEnterCtlNameForAlphaNumKB;
	if(disableEnter)
        v1 = (long) UI::Disabled;
	else
        v1 = (long) UI::Normal;
	if (disableClear)
        v2 = (long) UI::Disabled;
	else

        v2 = (long) UI::Normal;

	if ((( m_csCurrentContext.Find(_T("SmDataEntry")) == 0 ) && 
		(m_csCurrentContext.Find(_T("SmDataEntryAlphanumeric")) == -1 ) ) ||
		(m_csCurrentContext == _T("SmEnterBirthdate")) || 
		(m_csCurrentContext == _T("SmmKeyInWtTol")) ||
		(m_csCurrentContext == _T("SmmKeyInItemCode")) ||  // SR821
		(m_csCurrentContext == _T("SmCashierPassword")) || //dp185016 support glory device
		(m_csCurrentContext == _T("SmLoadLift")) || //dp185016 tesco cash management
    (co.IsInAssistMenus()))
	{
		csClearCtlName = _T("SMNumericKeyBoard");
		csEnterCtlName = _T("KeyBoardP4");
	}
	else
	{
		csClearCtlName = _T("NumericP3");
		csEnterCtlName = _T("NumericP4");
        csEnterCtlNameForAlphaNumKB = _T("AlphaNumKey3");
	}

	//TAR385361+
    if ( m_csCurrentContext.Find(_T("SmDataEntryAlphanumeric")) != -1)
	{
       csEnterCtlNameForAlphaNumKB = _T("AlphaNumKey3");
	}
    //TAR385361-

    m_pPSX->SetControlProperty(csEnterCtlName, UI::PROPERTYSTATE, v1);
    m_pPSX->SetControlProperty(csClearCtlName, UI::PROPERTYSTATE, v2);
    m_pPSX->SetControlProperty(csEnterCtlNameForAlphaNumKB, UI::PROPERTYSTATE, v1);
    //dp185016 tesco cash management
    if( m_csCurrentContext == _T("SmLoadLift") )
    {
      m_pPSX->SetControlProperty( _T("MinusKey"), UI::PROPERTYSTATE, 
        (v2.lVal == UI::Normal)?(long)UI::Disabled : (long)UI::Normal);
    }
    
    //+SR828
    if(m_csCurrentContext == _T("CmDataEntryWithKeyBoard") ||
       m_csCurrentContext == _T("EnterCouponValue") ||
	   m_csCurrentContext == _T("EnterQuantity") ||
       m_csCurrentContext == _T("KeyInCode") ||
       m_csCurrentContext == _T("SellBags"))	//SSCOP-882
    {
        m_pPSX->SetControlProperty(_T("Backspace"), UI::PROPERTYSTATE, v2);
    }
    //-SR828
}


//************************************************************************ 
//
// Function:		EchoDecimalInput
// Purpose:			This function echos user input decimal to echo area
// Parameters:      [in] cDigit - character to echo
//                  [in] bAllowAlpha - TRUE allows alpha characters to echo
//                                     FALSE numeric characters to echo(default)
// Returns:	        Returns echoed input
// Calls:           EnterClearKeys
//                  FmtDollar
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::EchoDecimalInput(const _TCHAR cDigit, bool bAllowAlpha)// echo input digits to the echo area
{
  _TCHAR szWork[2];
  CString csOut;

//TAR324840
  int cchData 
  = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, NULL, NULL );
  _TCHAR *pccbBuffer = new _TCHAR[(UINT)cchData];
        ::GetLocaleInfo( LOCALE_USER_DEFAULT,
                         LOCALE_SMONDECIMALSEP,
                         pccbBuffer, 
                         cchData); 
  _TCHAR cLocalizedDecimal = pccbBuffer[0];
  delete [] pccbBuffer;

  if (!nInputDigits) return 0;
  {
    int i = _tcsclen(szInputDigits);
    if (i >= nInputDigits) return -2;	// only allow up to a max of digits
    szWork[0] = cLocalizedDecimal;
    szWork[1] = 0;
    _tcscat(szInputDigits,szWork);
    _tcscat(szInputAsterx,_T("*"));
  }
  if (fInputHide)
  {
    csOut = szInputAsterx;
    if (csOut.GetLength()<nInputDigits) csOut += _T("_");
  }
  else
  {
    if (fInputDollar)
    {
      csOut = FmtDollar(_ttol(szInputDigits));
    }
    else
    {
      csOut = szInputDigits;
      if (csOut.GetLength()<nInputDigits) csOut += _T("_");
    }

  }
  EnterClearKeys();  // Enable Enter and Clear keys.

  if (!IsCharAlpha(cLocalizedDecimal) || bAllowAlpha) 
    Echo(csOut);

  return _ttol(szInputDigits);
}


#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		GetInput
// Purpose:			This function gets the current input echoed data
// Parameters:      [out] csInput - input echoed data
// Returns:	        Returns length of echoed input
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetInput(CString &csInput)// get the current input echoed data
{
  if (!nInputDigits) return 0;
  csInput = szInputDigits;
  return _tcsclen(szInputDigits);
}
//************************************************************************ 
//
// Function:		GetPsxObject
// Purpose:			This function gets a pointer to PSX object that was created 
//                  at Initialize.  Initialize must be called before this function
//                  is called.
// Parameters:      none
// Returns:	        m_pPSX - pointer to the PSX object
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CPSX * PSProceduresBase::GetPSXObject()
{
	if( m_bPsxInitialized )
	{
		return m_pPSX;
	}
	return NULL;
}

#ifndef _CPPUNIT
//+SR815 - Personalization
//************************************************************************ 
//
// Function:		BuildControlsList
// Purpose:			Builds a list of controls that use the StringID setting.
//                  The list is used to assign property translated text to
//                  controls.
// Parameters:      none
// Returns:	        0
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::BuildControlsList()
{
    static const TCHAR s_XMLBOOLTRUE[] = _T("True");
    static const TCHAR s_XMLPRIMARYLANGUAGE[] = _T("PrimaryLanguage");
    static const TCHAR s_XMLSTRINGID[] = _T("StringId");
  
    controlsForLangSwitch.RemoveAll();
    listButtonLists.RemoveAll();
    long nNumControls, nNumPrimaryControls;
    nNumControls = 0;
    nNumPrimaryControls = 0;

    CString strValue( _T("") );
    CStringArray strContextNameArray, strControlNameArray;
    BOOL bPrimaryLanguage = FALSE;
    if( PSX_SUCCESS == m_pPSX->GetContextNameList( strContextNameArray ) )
    {
        for( int i = 0; i < strContextNameArray.GetSize(); i++ )
        {
            if( PSX_SUCCESS == m_pPSX->GetControlNameList( strControlNameArray, strContextNameArray[i] ) )
            {
                for( int j = 0; j < strControlNameArray.GetSize(); j++ )
                {
                    bPrimaryLanguage = FALSE;
                    if( PSX_SUCCESS == m_pPSX->GetCustomDataVar( s_XMLPRIMARYLANGUAGE, strValue, _T(""), strContextNameArray[i] ) )
                    {
                        bPrimaryLanguage = ( strValue == s_XMLBOOLTRUE );
                    }
                    
                    if( PSX_SUCCESS == m_pPSX->GetCustomDataVar( s_XMLSTRINGID, strValue, strControlNameArray[j], strContextNameArray[i] ) )
                    {
                        
                        if( strValue.GetLength() )
                        {
                            nNumControls ++;
                            if (bPrimaryLanguage) nNumPrimaryControls++;
                            STRINGID_CONTROL* pControlDef = new STRINGID_CONTROL;
                            pControlDef->bPrimary = bPrimaryLanguage;
                            pControlDef->csContext = strContextNameArray[i];
                            pControlDef->csControl = strControlNameArray[j];
                            pControlDef->stringID = _ttoi( strValue );
                            controlsForLangSwitch.Add(pControlDef);
                        }
                    }
                    
                }
            }
        }
    }
    trace( L6, _T("Total controls=%d, Total Primary Controls %d, Total Customer mode = %d"),
            nNumControls, nNumPrimaryControls, nNumControls-nNumPrimaryControls);
    if( PSX_SUCCESS == m_pPSX->GetControlNameList( strControlNameArray ) )
    {
        COleVariant vType( _T("") );
        
        for( int i = 0; i < strControlNameArray.GetSize(); i++ )
        {
            if( PSX_SUCCESS == m_pPSX->GetConfigProperty( strControlNameArray[i], _T(""), UI::PROPERTYCONTROLTYPE, vType ) && _tcscmp( _bstr_t( vType.bstrVal ), UI::CONTROLTYPEBUTTONLIST ) == 0 )
            {
                listButtonLists.Add(strControlNameArray[i]);
            }
        }
    }
    
}

//************************************************************************ 
//
// Function:		ConvertToEscapeSequence
// Purpose:			This function converts the embedded '\' and 'n' characters 
//                  to an actual escape sequence '\n' new line. 
// Parameters:      [in] lpszText - Text containing embedded characters
//                  [in] lpszString - identifies the embedded characters to convert to
//                                    specified escape sequence
//                  [in] lpszEscSeq - identifies the escape sequence
// Returns:	        Returns text with the embedded escape sequence characters 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::ConvertToEscapeSequence(LPCTSTR lpszText, LPCTSTR lpszString, LPCTSTR lpszEscSeq)
{
	CString csTemp;
	csTemp = lpszText;
	int nIndex = 0;
	while (nIndex != -1)
	{
		nIndex = csTemp.Find(lpszString, nIndex);
		if (nIndex != -1)
		{
			// Commented BMediano
			//csTemp.Delete(nIndex, 2);
			//csTemp.Insert(nIndex, lpszEscSeq);

			//070904 : TAR 254760 BMediano - BEGIN
			//Fixed which will display properly the embedded newlines(\\n).
			//see TBscotdataentry.ini on c:\scot\Config
			if( nIndex > 0 )
			{	
				if( csTemp.Mid(nIndex-1,1) == _T("\\"))
				{
					csTemp.Delete(nIndex-1, 1);
					nIndex += 1;
				}
				else
				{
					csTemp.Delete(nIndex, 2);
					csTemp.Insert(nIndex, lpszEscSeq);
				}
			}
			else
			{
				csTemp.Delete(nIndex, 2);
				csTemp.Insert(nIndex, lpszEscSeq);
			}
			//070904 : TAR 254760 BMediano - END
		}
	}
	return csTemp;
}
#endif // _CPPUNIT
//************************************************************************ 
//
// Function:		Initialize
// Purpose:			This function is called to do all initilaization
// Parameters:      [in] szBmpsName - path name of SCOTBMPS.DAT file
//                  [in] szTextName - path name of SCOTMSGS.DAT file
//                  [in] szPlstName - path name of SCOTPLST.DAT file or NULL
//                  [in] szTareName - path name of SCOTTARE.DAT file or NULL
//                  [in] nTare1 - tare button 1 tare 1-10
//                  [in] nTare2 - tare button 2 tare 1-10
//                  [in] fMetric - Metric scale T/F
//                  [in] fLiveVid - Live video installed T/F
// Returns:	        0  - Success
//                  -1 - Failure
// Calls:           UnInitializeConfigData
//                  LoadPSText
//                  LoadSCOTStringConstants
//                  GetDeviceSpecificBMPFileNames
//          		AssignTextToAllCtlsInAllCtxs
// Side Effects:
// Notes:           
//
//************************************************************************
long PSProceduresBase::Initialize(LPCTSTR szBmpsName,
                              LPCTSTR szTextName,
                              LPCTSTR szPlstName,
                              LPCTSTR szTareName,
                              const int nTare1,
                              const int nTare2,
                              const bool fMetric,
                              const bool fLivVid)
{
    
	m_lMessageEntryId = 0;
	m_lMediaListEntryId = 0;
    m_lDetailsListEntryId = 0;
	m_lAMTareListEntryId =0;
	m_lSmmEntryId = 0;
	m_lDelaySecurityEntryId = 0;
    m_bFoundPickListItemsFromSearch= false;

    // (+) SR804
    ResetTenderMap();
    // (-) SR804

#ifndef _CPPUNIT
    if (m_pStrMgr == NULL)
    {
        m_pStrMgr = new CPSStringManager();
        LoadLanguageUIStrings(SCOT_LANGUAGE_PRIMARY);
    }
#endif
    PSXRC rc = PSX_E_FAIL;
	if( !m_bPsxInitialized )
	{
		trace( L0, _T(__TIMESTAMP__) );
    	
		if( CPSX::Initialize() )
        {
            ScotError( FATAL,SM_CAT_SYSTEM_CALL, SCOTAPP_PSX_CREATE );
	        return -1;
        }
#ifndef _CPPUNIT
        m_pPSX = new CPSX( &CPSXInterface::EventHandler );
#else
        m_pPSX = new CPSX();
#endif // _CPPUNIT
        CRect rect( 0, 0, 0, 0 ); //TAR292883
	    if( PSX_SUCCESS != m_pPSX->CreateDisplay( rect ) )
	    {
            ScotError( FATAL,SM_CAT_SYSTEM_CALL, SCOTAPP_PSX_CREATE );
	        return -1;
        }    
            
        m_pPSX->SetDisplayView( DisplayStandard );
//      SSCOADK-1885 NextGenUI - Allow SSCOUI to control visibility	
//        m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYVISIBLE, VARIANT_FALSE );
 
 #ifndef _DEBUG
	    if( !_tgetenv( _T("SCOTPC") ) )
	    {
            // Disable Cursor //
            m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYCURSOR, VARIANT_FALSE );

            // Make sure taskbar is not topmost //
            const DWORD ABM_SETSTATE = 0x0000000A;
            const TCHAR TASKBAR_WNDCLASS[] = _T("Shell_TrayWnd");
            APPBARDATA abd = { 0 };
            abd.cbSize = sizeof( APPBARDATA );
            abd.hWnd = ::FindWindow( TASKBAR_WNDCLASS, NULL );
            SHAppBarMessage( ABM_SETSTATE, &abd );
            
            // ++ NextGenUI screen should be the one TopMost
            // Put UI topmost //
            //COleVariant v;
            //if( PSX_SUCCESS == m_pPSX->GetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYHWND, v ) )
            //{
            //    ::SetWindowPos( (HWND)v.lVal, HWND_TOPMOST, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE );
            //}
            // NextGenUI
        }
#endif
    	
        BOOL bRetStatus = LoadSCOTStringConstants( co.csConfigFilePath );
		ASSERT( bRetStatus );

		// Set the environment variable for XML config file //
		CString csWork;
		GET_PARTITION_PATH(SCOTIMAGE, csWork.GetBuffer(_MAX_PATH));
		CPSX::SetEnvironmentVariable( _T("MediaPath"), csWork );
		csWork.ReleaseBuffer();

		GET_PARTITION_PATH(SCOTSOUND, csWork.GetBuffer(_MAX_PATH));
		CPSX::SetEnvironmentVariable( _T("AudioPath"), csWork );
		csWork.ReleaseBuffer();
	
        // Text on the task bar //
		m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYTEXT, _T("NCR SCOT") );


		rc = m_pPSX->SetLoadStringCallback( LoadStringCallbackHandler );
        if( PSX_SUCCESS != rc )
        {
         	trace( L6, _T("PS Initialize - PSX SetLoadStringCallback failed! rc = %d"), rc );
        }

		LoadPSText( szTextName, false );
		GetDeviceSpecificBMPFileNames();

		m_bPsxInitialized = true;
		
        if( co.GetfCMRewardInfo() )
		{
		    co.SetCMCustomerMessage(GetCustomerMessageDefault()); //SSCOB-1860

		    csPSCustomerRewardMessage = co.GetCMCustomerMessage();
		}
	}
	else
	{
        // Set the environment variable for AssistMode xml file path //
	    TCHAR szWork[_MAX_PATH] = _T("");
        _tmakepath( szWork, NULL,co.csConfigFilePath, NULL, NULL );
	    CPSX::SetEnvironmentVariable( _T("ConfigPath"), szWork );
	    CPSX::SetEnvironmentVariable( _T("AssistModeFileName"), ( co.GetcsTemplateAssistMode().Find( _T(".xml") ) != -1 ) ? co.GetcsTemplateAssistMode() : co.GetcsTemplateAssistMode() + _T(".xml") );
    
	    // Load the UI XML config file
	    TCHAR szXMLFileName[_MAX_PATH] = _T("");
	    TCHAR szXMLOverrideFileName[_MAX_PATH] = _T("");
	    _tmakepath( szXMLFileName, NULL,co.csConfigFilePath, co.GetcsDataXMLFile(), _T("XML") );
	    _tmakepath( szXMLOverrideFileName, NULL,co.csConfigFilePath, co.GetcsDataXMLFile(), _T("000") );
	    CFileFind FileSearch;
	    if( !FileSearch.FindFile( szXMLFileName ) )
	    {
		    ScotError( FATAL, SM_CAT_CONFIGURATION, SCOTAPP_MISS_FILE, _T("%s"), szXMLFileName );
            return -1;
	    }

	    rc = m_pPSX->LoadConfigFile( szXMLFileName, FileSearch.FindFile( szXMLOverrideFileName ) ? szXMLOverrideFileName : _T("") );
        if( PSX_SUCCESS != rc )
        {
       	    trace( L0, _T("PS Initialize - unable to load XML UI configuration file. rc = %d"), rc );
            return -1;
        }

        m_bPsxConfigLoaded = true;
//      SSCOADK-1885 NextGenUI - Allow SSCOUI to control visibility	    
//	    m_pPSX->SetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYVISIBLE, VARIANT_TRUE );
        COleVariant v;
        m_pPSX->GetControlProperty( UI::CONTROLDISPLAY, UI::PROPERTYHWND, v );
        HWND hWnd = (HWND)v.lVal;
        SetActiveWindow( hWnd );
     	
		if(co.GetfStateDualLanguage())
		{
			ps.SetRemoteLanguage(co.Getlanguage(0).szCode);
		}

		UnInitializeConfigData();    
		InitializeConfigData( szPlstName, szTareName );
    }

    nLastBmp = BMP_COUNTER;
	trace( L8,_T("PS initialized") );
    
    return 0;
};

void PSProceduresBase::ResetTenderMap()
{
    for (int i=0; i<MAXBUTTONNUM; ++i)
    {
        const TenderButton& btn = co.GetTenderButton(m_languageUsed, i);
        m_TenderBtnMapByBtnNum[ btn.nButtonNum ] = btn.csTenderText;
    } 
}

#ifndef _CPPUNIT

HRESULT PSProceduresBase::StartRemoteServer()
{    
	// Start Remote Server
	PSXRC rc = m_pPSX->StartServer( _T("FastLaneRemoteServer") );
    if( PSX_SUCCESS != rc )
    {
        trace( L6, _T("PS Initialize - PSX StartServer failed! rc = %d"), rc );
		ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_PSX_START_SERVER_FAILED,_T("%d"), rc); // TAR 294682
    }

    return (PSX_SUCCESS == rc);
}


//************************************************************************ 
//
// Function:		UnInitialize
// Purpose:			This function is called to do all unInitialization 
// Parameters:      none
// Returns:	        0 
// Calls:           LoadPSText
//                  ResetTransactionLists
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::UnInitialize(void)
{
	trace(L8,_T("Enter PS un-initialized"));
    ResetTransactionLists();
    //Stop Remote Server
	m_pPSX->StopServer();
    for (int i=0; i<(WORKMSGS_LAST - PS_MAXTEXT); i++)
    {
	    szPSText[i].Empty(); 
    }
    for (i=SCOT_LANGUAGE_PRIMARY; i<SCOT_LANGUAGE_MAX; i++)
    {  
        for (int j=0; j<MAXTARE; j++)
        {
            csPSTare[i][j].Empty();     //   = (LPCTSTR) "";
            lPSTareCode[i][j] = 0;
        }
    }
    
    csScaleUnits.Empty();
    LoadPSText( NULL, false );

	// TAR 317284 / 307999 - BEGIN
	OSVERSIONINFO verInfo;
	memset( &verInfo, 0, sizeof( verInfo ) );
	verInfo.dwOSVersionInfoSize = sizeof( verInfo );
	if	( GetVersionEx( &verInfo ) )
	{

		if	( verInfo.dwMajorVersion > 4 )
		{
			// Weird Bug - if you can solve it, please do. 
			// After calling delete on PSX, the CoCreateInstance in 
			// RPSW CNCREventLog::OpenA or CNCREventLog::OpenW 
			// begins to hang or crash, especially on NT.  
			// Also, OleUninitialize() hangs when 
			// CScottAppApp::ExitInstance() is returned from. 
			// I tried simply disabling logging at shutdown, 
			// but it was not enough of a solution, since 
			// ScotApp still hung or crashed at return from ExitInstance.
			delete m_pPSX;
			m_pPSX = NULL;
		    CPSX::Uninitialize();
		}
	}
	// TAR 317284 / 307999 - END
   	
    if (m_SigCapWindow && !m_bUsingPSXSigCapWindow)
    {
        DestroyWindow(m_SigCapWindow);
        m_SigCapWindow = NULL;
        m_bUsingPSXSigCapWindow = false;
        UnregisterClass(SIGCAP_WINDOWCLASS, AfxGetApp()->m_hInstance);
    }


    // TAR 393382 - Add tracing to verify that volume ctl thread exits.
    if(m_pVolCtl)
    {
        // +TAR 351654
        delete m_pVolCtl;    
        m_pVolCtl = NULL;
        // -TAR 351654

        trace(L6, _T("Disabled delayed volume control ..."));
    }

    trace( L8,_T("Exit PS un-initialized") );
    return 0;
}
   
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		LoadPSText
// Purpose:			This function loads message texts
// Parameters:      [in] szFileName - not used
//                  [in] fRepErr - not used
// Returns:	        0     
// Calls:           GetListSize
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::LoadPSText(LPCTSTR szFileName,const bool fRepErr)
{
#ifndef _CPPUNIT
  int nLastMsgNum = GetListSize(MSG_SCOTCORE_LANG_ONE) + 1;
  if (nLastMsgNum < PS_MAXTEXT && fRepErr)
  {
    //ScotError((RETURN,"Missing %d message(s) in file %s",
    //PS_MAXTEXT-nLastMsgNum,szFileName);
	//ScotError((RETURN,SM_CAT_CONFIGURATION, SCOTAPP_MISS_BMP,"Missing screen BMP file %s",GetBmpName(nBmp));
  }
  CString NullString;	
  for(int i=0; i < (WORKMSGS_LAST - PS_MAXTEXT); ++i)
  {
	szPSText[i] = NullString;	// within loadpstext()
  }
  szPSText[PS_BLANK - PS_MAXTEXT] = _T(" ");
#endif //_CPPUNIT
  return 0;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetKeyLabel
// Purpose:			This function sets assist mode keys label. This function
//                  must be called for every ButtonList defined in the XML for 
//                  the assist mode context.
// Parameters:      [in] sControlName - name of ButtonList control
// Returns:	        0 
// Calls:
// Side Effects:
// Notes:           The assist key control names must start with Key and followed
//                  by a number that indicates the index into csKeyLabeList (array 
//                  containing key labels) and uKeyCodeList (array containing key values).
//                  This function calls AssignAssistKeyText function that handles 
//                  the assignment of key labels and values to the controls.
//
//************************************************************************
long PSProceduresBase::SetKeyLabel(LPCTSTR sControlName, int nKey, LPCTSTR szDesc)	// key label
{	
	CString sIndex, sSubStr;
    // if we just need to set text for one button in the list
    if ( -1 != nKey)

    {
        COleVariant v1, v2, vReturn;
	    v1 = (long) nKey;
        v2 = szDesc;
        m_pPSX->SendCommand(sControlName, UI::COMMANDSETBUTTONTEXT, vReturn, 2, v1, v2 );
    }
    // Otherwise set the entire control's button lists
    else
    {
	    sSubStr = _T("Key");
	    sIndex = sControlName;

	    if (sIndex.Find(sSubStr) != -1)
	    {
		    sIndex.Delete(0, sSubStr.GetLength());
		    AssignAssistKeyText(sControlName,  _ttoi(sIndex));
	    }
    }
return 0;
}

//************************************************************************ 
//
// Function:		DisableAssistModeKey
// Purpose:			This function disables the assist keys
// Parameters:      [in] nKey - key index to disable
//                  [in] sControlName - ButtonList control name
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::DisableAssistModeKey( LPCTSTR sControlName, int nKey)
{
    CString csValue;
    COleVariant v1, v2, vReturn;
    if ( -1 != nKey)
    {
	    v1 = (long) nKey;
        v2 = (long) UI::Disabled; // disable
        m_pPSX->SendCommand(sControlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
    }
    else
    {
	    m_pPSX->GetCustomDataVar(_T("ButtonCount"), csValue, sControlName, m_csCurrentContext);
        if ( csValue.IsEmpty() )
        {
       	    trace(L6,_T("AssignAssistKeyText - ButtonCount is not specified in the XML") );
            return -1;
        }
        else
        {
            long nButtonCount = _ttoi(csValue);
            v2 = (long) UI::Disabled; // disable
            for (int i = 0; i < nButtonCount; i++)
            {
        	    v1 = (long) i;
                m_pPSX->SendCommand(sControlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
            }
        }
    }
    return 0;
}
//************************************************************************ 
//
// Function:		AssignAssistKeyText

// Purpose:			This function assigns the key label and values to the 
//                  specified ButtonList control
// Parameters:      [in] szCtlName - ButtonList control name
//                  [in] nIndex - index into csKeyLabeList (array containing 
//                  key labels) and uKeyCodeList (array containing key values
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************I
long PSProceduresBase::AssignAssistKeyText(LPCTSTR szCtlName, int nIndex)
{
    USES_CONVERSION;
    long lIndex;
	CString csValue;
	COleVariant vReturn,v, v1, v2;


	m_pPSX->GetCustomDataVar(_T("ButtonCount"), csValue, szCtlName, m_csCurrentContext);
    if ( csValue.IsEmpty() )
    {
       	trace(L6,_T("AssignAssistKeyText - ButtonCount is not specified in the XML") );
        return -1;
    }
    long nButtonCount = _ttoi(csValue);
    // reset the count to 0 to clear all previously set attributes
    v = (long) 0;
    m_pPSX->SetConfigProperty( szCtlName, m_csCurrentContext, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(szCtlName, UI::PROPERTYBUTTONCOUNT, v );
    // Now populate the Button lists
    v = (long) nButtonCount;
    m_pPSX->SetConfigProperty( szCtlName, m_csCurrentContext, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(szCtlName, UI::PROPERTYBUTTONCOUNT, v );
	CString str = _T("");
	COleSafeArray saTextArray, saItemCodeArray;

	saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

	for( long i = 0; i < nButtonCount; i++ )
	{
		str = SMStateBase::csKeyLabeList[i+nIndex];

        // TAR 238397 - Need to handle /n in button text.
		str = ps.ConvertToEscapeSequence(str, _T("\\n"), _T("\n"));

		// + TAR 314786
		BSTR bstrText=str.AllocSysString();
		saTextArray.PutElement( &i, bstrText);
		::SysFreeString(bstrText);
		// - TAR 314786

        // Check for condition where buttons need to be invisible
		if (SMStateBase::csKeyLabeList[i+nIndex].IsEmpty())
		{
			v1 = (long) i;
			v2 = VARIANT_FALSE;
			m_pPSX->SendCommand(szCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
		}

        // Check for conditions where buttons need to be disabled
		if ( (SMStateBase::uKeyCodeList[i+nIndex] == 0) && (m_csCurrentContext == _T("SmAssistMode")) ||
             (!co.fTenderCashAllowed && (SMStateBase::csKeyLabeList[i+nIndex] == _T("Cash") || SMStateBase::csKeyLabeList[i+nIndex] == _T("CASH"))) )
		{
			v1 = (long) i;
			v2 = (long) UI::Disabled; // disable
			m_pPSX->SendCommand(szCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
		}
		lIndex = i + nIndex;
		v = (long) lIndex;
		saItemCodeArray.PutElement(&i, &v);

     
	}
    v = saTextArray;
    m_pPSX->SendCommand(szCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand(szCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

	return 0;
}
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		LoadTenderList
// Purpose:			This function loads and assigns text, image, nad values 
//                  to the TenderImage ButtonList control.
// Parameters:      none
// Returns:	        0 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::LoadTenderList()
{
    USES_CONVERSION;
	long nButtonCount = 0;
    CString csCtlName = _T("TenderImage");
    CString csCtxName = _T("Finish");
	TRACE(_T("LoadTenderList\n"));
    //sscoadk-6740: set the customer language if ssco is just out from store mode
    if (co.GetfStateDualLanguage()  &&
         mp.sCurr->m_customerLanguage != m_languageUsed &&
         SMStateBase::fSAInTransaction 
         && !mp.sCurr->bFromAssistMenu)          
     {
         SetLanguage(mp.sCurr->m_customerLanguage);  
         trace(L6, _T("LoadTenderList: set language to cusotmer langugae"));
     }
    //sscoadk-6740-
    // reset the count to 0 to clear all previously set attributes
    COleVariant v = (long) 0;
    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

    nButtonCount = MAXBUTTONNUM;
    v = nButtonCount;

    CPSXRedrawLock csRedrawLock( csCtlName, _T("PSProceduresBase::LoadTenderList"));

    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
    m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	CString str = _T("");
    COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);
    COleVariant vReturn, v1, v2;
	TCHAR szPath[_MAX_PATH];

    for(long  i = 0; i < nButtonCount; i++ )
	{
        v1 = i;

		BOOL bDisableButtonByTB;

        bDisableButtonByTB = tb.DisableTenderButton((TBTENDERTYPE)co.GetTenderButton(m_languageUsed, i).nTenderType);
		
		if (bDisableButtonByTB)
		{
			v2 = (long) UI::Disabled; // disable
			m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
			
			int nTenderType = co.GetTenderButton(m_languageUsed, i).nTenderType;
			//trace(L6,_T("Tender button disabled by TB, Tender Type: %d"), nTenderType); 
		}

        if (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_COUPON) 
        {
            if (!co.GetfTenderShowUseCoupons())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if ((co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_FOODSTAMPS) ||
            (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_FOODSTAMPS) )
        {
            if (!co.GetfTenderEBTFSAllowed())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
            else if (SMStateBase::lFoodStampsDue == 0)
            {
			    v2 = (long) UI::Disabled; // disable
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONSTATE, vReturn, 2, v1, v2 );
            }
        }
        if (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_OTHERPAYMENT)
        {
            if (!co.GetfTenderOtherPayment())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_CASH)
        {
            if (!co.GetfTenderCashAllowed())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if ((co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_CREDIT) ||
            (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_CREDIT) )
        {
            if (!co.GetfTenderCreditAllowed())
            {

                v2 = VARIANT_FALSE;

			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if 	((co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_DEBIT) ||
			 (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBIT) ) 
        {
            if (!co.GetfTenderDebitAllowed())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if	((co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_EBT) ||
			 (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_EBT)) 
        {
            if (!co.GetfTenderEBTCBAllowed())
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK) 
        {
            if (! co.GetfTenderDebitAllowed()) 
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
		if ((co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_GIFTCARD) ||
			(co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_GIFTCARD))
        {
            if (co.GetnOperationsGiftCardRedemption() == 0)
            {
                v2 = VARIANT_FALSE;
			    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
            }
        }
        if (co.GetTenderButton(m_languageUsed, i).nTenderType == 0) 
        {
            v2 = VARIANT_FALSE;
		    m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );
        }

		//SR742+
		if (co.IsDegradedModeOn() )
		{
			m_lModeType = dm.GetDegradedModeType();

			switch(m_lModeType)
			{
				case DEGRADEDMODE_CARD_ONLY:
					{
						if ( (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_CASH) || 
							  co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK || //432511
						   ( (co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_CREDIT  || 
						      co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_DEBIT ||
                              co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_DEBITSAVING ||
                              co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_EBT) && 
							  co.GetTenderButton(m_languageUsed, i).bCashBack) )
						{
							v2 = VARIANT_FALSE;
							m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );		
						}		

					}break;
				case DEGRADEDMODE_CASH_ONLY:
					{
						if ( co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_CREDIT || 
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_DEBIT  ||
                             co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_DEBITSAVING ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_EBT    || 
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_FOODSTAMPS ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_GENERIC ||  //432117
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_CREDIT ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBIT  ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBIT_CASHBACK ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_LOYALTY ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBITSAVING ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_DEBITCHECKING ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_EBT ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_FOODSTAMPS ||
							 co.GetTenderButton(m_languageUsed, i).nTenderType == TB_TENDER_PIP_GIFTCARD )
						{
							v2 = VARIANT_FALSE;
							m_pPSX->SendCommand(csCtlName, UI::COMMANDSETBUTTONVISIBLE, vReturn, 2, v1, v2 );	
						}

					}break;
                case DEGRADEDMODE_OFF:
                    break;
				default:
					{
						trace(L6, _T("PSProceduresBase: %d is invalid degraded mode type."), m_lModeType);	
					}
			}
		}
		//SR742-

		str = co.GetTenderButton(m_languageUsed, i).csTenderText;
		// +TAR 314786
		BSTR bstrText = str.AllocSysString();
		saTextArray.PutElement(&i, bstrText );
		::SysFreeString(bstrText);
		// -TAR 314786
		CString csImagePath;
		GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));

	    _tmakepath(szPath,NULL,csImagePath,co.GetTenderButton(m_languageUsed, i).csTenderBitmap, NULL);
		csImagePath.ReleaseBuffer();
		str = szPath;
		// +TAR 314786
		BSTR bstrImage = str.AllocSysString();
		saImageArray.PutElement(&i, bstrImage );
		::SysFreeString(bstrImage);
		// -TAR 314786

        CString csTenderType;
        int nTenderType = co.GetTenderButton(m_languageUsed, i).nTenderType;

        if(nTenderType == TB_TENDER_DEBIT && 
            co.GetTenderButton(m_languageUsed, i).bCashBack)
        {
            csTenderType.Format(_T("%dcb"), nTenderType);
        }
        else
        {
           csTenderType.Format(_T("%d"), nTenderType);
        }

        // ++ NXTUI-73 SystemFunctions buttons
        //v = (long) co.TenderList[m_languageUsed][i].nButtonNum;
        str.Format(_T("%d;%s"), co.GetTenderButton(m_languageUsed, i).nButtonNum, csTenderType);
        v = str;
        // -- NXTUI-73

        saItemCodeArray.PutElement(&i, &v );  

    }
    v = saTextArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saImageArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );

    v = saItemCodeArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

    return 0;
}
//************************************************************************ 
//
// Function:		LoadLanguageList
// Purpose:			This function loads and assigns text, images and values 
//                  to the LanguageImage ButtonList control.
// Parameters:      none
// Returns:	        0 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::LoadLanguageList(void)
{
    USES_CONVERSION;
	long nButtonCount = 0;
    CString csCtlName = _T("LanguageImage");
    CString csCtxName = _T("SelectLanguage");
	TRACE(_T("LoadLanguageList\n"));
    // reset the count to 0 to clear all previously set attributes
    COleVariant v = (long) 0;
    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty(csCtlName, UI::PROPERTYBUTTONCOUNT, v );

    nButtonCount = co.GetNumberOfLanguagesSupported();
    v = nButtonCount;

    CPSXRedrawLock csRedrawLock(csCtlName, _T("PSProceduresBase::LoadLanguageList"));

    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
    m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

	CString str = _T("");
    COleSafeArray  saTextArray, saImageArray, saItemCodeArray;
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);
    COleVariant vReturn, v1;
	TCHAR szPath[_MAX_PATH];
	
    for(long  i = 0; i < nButtonCount; i++ )
	{
        // ++ NXTUI
        //v1 = i;
        str.Format(_T("%d;%s"), i, GetMSLocaleID(co.Getlanguage(i).szCode));
        v1 = str;
        // -- NXTUI

		str = co.Getlanguage(i).csName;
		if(_istdigit(*str))
		{
			int iMessageID=_ttoi(co.Getlanguage(i).csName);
			str = ps.GetPSText(iMessageID, (SCOT_LANGUAGE_TYPE)i);
		}
		// +TAR 314786
		BSTR bstrText = str.AllocSysString();
		saTextArray.PutElement(&i, bstrText );
		::SysFreeString(bstrText);
		// -TAR 314786
		CString csImagePath;
		GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));

	    _tmakepath(szPath,NULL,csImagePath,co.Getlanguage(i).csButtonBitmap, NULL);
		csImagePath.ReleaseBuffer();
		str = szPath;
		// +TAR 314786
		BSTR bstrImage = str.AllocSysString();
		saImageArray.PutElement(&i, bstrImage );
		::SysFreeString(bstrImage);
		// -TAR 314786

      saItemCodeArray.PutElement(&i, &v1 );  

    }
    v = saTextArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
   v = saImageArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

    return 0;
}


//************************************************************************ 
//
// Function:		LoadProdList
// Purpose:			This function load produce list from the pick list
//                  XML file
// Parameters:  
// Returns:	        0
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::LoadProdList(const CString& csXML_List)
{

	trace(L6, _T("+PSProceduresBase::LoadProdList"));

	m_bFoundPickListItemsFromSearch=false;

   // Reset this to _T("") since we are reloading list //
   m_strLastProduceLookupList = _T("");
	m_lTotalPicklistItemsSold = 0;
   m_lTotalPicklistItemsIncludeVoidItems = 0;//SR679
    //////////////////////////////////////////////////////

    CString csLangCode[SCOT_LANGUAGE_MAX];
    int i;
	if (co.GetfStateDualLanguage())
	{
        for (i = 0; i<co.GetNumberOfLanguagesSupported(); i++)
        {
    		ps.ConvertSCOTLangID2LCID(co.Getlanguage(i).szCode, csLangCode[i]);	
        }
	}
	else
	{
		csLangCode[0] = co.csPrimaryLanguageCode;
	}

	CString csXMLFileName;
#ifndef _CPPUNIT

	if (csXML_List.GetLength() == 0)
  	            csXMLFileName = co.csConfigFilePath + _T("\\") + XML_PICKLISTFILE;
	else
            	csXMLFileName = co.csConfigFilePath + _T("\\") + csXML_List;
#else 
    csXMLFileName = csXML_List;
#endif


	//tar 286744 -- In the "real world", errors can happen, so check for them.

    MSXML2::IXMLDOMDocumentPtr spDoc;
    HRESULT hr;
    hr = spDoc.CreateInstance(__uuidof( MSXML2::DOMDocument60) );
    if(FAILED(hr))
    {
		trace(L1, _T("ERROR: Failed to create XML DOM Object (for PickList). IS MS XML INSTALLED??"));
        return FALSE;
    }

    spDoc->async = VARIANT_FALSE; // need to use sync mode if accessing DOM immediately after Load
    	
	//Load PickList XML config file
	if( spDoc->load( (LPCTSTR)csXMLFileName ) != VARIANT_TRUE )
	{
		trace(L1, _T("ERROR: Load PickList xml file failed"));
        spDoc.Release();
		return FALSE;
	}

    trace(L6, _T("PickList xml file load completed successfully"));

	//Empty lists
	m_csPickQtyList.RemoveAll();
	m_mZeroWeightItems.RemoveAll(); //Japan RFC#1 (RFQ 893)
    m_mPickListItemsMap.RemoveAll();
    m_mPickListItemsMap.InitHashTable(co.GetPickListHashTableSize());
    bool bSortedList = co.GetfOperationsPicklistSortedAlphabetical();
    for (i=0; i < co.GetNumberOfLanguagesSupported(); i++)
    {
        ItemList[i].EmptyList();
        m_Search[i].EmptyList();
		QuickPickItemList[i].EmptyList();
        m_StorePopularItemList[i].EmptyList();//Personalization picklist
        CategoryList[i].EmptyCategoryList();
		SubCategoryList[i].EmptyCategoryList();
		MainCategoryList[i].EmptyCategoryList();

        CategoryList[i].BuildCategoryList(spDoc, csLangCode[i], bSortedList);
		//SubCategoryList[i].BuildSubCategoryList(spDoc, csLangCode[i]);

		//Build category and subcategory list
		POSITION pos = CategoryList[i].GetHeadPosition();
		CCategoryInfo* pItem = NULL;
		while( pos != NULL )
		{
			pItem = (CCategoryInfo*)CategoryList[i].GetNext( pos );
			CString csCatId = pItem->GetCategoryID();
			CString csCatDesc = pItem->GetCatDescription();
			CString csCategories = pItem->GetCategories();
			CString csAlphabetic = pItem->GetCatAlphabetic();			
			CString csGraphic = pItem->GetCatGraphic();
			CString csScreenTitle = pItem->GetScreenTitle();
			CString csInstructions = pItem->GetInstructions();
			bool bQuickPickItem = pItem->GetQuickPickItem();
			bool bIsItem = false;
			bool bFavorite = false;
			if ( csCategories.Find(TAB_FAV) != -1 )
			{
				bFavorite = true;
			}

            bool bSubCategory = false;

            bool bSubcatItem = false;
            if(!bSortedList && csInstructions.Find(SUBCATEGORY_ITEM)!= -1)
            {
                bSubcatItem = true;
            }

			if ( _ttoi(csCatId) > MAXCATEGORIES)
			{
				bSubCategory = true;
			}

		    if ( !csCategories.IsEmpty() && bSubCategory )
			{
				SubCategoryList[i].InsertInOrder( csCatId, csCatDesc, csCategories, csAlphabetic, 
                            csGraphic, csScreenTitle, csInstructions, bQuickPickItem, bSortedList, bSubcatItem);					
				//Add subcategories to the QuickPickItemList		
				if (co.fOperationsDisplayQuickPickItems && bQuickPickItem)
				{
					QuickPickItemList[i].InsertInOrder( csCatDesc, csCatId, csGraphic, bFavorite, 
                        bSubCategory, csCategories, _T(""), _T(""), bQuickPickItem, bIsItem, csAlphabetic, bSortedList );	
				}
                if (bFavorite)//Personalization picklist
                {
                    m_StorePopularItemList[i].InsertInOrder( csCatDesc, csCatId, csGraphic, bFavorite, bSubCategory, 
                        csCategories, _T(""), _T(""), bQuickPickItem, bIsItem, csAlphabetic, bSortedList );
                }
			}
			else if (!bSubCategory)
			{
				//Don't add Favorites category to the list if no items in it
			//	if ( (csCatId != TAB_FAV) ||( (csCatId == TAB_FAV) && ItemsInFavoritesList()))
                //Personalization picklist: the above case will be handle in SMProduceFavoritesBase.cpp
				{
					MainCategoryList[i].InsertInOrder( csCatId, csCatDesc, csCategories, csAlphabetic, 
                                    csGraphic, csScreenTitle, csInstructions, bQuickPickItem, bSortedList);	
					//Add categories to the QuickPickItemList		
					if (co.fOperationsDisplayQuickPickItems && bQuickPickItem)
					{
						QuickPickItemList[i].InsertInOrder( csCatDesc, csCatId, csGraphic, bFavorite, bSubCategory, 
                                        csCategories, _T(""), _T(""), bQuickPickItem, bIsItem, csAlphabetic, bSortedList);	
					}
				}				
			}
			trace(L7, _T("Category/Subcategory: Id=%s, Desc=%s, Categories=%s"), csCatId, csCatDesc, csCategories);
		}
		
		//Add subcategories to the ItemList		
		pos = SubCategoryList[i].GetHeadPosition();
		pItem = NULL;
		while( pos != NULL )
		{
			pItem = (CCategoryInfo*)SubCategoryList[i].GetNext( pos );
			CString csCategories = pItem->GetCategories();
			CString csCatDesc = pItem->GetCatDescription();
			CString csCatId = pItem->GetCategoryID();
			CString csGraphic = pItem->GetCatGraphic();
			bool bQuickPickItem = pItem->GetQuickPickItem();
            bool bSubcatItem = pItem->GetSubcatItem();
			bool bSubCategory = true;
			bool bIsItem = false;

			bool bFavorite = false;
			if ( csCategories.Find(TAB_FAV) != -1 )
			{
				bFavorite = true;
			}

            //only insert to item if its not a subcategory added to item
            if(!bSubcatItem)
            {
			    trace(L7, _T("SubCategory Item: Id=%s, Desc=%s, Graphic=%s"), csCatId, csCatDesc, csGraphic);
			    ItemList[i].InsertInOrder( csCatDesc, csCatId, csGraphic, 
                                               bFavorite, bSubCategory, csCategories, 
                                               _T(""), _T(""), bQuickPickItem, bIsItem, 
                                               _T(""), bSortedList); //main list will follow the option sorting
            }
            if(!bSortedList)
            {
                m_Search[i].InsertInOrder( csCatDesc, csCatId, csGraphic, 
                                        bFavorite, bSubCategory, csCategories, 
                                        _T(""), _T(""), bQuickPickItem, bIsItem, 
                                        _T(""),true); //search will always be alphabetically sorted
            }
		}

            //build item after category/subcategory so subcategory will display before the item in unsorted list
            if (i == 0)
            {
                ItemList[i].BuildList(spDoc, csLangCode[i], &m_mPickListItemsMap, &m_csPickQtyList, &m_mZeroWeightItems, bSortedList);
                if(!bSortedList)
                {
                    //search will always be alphabetically sorted
                    m_Search[i].BuildList(spDoc, csLangCode[i], &m_mPickListItemsMap, &m_csPickQtyList, &m_mZeroWeightItems, true);
                }
            } 
            else
            {
		        ItemList[i].BuildList(spDoc, csLangCode[i], NULL, NULL, NULL, bSortedList);
                if(!bSortedList)
                {
                    m_Search[i].BuildList(spDoc, csLangCode[i], NULL, NULL, NULL, true);
                }
            }

			//Add items to the QuickPickItemList		
			POSITION pos1 = ItemList[i].GetHeadPosition();
			CItemInfo* pItem2 = NULL;
			while( pos1 != NULL )
			{	
				pItem2 = (CItemInfo*)ItemList[i].GetNext( pos1 );

				CString csDesc = pItem2->GetItemName();
				CString csCode = pItem2->GetItemCode();
				CString csGraphic = pItem2->GetItemGraphic();
				bool bFavorite = pItem2->GetIsFavorite();
				bool bQuickPickItem = pItem2->GetQuickPickItem();
				bool bSubCategory = false;

				if ((bQuickPickItem) && (co.fOperationsDisplayQuickPickItems))
				{
                    trace(L7, _T("Quick Item: Code=%s, Desc=%s, subcat=%d, Favorite=%d "), 
                            csCode, csDesc, pItem2->GetIsSubCategory(), bFavorite);
                    if(bSortedList || (!bSortedList && !pItem2->GetIsSubCategory()))
                    {
                        QuickPickItemList[i].InsertInOrder( csDesc, csCode, csGraphic, bFavorite, 
                            bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem, true, _T(""), bSortedList);
                    }
				}
                //Personalization picklist: Add items to m_StorePopularItemList
                if (bFavorite)
				{
					trace(L7, _T("Store Popular Item: Code=%s, Desc=%s, Graphic=%s, Favorite=%d"), csCode, csDesc, csGraphic, bFavorite);
					m_StorePopularItemList[i].InsertInOrder( csDesc, csCode, csGraphic, bFavorite, 
                                bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem, true, _T(""), bSortedList);
				}
			}
    }

   spDoc.Release();

   trace(L6, _T("-PSProceduresBase::LoadProdList"));
 

   return 0;
}
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		GetNewDescriptions
// Purpose:			This function for pick list items adds an entry for the item's description and reading name.
// Parameters:          [in] strOriginalDescription - string containing the original desc
//						[in] strOriginalReadingName - string containing the original reading name	
//                      [in] strNewDescriptsArray - string array containing the new descs
//						[in] strNewReadingNamesArray - string array containing the new reading names
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::GetNewDescriptions( const CString& strOriginalDescription, const CString& strOriginalReadingName, CStringArray& strNewDescriptsArray, CStringArray& strNewReadingNamesArray )
{
	strNewDescriptsArray.RemoveAll();
	strNewReadingNamesArray.RemoveAll();
	
	// just return the originals //
	strNewDescriptsArray.Add( strOriginalDescription );
	strNewReadingNamesArray.Add( strOriginalReadingName );
}

//************************************************************************ 
//
// Function:		ReadPImgFiles
// Purpose:			This function reads the pick list images form the pick list
//                  image file.
// Parameters:      [out] pcsaCode - array of pick list item item code
//                  [out] pcsaBmpName - array of pick list item image
//                  [out] pcsaOverrideDesc - array of pick list item override desc
// Returns:	        none
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ReadPImgFiles(CStringArray *pcsaCode,CStringArray *pcsaBmpName,CStringArray *pcsaOverrideDesc)
{
  //This function is obsolete from release 3.2
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		EnableProduceScrollButtons
//
//************************************************************************
void PSProceduresBase::EnableProduceScrollButtons()
{
	//This function is commented out since the scrolling of picklist item buttons will have no dead end with the new hierarchical picklist feature -
    //the scroll buttons on Produce Favorites will be always enabled.
	/*CString csProduceContextName = co.GetContextPrefix() + s_szProduceContextName;
	VARIANT_BOOL vbUpVisible = VARIANT_FALSE, vbDownVisible = VARIANT_FALSE, vbCanScrollUp = VARIANT_FALSE, vbCanScrollDown = VARIANT_FALSE;
	UI::State stateUp = UI::Normal, stateDown = UI::Normal;

	COleVariant v, vReturn;
    v = (long)UI::PageUp;
    if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
	{
		vbCanScrollUp = vReturn.boolVal;		
	}	
	v = (long)UI::PageDown;
	if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
	{
		vbCanScrollDown = vReturn.boolVal;		
	}
    
	if( VARIANT_TRUE == vbCanScrollUp )
	{
		vbUpVisible = VARIANT_TRUE;
		vbDownVisible = VARIANT_TRUE;

		if( VARIANT_FALSE == vbCanScrollDown )
		{
			stateDown = UI::Disabled;
		}
	}
	else
	{
		stateUp = UI::Disabled;
		
		if( VARIANT_TRUE == vbCanScrollDown )
		{
			vbUpVisible = VARIANT_TRUE;
			vbDownVisible = VARIANT_TRUE;

		}		
	}
		
	if( VARIANT_FALSE == vbUpVisible )
	{
		v = VARIANT_FALSE;
		m_pPSX->SetConfigProperty( s_szProduceScrollUpName, csProduceContextName, UI::PROPERTYVISIBLE, v );
	}
	if( VARIANT_FALSE == vbDownVisible )
	{
		v = VARIANT_FALSE;
		m_pPSX->SetConfigProperty( s_szProduceScrollDownName, csProduceContextName, UI::PROPERTYVISIBLE, v );
	}
			
	v = VARIANT_FALSE;
	m_pPSX->SetConfigProperty( s_szProduceScrollDownName, csProduceContextName, UI::PROPERTYFLASH, v );
	v = (long)stateUp;
	m_pPSX->SetConfigProperty( s_szProduceScrollUpName, csProduceContextName, UI::PROPERTYSTATE, v );
    v = (long)stateDown;
	m_pPSX->SetConfigProperty( s_szProduceScrollDownName, csProduceContextName, UI::PROPERTYSTATE, v );
	
	if( VARIANT_TRUE == vbUpVisible )
	{
		v = VARIANT_TRUE;
		m_pPSX->SetConfigProperty( s_szProduceScrollUpName, csProduceContextName, UI::PROPERTYVISIBLE, v );
	}
	if( VARIANT_TRUE == vbDownVisible )
	{
		if( m_bFirstTimeNextButtonEnabled && UI::Normal == stateDown )
		{
			m_bFirstTimeNextButtonEnabled = FALSE;
			v = VARIANT_TRUE;
			m_pPSX->SetConfigProperty( s_szProduceScrollDownName, csProduceContextName, UI::PROPERTYFLASH, v );
			
			if( co.fStateSayPhrases )
			{
				dm.SayPhrase( GetSoundName( TOUCHNEXTPRODUCE ) );
			}
		}
		
		v = VARIANT_TRUE;
		m_pPSX->SetConfigProperty( s_szProduceScrollDownName, csProduceContextName, UI::PROPERTYVISIBLE, v );		
	}*/
}
//************************************************************************ 
//
// Function:		DisplaySelectedItem
// Purpose:			This function assigns the text, image, and values for 
//                  ProductImage ButtonList control.
// Parameters:      [in] csFirstchars - contains the first characters of 
//                                      selected items. This value only used to 
//                                      display the ProductImage.
//										May also be Favorites or CategoryN where N is a
//										number from 1-8.
//                  [in] bSelectByItemDescription - to select item by item description or not
// Returns:	        The number of selected items
// Calls:
// Side Effects:
// Notes:           The characters in csFirstChars are defined as a range such 
//                  as AB1(would show all the produce buttons starting with the 
//                  letter 'A' and 'B') in the XML file. A numeric value from 1 to 9
//                  tab range identifies what tab it is (i.e 1st, 2nd). This number is
//                  used to display a produce item whose text doesn't start with a letter
//                  (i.e. 4x8x16 Gray Solid Cap Block ). For 3.1 implementation,
//                  these items are displayed when 1st tab is selected.
//                  For Favorites in the XML file the tab's value must be specified
//                  as Favorites.
//					For Categories, csFirstChars must be CategoryN where N is a number from 1-8.
//
//************************************************************************
int PSProceduresBase::DisplaySelectedItem(CString csFirstchars, bool bSelectByItemDescription, bool bFavoritesButton)
{
#ifndef _CPPUNIT // TODO: make UT for AssignTareList()
	if( m_csCurrentContext == _T("SelectTare") )
	{
        AssignTareList();		
		  return 0;
	}

    // if m_csCurrentContext is not *LookUpItemXXXX or *ProduceFavoritesXXXX context
    if (!((m_csCurrentContext.Find(_T("LookUpItem")) != -1) || (m_csCurrentContext.Find(_T("ProduceFavorites")) != -1)))
    {
        trace(L6, _T("PSProceduresBase::DisplaySelectedItem do not handle DisplaySelectedItem() on %s context"), m_csCurrentContext);
        return 0;
    }

#endif // _CPPUNIT
	COleVariant v;
   m_strLastProduceLookupList = csFirstchars;

   bool bSortedList = co.GetfOperationsPicklistSortedAlphabetical();
   CSItemList *pItemList = NULL;
   if(!bSortedList && bSelectByItemDescription)
   {
       pItemList = &(m_Search[m_languageUsed]);
   }
   else
   {
       pItemList = &(ItemList[m_languageUsed]);
   }

//+ Personalization picklist
   CString csPickListPref = custInfo.GetPreference(_T("PicklistFavorites"));
   CSItemList *pPopularItemList;


   if(co.IsPicklistAssistEnable() && !m_PicklistAssistItemList[m_languageUsed].IsEmpty() && !bFavoritesButton)
   {
       pPopularItemList = &(m_PicklistAssistItemList[m_languageUsed]);
   }
   else if ( csPickListPref.GetLength() && !m_PersonalizedPopularItemList[m_languageUsed].IsEmpty() 
	         && (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0))
   {
       pPopularItemList = &(m_PersonalizedPopularItemList[m_languageUsed]);
   }
   else
   {
        pPopularItemList = &(m_StorePopularItemList[m_languageUsed]);
   }
//- Personalization picklist
   
    DWORD nButtonCount = pItemList->GetCount();
	if( !nButtonCount )
	{
		v = 0L;
		m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );
		//EnableProduceScrollButtons();
		return 0;
	}

//#ifndef _CPPUNIT // TODO: extract this code to another method.
	CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplaySelectedItem"));  //TAR 371980 fix

	CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CStringArray csCatDescList;
	CArray<ITEMTYPE,ITEMTYPE> nItemTypeList;
	
	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csCombineItemInfo = _T("");
	CString csCat(_T(""));
  

    int nTabIndex(-1);
    CString csAlphabetic, csPCategory;
    bool bFavoriteOn = FindCategory(TAB_FAV, csAlphabetic, csPCategory, nTabIndex);
	if(bSelectByItemDescription)
	{
		//Set locale before comparing strings
#ifndef _UNICODE
      char* localeBackup=strdup(setlocale(LC_CTYPE, NULL));
#else
      wchar_t* localeBackup=wcsdup(_wsetlocale(LC_CTYPE, NULL));
#endif
      SetLocale(GetLanguage());
      
      csFirstchars.MakeLower();
      trace(L6, _T("Searching picklist by item description for string %s"), csFirstchars);

      m_csScreenTitle=GetPSText(MSG_SEARCHFROMLISTTITLE); 
      if(!co.GetfOperationsAllowMultiPick())
	  {
		m_csInstructions=GetPSText(MSG_SEARCHFROMLISTINSTRUCTION); 
	  }
	  else
	  {
		m_csInstructions=GetPSText(MSG_SEARCHFROMLISTINSTRUCTION2); 
	  }

      CSortedArray<CString,CString> arrItemCode;
      pos = pItemList->GetHeadPosition();

	  /******************************************************/
	  //check to see if Search string has special characters
	  //check it here instead in the while loops below.
	  bool searchStringHasSpecialChar =  false;

	  for (int i = 0; i < csFirstchars.GetLength(); i++)
	  {
		  if (csFirstchars.GetAt(i) > 192 )
		  {
			  searchStringHasSpecialChar = true;
		  }
		  //trace(L6, _T("searchString[%d] =  %d , Length = %d"), i, searchString.GetAt(i), searchString.GetLength());
	  }
	  /******************************************************/


	  while( pos != NULL )
      {
         pItem = (CItemInfo*)pItemList->GetNext( pos );

         if (pItem->m_bIsSubCategory ||  
			(arrItemCode.GetSize()>0 && (arrItemCode.FindBinary(pItem->GetItemCode(), 0, arrItemCode.GetSize()-1)==TRUE)) )
            continue; 

         CString strName=pItem->GetItemName();

	
		 if(strName != _T(""))
         {
			 strName.MakeLower();

			 /******************************************************************/
			 // SSCOADK-2496 - determines if Search string as "space" char*
			 //If the search string has space but does not have special char. 
			 //then check if the search string is part of Item Name (strName). 
			 if (csFirstchars.Find( _T(' ')) > 0 && !searchStringHasSpecialChar)
			 {
				 if (strName.Find(csFirstchars) > -1)
				 {
					 ps.GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
						 csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );
					 
					 nButtonCount++; 
					 
					 arrItemCode.InsertBinary(pItem->GetItemCode(), FALSE);

				 }
				 //continue to next element in picklist.xml
				 continue;
			 }
			 /***************************************************************************/

			 TCHAR strTemp [255];
			 _tcscpy(strTemp, strName);

			 TCHAR * strSeps = _T(" ");
			 TCHAR * strToken = _tcstok(strTemp, strSeps);

			 while (strToken != NULL)
			 {
				 CString csToken = strToken;
				 
				 //SSCOADK-2496
				 //Only continue if search string does not have "space" char.
				 if ( csFirstchars.Find(_T(" ")) < 0)
				 {
					 if( (csToken.GetLength() < csFirstchars.GetLength()))
					 {
						 strToken =  _tcstok(NULL, strSeps);
						 continue;
					 }
				 }
				 
				 csToken = csToken.Left(csFirstchars.GetLength() ); 

				 bool bIsMatch=true;
				 if(co.GetfOperationsAllowFuzzySearch())
				 {
					 for(int i=0; i<csToken.GetLength(); i++)
					 {
						 CString csFuzzyChars;
						 GetFuzzyChars(csFirstchars.GetAt(i), csFuzzyChars);

						 //trace(L6, _T("Description is %s, Token is %s, char is %c, FuzzyChars is %s"), strName, csToken, csToken.GetAt(i), csFuzzyChars);
						 if(csFuzzyChars.Find(csToken.GetAt(i))<0)
						 {
							 bIsMatch=false;
							 break; //breaks for-loop
						 }
					 }					 
				 }
				 else
				 {
					 if(csToken.Left(csFirstchars.GetLength()).Compare(csFirstchars)!=0)
						 bIsMatch=false;
				 }
				 
				 if(bIsMatch)
				 {                  
					 ps.GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
						 csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );
					 
					 nButtonCount++; 
					 
					 arrItemCode.InsertBinary(pItem->GetItemCode(), FALSE);

					 break; //breaks while loop
				 }
				 strToken =  _tcstok(NULL, strSeps);
			 }//while         
		 }//if
      }//while
      
      arrItemCode.RemoveAll();

      //Set locale after comparing strings
#ifndef _UNICODE
      setlocale(LC_CTYPE, localeBackup);
#else
      _wsetlocale(LC_CTYPE, localeBackup);
#endif
      free(localeBackup);

   }
   else
   {
	CString csCategories(_T(""));
	CString csCategory(_T(""));
    if( csFirstchars == TAB_FAV )
	{
        SetCategoryTitles(csFirstchars);
        CString csCatAlphabetic;
        bool bFind = FindCategory(csFirstchars, csCatAlphabetic, csCategory, nTabIndex);
        pos = pPopularItemList->GetHeadPosition();
		while( pos != NULL )
		{
			pItem = (CItemInfo*)pPopularItemList->GetNext( pos );

			ps.GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
				                 csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csFirstchars );
			nButtonCount++;		
		}
	}
	else if( csFirstchars.Left( _tcslen( TAB_CATEGORY ) ) == TAB_CATEGORY )
	{
		csCategory = csFirstchars.Right( 1 );
		SetCategoryTitles(csCategory);
        CString csReturnCategory(_T(""));
        CString csCategoryID = _T("");
        nTabIndex = -1;
        bool bFind = FindCategory(csCategory, csReturnCategory, csCategoryID, nTabIndex);
		pos = pItemList->GetHeadPosition();
		while( pos != NULL )
		{
			pItem = (CItemInfo*)pItemList->GetNext( pos );

			csCat = _T("");
			csCategories = pItem->GetCategories();
			if ( !csCategories.IsEmpty() )
			{	
				csCat = GetCategory(csCategories, csCategory);
			}

			if( csCat == csCategory )
			{
				ps.GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					                 csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );					
				nButtonCount++;		
			}
		}
	}
	else 
	{
		// Alphabetic category //
		CString csCatAlphabetic;
		SetCategoryTitles(csFirstchars);
		bool bFind = FindCategory(csFirstchars, csCatAlphabetic, csCategory, nTabIndex);

        int n = csFirstchars.Find( _T('#') );
        BOOL bIncludeNumeric = ( -1 != n );
        if( n > -1 )
        {
            csFirstchars = csFirstchars.Left( n );
        }
        
        TCHAR cFirst = 0, cLast = 0;
        csFirstchars.MakeUpper();		
        if( csFirstchars.GetLength() )
        {
            cFirst = csFirstchars.GetAt( 0 );
            cLast = csFirstchars.GetLength() >= 2 ? csFirstchars.GetAt( 1 ) : csFirstchars.GetAt( 0 );
        }        

		CString strName( _T("") );
		pos = pItemList->GetHeadPosition();

		while( pos != NULL )
		{
			pItem = (CItemInfo*)pItemList->GetNext( pos );

			strName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
			strName.MakeUpper();

            if(strName.GetLength()) //tar 259310
			    if( ( bIncludeNumeric && _istdigit( strName.GetAt( 0 ) ) ) ||
				    ( _istalpha( strName.GetAt( 0 ) ) && ( strName.GetAt( 0 ) >= cFirst && strName.GetAt( 0 ) <= cLast ) ) )
			    {
					csCat = _T("");
					csCategories = pItem->GetCategories();
					if ( !csCategories.IsEmpty() )
					{	
						csCat = GetCategory(csCategories, csCategory);
					}
					if ( ( (pItem->GetSubCategories()).IsEmpty() || !csCat.IsEmpty() ) &&  //skip items belong to subcategory and <Categories> not explicitly configured
						!(pItem->GetIsSubCategory() && csCat.IsEmpty()) ) //skip subcategories not belong to the category
					{
						ps.GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					                         csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );						
						nButtonCount++;		
					}
					else
						trace(L7,_T("Skip subcategories and items belong to subcategories."));
			    }
		}				
	}

    if(csCategory == TAB_FAV)
    {
        m_ProduceLookupCategoryNumber = csCategory;
    }
    else
    {
        nTabIndex = nTabIndex + 1;
        m_ProduceLookupCategoryNumber.Format(_T("%d"),nTabIndex);
    }

    CString csLevels;
    CString csBaseLevel = GetCategoryName(csCategory);
    csLevels.Format(_T("TabSelected=%s;CurrentCategory=%s;Search=;"), m_ProduceLookupCategoryNumber, csBaseLevel);
    CLaneStateData laneStateData(m_pPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUIPicklistDisplayLevels"), csLevels);
   }

	
	v = (long)nButtonCount;

	CPSXRedrawLock csRedrawLock( s_szProduceButtonListName, _T("PSProceduresBase::DisplaySelectedItem"));   
    m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );

	ps.SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList, 
							 csImageList, csItemDataList, csCatDescList, nItemTypeList,
							 s_szProduceButtonListName, nButtonCount);

    //EnableProduceScrollButtons();	

//#endif // _CPPUNIT
   return nButtonCount;

}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		AssignTareList
// Purpose:			This function assigns the text, image, and values for 
//                  SelectTare ButtonList control.
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
long PSProceduresBase::AssignTareList()
{
    USES_CONVERSION;
    CString csCtlName = _T("TareList");
    CString csCtxName = _T("SelectTare");
    CString csCtlScrollUp = _T("CMTareUp");
    CString csCtlScrollDown = _T("CMTareDown");
	long nButtonCount;
    COleVariant v;

    CPSXRedrawLock csRedrawLock( csCtlName, _T("PSProceduresBase::AssignTareList"));
	nButtonCount = nTareCount[m_languageUsed];
	v = nButtonCount;
	m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
	m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

    CString str = _T("");
    COleSafeArray saTextArray, saImageArray, saItemCodeArray;
	TCHAR szPath[_MAX_PATH];
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saImageArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);
	for( long i = 0; i < nButtonCount; i++ )
	{
		str = csPSTare[m_languageUsed][i];
		// +TAR 314786
		BSTR bstrText = str.AllocSysString();
		saTextArray.PutElement(&i, bstrText );
		::SysFreeString(bstrText);
		// -TAR 314786
		CString csImagePath;
		GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));

		_tmakepath(szPath,NULL,csImagePath,csPSTareImage[m_languageUsed][i], NULL);
		csImagePath.ReleaseBuffer();
        str = szPath;
		// +TAR 314786
		BSTR bstrImage = str.AllocSysString();
		saImageArray.PutElement(&i, bstrImage );
		::SysFreeString(bstrImage);
		// -TAR 314786
	    v = lPSTareCode[m_languageUsed][i];
		saItemCodeArray.PutElement(&i, &v );
	}
	COleVariant vReturn;
    v = saTextArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saImageArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONLOGOLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

    // Disable the up/down arrows
    // Check if we can scroll down
    COleVariant vReturn1;
    vReturn = VARIANT_FALSE;
    v = (long)UI::PageDown;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDCANSCROLL, vReturn, 1, v );
    vReturn1 = VARIANT_FALSE;
    v = (long)UI::PageUp;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDCANSCROLL, vReturn1, 1, v );
    // If can't scroll up and down, then make the buttons invisible
    if( vReturn.boolVal == VARIANT_FALSE && vReturn1.boolVal == VARIANT_FALSE )
    {
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    // if can scroll up and down, then make the buttons visible and enabled
    else if( vReturn.boolVal == VARIANT_TRUE && vReturn1.boolVal == VARIANT_TRUE )
    {
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        v = (long) UI::Normal;
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYSTATE, v);
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYSTATE, v);
    }
    // if can scroll down, but not up, then disable up and enable down
    else if( vReturn.boolVal == VARIANT_TRUE && vReturn1.boolVal == VARIANT_FALSE )
    {
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        v = (long) UI::Disabled;

        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYSTATE, v);
        v = (long) UI::Normal;
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYSTATE, v);
    }
    // if can scroll up, but not down, then disable down and enable up
    else if( vReturn.boolVal == VARIANT_FALSE && vReturn1.boolVal == VARIANT_TRUE )
    {
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName, UI::PROPERTYVISIBLE, VARIANT_TRUE);
        v = (long) UI::Normal;
        m_pPSX->SetConfigProperty(csCtlScrollUp, csCtxName, UI::PROPERTYSTATE, v);
        v = (long) UI::Disabled;
        m_pPSX->SetConfigProperty(csCtlScrollDown, csCtxName,UI::PROPERTYSTATE, v);
    }
    return 0;
}
//************************************************************************ 
//
// Function:		AssignAMTareList
// Purpose:			This function assigns the text,and values for 
//                  AMTareList Receipt control.
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
long PSProceduresBase::AssignAMTareList()
{
    
	long nButtonCount;
	nButtonCount = nTareCount[m_languageUsed];

	for( long i = 0; i < nButtonCount; i++ )
	{
		
		COleVariant vValue;
		CString csItemEntryId;
		csItemEntryId.Format(_T("%d"), m_lAMTareListEntryId++);
		m_pPSX->CreateReceiptItem(_T("AMTareList"), csItemEntryId);
		vValue = csPSTare[m_languageUsed][i];
		m_pPSX->SetReceiptItemVariable(_T("AMTareList"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
		vValue = lPSTareCode[m_languageUsed][i];
		m_pPSX->SetReceiptItemVariable(_T("AMTareList"), csItemEntryId, _T("ITEM_SMTARECODE"), vValue);
		m_pPSX->UpdateReceiptControls(_T("AMTareList"));
		ps.SetSelection(_T("AMTareList"),_T("1"));
	}

	return 0;

}

//************************************************************************ 
//
// Function:		LoadTareInfo
// Purpose:			This function loads tare info from language specific Tare 
//                  description files (for dual language)if no default filename 
//                  (szName) is specified. Otherwise, just load the default tares
// Parameters:      [in] szName - Tare filename
// Returns:	        0
// Calls:           SetTareListToCurrentLanguage
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::LoadTareInfo(LPCTSTR szName)
{
  CString csWork, csTareName, csTareCode, csTareImage, csTemp, csTareFilePath;
  long lTare;
  int nLanguage;
  int i;
  nLanguage = SCOT_LANGUAGE_PRIMARY;
    
   if (szName==NULL)
  {
      csTareFilePath = co.csConfigFilePath + _T("\\") + co.Getlanguage(nLanguage).csTareFile;
  }
  else
  { 
      csTareFilePath = szName;
  }

  // open the tare info file for read only access
  do
  {
      if ((nLanguage < SCOT_LANGUAGE_MAX) && (co.GetfStateDualLanguage()))
      {
          csTareFilePath = co.csConfigFilePath + _T("\\") + co.Getlanguage(nLanguage).csTareFile;
           //TAR262985+
          //Figure out if *.000 or *.xxx exists for the second language
          CString csFile;
          CString csFile1;
          CString csFile2;
          TBRC nTBCallStatus;
          int nTerminalNumber = TBGetTerminalNumber();
          CString csFileName1, csFileName2;

          csFile = co.Getlanguage(nLanguage).csTareFile;
          csFile = csFile.Left(csFile.GetLength()-4);

          csFile1.Format(co.csConfigFilePath + _T("\\%s.%03d"), csFile, 0);
          csFile2.Format(co.csConfigFilePath + _T("\\%s.%03d"), csFile, nTerminalNumber);

          csFileName1.Format(_T("%s.000"),csFile);
          csFileName2.Format(_T("%s.%03d"),csFile, nTerminalNumber);
          nTBCallStatus = tb.CopyFileFromServer(csFileName1,csFile1,false); 
          nTBCallStatus = tb.CopyFileFromServer(csFileName2,csFile2,false);


          if (SMStateBase::OpenSCOTFile(csFile1, false, false))  //see if SCOTTARE.000 exists
          {
                //_tcscpy(csTareFilePath, csFile1);
                csTareFilePath = csFile1;
          }

    
          if (SMStateBase::OpenSCOTFile(csFile2, false, false))
          {
                csTareFilePath = csFile2;
          }
          //TAR262985-
          
      }
      i = 0;
      try
      {
          CStdioFile csfIn;
          bool bFileUnicode = CSCOTStringList::IsFileUnicode((LPCTSTR)csTareFilePath);
          trace(L6,_T("The file %s is Unicode? <%d>"), csTareFilePath, bFileUnicode );

          if (!bFileUnicode)
              csfIn.Open((LPCTSTR)csTareFilePath, CFile::modeRead);
          else
              csfIn.Open((LPCTSTR)csTareFilePath, CFile::modeRead |CFile::typeBinary);
          // read a complete line, each line is 

          // tare description=tare code,bitmap name - Bitmaps are displayed on tare buttons
          // for 3.1 requirements
          while (csfIn.ReadString(csWork))
          {
             if (csWork.Left(1) == 0xFEFF) //unicode file byte-order-mark
                csWork = csWork.Mid(1);
              csWork.TrimLeft();
              csWork.TrimRight();
              if(csWork.GetLength() > 0)
              {
                ParseTareInfo(csWork, i, nLanguage);
                i++;
                if (i==MAXTARE) break;  // IBM Smkt only supports 10 tares
              }
          }
          nTareCount[nLanguage] =i;
          // all done close the file
          csfIn.Close();
      }
      catch(CFileException *e)

      {
          eo.FileError(INFO,_T("Tare List"),*e);
          e->Delete();
      }

  
  }
  //while (szName == NULL && nLanguage < co.GetNumberOfLanguagesSupported());
  while (++nLanguage < co.GetNumberOfLanguagesSupported()); //TAR262985

  SetTareListToCurrentLanguage();

  return 0;
}    

void PSProceduresBase::ParseTareInfo(CString csTareLine, int i, int nLanguage)
{
      CString csTareName, csTareCode, csTareImage, csTemp, csTareFilePath;
     
      csTareName = csTareLine.SpanExcluding(_T("="));
      csTemp = csTareLine.Right(csTareLine.GetLength() - csTareName.GetLength() - 1);
      csTareCode = csTemp.SpanExcluding(_T(","));
      csTareImage = csTareLine.Right(csTemp.GetLength() - csTareCode.GetLength() - 1);
      csPSTare[nLanguage][i] = csTareName;
      // if no bitmap defined in the data file, then setto default bitmap
      if (csTareImage.IsEmpty())
      {
          // +mb185050
          CString csDefGraphics = co.csStateDefaultProduceBMPName;  
          int nIndex = csDefGraphics.Find(_T(".")); 
          CString csGraphicName = csDefGraphics.Left(nIndex-4);                   
          CString csGraphicExt = csDefGraphics.Right(csDefGraphics.GetLength() - nIndex);
        
          CString csLangCode[SCOT_LANGUAGE_MAX];
          ps.ConvertSCOTLangID2LCID(co.Getlanguage(nLanguage).szCode, csLangCode[nLanguage]);                    
          
          CString csGraphics = csGraphicName + csLangCode[nLanguage] + csGraphicExt;  
          
          csPSTareImage[nLanguage][i] = csGraphics;
          
          _TCHAR szPath[_MAX_PATH]; 
          CString csImagePath;
          GET_PARTITION_PATH(SCOTIMAGE, csImagePath.GetBuffer(_MAX_PATH));
          _tmakepath( szPath, NULL, csImagePath, csGraphics, NULL );
          csImagePath.ReleaseBuffer();
          
          CFileFind finder;
          if (!finder.FindFile(szPath))
          {
              trace(L6, _T("Image %s doesn't exist."), csGraphics); 
              csPSTareImage[nLanguage][i] = co.csStateDefaultProduceBMPName;
          }
          // -mb185050
      }
      else
      {
          csPSTareImage[nLanguage][i]  = csTareImage;
      }
      long lTare = _ttol(csTareCode);
      if (lTare >= -1)          // Tar 152490 allow -1 for "No Container" choice on "Tare Pick List"
          lPSTareCode[nLanguage][i] = lTare;
      else lPSTareCode[nLanguage][i] = i;  // if no tare code explicitly declared, default to line #
          
}

//************************************************************************ 
//
// Function:		Background
// Purpose:			This function gets the name of last bitmap.
// Parameters:      none
// Returns:	        returns string containing the last BMP name
// Calls:           GetBmpName
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::Background(void)// get name of last BMP
{
  if (nLastBmp==BMP_COUNTER)
    return _T("Initializing");
    
  return  GetBmpName(nLastBmp);
}

//************************************************************************ 
//
// Function:		AddMediaList
// Purpose:			This function adds text to the MediaList control on the view
// Parameters:      [in] text - text to add
// Returns:	        0
// Calls:           ConvertToEscapeSequence
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddMediaList(CString text)
{
	COleVariant vValue;
	CString csItemEntryId;

    csItemEntryId.Format(_T("%d"), m_lMediaListEntryId++);
	m_pPSX->CreateReceiptItem(_T("MediaList"), csItemEntryId);
	vValue = ConvertToEscapeSequence(text, _T("\\n"), _T("\n"));
	m_pPSX->SetReceiptItemVariable(_T("MediaList"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
	m_pPSX->UpdateReceiptControls(_T("MediaList"));

	return 0;
}

//************************************************************************ 
//
// Function:        AddDetailsList
// Purpose:         This function adds text to the DetailsList control on the view
// Parameters:      [in] text - text to add
// Returns:         0
// Calls:           ConvertToEscapeSequence
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddDetailsList(CString text)
{
    COleVariant vValue;
    CString csItemEntryId;

    csItemEntryId.Format(_T("%d"), m_lDetailsListEntryId++);
    m_pPSX->CreateReceiptItem(_T("DetailsList"), csItemEntryId);
    vValue = ConvertToEscapeSequence(text, _T("\\n"), _T("\n"));
    m_pPSX->SetReceiptItemVariable(_T("DetailsList"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
    m_pPSX->UpdateReceiptControls(_T("DetailsList"));

    return 0;
}

//************************************************************************ 
//
// Function:		AddSMMItemList
// Purpose:			This function adds text to the SMMItemList control on the view
// Parameters:      [in] text - text to add
// Returns:	        0
// Calls:           ConvertToEscapeSequence
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddSMMItemList(CString text)
{
	COleVariant vValue;
	CString csItemEntryId;

    csItemEntryId.Format(_T("%d"), m_lSmmEntryId++);
	m_pPSX->CreateReceiptItem(_T("SMMItemList"), csItemEntryId);
	vValue = ConvertToEscapeSequence(text, _T("\\n"), _T("\n"));;
	m_pPSX->SetReceiptItemVariable(_T("SMMItemList"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
	m_pPSX->UpdateReceiptControls(_T("SMMItemList"));

	return 0;
}

//************************************************************************ 
//
// Function:		AddDelaySecurityList
// Purpose:			This function adds text to the DelaySecurityList control on the view
// Parameters:      [in] text - text to add
// Returns:	        0
// Calls:           ConvertToEscapeSequence
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::AddDelaySecurityList(CString text)
{
	COleVariant vValue;
	CString csItemEntryId;

    csItemEntryId.Format(_T("%d"), m_lDelaySecurityEntryId++);
	m_pPSX->CreateReceiptItem(_T("DelaySecurityList"), csItemEntryId);
	vValue = ConvertToEscapeSequence(text, _T("\\n"), _T("\n"));;
	m_pPSX->SetReceiptItemVariable(_T("DelaySecurityList"), csItemEntryId, _T("ITEM_SMTEXT"), vValue);
	m_pPSX->UpdateReceiptControls(_T("DelaySecurityList"));

	return 0;
}

//************************************************************************ 
//
// Function:		ResetDelaySecurityList
// Purpose:			This function clears the DelaySecurityList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetDelaySecurityList()
{
	m_pPSX->ClearReceipt(_T("DelaySecurityList"));
	m_lDelaySecurityEntryId = 0;
	return 0;
}

//************************************************************************ 
//
// Function:		ClearDelaySecurityList
// Purpose:			This function hides the DelaySecurityList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ClearDelaySecurityList()
{
	COleVariant vValue;
	vValue = VARIANT_FALSE;
    m_pPSX->SetConfigProperty(_T("DelaySecurityList"), _T(""), UI::PROPERTYVISIBLE, vValue);
	return 0;
}

//************************************************************************ 
//
// Function:		ResetMediaList
// Purpose:			This function clears the MediaList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetMediaList()
{
	m_pPSX->ClearReceipt(_T("MediaList"));
	m_lMediaListEntryId = 0;
	return 0;
}


//************************************************************************ 
//
// Function:        ResetDetailsList
// Purpose:         This function clears the DetailsList receipt control
// Parameters:      none
// Returns:         0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetDetailsList()
{
    m_pPSX->ClearReceipt(_T("DetailsList"));
    m_lDetailsListEntryId = 0;
    return 0;
}

//************************************************************************ 
//
// Function:		ResetAMTareList
// Purpose:			This function clears the AMTareList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetAMTareList()
{
	m_pPSX->ClearReceipt(_T("AMTareList"));
	m_lAMTareListEntryId = 0;
	return 0;
}

//************************************************************************ 
//
// Function:		ResetSMMItemList
// Purpose:			This function clears the SMMItemList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ResetSMMItemList()
{
	m_pPSX->ClearReceipt(_T("SMMItemList"));
	m_lSmmEntryId = 0;
	return 0;
}

//************************************************************************ 
//
// Function:		ClearSMMItemList
// Purpose:			This function hides the SMMItemList receipt control
// Parameters:      none
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ClearSMMItemList()
{
	COleVariant vValue;
	vValue = VARIANT_FALSE;
    m_pPSX->SetConfigProperty(_T("SMMItemList"), _T(""), UI::PROPERTYVISIBLE, vValue);
	return 0;
}



//************************************************************************ 
//
// Function:		DelayMessage12
// Purpose:			This function sets global nDelayedMessage12 to the 
//                  specified message by nMsg
// Parameters:      [in] nMsg - new message id
// Returns:	        returns nMsg
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::DelayMessage12(PSTEXT nMsg)
{
  nDelayedMessage12 = nMsg;
  return nMsg;
}

//************************************************************************ 
//
// Function:		ShowDelayedMessages
// Purpose:			This function shows delayed msg
// Parameters:      none
// Returns:	        True - if message shown
//                  false - no message shown
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::ShowDelayedMessages(void)
{
  if (nDelayedMessage12!=PS_NOTEXT)
  {
    Message12(nDelayedMessage12);
    nDelayedMessage12 = PS_NOTEXT;
    return true;									  // message shown
  }
  return false;										  // no message shown
}

//************************************************************************ 
//
// Function:		ShowCashBackList
// Purpose:			This function assigns text, image, and values for the
//                  CashBackList ButtonList control.
// Parameters:      [in] lDue - exact amount
//                  [in] nBack - is an array of int's, each is a dollar
//                  amount (not cents) like 5,10,20,50,100... we have space
//                  for up to 6 CB's, the last CB must be a 0 to end the array
//                  [in] numBack - max number of cash back amounts
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ShowCashBackList(const long lDue, int nBack[], int numBack , bool bTenderEbt)
{
	USES_CONVERSION;
	CString csWork;
	CStringArray csCashBackArray;
	// BeginCodeLocale
	csWork.Format(GetPSText(TXT_EAMOUNT),FmtDollar(lDue));	
	csCashBackArray.Add( ConvertToEscapeSequence( csWork , _T("\\n"), _T("\n")) ); //RFC 295823
	// BeginCodeLocale

	for (long int i=0;i<numBack;i++)
	{
		csWork = _T("");
		if (nBack[i]>0)
		{
		  // BeginCodeLocale      
	      long lAmt = lDue+nBack[i]*co.nLocalePriceCentUnit;
		  csWork.Format(GetPSText(TXT_PAMOUNT),FmtDollar(nBack[i]*co.nLocalePriceCentUnit),FmtDollar(lAmt));
		  // BeginCodeLocale
		  csCashBackArray.Add(csWork);	     
        }
    }
    
	long nButtonCount = numBack+1;
    CString csCtlName = _T("CashBackList");
    CString csCtxName = _T("SelectPayment");
	//tar 226722
	if (bTenderEbt)
	{
		csCtlName = _T("EBTCashBackList");
        csCtxName = _T("SelectPaymentEBT");
	}
	//end of tar 226722
	COleVariant v = nButtonCount;

    CPSXRedrawLock csRedrawLock(csCtlName, _T("PSProceduresBase::ShowCashBackList"));

    m_pPSX->SetConfigProperty( csCtlName, csCtxName, UI::PROPERTYBUTTONCOUNT, v );
    m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );
    CString str = _T("");
    COleSafeArray saTextArray, saItemCodeArray;
    saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
    saItemCodeArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);
	int j = 1;
	for(  i = 0; i < nButtonCount; i++ )
	{
        str = csCashBackArray[i];
		// +TAR 314786
		BSTR bstrText = str.AllocSysString();
		saTextArray.PutElement(&i, bstrText );
		::SysFreeString(bstrText);
		// -TAR 314786
		v =(long) j;
        saItemCodeArray.PutElement( &i , &v);
		j++;
	}
	
	COleVariant vReturn;
    v = saTextArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
    v = saItemCodeArray;
    m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
    
    return 0;
}


//************************************************************************ 
//
// Function:		GetCoupon
// Purpose:			This function gets each coupon from the coupon list
// Parameters:      [in, out] lCouponIndex - index of a coupon item on the receipt
//                  [out] csDesc - Item description
//                  [out] csCode - Item code
//                  [out] lQty - Item quantity
//                  [out] lWgt - Item Weight
//                  [out] lPrc - Item price
//                  [out] lUPrc - Item extended price
//                  [out] lTcd -Item Tare code
//                  [out] lBardcodeType - Item barcode type
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetCoupon(long& lCouponIndex, CString& csDesc, CString& csCode, 
							long& lQty, long& lWgt, long& lPrc, long &lUPrc, long& lTcd, long& lBarcodeType)   // Get coupon in the coupon list
{
	USES_CONVERSION;
	CStringArray csItemEntryIdList;
	m_pPSX->FindReceiptItems(_T("SMReceiptCoupon"), csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize();  //Get the number of coupons
	CString csItemEntryId;
	COleVariant vValue;

	while(lCouponIndex <= nCountList)    
	{
		vValue.Clear();
		csItemEntryId = csItemEntryIdList.GetAt(lCouponIndex - 1);
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_CODE"), vValue);
	    vValue.ChangeType( VT_BSTR );
		csCode = OLE2T(vValue.bstrVal);

	    vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	    vValue.ChangeType( VT_BSTR );
		csDesc = OLE2T(vValue.bstrVal);

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_QUANTITY"), vValue);
	    vValue.ChangeType( VT_INT );
		lQty = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_WEIGHT"), vValue);
	    vValue.ChangeType( VT_INT );
		lWgt = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
	    vValue.ChangeType( VT_INT );
		lPrc = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_PRICE"), vValue);
	    vValue.ChangeType( VT_INT );
		lUPrc = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_TARECODE"), vValue);
	    vValue.ChangeType( VT_INT );
		lTcd = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	    vValue.ChangeType( VT_INT );
		lBarcodeType = vValue.lVal;

		vValue.Clear();
		PSXRC rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_VOID"), vValue);
		if (rc != PSX_SUCCESS)
			rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptCoupon"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

		if (rc != PSX_SUCCESS) //This coupon is not a voided coupon
		{
			lCouponIndex++; 
			return 1;
		}
		lCouponIndex++;
	}
	return 0;
}
//************************************************************************ 
//
// Function:		GetVisualItem

// Purpose:			This function gets each visual item from the item list
// Parameters:      [in, out] lVisualItemIndex - index of a visual item on the receipt
//                  [out] csDesc - Item description
//                  [out] csCode - Item code
//                  [out] lQty - Item quantity
//                  [out] lWgt - Item Weight
//                  [out] lPrc - Item price
//                  [out] lUPrc - Item extended price
//                  [out] lTcd -Item Tare code
//                  [out] lBardcodeType - Item barcode type
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetVisualItem(long& lVisualItemIndex, CString& csDesc, CString& csCode,
							long& lQty, long& lWgt, long& lPrc, long& lUPrc, long& lTcd, long& lBarcodeType) 
{
	//This function is not called anywhere in the core app. 
    USES_CONVERSION;
	CStringArray csItemEntryIdList;
	m_pPSX->FindReceiptItems(_T("SMReceiptVisualItem"), csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize(); 
	CString csItemEntryId;
	COleVariant vValue;

	while(lVisualItemIndex <= nCountList)    
	{
		vValue.Clear();
		csItemEntryId = csItemEntryIdList.GetAt(lVisualItemIndex - 1);
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_CODE"), vValue);
	    vValue.ChangeType( VT_BSTR );
		csCode = OLE2T(vValue.bstrVal);

	    vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_DESCRIPTION"), vValue);
	    vValue.ChangeType( VT_BSTR );
		csDesc = OLE2T(vValue.bstrVal);

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_QUANTITY"), vValue);
	    vValue.ChangeType( VT_INT );
		lQty = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_WEIGHT"), vValue);
	    vValue.ChangeType( VT_INT );
		lWgt = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_EXT_PRICE"), vValue);
	    vValue.ChangeType( VT_INT );
		lPrc = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_PRICE"), vValue);
	    vValue.ChangeType( VT_INT );
		lUPrc = vValue.lVal;



		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_TARECODE"), vValue);
	    vValue.ChangeType( VT_INT );
		lTcd = vValue.lVal;

		vValue.Clear();
		m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_SCAN_TYPE"), vValue);
	    vValue.ChangeType( VT_INT );
		lBarcodeType = vValue.lVal;

		vValue.Clear();
		PSXRC rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_VOID"), vValue);
		if (rc != PSX_SUCCESS)
			rc = m_pPSX->GetReceiptItemVariable(_T("SMReceiptVisualItem"), csItemEntryId, _T("ITEM_VOIDADDED"), vValue);

		if (rc != PSX_SUCCESS) 
		{
			lVisualItemIndex++; 
			return 1;
		}
		lVisualItemIndex++;
	}
	return 0;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		PSProceduresBase
// Purpose:			Class constructor
// Parameters:      none
// Returns:	        none
// Calls:           UnLoadSCOTStringConstants
// Side Effects:
// Notes:
//
//************************************************************************
PSProceduresBase::PSProceduresBase() :
    m_pStrMgr(NULL),
    m_pPSX( NULL ),
    m_bPsxConfigLoaded( false ),
    m_bPsxInitialized( false ),
    m_bFoundPickListItemsFromSearch(false),
    m_hWaitVolSet(0), m_pVolCtl(NULL),        // TAR 351654
    m_SigCapWindow(NULL), m_bUsingPSXSigCapWindow(false)
{
    lPSPointsTotal  =  0;	// total reward points for this trx
	lPSSavingsTotal =  0;	// total savings for this trx
    m_languageUsed      = SCOT_LANGUAGE_PRIMARY;
	m_bFirstTimeNextButtonEnabled = FALSE;

	nNumLinkedItems = 0; //tar 23376
	
	s_szProduceButtonListName = _T("ProductImage");
	s_szProduceContextName = _T("ProduceFavorites");
	
	s_szProduceScrollUpName = _T("CMProduceSelectPrev");
	s_szProduceScrollDownName = _T("CMProduceSelectNext");
	s_szQuickPickButtonListName = _T("QuickPickImage");
	s_szQuickPickContextName = _T("QuickPickItems");
    m_ProduceLookupCategoryNumber = _T("");
    m_ProduceLookupCategoryLevels = _T("");
    m_FirstLevelCategoryID = _T("");
    m_SecondLevelCategoryID = _T("");
	m_csCurrentContext.Empty();
    for (int i = 0; i < SCOT_LANGUAGE_MAX; i++)
    {
        ItemList[i].EmptyList();
        m_Search[i].EmptyList();
        CategoryList[i].EmptyCategoryList();
		QuickPickItemList[i].EmptyList();
        //+ Personalization picklist
        m_StorePopularItemList[i].EmptyList();
        m_PersonalizedPopularItemList[i].EmptyList();
        m_tempFavoritesFromServer[i].EmptyList();
        //- Personalization picklist
        //+ Dynamic QuickPick
        m_PersonalizedQuickPickItemList[i].EmptyList();
        //- Dynamic QuickPick

		m_PicklistAssistItemList[i].EmptyList();

		MainCategoryList[i].EmptyCategoryList();
		SubCategoryList[i].EmptyCategoryList();
    }
	if (m_csPickQtyList.GetSize())
		m_csPickQtyList.RemoveAll();

    //m_mPickListItemsMap.InitHashTable(1087);
    //m_mPickListItemsMap.RemoveAll();

	//+Japan RFC#1 (RFQ 893)
	if ( !m_mZeroWeightItems.IsEmpty() )
	{
		m_mZeroWeightItems.RemoveAll();
	}
	//-Japan RFC#1 (RFQ 893)
	UnLoadSCOTStringConstants();	// initialize all string lists to null

   m_bFoundPickListItemsFromSearch=false;

   // +TAR 351654
   m_hWaitVolSet = CreateEvent(NULL, FALSE, FALSE, NULL);
   // -TAR 351654

   //+SR828
   m_HKSensitiveControls.Add(CString(_T("Line1AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("Line2AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("Line3AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("Line4AlphaNumericKeys")));
   //-SR828
   
   m_HKSensitiveControls.Add(CString(_T("PFKBLine1AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("PFKBLine2AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("PFKBLine3AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("PFKBLine4AlphaNumericKeys")));
   
   m_HKSensitiveControls.Add(CString(_T("XMLine1AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("XMLine2AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("XMLine3AlphaNumericKeys")));
   m_HKSensitiveControls.Add(CString(_T("XMLine4AlphaNumericKeys")));
   
#ifdef _CPPUNIT
   nRegisterClassCalled=0;
#endif

}

//************************************************************************ 
//
// Function:		~PSProceduresBase
// Purpose:			Class destructor
// Parameters:      none
// Returns:	        none
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
PSProceduresBase::~PSProceduresBase()
{

	UnLoadSCOTStringConstants();	// initialize all string lists to null
	csPSCustomerRewardMessage.Empty();
	m_lastMessageBoxText.Empty();
	//+Japan RFC#1 (RFQ 893)
	if ( !m_mZeroWeightItems.IsEmpty() )
	{
		m_mZeroWeightItems.RemoveAll();
	}
	//-Japan RFC#1 (RFQ 893)

    try
    {
        CloseHandle(m_hWaitVolSet);     // TAR 351654
    }
    catch(...)      // Destructors must not throw exceptions.
    {
    }
}

#ifndef _CPPUNIT

//************************************************************************ 
//
// Function:		SetButtonText
// Purpose:			This function sets a button text
// Parameters:      [in] szCtlName - button control name
//                  [in] nButton - message id corresponding to button text
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetButtonText(LPCTSTR szCtlName, const PSTEXT nButton)
{

	Button(szCtlName, nButton, true);
}
//************************************************************************ 
//
// Function:		Button
// Purpose:			This function shows a button. If szContext is empty it
//                  will hide the button. 
// Parameters:      [in] szCtlName - button control name
//                  [in] szContext - button text
//                  [in] fEnable - T=Enabled, F=Disabled but still visible
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
long PSProceduresBase::Button(LPCTSTR szCtlName,     // show a button
					          LPCTSTR     szContent,  // button text
					          const bool fEnable)    // T=Enabled, F=Disabled but still visible
{
	COleVariant v;
	v = ConvertToEscapeSequence(szContent, _T("\\n"), _T("\n"));
    m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
	v.Clear();
	int length = _tcsclen(szContent);
    // If no text, then make the control invisible 
	if(length == 0)
	{
        m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
	}
    else
    {
        // Otherwise make sure that the control is visible before, setting text 
        m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	    if(fEnable)
            v = (long) UI::Normal;
	    else
            v = (long) UI::Disabled;
        m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYSTATE, v);
    }
    return 0;
}
//************************************************************************ 
//
// Function:		ButtonState
// Purpose:			This function sets the enabled/disabled visible/hidden 
//                  properties of a button. 
// Parameters:      [in] szCtlName - button control name
//                  [in] fEnable - T=Enabled, F=Disabled but still visible
//                  [in] fEnable - T=visible, F=hidden
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
long PSProceduresBase::ButtonState(LPCTSTR szCtlName,     // show a button
					          const bool fEnable,  // T=Enabled, F=Disabled but still visible
					          const bool fShow)    // T=Visable, F=hidden
{
	COleVariant v;
	if(fShow)
	{
        m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_TRUE);
	}
    else
    {
        m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYVISIBLE, VARIANT_FALSE);
    }
    
    if(fEnable)
        v = (long) UI::Normal;
    else
        v = (long) UI::Disabled;
    m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYSTATE, v);
    
    return 0;
}
//************************************************************************ 
//
// Function:		ButtonEnable
// Purpose:			This function enables or disables a button in the current context. 
// Parameters:      [in] szCtlName - button control name
//                  [in] fEnable - T=Enabled, F=Disabled but still visible
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
long PSProceduresBase::ButtonEnable(LPCTSTR szCtlName,     // control name
					          const bool fEnable)  // T=Enabled, F=Disabled but still visible
{
	COleVariant v;
    
    if(fEnable)
        v = (long) UI::Normal;
    else
        v = (long) UI::Disabled;
    m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYSTATE, v);
    
    return 0;
}
//************************************************************************ 
//
// Function:		Button
// Purpose:			This function shows a button. 
// Parameters:      [in] szCtlName - button control name
//                  [in] nText - message id corresponding to button text
//                  [in] fEnable - T=Enabled, F=Disabled but still visible
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::Button(LPCTSTR szCtlName,	// show a button
	   		              	  const PSTEXT nText,	// button text
					          const bool   fEnable)	// T=Enabled, F=Disabled but still visible
{
	Button(szCtlName, GetPSText(nText), fEnable);
    return 0;
}

//************************************************************************ 
//
// Function:		ButtonFlash
// Purpose:			This function used to flash or not to flash a button
// Parameters:      [in] szCtlName - button control name
//                  [in] fFlash - T=Flash, F=Not Flash
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ButtonFlash(LPCTSTR szCtlName,  // button to flash or not flash
                                   const bool fFlash) // make a button flash
{
	COleVariant v;
	if (fFlash)
		v = VARIANT_TRUE;
	else
		v = VARIANT_FALSE;
    m_pPSX->SetConfigProperty(szCtlName, m_csCurrentContext, UI::PROPERTYFLASH, v);

    return 0;
}

//************************************************************************ 
//
// Function:		SetCurrentFrame
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetCurrentFrame(LPCTSTR lpszContext)
{
	SetFrame(lpszContext);
}

//************************************************************************ 
//
// Function:		SetLeadthruText
// Purpose:			This function displays text in the lead through area
// Parameters:      [in] sztext1 - text to display in LeadThruText area
//                  [in] szText2 - text to display in LeadthruText2 area
// Returns:	        0
// Calls:           SetLeadthruText
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetLeadthruText(LPCTSTR szText1,LPCTSTR szText2)// display text in prompt area
{
	COleVariant v, v1;

	v =	ConvertToEscapeSequence(szText1, _T("\\n"), _T("\n"));
	m_pPSX->SetConfigProperty(_T("SMLeadthruText"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
    m_pPSX->SetConfigProperty(_T("LeadthruText"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
	if(szText2)
	{
		v1 = ConvertToEscapeSequence(szText2, _T("\\n"), _T("\n"));
	}
    else
    {
    	v1 = szText2;
    }

    m_pPSX->SetConfigProperty(_T("LeadthruText2"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v1);

    return 0;
}

//************************************************************************ 
//
// Function:		SetLeadthruText
// Purpose:			This function displays text in the lead through area
// Parameters:      [in] ntext1 - message id Corresponding to text to display 
//                  in LeadThruText area
//                  [in] nText2 - message id corresponding text to display in 
//                  LeadthruText2 area
// Returns:	        0
// Calls:           SetleadthruText
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetLeadthruText(const PSTEXT nText1, const PSTEXT nText2)// leadthru text string
{
  return SetLeadthruText(GetPSText(nText1), (nText2 == PS_BLANK) ? (LPCTSTR )NULL : GetPSText(nText2));
}

//************************************************************************ 
//
// Function:		SetLeadthruText
// Purpose:			This function displays text in the lead through area
// Parameters:      [in] ntext1 - message id Corresponding to text to display 
//                  in LeadThruText area
//                  [in] szText2 - text to display in LeadthruText2 area
// Returns:	        0
// Calls:           SetLeadthruText
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetLeadthruText(const PSTEXT nText1, LPCTSTR szText2)// leadthru text string
{
  return SetLeadthruText(GetPSText(nText1), szText2);
}

//************************************************************************ 
//
// Function:		SetLTTinto2
// Purpose:			This function formats one line into two line, then displays 
//                  it in the lead through area
// Parameters:      szLTText - text to display 
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetLTTinto2(LPCTSTR szLTText)// format one line into 2
{
    /*CString csLTT1stLine, csLTT2ndLine, csTexttoConvert;
    csTexttoConvert = szLTText;
    int iTotalLength = csTexttoConvert.GetLength();

    int iSplitLines = csTexttoConvert.Find(_T("|"));
    csLTT1stLine = csTexttoConvert.Left(iSplitLines);
    csLTT2ndLine = csTexttoConvert.Right(iTotalLength - (iSplitLines + 1));
    SetLeadthruText(csLTT1stLine, csLTT2ndLine);*/

    //SSCOADK-3345

    SetLeadthruText(szLTText);

    return 0;
}

//************************************************************************ 
//
// Function:		SetLTTinto2
// Purpose:			This function formats one line into two line, then displays 
//                  it in the lead through area
// Parameters:      nLTText - message id Corresponding to text to display
// Returns:         0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetLTTinto2(const PSTEXT nLTText)// format one line into 2
{
  return SetLTTinto2(GetPSText(nLTText));
}
//************************************************************************ 
//
// Function:		SetTitle
// Purpose:			This function displays text in title area
// Parameters:      [in] szText1 - text to display in Title1 area
//                  [in] szText2 - text to display in Title2 area
// Returns:	        0
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetTitle(LPCTSTR szText1, LPCTSTR szText2)// display text in title area
{
	COleVariant v;
	v =	ConvertToEscapeSequence(szText1, _T("\\n"), _T("\n")); // TAR 390967
    m_pPSX->SetConfigProperty(_T("Title1"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);

	if(szText2)
	{
		v=szText2;
        m_pPSX->SetConfigProperty(_T("Title2"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
	}
    return 0;
}

//************************************************************************ 
//
// Function:		SetTitle
// Purpose:			This function displays text in title area
// Parameters:      [in] nText1 - message id corresponding to text to display 
//                  in Title1 area
//                  [in] nText2 - message id corresponding to text to display 
//                  in Title2 area
// Returns:	        0	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
 long PSProceduresBase::SetTitle(const PSTEXT nText1, const PSTEXT nText2)// display text in title area
 {
   return SetTitle(GetPSText(nText1), (nText2 == PS_NOTEXT) ? (LPCSTR )NULL : GetPSText(nText2));
 }

//************************************************************************ 
//
// Function:		SetDisplayWMFlag
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
 void PSProceduresBase::SetDisplayWMFlag(bool bNeedDisplayWM) 
 {
	 m_bNeedDisplayWM = bNeedDisplayWM;
 }

#endif
//************************************************************************ 
//
// Function:		SetFrame
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
 void PSProceduresBase::SetFrame(LPCTSTR lpszContext) 
 {
	 m_csCurrentContext = lpszContext; 
//	 CPSXRedrawLock  csRedrawLock(_T("Display"), _T("PSProceduresBase::SetFrame"));
     //     SetLanguage(SCOT_LANGUAGE_PRIMARY);
     if (co.GetfStateDualLanguage()  &&
         !mp.sCurr->storeMode() &&
         !mp.sCurr->helpMode() &&
         mp.sCurr->m_customerLanguage != m_languageUsed &&
         SMStateBase::fSAInTransaction )
     {
         //CPSXRedrawLock  csRedrawLock(_T("Display"), _T("MPProcedures::ProcessNextState"));
         SetLanguage(mp.sCurr->m_customerLanguage);  
     }
     m_pPSX->SetTransactionVariable(_T("NextGenUISetFrame"), COleVariant(m_csCurrentContext));
 }


//************************************************************************ 
//
// Function:		SetCMFrame
// Purpose:		Set frame to custom prefixed version of lpszContext	
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
 void PSProceduresBase::SetCMFrame(LPCTSTR lpszContext) 
 {
     CString csContext(lpszContext);
     if(!IsInXM())//ignore context prefix in XM
     {
        csContext= co.GetContextPrefix() + lpszContext;
     }
	 m_csCurrentContext = csContext; 
     if (co.GetfStateDualLanguage()  &&
         !mp.sCurr->storeMode() &&
         !mp.sCurr->helpMode() &&
         mp.sCurr->m_customerLanguage != m_languageUsed &&
         SMStateBase::fSAInTransaction &&
         !mp.sCurr->bFromAssistMenu &&	// SSCOB-18178 avoid showing customer language while we are still in store mode no setting
		 csContext != _T("Startup")) 
     {
         //CPSXRedrawLock  csRedrawLock(_T("Display"), _T("MPProcedures::ProcessNextState"));
         SetLanguage(mp.sCurr->m_customerLanguage);  
         trace(L6, _T("SetCMFrame: set language to cusotmer langugae"));
     }
     else
     {
         trace(L6, _T("SetCMFrame: does not set the language"));
     }
     mp.sCurr->bFromAssistMenu = false; // SSCOB-18178 SSCOADK-6286
     m_pPSX->SetTransactionVariable(_T("NextGenUISetFrame"), COleVariant(csContext));
 }

//************************************************************************ 
//
// Function:		ShowFrame
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ShowFrame( bool bHelpState )
{
    if( m_bPsxConfigLoaded )
    {
        //sscoadk-6740+
        //set language right from showFrame to avoid the flicking such as: 
        //when scotapp exits from store mode, setCMFrame(setlanguage) is called before showframe() for the next customer mode state; 
        //this causes showing customer lanugage while ssco is still in store mode.
        //To fix this, we are not setting the lanugage in setCMFrame when bFromAssistMenu is true
        //language will be set at here
        if (co.GetfStateDualLanguage()  &&
            mp.sCurr->m_customerLanguage != m_languageUsed &&
            SMStateBase::fSAInTransaction 
            && !mp.sCurr->storeMode() 
            && !mp.sCurr->helpMode()
            && !mp.sCurr->bFromAssistMenu
			&& m_csCurrentContext.Find(_T("Startup"))== -1) 
        { 
            SetLanguage(mp.sCurr->m_customerLanguage);  
            trace(L6, _T("ShowFrame: set language to cusotmer langugae"));
        }

        //These four states are help states but need to have lanugage set to the customer language
        if (co.GetfStateDualLanguage()  &&
           !mp.sCurr->storeMode() &&
            mp.sCurr->m_customerLanguage != m_languageUsed &&
            SMStateBase::fSAInTransaction
            && !mp.sCurr->bFromAssistMenu)
        {
             if (    (m_csCurrentContext.Find(_T("InsertCoupon"))!= -1) || (m_csCurrentContext.Find(_T("TakeChange"))!= -1)
                  || (m_csCurrentContext.Find(_T("BagAndEas"))!= -1) || (m_csCurrentContext.Find(_T("InsertGiftCard"))!= -1) )
             {
                 SetLanguage(mp.sCurr->m_customerLanguage); 
                 trace(L6, _T("ShowFrame: set language to cusotmer langugae"));
             }
        }
        //SSCOADK-6740-

        CLaneStateData laneStateData(m_pPSX);
        laneStateData.SendNextGenUIAppState();
        trace( L7, _T("::ShowFrame - CurrentContext %s"), m_csCurrentContext );

        PSXRC rc = PSX_E_FAIL;
        if( RemoteMode() && _T("WaitForRemoteAssistance") != m_csCurrentContext )
        {
            rc = m_pPSX->SetContext( m_csCurrentContext, DisplayAlternate1 );
            ra.AcquireExclusiveAccess(ra.GetRemoteConnectionName());
        }
        else
        {
            rc = m_pPSX->SetContext( m_csCurrentContext );
            if (rc == PSX_SUCCESS)
            {
                COleVariant vValue;  
                if (!IsRCError(m_pPSX->GetConfigProperty(_T(""), m_csCurrentContext, UI::PROPERTYBACKGROUNDCONTEXT, vValue)) && VARIANT_TRUE == vValue.boolVal)
                {
                    if (mp.sCurr->storeMode() || co.IsInAssistMenus())
                        laneStateData.SendNextGenUIData(_T("NextGenUIStoreBackgroundContext"), m_csCurrentContext);
                    else
                        laneStateData.SendNextGenUIData(_T("NextGenUICustomerBackgroundContext"), m_csCurrentContext);    
                }
            }
        }

		//SR672 - Enhanced device error display		
		if (m_csCurrentContext.Find(_T("WithWebControl")) != -1)
		{
			COleVariant sURL(m_csCurrentDevErrURL); 
		
			m_pPSX->SetControlProperty(_T("DeviceErrorWeb"), UI::PROPERTYURL, sURL);
            m_pPSX->SetConfigProperty(_T("DeviceErrorWebTextURL"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, sURL);				
		}

        if( PSX_SUCCESS != rc )
        {
            trace( L6, _T("::ShowFrame - PSX SetContext API failed %d."), rc );            
        }
		if ( SMStateBase::nPsxRedrawCount!=0 )		// PSX's redraw logic has change, app needs to keep track
		{
			for (int i=1; i<=SMStateBase::nPsxRedrawCount; i++)
			{
				rc = m_pPSX->SetControlProperty( _T("Display"), UI::PROPERTYREDRAW, VARIANT_TRUE );
	            trace( L6, _T("::ShowFrame - REDRAW is set to TRUE ") );            
			}
			SMStateBase::nPsxRedrawCount = 0;
		}

        laneStateData.SendNextGenUIData(_T("RemoteProxyUserInputCompleted"), true);
    }
    else
    {
        trace( L7, _T("::ShowFrame - XML config not loaded yet, not showing context: %s"), m_csCurrentContext );
    }

    return 0;
}  

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetModeDisplay
// Purpose:			Display additional "Mode" indicator 
//                  training/attendant modes
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetModeDisplay(bool bShowModeDisplay)
{
    COleVariant vVisible;

    vVisible = bShowModeDisplay?VARIANT_TRUE:VARIANT_FALSE;
    m_pPSX->SetConfigProperty(_T("ATModeDisplay1"), _T(""), UI::PROPERTYVISIBLE, vVisible);
    m_pPSX->SetConfigProperty(_T("ATModeDisplay2"), _T(""), UI::PROPERTYVISIBLE, vVisible);
}
  
//************************************************************************ 
//
// Function:		GetCurrentMessageBoxText
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
LPCTSTR PSProceduresBase::GetCurrentMessageBoxText()
{
   return (LPCTSTR) m_lastMessageBoxText;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		InitializeConfigData
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::InitializeConfigData(LPCTSTR szPlstName, LPCTSTR szTareName)
{
#ifndef _CPPUNIT
  lScaleWeight  = 0;
  lTotal  = 0;
  lTaxes  = 0;
  nInputDigits = 0;               // max digits allowed for EchoInput() func

  nLastBmp = BMP_COUNTER;
  BOOL bRetStatus = LoadSCOTStringConstants(co.csConfigFilePath);
  ASSERT(bRetStatus);
  LoadPSText(NULL,false);
  
   // load tare info from language specific files if dual language turned on
  if (co.GetfStateDualLanguage())
  {
      LoadTareInfo();
  }
  else
  {
	  LoadTareInfo(szTareName);   //otherwise use default filename
  }  

  // TAR167887 The customer message must be saved off here if it was over-ridden by scotmsge.xxx 
  if (co.fCMRewardInfo)
  {
	if (-1 == co.nCMCustomerMessage)
	{
		co.csCMCustomerMessage.Empty();
	}
	else
	{
		co.csCMCustomerMessage = ps.GetPSText(co.nCMCustomerMessage);
	}
	csPSCustomerRewardMessage = co.csCMCustomerMessage;
  } 
 
#endif //_CPPUNIT
return 0;
};

//************************************************************************ 
//
// Function:		UnInitializeConfigData
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::UnInitializeConfigData(void)
{
#ifndef _CPPUNIT
  int i;
  for (i=SCOT_LANGUAGE_PRIMARY; i<SCOT_LANGUAGE_MAX; i++)
  {  
      for (int j=0; j<MAXTARE; j++)
      {
        csPSTare[i][j].Empty();     //   = (LPCTSTR) "";
        csPSTareImage[i][j].Empty();
        lPSTareCode[i][j] = 0;
      }
  }

  if (!co.fStateAllowLookUpItem)
  {
      for (int i = 0; i < SCOT_LANGUAGE_MAX; i++)
      {
          ItemList[i].EmptyList();
          m_Search[i].EmptyList();
          CategoryList[i].EmptyCategoryList();
		  QuickPickItemList[i].EmptyList();
          //+ Personalization picklist
          m_StorePopularItemList[i].EmptyList();
          m_PersonalizedPopularItemList[i].EmptyList();
          m_tempFavoritesFromServer[i].EmptyList();
          //- Personalization picklist
		  //+ Dynamic QuickPick
          m_PersonalizedQuickPickItemList[i].EmptyList();
          //- Dynamic QuickPick

		  m_PicklistAssistItemList[i].EmptyList();

		  SubCategoryList[i].EmptyCategoryList();
		  MainCategoryList[i].EmptyCategoryList();
      }
	  if (m_csPickQtyList.GetSize())
		m_csPickQtyList.RemoveAll();

    if (!m_mPickListItemsMap.IsEmpty())
        m_mPickListItemsMap.RemoveAll();

	  //+Japan RFC#1 (RFQ 893)
	  if ( !m_mZeroWeightItems.IsEmpty() )
	  {
		 m_mZeroWeightItems.RemoveAll();
		}
	  //-Japan RFC#1 (RFQ 893)
  }
  LoadPSText(NULL,false);        // reinit working strings
  UnLoadSCOTStringConstants();
#endif //_CPPUNIT
  return 0;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		GetCurrentItemAutoApprovalFlag
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetCurrentItemAutoApprovalFlag(bool &bAutoApprove)
{
	COleVariant vValue;
	CString csItemEntryId;

	csItemEntryId = GetSelection(_T("CMReceiptVoid"));
	m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
	vValue.ChangeType( VT_INT );
	bAutoApprove = !!vValue.lVal;

	return 0;

}

bool PSProceduresBase::IsThisItemVoided(const CString& strItemId, bool& bValue)
{
    COleVariant vValue;
    PSXRC rc = PSX_SUCCESS;
    bool bReturnValue = false;

    rc = m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), strItemId, _T("ITEM_VOID"), vValue);
    if (rc != PSX_SUCCESS)
        trace(L6, _T("PSProceduresBase::IsThisItemVoided(). rc = %d."), rc);
    else
    {
        vValue.ChangeType( VT_INT );
        bValue = !!vValue.lVal;
        bReturnValue = true;
    }
    return bReturnValue;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		SetCurrentItemAutoApprovalFlag
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetCurrentItemAutoApprovalFlag(const bool bAutoApprove)
{
	//*** NewUI ***//
	COleVariant vValue;
	CString csItemEntryId;
	// csItemEntryId.Format(_T("%d"), io.d.lEntryID); //PIM
	csItemEntryId = io.d.csItemID;
	if (!io.d.bLinkedItem)
	{
		vValue = (long)bAutoApprove;
		if (io.d.fRestricted)
		{
			m_pPSX->SetReceiptItemVariable(_T("SMReceiptRestricted"), csItemEntryId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
		}
  		m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_AUTOAPPROVAL_FLAG"), vValue);
        ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
	}
	return 0;
}

//************************************************************************ 
//
// Function:		SetLanguage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetLanguage(SCOT_LANGUAGE_TYPE language)
{
#ifndef _CPPUNIT
  if ((language < SCOT_LANGUAGE_MAX) && (m_languageUsed != language))
  { 
     m_strLastProduceLookupList = _T("" ); // TAR 258232 Force pick list reload
     //This next call sets the SCOTPriceSound language for when we say the price.
     //Not technically part of PSProceduresBase, but does deal with presentation
     //services in the abstract sense - BP 3/14/00
     //SCOTPriceSoundSetLanguage(co.language[m_languageUsed].szCode, co.language[m_languageUsed].csPriceSoundConfigFile); //tar 248292
     //tar 295814
     if (!co.IsInAssistMenus())
     {
	   trace(L6, _T("SetLanguage.  Language index is %d."), language);
       m_languageUsed = language;
       SetRemoteLanguage(co.language[m_languageUsed].szCode);
     }
  }
#endif //_CPPUNIT
}

//************************************************************************ 
//
// Function:		SetRemoteLanguage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetRemoteLanguage(const CString &csISOLanguageCode)
{
    CString csLCIDCode;
    ConvertSCOTLangID2LCID(csISOLanguageCode, csLCIDCode);

    _TCHAR *pszStopCharacter;
    unsigned long lcid = _tcstoul(csLCIDCode, &pszStopCharacter, 16);
    PSXRC psxrc = m_pPSX->SetLanguage( lcid );
}

#ifndef _CPPUNIT

void PSProceduresBase::SetNextGenUICustomerLanguage(SCOT_LANGUAGE_TYPE lang)
{
    SLanguageInfo lInfo = co.Getlanguage(lang);
    CString csLangID(co.Getlanguage(lang).szCode);
    SetNextGenUICustomerLanguage(csLangID);
}

void PSProceduresBase::SetNextGenUICustomerLanguage(const CString &csISOLanguageCode)
{
    CString csLCIDCode;
    ConvertSCOTLangID2LCID(csISOLanguageCode, csLCIDCode);

    _TCHAR *pszStopCharacter;
    unsigned long lcid = _tcstoul(csLCIDCode, &pszStopCharacter, 16);
    CLaneStateData laneStateData(m_pPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUICustomerLanguage"), (long)lcid);
}
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		GetLanguage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
SCOT_LANGUAGE_TYPE PSProceduresBase::GetLanguage()
{
  if(m_languageUsed >= SCOT_LANGUAGE_MAX)
	  trace(L6, _T("Language index is out of bound.  Language index is %d."), m_languageUsed);
  return m_languageUsed;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetTareListToCurrentLanguage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
//sets the CMTare list (used for display) using the tare descriptions for the current language
void PSProceduresBase::SetTareListToCurrentLanguage()
{
  CString csTareName;
  int i = 0;
  i = co.nOperationsTareContainer1 - 1;
  if (i >= 0)
  {
    SetPSText(BTT_TARE1, csPSTare[m_languageUsed][i]);
  }
  else
  {
    SetPSText(BTT_TARE1, _T("NOTEXT"));
  }
    
  i = co.nOperationsTareContainer2 - 1;
  if (i>=0)
  {
    SetPSText(BTT_TARE2, csPSTare[m_languageUsed][i]);
  }
  else
  {
    SetPSText(BTT_TARE2, _T("NOTEXT"));
  }
}  

//************************************************************************ 
//
// Function:		SetCMReceiptToCurrentLanguage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
//sets the CMReceipt (used for display) using sub item descriptions for the current language
void PSProceduresBase::SetCMReceiptToCurrentLanguage()
{
	trace(L6,_T("+PSProceduresBase::SetCMReceiptToCurrentLanguage()"));

	CStringArray csItemEntryIdList;
	COleVariant vValue;
	CString csItemEntryId;
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList, _T(""), _T("SUB_ITEM_DESCRIPTION"));

	int nTmpCountList = csItemEntryIdList.GetSize();

	if(nTmpCountList > 0)
	{
	    //Replace the message to the current language message.
	    for (int i=0; i<nTmpCountList; i++)
	    {
		    CString csDesc;
		    if (dm.fStateTakeawayBelt)
		    {
			    csDesc = GetPSText(MSG_ITEMNOTONBELT, SMStateBase::m_customerLanguage);
		    }
		    else
		    {
			    csDesc = GetPSText(MSG_ITEMNOTBAGGED, SMStateBase::m_customerLanguage);
		    }

		    csItemEntryId = csItemEntryIdList.GetAt(i);
		    m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("SUB_ITEM_DESCRIPTION"), vValue);
		    vValue.Clear();
		    vValue = csDesc;
		    m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("SUB_ITEM_DESCRIPTION"), vValue);
	    }

	    m_pPSX->UpdateReceiptControls(_T("CMSMReceipt")); 
	}

	//SSCOP-836+
	CStringArray csItemEntryIdList1;
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList1, _T(""), _T("ITEM_QUANTITY"));

    nTmpCountList = csItemEntryIdList1.GetSize();

	if(nTmpCountList > 0)
	{
	    //Replace the message to the current language message.
	    for (int j=0; j<nTmpCountList; j++)
	    {
		    csItemEntryId = csItemEntryIdList1.GetAt(j);
		    m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_QUANTITY"), vValue);
		    m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_QUANTITY"), vValue);
	    }

	    m_pPSX->UpdateReceiptControls(_T("CMSMReceipt")); 
	}
	//SSCOP-836-
	
    trace(L6,_T("-PSProceduresBase::SetCMReceiptToCurrentLanguage()"));
}

//************************************************************************ 
//
// Function:		ProcessDescription
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool ProcessDescription(char* szDesc, int nLineLength, 
	int& nLineBreaks, int nMaxAllowedLineBreaks, char* pResultBuffer, CString& szTextToDisplay, int nLines)
{
	// NewUI
	// This function is not needed
	// NewUI
  nLineBreaks = 0;
  szTextToDisplay = "";
  int nBuffLen = (szDesc) ? strlen(szDesc) : 0;
  bool bRetValue = false;
  if (!nLineLength || !nBuffLen)
  {
	// no need to process it
	return bRetValue;
  }
  memset(pResultBuffer, 0, 1+nBuffLen+(2*nMaxAllowedLineBreaks*sizeof(char)));
  int nCurrentIndex = 0;
  char *pStart, *pEnd, *pPrev;
  pStart = szDesc;
  pEnd   = (pStart + nBuffLen);
  while (((pEnd - pStart) > nLineLength) && (nLineBreaks < nMaxAllowedLineBreaks))
  {
	// there is some buffer to process
	pPrev = pStart;
	pStart = pStart + nLineLength;
	if (!isspace(*pStart)) // if not a space to break
	{
		// adjust pStart to a position where we can break it
		--pStart;
		while ((pStart > pPrev) && (!isspace(*pStart)))
		{
			--pStart;
		}
		if (!(pStart - pPrev))
		{
			// could not find a place to break at all, may be the word is so long
			// longer than line length specified
			return bRetValue;
		}
	} // of if not white space
	if (isspace(*pStart))

	{
		// assert space at start position
		// ASSERT(isspace(pStart));
		int nCurrLineLen = (pStart - pPrev)* sizeof(char);
		strncpy(pResultBuffer+nCurrentIndex, pPrev, nCurrLineLen);
		nCurrentIndex = nCurrentIndex + nCurrLineLen;
		if (nLineBreaks == nLines-1) //line breaks is always one less than the # of lines to display
			szTextToDisplay = pResultBuffer;
		pResultBuffer[nCurrentIndex++] = '\\';
		pResultBuffer[nCurrentIndex++] = 'n';
		++nLineBreaks;
		++pStart;

		bRetValue = true;
	}
	else
	{
		return bRetValue;
	} // of if white space
  } // of while 
  int nRemainingBufferLen = (pEnd - pStart) * sizeof(char);
  if (nRemainingBufferLen)
  {
	strncpy(pResultBuffer+nCurrentIndex, pStart, nRemainingBufferLen);
	nCurrentIndex = nCurrentIndex + nRemainingBufferLen;
  }
  if (nLineBreaks < nLines)
	  szTextToDisplay = pResultBuffer;
  return bRetValue;
}
//************************************************************************ 
//
// Function:		ShowTrainingModeOnScreen
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ShowTrainingModeOnScreen(bool bShowTrainingMode)
{
  CString text;

  if (bShowTrainingMode)
      if (co.IsAttendantModeOn())
      {
          text = GetPSText(MSG_SHOW_ATTENDANT_TRAINING);
      }
      else
      {
          text = GetPSText(MSG_TRAININGMODE);
      }
  else
  {
      if (co.IsAttendantModeOn())
      {
          text = GetPSText(MSG_SHOW_ATTENDANT);
      }
      else
      {
          text = _T("");
      }
  }

  COleVariant v;
  v = text;
  m_pPSX->SetTransactionVariable(_T("TrainingMessage"), v);
  SetModeDisplay( (co.IsAttendantModeOn() || co.IsRemoteScannerConnected() || bShowTrainingMode ) );
  return 0;
}

//************************************************************************ 
//
// Function:		ItemsInFavoritesList
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ItemsInFavoritesList(void)
{
    long nCount = 0;
	POSITION pos;

	//Get favorites items
	CItemInfo *pItem = NULL;
	CSItemList* pItemList = NULL;

    pItemList = &(ItemList[m_languageUsed]);
	pos = pItemList->GetHeadPosition();

	while( pos != NULL )
	{
		pItem = (CItemInfo*)pItemList->GetNext( pos );

		if( pItem->GetIsFavorite() )
		{
			nCount++;		
		}
	}
	
	//Get favorites categories
	CCategoryInfo *pCategory = NULL;
	CSCategoryList* pCategoryList = NULL;
	CString csCategories(_T(""));

    pCategoryList = &(CategoryList[m_languageUsed]);
	pos = pCategoryList->GetHeadPosition();

	while( pos != NULL )
	{
		pCategory = (CCategoryInfo*)pCategoryList->GetNext( pos );

		csCategories = pCategory->GetCategories();

		if( csCategories.Find(TAB_FAV) != -1 )
		{
			nCount++;		
		}
	}

	return nCount;
}

//************************************************************************ 
//
// Function:		ResetReport
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ResetReport(const CString& csReceiptSrc)
{
	m_pPSX->ClearReceipt(csReceiptSrc);
}

//************************************************************************ 
//
// Function:		HideReport
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::HideReport(const CString& csReceiptSrc)
{
	COleVariant vValue = VARIANT_FALSE;
    m_pPSX->SetControlProperty(csReceiptSrc, UI::PROPERTYVISIBLE, vValue);
}

//************************************************************************ 
//
// Function:		ShowReport
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowReport(const CString& csReceiptSrc)
{
	COleVariant vValue = VARIANT_TRUE;
    m_pPSX->SetControlProperty(csReceiptSrc, UI::PROPERTYVISIBLE, vValue);
}

//************************************************************************ 
//
// Function:		ageFromDateTime
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
int PSProceduresBase::ageFromDateTime(COleDateTime& date)
{
	COleDateTime today = COleDateTime::GetCurrentTime();

	int iAge = today.GetYear() - date.GetYear();

	if(today.GetMonth() < date.GetMonth())
		iAge--;
	else
		if(today.GetMonth() == date.GetMonth())
			if(today.GetDay() < date.GetDay())
				iAge--;

	if(iAge < 0)
		iAge = 0;

	return iAge;
}


//************************************************************************ 
//
// Function:		DateFormat
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::DateFormat(void)
{
 CString date_format;												//TAR 330874
  switch (co.nLocaleShortDateOrdering)
  {
	case 0: // Month-Day-Year
	  if (co.nLocaleNoCenturyDigit)
	  {
		date_format = GetPSText( LTT_MONTH_DAY_YEAR );				//TAR 330874
	  }		
	  else
	  {
		date_format = GetPSText( LTT_MONTH_DAY_YEAR_NOCENTURY );	//TAR 330874
	  }		
	  break;
	case 1: // Day-Month-Year
	  if (co.nLocaleNoCenturyDigit)
	  {
		date_format = GetPSText( LTT_DAY_MONTH_YEAR );				//TAR 330874
	  }
	  else
	  {
		date_format = GetPSText( LTT_DAY_MONTH_YEAR_NOCENTURY );	//TAR 330874
	  }		
	  break;
	case 2: // Year-Month-Day
	  if (co.nLocaleNoCenturyDigit)
	  {
		date_format = GetPSText( LTT_YEAR_MONTH_DAY );				//TAR 330874
	  }
	  else
	  {
		date_format = GetPSText( LTT_YEAR_MONTH_DAY_NOCENTURY );	//TAR 330874
	  }		
	  break;
	default:
	  break;
  }
  return date_format;
}

//************************************************************************ 
//
// Function:		DateFormat
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::DateFormat(int month, int day, int year)
{
  CString date_format;
  CString year_format;
  
  int nNoCenturyDigit=0;

  if (co.nLocaleNoCenturyDigit==0) nNoCenturyDigit = 2;
  if (co.nLocaleNoCenturyDigit==1) nNoCenturyDigit = 4;

  
  // Format the number entered with slashes to look like a date.
  year_format.Format(_T("%04d"), year);
      
  switch (co.nLocaleShortDateOrdering)
  {
     case 0: // Month-Day-Year                
        date_format.Format(_T("%02d%s%02d%s%s"),                              
                              month,co.csLocaleDateSeparator,                              
                              day,co.csLocaleDateSeparator,		                       
                              year_format.Right(nNoCenturyDigit));

     break;
     case 1: // Day-Month-Year
        date_format.Format(_T("%02d%s%02d%s%s"),                              
                              day,co.csLocaleDateSeparator,		                       
                              month,co.csLocaleDateSeparator,		                
                              year_format.Right(nNoCenturyDigit));
     break;
     case 2: // Year-Month-Day
        date_format.Format(_T("%s%s%02d%s%02d"),                              
                              year_format.Right(nNoCenturyDigit),co.csLocaleDateSeparator,
                              month,co.csLocaleDateSeparator,		                
                              day);
     break;
     default:
     break;
  }      
  return date_format;
}

//************************************************************************ 
//
// Function:		ShowTotalSavings
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowTotalSavings(const long lTotalTrxSavings)
{
	lPSSavingsTotal = lTotalTrxSavings;
	if (!co.fCMTotalSavings) return;

	ShowSavingsORRewards(true, lTotalTrxSavings);	
}

//************************************************************************ 
//
// Function:		ShowTotalRewardPoints
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowTotalRewardPoints(const long lTotalTrxPoints)
{
	lPSPointsTotal = lTotalTrxPoints;
	if (!co.fCMTotalRewardPoints) return;

	ShowSavingsORRewards(false, lTotalTrxPoints);
}

//************************************************************************ 
//
// Function:		ShowCustomerRewardMessage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowCustomerRewardMessage(LPCTSTR pCustomerMessage)
{
	if (pCustomerMessage)
	{
		csPSCustomerRewardMessage = pCustomerMessage;
	}
	else
	{
		csPSCustomerRewardMessage.Empty();
	}

	COleVariant vValue = csPSCustomerRewardMessage;
	m_pPSX->SetTransactionVariable(_T("CMCustomerMessage"), vValue);
}
#endif //_CPPUNIT

//************************************************************************ 
//
// Function:		ShowSavingsORRewards
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowSavingsORRewards(bool bShowSavings, long lValue)
{
    COleVariant vValue;

    if (bShowSavings)
    {
        if (co.fCMTotalSavings)
            vValue = VARIANT_TRUE;
        else
            vValue = VARIANT_FALSE;
        m_pPSX->SetConfigProperty(_T("CMAmountSavedLabel"), _T(""), UI::PROPERTYVISIBLE, vValue);
        m_pPSX->SetConfigProperty(_T("CMAmountSaved"), _T(""), UI::PROPERTYVISIBLE, vValue);    

        vValue = (long)lValue;
        m_pPSX->SetTransactionVariable(_T("CMAmountSaved"), vValue);
    }
    else
    {
        if (co.fCMTotalRewardPoints)
            vValue = VARIANT_TRUE;
        else
            vValue = VARIANT_FALSE;
        m_pPSX->SetConfigProperty(_T("CMPointEarnedLabel"), _T(""), UI::PROPERTYVISIBLE, vValue);
        m_pPSX->SetConfigProperty(_T("CMPointEarned"), _T(""), UI::PROPERTYVISIBLE, vValue);

        vValue = (long)lValue;
        m_pPSX->SetTransactionVariable(_T("CMPointEarned"), vValue);
    }
}

//************************************************************************ 
//
// Function:		ShowCMRewardInfoBox
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowCMRewardInfoBox(const long lTotalTrxSavings,
										const long lTotalTrxPoints,
										LPCTSTR pCustomerMessage)
{
    //Show savings in CM box as positive dollar amount
    long lPositiveTotalTrxSavings;
    if (lTotalTrxSavings < 0)       
        lPositiveTotalTrxSavings = -lTotalTrxSavings;
    else 
        lPositiveTotalTrxSavings = lTotalTrxSavings;

    if (!co.fCMRewardInfo) return;

    if (lTotalTrxSavings != LONG_MIN)
    {
        lPSSavingsTotal = lPositiveTotalTrxSavings;  //Tar 200458
    }
    ShowSavingsORRewards(true, lPSSavingsTotal);

    if (lTotalTrxPoints != LONG_MIN)
    {
        lPSPointsTotal = lTotalTrxPoints;
    }
    ShowSavingsORRewards(false, lPSPointsTotal);
    
    CString csCustomerMessage = pCustomerMessage;
    if (csCustomerMessage != _T("DefaultIsDoNotResetMessage"))
    {
        csPSCustomerRewardMessage = csCustomerMessage; 
        
        COleVariant vValue = csPSCustomerRewardMessage;
        m_pPSX->SetTransactionVariable(_T("CMCustomerMessage"), vValue);
    }
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		DeleteRewardItem
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::DeleteRewardItem(long lRewardID)
{
	CString csRewardEntryId;
	CString csRewardID;	
	CStringArray csEntryIdList;

	csRewardID.Format(_T("%d"), lRewardID);
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csEntryIdList, _T(""), _T("ITEM_EXCEPTION_CATEGORY"), _T("RewardLine"));
	int nCountList = csEntryIdList.GetSize();
	
	bool bFound = false;
	for (int i=0; i<nCountList; i++)
	{
		csRewardEntryId = csEntryIdList.GetAt(i);
		if (csRewardID == csRewardEntryId) 
		{
			bFound = true;
			break;
		}
	}
    
	if (bFound)    
	{
        ps.TraceItemVariables(_T("CMSMReceipt"), csRewardEntryId);
        trace(L7, _T("Deleting reward item %s"), csRewardEntryId);
		m_pPSX->RemoveReceiptItem(_T("CMSMReceipt"), csRewardEntryId); //remove reward item
		m_pPSX->UpdateReceiptControls(_T("CMSMReceipt"));
        //sscop-3370+
        //remove reward item from picklistreceipt
   		m_pPSX->RemoveReceiptItem(_T("PickListReceipt"), csRewardEntryId); 
		m_pPSX->UpdateReceiptControls(_T("PickListReceipt"));
        //sscop-3370-
	}
	return 0;
}

//************************************************************************ 
//
// Function:		AddRewardItem
// Purpose:			
// Parameters:
// Returns:	

// Calls:
// Side Effects:
// Notes:
//

//************************************************************************
long PSProceduresBase::AddRewardItem(const CString& csReceiptSrc, CString& csDesc, long lRewardID, long lValue, long lPosType, long lEntryIdForReference)
{
		CString csRewardItemId;
		CString csItemEntryId = _T("");;
		COleVariant vValue;
		CString csCat = _T("RewardLine");

		//add reward under a specific item entryId
		if (lPosType != BOTTOM_OF_RECEIPT)
		{
			if (csReceiptSrc == _T("CMSmallReceipt"))
			{
				CStringArray csItemEntryIdList;
				m_pPSX->FindReceiptItems(_T("CMSmallReceipt"), csItemEntryIdList);
				int nCountList = csItemEntryIdList.GetSize(); 
				if (nCountList>0)
				{
					csItemEntryId = csItemEntryIdList.GetAt(0);
				}
			}
			else
			{
				FindRctIndexForReward(lEntryIdForReference, lPosType, csItemEntryId);
			}
			if (csItemEntryId.GetLength() && csReceiptSrc != _T("CMSmallReceipt"))
			{
				csRewardItemId.Format(_T("%d"), lRewardID);
				m_pPSX->CreateReceiptItem(csReceiptSrc, csRewardItemId, _T(""), csItemEntryId);
			}
			else
			{
				csRewardItemId.Format(_T("%d"), lRewardID);
				m_pPSX->CreateReceiptItem(csReceiptSrc, csRewardItemId);
			}
		}
		else
		{
			//if (csReceiptSrc == _T("CMSmallReceipt"))
				//m_pPSX->ClearReceipt(csReceiptSrc);
			csRewardItemId.Format(_T("%d"), lRewardID);
			m_pPSX->CreateReceiptItem(csReceiptSrc, csRewardItemId);
		}
		
		vValue = csDesc;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csRewardItemId, _T("ITEM_DESCRIPTION"), vValue); 
		
		if (lValue != LONG_MIN)
		{
			vValue = (long)lValue;
			m_pPSX->SetReceiptItemVariable(csReceiptSrc, csRewardItemId, _T("ITEM_EXT_PRICE"), vValue); 
		}
	
		vValue = VARIANT_TRUE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csRewardItemId, _T("ITEM_REWARD"), vValue);
		
		vValue = csCat;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csRewardItemId, _T("ITEM_EXCEPTION_CATEGORY"), vValue);
			
		vValue = VARIANT_FALSE;
		m_pPSX->SetReceiptItemVariable(csReceiptSrc, csRewardItemId, _T("ITEM_SELECTABLE"), vValue);
	
		m_pPSX->UpdateReceiptControls(csReceiptSrc);

	
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			ShowMoreInfoOnReceipt(_T("CMSmallReceipt"), _T("CMSmallReceiptMoreInfo"));
		}	  
		return 0;
}

//************************************************************************ 
//
// Function:		FindRctIndexFromRewardID
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::FindRctIndexFromRewardID(long lRefEntryId, CString& csItemEntryId)
{
	CStringArray csItemEntryIdList;
	COleVariant vValue;
	CString csTmpEntryId;
	
	csItemEntryId = _T("");
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize();
	CString csRefEntryId;
	csRefEntryId.Format(_T("%d"), lRefEntryId);
	
	for (int i=0; i<nCountList; i++)
	{
		csTmpEntryId = csItemEntryIdList.GetAt(i);
	
		if (csTmpEntryId == csRefEntryId) 
		{
			csItemEntryId = csTmpEntryId;
			break;
		}
	}
	return 0;
}

//************************************************************************ 
//
// Function:		FindNextRctIndexFromRewardID
// Purpose:			Finds the receipt index following a reference ID for
//                  insertafter purposes
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::FindNextRctIndexFromRewardID(long lRefEntryId, CString& csItemEntryId)
{
	CStringArray csItemEntryIdList;
	COleVariant vValue;
	CString csTmpEntryId;
	
	csItemEntryId = _T("");
	m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize();
	CString csRefEntryId;
	csRefEntryId.Format(_T("%d"), lRefEntryId);
	
	for (int i=0; i<nCountList; i++)
	{
		csTmpEntryId = csItemEntryIdList.GetAt(i);
	
		if (csTmpEntryId == csRefEntryId) 
		{
            if (i != nCountList -1)
            {
                csItemEntryId = csTmpEntryId = csItemEntryIdList.GetAt(i+1);
            }
			break;
		}
	}
	return 0;
}

//************************************************************************ 
// ADDED FOR TAR 333450
// Function:    FindRctIndexForReward
// Purpose:     Finds the receipt index before or after a reference ID for
//              insertafter purposes
// Parameters:
// Returns: 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::FindRctIndexForReward(long lRefEntryId, long lPosType, CString& csItemEntryId)
{
    CStringArray csItemEntryIdList;
    COleVariant vValue;
    CString csTmpEntryId;
    
    csItemEntryId = _T("");
    m_pPSX->FindReceiptItems(_T("CMSMReceipt"), csItemEntryIdList);
    int nCountList = csItemEntryIdList.GetSize();
    CString csRefEntryId;
    csRefEntryId.Format(_T("%d"), lRefEntryId);
    
    for (int i=0; i<nCountList; i++)
    {
       csTmpEntryId = csItemEntryIdList.GetAt(i);
    
       if (csTmpEntryId == csRefEntryId) 
       {
            if (ABOVE_REF_ENTRYID == lPosType)
            {
               csItemEntryId = csTmpEntryId;
            }
            else 
            {
               if (BELOW_REF_ENTRYID == lPosType)
               {
                   for (int j=1; j<=nCountList-i-1; j++)
                   {
                      CString csPrevEntryId;
                      PSXRC rc = PSX_SUCCESS;
                      COleVariant vValue;
                      csPrevEntryId = csTmpEntryId;
                      csTmpEntryId = csItemEntryIdList.GetAt(i+j);
					  //411047+
					  //ingore non item receipt entries
					  if ( (csTmpEntryId.Find(_T("RCM")) != -1 ) ||
						   (csTmpEntryId.Find(_T("ShowSkip")) != -1 ) ||
                           (csTmpEntryId.Find(_T("LINK")) != -1) )
					  {
						  continue;
					  }
					  //411047-
                      rc = m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csTmpEntryId, _T("ITEM_REWARD"), vValue);
                      if (   (rc != PSX_SUCCESS)
                          || (vValue.boolVal != VARIANT_TRUE) )
                      {  // Done when past all previous associated rewards
                         //csItemEntryId = csPrevEntryId;
						  csItemEntryId = csTmpEntryId; //411047: rewardline is going to be added above csItementryId 

                         break;
                      }
                   }
               }
            }
            break;
        }
    }
    return 0;
}

//************************************************************************ 
//
// Function:		SetAttractScreenMessage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetAttractScreenMessage(LPCTSTR szText)
{
	COleVariant v;
	v = ConvertToEscapeSequence(szText, _T("\\n"), _T("\n"));	// Tar 227220 RJF 021103
    m_pPSX->SetConfigProperty(_T("CMScreenText"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
}

//************************************************************************ 
//
// Function:		SetAlphaNumericMessage
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetAlphaNumericMessage(LPCTSTR szText)
{
	COleVariant v;
	v=szText;
    m_pPSX->SetConfigProperty(_T("LeadthruText"), m_csCurrentContext, UI::PROPERTYTEXTFORMAT, v);
}

//************************************************************************ 
//
// Function:		DisconnectFromServer
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
BOOL PSProceduresBase::DisconnectFromServer( BOOL bSendDisconnectMsg )
{
    ra.ReleaseExclusiveAccess();
	if( bSendDisconnectMsg )
    {
        // Only DisconnectRemote if bSendDisconnectMsg. Not needed if !bSendDisconnectMsg because connection already down. //
        ra.DisconnectFromServer( );
    }
    dm.SoundDisconnectFromServer( bSendDisconnectMsg );
	
    return 0;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		RemoteMode
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
BOOL PSProceduresBase::RemoteMode()
{
	bool bRemote = ra.IsRemoteMode();
    return (bRemote ? TRUE : FALSE);
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetServerName
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetServerName(LPCTSTR serverName)
{   
	m_csServerName = serverName;

}
#endif //_CPPUNIT

//************************************************************************ 
//
// Function:		GetSelection
// Purpose:			Get the selected receipt item entry id
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetSelection(const CString& csReceipt)
{
	CString csItemEntryId = _T("");
	m_pPSX->GetSelectedReceiptItem(csReceipt, csItemEntryId);
	return csItemEntryId;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetSelection
// Purpose:			Select the receipt item with the id specified
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
PSXRC PSProceduresBase::SetSelection(const CString& csReceipt, const CString& csItemEntryId)
{
	PSXRC rc = m_pPSX->SetSelectedReceiptItem(csReceipt, csItemEntryId);
	return rc;
}

//************************************************************************ 
//
// Function:		CheckForScrolling
// Purpose:			Check if there is any selectable items below or above 
//                  the current item
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::CheckForScrolling(const CString& csReceiptSrc, const CString& csReceipt, const CString& csScrollMode)
{
	CStringArray csItemEntryIdList;
	CString csItemEntryId = _T("");
	CString csTmpItemEntryId = _T("");

	m_pPSX->GetSelectedReceiptItem(csReceipt, csItemEntryId);
	m_pPSX->FindReceiptItems(csReceiptSrc, csItemEntryIdList, _T(""), _T("ITEM_SELECTABLE"), VARIANT_TRUE);
	int nCountList = csItemEntryIdList.GetSize();
	int nItemsFound = 0;
	bool bScroll = false;
	for (int i=0; i<nCountList; i++)
	{
		csTmpItemEntryId = csItemEntryIdList.GetAt(i);
		if (csTmpItemEntryId == csItemEntryId)
		{
			nItemsFound = i + 1;
		}
	}


	if ((csScrollMode == _T("Up") && nItemsFound>1) || (csScrollMode == _T("Down") && nItemsFound<nCountList))
		bScroll = true;

	return bScroll;
}

#endif //CPPUNIT
//************************************************************************ 
//
// Function:		ShowMoreInfoOnReceipt
// Purpose:			Show "More Info" on small receipt
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::ShowMoreInfoOnReceipt(const CString& csReceipt, const CString& csControl)
{
#ifndef _CPPUNIT
  	COleVariant vValue, vReturnedCount, vIndex;

    m_pPSX->SendCommand(csReceipt, UI::COMMANDGETCOUNT, vReturnedCount);
	int nCountList = vReturnedCount.lVal;
	int lTextHeight = 0;
	for (int i = 0; i<nCountList; i++)
	{
		vIndex = (long)i;
        m_pPSX->SendCommand(csReceipt, UI::COMMANDGETITEMHEIGHT, vValue, 1, vIndex);
		vValue.ChangeType(VT_INT);
		long lItemHeight = vValue.lVal;
		lTextHeight += lItemHeight;
	}
		
    m_pPSX->GetControlProperty( csReceipt, UI::PROPERTYPOSITION, vValue);

	_bstr_t bstr = vValue.bstrVal;

	//convert bstr to RECT
	RECT rcRect = { 0 };	
	WCHAR *psz = wcstok( bstr, OLESTR(",") );
	if( psz )
	{
		rcRect.left = _wtol( psz );
	}		
	psz = wcstok( NULL, OLESTR(",") );
	if( psz )
	{
		rcRect.top = _wtol( psz );
	}
	psz = wcstok( NULL, OLESTR(",") );
	if( psz )
	{
		rcRect.right = rcRect.left + _wtol( psz );
	}
	psz = wcstok( NULL, OLESTR(",") );
	if( psz )
	{
		rcRect.bottom = rcRect.top + _wtol( psz );
	}
		
	long lBottom = rcRect.bottom;
	long lTop = rcRect.top;
	long lRectHeight = lBottom - lTop;

	if (lTextHeight>lRectHeight)
	{
		vValue = VARIANT_TRUE;
	}
	else
	{
		vValue = VARIANT_FALSE;
	}
    m_pPSX->SetConfigProperty(csControl, _T(""), UI::PROPERTYVISIBLE, vValue);  
#endif // _CPPUNIT
}

//************************************************************************ 
//
// Function:		GetControlHandle
// Purpose:			Get Window control handle

// Parameters:
// Returns:	
// Calls:

// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetControlHandle(HWND& hControlWnd)
{
	COleVariant vValue;
    if (!m_SigCapWindow)
    {
        m_pPSX->GetControlProperty(_T("CMSigCap"), UI::PROPERTYHWND, vValue );
        hControlWnd = (HWND)vValue.lVal;
        m_SigCapWindow = hControlWnd;
        m_bUsingPSXSigCapWindow = true;
    }
    else
    {
        hControlWnd = m_SigCapWindow;
    }

    if (!hControlWnd)
    {
        
        WNDCLASS wc = {0};
        wc.style = CS_PARENTDC;
        wc.lpfnWndProc = ::DefWindowProc;
        wc.hInstance = AfxGetApp()->m_hInstance;
        wc.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wc.lpszClassName = _T("SigCap");
        wc.hbrBackground = NULL;	    
        RegisterClass(&wc);
        
        hControlWnd = CreateWindowEx(0, _T("SigCap"), _T(""), WS_POPUP | WS_CLIPCHILDREN, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
        m_SigCapWindow = hControlWnd;
        m_bUsingPSXSigCapWindow = false;
    }

	return 0;
}

//************************************************************************ 
//
// Function:		LoadStringCallbackHandler
// Purpose:			

// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
BOOL LoadStringCallbackHandler( UINT nId, CString& strReturnedString )
{
	CString csTemp;
	
	// +tar381634
	if(ps.UseCustomerLanguageForLoadString())
	{
		csTemp = ps.GetPSText(nId, SMStateBase::m_customerLanguage); // tar381634
	}
	else
	{
		csTemp = ps.GetPSText(nId);
	}
	// -tar381634

	csTemp = ps.ConvertToEscapeSequence(csTemp, _T("\\t"), _T("\t"));
	csTemp = ps.ConvertToEscapeSequence(csTemp, _T("\\n"), _T("\n"));
	strReturnedString = csTemp;
	return true;
} 


#ifndef _CPPUNIT

//************************************************************************ 
//
// Function:		ExternalUIRequestExec
// Purpose:		    This is the main function that translates TB requests for UI change into PSX calls		
// Parameters:		[in/out] pUIRequest points to the UI command structure which holds both arguments and results of the command
// Returns:			returns PSX_SUCCESS if TB requested operations are successfully executed, otherwise the first actual failed return value from a PSX call
// Calls:			based on the TB request type, it calls appropriate ProcessPIM*() functions implemented in this file/class
// Side Effects:    pUIRequest may have received the results of the requested operations
// Notes:			After this call, App calls TBProcessMessage with the same argument to let TB use the results
//
//************************************************************************
long PSProceduresBase::ExternalUIRequestExec(PS_UI_CMD pUIRequest)
{
    long lRetVal = 0;
	PS_UI_CMD ptrUIRequest = pUIRequest;
	CString csReceiptSrc;
	CString csItemEntryId;
	long rc = (long) PSX_E_FAIL;
	while (ptrUIRequest)
	{
		switch (ptrUIRequest->eStructType)
		{
			case UI_STR_RECEIPT:
			{
				csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
				csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
				CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
				if (csReceiptSrc.IsEmpty() || csItemEntryId.IsEmpty())
				{
					break;
				}
				switch (ptrUIRequest->Rcpt.eItemCmd)
				{
					case UI_ITEM_CMD_READVARS:
					{
						rc = ProcessPIMReadReceiptVars(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_UPDATE:
					{
						rc = ProcessPIMItemUpdate(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_INSERT:
					{
						rc = ProcessPIMItemInsert(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_REMOVEITEM:
					{
						rc = m_pPSX->RemoveReceiptItem(csReceiptSrc, csItemEntryId); //remove reward item
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_REMOVEVARS:
					{
						rc = ProcessPIMRemoveVars(ptrUIRequest);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_CLEARRCPT:
					{
						rc = m_pPSX->ClearReceipt(csReceiptSrc);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_CLEARRCPTITEMVARS:
					{
						rc = m_pPSX->ClearReceiptItemVariables(csReceiptSrc, csItemEntryId, true);
						ptrUIRequest->lStatus = rc;
					}
					break;
					case UI_ITEM_CMD_LASTINLIST:
					default:
					break;
				}
			}
			break;
			case UI_STR_FINDITEM:
			{
				rc = ProcessPIMFindItem(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SELECTEDITEM:
			{
				rc = ProcessPIMSelectItem(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SCROLL:
			{
				rc = ProcessPIMScroll(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_TRANSVAR:
			{

				ProcessPIMTransactionVars(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_CONTEXT:
			{
				rc = ProcessPIMContext(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_SEND_EVENT:
			{
				rc = ProcessPIMSendEvent(ptrUIRequest);
				ptrUIRequest->lStatus = rc;
			}
			break;
			case UI_STR_LASTINLIST:
			default:
			break;
		} // end of switch (ptrUIRequest->eStructType)

		ptrUIRequest = ptrUIRequest->pNextUICommand;
	} // end of while (ptrUIRequest)
	if (!csReceiptSrc.IsEmpty())
	{
		if (csReceiptSrc == _T("CMSmallReceipt"))
		{
			ShowMoreInfoOnReceipt(_T("CMSmallReceipt"), _T("CMSmallReceiptMoreInfo"));
		}
		else
		{
			m_pPSX->UpdateReceiptControls(csReceiptSrc);
		}
	}
    return lRetVal;
} // end of PSProceduresBase::ExternalUIRequestExec()


//************************************************************************ 
//
// Function:		AddReceiptItemFromUIRequest
// Purpose:			Handle external UI request(insert receipt item command from TB) to add receipt item
// Parameters:		
//			csReceiptSrc			---> receipt source name
//			csRewardItemId			---> item identifier(unique withing the receipt source)
//			csEntryIdForReference   ---> add receipt item with reference to this reference-item id
// Returns:	whether item is added or not
// Calls:   CreateReceiptItem, FindRctIndexFromRewardID
// Side Effects: 
// Notes:
//
//************************************************************************
long PSProceduresBase::AddReceiptItemFromUIRequest(const CString& csReceiptSrc, CString csRewardItemId, CString csEntryIdForReference)
{
		CString csItemEntryId = _T("");;
		COleVariant vValue;
		CString csCat = _T("RewardLine");
		PSXRC rc = PSX_E_FAIL;

		//add reward under a specific item entryId
		if (!csEntryIdForReference.IsEmpty())
		{
			rc = m_pPSX->CreateReceiptItem(csReceiptSrc, csRewardItemId, csEntryIdForReference);
		}
		else
		{
			//csRewardItemId += _T("+"); 
			rc = m_pPSX->CreateReceiptItem(csReceiptSrc, csRewardItemId);

		}
		return rc;
}  // end of function: AddReceiptItemFromUIRequest()


//************************************************************************ 
//
// Function:		FindReceiptItemWithMultiple search criteria
// Purpose:			Find all items from current receipt-item list that match the search criteria
// Parameters:		[in]  receipt name and find criteria structure
// Parameters:		[out] find criteria structure that can store matched item-id values
// Returns:			PSX return code, PSX_SUCCESS if every criteria is successfully read, otherwise the first failure return value from PSX calls
// Calls:			FindReceiptItems       - retrieve items from the receipt
//					GetReceiptItemVariable - get the value of the specified 
//										     receipt item variable
// Side Effects:  if matching items are found, their ids are saved in the UI command and passed back to the caller
// Notes: enumerate items in the receipt source and search for items that match the criteria given by TB
//
//************************************************************************
long PSProceduresBase::FindReceiptItemWithMultipleCriteria(const CString& csReceiptName, S_UI_FindItem&  sUIFindCriteria) 
{
	CStringArray csItemEntryIdList, csMatchedList;
	m_pPSX->FindReceiptItems(csReceiptName, csItemEntryIdList);
	int nCountList = csItemEntryIdList.GetSize();
	int i = 0;
	PSXRC rc = PSX_SUCCESS;
	PSXRC finalrc = PSX_SUCCESS;

	CString csItemEntryId;

	COleVariant vValue;
	vValue.Clear();

	while (i < nCountList)
	{
		csItemEntryId = csItemEntryIdList.GetAt(i);

		int iUpdateVarNum = 0;
		PS_UI_VAR ptrUpdateVarCmd = NULL;
		bool bMatchingFound = false;
		for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
		{
			ptrUpdateVarCmd = sUIFindCriteria.pUiVars[iUpdateVarNum];
			if (ptrUpdateVarCmd)
			{
				rc = m_pPSX->GetReceiptItemVariable(csReceiptName, csItemEntryId, ptrUpdateVarCmd->lpcVarName, vValue);
				if (rc != PSX_SUCCESS)
				{
					finalrc = rc;
				}
				if (COleVariant(ptrUpdateVarCmd->vVarValue) == vValue)
				{
					// this criteria matched, continue to verify other criteria
					bMatchingFound = true;
				}
				else
				{
					// no need to check against other search criteria, failed to match
					bContinue = false; 
					bMatchingFound = false;
				}
			}
			else
			{
				// break from this loop
				// no more request entries for this receipt item
				bContinue = false;
			}
			vValue.Clear();
		} // end of for loop, this loop iterates on search criteria

		if (bMatchingFound)
		{
			trace(L0, _T("matching item found with item id:%s"), csItemEntryId);
			csMatchedList.Add(csItemEntryId);
		}
		++i;
	} // end of while loop, loop through receipt items

	int nMatchedItems = csMatchedList.GetSize();
	if (nMatchedItems)
	{
		// copy the list of matched ids to the TB structure
		sUIFindCriteria.ItemIdArray = (LPTSTR *) new LPTSTR[nMatchedItems];
		int i = 0;
		sUIFindCriteria.nArrSize = 0;
		while (i < nMatchedItems)
		{
			sUIFindCriteria.ItemIdArray[i] = NULL;
			CopySCOTString(sUIFindCriteria.ItemIdArray[i], csMatchedList[i]);
			++sUIFindCriteria.nArrSize;
			++i;
		}
	}
	return rc;
}  // end of function: FindReceiptItemWithMultipleCriteria()



//************************************************************************ 
//
// Function:		ProcessPIMFindItem
// Purpose:			Execute TB requested UI command(Find) and pass back the results to TB
// Parameters:      [in/out]  UI command received externally
// Returns:			returns PSX_SUCCESS if atleast one item found, otherwise returns PSX_E_INVALIDPARAM 
// Calls:			FindReceiptItemWithMultipleCriteria
// Side Effects:    argument will have the item-ids if matching items are found
// Notes:			The workings of this function are implemented in FindReceiptItemWithMultipleCriteria
//
//************************************************************************
long PSProceduresBase::ProcessPIMFindItem(PS_UI_CMD ptrUIRequest)
{
	//
	PSXRC rc = PSX_SUCCESS;
	CString csReceiptSrc(ptrUIRequest->Find.lpcRcptStructName);
	if (!csReceiptSrc.IsEmpty())
	{
		rc = (PSXRC) FindReceiptItemWithMultipleCriteria(csReceiptSrc, ptrUIRequest->Find); 
	}
	else
	{
		rc = PSX_E_INVALIDPARAM;
	}
	return rc;
} // end of function: ProcessPIMFindItem()




//************************************************************************ 
//
// Function:		ProcessPIMReadReceiptVars
// Purpose:			Execute TB requested UI command(ReadReceiptVars) and pass back the results to TB
// Parameters:    [in/out]  UI command received externally
// Returns:			returns PSX_SUCCESS if all var operations are successful, otherwise returns PSX_E_FAIL 
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMReadReceiptVars(PS_UI_CMD ptrUIRequest)
{
	int iUpdateVarNum = 0;
	PSXRC rc, finalrc;
	rc = PSX_SUCCESS;
	finalrc = PSX_SUCCESS;
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	COleVariant vValue;
	vValue.Clear();
	CString csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
	CString csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
	CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Rcpt.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{
			rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName, vValue);
			ptrUpdateVarCmd->vVarValue = vValue.Detach();
			ptrUpdateVarCmd->lOpStatus = rc;
			vValue.Clear();
			if (rc != PSX_SUCCESS)
			{
				finalrc = rc;
			}
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return finalrc;
} // end of function: ProcessPIMReadReceiptVars


//************************************************************************ 
//
// Function:	  ProcessPIMItemUpdate

// Purpose:		  Execute TB requested UI command(ReadReceiptVars) and pass back the results to TB
// Parameters:    [in/out]  UI command received externally
// Returns:		  returns PSX_SUCCESS if all var operations are successful, otherwise returns PSX_E_FAIL 
// Calls:		  SetReceiptItemVariable, GetReceiptItemVariable
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMItemUpdate(PS_UI_CMD ptrUIRequest)
{
	int iUpdateVarNum = 0;
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	PSXRC rc = PSX_SUCCESS;
	PSXRC finalrc = PSX_SUCCESS;
	COleVariant vValue;
	vValue.Clear();
	CString csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
	CString csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
	CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Rcpt.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{
			if (ptrUpdateVarCmd->eAccessCmd == UI_ACCESS_SET)
			{
				rc = m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName, ptrUpdateVarCmd->vVarValue);
			}
			else if (ptrUpdateVarCmd->eAccessCmd == UI_ACCESS_GET)
			{
				rc = m_pPSX->GetReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName, vValue);
				if (rc == PSX_SUCCESS)
				{
					ptrUpdateVarCmd->vVarValue = vValue.Detach();
				}
				ptrUpdateVarCmd->lOpStatus = rc;
				vValue.Clear();
			}
			ptrUpdateVarCmd->lOpStatus = rc;
			if (rc != PSX_SUCCESS)
			{
				finalrc = rc;
			}
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return finalrc;
}

//************************************************************************ 
//
// Function:	  ProcessPIMItemInsert
// Purpose:		  insert or add any requested receipt item that is new
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:		  SetReceiptItemVariable - set properties of the newly added item
//				  AddReceiptItemFromUIRequest() to insert the requested receipt item at the requested position
// Side Effects:  receipt will get new item added
// Notes:		  if there is no reference id is given, item is added at the bottom of the receipt
//
//************************************************************************
long PSProceduresBase::ProcessPIMItemInsert(PS_UI_CMD ptrUIRequest)
{
	CString csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
	CString csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
	CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
	PSXRC rc = PSX_SUCCESS;
	PSXRC finalrc = PSX_SUCCESS;
	rc = (PSXRC) AddReceiptItemFromUIRequest(csReceiptSrc, csItemEntryId, csItemEntryRefId);
	int iUpdateVarNum = 0;
	if (rc != PSX_SUCCESS)
	{
		trace(L0, _T("failed to insert line, rcptsrc:%s, entryid:%s, refentryid:%s"), csReceiptSrc, csItemEntryId, csItemEntryRefId);
		iUpdateVarNum = QUERY_MAX; // do not process any further
		finalrc = rc;
	}

	// set new receipt line properties
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Rcpt.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{

			rc = m_pPSX->SetReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName, ptrUpdateVarCmd->vVarValue);
			ptrUpdateVarCmd->lOpStatus = rc;
			if (rc != PSX_SUCCESS)
			{
				finalrc = rc;
			}
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return finalrc;
}


//************************************************************************ 
//
// Function:		ProcessPIMRemoveVars
// Purpose:			Remove receipt variable for a given receipt item requested by TB
// Parameters:    [in/out]  UI command received externally
// Returns:			operation status
// Calls:			RemoveReceiptItemVariable
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMRemoveVars(PS_UI_CMD ptrUIRequest)
{
	//
	int iUpdateVarNum = 0;
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	PSXRC rc = PSX_SUCCESS;
	CString csReceiptSrc =  (ptrUIRequest->Rcpt.lpcRcptStructName);
	CString csItemEntryId = (ptrUIRequest->Rcpt.lpcItemId);
	CString csItemEntryRefId(ptrUIRequest->Rcpt.lpcRefItemId);
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Rcpt.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{
			rc = m_pPSX->RemoveReceiptItemVariable(csReceiptSrc, csItemEntryId, ptrUpdateVarCmd->lpcVarName);
			ptrUpdateVarCmd->lOpStatus = rc;
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return rc;
}


//************************************************************************ 
//
// Function:		ProcessPIMSelectItem
// Purpose:			
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMSelectItem(PS_UI_CMD ptrUIRequest)
{
	PSXRC rc = PSX_SUCCESS;
	CString csReceipt(ptrUIRequest->Sel.lpcRcptStructName);
	if (csReceipt.IsEmpty())
	{
		rc = PSX_E_INVALIDPARAM;
		ptrUIRequest->lStatus = rc;
		return rc; 
	}

	CString csItemEntryId; 
	if (ptrUIRequest->Sel.eAccessCmd == UI_ACCESS_GET)
	{
		CString csItemEntryId = GetSelection(csReceipt); // receive the item id of current selection
		if (!csItemEntryId.IsEmpty())
		{
			if (ptrUIRequest->Sel.lpcItemId)
			{
				try
				{
					delete[] ptrUIRequest->Sel.lpcItemId;
				}catch(...)
				{
					// TB supposed to delete and reset this field but App still finds the field non-empty
					trace(L0, _T("Handled exception while deleting ptrUIRequest->Sel.lpcItemId"));
				}
			}
			CopySCOTString(ptrUIRequest->Sel.lpcItemId, csItemEntryId);
		}
		else
		{
			rc = PSX_E_FAIL;
		}
	}

	else if (ptrUIRequest->Sel.eAccessCmd == UI_ACCESS_SET)
	{
		// change the current selection
		CString csItemEntryId = ptrUIRequest->Sel.lpcItemId;
		if (!csItemEntryId.IsEmpty())
		{
			rc = SetSelection(csReceipt, csItemEntryId);
		}

	}
	return rc;
}


//************************************************************************ 
//
// Function:		ProcessPIMTransactionVars
// Purpose:			
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMTransactionVars(PS_UI_CMD ptrUIRequest)
{
	int iUpdateVarNum = 0;
	PSXRC rc = PSX_SUCCESS;
	PSXRC finalrc = PSX_SUCCESS;
	PS_UI_VAR ptrUpdateVarCmd = NULL;
	COleVariant vValue;
	vValue.Clear();
	for (bool bContinue = true ; ((bContinue) && (iUpdateVarNum < QUERY_MAX)); ++iUpdateVarNum)
	{
		ptrUpdateVarCmd = ptrUIRequest->Trans.pUiVars[iUpdateVarNum];
		if (ptrUpdateVarCmd)
		{
			if (ptrUpdateVarCmd->eAccessCmd == UI_ACCESS_GET)
			{
				rc = m_pPSX->GetTransactionVariable(ptrUpdateVarCmd->lpcVarName, vValue);
				ptrUpdateVarCmd->vVarValue = vValue.Detach();
				vValue.Clear();
			}
			else if (ptrUpdateVarCmd->eAccessCmd == UI_ACCESS_SET)
			{
				rc = m_pPSX->SetTransactionVariable(ptrUpdateVarCmd->lpcVarName, ptrUpdateVarCmd->vVarValue);
			}
			ptrUpdateVarCmd->lOpStatus = rc;
			if (rc != PSX_SUCCESS)
			{
				finalrc = rc;
			}
		}
		else
		{
			// break from this loop
			// no more request entries for this receipt item
			bContinue = false;
		}
	}
	return finalrc;
}

//************************************************************************ 
//
// Function:		ProcessPIMScroll
// Purpose:			
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMScroll(PS_UI_CMD ptrUIRequest)

{
	PSXRC rc = PSX_SUCCESS;
	return rc;
}

//************************************************************************ 
//
// Function:		ProcessPIMContext
// Purpose:			
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMContext(PS_UI_CMD ptrUIRequest)
{
	PSXRC rc = PSX_SUCCESS;
	return rc;
}

//************************************************************************ 
//
// Function:		ProcessPIMSendEvent
// Purpose:			
// Parameters:    [in/out]  UI command received externally
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ProcessPIMSendEvent(PS_UI_CMD ptrUIRequest)
{
    PSXRC rc = PSX_SUCCESS;
    CString csContext = ptrUIRequest->Send.lpcContextName;
    CString csControl = ptrUIRequest->Send.lpcControlName;
    trace(L6,_T("ProcessPIMSendEvent %d, %s, %s"),
        ptrUIRequest->Send.lEventCode, csContext, csControl);
    if (csContext == _T(""))
    {
        csContext = m_csCurrentContext;
    }
    switch (ptrUIRequest->Send.lEventCode)
    {
    case UI_EVENT_CHANGESTATE:
        {
            COleVariant vState = ptrUIRequest->Send.vEventData;
            rc = m_pPSX->SetConfigProperty(csControl, csContext, UI::PROPERTYSTATE, vState);
            trace(L6,_T("ProcessPIMSendEvent UI_EVENT_CHANGESTATE processing returning %x"), rc);
        }
        break;
    case UI_EVENT_CHANGEVISIBLE:
        {
            COleVariant v = ptrUIRequest->Send.vEventData;
            rc = m_pPSX->SetConfigProperty(csControl, csContext, UI::PROPERTYVISIBLE, v);
            trace(L6,_T("ProcessPIMSendEvent UI_EVENT_CHANGEVISIBLE processing returning %x"), rc);
        }
        break;
    case UI_EVENT_BUTTONCLICK:
        {
            COleVariant v = _T("");
            rc = m_pPSX->GenerateEvent(csControl, csContext, UI::EVENTCLICK, v);
            trace(L6,_T("ProcessPIMSendEvent UI_EVENT_BUTTONCLICK processing returning %x"), rc);
        }
        break;
    default:
        trace(L6,_T("ProcessPIMSendEvent unrecognized event code"));
    }
    
    return rc;
}

//************************************************************************ 
//
// Function:		GetPickQtyList
// Purpose:			Get quantity items in PickList		
// Parameters:		
// Returns:			A pointer to the pick quantity item list
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
CStringArray* PSProceduresBase::GetPickQtyList()
{
	return &m_csPickQtyList;
}
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		ConvertSCOTLangID2LCID
// Purpose:			Convert ISO code to LCID code		
// Parameters:		csISOCode - ISO language code
//					csLCIDCode - LCID language code
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::ConvertSCOTLangID2LCID(const CString& csISOCode, CString& csLCIDCode)
{
	if (csISOCode == _T("EN"))
		csLCIDCode = _T("0409");
	else if (csISOCode == _T("UK"))
		csLCIDCode = _T("0809");
	else if (csISOCode == _T("FR"))
		csLCIDCode = _T("0C0C");
	else if (csISOCode == _T("ES"))
		csLCIDCode = _T("080A");
	else
		csLCIDCode = csISOCode;

	return 0;
}
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetCategoryListText
// Purpose:			Set ButtonList control's button text
// Parameters:      csCtlName - ButtonList control name
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::SetCategoryListText(const CString& csCtlName, const CString& csItemCode)
{
	USES_CONVERSION;
   
	trace(L7, _T("+PSProceduresBase::SetCategoryListText, csCtlName=%s"), csCtlName);

	long nButtonCount;
    COleVariant v;
    COleVariant vReturn;

    CPSXRedrawLock csRedrawLock(csCtlName, _T("PSProceduresBase::SetCategoryListText"));

    bool bIsMainCategory = false;
    bool bFindSubCategory = false;
    bool bHasSubCategory = false;
    int nTabIndex(-1);
    
    CString csCategories(_T(""));
    CString csCategory(_T(""));
    CString csCatAlphabetic;
    CString csCategoryName(_T(""));
    CString csSubCategoryID(_T(""));

    if(!csItemCode.IsEmpty())
    {

        bIsMainCategory = FindCategory(csItemCode, csCatAlphabetic, csCategory, nTabIndex);

        csCategoryName = GetCategoryName(csCategory);

        bHasSubCategory = HasSubCategory(csCategory, csSubCategoryID);

        //if not a main category, try check subcategory
        if(!bIsMainCategory)
        {
            bFindSubCategory = FindSubCategory(csItemCode);
        }
    }

    if(co.fOperationsDisplayPickListSubCategoriesInTab && ((bIsMainCategory && bHasSubCategory) || bFindSubCategory ))
    {
        trace(L6, _T("PSProceduresBase::SetCategoryListText, showing SubCategories at Tab "));

        SetDisplayTabIsMainCategories(false);

        if(bFindSubCategory)
        {
            csCategory = GetFirstLevelCategoryID();
        }

        nButtonCount = SubCategoryList[m_languageUsed].GetCount();

        CString csText( _T("") );

        CStringArray csSATextStringArray;
        CStringArray csSACategoryIdStringArray;

        CCategoryInfo *pItem = NULL;
        CSCategoryList* pCatList = NULL;

        pCatList = &(SubCategoryList[m_languageUsed]);

        CString csCat = _T("");

        long nSubCategoriesCount = 0;

        for( long i = 0; i < nButtonCount; i++ )
        {
            pItem = (CCategoryInfo*)pCatList->GetAt( pCatList->FindIndex( i ) );
            csText = pItem->GetCatDescription();

            CString csCatId = pItem->GetCategoryID();
            CString csAlphabetic = pItem->GetCatAlphabetic();
            CString csInCategories = pItem->GetCategories();

            if ( !csInCategories.IsEmpty() )
            {   
                csCat = GetCategory(csInCategories, csCategory);
            }

            if( csCat == csCategory )
            {
                csSATextStringArray.Add(csText);
    
                if( csCatId != TAB_FAV )
                {
                    csText = csAlphabetic.GetLength() ? csAlphabetic : TAB_CATEGORY + csCatId;
                }
                else
                {
                    csText = TAB_FAV;
                }

                csSACategoryIdStringArray.Add(csText);

                nSubCategoriesCount++;

            }
        }

        COleSafeArray saTextArray; //list of category text
        COleSafeArray saCategoryIdArray; //list of category Ids

        saTextArray.Create(VT_BSTR, 1, (DWORD*)&nSubCategoriesCount);
        saCategoryIdArray.Create(VT_VARIANT, 1, (DWORD*)&nSubCategoriesCount);

        for(long in = 0; in < nSubCategoriesCount; in++)
        {
            csText = csSATextStringArray.GetAt(in);
            // +TAR 314786
            BSTR bstrText = csText.AllocSysString();
            saTextArray.PutElement(&in, bstrText );
            ::SysFreeString(bstrText);
            // -TAR 314786

            csText = csSACategoryIdStringArray.GetAt(in);

            v = csText;
            saCategoryIdArray.PutElement(&in, &v );

        }

        v = nSubCategoriesCount;
        m_pPSX->SetConfigProperty( csCtlName, _T(""), UI::PROPERTYBUTTONCOUNT, v );
        m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );


        v = saTextArray;
        m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
        v = saCategoryIdArray;
        m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
    }
    else
    {
        trace(L6, _T("PSProceduresBase::SetCategoryListText, showing MainCategories at Tab "));

        SetDisplayTabIsMainCategories(true);

        nButtonCount = MainCategoryList[m_languageUsed].GetCount();
        v = nButtonCount;
        m_pPSX->SetConfigProperty( csCtlName, _T(""), UI::PROPERTYBUTTONCOUNT, v );
        m_pPSX->SetControlProperty( csCtlName, UI::PROPERTYBUTTONCOUNT, v );

        CString csText( _T("") );
        COleSafeArray saTextArray; //list of category text
        COleSafeArray saCategoryIdArray; //list of category Ids

        saTextArray.Create(VT_BSTR, 1, (DWORD*)&nButtonCount);
        saCategoryIdArray.Create(VT_VARIANT, 1, (DWORD*)&nButtonCount);

        CCategoryInfo *pItem = NULL;
        CSCategoryList* pCatList = NULL;

        pCatList = &(MainCategoryList[m_languageUsed]);

        for( long i = 0; i < nButtonCount; i++ )
        {
            pItem = (CCategoryInfo*)pCatList->GetAt( pCatList->FindIndex( i ) );
            csText = pItem->GetCatDescription();

            CString csCatId = pItem->GetCategoryID();
            CString csAlphabetic = pItem->GetCatAlphabetic();

            // +TAR 314786
            BSTR bstrText = csText.AllocSysString();
            saTextArray.PutElement(&i, bstrText );
            ::SysFreeString(bstrText);
            // -TAR 314786
    
            if( csCatId != TAB_FAV )
            {
                csText = csAlphabetic.GetLength() ? csAlphabetic : TAB_CATEGORY + csCatId;
            }
            else
            {
                csText = TAB_FAV;
            }
            v = csText;

            saCategoryIdArray.PutElement(&i, &v );
        }

        v = saTextArray;
        m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );
        v = saCategoryIdArray;
        m_pPSX->SendCommand( csCtlName, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
    }


	trace(L7, _T("-PSProceduresBase::SetCategoryListText"));

	return 0;
}

//************************************************************************ 
//
// Function:		SetItemDesc
// Purpose:			Stores the Item Description
// Parameters:		[IN] CString csItemDesc
// Returns:	        NOTHING
// Calls:           
// Side Effects:
// Notes:			Stores the value to mItemDescBuffer
//
//************************************************************************
void PSProceduresBase::SetItemDesc( CString csItemDesc )
{
	mItemDescBuffer = _T("");
	mItemDescBuffer = csItemDesc;
}


//************************************************************************ 
//
// Function:		GetItemDesc
// Purpose:			Retrieves the Item Description
// Parameters:		NONE
// Returns:	        The Item Description
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetItemDesc()
{
	return mItemDescBuffer;
}
#endif // _CPPUNIT
//************************************************************************ 
//
// Function:		TraceItemVariables
// Purpose:			Traces the receipt variables that have been created
//                  for a specific item/receipt combination
// Parameters:		csReceiptSrc - the receipt being traced
//                  csItemEntryId - the item being traced
// Returns:	        nothing
// Calls:           
// Side Effects:
// Notes:           Trace level must be 7 or above to get the trace
//
//************************************************************************
void PSProceduresBase::TraceItemVariables(CString csReceiptSrc, CString csItemEntryId)
{
    if (co.GetnTracingCurrentLevel() >=7)
    {
        CStringArray csVariableList;
        m_pPSX->GetReceiptItemVariableList(csReceiptSrc, csItemEntryId, csVariableList);
        
        CString csTemp;      
        
        for (int i=0; i< csVariableList.GetSize(); i++)
        {
            if (i>0)
            {
                csTemp += _T(", ");
            }
            csTemp += csVariableList.GetAt(i);
        }
        trace(L7, _T("Receipt %s, Item %s - Variables: %s"), csReceiptSrc, csItemEntryId, csTemp);
    }
}

#ifndef _CPPUNIT
//+Japan RFC#1 (RFQ 893)
//************************************************************************ 
//
// Function:		GetPickListZeroWeightValue
// Purpose:			Returns the value of ZEROWEIGHTITEM of Picklist
// Parameters:		csItemCode - Item code
// Returns:	        bool - true or false
// Calls:           
// Side Effects:
// Notes:           
//
//************************************************************************
bool PSProceduresBase::GetPickListZeroWeightValue( const CString& csItemCode )
{
	trace(L6, _T("+PSProceduresBase::GetPickListZeroWeightValue, csItemCode = %s"), csItemCode );
	
	bool bZeroWeightItem = false; 
	m_mZeroWeightItems.Lookup( csItemCode, bZeroWeightItem );

	trace(L6, _T("-PSProceduresBase::GetPickListZeroWeightValue, csItemCode = %s"), csItemCode );
	return bZeroWeightItem ;
}
//-Japan RFC#1 (RFQ 893)

void PSProceduresBase::RegisterPsxEventHandler( void * EventHandler )
{
	CPSXInterface::Register( ( PPSXEVENTHANDLER) EventHandler );
}


void PSProceduresBase::UnRegisterPsxEventHandler( void * EventHandler )
{
	CPSXInterface::Unregister( ( PPSXEVENTHANDLER ) EventHandler );
}


void PSProceduresBase::AddPsxClickEventMapping( LPCTSTR KeyName, 
													PSEVT evtType,
													long  wParam,
													bool  bUseWParam,
													long  lParam, 
													bool  bUseLParam,
													void  *pMappingFxn )
{
	CPSXInterface::AddExternalPsxEventMappingEntry( KeyName, 
															evtType,
															wParam,
															bUseWParam,
															lParam, 
															bUseLParam,
															( CPSXInterface::PMAPPINGFXN ) pMappingFxn );
}


bool PSProceduresBase::RemovePsxClickEventMapping( LPCTSTR KeyName )
{
	return CPSXInterface::RemoveExternalPsxEventMappingEntry( KeyName );
}



#endif //_CPPUNIT
//************************************************************************ 
// Class :          CPSXRedrawLock
// Purpose:			Class to handle setting a Control's redraw false/true
// Parameters:		pPSX - pointer to PSX instance
//                  csControl - control name
// Notes:           The PSX object keeps an internal counter of the number
//                  of times the PROPERTYREDRAW property has been set to false. 
//                  The property must be set to true an equal number of times
//                  before redraws on the control will resume.  This
//                  Redraw lock class will set PROPERTYREDRAW to true when
//                  the object is created,  and set it to false in the 
//                  destructor.  
//                  The object should be created within a method that needs
//                  to freeze redraws.  It will automatically set PROPERTYREDRAW
//                  to true (decrementing the "don't redraw" counter) when
//                  the object falls out of scope at the end of the method.
//
//************************************************************************
CPSXRedrawLock::CPSXRedrawLock(CString csControl, CString csTraceID)
{
    m_csControl = csControl;
    m_csTraceID = csTraceID;
    ps.GetPSXObject()->SetControlProperty( m_csControl, UI::PROPERTYREDRAW, VARIANT_FALSE );
    trace(L7, _T("CPSXRedrawLock(%s) Set control \"%s\" PROPERTYREDRAW to FALSE"),
                  m_csTraceID, m_csControl);
}
CPSXRedrawLock::~CPSXRedrawLock()
{
    ps.GetPSXObject()->SetControlProperty( m_csControl, UI::PROPERTYREDRAW, VARIANT_TRUE );
    trace(L7, _T("CPSXRedrawLock(%s) Set control \"%s\" PROPERTYREDRAW to TRUE"),
                  m_csTraceID, m_csControl);
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		ShowCMWeightNumber (SR682)
// Purpose:			Show weight expected/observed & number of items
//					on lane according to ShowWeightInfoOnLane, SR682
// Parameters:		[IN] CString csItemDesc, CString observedWt, long numItems
// Returns:	        Nothing
// Calls:           SetConfigProperty - set specific configuration property 
//										for a control
//					SetTransactionVariable - set value for a specific transaction
//											 variable
//
// Side Effects:	None
// Notes:			We manipulate 6 controls within 4 contexts here
//					Contexts: Unexpected Increase, Unexpected Decrease,
//					Picking Up Item and Weight Mismatch.
//					Label controls are separate from variable controls
//					to allow for translated abbreviations in other languages
//
//************************************************************************
void PSProceduresBase::ShowCMWeightNumber(CString expectedWt, CString observedWt, long numItems)
{
	COleVariant v1 = expectedWt;
	COleVariant v2 = observedWt;
	COleVariant v3 = numItems;
	COleVariant vValue = VARIANT_TRUE;

	m_pPSX->SetTransactionVariable( _T("CMWeightExpectedAmount"), v1 );
	m_pPSX->SetTransactionVariable( _T("CMWeightObservedAmount"), v2 );
	m_pPSX->SetTransactionVariable( _T("CMItemNumberAmount"), v3 );

	// 6 Controls are shared by 4 contexts. Must show label and variable controls where necessary
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightRemovedText"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightAddedText"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightRemovedText"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	//sr714+
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	//sr714-
}

//************************************************************************ 
//
// Function:		HideCMWeightNumber (SR682)
// Purpose:			Hide weight expected/observed & number of items
//					on lane according to ShowWeightInfoOnLane, SR682
// Parameters:		None
// Returns:	        Nothing
// Calls:           SetConfigProperty - set specific configuration property 
//										for a control
//
// Side Effects:	None
// Notes:			We manipulate 6 controls within 4 contexts here
//					Contexts: Unexpected Increase, Unexpected Decrease,
//					Picking Up Item and Weight Mismatch.
//					Label controls are separate from variable controls
//					to allow for translated abbreviations in other languages
//
//************************************************************************
void PSProceduresBase::HideCMWeightNumber()
{
	COleVariant vValue = VARIANT_FALSE;

	// 6 Controls are shared by 4 contexts. Must hide label and variable controls where necessary
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecMisMatchWeight"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightRemovedText"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecUnexpectedDecrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightAddedText"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecUnExpectedIncrease"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightRemovedText"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("PickingUpItems"), UI::PROPERTYVISIBLE, vValue);
	//sr714+
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberText"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightExpectedAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMWeightObservedAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	m_pPSX->SetConfigProperty( _T("CMItemNumberAmount"), _T("SecBagBoxNotValid"), UI::PROPERTYVISIBLE, vValue);
	//sr714-
}

//************************************************************************ 
//
// Function:		DisplayNextCategoryItem
// Purpose:			Display next category items when scroll
// Parameters:		[in] lScroll - scroll type: 2-down, 3-up
//						  					 
// Returns:			
// Calls:			DisplaySelectedItem
//		
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::DisplayNextCategoryItem( const long lScroll)
{
	trace(L7, _T("+PSProceduresBase::DisplayNextCategoryItem, lScroll=%d"), lScroll);

    COleVariant v, vReturn;

	if ( CanScroll(lScroll) )
	{
		v = lScroll;
		m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDSCROLL, vReturn, 1, v );
		return;
	}
	else 
	{
		CString csFirstCat(_T("")), csLastCat(_T(""));
		CCategoryInfo* pItem = NULL;
		int nCount = MainCategoryList[m_languageUsed].GetCount();
	
		POSITION pos = MainCategoryList[m_languageUsed].GetTailPosition();
		if ( pos != NULL )
		{
			pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetAt( pos );
			csLastCat = pItem->GetCatAlphabetic();
			if (csLastCat.IsEmpty())
				csLastCat = pItem->GetCategoryID();
		}

		pos = MainCategoryList[m_languageUsed].GetHeadPosition();
		if ( pos != NULL )
		{
			pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetAt( pos );
			csFirstCat = pItem->GetCatAlphabetic();
			if (csFirstCat.IsEmpty())
				csFirstCat = pItem->GetCategoryID();
		}
		
		CString csCat(_T(""));
		int nIndex = -1;
		while( pos != NULL )
		{		
			CCategoryInfo* pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetNext( pos );
			if (pItem != NULL )
			{
				nIndex++;
				csCat = pItem->GetCatAlphabetic();
				if (csCat.IsEmpty())
					csCat = pItem->GetCategoryID();

				CString csCategory;
				if ( m_strLastProduceLookupList.Left( _tcslen( TAB_CATEGORY ) ) == TAB_CATEGORY )
				{
					csCategory = m_strLastProduceLookupList.Right(1);
				}
				else
				{
					csCategory = m_strLastProduceLookupList;
				}
				if ( !csCat.CompareNoCase(csCategory)  )
				{
					if (lScroll == 3) //up
					{
						if (csCat == csFirstCat) //go to last category
						{
							nIndex = nCount - 1;
							pos = MainCategoryList[m_languageUsed].GetTailPosition();	
						}
						else
						{
							nIndex = nIndex - 1;
							pos = MainCategoryList[m_languageUsed].FindIndex(nIndex);		
						}
						pItem= (CCategoryInfo*)MainCategoryList[m_languageUsed].GetAt(pos);
					}
					else //down
					{	
						if (csCat == csLastCat) //go to first category
						{
							nIndex = 0;
							pos = MainCategoryList[m_languageUsed].GetHeadPosition();
							pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetAt( pos );

						}
						else
						{
							nIndex = nIndex + 1;
							pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetNext( pos );
						}
					}

					if (pItem != NULL)
					{	
						CString csCatToDisplay;
						csCat = pItem->GetCatAlphabetic();

						if (csCat.IsEmpty())
						{
							csCat = pItem->GetCategoryID(); 
							if (csCat != TAB_FAV)
							{
								csCatToDisplay = TAB_CATEGORY + csCat;

							}
							else
							{
								csCatToDisplay = csCat;
							}
						}
						else
						{
							csCatToDisplay = csCat;
						}
							
						DisplaySelectedItem(csCatToDisplay);

						//Set selected category button state to Pushed
						COleVariant vIndex = (long)nIndex;
						v = (long) UI::Pushed;
						m_pPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, v );
						
						//Scroll all the way down to the last page of the category
						/*VARIANT_BOOL vbCanScrollDown = VARIANT_FALSE;
						if (lScroll == 3) //up
						{
							v = (long)UI::PageDown;
							if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
							{
								vbCanScrollDown = vReturn.boolVal;		
							}
							while (vbCanScrollDown)
							{
								m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDSCROLL, vReturn, 1, v );
								v = (long)UI::PageDown;
								if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
								{
									vbCanScrollDown = vReturn.boolVal;		
								}
							}
						}*/
						break;
					}
						
				}
			}
		} 
	}
	trace(L7, _T("-PSProceduresBase::DisplayNextCategoryItem"));
}

//************************************************************************ 
//
// Function:		DisplaySelectedSubItem
// Purpose:			Display picklist items under the selected subcategory
// Parameters:		[in] csSubCategory - selected subcategory
//						  					 
// Returns:			
// Calls:			GetPicklistItemInfo 
//					SetPickListButtonProperties 
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::DisplaySelectedSubItem(CString csSubCategory)
{
	trace(L7, _T("+PSProceduresBase::DisplaySelectedSubItem, csSubCategory=%s"), csSubCategory);

	CSItemList *pItemList = &(ItemList[m_languageUsed]);
	
	COleVariant v, vReturn;
	DWORD nButtonCount = pItemList->GetCount();
	if( !nButtonCount )
	{
		v = 0L;
		m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );
		EnableProduceScrollButtons();
		return;
	}

	CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplaySelectedSubItem"));  //TAR 371980 fix ;
   CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CStringArray csCatDescList;
	CArray<ITEMTYPE,ITEMTYPE> nItemTypeList;
	
	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csCategories(_T(""));	
	CString strName( _T("") );

	CString csCombineItemInfo = _T("");  
	SetCategoryTitles(csSubCategory);
	
	pos = pItemList->GetHeadPosition();
	while( pos != NULL )
	{
		pItem = (CItemInfo*)pItemList->GetNext( pos );

		strName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
		strName.MakeUpper();

		CString csCat(_T(""));
		CString csSubCat(_T(""));
		CString csItemSubCat(_T(""));
		//Get subcategory for subcategory
		csCategories = pItem->GetCategories();
		if ( !csCategories.IsEmpty() )
		{
			csSubCat = GetCategory(csCategories, csSubCategory);
			csCat = csSubCat;
		}
		//Get subcategory for item
		csCategories = pItem->GetSubCategories();
		if ( !csCategories.IsEmpty() )
		{
			csItemSubCat = GetCategory(csCategories, csSubCategory);
		}	

		//Get items and subcategories belong to the same subcategory 
		if ( !csSubCat.IsEmpty() || !csItemSubCat.IsEmpty() )
		{
			GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					             csItemDataList, csCatDescList, nItemTypeList, s_szProduceButtonListName, csCat );
			nButtonCount++;	
		}
	}
	
	v = (long)nButtonCount;

   CPSXRedrawLock csRedrawLock( s_szProduceButtonListName, _T("PSProceduresBase::DisplaySelectedSubItem"));
	m_pPSX->SetControlProperty( s_szProduceButtonListName, UI::PROPERTYBUTTONCOUNT, v );

	SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList, 
							 csImageList, csItemDataList, csCatDescList, nItemTypeList,
							 s_szProduceButtonListName, nButtonCount);  

	trace(L7, _T("-PSProceduresBase::DisplaySelectedSubItem"));
}

//************************************************************************ 
//
// Function:		DisplayQuickPickItem
// Purpose:			Display picklist items/categories on Scan & Bag
// Parameters:		
//						  					 
// Returns:			
// Calls:			GetPicklistItemInfo - get picklist item information
//					SetPickListButtonProperties - set picklist button properties for display
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::DisplayQuickPickItem()
{
	trace(L7, _T("+PSProceduresBase::DisplayQuickPickItem"));

	//CSItemList *pItemList = &(QuickPickItemList[m_languageUsed]);

	//+ Dynamic Quickpick 
    CString csPickListPref = custInfo.GetPreference(_T("PicklistFavorites"));
    CSItemList *pItemList;

    if ( (_ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems"))) > 0) && 
             (_ttoi(custInfo.GetOption(_T("DynamicQuickPickItems"))) > 0) )
	{            
       if ( csPickListPref.GetLength()  && !m_PersonalizedQuickPickItemList[m_languageUsed].IsEmpty() )        
        {
            pItemList = &(m_PersonalizedQuickPickItemList[m_languageUsed]);
        }
        else
        {
            pItemList = &(QuickPickItemList[m_languageUsed]);
        }
	}
	else
	{
       pItemList = &(QuickPickItemList[m_languageUsed]);
	}

    //- Dynamic Quickpick 


	COleVariant v, vReturn;
	DWORD nButtonCount = pItemList->GetCount();
	if( !nButtonCount )
	{
		v = 0L;
		m_pPSX->SetControlProperty( s_szQuickPickButtonListName, UI::PROPERTYBUTTONCOUNT, v );
		return;
	}

   CPSXUserInputLock userInputLock(_T("PSProceduresBase::DisplayQuickPickItem"));  //TAR 371980 fix 

	CStringArray csSubCatDescList;
	CStringArray csItemDescList;
	CStringArray csItemCodeTextList;
	CStringArray csCatDescList;
	CStringArray csImageList;
	CStringArray csItemDataList;
	CArray<ITEMTYPE,ITEMTYPE> nItemTypeList;
	
	CItemInfo *pItem = NULL;
	POSITION pos = NULL;
	nButtonCount = 0;
	CString csName;
	CString csSubControl(_T(""));

	CString csCombineItemInfo; 
	
	pos = pItemList->GetHeadPosition();
	while( pos != NULL )
	{
		pItem = (CItemInfo*)pItemList->GetNext( pos );

		csName = pItem->GetReadingName().GetLength() ? pItem->GetReadingName() : pItem->GetItemName();
		csName.MakeUpper();

		GetPicklistItemInfo( pItem, csItemDescList, csSubCatDescList, csItemCodeTextList, csImageList, 
					                 csItemDataList, csCatDescList, nItemTypeList, s_szQuickPickButtonListName );

		nButtonCount++;	
	}
	
	v = (long)nButtonCount;

	CPSXRedrawLock csRedrawLock( s_szQuickPickButtonListName, _T("PSProceduresBase::DisplayQuickPickItem"));
   m_pPSX->SetControlProperty( s_szQuickPickButtonListName, UI::PROPERTYBUTTONCOUNT, v );

	SetPickListButtonProperties(csItemDescList, csSubCatDescList, csItemCodeTextList, 
							 csImageList, csItemDataList, csCatDescList, nItemTypeList,
							 s_szQuickPickButtonListName, nButtonCount);


	trace(L7, _T("-PSProceduresBase::DisplayQuickPickItem"));
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		PushLevels
// Purpose:			push categories/subcategories into the stack
// Parameters:		[in]  csPreviousLevel - previous category 
//						  csPreviousLevel - current category 					 
// Returns:			
// Calls:			
// Side Effects:	Modifying m_pLevelStack
// Notes:
//
//************************************************************************
void PSProceduresBase::PushLevels(const CString& csPreviousLevel, const CString& csCurrentLevel)
{
    LevelsRecord *pLevelRecord = new LevelsRecord;
    
    pLevelRecord->PreviousLevel = csPreviousLevel;		
    pLevelRecord->CurrentLevel = csCurrentLevel;
    
    if (IsLevelEmpty() && m_ProduceLookupCategoryNumber.IsEmpty())
    {
        m_ProduceLookupCategoryNumber = csPreviousLevel;
        m_ProduceLookupCategoryLevels = _T("");
        m_FirstLevelCategoryID = _T("");
        m_SecondLevelCategoryID = _T("");
    }

    m_pLevelStack.push(pLevelRecord);

    CString csLevels;
    CString csLevelName = GetCategoryName(csCurrentLevel);

    if(m_ProduceLookupCategoryLevels.IsEmpty())
    {
        //saving first level & seond level categories of heirarchy
        m_FirstLevelCategoryID = csPreviousLevel;
        m_SecondLevelCategoryID = csCurrentLevel;

        //for the heirarchy, add the main category level
        CString csBaseLevel = GetCategoryName(csPreviousLevel);
        m_ProduceLookupCategoryLevels = m_ProduceLookupCategoryLevels + csBaseLevel;
    }

    m_ProduceLookupCategoryLevels = m_ProduceLookupCategoryLevels + _T(",") + csLevelName;

    csLevels.Format(_T("TabSelected=%s;CurrentCategory=%s;Search=;"), m_ProduceLookupCategoryNumber, m_ProduceLookupCategoryLevels);

    CLaneStateData laneStateData(m_pPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUIPicklistDisplayLevels"), csLevels);
}

//************************************************************************ 
//
// Function:		PopLevels
// Purpose:			Pop categories/subcategories of the stack
// Parameters:		[out] csPreviousLevel - previous category 
//						  csPreviousLevel - current category 					 
// Returns:			
// Calls:			
// Side Effects:	Modifying m_pLevelStack
// Notes:
//
//************************************************************************
void PSProceduresBase::PopLevels(CString& csPreviousLevel, CString& csCurrentLevel)
{
    LevelsRecord *pLevelRecord = m_pLevelStack.pop();
    
    csPreviousLevel  = pLevelRecord->PreviousLevel;	
    csCurrentLevel   = pLevelRecord->CurrentLevel;	
    
    delete pLevelRecord;
    
    CString csLevels;
    CString csSubCatName = GetCategoryName(csCurrentLevel);

    CString csRemoveString = _T(",");

    if(IsLevelEmpty())
    {
        CString csMainCatName = GetCategoryName(csPreviousLevel);
        csRemoveString = csMainCatName + _T(",");
    }

    csRemoveString = csRemoveString + csSubCatName;

    m_ProduceLookupCategoryLevels.Replace(csRemoveString, _T(""));

    csLevels.Format(_T("TabSelected=%s;CurrentCategory=%s;Search=;"), m_ProduceLookupCategoryNumber, m_ProduceLookupCategoryLevels);

    CLaneStateData laneStateData(m_pPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUIPicklistDisplayLevels"), csLevels);
}

//************************************************************************ 
//
// Function:		DeleteAllLevels
// Purpose:			Remove all categories/subcategories in the stack
// Parameters:		
//												 
// Returns:			
// Calls:			
// Side Effects:	Modifying m_pLevelStack
// Notes:
//
//************************************************************************
void PSProceduresBase::DeleteAllLevels(void)
{
    m_pLevelStack.deleteAll();
    
    m_ProduceLookupCategoryNumber = _T("");
    m_ProduceLookupCategoryLevels = _T("");
    m_FirstLevelCategoryID = _T("");
    m_SecondLevelCategoryID = _T("");

    CLaneStateData laneStateData(m_pPSX);
    laneStateData.SendNextGenUIData( _T("NextGenUIPicklistDisplayLevels"), (CString) _T(""));
}


//************************************************************************ 
//
// Function:		IsLevelEmpty
// Purpose:			Check to see if still there is a category/subcategory in the stack
// Parameters:		
//												 
// Returns:			true if empty, false if not empty
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::IsLevelEmpty(void)
{
	return m_pLevelStack.isEmpty();
}

CString PSProceduresBase::GetCategoryName(CString csCatID)
{
		POSITION pos = CategoryList[m_languageUsed].GetHeadPosition();
        CString csCategoryName = _T("");
		CCategoryInfo* pItem = NULL;
		while( pos != NULL )
		{
			pItem = (CCategoryInfo*)CategoryList[m_languageUsed].GetNext( pos );
			CString csId = pItem->GetCategoryID();
            if (csId.Compare(csCatID) == 0)
            {
			   csCategoryName = pItem->GetCatDescription();
			   break;
            }
        }
        if (csCategoryName.IsEmpty())
        {
            return csCatID;
        }
        else
        {
            return csCategoryName;
        }
}



#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		CanScroll
// Purpose:			Check to see if the button list can scroll
// Parameters:		[in] lScroll - scroll type: 2-down, 3-up					
//							 
// Returns:			true if can scroll, false if can not scroll
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::CanScroll(const long lScroll)
{
	trace(L6, _T("+PSProceduresBase::CanScroll"));

	VARIANT_BOOL vbCanScrollUp = VARIANT_FALSE, vbCanScrollDown = VARIANT_FALSE;
	COleVariant v, vReturn;
	bool bCanScroll = false;

    v = (long)UI::PageUp;
    if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
	{
		vbCanScrollUp = vReturn.boolVal;		
	}	
	v = (long)UI::PageDown;
	if( PSX_SUCCESS == m_pPSX->SendCommand( s_szProduceButtonListName, UI::COMMANDCANSCROLL, vReturn, 1, v ) )
	{
		vbCanScrollDown = vReturn.boolVal;		
	}
	if ( (lScroll == 2 && vbCanScrollDown == VARIANT_TRUE ) ||//down
		 (lScroll == 3 && vbCanScrollUp == VARIANT_TRUE) )//up
	{
		bCanScroll = true;
	}

	trace(L6, _T("-PSProceduresBase::CanScroll, lScroll=%d, bCanScroll=%d"), lScroll, bCanScroll);

	return bCanScroll;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		FindCategory
// Purpose:			Find a category in the MainCategory list
// Parameters:		[in] csCategory - category to be found in the list
//					   [out] csRetCategory - category returned
//					      csCategoryId - category id returned//							 
// Returns:			true if found, false if not found
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::FindCategory(const CString& csCategory, CString& csRetCategory, CString& csCategoryId, int& nTabIndex)
{
	POSITION pos = MainCategoryList[m_languageUsed].GetHeadPosition();
	bool bFind = false;

	CString csCatAlpha(_T(""));
    nTabIndex = 0;
	while( pos != NULL )
	{		
		CCategoryInfo* pItem = (CCategoryInfo*)MainCategoryList[m_languageUsed].GetNext( pos );
		if ( pItem != NULL )
		{
			csCatAlpha = pItem->GetCatAlphabetic();
			csCategoryId = pItem->GetCategoryID();
			CString csCategoryAndID = TAB_CATEGORY + csCategoryId;
			if ( (csCategory.CompareNoCase(csCategoryId) == 0) || (csCategory.CompareNoCase(csCatAlpha) == 0)
                || csCategory.CompareNoCase(csCategoryAndID)==0)
			{
				if ( csCatAlpha.GetLength() )
				{
					csRetCategory = csCatAlpha;
				}
				else if ( csCategoryId == TAB_FAV )
				{
					csRetCategory = csCategoryId;
				}
				else
				{
					csRetCategory = csCategoryAndID;
				}

				bFind = true;
				break;
			}
            else
            {
                if ( csCategoryId == TAB_FAV )
				{
                    //if favorite found before finding the category, subtract from index
                    nTabIndex = nTabIndex - 1;
				}
            }
            nTabIndex++;
		}
	}
    if (!bFind)
    {
        nTabIndex = -1;
    }
	return bFind;
}

//************************************************************************ 
//
// Function:		GetCategory
// Purpose:			Get the matching category/subcategory in the <InCategories>, 
//                  <Categories>, or <SubCategories> list
// Parameters:		[in] csCategories - list of parent categories/subcategories
//						 csCategory - category/subcategory to be found in the list
//							 
// Returns:			category/subcategory found
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetCategory(const CString& csCategories, const CString& csCategory)
{
	CString str( csCategories );
	CString csCat(_T(""));

	TCHAR *psz = _tcstok( str.GetBuffer( 0 ), _T(",") );
	while( psz )
	{
		CString csCat2 = psz;
		if (csCat2 == csCategory)
		{
			csCat = csCat2;
			break;
		}
		psz = _tcstok( NULL, _T(",") );
	}
	str.ReleaseBuffer();

	return csCat;
}

//************************************************************************ 
//
// Function:        FindSubCategory
// Purpose:         Find a subcategory in the SubCategory list
// Parameters:      [in] csSubCategory - category to be found in the list
//                     [out] csRetCategory - category returned
//                        csCategoryId - category id returned//
// Returns:         true if found, false if not found
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::FindSubCategory(const CString& csSubCategory)
{
    bool bFind = false;

    int nLen = csSubCategory.GetLength();

    CString csSubCat = csSubCategory.Right( nLen - _tcslen(TAB_CATEGORY) );

    if(csSubCat.IsEmpty())
    {
        return bFind;
    }

    POSITION pos = SubCategoryList[m_languageUsed].GetHeadPosition();

    CString csCatAlpha(_T(""));
    CString csCategoryId(_T(""));

    while( pos != NULL )
    {
        CCategoryInfo* pItem = (CCategoryInfo*)SubCategoryList[m_languageUsed].GetNext( pos );
        if ( pItem != NULL )
        {
            csCatAlpha = pItem->GetCatAlphabetic();
            csCategoryId = pItem->GetCategoryID();
            
            if ( (csSubCat.CompareNoCase(csCategoryId) == 0) || (csSubCat.CompareNoCase(csCatAlpha) == 0) )
            {
                bFind = true;
                break;
            }
        }
    }
    return bFind;
}

//************************************************************************ 
//
// Function:        GetFirstLevelCategoryID
// Purpose:         Get a main category ID to corectly get button index of the 
//                  selected SubCategory from the main TAB
// Parameters:      
//
// Returns:         the main category ID of which the SubCategories in the main Tab belongs
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetFirstLevelCategoryID(void)
{
    return m_FirstLevelCategoryID;
}

CString PSProceduresBase::GetSecondLevelCategoryID(void)
{
    return m_SecondLevelCategoryID;
}

void PSProceduresBase::SetSubCategorySelectedButton(long lIndex)
{
    COleVariant vIndex( 0L ), vParam( 0L ), vReturn( VARIANT_FALSE );

    vIndex = lIndex;
    vParam = (long) UI::Pushed;
    m_pPSX->SendCommand( _T("TabSelected"), UI::COMMANDSETBUTTONSTATE, vReturn, 2, vIndex, vParam );
}

CString PSProceduresBase::GetProduceLookupCategoryLevels(void)
{
    return m_ProduceLookupCategoryLevels;
}

void PSProceduresBase::SetProduceLookupCategoryNumber(CString csIndexOfCategory)
{
    m_ProduceLookupCategoryNumber = csIndexOfCategory;
}


//************************************************************************ 
//
// Function:		GetPicklistItemsSold
// Purpose:			Get number of picklist items sold
// Parameters:		
//							 
// Returns:			m_lTotalPicklistItemsSold			
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetPicklistItemsSold()
{ 
	return m_lTotalPicklistItemsSold;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		SetPicklistItemsSold
// Purpose:			Set number of picklist items sold
// Parameters:		[in] lTotalPicklistItemsSold - total picklist items sold
//							 
// Returns:			
// Calls:			
// Side Effects:	Modifying m_lTotalPicklistItemsSold
// Notes:
//
//************************************************************************
void PSProceduresBase::SetPicklistItemsSold(long lTotalPicklistItemsSold)
{ 
	m_lTotalPicklistItemsSold = lTotalPicklistItemsSold;
}

//************************************************************************ 
//
// Function:		GetScreenTitle
// Purpose:			Get category title for display on Lookup Items screen
// Parameters:		
//							 
// Returns:			m_csScreenTitle		
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetScreenTitle()
{ 
	return m_csScreenTitle;
}

//************************************************************************ 
//
// Function:		GetInstructions
// Purpose:			Get category instructions for display on Lookup Items screen
// Parameters:	
//							 
// Returns:			m_csInstructions	
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetInstructions()
{ 
	return m_csInstructions;
}

#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		SetCategoryTitles
// Purpose:			Set category titles to display on Lookup Items screen
// Parameters:		[in] csCategory - picklist category
//							 
// Returns:			
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetCategoryTitles(const CString& csCategory)
{ 
	POSITION pos = CategoryList[m_languageUsed].GetHeadPosition();
	CCategoryInfo* pItem = NULL;
	CString csCatId(_T(""));
	CString csCatAlpha(_T(""));

	while( pos != NULL )
	{		
		pItem = (CCategoryInfo*)CategoryList[m_languageUsed].GetNext( pos );
		if ( pItem != NULL )
		{
			csCatAlpha = pItem->GetCatAlphabetic();
			csCatId = pItem->GetCategoryID();
		
			if ( (csCategory.CompareNoCase(csCatId) == 0) || (csCategory.CompareNoCase(csCatAlpha) == 0) )
			{
				m_csScreenTitle = pItem->GetScreenTitle();
				//m_csInstructions = pItem->GetInstructions(); // SSCOP-2575

				if (m_csScreenTitle.IsEmpty())
				{
					m_csScreenTitle = pItem->GetCatDescription();
					//m_csInstructions = GetPSText( MSG_LOOKUP_TITLE2 ); // +SSCOP-2575
				}
				break;
			}
		}
	}
}
#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		GetPicklistItemInfo
// Purpose:			Get picklist item information from ItemList
// Parameters:		[in] csControl - buttonlist control name
//                       pItem - pointer to an item node in the ItemList
//						 csCat - picklist category
//					[out] csItemDescList - list of item description
//						  csSubCatDescList - list of subcategory description
//						  csItemCodeTextList - list of item code
//						  csImageList - list of picklist images
//						  csItemDataList - list of item data
//						  csCatDescList - list of category description
//						  nItemTypeList - list of item type
//							 
// Returns:			
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::GetPicklistItemInfo( CItemInfo *pItem, 
										    CStringArray& csItemDescList, 
										    CStringArray& csSubCatDescList, 
										    CStringArray& csItemCodeTextList, 
										    CStringArray& csImageList, 
										    CStringArray& csItemDataList,
											CStringArray& csCatDescList,
											CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
											const CString& csControl,
										    const CString& csCat )
{
	trace(L7, _T("+PSProceduresBase::GetPicklistItemInfo, csControl=%s, csCat=%s"), csControl, csCat);

	CString csInfo(_T(""));
	ITEMTYPE nItemType = TYPE_CATEGORY;
	TCHAR szPath[MAX_PATH] = _T("");
	CString csImagePath;
#ifndef _CPPUNIT
	GET_PARTITION_PATH(co.GetcsPicklistImagePath(), csImagePath.GetBuffer(_MAX_PATH)); //RFC381050
	_tmakepath( szPath, NULL, csImagePath, NULL, NULL );
	csImagePath.ReleaseBuffer();
#endif //_CPPUNIT

	//Get item description
	if ( pItem->GetIsItem() ) 
	{
		csInfo = pItem->GetItemName();
		nItemType = TYPE_ITEM;
	}
	csItemDescList.Add(csInfo);

	//Get subcategory description
	csInfo = _T("");
	if ( pItem->GetIsSubCategory() ) 
	{
		csInfo = pItem->GetItemName();
		nItemType = TYPE_SUBCATEGORY;
	}
	csSubCatDescList.Add(csInfo);

	if ( csControl == s_szQuickPickButtonListName ) 
	{
		//Get category description
		csInfo = _T("");
		if ( !pItem->GetIsSubCategory() && !pItem->GetIsItem() ) 
		{
			csInfo = pItem->GetItemName();
		}
		csCatDescList.Add(csInfo);
	}

	//Get image
	csInfo = szPath;
    CString csImage = (LPCTSTR)pItem->GetItemGraphic();
    csInfo += csImage;
	csImageList.Add(csInfo);

	//Get item code text
	csInfo = _T("");
	if ( co.GetfOperationsDisplayPickListItemCode() )
	{
		if ( pItem->GetIsItem() )
		{
			csInfo = pItem->GetItemCode();
		}
	}
	csItemCodeTextList.Add(csInfo);
	
	//Combined item information
	CString csCombineItemInfo;
	csCombineItemInfo = pItem->GetItemCode();
	csCombineItemInfo += STR_SEPARATOR;
	csCombineItemInfo += pItem->GetItemName();
	csCombineItemInfo += STR_SEPARATOR;
	csCombineItemInfo += pItem->GetIsSubCategory()?"True":"False";
	csCombineItemInfo += STR_SEPARATOR;
	if ( csControl == s_szQuickPickButtonListName ) 
	{
		csCombineItemInfo += pItem->GetIsItem()?"True":"False";
		csCombineItemInfo += STR_SEPARATOR;
		csCombineItemInfo += pItem->GetAlphabetic();
		csCombineItemInfo += STR_SEPARATOR;
		csCombineItemInfo += pItem->GetCategories();
	}
	else
	{
		csCombineItemInfo += csCat;
	}
    if( pItem->GetIsItem()
        && co.GetfOperationsUsePicklistXMLImageName() 
        && !csImage.IsEmpty() )
    {
        csCombineItemInfo += STR_SEPARATOR;
        csCombineItemInfo += pItem->GetItemGraphic();
    }
    
	csItemDataList.Add(csCombineItemInfo);
	nItemTypeList.Add(nItemType);

	trace(L7, _T("-PSProceduresBase::GetPicklistItemInfo"));
}

//************************************************************************ 
//
// Function:		SetPickListButtonProperties
// Purpose:			Set picklist button properties for display
// Parameters:		[in] csControl - buttonlist control name
//						 nButtonCount - picklist button count
//						 csItemDescList - list of item description
//						 csSubCatDescList - list of subcategory description
//						 csItemCodeTextList - list of item code
//						 csImageList - list of picklist images
//						 csItemDataList - list of item data
//						 csCatDescList - list of category description
//						 nItemTypeList - list of item type
//							 
// Returns:			
// Calls:			SetControlProperty	   - PSX API to set button properties 
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetPickListButtonProperties( CStringArray& csItemDescList, 
												 CStringArray& csSubCatDescList, 
												 CStringArray& csItemCodeTextList, 
												 CStringArray& csImageList, 
												 CStringArray& csItemDataList,
												 CStringArray& csCatDescList,
												 CArray<ITEMTYPE, ITEMTYPE>& nItemTypeList,
												 const CString& csControl,
												 int nButtonCount )
{
	trace(L6, _T("+PSProceduresBase::SetPickListButtonProperties, csControl=%s, nButtonCount=%d"), csControl, nButtonCount);
	
	if ( nButtonCount>0 )
	{
		COleVariant v;
		CString csSubControl;
	
		for (int i=0; i<nButtonCount; i++)
		{
			//Get item type
			ITEMTYPE nItemType = nItemTypeList.GetAt(i);
			csSubControl.Format(_T("%s[%d]"), csControl, i);

			//Set button data
			CString csItemData = csItemDataList.GetAt(i);
			v = csItemData;
			m_pPSX->SetControlProperty(csSubControl, UI::PROPERTYDATA, v);
	
			//Set item description
			CString csItemDesc = _T("");
            if (nItemType == TYPE_ITEM)
			{
                csItemDesc = csItemDescList.GetAt(i);
			}
			v = csItemDesc;
			csSubControl.Format(_T("%s[%d].ItemDescription"), csControl, i);
			m_pPSX->SetControlProperty(csSubControl, UI::PROPERTYTEXT, v);

			//Set subcategory description
			CString csSubCatDesc = csSubCatDescList.GetAt(i);
			v = csSubCatDesc;
			csSubControl.Format(_T("%s[%d].SubCategoryDescription"), csControl, i);
			m_pPSX->SetControlProperty(csSubControl, UI::PROPERTYTEXT, v);

			//Set item code text
            CString csItemCodeText = _T("");
            if (nItemType == TYPE_ITEM)
			{
                csItemCodeText = csItemCodeTextList.GetAt(i);
			}
			v = csItemCodeText;
			csSubControl.Format(_T("%s[%d].ItemCode"), csControl, i);
			m_pPSX->SetControlProperty(csSubControl, UI::PROPERTYTEXT, v);

			if ( csControl == s_szQuickPickButtonListName )
			{
				//Set category description
				CString csCatDesc = csCatDescList.GetAt(i);
				v = csCatDesc; 
				csSubControl.Format(_T("%s[%d].CategoryDescription"), csControl, i);
				m_pPSX->SetControlProperty(csSubControl, UI::PROPERTYTEXT, v);
			}
			
		}
	}
	trace(L6, _T("-PSProceduresBase::SetPickListButtonProperties"));
}

//SR679
long PSProceduresBase::GetPicklistItemsIncludeVoidItems()
{ 
	return m_lTotalPicklistItemsIncludeVoidItems;
}

void PSProceduresBase::SetPicklistItemsIncludeVoidItems(long lTotalPicklistItemsIncludeVoidItems)
{ 
	m_lTotalPicklistItemsIncludeVoidItems = lTotalPicklistItemsIncludeVoidItems;
}

#endif
//************************************************************************ 
//
// Function:		GetParentCategories
// Purpose:			Get parent categories for the selected subcategory
// Parameters:		[in] csCategories - category/subcategory list stored in <InCategories> 
//					for the subcategory selected
//                  [out] csParents - list of parent categories in the hierarchy						  						 
// Returns:			
// Calls:			FindCategory, SetLastProduceLookupList
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::GetParentCategories( const CString& csCategories, CStringArray& csParents)
{
    CString csTmpCategories = csCategories;
    int nIndex;
    while (!csTmpCategories.IsEmpty())
    {
        CString str( csTmpCategories );
        CString csCat(_T(""));
        CString csRetCategory(_T(""));
        CString csCategoryId(_T(""));
        TCHAR *psz = _tcstok( str.GetBuffer( 0 ), _T(",") );
        while( psz )
        {
            csCat = psz;
            bool bFind = FindCategory(csCat, csRetCategory, csCategoryId, nIndex);
            trace(L6,_T("FindCategory result: csCat[%s] return[%s] id[%s] Index[%d]")
                    ,csCat, csRetCategory, csCategoryId, nIndex);
            if ( bFind )
            {
                psz = _tcstok( NULL, _T(",") );
                csParents.Add(csCat);
                csTmpCategories = _T("");
                SetLastProduceLookupList(csRetCategory);

                if(csCat == TAB_FAV)
                {
                    m_ProduceLookupCategoryNumber = csCat;
                }
                else
                {
                    int index;
                    CString csCategory, csId;
                    if(FindCategory(TAB_FAV, csCategory, csId, index))
                    {
                        nIndex = nIndex + 1;
                    }
                    m_ProduceLookupCategoryNumber.Format(_T("%d"),nIndex);
                }
                break;
            }
            else
            {
                if( csRetCategory.IsEmpty() && nIndex == -1)
                {
                    trace(L4,_T("WARNING: Can't find category[%s]. End Search. "), csCat);
                    csTmpCategories.Empty();
                    break;
                }
                POSITION pos = SubCategoryList[m_languageUsed].GetHeadPosition();
                bFind = false;
                CString csCatId(_T(""));

                while( pos != NULL )
                {       
                    CCategoryInfo* pItem = (CCategoryInfo*)SubCategoryList[m_languageUsed].GetNext( pos );
                    if ( pItem != NULL )
                    {
                        csCatId = pItem->GetCategoryID();
                        
                        if ( csCat.CompareNoCase(csCatId) == 0 )
                        {
                            csParents.Add(csCat);
                            csTmpCategories = pItem->GetCategories();
                            break;
                        }
                    }
                }
            }
            psz = _tcstok( NULL, _T(",") );
        }
        str.ReleaseBuffer();
    }
}

bool PSProceduresBase::HasSubCategory(CString csMainCategory, CString &csFirstSubCategoryID)
{
    bool bValue = false;
    CString csCat(_T(""));

    CCategoryInfo *pItem = NULL;
    CSCategoryList* pCatList = NULL;

    pCatList = &(SubCategoryList[m_languageUsed]);
    long lButtonCount = SubCategoryList[m_languageUsed].GetCount();

    for( long i = 0; i < lButtonCount; i++ )
    {
        pItem = (CCategoryInfo*)pCatList->GetAt( pCatList->FindIndex( i ) );

        CString csInCategories = pItem->GetCategories();

        if ( !csInCategories.IsEmpty() )
        {   
            csCat = GetCategory(csInCategories, csMainCategory);
        }

        if( csCat == csMainCategory )
        {
            csFirstSubCategoryID = pItem->GetCategoryID();
            bValue = true;
            break;
        }
    }

    return bValue;

}

//************************************************************************ 
//
// Function:		GetTabSelectedIndexofSubCategory
// Purpose:			Get the position of a subcategory withing a parent category
// Parameters:		[in] csSubCategory - category/subcategory list stored in <InCategories> 
//                  [out] csParents - list of parent categories in the hierarchy						  						 
// Returns:			
// Calls:			FindCategory, SetLastProduceLookupList
// Side Effects:
// Notes:
//
//************************************************************************
long PSProceduresBase::GetTabSelectedIndexofSubCategory(CString csMainCategoryID, CString csSubCategoryID)
{
    CString csCat(_T(""));

    CCategoryInfo *pItem = NULL;
    CSCategoryList* pCatList = NULL;

    pCatList = &(SubCategoryList[m_languageUsed]);
    long lButtonCount = SubCategoryList[m_languageUsed].GetCount();

    long lTabIndex = 0;

    for(long i = 0; i < lButtonCount; i++ )
    {
        pItem = (CCategoryInfo*)pCatList->GetAt( pCatList->FindIndex( i ) );

        CString csInCategories = pItem->GetCategories();

        if ( !csInCategories.IsEmpty() )
        {   
            csCat = GetCategory(csInCategories, csMainCategoryID);
        }

        if( csCat == csMainCategoryID )
        {
            if(csSubCategoryID == pItem->GetCategoryID())
            {
                break;
            }
            lTabIndex++;
        }
    }

    return lTabIndex;
}

//+TAR 371980
// Class CPXUserInputLock
CPSXUserInputLock::CPSXUserInputLock(CString csTraceID)
{
	m_csTraceID = csTraceID;	
	ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYUSERINPUT, VARIANT_FALSE);
	trace(L7, _T("CPSXUserInputLock(%s) Set control PROPERTYUSERINPUT to FALSE"), m_csTraceID);
}


CPSXUserInputLock::~CPSXUserInputLock()
{
	ps.GetPSXObject()->SetControlProperty(UI::CONTROLDISPLAY, UI::PROPERTYUSERINPUT, VARIANT_TRUE);
	trace(L7, _T("CPSXUserInputLock(%s) Set control PROPERTYUSERINPUT to TRUE"), m_csTraceID);
}
//-TAR 371980

//************************************************************************ 
//
// Function:		SetFoundPickListItemsFromSearch
// Purpose:			set the value of the variable m_bFoundPickListItemsFromSearch
// Parameters:		[in] b - a bool variable		  						 
// Returns:			
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetFoundPickListItemsFromSearch(bool b)
{
  m_bFoundPickListItemsFromSearch=b;
}

//************************************************************************ 
//
// Function:		GetFoundPickListItemsFromSearch
// Purpose:			Get the value of the variable m_bFoundPickListItemsFromSearch
// Parameters:				  						 
// Returns:			The value of the variable m_bFoundPickListItemsFromSearch 
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::GetFoundPickListItemsFromSearch()
{
  return m_bFoundPickListItemsFromSearch;
}

//************************************************************************ 
//
// Function:		SetDisplayTabIsMainCategories
// Purpose:			set the value of the variable m_bDisplayTabIsMainCategories
// Parameters:		[in] b - a bool variable		  						 
// Returns:			
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetDisplayTabIsMainCategories(bool b)
{
  m_bDisplayTabIsMainCategories=b;
}

//************************************************************************ 
//
// Function:		GetDisplayTabIsMainCategories
// Purpose:			Get the value of the variable m_bDisplayTabIsMainCategories
// Parameters:				  						 
// Returns:			The value of the variable m_bDisplayTabIsMainCategories 
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
bool PSProceduresBase::GetDisplayTabIsMainCategories()
{
  return m_bDisplayTabIsMainCategories;
}

//************************************************************************ 
//
// Function:		GetLCID
// Purpose:			Get the LCID a languange
// Parameters:	   [in] SCOT_LANGUAGE_TYPE lang						 
// Returns:			A CString value that holds the 4-digit Hex-value LCID in lower case
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetLCID(SCOT_LANGUAGE_TYPE lang)
{
   SLanguageInfo lInfo = co.Getlanguage(lang);
   CString csLangID(co.Getlanguage(lang).szCode);
   CString csLCID;
   ps.ConvertSCOTLangID2LCID(csLangID, csLCID);
   return csLCID;
}
//************************************************************************ 
//
// Function:		GetKeyboardContextOfCurrentLang
// Purpose:			Get the context for the keyboard corresponding to the current language
// Parameters:				  						 
// Returns:			A CString value that contains the context for the keyboard corresponding to the current language
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetKeyboardContextOfCurrentLang()
{
   return AppendContextWithLCID(_T("Keyboard"));
}

CString PSProceduresBase::AppendContextWithLCID(CString csContextIn)
{
   CString csContext = csContextIn;
   CString csLCID=GetLCID(GetLanguage());
   
   if (co.GetfStateDualLanguage()  &&
       !mp.sCurr->storeMode() &&
       !mp.sCurr->helpMode() &&
       mp.sCurr->m_customerLanguage != m_languageUsed)
   {
       //CPSXRedrawLock  csRedrawLock(_T("Display"), _T("MPProcedures::ProcessNextState"));
       csLCID=GetLCID(mp.sCurr->m_customerLanguage);  
   }
   
   
   if(csLCID.IsEmpty())
   {
      csLCID.Format(_T("%04lx"), GetUserDefaultLCID());
   }
   csLCID.MakeLower();
   csContext+=csLCID;

   CString csReference;
   PSXRC rc=m_pPSX->GetCustomDataVar(_T("ContextReference"), csReference, _T(""), csContext);
   if((PSX_SUCCESS==rc) && (!csReference.IsEmpty()))
   {
      csContext=csReference;
   }

   return csContext;
}

//************************************************************************ 
//
// Function:		GetOperatorKeyboardContextOfCurrentLang
// Purpose:			Get the context for the operator keyboard corresponding to the current language
// Parameters:				  						 
// Returns:			A CString value that contains the context for the keyboard corresponding to the current language
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetOperatorKeyboardContextOfCurrentLang()
{
    CString csContext=_T("Operator");

    csContext += GetKeyboardContextOfCurrentLang();
    
    return csContext;
}


//************************************************************************ 
//
// Function:		GetLocaleInfoString
// Purpose:			Get the locale string by LCID
// Parameters:		[in] LCID lcid	  						 
// Returns:			A CString value that contains the locale info
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetLocaleInfoString(LCID lcid)
{
    CString bstrRetBuf;
    TCHAR arcBuf[128];
    memset(arcBuf, 0, sizeof(arcBuf));

    //Loading the English name fo the locale
    GetLocaleInfo( lcid, LOCALE_SENGLANGUAGE, arcBuf, 127);
    bstrRetBuf = arcBuf;
    memset(arcBuf, 0, sizeof(arcBuf));

    //Loading the English name for the country/region
    GetLocaleInfo( lcid, LOCALE_SENGCOUNTRY, arcBuf, 127);
    if( *arcBuf )
    {
        bstrRetBuf += TEXT("_");
        bstrRetBuf += arcBuf;
    }

    //Loading the code page
    memset(arcBuf, 0, sizeof(arcBuf));
    if( (GetLocaleInfo( lcid, LOCALE_IDEFAULTANSICODEPAGE, arcBuf, 127)
        || GetLocaleInfo( lcid, LOCALE_IDEFAULTCODEPAGE, arcBuf, 127))
        &&  *arcBuf )
    {
        bstrRetBuf += TEXT(".");
        bstrRetBuf += arcBuf;
    }

    return bstrRetBuf;
}

#ifndef _CPPUNIT
//************************************************************************ 
//
// Function:		GetISOLanguageCode
// Purpose:			Get the ISO language string by LCID
// Parameters:		[in] CString csLCID	  						 
// Returns:			A CString value that contains the language string
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetISOLanguageCode(CString csLCID)
{
    CString bstrRetBuf;
    TCHAR arcBuf[128];
    memset(arcBuf, 0, sizeof(arcBuf));

    //Loading ISO language code
    GetLocaleInfo( _tcstoul(csLCID, 0, 16), LOCALE_SISO639LANGNAME, arcBuf, 127);
    bstrRetBuf = arcBuf;
    memset(arcBuf, 0, sizeof(arcBuf));

    //Loading ISO country/region code
    GetLocaleInfo( _tcstoul(csLCID, 0, 16), LOCALE_SISO3166CTRYNAME, arcBuf, 127);
    if( *arcBuf )
    {
        bstrRetBuf += TEXT("_");
        bstrRetBuf += arcBuf;
    }

    return bstrRetBuf;
}
#endif //_CPPUNIT
//************************************************************************ 
//
// Function:		SetLocale
// Purpose:			Set locale according to the given language
// Parameters:		[in] SCOT_LANGUAGE_TYPE lang	  						 
// Returns:			
// Calls:			GetLocaleInfoString, GetLCID
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::SetLocale(SCOT_LANGUAGE_TYPE lang)
{
   CString csLCID=GetLCID(lang);
   CString csLocaleInfo=GetLocaleInfoString(_tcstoul(csLCID, 0, 16));
#ifndef _UNICODE
   char* buffer=(char*) csLocaleInfo.GetBuffer(csLocaleInfo.GetLength());
   setlocale(LC_CTYPE, buffer);
#else
   wchar_t* buffer=(wchar_t*) csLocaleInfo.GetBuffer(csLocaleInfo.GetLength());
   _wsetlocale(LC_CTYPE, buffer);
#endif
   trace(L6, _T("Called setlocale %s"), buffer);
   csLocaleInfo.ReleaseBuffer();  
}

//************************************************************************ 
//
// Function:		GetFuzzyChars
// Purpose:			Set the Fuzzy character set for the input character
// Parameters:		[in] TCHAR c
//                [in] csFuzzyChars	  						 
// Returns:			
// Calls:			GetLocaleInfoString, GetLCID
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::GetFuzzyChars(const TCHAR c, CString & csFuzzyChars)
{
   csFuzzyChars=c;

   switch(c)
   {
   case _T('a'):
      csFuzzyChars+=(_T("åáàâäã"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x0105);//a ogonek
#endif
      break;
   case _T('e'):
      csFuzzyChars+=(_T("éèêë"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x0117);//e dot above
      csFuzzyChars+=(wchar_t)(0x0119);//e ogonek
#endif
      break;
   case _T('i'):
      csFuzzyChars+=(_T("íìîï"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x012F);//i ogonek
#endif
      break;
   case _T('n'):
      csFuzzyChars+=(_T("ñ"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x0144);//n acute
#endif
      break;
   case _T('o'):
      csFuzzyChars+=(_T("óòôöõ"));
      break;
   case _T('u'): 
      csFuzzyChars+=(_T("úùûü"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x0173);//u ogonek
      csFuzzyChars+=(wchar_t)(0x016B);//u macron
#endif      
      break;
   case _T('y'):
      csFuzzyChars+=(_T("ýÿ"));
#ifdef _UNICODE
      csFuzzyChars+=(wchar_t)(0x1EF3);//y grave
      csFuzzyChars+=(wchar_t)(0x1077);//y circumflux
      csFuzzyChars+=(wchar_t)(0x1EF9);//y Tilde
#endif
      break;
#ifdef _UNICODE
   case _T('c'):
      csFuzzyChars+=(wchar_t)(0x0107);//c acute 
      csFuzzyChars+=(wchar_t)(0x010D);//c caron 
      break;
   case _T('g'):
      csFuzzyChars+=(wchar_t)(0x011F);//g breve
      break;
   case _T('l'):
      csFuzzyChars+=(wchar_t)(0x0142);//l stroke
      break;
   case _T('s'):
      csFuzzyChars+=(wchar_t)(0x015F);//s cedilla
      csFuzzyChars+=(wchar_t)(0x0161);//s caron
      csFuzzyChars+=(wchar_t)(0x015B);//s acute
      break;
   case _T('w'):
      csFuzzyChars+=(wchar_t)(0x1E83);//w grave
      csFuzzyChars+=(wchar_t)(0x1E82);//w acute
      csFuzzyChars+=(wchar_t)(0x1075);//w circumflux
      csFuzzyChars+=(wchar_t)(0x1E84);//w dlaeresis
      break;
   case _T('z'):
      csFuzzyChars+=(wchar_t)(0x017E);//z caron
      csFuzzyChars+=(wchar_t)(0x017A);//z acute
      csFuzzyChars+=(wchar_t)(0x017C);//z dot above
      break;
#endif
   }
}

#ifndef _CPPUNIT
/**
 * See header file for method information.
 */
void PSProceduresBase::CreateVolCtlThread(bool bSetInitial /* =true */)
{
    ASSERT(m_bPsxInitialized);

    if(m_pVolCtl == NULL)                        // Lazy init.         
    {   
        ASSERT(m_hWaitVolSet != 0);
        m_pVolCtl = new CVolumeControl(bSetInitial? SETDEFAULT:NOFUNCTION, m_hWaitVolSet);  //TAR 404826

            m_pVolCtl->SetDefaultVolume();       // Handle immediate request.
   }
	
	m_pVolCtl->ShowVolumeLogo(); //TAR 404826
}

//SR672 - Enhanced device error display
void PSProceduresBase::SetDevErrURL(LPCTSTR lpszURL)
 {
	 CString sCurrURL = lpszURL;
	 m_csCurrentDevErrURL = sCurrURL;
 }


//+ Personalization picklist
//************************************************************************ 
//
// Function:		LoadPersonalizedPopularList
// Purpose:			This function parse a list of item codes and add them
//                  in m_tempFavoritesFromServer if the item is in ItemList;
//                  send the items not in ItemList back to the personalization
//                  server; add items from m_StorePopularItemList to 
//                  m_PersonalizedPopularItemList and add item of 
//                  m_tempFavoritesFromServer to m_PersonalizedPopularItemlist base
//                  on the order of item came from the personalization server
//
// Parameters:  [CString] a list of item codes separated by a token, such as ","
// Returns:	        0
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************

long PSProceduresBase::LoadPersonalizedPopularList(const CString& csUPCList)
{
	trace(L6, _T("+PSProceduresBase::LoadPersonalizedPopularList"));

    for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
    {
         m_PersonalizedPopularItemList[i].EmptyList();
         m_tempFavoritesFromServer[i].EmptyList();
    }
    CString csUPCCodeList = csUPCList;
    CString csUPCCode;
    CString csPersonalizedPicklistFailItems; //Items not in ItemList

    int index = 0;
    bool bSortedList = co.GetfOperationsPicklistSortedAlphabetical();
    int nPersonalizedPopularItem = 0;
    while ((index!= -1) && (nPersonalizedPopularItem < _ttoi(custInfo.GetOption(_T("PersonalizedPicklistItems")))))
    {
        bool bFoundInPicklist = false;
        index = csUPCCodeList.Find(TOKEN_SEPARATOR);
        if (index != -1)
        {
            csUPCCode = csUPCCodeList.Left(index);
            csUPCCodeList = csUPCCodeList.Mid(index + _tcsclen(TOKEN_SEPARATOR));
        }
        else //the last item code of the list
        {
            csUPCCode = csUPCCodeList;
        }
               
        for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
        {
            //Add items to m_tempFavoritesFromServer 		
			POSITION pos = ItemList[i].GetHeadPosition();
			CItemInfo* pPopularItem = NULL;
                   
			while( pos != NULL )
			{	
				pPopularItem = (CItemInfo*)ItemList[i].GetNext( pos );
                CString csCode = pPopularItem->GetItemCode();
                    
                if(csUPCCode == csCode) 
                {
                    bFoundInPicklist = true;
				    CString csDesc = pPopularItem->GetItemName();
				    CString csGraphic = pPopularItem->GetItemGraphic();
				    bool bQuickPickItem = pPopularItem->GetQuickPickItem();
				    bool bSubCategory = false;
                    bool bFavorite = true;
				       
					trace(L7, _T("m_tempFavoritesFromServer Item: Code=%s, Desc=%s, Graphic=%s"), csCode, csDesc, csGraphic);
                    CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
                    POSITION pos = m_tempFavoritesFromServer[i].GetTailPosition();
	                m_tempFavoritesFromServer[i].InsertAfter( pos, pCurrentItem );
                    break;
				}
			}
        }
        if(bFoundInPicklist)
        {
            nPersonalizedPopularItem++;
        }
        else
        {
            csPersonalizedPicklistFailItems += csUPCCode;
            csPersonalizedPicklistFailItems += TOKEN_SEPARATOR;
        }
    }
    if (!csPersonalizedPicklistFailItems.IsEmpty())
    {
        csPersonalizedPicklistFailItems.TrimRight(TOKEN_SEPARATOR);//take out the last TOKEN_SEPARATOR
        custInfo.SetDataString(_T("PicklistFavoritesFail"), csPersonalizedPicklistFailItems);
    }
    trace(L6, _T("PicklistFavoritesFail = %s"), csPersonalizedPicklistFailItems);
    trace(L6, _T("Number of personalized popular items displayed: %d"), nPersonalizedPopularItem);

   for ( i=0; i < co.GetNumberOfLanguagesSupported(); i++)
   {
        //Add subcategories to m_PersonalizedPopularItemList
        POSITION pos1 = SubCategoryList[i].GetHeadPosition();
		CCategoryInfo* pItem = NULL;
		while( pos1 != NULL )
		{
			pItem = (CCategoryInfo*)SubCategoryList[i].GetNext( pos1 );
			CString csCategories = pItem->GetCategories();
			CString csCatDesc = pItem->GetCatDescription();
			CString csCatId = pItem->GetCategoryID();
			CString csGraphic = pItem->GetCatGraphic();
			bool bQuickPickItem = pItem->GetQuickPickItem();
			bool bSubCategory = true;
			bool bIsItem = false;

			bool bFavorite = false;
			if ( csCategories.Find(TAB_FAV) != -1 )
			{
				bFavorite = true;
			}
            trace(L7, _T("SubCategory Item: Id=%s, Desc=%s, Graphic=%s"), csCatId, csCatDesc, csGraphic);
            if (bFavorite)
            { 
                m_PersonalizedPopularItemList[i].InsertInOrder(csCatDesc, csCatId, csGraphic, bFavorite,
                                            bSubCategory, csCategories, _T(""), _T(""), bQuickPickItem, 
                                            bIsItem, _T(""), bSortedList);
            }   
        }
        //Add items of m_StorePopularItemList to m_PersonalizedPopularItemList
		POSITION pos2 = m_StorePopularItemList[i].GetHeadPosition();
		CItemInfo* pPopularItem = NULL;
   
	    while ( pos2 != NULL ) 
		{	
			pPopularItem = (CItemInfo*)m_StorePopularItemList[i].GetNext( pos2 );

			CString csDesc = pPopularItem->GetItemName();
			CString csCode = pPopularItem->GetItemCode();
			CString csGraphic = pPopularItem->GetItemGraphic();
			bool bFavorite = pPopularItem->GetIsFavorite();
			bool bQuickPickItem = pPopularItem->GetQuickPickItem();
			bool bSubCategory = false;

            POSITION pos3 = m_tempFavoritesFromServer[i].GetHeadPosition();
            CItemInfo* pFavoritesItem = NULL;
            bool bFound = false;
                
		    while ( pos3 != NULL )
            {
                pFavoritesItem = (CItemInfo*)m_tempFavoritesFromServer[i].GetNext( pos3 );
                CString csFavoritesItemCode = pFavoritesItem->GetItemCode();
                if (csCode == csFavoritesItemCode) //Do not add item already in m_tempFavoritesFromServer
                {
                    bFound = true;
                    break;
                }
            }
            if(!bFound)
            {  
                m_PersonalizedPopularItemList[i].InsertInOrder(csDesc, csCode, csGraphic, bFavorite, 
                    bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem, true, _T(""), bSortedList);
                bFound = false;
            }
	    }
        //Add m_tempFavoritesFromServer to m_PersonalizedPopularItemList in the order of items came from
        //Personalization server
        POSITION pos4 = m_tempFavoritesFromServer[i].GetTailPosition();
		CItemInfo* pPersonalItem = NULL;
       
	    while ( pos4 != NULL ) 
		{	
			pPersonalItem = (CItemInfo*)m_tempFavoritesFromServer[i].GetPrev( pos4 );

			CString csDesc = pPersonalItem->GetItemName();
			CString csCode = pPersonalItem->GetItemCode();
			CString csGraphic = pPersonalItem->GetItemGraphic();
			bool bFavorite = pPersonalItem->GetIsFavorite();
			bool bQuickPickItem = pPersonalItem->GetQuickPickItem();
			bool bSubCategory = false;
            CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
	        POSITION pos = m_PersonalizedPopularItemList[i].GetHeadPosition();
	        m_PersonalizedPopularItemList[i].InsertBefore( pos, pCurrentItem );
        }
          
	}
    UpdateFailedDisplayPersonalPicklistItems(csUPCList);

    trace(L6, _T("-PSProceduresBase::LoadPersonalizedPopularList"));
    return 0;
}

//************************************************************************ 
//
// Function:		AddItemToList
// Purpose:			Add sold items to a CStringArray which
//                  will exclude the duplicate item; this function is called by
//                  UpdatePicklistFavoritesMetrics()
//
// Parameters:		[CString] csItemSent - Item code, [CStringArray] csPersonalizedPickList	  						 
// Returns:			None
// Calls:			
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::AddItemToList(const CString csItemSent, CStringArray& csPersonalizedPicklist)
{
    CString csItemSold = csItemSent;
  
    bool bFound = false;
    
    for (int i=0; i < csPersonalizedPicklist.GetSize(); i++)
    {
        if (csPersonalizedPicklist.GetAt(i)== csItemSold)
        {
            bFound = true;
            break;
        }
    }

    if (!bFound) //there is no same item in the csMyPickList
    {
        csPersonalizedPicklist.Add(csItemSold);
        trace(L7, _T("ItemCode %s is added"), csItemSold);
    }
}

//************************************************************************ 
//
// Function:		UpdatePicklistFavoritesMetrics
// Purpose:			Parse a CStringArray which contains the picklist sold items info 
//                  with the format as: itemcode@@category ;check to see if the picklist
//                  sold item belongs to m_tempFavoritesFromServer, if not, then check to see if
//                  the sold item belongs to m_StorePopularItemList. If it belongs to neither 
//                  of them, then it must belongs to other group.  Update the metrics 
//                  to the server;this function is called by UpdatePicklistSoldItemData()
//
// Parameters:  [CStringArray] picklist sold item info, such as 1234@@Favorites
// Returns:	        none
// Calls:           
// Side Effects:
// Notes: The same item can be picked from favorites tab or other categories. In
//        order to distiguish them, check the TAB_FAV
//
//**************************************************************************
void PSProceduresBase::UpdatePicklistFavoritesMetrics(CStringArray& csPicklistSoldItems, CStringArray& csQuickPickItemSold)
{
    int index = 0;
    CString csPicklistSold, csInCategory, csPersonalizedPicklistItemCode, csItemCode;
    CStringArray csPersonalizedPicklist;
    CItemInfo* pItem = NULL;
    long lPersonalFavorites = 0; 
    long lStoreFavorites = 0; 
    long lOtherFavorites = 0;
        for ( int i=0; i < csPicklistSoldItems.GetSize(); i++)
        {  
            csPicklistSold = csPicklistSoldItems.GetAt(i);
            trace(L7, _T("PicklistSold = %s"), csPicklistSold);
   
            index = csPicklistSold.Find(STR_SEPARATOR);
            if (index != -1)
            {
                csPersonalizedPicklistItemCode = csPicklistSold.Left(index);
                AddItemToList(csPersonalizedPicklistItemCode,csPersonalizedPicklist);
                csInCategory = csPicklistSold.Mid(index + _tcsclen(STR_SEPARATOR));
            }
            bool bFound = false;
            POSITION pos = m_tempFavoritesFromServer[m_languageUsed].GetHeadPosition();
            while (pos !=NULL)
            {
                pItem = (CItemInfo*)m_tempFavoritesFromServer[m_languageUsed].GetNext(pos);
                csItemCode = pItem->GetItemCode();
                if ((csPersonalizedPicklistItemCode == csItemCode) && (csInCategory == TAB_FAV))
                {
                    lPersonalFavorites++;
                    bFound = true;
                    trace(L7, _T("Item code %s is in PersonalFavorites"), csPersonalizedPicklistItemCode);
                    break;
                }
            }
            if (!bFound)
            {
                POSITION pos1 = m_StorePopularItemList[m_languageUsed].GetHeadPosition();
                 
			    while ( pos1 != NULL ) 
			    {	
				    pItem = (CItemInfo*)m_StorePopularItemList[i].GetNext( pos1 );
                    csItemCode = pItem->GetItemCode();
                    if ((csPersonalizedPicklistItemCode == csItemCode) && (csInCategory == TAB_FAV))
                    {
                        lStoreFavorites++;
                        bFound = true;
                        trace(L7, _T("Item code %s is in StoreFavorites"), csPersonalizedPicklistItemCode);
                        break;
                    }
                  }
             }
               
        } 
        lOtherFavorites = (csPicklistSoldItems.GetSize() - lPersonalFavorites - lStoreFavorites);
        if (lPersonalFavorites > 0)
          custInfo.SetMetricLong(_T("PicklistPersonalFavoritesSold"), lPersonalFavorites);
        if (lStoreFavorites >0)
          custInfo.SetMetricLong(_T("PicklistStoreFavoritesSold"), lStoreFavorites);
        if (lOtherFavorites >0)
          custInfo.SetMetricLong(_T("PicklistOtherSold"), lOtherFavorites);
        trace(L6, _T("PicklistPersonalFavoritesSold = %1d, PicklistStoreFavoritesSold = %1d, PicklistOtherSold = %1d"),
            lPersonalFavorites, lStoreFavorites, lOtherFavorites);

		//+ Dynamic QuickPick
        if (csQuickPickItemSold.GetSize())
        {
           ps.UpdateQuickPickSoldItem(csQuickPickItemSold, csPersonalizedPicklist);
        }
        //- Dynamic QuickPick

        CString csPersonalizedPicklistItems;
        for ( int j=0; j < csPersonalizedPicklist.GetSize(); j++)
        {  
            csPersonalizedPicklistItems += csPersonalizedPicklist.GetAt(j);
            if (j < (csPersonalizedPicklist.GetSize() - 1))//Do not add TOKEN_SEPARATOR on the last entry
            {
                csPersonalizedPicklistItems += TOKEN_SEPARATOR;
            }
        }
        if (!csPersonalizedPicklistItems.IsEmpty())
        {
            custInfo.SetDataString(_T("PicklistFavoritesUpdate"), csPersonalizedPicklistItems);
            trace(L6, _T("PicklistFavoritesUpdate = %s"), csPersonalizedPicklistItems);
        }
}

//************************************************************************ 
//
// Function:		UpdatePicklistSoldItemData
// Purpose:			This function parse a CStringArray which contains the picklist
//                  sold items info with the format as: time@@itemcode@@category
//                  update the picklist sold item metrics to the server
//
// Parameters:  [CStringArray] picklist sold item info, such as 31000@@1234@@Favorites
// Returns:	        none
// Calls:           
// Side Effects:
// Notes:
//
//**************************************************************************
void PSProceduresBase::UpdatePicklistSoldItemData(const CStringArray& csPicklistItemSoldInfo, CStringArray& csQuickPickItemSold)
{
    trace(L6, _T("+PSProceduresBase::UpdatePicklistSoldItemData"));
    int index = 0;
    CString csPicklistTime, csPicklistItemSold;
    CStringArray csPicklistSoldItems;
    for (int i=0; i < csPicklistItemSoldInfo.GetSize(); i++)
    {
        csPicklistItemSold = csPicklistItemSoldInfo.GetAt(i);
        trace(L6, _T("csPicklistItemSold = %s"), csPicklistItemSold);
   
        index = csPicklistItemSold.Find(STR_SEPARATOR);
        if (index != -1)
        {
            csPicklistTime += csPicklistItemSold.Left(index);
            csPicklistSoldItems.Add(csPicklistItemSold.Mid(index + _tcsclen(STR_SEPARATOR)));
        }
        if (i < (csPicklistItemSoldInfo.GetSize() - 1)) //Do not add TOKEN_SEPARATOR on the last entry
        {
            csPicklistTime += TOKEN_SEPARATOR;
        }
        
    }
    UpdatePicklistFavoritesMetrics(csPicklistSoldItems, csQuickPickItemSold);
    if (!csPicklistTime.IsEmpty())
    {
        custInfo.SetDataString(_T("TimeSelectingPicklistItems"), csPicklistTime);
        trace(L6, _T("TimeSelectingPicklistItems = %s "), csPicklistTime);
    }
    trace(L6, _T("-PSProceduresBase::UpdatePicklistSoldItemData"));
}

//************************************************************************ 
//
// Function:		UpdateFailedDisplayPersonalPicklistItems
// Purpose:			Parse a list of Personalized item and calculate the number
//                  of items that came from the server but were not displayed 
//                  on the favorites tab, and update the data to the server; this
//                  function is called by LoadPersonalizedPopularList()
//
// Parameters:  [CString] a list of item codes separated by a token, such as ","
// Returns:	        none
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************
void PSProceduresBase::UpdateFailedDisplayPersonalPicklistItems(const CString& csUPCList)
{
    long lPersonalizedPopularItem = m_tempFavoritesFromServer[m_languageUsed].GetCount();
    CString csUPCCodeList = csUPCList;
    int index = 0; 
    int lPersonalPicklistItemFromServer = 0;
    while ((index != -1) && !csUPCCodeList.IsEmpty())
    {
        index = csUPCCodeList.Find(TOKEN_SEPARATOR);
        csUPCCodeList = csUPCCodeList.Mid(index +_tcsclen(TOKEN_SEPARATOR));
        lPersonalPicklistItemFromServer++;
    }
    long lItemFailedDisplay = lPersonalPicklistItemFromServer - lPersonalizedPopularItem;
    if (lItemFailedDisplay > 0)
    {
        custInfo.SetMetricLong(_T("PicklistPersonalFavoritesNotDisplay"), lItemFailedDisplay);
        trace (L6, _T("PicklistPersonalFavoritesNotDisplay = %ld"), lItemFailedDisplay);
    }

}
//- Personalization picklist

//+ Personalization age prompt
void PSProceduresBase::SetInputText(LPCTSTR lpszInput) 
{
    _tcscpy(szInputDigits,lpszInput);
}
//- Personalization age prompt


//+ Dynamic Quickpick
//************************************************************************ 
//
// Function:        LoadPersonalizedQuickPickList
// Purpose:         This function parses a list of item codes from the store server and 
//                  adds them in to m_tempFavoritesFromServer.  
//                  ; add items from m_tempFavoritesFromServer to m_PersonalizedQuickPickItemList based
//                  on the order of item came from the personalization server; add items from
//                  QuickPickItemList to m_PersonalizedQuickPickItemList 
//                  
//                  
//
// Parameters:  [CString] a list of item codes separated by a token, such as ","
// Returns:         0
// Calls:           
// Side Effects:
// Notes:
//
//************************************************************************

long PSProceduresBase::LoadPersonalizedQuickPickList(const CString& csUPCList)
{
    trace(L6, _T("+PSProceduresBase::LoadPersonalizedQuickPickList"));

  
     for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
    {
         m_PersonalizedQuickPickItemList[i].EmptyList();
         m_tempFavoritesFromServer[i].EmptyList();
    }

    CString csUPCCodeList = csUPCList;
    CString csUPCCode;
    CString cdDefaultQuickPickItems; //Item codes of items from the store's default quickpick

    int index = 0;
       
    int nPersonalizedQuickpickItem = 0;

   
    while ((index!= -1) && (nPersonalizedQuickpickItem < _ttoi(custInfo.GetOption(_T("DynamicQuickPickItems")))))
    {
        bool bFoundInPicklist = false;
        index = csUPCCodeList.Find(TOKEN_SEPARATOR);
        if (index != -1)
        {
            csUPCCode = csUPCCodeList.Left(index);
            csUPCCodeList = csUPCCodeList.Mid(index + _tcsclen(TOKEN_SEPARATOR));
        }
        else //the last item code of the list
        {
            csUPCCode = csUPCCodeList;
        }
               
        for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
        {
            //Add items to m_tempFavoritesFromServer        
            POSITION pos = ItemList[i].GetHeadPosition();
            CItemInfo* pPopularItem = NULL;
                   
            while( pos != NULL )
            {   
                pPopularItem = (CItemInfo*)ItemList[i].GetNext( pos );
                CString csCode = pPopularItem->GetItemCode();
                    
                if(csUPCCode == csCode) 
                {
                    bFoundInPicklist = true;
                    CString csDesc = pPopularItem->GetItemName();
                    CString csGraphic = pPopularItem->GetItemGraphic();
                    bool bQuickPickItem = pPopularItem->GetQuickPickItem();
                    bool bSubCategory = false;
                    bool bFavorite = true;
                       
                    trace(L7, _T("m_tempFavoritesFromServer Item: Code=%s, Desc=%s, Graphic=%s"), csCode, csDesc, csGraphic);
                    CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
                    POSITION pos = m_tempFavoritesFromServer[i].GetTailPosition();
                    m_tempFavoritesFromServer[i].InsertAfter( pos, pCurrentItem );
                    break;
                }
            }
        }
        if(bFoundInPicklist)   
        {
            nPersonalizedQuickpickItem++;
        }
      
    }

    trace(L6, _T("Number of personalized popular items displayed as Quickpick: %d"), nPersonalizedQuickpickItem);
  

   for ( i=0; i < co.GetNumberOfLanguagesSupported(); i++)
   {
        //Add m_tempFavoritesFromServer to m_PersonalizedQuickPickItemList 
        //In the order of items came fromPersonalization server
        POSITION pos2 = m_tempFavoritesFromServer[i].GetTailPosition();
        CItemInfo* pPersonalItem = NULL;
       
        while ( pos2 != NULL ) 
        {   
            pPersonalItem = (CItemInfo*)m_tempFavoritesFromServer[i].GetPrev( pos2 );

            CString csDesc = pPersonalItem->GetItemName();
            CString csCode = pPersonalItem->GetItemCode();
            CString csGraphic = pPersonalItem->GetItemGraphic();
            bool bFavorite = pPersonalItem->GetIsFavorite();
            bool bQuickPickItem = pPersonalItem->GetQuickPickItem();
            bool bSubCategory = false;
            CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
            POSITION pos = m_PersonalizedQuickPickItemList[i].GetHeadPosition();
            m_PersonalizedQuickPickItemList[i].InsertBefore( pos, pCurrentItem );
        }
          

        //Add QuickPickItemList to m_PersonalizedQuickPickItemList
        POSITION pos3 = QuickPickItemList[i].GetHeadPosition();
        CItemInfo* pQuickPickItem = NULL;
   
        while ( pos3 != NULL ) 
        {   
            pQuickPickItem = (CItemInfo*)QuickPickItemList[i].GetNext( pos3 );

            CString csDesc = pQuickPickItem->GetItemName();
            CString csCode = pQuickPickItem->GetItemCode();
            CString csGraphic = pQuickPickItem->GetItemGraphic();
            bool bFavorite = pQuickPickItem->GetIsFavorite();
            bool bQuickPickItem = pQuickPickItem->GetQuickPickItem();
            bool bSubCategory = false;


            POSITION pos4 = m_tempFavoritesFromServer[i].GetHeadPosition();
            CItemInfo* pTempPersonalItem = NULL;
            bool bFound = false;
            while ( pos4 != NULL ) 
            {   
                pTempPersonalItem = (CItemInfo*)m_tempFavoritesFromServer[i].GetNext( pos4 );
                CString csTargetCode = pTempPersonalItem->GetItemCode();

                if (csTargetCode == csCode) //Do not add item already in m_tempFavoritesFromServer
                {
                    bFound = true;
                    break;
                }
            }

            if (!bFound)
            {

                bFound = false;

                CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
                POSITION pos = m_PersonalizedQuickPickItemList[i].GetTailPosition();
                m_PersonalizedQuickPickItemList[i].InsertAfter( pos, pCurrentItem );
                 
            }

        }

    }
               
    trace(L6, _T("-PSProceduresBase::LoadPersonalizedQuickPickList"));
    return 0;
}

//************************************************************************ 
//
// Function:        UpdateQuickPickSoldItem
// Purpose:         This function parse a CStringArray which contains the picklist
//                        sold item's item code and merges them into csPersonalizedPicklistItems.
//                  
//
// Parameters:  [CStringArray] picklist sold item's item code,
// Returns:        none
// Calls:           
// Side Effects:
// Notes:
//
//**************************************************************************
void PSProceduresBase::UpdateQuickPickSoldItem(const CStringArray& csQuickPickItemSoldList, CStringArray& csPersonalizedPicklistItems)
{
    trace(L6, _T("+PSProceduresBase::UpdateQuickPickSoldItem"));
    int index = 0;

    CString csQuickPickItemSold;

    for (int i=0; i < csQuickPickItemSoldList.GetSize(); i++)
    {

        csQuickPickItemSold = csQuickPickItemSoldList.GetAt(i);

        trace(L6, _T("QuickPick Sold Item:: %s"), csQuickPickItemSold );

        AddItemToList(csQuickPickItemSold,csPersonalizedPicklistItems);
               
    }


    trace(L6, _T("-PSProceduresBase::UpdateQuickPickSoldItem"));
}
//- Dynamic Quickpick
#endif //_CPPUNIT

// (+) SR804
void PSProceduresBase::EchoAmountPaidToReceipt(int nTenderType, long lAmountPaid)
{
    CString csLeftCol;
    //sscoadk-3121: use the tender description from tenderDetail if it is available
    if (!m_csTenderDescription.IsEmpty()) 
    {
        csLeftCol = m_csTenderDescription;   
    }
    else
    {
        csLeftCol.Format( _T("%s"),
                      m_TenderBtnMapByBtnNum[nTenderType] );
    }


    CString csRightCol;
    csRightCol.Format( _T("%s"), 
                       FmtDollar(lAmountPaid) );

    COleVariant vMsg(csLeftCol);
    COleVariant vDue(csRightCol);

    // create item entry
    CString csItemEntryId;    
    // generate unique id for this entry
    csItemEntryId.Format(_T("amount-%d"), GetTickCount());

    PSXRC rc = m_pPSX->CreateReceiptItem(_T("CMSMReceipt"), csItemEntryId);
    if (PSX_SUCCESS == rc)
    {
        // modify variables for this entry
        m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_MSG"), vMsg);
        m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("ITEM_EXT_PRICE"), vDue);

        // update this entry
        m_pPSX->UpdateReceiptControls(_T("CMSMReceipt"));
    }
}  
// (-) SR804

#ifndef _CPPUNIT
//SR828
//************************************************************************ 
//
// Function:		PopulateHKSensitiveControls
// Purpose:			This function populates the controls in the array m_HKSensitiveControls on the keyboard
// Parameters:      csCtxName, hkStatus, bShiftOn, bAltGrOn, bSymbolOn
// Returns:	        
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
void PSProceduresBase::PopulateHKSensitiveControls(CString csCtxName, HKStatus hkStatus, bool bShiftOn, bool bAltGrOn, bool bSymbolOn)
{
  CString csCustDataName;
  switch(hkStatus)
  {
    case HK_NORMAL:
      csCustDataName=_T("Text");
      break;
    case HK_ACUTEON:
      csCustDataName=_T("TextAcuteOn");
      break;
    case HK_GRAVEON:
      csCustDataName=_T("TextGraveOn");
      break;
    case HK_CIRCUMFLEXON:
      csCustDataName=_T("TextCircumflexOn");
      break;
    case HK_UMLAUTON:
      csCustDataName=_T("TextUmlautOn");
      break;
    case HK_TILDEON:
      csCustDataName=_T("TextTildeOn");
      break;
  }

  if(bShiftOn)
     csCustDataName+=_T("ShiftOn");
  if(bAltGrOn)
     csCustDataName+=_T("AltGrOn");
  if(bSymbolOn)
      csCustDataName=_T("TextSymbolOn");

  CLaneStateData laneStateData(m_pPSX);

  CString csValue = laneStateData.CreateNextGenUIHotKeyState(csCustDataName, bShiftOn);
  laneStateData.SendNextGenUIData(_T("NextGenUIHotKeyState"), csValue );

  for(int i=0; i<m_HKSensitiveControls.GetSize(); i++)
  {
    CString csCustDataValue=_T("");
    CString csControl=m_HKSensitiveControls[i]; 
    
    PSXRC rc=m_pPSX->GetCustomDataVar(csCustDataName, csCustDataValue, csControl, csCtxName);

    bool bParseCustDataValue=false;
    if(PSX_SUCCESS==rc && (!csCustDataValue.IsEmpty()))
    {
      bParseCustDataValue=true;
    }
    else
    {
      CString csDefaultText=_T("Text");
      if(bShiftOn)
         csDefaultText+=_T("ShiftOn");
      if(bAltGrOn)
         csDefaultText+=_T("AltGrOn");
      if(bSymbolOn)
          csDefaultText=_T("TextSymbolOn");

      if(csCustDataName!=csDefaultText)
      {
        PSXRC rc=m_pPSX->GetCustomDataVar(csDefaultText, csCustDataValue, csControl, csCtxName);
        if(PSX_SUCCESS==rc && (!csCustDataValue.IsEmpty()))
        {
          bParseCustDataValue=true;
        }
      }
    }

    if(bParseCustDataValue)
    {    
      long nCount=0;
      nCount=GetTokenCount(csCustDataValue, _T(' '));

      COleVariant v, vReturn;
      COleSafeArray saTextArray;
      COleSafeArray saDataArray;
      saTextArray.Create(VT_BSTR, 1, (DWORD*)&nCount);
      saDataArray.Create(VT_VARIANT, 1, (DWORD*)&nCount);

      nCount=0;
      while(!csCustDataValue.IsEmpty())
      {       
       int nIndex=0;
       nIndex=csCustDataValue.Find(_T(" "));         
       CString csText;
       if(nIndex>=0)
       {
          csText=csCustDataValue.Left(nIndex);
          csCustDataValue=csCustDataValue.Right(csCustDataValue.GetLength()-nIndex-1);
       }
       else
       {
          csText=csCustDataValue;
          csCustDataValue=_T("");
       }

       BSTR bstr=csText.AllocSysString();
       saTextArray.PutElement(&nCount, bstr);
       ::SysFreeString(bstr);

       CString csData=csText;
       COleVariant strVar(csText+_T(";"));//_T(";") is used as the padding to force psx to return strings for the number keys. 
                                          //We ignore the padding in SMStateBase::PSParse(...) when calling PSCharKey(...)
       VARIANT vData=strVar.Detach();
       saDataArray.PutElement(&nCount, &vData);

       nCount++;
      }

      v=nCount;
      m_pPSX->SetConfigProperty ( m_HKSensitiveControls[i], csCtxName, UI::PROPERTYREDRAW, VARIANT_FALSE );
      m_pPSX->SetControlProperty( m_HKSensitiveControls[i], UI::PROPERTYBUTTONCOUNT, 0L );
      m_pPSX->SetControlProperty( m_HKSensitiveControls[i], UI::PROPERTYBUTTONCOUNT, v );
      if( nCount )
      {
	      v = saTextArray;
	      m_pPSX->SendCommand( m_HKSensitiveControls[i], UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

	      v = saDataArray;
	      m_pPSX->SendCommand( m_HKSensitiveControls[i], UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );
      }

      m_pPSX->SetConfigProperty( m_HKSensitiveControls[i], csCtxName, UI::PROPERTYREDRAW, VARIANT_TRUE);
    }
    else
    {
        if(bSymbolOn)
        {
            trace(L6,_T("TextSymbolOn empty data. Empty buttonlist [%s]"),m_HKSensitiveControls[i]);
           // m_pPSX->SetControlProperty( m_HKSensitiveControls[i], UI::PROPERTYBUTTONCOUNT, 0L );
        }
    }
  }
}

//SR828
//************************************************************************ 
//
// Function:		PopulateHotKeys
// Purpose:			This function populates the "hotkeys" control (a buttonlist control) on the keyboard
// Parameters:      csCtxName
// Returns:	        
// Calls:
// Side Effects:
// Notes:           
//
//************************************************************************
void PSProceduresBase::PopulateHotKeys(CString csCtxName)
{
  COleVariant v, vReturn;

  long nTextCount=0;
  long nDataCount=0;

  CString csText;
  CString csData;

  CString csHotKeysCtrl = (csCtxName.Find(_T("ProduceFavorites")) != -1 || csCtxName.Find(_T("LookUpItem")) != -1 ) ? _T("PFKBHotKeys") : _T("HotKeys");
trace(L6, _T("PSProceduresBase::PopulateHotKeys csHotKeysCtrl = %s"), csHotKeysCtrl);
  PSXRC rcText=m_pPSX->GetCustomDataVar(_T("Text"), csText, csHotKeysCtrl, csCtxName);
  PSXRC rcData=m_pPSX->GetCustomDataVar(_T("Data"), csData, csHotKeysCtrl, csCtxName);

  if((PSX_SUCCESS==rcText) && (!csText.IsEmpty()))
  {
    nTextCount=0;
    nTextCount=GetTokenCount(csText, _T(' '));
  }

  if((PSX_SUCCESS==rcData) && (!csData.IsEmpty()))
  {
    nDataCount=0;
    nDataCount=GetTokenCount(csData, _T(' '));
  }

  if(nTextCount!=nDataCount)
  {
    trace(L7, _T("nTextCount %l does not equal nDataCount %l"), nTextCount, nDataCount);
    return;
  }

  COleSafeArray saTextArray;
  saTextArray.Create(VT_BSTR, 1, (DWORD*)&nTextCount);
  nTextCount=0;
  while(!csText.IsEmpty())
  {       
    int nIndex=0;
    nIndex=csText.Find(_T(" "));         
    CString csToken;
    if(nIndex>=0)
    {
      csToken=csText.Left(nIndex);
      csText=csText.Right(csText.GetLength()-nIndex-1);
    }
    else
    {
      csToken=csText;
      csText=_T("");
    }

    BSTR bstr=csToken.AllocSysString();
    saTextArray.PutElement(&nTextCount, bstr);
    ::SysFreeString(bstr);

    nTextCount++;
  }

  COleSafeArray saDataArray;
  saDataArray.Create(VT_VARIANT, 1, (DWORD*)&nDataCount);
  nDataCount=0;
  while(!csData.IsEmpty())
  {       
    int nIndex=0;
    nIndex=csData.Find(_T(" "));         
    CString csToken;
    if(nIndex>=0)
    {
      csToken=csData.Left(nIndex);
      csData=csData.Right(csData.GetLength()-nIndex-1);
    }
    else
    {
      csToken=csData;
      csData=_T("");
    }

    if(csToken.IsEmpty())
     csToken=_T("HKNormal");

    COleVariant strVar(csToken);
    VARIANT vData=strVar.Detach();
    saDataArray.PutElement(&nDataCount, &vData);

    nDataCount++;

    trace(L7, _T("csToken is %s, csData %s"), csToken, csData);         
  }

  v=nTextCount;
  m_pPSX->SetConfigProperty(csHotKeysCtrl, csCtxName, UI::PROPERTYREDRAW, VARIANT_FALSE);
  m_pPSX->SetControlProperty( csHotKeysCtrl,UI::PROPERTYBUTTONCOUNT, v );

  if( nTextCount )
  {
	  v = saTextArray;
	  m_pPSX->SendCommand( csHotKeysCtrl, UI::COMMANDSETBUTTONTEXTLIST, vReturn, 1, v );

	  v = saDataArray;
	  m_pPSX->SendCommand( csHotKeysCtrl, UI::COMMANDSETBUTTONDATALIST, vReturn, 1, v );

      for( long i = 0L; i < nTextCount; i++ )
      {
         COleVariant pvParams[2];
         pvParams[0]=i;
         pvParams[1]=(long) UI::Normal;
         m_pPSX->SendCommand( csHotKeysCtrl, UI::COMMANDSETBUTTONSTATE, vReturn, 2, pvParams );
      }
  }

  m_pPSX->SetConfigProperty(csHotKeysCtrl, csCtxName, UI::PROPERTYREDRAW, VARIANT_TRUE);
}

//SR828
long PSProceduresBase::GetTokenCount(const CString& csStr, const TCHAR cDelimiter)
{
  long nCount=0;
  for(int i=0; i<csStr.GetLength(); i++)
  {
     if(csStr.GetAt(i)==cDelimiter)
     {
        nCount++;
     }
   }
   if(csStr.GetAt(csStr.GetLength()-1)!=cDelimiter)
   {
     nCount++;
   }
   return nCount;
}

#endif //_CPPUNIT
//+SSCOB-1860 
//************************************************************************ 
//
// Function:		GetCustomerMessageDefault
// Purpose:			Method will retrieve a translated version of the
//                  CustomerMessage ID from Scotopts, or will
//                  return blank if -1 is message id.
// Parameters:      None
// Returns:			CStroing
// Calls:			GetPSText
// Side Effects:
// Notes:
//
//************************************************************************
CString PSProceduresBase::GetCustomerMessageDefault()
{
    CString csCustomerMessage;
    if( -1 == co.GetnCMCustomerMessage() )
    {
        // so, customer name/message has to hide
        csCustomerMessage.Empty();
    }
    else
    {
        csCustomerMessage = ps.GetPSText( co.GetnCMCustomerMessage() );
    }
    
    return csCustomerMessage;
}


//-SSCOB-1860
//+SSCOP-3531
CString PSProceduresBase::GetCurrentContext()
{
    if (m_csCurrentContext.Find(_T("_KeyPressed")) != -1)
    {
        m_csCurrentContext.Replace(_T("_KeyPressed"), _T(""));
    }
    return m_csCurrentContext;
}
//-SSCOP-3531


bool PSProceduresBase::IsPicklistAssistItemListEmpty(void)
{
    return  (m_PicklistAssistItemList[m_languageUsed].IsEmpty() != 0) ? true : false;
}

void PSProceduresBase::ClearPicklistAssistItemList(void)
{
	trace(L6, _T("+PSProceduresBase::ClearPicklistAssistItemList"));
	for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
    {
		 m_PicklistAssistItemList[i].EmptyList();
    }
}


long PSProceduresBase::LoadPicklistAssistItemList(const CString& csPLAItemList)
{
	trace(L6, _T("+PSProceduresBase::LoadItemList"));
	
    for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
    {
         m_PicklistAssistItemList[i].EmptyList();
    }

    CString csUPCCodeList = csPLAItemList;
    CString csUPCCode;

    int index = 0;
       
    int nItemListCount = 0;
    while (index!= -1)
    {
        bool bFoundInPicklist = false;
        bool bLastItem = false;
        index = csUPCCodeList.Find(TOKEN_SEPARATOR);
        if (index != -1)
        {
            csUPCCode = csUPCCodeList.Left(index);
            csUPCCodeList = csUPCCodeList.Mid(index + _tcsclen(TOKEN_SEPARATOR));
        }
        else //the last item code of the list
        {
            csUPCCode = csUPCCodeList;
            bLastItem = true;
        }
             
        for (int i=0; i < co.GetNumberOfLanguagesSupported(); i++)
        {
            //Add items to m_PicklistAssistItemList 		
			POSITION pos = ItemList[i].GetHeadPosition();
			CItemInfo* pPopularItem = NULL;
                   
			while( pos != NULL )
			{	
				pPopularItem = (CItemInfo*)ItemList[i].GetNext( pos );
                CString csCode = pPopularItem->GetItemCode();
                trace(L7, _T("m_PicklistAssistItemList Item: Code=%s"), csCode);
				
                if(csUPCCode == csCode) 
                {
                    bFoundInPicklist = true;
				    CString csDesc = pPopularItem->GetItemName();
				    CString csGraphic = pPopularItem->GetItemGraphic();
					bool bFavorite = pPopularItem->GetIsFavorite();
				    bool bQuickPickItem = pPopularItem->GetQuickPickItem();
				    bool bSubCategory = false;
                    
				       
					trace(L7, _T("m_PicklistAssistItemList Item: Code=%s, Desc=%s, Graphic=%s"), csCode, csDesc, csGraphic);
                    CItemInfo *pCurrentItem = new CItemInfo(csDesc, csCode, csGraphic, bFavorite, bSubCategory, _T(""), _T(""), _T(""), bQuickPickItem);
                    POSITION posAssist = m_PicklistAssistItemList[i].GetTailPosition();
	                m_PicklistAssistItemList[i].InsertAfter( posAssist, pCurrentItem );
                    break;
				}
			}
            if (bLastItem) //bwdFav Add "FavoritesSubCategory" button    
            {
                //Add items to m_PicklistAssistItemList             
                POSITION pos1 = MainCategoryList[i].GetHeadPosition();
                CCategoryInfo* pItem = NULL;
                bool bFound = false;
                while( pos1 != NULL && !bFound)
                {
                    pItem = (CCategoryInfo*)SubCategoryList[i].GetNext( pos1 );
                    CString csCatId = pItem->GetCategoryID();
                    if (csCatId.Find(TAB_FAV) != -1)
                    {
                        CString csCatDesc = ps.GetPSText(BTT_MORE_POPULAR, (SCOT_LANGUAGE_TYPE) i);
                        CString csCategories = pItem->GetCategories();
                        CString csAlphabetic = pItem->GetCatAlphabetic();			
                        CString csGraphic = pItem->GetCatGraphic();
                        CString csScreenTitle = pItem->GetScreenTitle();
                        CString csInstructions = pItem->GetInstructions();
                        CString csInCategories = TAB_FAV;

                        //switch description and ID for button handling
                        CCategoryInfo *pNewCatItem = new CCategoryInfo( csCatDesc, csCatId, csCategories, csAlphabetic, csGraphic, csScreenTitle, csInstructions, csInCategories );
                        POSITION posAssist = m_PicklistAssistItemList[i].GetTailPosition();
                        m_PicklistAssistItemList[i].InsertAfter( posAssist, pNewCatItem );
                        bFound = true;
                        //nItemListCount++;
                    }
                }
            }
        }
        if(bFoundInPicklist)
        {
            nItemListCount++;
        }
    }
    trace(L6, _T("Number of PicklistAssist items displayed: %d"), nItemListCount);
 
    trace(L6, _T("-PSProceduresBase::LoadPicklistAssistItemList"));
    return 0;
}

bool PSProceduresBase::IsPickListItem(const CString& csItemCode)
{
    trace(L6, _T("+PSProceduresBase::IsPickListItem"));
    bool returnBoolean = false;
#ifndef _CPPUNIT
    CPickListItemInfo plaCurrentPickListItem;
    returnBoolean = m_mPickListItemsMap.Lookup( csItemCode, plaCurrentPickListItem );
#endif //_CPPUNIT
    trace(L6, _T("PSProceduresBase::IsPickListItem returns: %d"), returnBoolean);
 
    trace(L6, _T("-PSProceduresBase::IsPickListItem"));
    return returnBoolean;
}

//+SSCOADK-1021 
bool PSProceduresBase::IsInXM()
{
	return m_bInXM;
}	

void PSProceduresBase::SetInXM(bool bInXM)
{
	m_bInXM = bInXM;
}
//-SSCOADK-1021 

void PSProceduresBase::LoadLanguageUIStrings(const int nLanguageID)
{

    CString csCode;
    ps.ConvertSCOTLangID2LCID(co.Getlanguage(nLanguageID).szCode, csCode);
    trace(L6,_T("LoadAndSaveLanguageStringToMap code[%s]"), csCode);

    _TCHAR *pszStopCharacter;
    int nLangCode = _tcstoul(csCode, &pszStopCharacter, 16); 
    m_pStrMgr->LoadAndSaveLanguageStringToMap(nLanguageID, nLangCode);
}

CString PSProceduresBase::GetcsTenderDescription()
{
    return m_csTenderDescription;
}

void PSProceduresBase::SetcsTenderDescription(CString csValue)
{
    m_csTenderDescription = csValue;
}

//VSTS-267145
CString PSProceduresBase::GetLastItemReceiptEntryId()
{
	return m_csSavedItemEntryId;
}
//VSTS-267145

void PSProceduresBase::SetCurrentItemAsMobileItem(const bool bMobileItem)
{
	if (!io.d.bLinkedItem)
	{
        CString csItemEntryId = io.d.csItemID;
	    COleVariant vValue = VARIANT_FALSE;
        if (bMobileItem)
            vValue = VARIANT_TRUE;

        m_pPSX->SetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("MOBILE_ITEM_FLAG"), vValue);
        ps.GetPSXObject()->UpdateReceiptControls(_T("CMSMReceipt"));
	}
}

bool PSProceduresBase::GetCurrentItemMobileItemFlag()
{
	COleVariant vValue;

    CString csItemEntryId = GetSelection(_T("CMReceiptVoid"));
	m_pPSX->GetReceiptItemVariable(_T("CMSMReceipt"), csItemEntryId, _T("MOBILE_ITEM_FLAG"), vValue);
	vValue.ChangeType( VT_INT );

    return !!vValue.lVal;
}
#ifndef _CPPUNIT
void PSProceduresBase::SetShiftAndSymbolState(const CString csContextName, const bool bSymbolOn)
{
    if(bSymbolOn)
    {
        m_pPSX->SetConfigProperty(_T("ShiftKey1"), csContextName, UI::PROPERTYSTATE, (long)UI::Disabled);
        m_pPSX->SetConfigProperty(_T("SymbolsKey"), csContextName, UI::PROPERTYTEXTFORMAT, 
                                        GetPSText(_T("SymbolButtonTextOn"),m_languageUsed));
    }
    else
    {
        m_pPSX->SetConfigProperty(_T("ShiftKey1"), csContextName, UI::PROPERTYSTATE, (long)UI::Normal);
        m_pPSX->SetConfigProperty(_T("SymbolsKey"), csContextName, UI::PROPERTYTEXTFORMAT, 
                                        GetPSText(_T("SymbolButtonTextOff"),m_languageUsed));
    }

}
#endif