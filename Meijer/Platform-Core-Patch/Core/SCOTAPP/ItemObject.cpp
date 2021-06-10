//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  ItemObject.cpp
//
// TITLE:   ItemObject implementation file
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Common.h"
#include "SMState.h"                // MGV base state catch all

#define COMP_ID ID_IO
#define T_ID _T("IO")

//Used for array of barcode with Supplemental length start
#define NUMOFUPCLENGTHS 8
#define NUMOFSUPPLEMENTLENGTHS 2
// end

/////////////////////////////
void ItemObject::Reset(void)
{
  csOrigCode			= _T("");								  // item code scanned, picked or keyed
  csItemCode			= _T("");								  // item code without the scan type if any
  csOrgItemDescription  = _T("");								  // item description if item is a linked item         
  
  // BeginCodeWLDBSA11
  csItemCodeWOPrice = _T("");					  // SA need a special UPC withoout Price 
                                    // in case of Embedded Price
  // EndCodeWLDBSA11

  lQuantityEntered = 0;						      // should be the same as sold below
  lPriceEntered = 0;							  // in cents, used rarely
  lWeightEntered = 0;							  // in 1/100ths lb or 1/1000 ths kg, with tare
  lUnitPriceEntered = 0;						  // in cents, used rarely
  lExtPriceEntered = 0;                           //Tar 137876 add extended price
  
  lTareCode = 0;									  // tare code 0,1,2...
  lRestCode = 0;									  // 0-no, 1-show msg, 2-no msg, 3-void it, 4-voided
  lCoupCode = 0;									  // 0-not a coupon, 1-both vis and aud, 2-no msg, 3-visual msg only: TAR#102019 & #120972
  lVisualCode = 0;									  // 0-no, 1-show msg, 2-no msg
  cScanType = 0;									  // A-UPC, E-UPC_E, F-EAN
  bBizerbaScale = false; 

  fVoid = false;
  fNotForSale = false;
  nLoyaltyCardScanned = 0;
  b_gIsThisItemPriceEmbedded = false;
  b_gHasCustomCodeBeenFound = false;
  b_gHasSupplementalCodeBeenTruncated = false;
  l_gPriceEmbeddedWt = 0;
  b_glTagDeactivated = false;
  SMStateBase::g_bMatchedReceivedForTAB = false;
  SMStateBase::g_lTABMisMatchApprWt = 0;

  d.Reset();							// reset item details
  xd.Reset();							// reset item extended details
  lUpdateFields = 0;
  SMStateBase::g_lNumberOfWLDBEntriesThisItem = -1;
  SMStateBase::g_bZeroWtExpected = false;  //TAR197477 TAB
  SMStateBase::g_bMatchedWtWaitingOnEAS = false;	//219883
  SMStateBase::m_bEASHappened = false;	//219883
  SMStateBase::ms_bIsCurrentItemFromPickList = false;
  SMStateBase::g_bOnScannerEnable = false;	// TAR 298537
  SMStateBase::g_bOnItemOK = false;			// TAR 298537
  SMStateBase::g_bSkipBaggingButtonTimerExpires = false; //Sr778
}

void ItemDetails::Reset(void)
{
  csDescription = _T("");							  // last item desc
  csItemCode    = _T("");
  
  lExtPrice = 0;									  // last extended price from item sale print line
  lUnitPrice = 0;									  // last unit price for qty or weight
  lQtySold = 0;										  // last qty sold
  lWgtSold = 0;										  // last weight sold
  lDealQty = 0;										  // last deal quantity of the items MGG
  // BeginCodeWLDBSA11
  lDepartment	= 0;									// last department
  // EndCodeWLDBSA11
  
  fQuantityConfirmed = false;
  fQuantityLimitExceeded = false;

  fCoupon = false;                  // item is a coupon
  fQuantityRequired = false;        // item needed quantity
  fWeightRequired = false;          // item neede weight
  fPriceRequired  = false;          // item needed price
  fTareRequired   = false;			// item needs tare code
  fNotFound = false;                // item was not found
  fNotForSale = false;              // item is not for sale, recalled
  fVisualVerify = false;
  
  fRestricted = false;				
  nRestrictedAge = 99; 
  fVoid = false;  
  bLinkedItem = false;
  fDetails = false;									// full details are ready to show
  lEntryID = INVALID_ENTRYID;
  csItemID.Empty();
  csExtendedDeptInfo.Empty();		// SR 141 - RFC 283384 

  nCrateableScreenAnswer	= CRATEABLE_SCREEN_NOT_ENTERED;	// Tri-state:If customer answers No=0; Yes=1; Never entered screen = 2;
  lRequireSecBagging = TB_LET_SECURITY_DECIDE;
  lRequireSecSubstChk = TB_LET_SECURITY_DECIDE;
  bSecurityTag = false;
  bZeroWeightItem = false;
  bPickListItem = false;

}

