// PrinterProperties.h: interface for the CPrinterProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTERPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_)
#define AFX_PRINTERPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MMPERINCH	25.400

#define SLP_EMPTY_UNKNOWN 0xabcddcba

class PrinterCommon
{
public:
	long	m_lCapCharacterSet;
	BOOL	m_bCapConcurrentJrnRec;
	BOOL	m_bCapConcurrentJrnSlp;
	BOOL	m_bCapConcurrentRecSlp;
	BOOL	m_bCapCoverSensor;
	BOOL	m_bCapTransaction;
	BOOL	m_bAsyncMode;
	long	m_lCharacterSet;
	bool	m_bCoverOpen;
	long	m_lErrorLevel;
	long	m_lErrorStation;
	BOOL	m_bFlagWhenIdle;
	long	m_lMapMode;
	long	m_lRotateSpecial;
};

class PrinterJournal
{
public:
	BOOL	m_bCapJrnPresent;
	BOOL	m_bCapJrn2Color;
	BOOL	m_bCapJrnBold;
	BOOL	m_bCapJrnDhigh;
	BOOL	m_bCapJrnDwide;
	BOOL	m_bCapJrnDwideDhigh;
	BOOL	m_bCapJrnEmptySensor;
	BOOL	m_bCapJrnItalic;
	BOOL	m_bCapJrnNearEndSensor;
	BOOL	m_bCapJrnUnderline;
	long	m_lJrnLineChars;
	long	m_lJrnLineHeight;
	long	m_lJrnLineSpacing;
	long	m_lJrnLineWidth;
	BOOL	m_bJrnLetterQuality;
	BOOL	m_bJrnEmpty;
	BOOL	m_bJrnNearEnd;
};

class PrinterReceipt
{
public:
	BOOL	m_bCapRecPresent;
	BOOL	m_bCapRec2Color;
	BOOL	m_bCapRecBarCode;
	BOOL	m_bCapRecBitmap;
	BOOL	m_bCapRecBold;
	BOOL	m_bCapRecDhigh;
	BOOL	m_bCapRecDwide;
	BOOL	m_bCapRecDwideDhigh;
	BOOL	m_bCapRecEmptySensor;
	BOOL	m_bCapRecItalic;
	BOOL	m_bCapRecLeft90;
	BOOL	m_bCapRecNearEndSensor;
	BOOL	m_bCapRecPapercut;
	BOOL	m_bCapRecRight90;
	BOOL	m_bCapRecRotate180;
	BOOL	m_bCapRecStamp;
	BOOL	m_bCapRecUnderline;
	long	m_lRecLineChars;
	long	m_lRecLineHeight;
	long	m_lRecLineSpacing;
	long	m_lRecLineWidth;
	BOOL	m_bRecLetterQuality;
	BOOL	m_bRecEmpty;
	BOOL	m_bRecNearEnd;
	long	m_lRecSidewaysMaxLines;
	long	m_lRecSidewaysMaxChars;
	long	m_lRecLinesToPaperCut;
};

class PrinterSlip 
{
public:
   PrinterSlip() {
	   m_bCapSlpPresent=TRUE;		// CapSlpPresent
	   m_bCapSlpFullslip=TRUE;		// CapSlpFullslip
	   m_bCapSlp2Color=FALSE;		// CapSlp2Color
	   m_bCapSlpBarCode=FALSE;		// CapSlpBarCode
	   m_bCapSlpBitmap=TRUE;		// CapSlpBitmap
	   m_bCapSlpBold=TRUE;		// CapSlpBold
	   m_bCapSlpDhigh=FALSE;		// CapSlpDhigh
	   m_bCapSlpDwide=TRUE;		// CapSlpDwide
	   m_bCapSlpDwideDhigh=FALSE;		// CapSlpDwideDhigh
	   m_bCapSlpEmptySensor=TRUE;		// CapSlpEmptySensor
	   m_bCapSlpItalic=FALSE;		// CapSlpItalic
	   m_bCapSlpLeft90=TRUE;		// CapSlpLeft90
	   m_bCapSlpNearEndSensor=TRUE;		// CapSlpNearEndSensor
	   m_bCapSlpRight90=TRUE;		// CapSlpRight90
	   m_bCapSlpRotate180=TRUE;		// CapSlpRotate180
	   m_bCapSlpUnderline=FALSE;		// CapSlpUnderline
	   m_lSlpLineChars=66;			// SlpLineChars
	   m_lSlpLineHeight=7;			// SlpLineHeight
	   m_lSlpLineSpacing=10;			// SlpLineSpacing
	   m_lSlpLineWidth=660;		// SlpLineWidth
	   m_bSlpLetterQuality=FALSE;		// SlpLetterQuality
	   m_bSlpEmpty=TRUE;		// SlpEmpty
	   m_bSlpNearEnd=FALSE;		// SlpNearEnd
	   m_lSlpSidewaysMaxLines=33;			// SlpSidewaysMaxLines=
	   m_lSlpSidewaysMaxChars=500;		// SlpSidewaysMaxChars;
	   m_lSlpMaxLines=0;			// SlpMaxLines;
	   m_lSlpLinesNearEndToEnd=8;			// SlpLinesNearEndToEnd;
   };
	BOOL	m_bCapSlpPresent;
	BOOL	m_bCapSlpFullslip;
	BOOL	m_bCapSlp2Color;
	BOOL	m_bCapSlpBarCode;
	BOOL	m_bCapSlpBitmap;
	BOOL	m_bCapSlpBold;
	BOOL	m_bCapSlpDhigh;
	BOOL	m_bCapSlpDwide;
	BOOL	m_bCapSlpDwideDhigh;
	BOOL	m_bCapSlpEmptySensor;
	BOOL	m_bCapSlpItalic;
	BOOL	m_bCapSlpLeft90;
	BOOL	m_bCapSlpNearEndSensor;
	BOOL	m_bCapSlpRight90;
	BOOL	m_bCapSlpRotate180;
	BOOL	m_bCapSlpUnderline;
	long	m_lSlpLineChars;
	long	m_lSlpLineHeight;
	long	m_lSlpLineSpacing;
	long	m_lSlpLineWidth;
	BOOL	m_bSlpLetterQuality;
	BOOL	m_bSlpEmpty;
	BOOL	m_bSlpNearEnd;
	long	m_lSlpSidewaysMaxLines;
	long	m_lSlpSidewaysMaxChars;
	long	m_lSlpMaxLines;
	long	m_lSlpLinesNearEndToEnd;
};

