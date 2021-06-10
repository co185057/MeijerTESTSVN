// PrinterProperties.cpp: implementation of the CPrinterProperties class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "PrinterProperties.h"
#include "PrinterGlobal.h"
#include "OposPtr.hi"
//#include "std_opos.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinterProperties::CPrinterProperties()
{
	m_ppRecMetrics = NULL;
	m_ppSlpMetrics = NULL;
	m_ppJrnMetrics = NULL;
	m_bInitialized = FALSE;
}

CPrinterProperties::~CPrinterProperties()
{
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
void CPrinterProperties::Initialize( CString sModel, int nSlipToggle )
{
	m_bSlipStatusToggle = (nSlipToggle > 0);
	m_bInitialized = TRUE;

	// "Axiohm7156" AND default value.
	m_Cmn = g_Axiohm7156_Cmn;
	m_sErrorString = _T("");
	m_sCharacterSetList = AXIOHM7156_CHARSETLIST;
	m_sFontTypefaceList = AXIOHM7156_FONTLIST;

	m_Jrn = g_Axiohm7156_Jrn;
	m_sJrnLineCharsList = AXIOHM7156_JRNLINECHARS;
	m_ppJrnMetrics = NULL;
	m_lJrnActiveMetric = -1;

	m_Rec = g_Axiohm7156_Rec;
	m_sRecLineCharsList = AXIOHM7156_RECLINECHARS;
	m_sRecBarCodeRotationList = AXIOHM7156_RECBARCODE;
	m_ppRecMetrics = new PrinterMetrics*[ 3 ];
	m_ppRecMetrics[0] = new PrinterMetrics;
	*m_ppRecMetrics[0] = g_Axiohm7156_RecMetric;
	m_ppRecMetrics[1] = new PrinterMetrics;
	*m_ppRecMetrics[1] = g_Axiohm7156_RecMetric_Comp;
	m_ppRecMetrics[2] = NULL;
	m_lRecActiveMetric = 0;

	m_Slp = g_Axiohm7156_Slp;
	m_sSlpLineCharsList = AXIOHM7156_SLPLINECHARS;
	m_sSlpBarCodeRotationList = AXIOHM7156_SLPBARCODE;
	m_ppSlpMetrics = new PrinterMetrics*[ 3 ];
	m_ppSlpMetrics[0] = new PrinterMetrics;
	*m_ppSlpMetrics[0] = g_Axiohm7156_SlpMetric;
	m_ppSlpMetrics[1] = new PrinterMetrics;
	*m_ppSlpMetrics[1] = g_Axiohm7156_SlpMetric_Comp;
	m_ppSlpMetrics[2] = NULL;
	m_lSlpActiveMetric = 0;
	m_lSlpLastEmptyStatus = SLP_EMPTY_UNKNOWN;

	if ( sModel.CompareNoCase( _T("TM-H5000") ) == 0 )
	{
		// This was done in support of Bunnings.  Bunnings reads RecLineCharsList and performs
		// a calculation on the first value to get the second.
		m_sRecLineCharsList = _T("42,56");
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
void CPrinterProperties::Uninitialize()
{
	PrinterMetrics** ppMetric;

	if ( m_ppJrnMetrics )
	{
		for ( ppMetric=m_ppJrnMetrics; *ppMetric!=NULL; ppMetric++ )	
		{
			delete *ppMetric;
		}
		delete[] m_ppJrnMetrics;
	}

	if ( m_ppRecMetrics )
	{
		for ( ppMetric=m_ppRecMetrics; *ppMetric!=NULL; ppMetric++ )	
		{
			delete *ppMetric;
		}
		delete[] m_ppRecMetrics;
	}

	if ( m_ppSlpMetrics )
	{
		for ( ppMetric=m_ppSlpMetrics; *ppMetric!=NULL; ppMetric++ )	
		{
			delete *ppMetric;
		}
		delete[] m_ppSlpMetrics;
	}

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
long CPrinterProperties::GetPropertyNumber( long lPropIndex )
{
	long lResult = 0;

	switch( lPropIndex )
	{
	case PIDXPtr_AsyncMode:
		lResult = m_Cmn.m_bAsyncMode;
		break;

	case PIDXPtr_CharacterSet:
		lResult = m_Cmn.m_lCharacterSet;
		break;

	case PIDXPtr_CoverOpen:
		lResult = m_Cmn.m_bCoverOpen;
		break;

	case PIDXPtr_ErrorStation:
		lResult = m_Cmn.m_lErrorStation;
		break;

	case PIDXPtr_FlagWhenIdle:
		lResult = m_Cmn.m_bFlagWhenIdle;
		break;

	case PIDXPtr_JrnEmpty:
		lResult = m_Jrn.m_bJrnEmpty;
		break;

	case PIDXPtr_JrnLetterQuality:
		lResult = m_Jrn.m_bJrnLetterQuality;
		break;

	case PIDXPtr_JrnLineChars:
		lResult = m_Jrn.m_lJrnLineChars;
		break;

	case PIDXPtr_JrnLineHeight:
		lResult = m_Jrn.m_lJrnLineHeight;
		break;

	case PIDXPtr_JrnLineSpacing:
		lResult = m_Jrn.m_lJrnLineSpacing;
		break;

	case PIDXPtr_JrnLineWidth:
		lResult = m_Jrn.m_lJrnLineWidth;
		break;

	case PIDXPtr_JrnNearEnd:
		lResult = m_Jrn.m_bJrnNearEnd;
		break;

	case PIDXPtr_MapMode:
		lResult = m_Cmn.m_lMapMode;
		break;

	case PIDXPtr_RecEmpty:
		lResult = m_Rec.m_bRecEmpty;
		break;

	case PIDXPtr_RecLetterQuality:
		lResult = m_Rec.m_bRecLetterQuality;
		break;

	case PIDXPtr_RecLineChars:
		lResult = m_Rec.m_lRecLineChars;
		break;

	case PIDXPtr_RecLineHeight:
		lResult = m_Rec.m_lRecLineHeight;
		break;

	case PIDXPtr_RecLineSpacing:
		lResult = m_Rec.m_lRecLineSpacing;
		break;

	case PIDXPtr_RecLinesToPaperCut:
		lResult = m_Rec.m_lRecLinesToPaperCut;
		break;

	case PIDXPtr_RecLineWidth:
		lResult = m_Rec.m_lRecLineWidth;
		break;

	case PIDXPtr_RecNearEnd:
		lResult = m_Rec.m_bRecNearEnd;
		break;

	case PIDXPtr_RecSidewaysMaxChars:
		lResult = m_Rec.m_lRecSidewaysMaxChars;
		break;

	case PIDXPtr_RecSidewaysMaxLines:
		lResult = m_Rec.m_lRecSidewaysMaxLines;
		break;

	case PIDXPtr_SlpEmpty:
		if ( m_bSlipStatusToggle )
		{
			if ( m_lSlpLastEmptyStatus == m_Slp.m_bSlpEmpty)
			{
				m_Slp.m_bSlpEmpty = !m_Slp.m_bSlpEmpty;
				m_lSlpLastEmptyStatus = SLP_EMPTY_UNKNOWN;
			}
			else
			{
				m_lSlpLastEmptyStatus = m_Slp.m_bSlpEmpty;
			}
		}
		lResult = m_Slp.m_bSlpEmpty;
		break;

	case PIDXPtr_SlpLetterQuality:
		lResult = m_Slp.m_bSlpLetterQuality;
		break;

	case PIDXPtr_SlpLineChars:
		lResult = m_Slp.m_lSlpLineChars;
		break;

	case PIDXPtr_SlpLineHeight:
		lResult = m_Slp.m_lSlpLineHeight;
		break;

	case PIDXPtr_SlpLinesNearEndToEnd:
		lResult = m_Slp.m_lSlpLinesNearEndToEnd;
		break;

	case PIDXPtr_SlpLineSpacing:
		lResult = m_Slp.m_lSlpLineSpacing;
		break;

	case PIDXPtr_SlpLineWidth:
		lResult = m_Slp.m_lSlpLineWidth;
		break;

	case PIDXPtr_SlpMaxLines:
		lResult = m_Slp.m_lSlpMaxLines;
		break;

	case PIDXPtr_SlpNearEnd:
		lResult = m_Slp.m_bSlpNearEnd;
		break;

	case PIDXPtr_SlpSidewaysMaxChars:
		lResult = m_Slp.m_lSlpSidewaysMaxChars;
		break;

	case PIDXPtr_SlpSidewaysMaxLines:
		lResult = m_Slp.m_lSlpSidewaysMaxLines;
		break;

	case PIDXPtr_ErrorLevel:
		lResult = m_Cmn.m_lErrorLevel;
		break;

	case PIDXPtr_RotateSpecial:
		lResult = m_Cmn.m_lRotateSpecial;
		break;

	case PIDXPtr_CapConcurrentJrnRec:
		lResult = m_Cmn.m_bCapConcurrentJrnRec;
		break;

	case PIDXPtr_CapConcurrentJrnSlp:
		lResult = m_Cmn.m_bCapConcurrentJrnSlp;
		break;

	case PIDXPtr_CapConcurrentRecSlp:
		lResult = m_Cmn.m_bCapConcurrentRecSlp;
		break;

	case PIDXPtr_CapCoverSensor:
		lResult = m_Cmn.m_bCapCoverSensor;
		break;

	case PIDXPtr_CapJrn2Color:
		lResult = m_Jrn.m_bCapJrn2Color;
		break;

	case PIDXPtr_CapJrnBold:
		lResult = m_Jrn.m_bCapJrnBold;
		break;

	case PIDXPtr_CapJrnDhigh:
		lResult = m_Jrn.m_bCapJrnDhigh;
		break;

	case PIDXPtr_CapJrnDwide:
		lResult = m_Jrn.m_bCapJrnDwide;
		break;

	case PIDXPtr_CapJrnDwideDhigh:
		lResult = m_Jrn.m_bCapJrnDwideDhigh;
		break;

	case PIDXPtr_CapJrnEmptySensor:
		lResult = m_Jrn.m_bCapJrnEmptySensor;
		break;

	case PIDXPtr_CapJrnItalic:
		lResult = m_Jrn.m_bCapJrnItalic;
		break;

	case PIDXPtr_CapJrnNearEndSensor:
		lResult = m_Jrn.m_bCapJrnNearEndSensor;
		break;

	case PIDXPtr_CapJrnPresent:
		lResult = m_Jrn.m_bCapJrnPresent;
		break;

	case PIDXPtr_CapJrnUnderline:
		lResult = m_Jrn.m_bCapJrnUnderline;
		break;

	case PIDXPtr_CapRec2Color:
		lResult = m_Rec.m_bCapRec2Color;
		break;

	case PIDXPtr_CapRecBarCode:
		lResult = m_Rec.m_bCapRecBarCode;
		break;

	case PIDXPtr_CapRecBitmap:
		lResult = m_Rec.m_bCapRecBitmap;
		break;

	case PIDXPtr_CapRecBold:
		lResult = m_Rec.m_bCapRecBold;
		break;

	case PIDXPtr_CapRecDhigh:
		lResult = m_Rec.m_bCapRecDhigh;
		break;

	case PIDXPtr_CapRecDwide:
		lResult = m_Rec.m_bCapRecDwide;
		break;

	case PIDXPtr_CapRecDwideDhigh:
		lResult = m_Rec.m_bCapRecDwideDhigh;
		break;

	case PIDXPtr_CapRecEmptySensor:
		lResult = m_Rec.m_bCapRecEmptySensor;
		break;

	case PIDXPtr_CapRecItalic:
		lResult = m_Rec.m_bCapRecItalic;
		break;

	case PIDXPtr_CapRecLeft90:
		lResult = m_Rec.m_bCapRecLeft90;
		break;

	case PIDXPtr_CapRecNearEndSensor:
		lResult = m_Rec.m_bCapRecNearEndSensor;
		break;

	case PIDXPtr_CapRecPapercut:
		lResult = m_Rec.m_bCapRecPapercut;
		break;

	case PIDXPtr_CapRecPresent:
		lResult = m_Rec.m_bCapRecPresent;
		break;

	case PIDXPtr_CapRecRight90:
		lResult = m_Rec.m_bCapRecRight90;
		break;

	case PIDXPtr_CapRecRotate180:
		lResult = m_Rec.m_bCapRecRotate180;
		break;

	case PIDXPtr_CapRecStamp:
		lResult = m_Rec.m_bCapRecStamp;
		break;

	case PIDXPtr_CapRecUnderline:
		lResult = m_Rec.m_bCapRecUnderline;
		break;

	case PIDXPtr_CapSlp2Color:
		lResult = m_Slp.m_bCapSlp2Color;
		break;

	case PIDXPtr_CapSlpBarCode:
		lResult = m_Slp.m_bCapSlpBarCode;
		break;

	case PIDXPtr_CapSlpBitmap:
		lResult = m_Slp.m_bCapSlpBitmap;
		break;

	case PIDXPtr_CapSlpBold:
		lResult = m_Slp.m_bCapSlpBold;
		break;

	case PIDXPtr_CapSlpDhigh:
		lResult = m_Slp.m_bCapSlpDhigh;
		break;

	case PIDXPtr_CapSlpDwide:
		lResult = m_Slp.m_bCapSlpDwide;
		break;

	case PIDXPtr_CapSlpDwideDhigh:
		lResult = m_Slp.m_bCapSlpDwideDhigh;
		break;

	case PIDXPtr_CapSlpEmptySensor:
		lResult = m_Slp.m_bCapSlpEmptySensor;
		break;

	case PIDXPtr_CapSlpFullslip:
		lResult = m_Slp.m_bCapSlpFullslip;
		break;

	case PIDXPtr_CapSlpItalic:
		lResult = m_Slp.m_bCapSlpItalic;
		break;

	case PIDXPtr_CapSlpLeft90:
		lResult = m_Slp.m_bCapSlpLeft90;
		break;

	case PIDXPtr_CapSlpNearEndSensor:
		lResult = m_Slp.m_bCapSlpNearEndSensor;
		break;

	case PIDXPtr_CapSlpPresent:
		lResult = m_Slp.m_bCapSlpPresent;
		break;

	case PIDXPtr_CapSlpRight90:
		lResult = m_Slp.m_bCapSlpRight90;
		break;

	case PIDXPtr_CapSlpRotate180:
		lResult = m_Slp.m_bCapSlpRotate180;
		break;

	case PIDXPtr_CapSlpUnderline:
		lResult = m_Slp.m_bCapSlpUnderline;
		break;

	case PIDXPtr_CapCharacterSet:
		lResult = m_Cmn.m_lCapCharacterSet;
		break;

	case PIDXPtr_CapTransaction:
		lResult = m_Cmn.m_bCapTransaction;
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
void CPrinterProperties::SetPropertyNumber( long lPropIndex, long lProperty )
{
	switch( lPropIndex )
	{
	case PIDXPtr_AsyncMode:
		m_Cmn.m_bAsyncMode = lProperty;
		break;

	case PIDXPtr_CharacterSet:
		if ( ((lProperty > 100) && (lProperty < 200)) ||
			 ((lProperty >= 400) && (lProperty <= 990)) ||
			 (lProperty == PTR_CS_ASCII) ||
			 (lProperty == PTR_CS_WINDOWS) ||
			 (lProperty >= 1000) )
		{
			m_Cmn.m_lCharacterSet = lProperty;
		}
		break;

	case PIDXPtr_FlagWhenIdle:
		m_Cmn.m_bFlagWhenIdle = lProperty;
		break;

	case PIDXPtr_JrnLetterQuality:
		m_Jrn.m_bJrnLetterQuality = lProperty;
		break;

	case PIDXPtr_RecLetterQuality:
		m_Rec.m_bRecLetterQuality = lProperty;
		break;

	case PIDXPtr_SlpLetterQuality:
		m_Slp.m_bSlpLetterQuality = lProperty;
		break;

	case PIDXPtr_RotateSpecial:
		if ( (lProperty == PTR_RP_NORMAL) ||
			 (lProperty == PTR_RP_RIGHT90) ||
			 (lProperty == PTR_RP_LEFT90) ||
			 (lProperty == PTR_RP_ROTATE180) )
		{
			m_Cmn.m_lRotateSpecial = lProperty;
		}
		break;

	// NOTE:  Changes to the following can modify several properties based
	//        on the Printer Metrics
	case PIDXPtr_MapMode:
		if ( ((lProperty == PTR_MM_DOTS) ||
			 (lProperty == PTR_MM_TWIPS) ||
			 (lProperty == PTR_MM_ENGLISH) ||
			 (lProperty == PTR_MM_METRIC)) &&
			 (lProperty != m_Cmn.m_lMapMode) )
		{
			m_Cmn.m_lMapMode = lProperty;
			UpdateJrnMetrics();
			UpdateRecMetrics();
			UpdateSlpMetrics();
		}
		break;

	case PIDXPtr_JrnLineHeight:
	case PIDXPtr_RecLineHeight:
	case PIDXPtr_SlpLineHeight:
		//Ignore since no known printers support setting the line height
		break;

	case PIDXPtr_JrnLineChars:
		if ( (lProperty > 0) && m_ppJrnMetrics )
		{	
			PrinterMetrics**	ppMetric;
			long				lMetric = 0;

			for(ppMetric=m_ppJrnMetrics; *ppMetric!=NULL; ppMetric++) 
			{
				if ( (*ppMetric)->m_nLineChars >= lProperty )
				{
					if( lMetric != m_lJrnActiveMetric )
					{
						m_Jrn.m_lJrnLineChars = lProperty;
						m_lJrnActiveMetric = lMetric;
						UpdateJrnMetrics();
						break;
					}
				}
				lMetric++;
			}
		}
		break;

	case PIDXPtr_JrnLineSpacing:
		if ( m_ppJrnMetrics )
		{
			long nSpacing;
			float fConvert;

			fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotHigh;
			nSpacing = ConvertFromMapMode( fConvert, lProperty );
			nSpacing -= m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotLineHeight;

			if ( nSpacing < 0 )
			{
				nSpacing = 0;
			}

			if ( nSpacing > m_ppJrnMetrics[m_lJrnActiveMetric]->m_nMaxDotLineWhitespace )
			{
				nSpacing = m_ppJrnMetrics[m_lJrnActiveMetric]->m_nMaxDotLineWhitespace;
			}

			m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotLineWhitespace = nSpacing;
			UpdateJrnMetrics();
		}
		break;

	case PIDXPtr_RecLineChars:
		if ( (lProperty > 0) && m_ppRecMetrics )
		{	
			PrinterMetrics**	ppMetric;
			long				lMetric = 0;

			for(ppMetric=m_ppRecMetrics; *ppMetric!=NULL; ppMetric++) 
			{
				if ( (*ppMetric)->m_nLineChars >= lProperty )
				{
					if( lMetric != m_lRecActiveMetric )
					{
						m_Rec.m_lRecLineChars = lProperty;
						m_lRecActiveMetric = lMetric;
						UpdateRecMetrics();
						break;
					}
				}
				lMetric++;
			}
		}
		break;

	case PIDXPtr_RecLineSpacing:
		if ( m_ppRecMetrics )
		{
			long nSpacing;
			float fConvert;

			fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotHigh;
			nSpacing = ConvertFromMapMode( fConvert, lProperty );
			nSpacing -= m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineHeight;

			if ( nSpacing < 0 )
			{
				nSpacing = 0;
			}

			if ( nSpacing > m_ppRecMetrics[m_lRecActiveMetric]->m_nMaxDotLineWhitespace )
			{
				nSpacing = m_ppRecMetrics[m_lRecActiveMetric]->m_nMaxDotLineWhitespace;
			}

			m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineWhitespace = nSpacing;
			UpdateRecMetrics();
		}
		break;

	case PIDXPtr_SlpLineChars:
		if ( (lProperty > 0) && m_ppSlpMetrics )
		{	
			PrinterMetrics**	ppMetric;
			long				lMetric = 0;

			for(ppMetric=m_ppSlpMetrics; *ppMetric!=NULL; ppMetric++) 
			{
				if ( (*ppMetric)->m_nLineChars >= lProperty )
				{
					if( lMetric != m_lSlpActiveMetric )
					{
						m_Slp.m_lSlpLineChars = lProperty;
						m_lSlpActiveMetric = lMetric;
						UpdateSlpMetrics();
						break;
					}
				}
				lMetric++;
			}
		}
		break;

	case PIDXPtr_SlpLineSpacing:
		if ( m_ppSlpMetrics )
		{
			long nSpacing;
			float fConvert;

			fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotHigh;
			nSpacing = ConvertFromMapMode( fConvert, lProperty );
			nSpacing -= m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineHeight;

			if ( nSpacing < 0 )
			{
				nSpacing = 0;
			}

			if ( nSpacing > m_ppSlpMetrics[m_lSlpActiveMetric]->m_nMaxDotLineWhitespace )
			{
				nSpacing = m_ppSlpMetrics[m_lSlpActiveMetric]->m_nMaxDotLineWhitespace;
			}

			m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineWhitespace = nSpacing;
			UpdateSlpMetrics();
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
CString CPrinterProperties::GetPropertyString( long lPropIndex )
{
	CString sRet;
	switch( lPropIndex )
	{
	case PIDXPtr_CharacterSetList:
		sRet = m_sCharacterSetList;
		break;

	case PIDXPtr_JrnLineCharsList:
		sRet = m_sJrnLineCharsList;
		break;

	case PIDXPtr_RecLineCharsList:
		sRet = m_sRecLineCharsList;
		break;

	case PIDXPtr_SlpLineCharsList:
		sRet = m_sSlpLineCharsList;
		break;

	case PIDXPtr_ErrorString:
		sRet = m_sErrorString;
		break;

	case PIDXPtr_FontTypefaceList:
		sRet = m_sFontTypefaceList;
		break;

	case PIDXPtr_RecBarCodeRotationList:
		sRet = m_sRecBarCodeRotationList;
		break;

	case PIDXPtr_SlpBarCodeRotationList:
		sRet = m_sSlpBarCodeRotationList;
		break;

	default:
		break;
	}
	return sRet;
}

//
// Function:  UpdateJrnMetrics
// Purpose:  Update the metrics based properties of the journal print
//           station.
// Inputs:  None
// Outputs:  None
// Assumptions:  This is loosely based on the OPOS Printer SO.  It should
//               cover most printer requirements; however, additional work
//               may be required for some printers.
// Comments:  None
// 
void CPrinterProperties::UpdateJrnMetrics( )
{
	long lValue;
	float fConvert;
	
	if ( m_ppJrnMetrics == NULL )
	{
		return;
	}

	// LineChars is set by SetPropertyNumber

	// LineHeight
	fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotLineHeight );
	m_Jrn.m_lJrnLineHeight = lValue;

	// LineSpacing
	fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, 
		m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotLineHeight +
		m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotLineWhitespace);
	m_Jrn.m_lJrnLineHeight = lValue;

	// LineWidth
	fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotWide;
	lValue = ConvertToMapMode( fConvert, m_Jrn.m_lJrnLineChars *
		m_ppJrnMetrics[m_lJrnActiveMetric]->m_nDotCharWidth );
	m_Jrn.m_lJrnLineHeight = lValue;

}

//
// Function:  UpdateRecMetrics
// Purpose:  Update the metrics based properties of the receipt print
//           station.
// Inputs:  None
// Outputs:  None
// Assumptions:  This is loosely based on the OPOS Printer SO.  It should
//               cover most printer requirements; however, additional work
//               may be required for some printers.
// Comments:  None
// 
void CPrinterProperties::UpdateRecMetrics( )
{
	long lValue;
	float fConvert;
	
	if ( m_ppRecMetrics == NULL )
	{
		return;
	}

	// LineChars is set by SetPropertyNumber

	// LineHeight
	fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineHeight );
	m_Rec.m_lRecLineHeight = lValue;

	// LineSpacing
	fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, 
		m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineHeight +
		m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineWhitespace);
	m_Rec.m_lRecLineHeight = lValue;

	// LineWidth
	fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotWide;
	lValue = ConvertToMapMode( fConvert, m_Rec.m_lRecLineChars *
		m_ppRecMetrics[m_lRecActiveMetric]->m_nDotCharWidth );
	m_Rec.m_lRecLineHeight = lValue;

	double dDotsPerLine = m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineHeight +
		m_ppRecMetrics[m_lRecActiveMetric]->m_nDotLineWhitespace;
	
	// SidewaysMaxLines
	m_Rec.m_lRecSidewaysMaxLines = (long)(m_Rec.m_lRecLineChars *
		m_ppRecMetrics[m_lRecActiveMetric]->m_fSidewaysLineRatio);

	// SidewaysMaxChars does not change

	// LinesToPaperCut
    long nDotsToPaperCutTear = m_Rec.m_bCapRecPapercut
		? m_ppRecMetrics[m_lRecActiveMetric]->m_nDotsToPaperCut
        : m_ppRecMetrics[m_lRecActiveMetric]->m_nDotsToPaperTear;
    m_Rec.m_lRecLinesToPaperCut = (long)
        (( nDotsToPaperCutTear + dDotsPerLine - 1 ) / dDotsPerLine );

}

//
// Function:  UpdateSlpMetrics
// Purpose:  Update the metrics based properties of the slip print
//           station.
// Inputs:  None
// Outputs:  None
// Assumptions:  This is loosely based on the OPOS Printer SO.  It should
//               cover most printer requirements; however, additional work
//               may be required for some printers.
// Comments:  None
// 
void CPrinterProperties::UpdateSlpMetrics( )
{
	long lValue;
	float fConvert;
	
	if ( m_ppSlpMetrics == NULL )
	{
		return;
	}

	// LineChars is set by SetPropertyNumber

	// LineHeight
	fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineHeight );
	m_Slp.m_lSlpLineHeight = lValue;

	// LineSpacing
	fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotHigh;
	lValue = ConvertToMapMode( fConvert, 
		m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineHeight +
		m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineWhitespace);
	m_Slp.m_lSlpLineHeight = lValue;

	// LineWidth
	fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotWide;
	lValue = ConvertToMapMode( fConvert, m_Slp.m_lSlpLineChars *
		m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotCharWidth );
	m_Slp.m_lSlpLineHeight = lValue;

	double dDotsPerLine = m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineHeight +
		m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotLineWhitespace;
	
	// SidewaysMaxLines
	m_Slp.m_lSlpSidewaysMaxLines = (long)(m_Slp.m_lSlpLineChars *
		m_ppSlpMetrics[m_lSlpActiveMetric]->m_fSidewaysLineRatio);

	// SidewaysMaxChars does not change

	// LinesNearEndToEnd
	m_Slp.m_lSlpLinesNearEndToEnd = (long)
        ( ( m_ppSlpMetrics[m_lSlpActiveMetric]->m_nDotsNearEndToEnd + 
		      dDotsPerLine - 1 ) / dDotsPerLine );

}

//
// Function:  ConvertFromMapMode
// Purpose:  Converts a value expressed in MapMode units to the Dots format
//           that is used internally.
// Inputs:  fConvert - conversion factor based on the printer type/metrics
//          nUnits - the value to be converted.
// Outputs:  the converted value
// Assumptions:  None
// Comments:  None
//
long CPrinterProperties::ConvertFromMapMode( float fConvert, long nUnits )
{
	long lResult;

	switch( m_Cmn.m_lMapMode )
	{
	case PTR_MM_TWIPS:
		{
			double dMapModeUnitPerMM = 1440 / MMPERINCH;
			lResult = (long) ( ((nUnits/dMapModeUnitPerMM) / fConvert) + 0.5 );
		}
		break;

	case PTR_MM_ENGLISH:
		{
			double dMapModeUnitPerMM = 1000 / MMPERINCH;
			lResult = (long) ( ((nUnits/dMapModeUnitPerMM) / fConvert) + 0.5 );
		}
		break;

	case PTR_MM_METRIC:
		{
			double dMapModeUnitPerMM = 100.0;
			lResult = (long) ( ((nUnits/dMapModeUnitPerMM) / fConvert) + 0.5 );
		}
		break;

	case PTR_MM_DOTS:
	default:
		lResult = nUnits;
		break;
	}

	return lResult;
}

//
// Function:  ConvertToMapMode
// Purpose:  Converts a value from the internal Dots representation to the 
//           a number of units in the specified MapMode.
// Inputs:  fConvert - conversion factor based on the printer type/metrics
//          nDots - the value to be converted.
// Outputs:  the converted value
// Assumptions:  None
// Comments:  None
//
long CPrinterProperties::ConvertToMapMode( float fConvert, long nDots )
{
	long lResult;

	switch( m_Cmn.m_lMapMode )
	{
	case PTR_MM_TWIPS:
		{
			double dMapModeUnitPerMM = 1440 / MMPERINCH;
			lResult = (long) ( (dMapModeUnitPerMM * fConvert * nDots) + 0.5 );
		}
		break;

	case PTR_MM_ENGLISH:
		{
			double dMapModeUnitPerMM = 1000 / MMPERINCH;
			lResult = (long) ( (dMapModeUnitPerMM * fConvert * nDots) + 0.5 );
		}
		break;

	case PTR_MM_METRIC:
		{
			double dMapModeUnitPerMM = 100.0;
			lResult = (long) ( (dMapModeUnitPerMM * fConvert * nDots) + 0.5 );
		}
		break;

	case PTR_MM_DOTS:
	default:
		lResult = nDots;
		break;
	}

	return lResult;
}


//
// Fucntion:  ConvertHeight
// Purpose:  Determines appropriate converstion factor and calls the convert
//           function
// Inputs:  Station - The printer station for the value to be converted.
//          nUnits - the value to be converted.
// Outputs:  the converted value
// Assumptions:  None
// Comments:  None
//
long CPrinterProperties::ConvertHeight( long Station, long nUnits )
{
	float fConvert = 1;

    switch(Station)
    {
    case PTR_S_RECEIPT:
		if (m_ppRecMetrics)
			fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotHigh;
        break;

    case PTR_S_JOURNAL:
		if (m_ppJrnMetrics)
			fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotHigh;
        break;

    case PTR_S_SLIP:
		if (m_ppSlpMetrics)
			fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotHigh;
        break;
    }

	return ConvertFromMapMode( fConvert, nUnits );
}

//
// Fucntion:  ConvertWidth
// Purpose:  Determines appropriate converstion factor and calls the convert
//           function
// Inputs:  Station - The printer station for the value to be converted.
//          nUnits - the value to be converted.
// Outputs:  the converted value
// Assumptions:  None
// Comments:  None
//
long CPrinterProperties::ConvertWidth( long Station, long nUnits )
{
    float fConvert = 1;

    switch(Station)
    {
    case PTR_S_RECEIPT:
		if (m_ppRecMetrics)
		    fConvert = m_ppRecMetrics[m_lRecActiveMetric]->m_fMMPerDotWide;
        break;

    case PTR_S_JOURNAL:
		if (m_ppJrnMetrics)
		    fConvert = m_ppJrnMetrics[m_lJrnActiveMetric]->m_fMMPerDotWide;
        break;

    case PTR_S_SLIP:
		if (m_ppSlpMetrics)
			fConvert = m_ppSlpMetrics[m_lSlpActiveMetric]->m_fMMPerDotWide;
        break;
    }

	return ConvertFromMapMode( fConvert, nUnits );
}


void CPrinterProperties::SlipInserted()
{
	if ( m_bInitialized )
	{
		m_lSlpLastEmptyStatus = SLP_EMPTY_UNKNOWN;
		this->m_Slp.m_bSlpEmpty = FALSE;
	}
}

void CPrinterProperties::SlipRemoved()
{
	if ( m_bInitialized )
	{
		m_lSlpLastEmptyStatus = SLP_EMPTY_UNKNOWN;
		this->m_Slp.m_bSlpEmpty = TRUE;
	}
}


void CPrinterProperties::Override( COposRegistry& oReg, CmDataCapture* dCap )
{
   CDataCapEntryExit DCE( *dCap, _T("CPrinterProperties::OverridePOSPrinter") );
	CMapStringToPtr myProp;

	InitializePropertyMap( myProp );

	// Process Overrides
	CString csKey;
	CString csValue;
	DWORD dwIndex = 0;
	long* lProp;
	long lVal = ERROR_SUCCESS;

	for( dwIndex=0; lVal == ERROR_SUCCESS; dwIndex++ )
	{
		lVal = oReg.RegEnumValue( dwIndex, csKey, csValue );
		if ( lVal == ERROR_SUCCESS )
		{
			if ( myProp.Lookup( csKey, (void*&)lProp ) )
			{
				dCap->DCPrintf( 0x01, _T("Override %s(%d) with %s"), csKey, *lProp, csValue ); 
				OverrideProperty( *lProp, csValue );
			}
			else
			{
				dCap->DCPrintf( 0x01, _T("Override - Unknown property %s"), csKey ); 
			}
		}
		else
		{
			dCap->DCPrintf( 0x01, _T("Override Terminated %d"), lVal ); 
		}

	}


	EmptyPropertyMap( myProp );
	return;
}

void CPrinterProperties::OverrideProperty( long lProp, CString& csVal)
{
	long lVal = _ttol( csVal );

	switch( lProp )
	{
	case PIDXPtr_AsyncMode:
		m_Cmn.m_bAsyncMode = lVal;
		break;

	case PIDXPtr_CharacterSet:
		m_Cmn.m_lCharacterSet = lVal;
		break;

	case PIDXPtr_CoverOpen:
		if ( lVal )
		{
			m_Cmn.m_bCoverOpen = true;
		}
		else
		{
			m_Cmn.m_bCoverOpen = false;
		}
		break;

	case PIDXPtr_ErrorStation:
		m_Cmn.m_lErrorStation = lVal;
		break;

	case PIDXPtr_FlagWhenIdle:
		m_Cmn.m_bFlagWhenIdle = lVal;
		break;

	case PIDXPtr_JrnEmpty:
		m_Jrn.m_bJrnEmpty = lVal;
		break;

	case PIDXPtr_JrnLetterQuality:
		m_Jrn.m_bJrnLetterQuality = lVal;
		break;

	case PIDXPtr_JrnLineChars:
		m_Jrn.m_lJrnLineChars = lVal;
		break;

	case PIDXPtr_JrnLineHeight:
		m_Jrn.m_lJrnLineHeight = lVal;
		break;

	case PIDXPtr_JrnLineSpacing:
		m_Jrn.m_lJrnLineSpacing = lVal;
		break;

	case PIDXPtr_JrnLineWidth:
		m_Jrn.m_lJrnLineWidth = lVal;
		break;

	case PIDXPtr_JrnNearEnd:
		m_Jrn.m_bJrnNearEnd = lVal;
		break;

	case PIDXPtr_MapMode:
		m_Cmn.m_lMapMode = lVal;
		break;

	case PIDXPtr_RecEmpty:
		m_Rec.m_bRecEmpty = lVal;
		break;

	case PIDXPtr_RecLetterQuality:
		m_Rec.m_bRecLetterQuality = lVal;
		break;

	case PIDXPtr_RecLineChars:
		m_Rec.m_lRecLineChars = lVal;
		break;

	case PIDXPtr_RecLineHeight:
		m_Rec.m_lRecLineHeight = lVal;
		break;

	case PIDXPtr_RecLineSpacing:
		m_Rec.m_lRecLineSpacing = lVal;
		break;

	case PIDXPtr_RecLinesToPaperCut:
		m_Rec.m_lRecLinesToPaperCut = lVal;
		break;

	case PIDXPtr_RecLineWidth:
		m_Rec.m_lRecLineWidth = lVal;
		break;

	case PIDXPtr_RecNearEnd:
		m_Rec.m_bRecNearEnd = lVal;
		break;

	case PIDXPtr_RecSidewaysMaxChars:
		m_Rec.m_lRecSidewaysMaxChars = lVal;
		break;

	case PIDXPtr_RecSidewaysMaxLines:
		m_Rec.m_lRecSidewaysMaxLines = lVal;
		break;

	case PIDXPtr_SlpEmpty:
		m_Slp.m_bSlpEmpty = lVal;
		break;

	case PIDXPtr_SlpLetterQuality:
		m_Slp.m_bSlpLetterQuality = lVal;
		break;

	case PIDXPtr_SlpLineChars:
		m_Slp.m_lSlpLineChars = lVal;
		break;

	case PIDXPtr_SlpLineHeight:
		m_Slp.m_lSlpLineHeight = lVal;
		break;

	case PIDXPtr_SlpLinesNearEndToEnd:
		m_Slp.m_lSlpLinesNearEndToEnd = lVal;
		break;

	case PIDXPtr_SlpLineSpacing:
		m_Slp.m_lSlpLineSpacing = lVal;
		break;

	case PIDXPtr_SlpLineWidth:
		m_Slp.m_lSlpLineWidth = lVal;
		break;

	case PIDXPtr_SlpMaxLines:
		m_Slp.m_lSlpMaxLines = lVal;
		break;

	case PIDXPtr_SlpNearEnd:
		m_Slp.m_bSlpNearEnd = lVal;
		break;

	case PIDXPtr_SlpSidewaysMaxChars:
		m_Slp.m_lSlpSidewaysMaxChars = lVal;
		break;

	case PIDXPtr_SlpSidewaysMaxLines:
		m_Slp.m_lSlpSidewaysMaxLines = lVal;
		break;

	case PIDXPtr_ErrorLevel:
		m_Cmn.m_lErrorLevel = lVal;
		break;

	case PIDXPtr_RotateSpecial:
		m_Cmn.m_lRotateSpecial = lVal;
		break;

	case PIDXPtr_CapConcurrentJrnRec:
		m_Cmn.m_bCapConcurrentJrnRec = lVal;
		break;

	case PIDXPtr_CapConcurrentJrnSlp:
		m_Cmn.m_bCapConcurrentJrnSlp = lVal;
		break;

	case PIDXPtr_CapConcurrentRecSlp:
		m_Cmn.m_bCapConcurrentRecSlp = lVal;
		break;

	case PIDXPtr_CapCoverSensor:
		m_Cmn.m_bCapCoverSensor = lVal;
		break;

	case PIDXPtr_CapJrn2Color:
		m_Jrn.m_bCapJrn2Color = lVal;
		break;

	case PIDXPtr_CapJrnBold:
		m_Jrn.m_bCapJrnBold = lVal;
		break;

	case PIDXPtr_CapJrnDhigh:
		m_Jrn.m_bCapJrnDhigh = lVal;
		break;

	case PIDXPtr_CapJrnDwide:
		m_Jrn.m_bCapJrnDwide = lVal;
		break;

	case PIDXPtr_CapJrnDwideDhigh:
		m_Jrn.m_bCapJrnDwideDhigh = lVal;
		break;

	case PIDXPtr_CapJrnEmptySensor:
		m_Jrn.m_bCapJrnEmptySensor = lVal;
		break;

	case PIDXPtr_CapJrnItalic:
		m_Jrn.m_bCapJrnItalic = lVal;
		break;

	case PIDXPtr_CapJrnNearEndSensor:
		m_Jrn.m_bCapJrnNearEndSensor = lVal;
		break;

	case PIDXPtr_CapJrnPresent:
		m_Jrn.m_bCapJrnPresent = lVal;
		break;

	case PIDXPtr_CapJrnUnderline:
		m_Jrn.m_bCapJrnUnderline = lVal;
		break;

	case PIDXPtr_CapRec2Color:
		m_Rec.m_bCapRec2Color = lVal;
		break;

	case PIDXPtr_CapRecBarCode:
		m_Rec.m_bCapRecBarCode = lVal;
		break;

	case PIDXPtr_CapRecBitmap:
		m_Rec.m_bCapRecBitmap = lVal;
		break;

	case PIDXPtr_CapRecBold:
		m_Rec.m_bCapRecBold = lVal;
		break;

	case PIDXPtr_CapRecDhigh:
		m_Rec.m_bCapRecDhigh = lVal;
		break;

	case PIDXPtr_CapRecDwide:
		m_Rec.m_bCapRecDwide = lVal;
		break;

	case PIDXPtr_CapRecDwideDhigh:
		m_Rec.m_bCapRecDwideDhigh = lVal;
		break;

	case PIDXPtr_CapRecEmptySensor:
		m_Rec.m_bCapRecEmptySensor = lVal;
		break;

	case PIDXPtr_CapRecItalic:
		m_Rec.m_bCapRecItalic = lVal;
		break;

	case PIDXPtr_CapRecLeft90:
		m_Rec.m_bCapRecLeft90 = lVal;
		break;

	case PIDXPtr_CapRecNearEndSensor:
		m_Rec.m_bCapRecNearEndSensor = lVal;
		break;

	case PIDXPtr_CapRecPapercut:
		m_Rec.m_bCapRecPapercut = lVal;
		break;

	case PIDXPtr_CapRecPresent:
		m_Rec.m_bCapRecPresent = lVal;
		break;

	case PIDXPtr_CapRecRight90:
		m_Rec.m_bCapRecRight90 = lVal;
		break;

	case PIDXPtr_CapRecRotate180:
		m_Rec.m_bCapRecRotate180 = lVal;
		break;

	case PIDXPtr_CapRecStamp:
		m_Rec.m_bCapRecStamp = lVal;
		break;

	case PIDXPtr_CapRecUnderline:
		m_Rec.m_bCapRecUnderline = lVal;
		break;

	case PIDXPtr_CapSlp2Color:
		m_Slp.m_bCapSlp2Color = lVal;
		break;

	case PIDXPtr_CapSlpBarCode:
		m_Slp.m_bCapSlpBarCode = lVal;
		break;

	case PIDXPtr_CapSlpBitmap:
		m_Slp.m_bCapSlpBitmap = lVal;
		break;

	case PIDXPtr_CapSlpBold:
		m_Slp.m_bCapSlpBold = lVal;
		break;

	case PIDXPtr_CapSlpDhigh:
		m_Slp.m_bCapSlpDhigh = lVal;
		break;

	case PIDXPtr_CapSlpDwide:
		m_Slp.m_bCapSlpDwide = lVal;
		break;

	case PIDXPtr_CapSlpDwideDhigh:
		m_Slp.m_bCapSlpDwideDhigh = lVal;
		break;

	case PIDXPtr_CapSlpEmptySensor:
		m_Slp.m_bCapSlpEmptySensor = lVal;
		break;

	case PIDXPtr_CapSlpFullslip:
		m_Slp.m_bCapSlpFullslip = lVal;
		break;

	case PIDXPtr_CapSlpItalic:
		m_Slp.m_bCapSlpItalic = lVal;
		break;

	case PIDXPtr_CapSlpLeft90:
		m_Slp.m_bCapSlpLeft90 = lVal;
		break;

	case PIDXPtr_CapSlpNearEndSensor:
		m_Slp.m_bCapSlpNearEndSensor = lVal;
		break;

	case PIDXPtr_CapSlpPresent:
		m_Slp.m_bCapSlpPresent = lVal;
		break;

	case PIDXPtr_CapSlpRight90:
		m_Slp.m_bCapSlpLeft90 = lVal;
		break;

	case PIDXPtr_CapSlpRotate180:
		m_Slp.m_bCapSlpLeft90 = lVal;
		break;

	case PIDXPtr_CapSlpUnderline:
		m_Slp.m_bCapSlpUnderline = lVal;
		break;

	case PIDXPtr_CapCharacterSet:
		m_Cmn.m_lCapCharacterSet = lVal;
		break;

	case PIDXPtr_CapTransaction:
		m_Cmn.m_bCapTransaction = lVal;
		break;

	case PIDXPtr_CharacterSetList:
		m_sCharacterSetList = csVal;
		break;

	case PIDXPtr_JrnLineCharsList:
		m_sJrnLineCharsList = csVal;
		break;

	case PIDXPtr_RecLineCharsList:
		m_sRecLineCharsList = csVal;
		break;

	case PIDXPtr_SlpLineCharsList:
		m_sSlpLineCharsList = csVal;
		break;

	case PIDXPtr_ErrorString:
		m_sErrorString = csVal;
		break;

	case PIDXPtr_FontTypefaceList:
		m_sFontTypefaceList = csVal;
		break;

	case PIDXPtr_RecBarCodeRotationList:
		m_sRecBarCodeRotationList = csVal;
		break;

	case PIDXPtr_SlpBarCodeRotationList:
		m_sSlpBarCodeRotationList = csVal;
		
	default:
		// Do nothing
		break;
	}
}


void CPrinterProperties::InitializePropertyMap( CMapStringToPtr& oMap )
{
	oMap.SetAt(_T("OE_AsyncMode"), new long(PIDXPtr_AsyncMode));
	oMap.SetAt(_T("OE_CharacterSet"), new long(PIDXPtr_CharacterSet));
	oMap.SetAt(_T("OE_CoverOpen"), new long(PIDXPtr_CoverOpen));
	oMap.SetAt(_T("OE_ErrorStation"), new long(PIDXPtr_ErrorStation));
	oMap.SetAt(_T("OE_FlagWhenIdle"), new long(PIDXPtr_FlagWhenIdle));
	oMap.SetAt(_T("OE_JrnEmpty"), new long(PIDXPtr_JrnEmpty));
	oMap.SetAt(_T("OE_JrnLetterQuality"), new long(PIDXPtr_JrnLetterQuality));
	oMap.SetAt(_T("OE_JrnLineChars"), new long(PIDXPtr_JrnLineChars));
	oMap.SetAt(_T("OE_JrnLineHeight"), new long(PIDXPtr_JrnLineHeight));
	oMap.SetAt(_T("OE_JrnLineSpacing"), new long(PIDXPtr_JrnLineSpacing));
	oMap.SetAt(_T("OE_JrnLineWidth"), new long(PIDXPtr_JrnLineWidth));
	oMap.SetAt(_T("OE_JrnNearEnd"), new long(PIDXPtr_JrnNearEnd));
	oMap.SetAt(_T("OE_MapMode"), new long(PIDXPtr_MapMode));
	oMap.SetAt(_T("OE_RecEmpty"), new long(PIDXPtr_RecEmpty));
	oMap.SetAt(_T("OE_RecLetterQuality"), new long(PIDXPtr_RecLetterQuality));
	oMap.SetAt(_T("OE_RecLineChars"), new long(PIDXPtr_RecLineChars));
	oMap.SetAt(_T("OE_RecLineHeight"), new long(PIDXPtr_RecLineHeight));
	oMap.SetAt(_T("OE_RecLineSpacing"), new long(PIDXPtr_RecLineSpacing));
	oMap.SetAt(_T("OE_RecLinesToPaperCut"), new long(PIDXPtr_RecLinesToPaperCut));
	oMap.SetAt(_T("OE_RecLineWidth"), new long(PIDXPtr_RecLineWidth));
	oMap.SetAt(_T("OE_RecNearEnd"), new long(PIDXPtr_RecNearEnd));
	oMap.SetAt(_T("OE_RecSidewaysMaxChars"), new long(PIDXPtr_RecSidewaysMaxChars));
	oMap.SetAt(_T("OE_RecSidewaysMaxLines"), new long(PIDXPtr_RecSidewaysMaxLines));
	oMap.SetAt(_T("OE_SlpEmpty"), new long(PIDXPtr_SlpEmpty));
	oMap.SetAt(_T("OE_SlpLetterQuality"), new long(PIDXPtr_SlpLetterQuality));
	oMap.SetAt(_T("OE_SlpLineChars"), new long(PIDXPtr_SlpLineChars));
	oMap.SetAt(_T("OE_SlpLineHeight"), new long(PIDXPtr_SlpLineHeight));
	oMap.SetAt(_T("OE_SlpLinesNearEndToEnd"), new long(PIDXPtr_SlpLinesNearEndToEnd));
	oMap.SetAt(_T("OE_SlpLineSpacing"), new long(PIDXPtr_SlpLineSpacing));
	oMap.SetAt(_T("OE_SlpLineWidth"), new long(PIDXPtr_SlpLineWidth));
	oMap.SetAt(_T("OE_SlpMaxLines"), new long(PIDXPtr_SlpMaxLines));
	oMap.SetAt(_T("OE_SlpNearEnd"), new long(PIDXPtr_SlpNearEnd));
	oMap.SetAt(_T("OE_SlpSidewaysMaxChars"), new long(PIDXPtr_SlpSidewaysMaxChars));
	oMap.SetAt(_T("OE_SlpSidewaysMaxLines"), new long(PIDXPtr_SlpSidewaysMaxLines));
	oMap.SetAt(_T("OE_ErrorLevel"), new long(PIDXPtr_ErrorLevel));
	oMap.SetAt(_T("OE_RotateSpecial"), new long(PIDXPtr_RotateSpecial));
	oMap.SetAt(_T("OE_CapConcurrentJrnRec"), new long(PIDXPtr_CapConcurrentJrnRec));
	oMap.SetAt(_T("OE_CapConcurrentJrnSlp"), new long(PIDXPtr_CapConcurrentJrnSlp));
	oMap.SetAt(_T("OE_CapConcurrentRecSlp"), new long(PIDXPtr_CapConcurrentRecSlp));
	oMap.SetAt(_T("OE_CapCoverSensor"), new long(PIDXPtr_CapCoverSensor));
	oMap.SetAt(_T("OE_CapJrn2Color"), new long(PIDXPtr_CapJrn2Color));
	oMap.SetAt(_T("OE_CapJrnBold"), new long(PIDXPtr_CapJrnBold));
	oMap.SetAt(_T("OE_CapJrnDhigh"), new long(PIDXPtr_CapJrnDhigh));
	oMap.SetAt(_T("OE_CapJrnDwide"), new long(PIDXPtr_CapJrnDwide));
	oMap.SetAt(_T("OE_CapJrnDwideDhigh"), new long(PIDXPtr_CapJrnDwideDhigh));
	oMap.SetAt(_T("OE_CapJrnEmptySensor"), new long(PIDXPtr_CapJrnEmptySensor));
	oMap.SetAt(_T("OE_CapJrnItalic"), new long(PIDXPtr_CapJrnItalic));
	oMap.SetAt(_T("OE_CapJrnNearEndSensor"), new long(PIDXPtr_CapJrnNearEndSensor));
	oMap.SetAt(_T("OE_CapJrnPresent"), new long(PIDXPtr_CapJrnPresent));
	oMap.SetAt(_T("OE_CapJrnUnderline"), new long(PIDXPtr_CapJrnUnderline));
	oMap.SetAt(_T("OE_CapRec2Color"), new long(PIDXPtr_CapRec2Color));
	oMap.SetAt(_T("OE_CapRecBarCode"), new long(PIDXPtr_CapRecBarCode));
	oMap.SetAt(_T("OE_CapRecBitmap"), new long(PIDXPtr_CapRecBitmap));
	oMap.SetAt(_T("OE_CapRecBold"), new long(PIDXPtr_CapRecBold));
	oMap.SetAt(_T("OE_CapRecDhigh"), new long(PIDXPtr_CapRecDhigh));
	oMap.SetAt(_T("OE_CapRecDwide"), new long(PIDXPtr_CapRecDwide));
	oMap.SetAt(_T("OE_CapRecDwideDhigh"), new long(PIDXPtr_CapRecDwideDhigh));
	oMap.SetAt(_T("OE_CapRecEmptySensor"), new long(PIDXPtr_CapRecEmptySensor));
	oMap.SetAt(_T("OE_CapRecItalic"), new long(PIDXPtr_CapRecItalic));
	oMap.SetAt(_T("OE_CapRecLeft90"), new long(PIDXPtr_CapRecLeft90));
	oMap.SetAt(_T("OE_CapRecNearEndSensor"), new long(PIDXPtr_CapRecNearEndSensor));
	oMap.SetAt(_T("OE_CapRecPapercut"), new long(PIDXPtr_CapRecPapercut));
	oMap.SetAt(_T("OE_CapRecPresent"), new long(PIDXPtr_CapRecPresent));
	oMap.SetAt(_T("OE_CapRecRight90"), new long(PIDXPtr_CapRecRight90));
	oMap.SetAt(_T("OE_CapRecRotate180"), new long(PIDXPtr_CapRecRotate180));
	oMap.SetAt(_T("OE_CapRecStamp"), new long(PIDXPtr_CapRecStamp));
	oMap.SetAt(_T("OE_CapRecUnderline"), new long(PIDXPtr_CapRecUnderline));
	oMap.SetAt(_T("OE_CapSlp2Color"), new long(PIDXPtr_CapSlp2Color));
	oMap.SetAt(_T("OE_CapSlpBarCode"), new long(PIDXPtr_CapSlpBarCode));
	oMap.SetAt(_T("OE_CapSlpBitmap"), new long(PIDXPtr_CapSlpBitmap));
	oMap.SetAt(_T("OE_CapSlpBold"), new long(PIDXPtr_CapSlpBold));
	oMap.SetAt(_T("OE_CapSlpDhigh"), new long(PIDXPtr_CapSlpDhigh));
	oMap.SetAt(_T("OE_CapSlpDwide"), new long(PIDXPtr_CapSlpDwide));
	oMap.SetAt(_T("OE_CapSlpDwideDhigh"), new long(PIDXPtr_CapSlpDwideDhigh));
	oMap.SetAt(_T("OE_CapSlpEmptySensor"), new long(PIDXPtr_CapSlpEmptySensor));
	oMap.SetAt(_T("OE_CapSlpFullslip"), new long(PIDXPtr_CapSlpFullslip));
	oMap.SetAt(_T("OE_CapSlpItalic"), new long(PIDXPtr_CapSlpItalic));
	oMap.SetAt(_T("OE_CapSlpLeft90"), new long(PIDXPtr_CapSlpLeft90));
	oMap.SetAt(_T("OE_CapSlpNearEndSensor"), new long(PIDXPtr_CapSlpNearEndSensor));
	oMap.SetAt(_T("OE_CapSlpPresent"), new long(PIDXPtr_CapSlpPresent));
	oMap.SetAt(_T("OE_CapSlpRight90"), new long(PIDXPtr_CapSlpRight90));
	oMap.SetAt(_T("OE_CapSlpRotate180"), new long(PIDXPtr_CapSlpRotate180));
	oMap.SetAt(_T("OE_CapSlpUnderline"), new long(PIDXPtr_CapSlpUnderline));
	oMap.SetAt(_T("OE_CapCharacterSet"), new long(PIDXPtr_CapCharacterSet));
	oMap.SetAt(_T("OE_CapTransaction"), new long(PIDXPtr_CapTransaction));
	oMap.SetAt(_T("OE_CharacterSetList"), new long(PIDXPtr_CharacterSetList));
	oMap.SetAt(_T("OE_JrnLineCharsList"), new long(PIDXPtr_JrnLineCharsList));
	oMap.SetAt(_T("OE_RecLineCharsList"), new long(PIDXPtr_RecLineCharsList));
	oMap.SetAt(_T("OE_SlpLineCharsList"), new long(PIDXPtr_SlpLineCharsList));
	oMap.SetAt(_T("OE_ErrorString"), new long(PIDXPtr_ErrorString));
	oMap.SetAt(_T("OE_FontTypefaceList"), new long(PIDXPtr_FontTypefaceList));
	oMap.SetAt(_T("OE_RecBarCodeRotationList"), new long(PIDXPtr_RecBarCodeRotationList));
	oMap.SetAt(_T("OE_SlpBarCodeRotationList"), new long(PIDXPtr_SlpBarCodeRotationList));
}

void CPrinterProperties::EmptyPropertyMap( CMapStringToPtr& oMap )
{
	POSITION pos;
	long* pLong;
	CString key;

	for( pos = oMap.GetStartPosition(); pos != NULL; )
	{
		oMap.GetNextAssoc( pos, key, (void*&)pLong);
		delete pLong;
	}

	oMap.RemoveAll();

}
