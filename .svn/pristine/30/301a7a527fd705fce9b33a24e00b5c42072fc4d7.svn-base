// PrinterProperties.h: interface for the CFiscalProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FISCALPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_)
#define AFX_FISCALPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const double dMMPerInch = 25.400;

////////////////////////////////////////////////////////////////////////
// Printer Properties
////////////////////////////////////////////////////////////////////////
// ** Capabilities **

class CFPrinterStnCaps
{
public:
	BOOL m_CapPresent;
	BOOL m_CapEmptySensor;
	BOOL m_CapNearEndSensor;
};

class CFPrinterCaps
{
public:
	BOOL m_CapAdditionalLines;
	BOOL m_CapAmountAdjustment;
	BOOL m_CapAmountNotPaid;
	BOOL m_CapCheckTotal;

	CFPrinterStnCaps m_Jrn;
	CFPrinterStnCaps m_Rec;
   CFPrinterStnCaps m_Slp;

	BOOL m_CapCoverSensor;
	BOOL m_CapFixedOutput;
	BOOL m_CapHasVatTable;
	BOOL m_CapIndependentHeader;
	BOOL m_CapItemList;

	BOOL m_CapDuplicateReceipt;
	BOOL m_CapSlpFullSlip;
	BOOL m_CapSlpFiscalDocument;
	BOOL m_CapSlpValidation;

	BOOL m_CapValidateJournal;

	BOOL m_CapNonFiscalMode;
	BOOL m_CapOrderAdjustmentFirst;
	BOOL m_CapPercentAdjustment;
	BOOL m_CapPositiveAdjustment;
	BOOL m_CapPowerLossReport;
	BOOL m_CapPredefinedPaymentLines;
	BOOL m_CapReceiptNotPaid;

	BOOL m_CapDoubleWidth;

	BOOL m_CapSubAmountAdjustment;
	BOOL m_CapSubPercentAdjustment;
	BOOL m_CapSubTotal;
	BOOL m_CapTrainingMode;

	BOOL m_CapRemainingFiscalMemory;
	BOOL m_CapResevedWord;
	BOOL m_CapSetHeader;
	BOOL m_CapSetPOSID;
	BOOL m_CapSetStoreFiscalID;
	BOOL m_CapSetTrailer;
	BOOL m_CapVatTable; //for CapSetVatTable

	BOOL m_CapXReport;
	//   (End)
};

// ** Properties **
class CFPrinterStnNumericProps
{
public:
	~CFPrinterStnNumericProps(){}
	BOOL m_Empty;
	BOOL m_NearEnd;
};


class CFPrinterNumericProps
{
public:
    BOOL m_AsyncMode;
	BOOL m_CheckTotal;
	BOOL m_CoverOpen;
	LONG m_CountryCode;
	BOOL m_FlagWhenIdle;	//This flag is set but not used as all methods are performed synchrnously.
	BOOL m_DayOpened;
	LONG m_AmountDecimalPlaces;

	CFPrinterStnNumericProps m_Jrn;

	CFPrinterStnNumericProps m_Rec;

	LONG m_SlipSelection;
	LONG m_DescriptionLength;
	BOOL m_DuplicateReceipt;

	CFPrinterStnNumericProps m_Slp;

	BOOL m_ErrorLevel;
	LONG m_ErrorOutID;
	LONG m_ErrorState;
	LONG m_ErrorStation;
	LPTSTR m_ErrorString;


    LONG m_MessageLength;
	LONG m_NumHeaderLines;
	LONG m_NumTrailerLines;
	LONG m_NumVatRates;
	LPTSTR m_PredefinedPaymentLines;

	LONG m_PrinterState;
	LONG m_QuantityDecimalPlaces;
	LONG m_QuantityLength;
	LONG m_RemainingFiscalMemory;
	LPTSTR m_ReservedWord;
	BOOL m_TrainingModeActive;

	long	m_PowerNotify;
	long	m_CapPowerReporting;
	long	m_PowerState;
	long	m_FixedDocumentType;
	long	m_FiscalDocumentType;
	
    //   (End)
};




class CFiscalProperties 
{
public:
	CFiscalProperties( void );
	~CFiscalProperties( void );

	void Initialize( bool bEuroStatus );
	void Uninitialize() const;
	CString GetPropertyString( long lPropIndex ) const;
	long GetPropertyNumber( long lPropIndex ) const;
	void SetPropertyNumber(long lPropIndex, long lProperty);

    void SetFiscalProperty( long lPropIndex, long lPropValue );
	
private:
    CFPrinterCaps* m_Caps;
    CFPrinterNumericProps* m_Props;
};

#endif // !defined(AFX_FISCALPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_)
