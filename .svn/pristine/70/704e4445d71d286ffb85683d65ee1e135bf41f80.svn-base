// PrinterGlobal.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTERGLOBAL_H__8275B966_E7F6_467D_9351_8A2FCE7D705A__INCLUDED_)
#define AFX_PRINTERGLOBAL_H__8275B966_E7F6_467D_9351_8A2FCE7D705A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrinterProperties.h"

// **************************************************************
// Property and Metric definitions for the Axiohm 7156 Printer


#define AXIOHM7156_CHARSETLIST			_T("437,850,858,998,999,101")
#define AXIOHM7156_FONTLIST				_T("")
#define AXIOHM7156_JRNLINECHARS			_T("")
#define AXIOHM7156_RECLINECHARS			_T("44,56")
#define AXIOHM7156_RECBARCODE			_T("0")
#define AXIOHM7156_SLPLINECHARS			_T("66,80")
#define AXIOHM7156_SLPBARCODE			_T("0")

// **************************************************************
// Property and Metric definitions for the Axiohm 7156 Printer

static const PrinterCommon g_Axiohm7156_Cmn = 
{
	998,		// CapCharacterSet
	FALSE,		// CapConcurrentJrnRec
	FALSE,		// CapConcurrentJrnSlp
	FALSE,		// CapConcurrentRecSlp
	TRUE,		// CapCoverSensor
	TRUE,		// CapTransaction
	FALSE,		// AsyncMode
	0,			// CharacterSet
	FALSE,		// CoverOpen
	1,			// ErrorLevel
	0,			// ErrorStation
	FALSE,		// FlagWhenIdle
	1,			// MapMode
	1			// RotateSpecial

};

static const PrinterJournal g_Axiohm7156_Jrn =
{
	FALSE,		// CapJrnPresent
	FALSE,		// CapJrn2Color
	FALSE,		// CapJrnBold
	FALSE,		// CapJrnDhigh
	FALSE,		// CapJrnDwide
	FALSE,		// CapJrnDwideDhigh
	FALSE,		// CapJrnEmptySensor
	FALSE,		// CapJrnItalic
	FALSE,		// CapJrnNearEndSensor
	FALSE,		// CapJrnUnderline
	0,			// JrnLineChars
	0,			// JrnLineHeight
	0,			// JrnLineSpacing
	0,			// JrnLineWidth
	FALSE,		// JrnLetterQuality
	FALSE,		// JrnEmpty
	FALSE		// JrnNearEnd
};

static const PrinterReceipt g_Axiohm7156_Rec =
{
	TRUE,		// CapRecPresent
	FALSE,		// CapRec2Color
	TRUE,		// CapRecBarCode
	TRUE,		// CapRecBitmap
	FALSE,		// CapRecBold
	TRUE,		// CapRecDhigh
	TRUE,		// CapRecDwide
	TRUE,		// CapRecDwideDhigh
	TRUE,		// CapRecEmptySensor
	FALSE,		// CapRecItalic
	TRUE,		// CapRecLeft90
	TRUE,		// CapRecNearEndSensor
	TRUE,		// CapRecPapercut
	TRUE,		// CapRecRight90
	TRUE,		// CapRecRotate180
	FALSE,		// CapRecStamp
	FALSE,		// CapREcUnderline
	44,			// RecLineChars
	18,			// RecLineHeight
	20,			// RecLineSpacing
	440,		// RecLineWidth
	FALSE,		// RecLetterQuality
	FALSE,		// RecEmpty
	FALSE,		// RecNearEnd
	22,			// RecSidewaysMaxLines
	500,		// RecSidewaysMaxChars
	6			// RecLinesToPaperCut
};

static const PrinterSlip g_Axiohm7156_Slp;
/***  // default values in base contructor
 =
{
	TRUE,		// CapSlpPresent
	TRUE,		// CapSlpFullslip
	FALSE,		// CapSlp2Color
	FALSE,		// CapSlpBarCode
	TRUE,		// CapSlpBitmap
	TRUE,		// CapSlpBold
	FALSE,		// CapSlpDhigh
	TRUE,		// CapSlpDwide
	FALSE,		// CapSlpDwideDhigh
	TRUE,		// CapSlpEmptySensor
	FALSE,		// CapSlpItalic
	TRUE,		// CapSlpLeft90
	TRUE,		// CapSlpNearEndSensor
	TRUE,		// CapSlpRight90
	TRUE,		// CapSlpRotate180
	FALSE,		// CapSlpUnderline
	66,			// SlpLineChars
	7,			// SlpLineHeight
	10,			// SlpLineSpacing
	660,		// SlpLineWidth
	FALSE,		// SlpLetterQuality
	FALSE,		// SlpEmpty
	FALSE,		// SlpNearEnd
	33,			// SlpSidewaysMaxLines;
	500,		// SlpSidewaysMaxChars;
	0,			// SlpMaxLines;
	8			// SlpLinesNearEndToEnd;
};
*****/

