// FiscalProperties.cpp: implementation of the CFiscalProperties class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "FiscalProperties.h"


// Global Data
//-- Capabilities - for 2213 Type 1 and Type 2-

CFPrinterCaps PrinterCaps2213_T1 =
{
    TRUE,          // m_CapAdditionalLines
    TRUE,          // m_CapAmountAdjustment
    TRUE,          // m_CapAmountNotPaid - for 2213
    TRUE ,          // m_CapCheckTotal
    {
        TRUE,      // m_Jrn.m_CapPresent
        TRUE,      // m_Jrn.m_CapEmptySensor
		TRUE      // m_Jrn.m_CapNearEndSensor - True for Type 1
    },
    {
        TRUE,      // m_Rec.m_CapPresent
        TRUE,      // m_Rec.m_CapEmptySensor
		TRUE      // m_Rec.m_CapNearEndSensor - True for Type 1
    },
	{
        FALSE,       // m_Slp.m_CapPresent
        FALSE,      // m_Slp.m_CapEmptySensor// FIX-10000-DS#A change from FALSE to TRUE
		FALSE       // m_Slp.m_CapNearEndSensor
    },
    FALSE ,           // m_CapCoverSensor
    TRUE ,            // m_CapFixedOutput
    FALSE  ,          // m_CapHasVatTable
    TRUE ,            // m_CapIndependentHeader
    FALSE ,           // m_CapItemList
    
    FALSE,          // m_CapDuplicateReceipt
    FALSE ,          // m_CapSlpFullSlip - True for Type 1
    FALSE ,          // m_CapSlpFiscalDocument
    FALSE ,          // m_CapSlpValidation

    FALSE ,          // m_CapValidateJournal

    TRUE,          // m_CapNonFiscalMode
    FALSE ,          // m_CapOrderAdjustmentFirst
    FALSE ,          // m_CapPercentAdjustment
    TRUE ,          // m_CapPositiveAdjustment
	FALSE,          // m_CapPowerLossReport
    FALSE ,          // m_CapPredefinedPaymentLines
    TRUE ,          // m_CapReceiptNotPaid

    TRUE,          // m_CapDoubleWidth - TRUE for 2213 and 2214// FIX-00001-DS#A

    TRUE,          // m_CapSubAmountAdjustment
    FALSE ,          // m_CapSubPercentAdjustment
    TRUE ,          // m_CapSubTotal
    TRUE ,          // m_CapTrainingMode

	FALSE,          // m_CapRemainingFiscalMemory
    TRUE ,          // m_CapResevedWord
    TRUE ,          // m_CapSetHeader
    FALSE ,          // m_CapSetPOSID
	FALSE,          // m_CapSetStoreFiscalID
    FALSE ,          // m_CapSetTrailer
    FALSE ,          // m_CapSetVatTable

    FALSE           // m_CapXReport
}; // Type 1


//-- Properties --

static CFPrinterNumericProps PrinterProps2213_T1 =
{
    FALSE,          // BOOL m_AsyncMode
    TRUE,          // BOOL m_CheckTotal
    FALSE,          // BOOL m_CoverOpen
    FPTR_CC_ITALY,  // 	LONG m_CountryCode
    FALSE,          // BOOL m_FlagWhenIdle
    FALSE,			// BOOL m_DayOpened - false initially
	0,				//	LONG m_AmountDecimalPlaces : 0 coz Lira has no fraction part
	
    {
        FALSE,      // BOOL m_Jrn.m_Empty
        FALSE       // BOOL m_Jrn.m_NearEnd
    },

    {
        FALSE,     // BOOL m_Rec.m_Empty
        FALSE      // BOOL m_Rec.m_NearEnd
    },
    0,            // LONG m_SlipSelection
    24,			  // LONG m_DescriptionLength  
    FALSE,        // BOOL m_DuplicateReceipt

    {
        FALSE,    // BOOL m_Slp.m_Empty
        FALSE     // BOOL m_Slp.m_NearEnd
    },
    FALSE,        // BOOL m_ErrorLevel 
    0,            // LONG m_ErrorOutID
    500,          // LONG m_ErrorState
    0,            // LONG m_ErrorStation
    _T("[Error]"),    // CString m_ErrorString 

    30,            // LONG m_MessageLength 
    5,            // LONG m_NumHeaderLines
    0,            // LONG m_NumTrailerLines
	0,            // LONG m_NumVatRates 
    _T("[Error]"),    // CString m_PredefinedPaymentLines 

    1,            // LONG m_PrinterState 
	0,            // LONG m_QuantityDecimalPlaces 
    12,           // LONG m_QuantityLength
    0,            // LONG m_RemainingFiscalMemory
    _T("TOTALE"),          // CString m_ReservedWord
	FALSE,          // BOOL m_TrainingModeActive

	//mee
	OPOS_PN_DISABLED,	//PowerNotify
	OPOS_PR_NONE,		//CapPowerReporting
	OPOS_PS_UNKNOWN,		//PowerState
	OPOS_FPTR_FO_CLASS2_INVOICE,
	OPOS_FPTR_FO_CLASS2_RECEIPT

};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFiscalProperties::CFiscalProperties()
{
    m_Caps = &PrinterCaps2213_T1;
    m_Props = &PrinterProps2213_T1;
}