ItemDetails& ItemDetails::operator=(ItemDetails &d)
{
  csDescription				= d.csDescription;      // last item desc
  csItemCode				= d.csItemCode;
  
  lExtPrice					= d.lExtPrice;           // last extended price from item sale print line
  lUnitPrice				= d.lUnitPrice;          // last unit price for qty or weight
  lQtySold					= d.lQtySold;            // last qty sold
  lWgtSold					= d.lWgtSold;            // last weight sold
  lDealQty					= d.lDealQty;            // last deal quantity of the items MGG
  // BeginCodeWLDBSA11
  lDepartment				= d.lDepartment;		// last department
  // EndCodeWLDBSA11
  
  fQuantityConfirmed		= d.fQuantityConfirmed;
  fQuantityLimitExceeded	= d.fQuantityLimitExceeded;

  fCoupon					= d.fCoupon;             // item is a coupon
  fQuantityRequired			= d.fQuantityRequired;   // item needed quantity
  fWeightRequired			= d.fWeightRequired;     // item neede weight
  fPriceRequired			= d.fPriceRequired;      // item needed price
  fTareRequired				= d.fTareRequired;		 // item needed tare for weight
  fNotFound					= d.fNotFound;           // item was not found
  fNotForSale				= d.fNotForSale;         // item is not for sale, recalled
  fVisualVerify				= d.fVisualVerify;
  
  fRestricted				= d.fRestricted;	
  nRestrictedAge			= d.nRestrictedAge; 	
  fDetails					= d.fDetails;			// full details are ready to show
  fVoid						= d.fVoid;
  bLinkedItem				= d.bLinkedItem;                
  lEntryID					= d.lEntryID;
  csItemID					= d.csItemID;
  csExtendedDeptInfo		= d.csExtendedDeptInfo; // SR 141 - RFC 283384

  nCrateableScreenAnswer	= d.nCrateableScreenAnswer;	// Tri-state:If customer answers No=0; Yes=1; Never entered screen = 2;
  lRequireSecBagging		= d.lRequireSecBagging;
  lRequireSecSubstChk		= d.lRequireSecSubstChk;
  bSecurityTag				= d.bSecurityTag;
  bZeroWeightItem           = d.bZeroWeightItem;

  return *this;
}

void ItemExtendedDetails::Reset(void)
{
	csCategory = _T("");         //Exception category description
	ExceptionCode = 0;	     //Exception processing code
	dwExceptionFlags = NULL; //Other exceptions
	dwTBOptionFlags = NULL;  //For TB use only
	csExceptionWav = _T("");     //WAV associated with exception item
}

ItemExtendedDetails& ItemExtendedDetails::operator=(ItemExtendedDetails &xd)
{
	csCategory = xd.csCategory;
	ExceptionCode = xd.ExceptionCode;
	dwExceptionFlags = xd.dwExceptionFlags;
	dwTBOptionFlags = xd.dwTBOptionFlags;
	csExceptionWav = xd.csExceptionWav;

	return *this;
}

void TotalDetails::Reset(void)
{
  lTaxDue = 0;
  lTotal = 0;
}

TotalDetails& TotalDetails::operator=(TotalDetails &d)
{
  lTaxDue = d.lTaxDue;						  // last tax due from totals display
  lTotal	= d.lTotal;							  // last total
  return *this;
}

void TenderDetails::Reset(void)
{
  lTender	= 0;					  // last tender from balance or change display
  lBalance	= 0;					  // last balance
  lChange	= 0;					  // last change
  csTender	= _T("");					  // last tender desc
}

TenderDetails& TenderDetails::operator=(TenderDetails &d)
{
  lTender		= d.lTender;					  // last tender from balance or change display
  lBalance	= d.lBalance;					  // last balance
  lChange		= d.lChange;					  // last change
  csTender	= d.csTender;					  // last tender desc
  return *this;
}


/* ------------------------------------------------------------

	GetcsItemCode()

	Return Item Code without Bar Code Type

------------------------------------------------------------ */

CString ItemObject::GetcsItemCode(CString csOrigCode)
{
	// BeginCodeLocaleUPC
	if (_tcsclen(csOrigCode)!=0) return csItemCode;
	return _T("");
  // -old-code-
	// int iHeader=0;
	// if (csOrigCode.GetLength() >= 2)
	// {
	// 	if (csOrigCode[0] == 0x41) iHeader = 1; 
	// 	else if (csOrigCode[0] == 0x45) iHeader = 1;
	// 	else if ((csOrigCode[0] == 0x46) && (csOrigCode[1] != 0x46)) iHeader = 1;
	// 	else if ((csOrigCode[0] == 0x46) && (csOrigCode[1] == 0x46)) iHeader = 2;
	// 	else if ((csOrigCode[0] == 0x42) && (csOrigCode[1] == 0x33)) iHeader = 2;
	// 	return csOrigCode.Mid(iHeader);		
	// }					
	// return csOrigCode;
	// EndCodeLocaleUPC	
}

