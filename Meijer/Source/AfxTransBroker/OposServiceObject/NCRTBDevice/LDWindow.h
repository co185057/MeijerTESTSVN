// LDWindow.h: interface for the CLDWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDWINDOW_H__1B1F8BE2_09CE_4C4D_B0DF_3C89D2BED41E__INCLUDED_)
#define AFX_LDWINDOW_H__1B1F8BE2_09CE_4C4D_B0DF_3C89D2BED41E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLDWindow  
{
public:
	CLDWindow( long lRows, long lColumns, CmDataCapture& cmDC );
	virtual ~CLDWindow();

	long ClearText( void );
	long DisplayText( LPCTSTR strText );
	long DisplayTextAt( long Row, long Column, LPCTSTR strTest );
	long GetPropertyNumber( long lPropIndex );
	void SetPropertyNumber(long lPropIndex, long lProperty);

	long Rows( void );
	long Columns( void );
	LPCTSTR Data();


private:
	void    ScrollWindow();

	long	m_lInterCharacterWait;
	long	m_lMarqueeType;
	long	m_lMarqueeFormat;
	long	m_lMarqueeUnitWait;
	long	m_lMarqueeRepeatWait;
	long	m_lCursorColumn;
	long	m_lCursorRow;
	long	m_lColumns;
	long	m_lRows;
	BOOL	m_bCursorUpdate;
	TCHAR*	m_strData;
	CmDataCapture*	m_cmDc;
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_LDWINDOW_H__1B1F8BE2_09CE_4C4D_B0DF_3C89D2BED41E__INCLUDED_)