CFiscalProperties::~CFiscalProperties()
{
    m_Caps = NULL;
    m_Props = NULL;
}

//
// Function:  Initailize
// Purpose: Loads default property values and printer metrics based on
//          the model of printer to be used.
// Inputs:  sModel - String identifying the printer model
// Outputs:  None
// Assumptions:  Initially, the Axiohm 7156 is the only supported printer
//               New printes can be added by modifying this function and
//               providing the default values in PrinterGlobal.h and 
//               PrinterGlobal.cpp
// Comments:
//
void CFiscalProperties::Initialize( bool bEuroStatus )
{
    if ( bEuroStatus )
    {
        m_Props->m_AmountDecimalPlaces = 2;
    }
}

// 
// Function:  Uninitialize
// Purpose:  Cleans up resources allocated during Initialize
// Input:  None
// Output:  None
// Assumptions:  None
// Comments:  None
//
void CFiscalProperties::Uninitialize() const
{
}

//
// Function:  GetPropertyNumber
// Purpose:  Implementation of the OPOS SO method for returning numeric
//           property values.
// Input: lPropIndex - identifies the property to be returned.
// Output:  The Value of the property.
// Assumption:  The calling method is responsible for validation of 
//              lPropIndex.  This function assumes it will be passed a
//              valid lPropIndex.
// Comments: None
//
long CFiscalProperties::GetPropertyNumber( long lPropIndex ) const
{
	long lResult = 0;

	switch( lPropIndex )
	{
    case PIDXFptr_AmountDecimalPlaces:
        // Need to check EuroStatus for real amount
        lResult = m_Props->m_AmountDecimalPlaces;
        break;

    case PIDXFptr_AsyncMode:
        lResult = m_Props->m_AsyncMode;
        break;

    case PIDXFptr_CheckTotal:
        lResult = m_Props->m_CheckTotal;
        break;

    case PIDXFptr_CountryCode:
        lResult = m_Props->m_CountryCode;
        break;

    case PIDXFptr_CoverOpen:
        lResult = m_Props->m_CoverOpen;
        break;

    case PIDXFptr_DayOpened:
        lResult = m_Props->m_DayOpened;
        break;

    case PIDXFptr_DescriptionLength:
        lResult = m_Props->m_DescriptionLength;
        break;

    case PIDXFptr_DuplicateReceipt:
        lResult = m_Props->m_DuplicateReceipt;
        break;

    case PIDXFptr_ErrorLevel:
        lResult = m_Props->m_ErrorLevel;
        break;

    case PIDXFptr_ErrorOutID:
        lResult = m_Props->m_ErrorOutID;
        break;

    case PIDXFptr_ErrorState:
        lResult = m_Props->m_ErrorState;
        break;

    case PIDXFptr_ErrorStation:
        lResult = m_Props->m_ErrorStation;
        break;

    case PIDXFptr_FlagWhenIdle:
        lResult = m_Props->m_FlagWhenIdle;
        break;

    case PIDXFptr_JrnEmpty:
        lResult = m_Props->m_Jrn.m_Empty;
        break;

    case PIDXFptr_JrnNearEnd:
        lResult = m_Props->m_Jrn.m_NearEnd;
        break;

    case PIDXFptr_MessageLength:
        lResult = m_Props->m_MessageLength;
        break;

    case PIDXFptr_NumHeaderLines:
        lResult = m_Props->m_NumHeaderLines;
        break;

    case PIDXFptr_NumTrailerLines:
        lResult = m_Props->m_NumTrailerLines;
        break;

    case PIDXFptr_NumVatRates:
        lResult = m_Props->m_NumVatRates;
        break;

    case PIDXFptr_PrinterState:
        // This property needs a round trip to the actual device
        lResult = m_Props->m_PrinterState;
        break;

    case PIDXFptr_QuantityDecimalPlaces:
        lResult = m_Props->m_QuantityDecimalPlaces;
        break;

    case PIDXFptr_QuantityLength:
        lResult = m_Props->m_QuantityLength;
        break;

    case PIDXFptr_RecEmpty:
        lResult = m_Props->m_Rec.m_Empty;
        break;

    case PIDXFptr_RecNearEnd:
        lResult = m_Props->m_Rec.m_NearEnd;
        break;

    case PIDXFptr_RemainingFiscalMemory:
        lResult = m_Props->m_RemainingFiscalMemory;
        break;

    case PIDXFptr_SlpEmpty:
        lResult = m_Props->m_Slp.m_Empty;
        break;

    case PIDXFptr_SlpNearEnd:
        lResult = m_Props->m_Slp.m_NearEnd;
        break;

    case PIDXFptr_SlipSelection:
        lResult = m_Props->m_SlipSelection;
        break;

    case PIDXFptr_TrainingModeActive:
        lResult = m_Props->m_TrainingModeActive;
        break;

    // * Capabilities *

    case PIDXFptr_CapAdditionalLines:
        lResult = m_Caps->m_CapAdditionalLines;
        break;

    case PIDXFptr_CapAmountAdjustment:
        lResult = m_Caps->m_CapAmountAdjustment;
        break;

    case PIDXFptr_CapAmountNotPaid:
        lResult = m_Caps->m_CapAmountNotPaid;
        break;

    case PIDXFptr_CapCheckTotal:
        lResult = m_Caps->m_CapCheckTotal;
        break;

    case PIDXFptr_CapCoverSensor:
        lResult = m_Caps->m_CapCoverSensor;
        break;

    case PIDXFptr_CapDoubleWidth:
        lResult = m_Caps->m_CapDoubleWidth;
        break;

    case PIDXFptr_CapDuplicateReceipt:
        lResult = m_Caps->m_CapDuplicateReceipt;
        break;

    case PIDXFptr_CapFixedOutput:
        lResult = m_Caps->m_CapFixedOutput;
        break;

    case PIDXFptr_CapHasVatTable:
        lResult = m_Caps->m_CapHasVatTable;
        break;

    case PIDXFptr_CapIndependentHeader:
        lResult = m_Caps->m_CapIndependentHeader;
        break;

    case PIDXFptr_CapItemList:
        lResult = m_Caps->m_CapItemList;
        break;

    case PIDXFptr_CapJrnEmptySensor:
        lResult = m_Caps->m_Jrn.m_CapEmptySensor;
        break;

    case PIDXFptr_CapJrnNearEndSensor:
        lResult = m_Caps->m_Jrn.m_CapNearEndSensor;
        break;

    case PIDXFptr_CapJrnPresent:
        lResult = m_Caps->m_Jrn.m_CapPresent;
        break;

    case PIDXFptr_CapNonFiscalMode:
        lResult = m_Caps->m_CapNonFiscalMode;
        break;

    case PIDXFptr_CapOrderAdjustmentFirst:
        lResult = m_Caps->m_CapOrderAdjustmentFirst;
        break;

    case PIDXFptr_CapPercentAdjustment:
        lResult = m_Caps->m_CapPercentAdjustment;
        break;

    case PIDXFptr_CapPositiveAdjustment:
        lResult = m_Caps->m_CapPositiveAdjustment;
        break;

    case PIDXFptr_CapPowerLossReport:
        lResult = m_Caps->m_CapPowerLossReport;
        break;

    case PIDXFptr_CapPredefinedPaymentLines:
        lResult = m_Caps->m_CapPredefinedPaymentLines;
        break;

    case PIDXFptr_CapReceiptNotPaid:
        lResult = m_Caps->m_CapReceiptNotPaid;
        break;

    case PIDXFptr_CapRecEmptySensor:
        lResult = m_Caps->m_Rec.m_CapEmptySensor;
        break;

    case PIDXFptr_CapRecNearEndSensor:
        lResult = m_Caps->m_Rec.m_CapNearEndSensor;
        break;

    case PIDXFptr_CapRecPresent:
        lResult = m_Caps->m_Rec.m_CapPresent;
        break;

    case PIDXFptr_CapRemainingFiscalMemory:
        lResult = m_Caps->m_CapRemainingFiscalMemory;
        break;

    case PIDXFptr_CapReservedWord:
        lResult = m_Caps->m_CapResevedWord;
        break;

    case PIDXFptr_CapSetHeader:
        lResult = m_Caps->m_CapSetHeader;
        break;

    case PIDXFptr_CapSetPOSID:
        lResult = m_Caps->m_CapSetPOSID;
        break;

    case PIDXFptr_CapSetStoreFiscalID:
        lResult = m_Caps->m_CapSetStoreFiscalID;
        break;

    case PIDXFptr_CapSetTrailer:
        lResult = m_Caps->m_CapSetTrailer;
        break;

    case PIDXFptr_CapSetVatTable:
        // ???
        lResult = m_Caps->m_CapVatTable;
        break;

    case PIDXFptr_CapSlpEmptySensor:
        lResult = m_Caps->m_Slp.m_CapEmptySensor;
        break;

    case PIDXFptr_CapSlpFiscalDocument:
        lResult = m_Caps->m_CapSlpFiscalDocument;
        break;

    case PIDXFptr_CapSlpFullSlip:
        lResult = m_Caps->m_CapSlpFullSlip;
        break;

    case PIDXFptr_CapSlpNearEndSensor:
        lResult = m_Caps->m_Slp.m_CapNearEndSensor;
        break;

    case PIDXFptr_CapSlpPresent:
        lResult = m_Caps->m_Slp.m_CapPresent;
        break;

    case PIDXFptr_CapSlpValidation:
        lResult = m_Caps->m_CapSlpValidation;
        break;

    case PIDXFptr_CapSubAmountAdjustment:
        lResult = m_Caps->m_CapSubAmountAdjustment;
        break;

    case PIDXFptr_CapSubPercentAdjustment:
        lResult = m_Caps->m_CapSubPercentAdjustment;
        break;

    case PIDXFptr_CapSubtotal:
        lResult = m_Caps->m_CapSubTotal;
        break;

    case PIDXFptr_CapTrainingMode:
        lResult = m_Caps->m_CapTrainingMode;
        break;

    case PIDXFptr_CapValidateJournal:
        lResult = m_Caps->m_CapValidateJournal;
        break;

    case PIDXFptr_CapXReport:
        lResult = m_Caps->m_CapXReport;
        break;

	default:
		break;
	}

	return lResult;
}

