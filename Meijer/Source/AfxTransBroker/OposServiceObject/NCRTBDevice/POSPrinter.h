// POSPrinter.h: interface for the CPOSPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
#define AFX_POSPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"

class CPrinterBitmap
{
public:
	CString m_strFileName;
	long	m_lWidth;
	long	m_lAlignment;
	long	m_lStation;
	BYTE	m_BitmapNumber;
};

typedef CMap<int, int, CPrinterBitmap *, CPrinterBitmap *> CPrinterBitmapMap;

class CPOSPrinter : public CTBDevice  
{
public:
	CPOSPrinter(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CPOSPrinter();
	virtual CString GetDeviceModel( void ) const { return CString("POSPrinter");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );
    virtual long DirectIO( long nCommand, long* plData, CString& sData );

	// Operations
    long BeginInsertion(long Timeout);
    long BeginRemoval(long Timeout);
    long EndInsertion();
    long EndRemoval();
    long CutPaper(long Percentage);
    long PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition);
    long PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment);
    long PrintImmediate(long Station, LPCTSTR Data);
    long PrintNormal(long Station, LPCTSTR Data);
    long PrintTwoNormal(long Stations, LPCTSTR Data1, LPCTSTR Data2);
    long RotatePrint(long Station, long Rotation);
    long SetBitmap(long BitmapNumber, long Station, LPCTSTR FileName, long Width, long Alignment);
    long SetLogo(long Location, LPCTSTR Data);
	long TransactionPrint(long Station, long Control);
	long ValidateData(long Station, LPCTSTR Data);
	void ReportPropertyNumber( long lProperty, long lValue );

private:
	static UINT OutputCompleteProc( void* lpParam );
	long NotifyTB( BYTE* pBuffer, long lBufferLen );
	long NotifyTBBitmap( CPrinterBitmap* Bitmap );
	long NotifyTBLogo( long Location, LPCTSTR strData );

	BOOL	m_bTransactionMode;
	BOOL	m_bRotateMode;
	long	m_lOutputDelay;
    BOOL    m_bPrinterMultiBitmapEnabled;
	int		m_nDirectIO;
	int		m_nTB5_NoNull;


    CPrinterBitmapMap m_receiptBitmaps;
    CPrinterBitmapMap m_slipBitmaps;
	CString			m_strTopLogo;
	CString			m_strBottomLogo;
	int				m_HdrVersion;
};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_POSPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