class PrinterMetrics
{
public:
    // Physical minimum unit size.
    float m_fMMPerDotHigh;              // MM per vertical dot.
    float m_fMMPerDotWide;              // MM per horizontal dot.
    // The metrics.
    long  m_nLineChars;                 // Max characters on a line.
    long  m_nDotLineHeight;             // Line height in dots.
    long  m_nDotLineWhitespace;         // Default line spacing in dots.
    long  m_nMaxDotLineWhitespace;      // Maximum line spacing in dots.
    long  m_nDotCharWidth;              // Character width in dots.
    long  m_nDotMargin;                 // Margin from leftmost bit-image bit to
                                        //   first text dot of first character.
    long  m_nDotMarginRight;            // Margin from last text dot of last
                                        //   character to rightmost bit-image bit.
    float m_fSidewaysLineRatio;         // Max # sideways lines =
                                        //   LineChars * (this ratio).
    float m_fBitmapDotRatio;            // Width of bitmap dot =
                                        //   Width of text dot * (this ratio).
    long  m_nDotsToPaperCut;            // Dots from end of print line to paper cutter.
                                        //   Include enough to leave a small margin.
    long  m_nDotsToPaperTear;           // Same, but to tear bar.
    long  m_nDotsNearEndToEnd;          // Dots from slip "near end" sensor until
                                        //   cannot print another line.
    float m_fBitmapDotHeightMM;         // This is a kludge added to support printers
                                        //   that seem to use a different dot height
                                        //   for bitmaps and dot feeds.
                                        //   A value of 0 indicates that the dot
                                        //   height is the same as for dot feeds.
                                        //   If non-zero, use the specified value as
                                        //   the dot height for bitmap printing.
};



class CPrinterProperties 
{
public:
	CPrinterProperties( void );
	~CPrinterProperties( void );

	void Initialize( CString sModel, int nSlipToggle );
	void Uninitialize();
	CString GetPropertyString( long lPropIndex );
	long GetPropertyNumber( long lPropIndex );
	void SetPropertyNumber(long lPropIndex, long lProperty);
	long ConvertHeight( long Station, long nUnits );
	long ConvertWidth( long Station, long nUnits );
	void SlipInserted();
	void SlipRemoved();

	void Override( COposRegistry& oReg, CmDataCapture* dCap );
	
private:
	void UpdateJrnMetrics();
	void UpdateRecMetrics();
	void UpdateSlpMetrics();
	void InitializePropertyMap( CMapStringToPtr& oMap );
	void EmptyPropertyMap( CMapStringToPtr& oMap );
	void OverrideProperty( long lProp, CString& csVal);
	long ConvertFromMapMode( float fConvert, long nUnits );
	long ConvertToMapMode( float fConvert, long nDots );

	// Common Properties
	CString						m_sCharacterSetList;
	CString						m_sErrorString;
	CString						m_sFontTypefaceList;
	PrinterCommon				m_Cmn;
	bool						m_bInitialized;

	// Journal Station Properties
	CString						m_sJrnLineCharsList;
	PrinterJournal				m_Jrn;
	PrinterMetrics**			m_ppJrnMetrics;
	long						m_lJrnActiveMetric;

	// Receipt Station Properties
	CString						m_sRecLineCharsList;
	CString						m_sRecBarCodeRotationList;
	PrinterReceipt				m_Rec;
	PrinterMetrics**			m_ppRecMetrics;
	long						m_lRecActiveMetric;

	// Slip Station Properties
	CString						m_sSlpLineCharsList;
	CString						m_sSlpBarCodeRotationList;
	PrinterSlip					m_Slp;
	PrinterMetrics**			m_ppSlpMetrics;
	long						m_lSlpActiveMetric;
	long						m_lSlpLastEmptyStatus;
	bool						m_bSlipStatusToggle;
};

#endif // !defined(AFX_PRINTERPROPERTIES_H__F7022A5D_BFB0_447C_A440_DE33E3DF384B__INCLUDED_)