//
// Function:  SetPropertyNumber
// Purpose:  Implementation of the OPOS SO method for setting the numeric
//           property values.
// Input: lPropIndex - identifies the property to be modified
//        lProperty - the new value of the property
// Output:  None
// Assumption:  The calling method is responsible for validation of 
//              lPropIndex.  This function assumes it will be passed a
//              valid lPropIndex.
// Comments: None
//
void CFiscalProperties::SetPropertyNumber( long lPropIndex, long lProperty )
{
	switch( lPropIndex )
	{
    case PIDXFptr_AsyncMode:
        m_Props->m_AsyncMode = lProperty;
        break;

    case PIDXFptr_CheckTotal:
	    if( m_Caps->m_CapCheckTotal )
        {
            m_Props->m_CheckTotal = lProperty;
        }
        break;

    case PIDXFptr_DuplicateReceipt:
	    if( m_Caps->m_CapDuplicateReceipt )
        {
            m_Props->m_DuplicateReceipt = lProperty;
        }
        break;

    case PIDXFptr_FlagWhenIdle:
        m_Props->m_FlagWhenIdle = lProperty;
        break;

    case PIDXFptr_SlipSelection:
	    if( lProperty == FPTR_SS_FULL_LENGTH)
        {
            m_Props->m_SlipSelection = lProperty;
            m_Props->m_FiscalDocumentType = OPOS_FPTR_FO_CLASS2_RECEIPT;
        }
	    else if( lProperty == FPTR_SS_VALIDATION )
        {
            m_Props->m_SlipSelection = lProperty;
            m_Props->m_FiscalDocumentType = OPOS_FPTR_FO_CLASS3;
        }
        break;

	default:
		break;
	}
}