// BeginCodeLocaleUPC
bool ItemObject::IsCoupon(void)
{
	CString CSCustomCoupon;
	bool bVariableLength = false;
    for (int index=0; index < co.csaCouponlist.GetSize(); index++)
    {	// check each defined coupon format for match
	    CSCustomCoupon = co.csaCouponlist.GetAt(index);

		//GS1 databar(RSS) coupon
		if ( CSCustomCoupon.GetAt(CSCustomCoupon.GetLength() - 1) == '+' )
			bVariableLength = true;

        if ((io.csItemCode.GetLength() == CSCustomCoupon.GetLength())||
		(bVariableLength && (io.csItemCode.GetLength() >= CSCustomCoupon.GetLength())) )

		{	// first check if lengths match
			int pos = 0;
			while(pos < CSCustomCoupon.GetLength())
			{	// compare each character
                _TCHAR strChar = CSCustomCoupon.GetAt(pos);
				// ignore x and c
			    if((strChar != 'x') && (strChar != 'c') && (strChar != '+'))
				{
					if(strChar != io.csItemCode.GetAt(pos))
					{	// doesn't match so go to next coupon format
						break;
					}
				}
				pos++;
			} 
			if(pos == CSCustomCoupon.GetLength())
				return TRUE;
		}
	}
	return FALSE;
}  
// EndCodeLocaleUPC
// Supplemental code handling start
/*****************************************************************************
* Function:       HasSupplementalCode (csUPC)
*  (PARAMETER takes the UPC number)	
* Purpose:        to find the supplemental code, and zero it out if configured
*                 to do so
*
* Comments:
*                 Checks to see which scan type the UPC is for, Zeroes out the
*                 appropriate digits if the option fOperationsSubstituteZeroForSupplementalCode,
*                 is set to TRUE, and the resulting UPC will be sent to Security 
*                 
*****************************************************************************/
 
bool ItemObject::HasSupplementalCode(CString csUPC)
{

	CString UPCZeroedSupplement = csUPC;

	if (csUPC.IsEmpty())
		return false;

	bool bScanTypeHasSupplement = (((cScanType == SCAN_SDT_UPCA_S) || (cScanType == SCAN_SDT_UPCE_S) ||
		                     (cScanType == SCAN_SDT_EAN13_S) || (cScanType == SCAN_SDT_EAN8_S)) 
							 && (co.fOperationsTruncateSupplementalCodeForSecurity));

	if (!bScanTypeHasSupplement)
		return false;


	int iLengthofUPC = csUPC.GetLength();

	//UPC-E + 2 --> total length 9 or 10, Supplemental code length 2
	//UPC-E + 5 --> total length 12 or 13, Supplemental code length 5
	//EAN-13 + 2 --> total length 15, Supplemental code length 2
	//EAN-13 + 5 --> total length 18, Supplemental code length 5
	//UPC-A + 2 --> total length 14 , Supplemental code length 2
	//UPC-A + 5 --> total length 17 , Supplemental code length 5

	const int ArrayforSupLength [NUMOFUPCLENGTHS] [NUMOFSUPPLEMENTLENGTHS]= 
	{{9,2},{10,2},{12,5},{13,5},{14,2},{17,5},{15,2},{18,5}};

	int iSupplementSize = 0;

	for (int i=0; i < NUMOFUPCLENGTHS; i++)
	{
		if (ArrayforSupLength [i] [0] == csUPC.GetLength())
		{ 
			iSupplementSize = ArrayforSupLength [i] [1];
			break;
		}
	}
	if (iSupplementSize)
	{	
		io.csItemCodeSupplementalCodeTruncated = csUPC.Left((csUPC.GetLength()) - iSupplementSize);
		return true;
	}

/* commented out to just truncate, not replace supplemental code with zeroes
	UPCZeroedSupplement = csUPC.Left((csUPC.GetLength()) - iSupplementSize);
	if (iSupplementSize)
	{
		for (int j=0; j < iSupplementSize; j++)
			UPCZeroedSupplement += "0";

		io.csItemCodeSupplementalCodeZeroed = UPCZeroedSupplement;
		return true;
	}
*/					
return false;
}				
				



//Supplemental Code handling end
// BeginCodeWLDBSA11
/*****************************************************************************
* Function:       IsEmbeddedPrice (csUPC)
*  (PARAMETER takes the UPC number from Attract, zero from other locations)	TAR#123822  LPM101499
* Purpose:        Validate item code, expanding & extracting price data
*
* Comments:
*                 Validates item_code returning the code right-justified
*                 in a field of 12 digits, stripped of check digit and with
*                 price digits zeroed.
*                 Also outputs price data: null if no imbedded price or '-'
*                 if invalid, in which case return code is set FALSE.
*                 Returns TRUE if price is valid, FALSE otherwise.
*                 Input fields:   item_code
*                 Output fields:  valid_code
*                 Updated fields: valid_code
*****************************************************************************/
 
/*****************************************************************************/
/* Weighted product tables used in price verification routines.              */
/*****************************************************************************/
static int  two_minus_wp [] = {0, 2, 4, 6, 8, 9, 1, 3, 5, 7},
            five_minus_wp[] = {0, 5, 9, 4, 8, 3, 7, 2, 6, 1},
            three_plus_wp[] = {0, 3, 6, 9, 2, 5, 8, 1, 4, 7},
            five_plus_wp [] = {0, 5, 1, 6, 2, 7, 3, 8, 4, 9};

bool Validate_Code( LPTSTR, int );
void Expand_UPC( LPTSTR, int );
void Add_Zeroes( LPTSTR, int );
void UPC_Get_Price( LPTSTR, LPTSTR );
void EAN_Get_Price( LPTSTR, LPTSTR );
 
