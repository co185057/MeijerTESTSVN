//////////////////////////////////////////////////////////////////////////////
//
// FILE:  ItemObject.H
//
// TITLE:   Class interface definition for a transaction item
//
// 
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////
//
// CHANGES: Start
// CHANGES: End
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _ItemObject
#define _ItemObject
#include "DllDefine.h"
#include "IItemObject.h"

// BeginCodeLocaleUPC
//Begin RFC 412256
#include "OposScan.h"
/* 
  SA250050 - Remove non-OPOS SCAN_SDT_RSS value '502' and replace with the actual OPOS compliant scan type to prevent 
  'out-of-range' errors which are caused by passing an invalid barcode type (i.e. 502) to the TB and sales applications. 
  UnifiedPOS v1.8 adds SCAN_SDT_RSS14 and SCAN_SDT_RSS_EXPANDED which are later changed in UnifiedPOS v1.12 respectively 
  to SCAN_SDT_GS1DATABAR and SCAN_SDT_GS1DATABAR_E. We will use the latter and their values.

#define SCAN_SDT_RSS 502  //This is a temporily fix for RSS barcode since OPOS doesn't support RSS
						   //scan type yet. This line need be removed after OPOS supports RSS scan type in 
						   //"OposScan.h"  //tar 247406
*/   

// tn. Comment out the following two lines since they are defined in the Oposscan.h already (10/18/2011)

//#define SCAN_SDT_GS1DATABAR    131   // GS1 DataBar Omnidirectional (normal or stacked), prev SCAN_SDT_RSS14 
//#define SCAN_SDT_GS1DATABAR_E  132   // GS1 DataBar Expanded (normal or stacked), prev SCAN_SDT_RSS_EXPANDED

//End RFC 412256

// EndCodeLocaleUPC

//////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 ItemDetails : public CObject
{
public:
  CString csDescription;            // last item desc
  CString csItemCode;               // last item number
  CString csErrDescription;         // used to display extra tender errors on Card Misread screen
 	
  long lExtPrice;                   // last extended price from item sale print line
  long lUnitPrice;                  // last unit price for qty or weight
  long lQtySold;                    // last qty sold
  long lWgtSold;                    // last weight sold
  long lDealQty;                    // last deal quantity of the items MGG
  long lDepartment;                 // last departement

  bool fQuantityConfirmed;			// means the user confirmed a high quantity threshhold
  bool fQuantityLimitExceeded;			// means the user confirmed a per item quantity limit

  bool fCoupon;                     // item is a coupon
  bool fQuantityRequired;           // item needed quantity
  bool fWeightRequired;             // item neede weight
  bool fPriceRequired;              // item needed price
  bool fTareRequired;               // item needed tare
  bool fNotFound;                   // item was not found
  bool fNotForSale;                 // item is not for sale, recalled
  bool fVisualVerify;             
	
  bool fRestricted;	                // item is restricted
  int  nRestrictedAge;              // The age limitation for restricted items
  bool fDetails;										// full details are ready to show
  bool fVoid;                       // is item voided?

  bool bLinkedItem;                 //Tar 125389// is linked item
  long lEntryID;

  int nCrateableScreenAnswer;		// Tri-state:If customer answers No=0; Yes=1; Never entered screen = 2;
  long lRequireSecBagging;			// override security bagging enforcement
  long lRequireSecSubstChk;			// override security substitution check
  bool bSecurityTag;				// item has a security tag
  bool bZeroWeightItem;             // item is an zero weight item
  bool bPickListItem;				// Is item from Pick List
  CString csItemID;					// unique item id, to replace lEntryID, do not use lEntryID inside SCOTApp
  CString csExtendedDeptInfo;		// string containing the extended department information SR 141 - RFC 283384 

public:
  void Reset(void);
  ItemDetails& operator=(ItemDetails&);
};

//////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 ItemExtendedDetails : public CObject
{
public:
  CString csCategory;     //Exception category description
	long ExceptionCode;	//Exception processing code
	DWORD dwExceptionFlags; //Other exceptions
	DWORD dwTBOptionFlags;  //For TB use only
  CString csExceptionWav;  //WAV associated with exception item
public:
  void Reset(void);
  ItemExtendedDetails& operator=(ItemExtendedDetails&);
};
//////////////////////////////////////////////////////////////////////////////

