// LDWindow.cpp: implementation of the CLDWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LDWindow.h"
#include "string.h"
#include "OposDisp.h"
#include "Oposdisp.hi"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLDWindow::CLDWindow( long lRows, long lColumns, CmDataCapture& cmDc ) :
			m_lRows(lRows), m_lColumns(lColumns), m_lCursorRow(0),
			m_lCursorColumn(0), m_bCursorUpdate(TRUE), 
			m_lMarqueeType(DISP_MT_NONE), m_lMarqueeFormat(DISP_MF_WALK),
			m_lMarqueeUnitWait(0), m_lMarqueeRepeatWait(0),
			m_lInterCharacterWait(0), m_cmDc( &cmDc )
{
	CDataCapEntryExit DCE( cmDc, _T("CLDWindow::CLDWindow") );
	m_strData = new TCHAR[lRows*lColumns + 1];
	if ( m_strData )
	{
		ClearText();
	}
}

CLDWindow::~CLDWindow()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::~CLDWindow") );

	if ( m_strData )
	{
		delete[] m_strData;
	}
}

//
// Function:  ClearText
// Purpose:  Clears the window to blanks and sets the CursorRow and CursorColumn
//           to zero.
// Inputs:  None
// Outputs:  OPOS_SUCCESS - The method was successful.
// Assumptions:  Viewport, Marquee, and Teletype operations are not duplicated in
//               this SO since they really only affect the display of the device.
//               The SO reports the full contents of the individual windows defined
//               defined on the device.
// Comments:  None
//
long
CLDWindow::ClearText( )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::ClearText") );

	memset( m_strData, _T(' '), m_lRows*m_lColumns );
	m_strData[m_lRows*m_lColumns] = NULL;
	return OPOS_SUCCESS;
}

// 
// Function:  DisplayText
// Purpose:  Text is added to the display starting at the location specified by 
//           CursorRow and CursorColumn.  Text is added to the window according
//           to the rules defined in the OPOS Application Guide.
// Inputs:  strText - The text to be added to the window
// Outputs:  OPOS_SUCCESS - The method was successful.
// Assumptions: None
// Comments: None
//
long
CLDWindow::DisplayText( LPCTSTR strText )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::DisplayText") );

	return DisplayTextAt( m_lCursorRow, m_lCursorColumn, strText );
}

// 
// Function:  DisplayTextAt
// Purpose:  Text is added to the display starting at the specified location.
//           Text is added to the window according to the rules defined in the
//           OPOS Application Guide.
// Inputs:  Row - The start row for the text
//          Column - The start column for the text
//          strText - The text to be displayed
// Outputs:
//    OPOS_SUCCESS - The mehtod was successful.
//    OPOS_E_ILLEGAL - The arguments were incorrect.
// Assumptions:  None
// Comments:  None
//
long
CLDWindow::DisplayTextAt(long Row, long Column, LPCTSTR strText )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::DisplayTextAt") );

	DWORD	dwLength = _tcslen( strText );
	long	lCol = Column;
	long	lRow = Row;
	DWORD	dwChar;

	if ( (Row > m_lRows) || (Column > m_lColumns) )
	{
		return OPOS_E_ILLEGAL;
	}
	
	for( dwChar = 0; dwChar < dwLength; dwChar++ )
	{
		if ( strText[dwChar] == 10 )
		{
			m_cmDc->DCPrintf(TRACE_ALWAYS, _T("Found a '10' in display text."));

			lCol = 0;
			continue;
		}

		if ( strText[dwChar] == 13 )
		{
			m_cmDc->DCPrintf(TRACE_ALWAYS, _T("Found a '13' in display text."));
			lCol = 0;

			lRow++;
			if( lRow == m_lRows )
			{
				ScrollWindow();
				lRow--;
			}
			continue;
		}

		if ( lCol == m_lColumns )
		{
			m_cmDc->DCPrintf(TRACE_ALWAYS, _T("Reached End of Column in display text."));
			lCol = 0;
			lRow++;
			if( lRow == m_lRows )
			{
				ScrollWindow();
				lRow--;
			}
		}

		m_strData[(lRow*m_lColumns) + lCol] = strText[dwChar];
		lCol++;
	}
	
	if ( m_bCursorUpdate )
	{
		m_lCursorColumn = lCol;
		m_lCursorRow = lRow;
	}

	return OPOS_SUCCESS;
}