bool ItemObject::IsEmbeddedPrice(CString csUPC)
{	
  int i = 0;
  int length;
  bool valid_code = FALSE;           /* initialise bad return code */
  _TCHAR price[20];		
  int UPC_or_EAN_8;
  _TCHAR SecurityUPCCode[50] = _T("");	//used to be 13, changed to 50 for Supplemental code and Code39
  // 3/4/1999
  csItemCodeWOPrice = _T("");
    
    // TAR 252733 - set flag to reset flag to false for security maintenance
    io.b_gIsThisItemPriceEmbedded = false;

	// Added line to get data from ItemObject :
	// ---------------------------------------------------------------------
	//RFC 212888 BZ: allocate an array for the actual barcode using the length 
    //defined in the registry 	
	LPTSTR item_code = new _TCHAR[co.nMaxBarcodesLength+1]; 

	//added to keep the io.csItemCode from being changed in here 
	CString cstempItemCode = csItemCode;

	//Begin TAR123822 	LPM101399	

	//if the cstempItemCode isn't set yet, then we must have come from the Attract screen
	//  have to use the one passed in
	if (cstempItemCode = _T(""))	cstempItemCode = csUPC;

	// check if any alpha characters at beginning of UPC (put there by PLU server)
	int nItemCodeLen = cstempItemCode.GetLength();
	
	//TAR234641+ CHU 052903 for code128
    if ( nItemCodeLen && (!_istdigit(cstempItemCode[0])) && (cScanType == SCAN_SDT_Code128) && (cstempItemCode.Left(2) == _T("B3")))   //modified for Code128
    {
       	cstempItemCode = cstempItemCode.Right(nItemCodeLen - 2); //Take the "B3" out
	}//TAR234641-
	else if ( nItemCodeLen && (!_istdigit(cstempItemCode[0])) && (cScanType != SCAN_SDT_Code39))   //modified for Code39
	{
		// item code is with scan type prefix
		// remove alpha characters from UPC, assumes prefix is never more than one character
		cstempItemCode = cstempItemCode.Right(nItemCodeLen - 1);
	}
	//end TAR123822

	//added for code39 start
	else if (nItemCodeLen && (cScanType == SCAN_SDT_Code39) && (cstempItemCode.Left(2) == _T("B1")))
		cstempItemCode = cstempItemCode.Right(nItemCodeLen - 2);

	//added for supplemental code
	io.b_gHasSupplementalCodeBeenTruncated = io.HasSupplementalCode(cstempItemCode); //was csUPC
	if (io.b_gHasSupplementalCodeBeenTruncated)
	{
		delete [] item_code;
		return false;
	}
	//if ( (strlen((LPCTSTR) cstempItemCode) +1 ) > ITEM_CODE_LENGTH ) return FALSE;
	if ( (cstempItemCode.GetLength() + 1 ) > co.nMaxBarcodesLength ) 
	{
		delete [] item_code;	// potential memory leak if don't delete this LPM022503
		return false;	//RFC 212888 BZ
	}
	// BeginCodeLocaleUPC
	_tcscpy (item_code, cstempItemCode); // item code without scan type
	_tcscpy (SecurityUPCCode, cstempItemCode);	//TAR123822	LPM101399

	//New code to handle In-Store barcodes start 
    
	if (cstoreCode.FindMatchingPatternCList(cstempItemCode))  
	{
		_tcscpy (SecurityUPCCode, cstoreCode.ZeroedPriceCList(cstempItemCode));
		io.b_gHasCustomCodeBeenFound = true;
		io.csItemCodeVariablePartZeroed = SecurityUPCCode;
		delete [] item_code;
        return cstoreCode.IsPatternPriceEmbedded(cstempItemCode);//RFC239992  If InStore barcode defined as price embedded
	}
	else 
		io.b_gHasCustomCodeBeenFound = false;

    if(co.fOperationsDisableBasePriceEmbeddedDefinitions)//RFC239992
    {
		delete [] item_code;
		return false;
    }


	//New code to handle In-Store barcodes end
    // -old-code-

	// strcpy (item_code, (LPCTSTR)GetcsItemCode(csOrigCode)); 	// item code without scan type
	// EndCodeLocaleUPC	

	// the only issue is with 8 digits that can UPC_E or EAN_8
	// we get this info from cScanType
	UPC_or_EAN_8 = (_TCHAR)cScanType;					// A-UPC, E-UPC_E, F-EAN	
   							

	// Original Code
	// ---------------------------------------------------------------------
	_tcscpy( price, _T("") );

	/***************************************************************************/
	/*                                                                         */
	/* Validate and expand item code depending on length:                      */
	/*                                                                         */
	/*  length < 7  - velocity codes                                           */
	/*  length = 7  - UPC-E without check digit (if leading 0)                 */
	/*              - velocity code (if no leading 0)                          */
	/*  length = 8  - EAN-8 or UPC-E with check digit (if leading 0)           */
	/*              - EAN-8 (if no leading 0)                                  */
	/*  length = 9  - invalid                                                  */
	/*  length = 10 - UPC-A with no check digit (implied leading 0)            */
	/*  length = 11 - UPC-A with no check digit                                */
	/*  length = 12 - UPC-A with check digit                                   */
	/*  length = 13 - EAN-13                                                   */
	/*  length > 13 - currently invalid                                        */
	/*                                                                         */
	/***************************************************************************/

	length = _tcsclen( item_code );

	/* let's make sure our item_code is all integers and that at least */
	/* one digit is non-zero                                           */
	for( i = 0; i < length; i++ )
	{
		/* if it isn't an integer, let's get outta here */
		if( !_istdigit( item_code[i] ) )
		{
			delete [] item_code;
			return( FALSE );
		}  
		/* if we found a non-zero integer, we may have a valid code */
		if( item_code[i] != _T('0') )
			 valid_code = TRUE;
	}

	/* if we don't have a valid_code or an improper length, let's bail.... */
	if( !valid_code || length == 9 || length < 1 || length > 13 )
	{
		delete [] item_code;
		return( FALSE );
	}

	/* Check lengths to determine what to do with code */
	if( length < 7 || length == 10 || length == 11 )
	{
		/* Right-justify simple codes in a field of 12 zeroes */
		Add_Zeroes( item_code, length );
	}
	else if( length == 7 )
	{
		/* UPC-E without check digit must begin with 0, else it's velocity code */
		if( item_code[0] == _T('0') )
		{
			 /* Expand UPC-E to 12 digits */
			 Expand_UPC( item_code, length );
		}
		else
		{
			 /* Expand velocity code to 12 digits */
			 Add_Zeroes(item_code, length);
		}
	}
	else if( length == 8 )
	{
		/* UPC-E with check digit or EAN-8 */
		// BeginCodeLocaleUPC
		if( item_code[0] == _T('0') && (	UPC_or_EAN_8 == SCAN_SDT_UPCE ) )
    // -old-code-
		// if( item_code[0] == '0' && UPC_or_EAN_8 == UPC_E )
		// EndCodeLocaleUPC			

		{
			 /* Expand UPC-E to 12 digits */
			 Expand_UPC( item_code, length );
		}
		else
		{
			 /* Expand EAN-8 to 13 digits */
			 Add_Zeroes( item_code, length );
		}

		/* Pass Validate_Code the expanded length of the item code */
		/* Typecasting with IN - WIN32 Graphical Client - Murali 970715   */
		valid_code = Validate_Code( item_code, (int)_tcsclen( item_code ) );
	}
	else
	{
		/* Validate UPC-A (12 digits) or EAN-13 - both have check digit */
		valid_code = Validate_Code( item_code, length );
	}

	/***************************************************************************/
	/* Now extract the price from a type-UPC A2:                               */
	/*  Must be between 11 and 13 digits in length							   */
	/*  First 2 digits must be 02											   */
	/*  First 2 digits must be 20 for EAN13                                    */
	/***************************************************************************/
	if( valid_code && length > 10 )
	{
		/*************************************************************************/
		/* UPC A2 codes that begin with 2 (02 when expanded above) are			 */
		/*  originally 11 or 12 digits long, but 13 when expanded above.		 */
		/* EAN13 codes that begin with 02 are 13 digits long					 */
		/*  and can be calculated the same as UPC A2/CD							 */
		/* BOTH are price embedded												 */
		/*************************************************************************/
		if( item_code[0] == _T('0') && item_code[1] == _T('2') && length <= 13 )
		{
		  if (co.fOperationsUPCAPriceEmbeddedVerifier)	// price verifier is the 7th digit, 5-digit price field UPC type2 with EAN standard
		  {// extract the price and replace with zeros
			UPC_Get_Price( item_code, price );
		  }
		  else	//price verifier is the 8th digit		4-digit price field
		  {// extract the price and replace with zeros
			EAN_Get_Price( item_code, price );
		  }
		  // if price is - then the price embedded validation failed
		  if( price[0] == _T('-') )
		  {
			valid_code = FALSE;
		  }
		  else
		  {	//TAR123822 LPM101399	Security needs to be sent UPC differently
		 	(void) _tcscpy(SecurityUPCCode, item_code);
		  }
		}
		/*************************************************************************/
		/* EAN13 codes begining with 20 are originally 13 chars long			 */
		/* 	AND are price embedded but are NOT calculated like UPC A2/CD		 */
		/*************************************************************************/
		else if( item_code[0] == _T('2') && item_code[1] == _T('0') && length == 13) 
		{
			// extract the price and replace with zeros
			EAN_Get_Price( item_code, price );	// 4 digit price field
			// if price is - then the price embedded validation failed
			if( price[0] == _T('-') )
			{
				valid_code = FALSE;
			}
			else
			{	//TAR123822 LPM101399	Security needs to be sent UPC differently
				(void) _tcscpy(SecurityUPCCode, item_code);
			}
		}
	}
	delete [] item_code;
	
	//length of price is null means NOT Price Embedded	
	if ((_tcsclen(price)==0) || (!valid_code)) 
	{
		return FALSE;
	}
	else 
	{
		// Added 3/4/1999  price embedded UPC
		csItemCodeWOPrice = SecurityUPCCode;	//was item_code.now using the security one
		io.b_gIsThisItemPriceEmbedded = true;
		return TRUE;
	}
}