#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  TotalDetails : public CObject
{
public:
  long lTaxDue;										  // last tax due from totals display
  long lTotal;										  // last total
public:
  void Reset(void);
  TotalDetails& operator=(TotalDetails&);
};

//////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  TenderDetails : public CObject
{
public:
  long lTender;										  // last tender from balance or change display
  long lBalance;									  // last balance
  long lChange;										  // last change
  CString csTender;								  // last tender desc
public:
  void Reset(void);
  TenderDetails& operator=(TenderDetails&);
};

//////////////////////////////////////////////////////////////////////////////
#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
  ItemObject : public CObject, public IItemObject   // a transaction item
{
public:
  // data entered by user
  CString csOrigCode;               // item code scanned, picked or keyed
  CString csItemCode;               // item code without scan type
	
  CString csItemCodeWOPrice;  		  // SA need a special UPC withoout Price 
                                    // in case of Embedded Price
  CString csItemCodeSupplementalCodeTruncated;   //SA need a special UPC without Supplemental Code
  CString csItemCodeVariablePartZeroed;
  long lQuantityEntered;					  // should be the same as sold below
  long lPriceEntered;							  // in cents, used rarely
  long lUnitPriceEntered;							  // in cents, used rarely
  long lExtPriceEntered;                      //Tar 137876 add extended price
  long lWeightEntered;						  // in 1/100ths lb or 1/1000 ths kg, with tare
  long lTotalWeightEntered;						  // in 1/100ths lb or 1/1000 ths kg, with tare, Includes TARE weight(TAR 120968)
  int  nLoyaltyCardScanned;			// 1 is for first time, 2 is for second time or from there onwards
  bool bLoyaltyCard;
  bool fVoid;											  // voiding this item
  bool fNotForSale;                 // ite is not for sale
  bool bBizerbaScale;
	
  long cScanType;									  // A-UPC, E-UPC_E, F-EAN
    
  // these codes are set by SCOT depending on the current status
  // of this item vs restricted items and coupons
  long lRestCode;									  // 0-no, 1-show msg, 2-no msg, 3-void it, 4-voided
  long lCoupCode;									  // 0-not a coupon, 1-both vis and aud, 2-no msg, 3-visual msg only: TAR#102019 & #120972
  long lVisualCode;									  // 0-no, 1-show msg, 2-no msg
  long lTareCode;									  // tare code 0,1,2...
  CString csOrgItemDescription;                       // Orginal item description if item is a linked item
	
  ItemDetails d;                    // item sold details
  ItemExtendedDetails xd;           // item sold extended details
  long lUpdateFields;

public:
  void Reset(void);

  //force weighing of price embedded items 
  bool b_gIsThisItemPriceEmbedded;
  long l_gPriceEmbeddedWt;

  bool b_gHasSupplementalCodeBeenTruncated;
  bool b_gHasCustomCodeBeenFound;

  //EAS tag deactivation
  bool b_glTagDeactivated;
  bool b_glExpectingEASEvent;

	// BeginCodeWLDBSA11
  bool IsEmbeddedPrice(CString csUPC);
	// EndCodeWLDBSA11

  //Supplemental Code handling start
  bool HasSupplementalCode(CString csUPC);
  //Supplemental Code handling end

  // BeginCodeLocaleUPC
  bool IsCoupon(void);	//102019 scantype golbal not supposed to be changed LPM120899
	// EndCodeLocaleUPC
	
  CString GetcsItemCode( CString csUPC );	
};


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 

 CustomMessage : public CObject      // TAR 127247
{
 public:
  CString csLeadThruText;  // Screen leadthru text
  CString csScreenText;    // Screen message text
  CString csPlayWave;      // Play wave file
  CString csE_ReceiptText; // Text to display in E-Receipt as informational line
  bool bHiliteReceiptText; // Specify whether to display text in a different color.
  bool bNeedItemCancel;    // Flag to tell core to cancel item and disable pending security
  bool bApprovalNeeded;    // Flag for Approval Needed, treated as Unknown Item approval and
                           // valid only when 'bNeedItemCancel' is TRUE; 
  bool bGoBackScanAndBag;       // From Message Screen, go back ScanAndBag after OK button is pressed.
  CString csRAP_MessageText;
  bool bHiliteRAPText;

 public:
  void Reset(void);
  CustomMessage& operator=(CustomMessage&);
  
};

#endif