//
// Function:  GetPropertyNumber
// Purpose:  Get the value of one of the numeric window properties.
// Inputs:  lPropIndex - Identifier for the property to be returned.
// Outputs:  The value of the property.
// Assumptions:  None
// Comments:  None
//
long CLDWindow::GetPropertyNumber( long lPropIndex )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::GetPropertyNumber") );
	long lResult = 0;

	switch( lPropIndex )
	{
	case PIDXDisp_Columns:
		lResult = m_lColumns;
		break;

	case PIDXDisp_CursorColumn:
		lResult = m_lCursorColumn;
		break;

	case PIDXDisp_CursorRow:
		lResult = m_lCursorRow;
		break;

	case PIDXDisp_CursorUpdate:
		lResult = m_bCursorUpdate;
		break;

	case PIDXDisp_InterCharacterWait:
		lResult = m_lInterCharacterWait;
		break;

	case PIDXDisp_MarqueeRepeatWait:
		lResult = m_lMarqueeRepeatWait;
		break;

	case PIDXDisp_MarqueeType:
		lResult = m_lMarqueeType;
		break;

	case PIDXDisp_MarqueeUnitWait:
		lResult = m_lMarqueeUnitWait;
		break;

	case PIDXDisp_Rows:
		lResult = m_lRows;
		break;

	case PIDXDisp_MarqueeFormat:
		lResult = m_lMarqueeFormat;
		break;

	default:
		break;
	}

	return lResult;
}

//
// Function:  SetPropertyNumber
// Purpose:  Sets the value of one of the numeric window properties.
// Inputs:  lPropIndex - Identifier for the property to be set.
//          lProperty - Value of the property
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CLDWindow::SetPropertyNumber(long lPropIndex, long lProperty)
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::SetPropertyNumber") );
	switch( lPropIndex )
	{
	case PIDXDisp_Columns:
	case PIDXDisp_Rows:
		// Rows and Columns are read-only
		break;

	case PIDXDisp_CursorColumn:
		if ( lProperty < m_lColumns )
		{
			m_lCursorColumn = lProperty;
		}
		break;

	case PIDXDisp_CursorRow:
		if ( lProperty < m_lRows )
		{
			m_lCursorRow = lProperty;
		}
		break;

	case PIDXDisp_CursorUpdate:
		m_bCursorUpdate = lProperty;
		break;

	case PIDXDisp_InterCharacterWait:
		m_lInterCharacterWait = lProperty;
		break;

	case PIDXDisp_MarqueeRepeatWait:
		m_lMarqueeRepeatWait = lProperty;
		break;

	case PIDXDisp_MarqueeType:
		m_lMarqueeType = lProperty;
		break;

	case PIDXDisp_MarqueeUnitWait:
		m_lMarqueeUnitWait = lProperty;
		break;

	case PIDXDisp_MarqueeFormat:
		m_lMarqueeFormat = lProperty;
		break;

	default:
		break;
	}
}

// 
// Function:  Rows
// Purpose:  Returns the number of rows in the window.
// Inputs:  None
// Outputs:  The number of rows in the window
// Assumptions:  None
// Comments:  None
//
long CLDWindow::Rows( void )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::Rows") );
	return m_lRows;
}

// 
// Function:  Columns
// Purpose:  Returns the number of columnss in the window.
// Inputs:  None
// Outputs:  The number of columns in the window
// Assumptions:  None
// Comments:  None
//
long CLDWindow::Columns( void )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::Columns") );

	return m_lColumns;
}

// 
// Function:  Data
// Purpose:  Returns the text contained in the window
// Inputs:  None
// Outputs:  The text contained in the window
// Assumptions:  None
// Comments:  None
//
LPCTSTR CLDWindow::Data()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::Data") );
	return m_strData;
}

// 
// Function:  ScrollWindow
// Purpose:  Scrolls the text contained in the window based on the number of 
//           rows and columns using the rules defined in the OPOS Application
//           Guide.
// Inputs:  None
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CLDWindow::ScrollWindow()
{
	CDataCapEntryExit DCE( m_cmDc, _T("CLDWindow::ScrollWindow") );
	memmove( m_strData, &m_strData[m_lColumns], (m_lRows-1)*m_lColumns );
	memset( &m_strData[ (m_lRows-1)*m_lColumns], _T(' '), m_lColumns );
}