/*****************************************************************************
* Function:       Validate_Code
* Purpose:        Validate item code
*
* Comments:
*                 Validates item_code returning the code right-justified
*                 in a field of 12 digits, stripped of check digit.
*                 Uses modulo-10 method described in ANA Operations Guide.
*                 Returns TRUE if code is valid, FALSE otherwise.
*                 Input fields:   item_code,length
*                 Output fields:  valid_code
*                 Updated fields: item_code
*****************************************************************************/
bool Validate_Code( LPTSTR item_code, int length )
{
	int i;
	int sum = 0;
	_TCHAR temp_code[13];

	/***************************************************************************/
	/* Starting from the last digit before the check digit,                    */
	/* add every alternate digit back to the first digit,                      */
	/* then multiply this by three.                                            */
	/***************************************************************************/
	for( i = length - 2; i >= 0; i -= 2 )
		sum += (int) item_code[i] - _T('0');

	sum *= 3;

	/* Then add every other digit to this total */
	for( i = length - 3; i >= 0; i -= 2 )
		sum += (int) item_code[i] - _T('0');

	/* And finally, add the check digit */
	sum += (int) item_code[length - 1] - _T('0');

	/* Remove the check digit from end of code */
	//item_code[length - 1] = '\0';

	/* Expand UPC-A code (now 11 digits) to 12 digits */
	if( length == 12 )
	{
		_tcscpy( temp_code, item_code );
		item_code[0] = _T('0');
		_tcscpy( item_code + 1, temp_code );
	}

	/* The sum should be a multiple of 10 if the number is valid */
	if( sum % 10 )
		return( FALSE );
	else
		return( TRUE );
}