static const PrinterMetrics g_Axiohm7156_RecMetric =
{
    (float) (1.0/6.0),          // m_fMMPerDotHigh
    (float) (1.0/6.0),          // m_fMMPerDotWide
    44,                         // m_nLineChars
    18,                         // m_nDotLineHeight
    2,                          // m_nDotLineWhitespace
    12,                         // m_nMaxDotLineWhitespace
    10,                         // m_nDotCharWidth
    4,                          // m_nDotMargin      [ (4 + 44*10 +
    4,                          // m_nDotMarginRight [  4) = 448
    (float) 0.5,                // m_fSidewaysLineRatio
    (float) 1.0,                // m_fBitmapDotRatio
    17*6,                       // m_nDotsToPaperCut
    23*6,                       // m_nDotsToPaperTear
    0,                          // m_nDotsNearEndToEnd
    0                           // m_fBitmapDotHeightMM
};

static const PrinterMetrics g_Axiohm7156_RecMetric_Comp =
{
    (float) (1.0/6.0),          // m_fMMPerDotHigh
    (float) (1.0/6.0),          // m_fMMPerDotWide
    56,                         // m_nLineChars
    18,                         // m_nDotLineHeight
    2,                          // m_nDotLineWhitespace
    12,                         // m_nMaxDotLineWhitespace
    8,                          // m_nDotCharWidth
    0,                          // m_nDotMargin      [ (0 + 56*8 +
    0,                          // m_nDotMarginRight [  0) = 448
    (float) (0.5*44.0/56.0),    // m_fSidewaysLineRatio
    (float) 1.0,                // m_fBitmapDotRatio
    17*6,                       // m_nDotsToPaperCut
    23*6,                       // m_nDotsToPaperTear
    0,                          // m_nDotsNearEndToEnd
    0                           // m_fBitmapDotHeightMM
};

static const PrinterMetrics g_Axiohm7156_SlpMetric =
{
    (float) (MMPERINCH/72),    // m_fMMPerDotHigh
    (float) (120.7/660),        // m_fMMPerDotWide (half-dot size)
    66,                         // m_nLineChars
    7,                          // m_nDotLineHeight
    3,                          // m_nDotLineWhitespace
    12,                         // m_nMaxDotLineWhitespace
    10,                         // m_nDotCharWidth
    0,                          // m_nDotMargin      [ (0 + 66*10 +
    0,                          // m_nDotMarginRight [  0) = 660
    (float) 0.5,                // m_fSidewaysLineRatio
    (float) 1.0,                // m_fBitmapDotRatio
    0,                          // m_nDotsToPaperCut
    0,                          // m_nDotsToPaperTear
    75,                         // m_nDotsNearEndToEnd
    0                           // m_fBitmapDotHeightMM
};

static const PrinterMetrics g_Axiohm7156_SlpMetric_Comp =
{
    (float) (MMPERINCH/72),    // m_fMMPerDotHigh
    (float) (120.7/800),        // m_fMMPerDotWide (half-dot size)
    80,                         // m_nLineChars
    7,                          // m_nDotLineHeight
    3,                          // m_nDotLineWhitespace
    12,                         // m_nMaxDotLineWhitespace
    10,                         // m_nDotCharWidth
    0,                          // m_nDotMargin      [ (0 + 80*10/BitmapDotRatio +
    0,                          // m_nDotMarginRight [  0) = 660
    (float) 0.5,                // m_fSidewaysLineRatio
    (float) (800.0/660.0),      // m_fBitmapDotRatio
    0,                          // m_nDotsToPaperCut
    0,                          // m_nDotsToPaperTear
    75,                         // m_nDotsNearEndToEnd
    0                           // m_fBitmapDotHeightMM
};

//
// ******************************************************************

#endif // !defined(AFX_PRINTERGLOBAL_H__8275B966_E7F6_467D_9351_8A2FCE7D705A__INCLUDED_)