//
// Function:  GetPropertyString
// Purpose:  Implementation of the OPOS SO method for returning string
//           property values.
// Input: lPropIndex - identifies the property to be returned.
// Output:  The Value of the property.
// Assumption:  The calling method is responsible for validation of 
//              lPropIndex.  This function assumes it will be passed a
//              valid lPropIndex.
// Comments: 
//   The corresponding SetPropertyString is not needed for the Printer
//   since all of the printer's string properties are read only.
//
CString CFiscalProperties::GetPropertyString( long lPropIndex ) const
{
	CString sRet = "";

   	switch( lPropIndex )
    {
    case PIDXFptr_ErrorString:
        sRet = m_Props->m_ErrorString;
        break;

    case PIDXFptr_PredefinedPaymentLines:
        sRet = m_Props->m_PredefinedPaymentLines;
        break;

    case PIDXFptr_ReservedWord:
        sRet = m_Props->m_ReservedWord;
        break;

    default:
        break;
    }

	return sRet;
}


void CFiscalProperties::SetFiscalProperty( long lPropIndex, long lPropValue )
{
    switch ( lPropIndex )
    {
    case PIDXFptr_TrainingModeActive:
        m_Props->m_TrainingModeActive = (lPropValue != 0);
        break;
    case PIDXFptr_DayOpened:
        m_Props->m_DayOpened = (lPropValue != 0);
        break;
    case PIDXFptr_PrinterState:
        m_Props->m_PrinterState = lPropValue;
        break;
    default:
        break;
    }
}