/**************************************
* Function:       Expand_UPC
* Purpose:        Expand a UPC-E short code to 12 digits
*
* Comments:
*                 Expands item_code returning the code right-justified in
*                 int a field of 12 digits.
*                 Input item_code is expected to be a valid 7 or 8 digit
*                 UPC-E code.
*                 Input fields:   length
*                 Output fields:  (none)
*                 Updated fields: item_code
*/
void Expand_UPC(LPTSTR item_code, int length)
{
  _TCHAR out_code[13];

  (void) _tcscpy(out_code, _T("000000000000")); /* 12 zeroes                        */

  /***************************************************************************/
  /* _TCHAR 1 is always 0                                                      */
  /* chars 2 & 3 are chars 2 & 3 of UPC-E code                               */
  /***************************************************************************/
  (void) _tcsnccpy(out_code+1, item_code+1, 2);

  /***************************************************************************/
  /* Middle chars of full code depend on 7th _TCHAR of short code              */
  /* See ANA Operating Manual section 6.7.2 for details                      */
  /***************************************************************************/
  switch (item_code[6]) {

    case _T('0') : (void) _tcsnccpy(out_code+8, item_code+3, 3);
               break;

    case _T('1') : (void) _tcsnccpy(out_code+8, item_code+3, 3);
               out_code[3] = _T('1');
               break;

    case _T('2') : (void) _tcsnccpy(out_code+8, item_code+3, 3);
               out_code[3] = _T('2');
               break;

    case _T('3') : (void) _tcsnccpy(out_code+9, item_code+4, 2);
               out_code[3] = item_code[3];
               break;

    case _T('4') : (void) _tcsnccpy(out_code+3, item_code+3, 2);
               out_code[10] = item_code[5];
               break;

    case _T('5') : (void) _tcsnccpy(out_code+3, item_code+3, 3);
               out_code[10] = _T('5');
               break;

    case _T('6') : (void) _tcsnccpy(out_code+3, item_code+3, 3);
               out_code[10] = _T('6');
               break;

    case _T('7') : (void) _tcsnccpy(out_code+3, item_code+3, 3);
               out_code[10] = _T('7');
               break;

    case _T('8') : (void) _tcsnccpy(out_code+3, item_code+3, 3);
               out_code[10] = _T('8');
               break;

    case _T('9') : (void) _tcsnccpy(out_code+3, item_code+3, 3);
               out_code[10] = _T('9');
               break;

  } /* switch */

  /***************************************************************************/
  /* Need to copy check digit for 8-digit codes, or add a preceding        */
  /* zero for 7-digit codes (to make 12 digits).                           */
  /***************************************************************************/
  if (length == 8) {
    out_code[11] = item_code[7];
    (void) _tcscpy(item_code, out_code);

  } else {   /* length == 7 */
    (void) _tcsnccpy(item_code + 1, out_code, 11);
    item_code[0]  =  _T('0');
    item_code[12] = _T('\0');
  }
  return;
}

/**************************************
 * Function:       Add_Zeroes
 * Purpose:        Add zeroes to the front of item_code
 *
 * Comments:
 *                 Add zeroes to the front of item_code producing a 12 digit
 *                 right-justified code, unless the input string is 8 digits
 *                 long int which case it must be EAN-8 so is expanded to 13
 *                 digits (the 13th digit being the check digit which is
 *                 later removed).
 *                 Input fields:   length
 *                 Output fields:  (none)
 *                 Updated fields: item_code
 */
