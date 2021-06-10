// PrinterStation.h: interface for the CPrinterStation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTERSTATION_H__5A124411_6FC2_11D1_BC97_00A0249EA5CE__INCLUDED_)
#define AFX_PRINTERSTATION_H__5A124411_6FC2_11D1_BC97_00A0249EA5CE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "PrinterDib.h"
#include "SharedQueue.h"

class CNullService;


class CLCRString : public CString
{
public:
    CLCRString()
        {   Cols = 0;   }
    ~CLCRString() {}
    void Empty()
        {   Cols = 0;
            CString::Empty();
        }
    void IncrCols(unsigned Count)
        {   Cols += Count;   }
    unsigned GetCols() const
        {   return Cols;   }
protected:
    unsigned Cols;
};

// Possible paper states

class CPrinterStation  
   {
   public:
	   CPrinterStation(long nStation, long nNameId, CmDataCapture& cmDc);
      ~CPrinterStation() {}
      long GetPropertyNumber( long lPropIndex );
      CString GetPropertyString( long lPropIndex );
      void SetPropertyNumber(long lPropIndex, long lProperty);
      long ReadRegistry( const COposRegistry& oRegistry, CmDataCapture& cmDc );
      BOOL AddToComboBox( CComboBox& comboStation ) const;
      void SetPaperStates( CComboBox& comboSlip ) const;
      void SetPaperStatus( long nState );
      void SetService( CNullService* pService ) { m_pService = pService; }
      long OnError( long lResult, BOOL bAsync = TRUE );
      long PrintRequest( LPCTSTR sText ) { return ParsePrintData( CString(sText) ); }
      void DisplayOutput( CEdit& editData ) { 
         editData.SetWindowText( m_sOutputText );
         editData.SetSel( m_sOutputText.GetLength(), m_sOutputText.GetLength());
         }
      void OnClearOutput( void ) { m_sOutputText.Empty(); }
      long WaitForPaperState( int   nPaperState, long lTimeout );
      long PrintBitmap( class CNullPrinter& rPrinter, LPCTSTR  sFileName, long lWidth, long lAlignment );
      void DrawSelectedBitmap( CComboBox& comboBitmap );
      long SetBitmap( long nBitmapIndex, LPCTSTR sFileName, long lWidth, long lAlignment );
   private:
      void ReadBarcodeRotationList( COposRegistry& oStationRegistry );
      CString BuildBarCodeRotationList( void ) const;
   private:
      DWORD m_bCap2Color         : 1;
      DWORD m_bCapBold           : 1;
      DWORD m_bCapDblHigh        : 1;
      DWORD m_bCapDblWide        : 1;
      DWORD m_bCapDblWideDblHigh : 1;
      DWORD m_bCapEmptySensor    : 1;
      DWORD m_bCapItalic         : 1;
      DWORD m_bCapNearEndSensor  : 1;
      DWORD m_bCapUnderline      : 1;
      DWORD m_bCapBarcode        : 1;
      DWORD m_bCapBitmap         : 1;
      DWORD m_bCapLeft90         : 1;
      DWORD m_bCapPapercut       : 1;
      DWORD m_bCapRight90        : 1;
      DWORD m_bCapRotate180      : 1;
      DWORD m_bCapStamp          : 1;
      DWORD m_bCapFullSlip       : 1;
      DWORD m_bLetterQuality     : 1;
      DWORD m_bPresent           : 1;
      DWORD m_bBarcodeRotate0    : 1;
      DWORD m_bBarcodeRotateL90  : 1;
      DWORD m_bBarcodeRotateR90  : 1;
      DWORD m_bBarcodeRotate180  : 1;
      DWORD m_bAutoLineFeed      : 1;
      DWORD m_nPaperState        : 2;
      DWORD m_nDesiredPaperState : 2;
      DWORD m_nStation           : 3;
      DWORD m_bWinRunnerSupport  : 1;
      long  m_nLineChars;
      long  m_nLineHeight;
      long  m_nLineSpacing;
      long  m_nLineWidth;
      long  m_nLinesToPaperCut;
      long  m_nSidewaysMaxChars;
      long  m_nSidewaysMaxLines;
      long  m_nLinesNearEndToEnd;
      long  m_nMaxLines;
      CString           m_sLineCharsList;
      CString           m_sStationName;
      CCriticalSection  m_csAccess;
      CNullService*     m_pService;
      CString           m_sOutputText;
      CEvent            m_evPaperChange;
      CPrinterDib       m_dibBitmaps[20];
   	CSharedDataQueue  m_sdqData;

   private:
      friend class CPrinterDib;
      friend class CNullPrinter;
   private: // Curtiss' stuff
      long        m_nUpdates;             // Set to bit mask of characteristics to update.
      // These are reset at start of each print request.
      int         m_nHorizScale;          // 1 = SW, 2 = DW, etc.
      int         m_nVertScale;           // 1 = SH, 2 = DH, etc.
      BOOL        m_bBold;
      int         m_nUnderline;
      BOOL        m_bReverse;
      BOOL        m_bAlternateColor;      // Added by WTS.
      // The following flag tracks whether a non-SHSW mode is currently set.
      BOOL        m_bNotSHSW;
      CLCRString* m_psLCR;
      CLCRString  m_sLCRLeft;
      CLCRString  m_sLCRCenter;
      CLCRString  m_sLCRRight;
      BOOL        m_bPrintBuffer;
      CString     m_sLogoTop;
      CString     m_sLogoBottom;
      long        m_cMaxHorizScale;
      long        m_cMaxVertScale;
      long        m_cMaxUnderline;
      long        m_cReverseVideo;
      long        m_nLeftPadChars;
      long        m_nColumns;
      CmDataCapture&    m_cmDc;
      void ProcessEOL( CString* psDestLine, CString* psTermination = NULL );
      long ParsePrintData( const CString& sSrc );
      long PrintRequestEx( const CString& sSrc );
      enum GMSType
         {
         GMS_Normal,                     // Set characteristics back to normal.
         GMS_SHSW,                       // Set scaling to SH/SW.
         GMS_Update                      // Update changed characteristics.
         };
      CString GetModeString( GMSType eGMSType ) { return _T(""); }
   };


#endif // !defined(AFX_PRINTERSTATION_H__5A124411_6FC2_11D1_BC97_00A0249EA5CE__INCLUDED_)