void Add_Zeroes( LPTSTR item_code, int length )
{
   _TCHAR out_code[14];
   
   _tcscpy( out_code, _T("000000000000") );
   
   /* Expand EAN-8 codes to 13 digits and others to 12 digits */
   if( length == 8 )
      _tcscpy( out_code+5, item_code );
   else
      _tcscpy( out_code+12-length, item_code );
   
   _tcscpy( item_code, out_code );
}


/****************************************************************************
 * Function:       UPC_Get_Price
 * Purpose:        Extract price details from UPCA type-2 codes
 *
 * Comments:
 *                 Extract 5-digit price from item_code (last 5 digits).
 *                 The price is then verified by adding the weighted products
 *                 of the 5 price digits and the preceding verifier digit.
 *                 Weighted products are as follows:
 *                   Verifier  digit : 5- weighted
 *                   1st price digit : 5+ weighted
 *                   2nd price digit : 2- weighted
 *                   3rd price digit : 5- weighted
 *                   4th price digit : 5+ weighted
 *                   5th price digit : 2- weighted
 *                 See ANA Operating Manual for more detail. The resulting
 *                 value should be a multiple of 10; price is set = '-' if
 *                 the price is invalid. Price digits in item_code are zeroed
 *                 after use.
 *                 Input fields:   item_code
 *                 Output fields:  price
 *                 Updated fields: item_code
 */
void UPC_Get_Price(LPTSTR item_code, LPTSTR price)
{
	// this procedure is used to calculate 5 digit price field for variable weight formats whether 
	// UPCA (type-2) or EAN 02  (UPCAPriceEmbeddedVerifier = Y, which is the European standard)
	int i;
	int sum = 0;                           /* holds the checksum running total */
	_TCHAR price_check;
	_TCHAR computed_price_check;

	/***************************************************************************/
	/* ncr_cmh 7/8/94                                                          */
	/* Changed this routine so that it correctly caluculates and verifies the  */
	/* price check digit according to UPC Guideline #11 from the UPC Guidelines*/
	/* Manual. The previous algorithm was incorrectly calculating the price    */
	/* check and not doing the correct validation.                             */
	/***************************************************************************/

	/***************************************************************************/
	/* First extract the price check digit from the end                        */
	/***************************************************************************/
	(void) _tcscpy(price, item_code+7);
	price_check = item_code[6];  //TAR230319 it should be 6 not 7 since it starts from 0



	/***************************************************************************/
	/* If the price verifier digit is non-zero then verify the price           */
	/*                                                        CR  20 Jul 90    */
	/***************************************************************************/
	if (price_check != _T('0')) 
	{
		/****************************************************************************/
		/* RFC157948 LPM121800														*/
		/* The EAN convention defined in the EAN primer document states that		*/
		/* EAN13 starting with 02 AND UPC A2 may have the same format as follows	*/
		/* 02 iiii P ppppp C where i=UPC, P=price verifier, p=price, C=ck digit		*/
		/*																			*/
		/* Algorithm for a 5 digit price check-digit								*/
		/*  1. Compute the weighted product for each digit position in the 5-digit	*/
		/*     price field															*/
		/*  2. Sum the weighted products found in STEP1.							*/
		/*  3. Take the ten's complement of the units position of the sum found in	*/
		/*     STEP 2.																*/
		/*  4. Find the number whose 5-weighted product is the same as the result	*/
		/*     in STEP 3															*/
		/*  The result of STEP 4 is the price check-digit							*/   
		/****************************************************************************/
		sum = five_plus_wp [price[0] - _T('0')] +
		  two_minus_wp [price[1] - _T('0')] +
		  five_minus_wp[price[2] - _T('0')] +
		  five_plus_wp [price[3] - _T('0')] +
		  two_minus_wp [price[4] - _T('0')] ;

		computed_price_check =  (10 - (sum % 10)) % 10;

		for (i=0;five_minus_wp[i] != computed_price_check;i++);
		
		computed_price_check = i + _T('0');

		#ifdef _DEBUG
			_tprintf(_T("Item code = %s, Price = %s, Price Check = %c, Computed PC = %c\n"),
					item_code, price, price_check, computed_price_check);
		#endif

		/************************************************************************/
		/* Set price = '-' if price is invalid									*/
		/* RFC157948 LPM121800													*/
		/* The EAN convention defined in the EAN primer document states that	*/
		/* EAN13 starting with 02 AND UPC A2 may have the same format as follows*/
		/* 02 iiii P ppppp C where i=UPC, P=price verifier, p=price, C=ck digit	*/
		/************************************************************************/
		if (computed_price_check != price_check  ) 
			(void) _tcscpy(price, _T("-"));
	}
	/***************************************************************************/
	/* Finally, zero the price digits in item_code                             */
	/***************************************************************************/
	//if (price != "-")   //TAR230319 CHU
	if ( price[0] != _T('-') )
		(void) _tcscpy(item_code+6, _T("0000000"));	//TAR230319: Item code should have 02iiii0000000 
	
	return;
}

/*******************************************************************************
 * Function:       EAN_Get_Price
 * Purpose:        Extract price details from an EAN type-2 code
 *
 * Comments:
 *                 Extracts 4-digit price from item_code (last 4 digits).
 *                 The price is then verified by adding the weighted products
 *                 of the 4 price digits and the preceding verifier digit.
 *                 Weighted products are as follows:
 *                  1st price digit : 2- weighted
 *                  2nd price digit : 2- weighted
 *                  3rd price digit : 3+ weighted
 *                  4th price digit : 5- weighted
 *                 See ANA Operating Manual or UPC Guidelines Manual for more
 *                 detail. The resulting value should be a multiple of 10;
 *                 price is set = '-' if the price is invalid. Price digits
 *                 in item_code are zeroed after use.
 *                 Input fields:   item_code
 *                 Output fields:  price
 *                 Updated fields: item_code
 */
void EAN_Get_Price(LPTSTR item_code, LPTSTR price)
{
	// this procedure is used to calculate 4 digit price field for variable weight formats 
	// EAN 20 and EAN 02 when UPCAPriceEmbeddedVerifier = N
	int nlength = _tcsclen( item_code );
	int sum = 0;                           /* holds the checksum running total  */
	_TCHAR price_check;
	_TCHAR computed_price_check;
	_TCHAR temp_code[13];
	_tcscpy( temp_code, item_code );
	temp_code[nlength - 1] = _T('\0');


	/**************************************************************************/
	/* ncr_cmh 7/8/94                                                         */
	/* Changed this routine so that it correctly calculates and verifies the  */
	/* price check digit according to the General EAN Specifications Manual.  */
	/* The previous algorithm was incorrectly calculating the price           */
	/* check and not doing the correct validation.                            */
	/**************************************************************************/

	/**************************************************************************/
	/* First extract the price verifier digit from the 8th position			  */
	/**************************************************************************/
	(void) _tcscpy(price, temp_code+8);
	price_check = item_code[7];

    /************************************************************************/
	/* RFC157948 LPM121800													*/
	/* The EAN convention defined in the EAN primer document states that	*/
	/* EAN13 price embedded starting with 2x shall have the following format*/
	/* 20 iiiii P pppp C where i=UPC, P=price verifier, p=price, C=ck digit	*/
	/* If EAN13 02 price verifier is the 8th digit, this algorithm applies  */
	/*																		*/
    /* Algorithm for determining a 4 digit price verifier					*/
    /*   1. Compute the weighted product for each digit position in the 4   */
    /*      digit price field.                                              */
    /*   2. Sum the weighted products found in STEP 1                       */
    /*   3. Multiply the sum found in STEP 2 by 3                           */
    /*   4. The price-check digit is the units position of the product found*/
    /*      in STEP 3.                                                      */
    /************************************************************************/
    sum = two_minus_wp [price[0] - _T('0')] +
          two_minus_wp [price[1] - _T('0')] +
          three_plus_wp[price[2] - _T('0')] +
          five_minus_wp[price[3] - _T('0')] ;

    sum *= 3;

    computed_price_check = (sum % 10) + _T('0');

	#ifdef _DEBUG
		_tprintf(_T("Item code = %s, Price = %s, Price Check = %c, Computed PC = %c\n"),
				item_code, price, price_check, computed_price_check);
	#endif

	/************************************************************************/
	/* Set price = '-' if price is invalid									*/
	/* RFC157948 LPM121800													*/
	/* The EAN convention defined in the EAN primer document states that	*/
	/* EAN13 starting with 20 shall have the following format				*/
	/* 20 iiiii P pppp C where i=UPC, P=price verifier, p=price, C=ck digit	*/
    /************************************************************************/
	if (computed_price_check != price_check  ) 
		(void) _tcscpy(price, _T("-"));
	/***************************************************************************/
	/* Finally, if it is price embedded, zero the price digits in item_code                             */
	/* but leave the 13th digit alone, as it is not part of the price			 */
	/***************************************************************************/
	else
		(void) _tcscpy(item_code+7, _T("000000"));  //TAR230319 CHu: The item code should only have 20iiiii000000

	return;
}

// End of IsEmbeddedPrice() functions
////////////////////////////////////////////////////////////////////////////////////////////
// EndCodeWLDBSA11

// TAR 127247
CustomMessage& CustomMessage::operator=(CustomMessage &d)
{
  csLeadThruText     = d.csLeadThruText; 
  csScreenText       = d.csScreenText; 
  csPlayWave         = d.csPlayWave;     
  csE_ReceiptText    = d.csE_ReceiptText; 
  bHiliteReceiptText = d.bHiliteReceiptText; 
  bNeedItemCancel    = d.bNeedItemCancel ;    
  bApprovalNeeded    = d.bApprovalNeeded ;
  bGoBackScanAndBag  = d.bGoBackScanAndBag;
  csRAP_MessageText  = d.csRAP_MessageText;
  bHiliteRAPText     = d.bHiliteRAPText;
  return *this;
}

void CustomMessage::Reset()
{
  csLeadThruText = _T(""); 
  csScreenText = _T(""); 
  csPlayWave = _T("");     
  csE_ReceiptText = _T(""); 
  bHiliteReceiptText = false; 
  bNeedItemCancel = false;    
  bApprovalNeeded = false; 
  bGoBackScanAndBag = false;
  csRAP_MessageText = _T("");
  bHiliteRAPText = false;
}
