// PrinterStation.cpp: implementation of the CPrinterStation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ncrnulldevice.h"
#include "PrinterStation.h"
#include <NCRptr.h>
#include "NullService.h"
#include "NullPrinter.h"
#include "PrinterRequests.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


static CString GetStationName(
   long nStation )
   {
   ASSERT( nStation == PTR_S_JOURNAL
      || nStation == PTR_S_RECEIPT
      || nStation == PTR_S_SLIP );
   LPCTSTR pString = _T("NullReceiptOutput");
   if ( nStation == PTR_S_JOURNAL )
      pString = _T("NullJournalOutput");
   else if ( nStation == PTR_S_SLIP )
      pString = _T("NullSlipOutput");
   return CString( pString );
   }


// Since there is no Rhyme or Reason to the layout of the PIDX value between 
// stations, we map the PIDX value passed ( station specific ) to a more general index.
// We store the properties by station.

typedef enum 
   {
   PIDXInvalid = 0,
   PIDXEmpty,
   PIDXLetterQuality,
   PIDXLineChars,
   PIDXLineHeight,
   PIDXLineSpacing,
   PIDXLineWidth,
   PIDXNearEnd,
   PIDXSidewaysMaxChars,
   PIDXSidewaysMaxLines,
   PIDXCap2Color,
   PIDXCapBold,
   PIDXCapDhigh,
   PIDXCapDwide,
   PIDXCapDwideDhigh,
   PIDXCapEmptySensor,
   PIDXCapItalic,
   PIDXCapNearEndSensor,
   PIDXCapUnderline,
   PIDXCapPresent,
   PIDXCapBarCode,
   PIDXCapBitmap,
   PIDXCapLeft90,
   PIDXCapRight90,
   PIDXCapRotate180,
   PIDXCapPapercut,
   PIDXCapStamp,
   PIDXCapFullslip,     
   PIDXLinesToPaperCut,
   PIDXLinesNearEndToEnd,
   PIDXMaxLines,
   PIDXBarCodeRotationList,
   PIDXLineCharsList,
   } STATION_PROPERTY_E;


// The center column is for testing to see that the table is built correctly

static struct 
   {
   long                 nPrinterPidx;
   long                 nPrinterStation;
   STATION_PROPERTY_E   ePidx;
   } MapStationProperties[] = 
   {
   PIDXPtr_JrnEmpty,             PTR_S_JOURNAL,       PIDXEmpty,
   PIDXPtr_RecEmpty,             PTR_S_RECEIPT,       PIDXEmpty,
   PIDXPtr_SlpEmpty,             PTR_S_SLIP,          PIDXEmpty,
   PIDXPtr_JrnLetterQuality,     PTR_S_JOURNAL,       PIDXLetterQuality, 
   PIDXPtr_RecLetterQuality,     PTR_S_RECEIPT,       PIDXLetterQuality, 
   PIDXPtr_SlpLetterQuality,     PTR_S_SLIP,          PIDXLetterQuality, 
   PIDXPtr_JrnLineChars,         PTR_S_JOURNAL,       PIDXLineChars,
   PIDXPtr_RecLineChars,         PTR_S_RECEIPT,       PIDXLineChars,
   PIDXPtr_SlpLineChars,         PTR_S_SLIP,          PIDXLineChars,
   PIDXPtr_JrnLineHeight,        PTR_S_JOURNAL,       PIDXLineHeight,
   PIDXPtr_RecLineHeight,        PTR_S_RECEIPT,       PIDXLineHeight,
   PIDXPtr_SlpLineHeight,        PTR_S_SLIP,          PIDXLineHeight,
   PIDXPtr_JrnLineSpacing,       PTR_S_JOURNAL,       PIDXLineSpacing,
   PIDXPtr_RecLineSpacing,       PTR_S_RECEIPT,       PIDXLineSpacing,
   PIDXPtr_SlpLineSpacing,       PTR_S_SLIP,          PIDXLineSpacing,
   PIDXPtr_JrnLineWidth,         PTR_S_JOURNAL,       PIDXLineWidth,
   PIDXPtr_RecLineWidth,         PTR_S_RECEIPT,       PIDXLineWidth,
   PIDXPtr_SlpLineWidth,         PTR_S_SLIP,          PIDXLineWidth,
   PIDXPtr_JrnNearEnd,           PTR_S_JOURNAL,       PIDXNearEnd,
   PIDXPtr_RecNearEnd,           PTR_S_RECEIPT,       PIDXNearEnd,
   PIDXPtr_SlpNearEnd,           PTR_S_SLIP,          PIDXNearEnd,
   PIDXPtr_CapJrn2Color,         PTR_S_JOURNAL,       PIDXCap2Color,
   PIDXPtr_CapRec2Color,         PTR_S_RECEIPT,       PIDXCap2Color,
   PIDXPtr_CapSlp2Color,         PTR_S_SLIP,          PIDXCap2Color,
   PIDXPtr_CapJrnBold,           PTR_S_JOURNAL,       PIDXCapBold,
   PIDXPtr_CapRecBold,           PTR_S_RECEIPT,       PIDXCapBold,
   PIDXPtr_CapSlpBold,           PTR_S_SLIP,          PIDXCapBold,
   PIDXPtr_CapJrnDhigh,          PTR_S_JOURNAL,       PIDXCapDhigh,
   PIDXPtr_CapRecDhigh,          PTR_S_RECEIPT,       PIDXCapDhigh,
   PIDXPtr_CapSlpDhigh,          PTR_S_SLIP,          PIDXCapDhigh,
   PIDXPtr_CapJrnDwide,          PTR_S_JOURNAL,       PIDXCapDwide,
   PIDXPtr_CapRecDwide,          PTR_S_RECEIPT,       PIDXCapDwide,
   PIDXPtr_CapSlpDwide,          PTR_S_SLIP,          PIDXCapDwide,
   PIDXPtr_CapJrnDwideDhigh,     PTR_S_JOURNAL,       PIDXCapDwideDhigh,
   PIDXPtr_CapRecDwideDhigh,     PTR_S_RECEIPT,       PIDXCapDwideDhigh,
   PIDXPtr_CapSlpDwideDhigh,     PTR_S_SLIP,          PIDXCapDwideDhigh,
   PIDXPtr_CapJrnEmptySensor,    PTR_S_JOURNAL,       PIDXCapEmptySensor,
   PIDXPtr_CapRecEmptySensor,    PTR_S_RECEIPT,       PIDXCapEmptySensor,
   PIDXPtr_CapSlpEmptySensor,    PTR_S_SLIP,          PIDXCapEmptySensor,
   PIDXPtr_CapJrnItalic,         PTR_S_JOURNAL,       PIDXCapItalic,
   PIDXPtr_CapRecItalic,         PTR_S_RECEIPT,       PIDXCapItalic,
   PIDXPtr_CapSlpItalic,         PTR_S_SLIP,          PIDXCapItalic,
   PIDXPtr_CapJrnNearEndSensor,  PTR_S_JOURNAL,       PIDXCapNearEndSensor,
   PIDXPtr_CapRecNearEndSensor,  PTR_S_RECEIPT,       PIDXCapNearEndSensor,
   PIDXPtr_CapSlpNearEndSensor,  PTR_S_SLIP,          PIDXCapNearEndSensor,
   PIDXPtr_CapJrnUnderline,      PTR_S_JOURNAL,       PIDXCapUnderline,
   PIDXPtr_CapRecUnderline,      PTR_S_RECEIPT,       PIDXCapUnderline, 
   PIDXPtr_CapSlpUnderline,      PTR_S_SLIP,          PIDXCapUnderline,
   PIDXPtr_CapJrnPresent,        PTR_S_JOURNAL,       PIDXCapPresent,
   PIDXPtr_CapRecPresent,        PTR_S_RECEIPT,       PIDXCapPresent,
   PIDXPtr_CapSlpPresent,        PTR_S_SLIP,          PIDXCapPresent,
   PIDXPtr_CapRecBarCode,        PTR_S_RECEIPT,       PIDXCapBarCode,
   PIDXPtr_CapSlpBarCode,        PTR_S_SLIP,          PIDXCapBarCode,
   PIDXPtr_CapRecBitmap,         PTR_S_RECEIPT,       PIDXCapBitmap,
   PIDXPtr_CapSlpBitmap,         PTR_S_SLIP,          PIDXCapBitmap,
   PIDXPtr_CapRecLeft90,         PTR_S_RECEIPT,       PIDXCapLeft90,
   PIDXPtr_CapSlpLeft90,         PTR_S_SLIP,          PIDXCapLeft90,
   PIDXPtr_CapRecRight90,        PTR_S_RECEIPT,       PIDXCapRight90,
   PIDXPtr_CapSlpRight90,        PTR_S_SLIP,          PIDXCapRight90,
   PIDXPtr_CapRecRotate180,      PTR_S_RECEIPT,       PIDXCapRotate180,
   PIDXPtr_CapSlpRotate180,      PTR_S_SLIP,          PIDXCapRotate180,
   PIDXPtr_CapRecPapercut,       PTR_S_RECEIPT,       PIDXCapPapercut,        
   PIDXPtr_CapRecStamp,          PTR_S_RECEIPT,       PIDXCapStamp,           
   PIDXPtr_CapSlpFullslip,       PTR_S_SLIP,          PIDXCapFullslip,        
   PIDXPtr_RecSidewaysMaxChars,  PTR_S_RECEIPT,       PIDXSidewaysMaxChars,
   PIDXPtr_SlpSidewaysMaxChars,  PTR_S_SLIP,          PIDXSidewaysMaxChars,
   PIDXPtr_RecSidewaysMaxLines,  PTR_S_RECEIPT,       PIDXSidewaysMaxLines,
   PIDXPtr_SlpSidewaysMaxLines,  PTR_S_SLIP,          PIDXSidewaysMaxLines,
   PIDXPtr_RecLinesToPaperCut,   PTR_S_RECEIPT,       PIDXLinesToPaperCut, 
   PIDXPtr_SlpLinesNearEndToEnd, PTR_S_SLIP,          PIDXLinesNearEndToEnd,  
   PIDXPtr_SlpMaxLines,          PTR_S_SLIP,          PIDXMaxLines,
   // String properties
   PIDXPtr_JrnLineCharsList,     PTR_S_JOURNAL,       PIDXLineCharsList,
   PIDXPtr_RecLineCharsList,     PTR_S_RECEIPT,       PIDXLineCharsList,
   PIDXPtr_SlpLineCharsList,     PTR_S_SLIP,          PIDXLineCharsList,
   PIDXPtr_RecBarCodeRotationList,  PTR_S_RECEIPT,    PIDXBarCodeRotationList,
   PIDXPtr_SlpBarCodeRotationList,  PTR_S_SLIP,       PIDXBarCodeRotationList,
   };


// FindGenericPidx
// Must be found and must match station!!

static STATION_PROPERTY_E FindGenericPidx( 
   long  lPropIndex,
   long  lStation )
   {
   STATION_PROPERTY_E ePidx = PIDXInvalid;
   int nIndex;
   for ( nIndex = 0; nIndex< ARRAY_ELEMENT_COUNT( MapStationProperties ); nIndex++ )
      {
      if ( MapStationProperties[ nIndex ].nPrinterPidx == lPropIndex )
         {
         ePidx = MapStationProperties[ nIndex ].ePidx;
         ASSERT( lStation == MapStationProperties[ nIndex ].nPrinterStation );
         break;
         }
      }
   ASSERT( ePidx != PIDXInvalid );
   return ePidx;
   }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinterStation::CPrinterStation(
   long nStation,
   long nNameId,
   CmDataCapture& cmDc )
   : m_cmDc( cmDc ),
   m_bCap2Color( FALSE ),
   m_bCapBold( FALSE ),
   m_bCapDblHigh( FALSE ),
   m_bCapDblWide( FALSE ),
   m_bCapDblWideDblHigh( FALSE ),
   m_bCapEmptySensor( FALSE ),
   m_bCapItalic( FALSE ),
   m_bCapNearEndSensor( FALSE ),
   m_bCapUnderline( FALSE ),
   m_bCapBarcode( FALSE ),
   m_bCapBitmap( FALSE ),
   m_bCapLeft90( FALSE ),
   m_bCapPapercut( FALSE ),
   m_bCapRight90( FALSE ),
   m_bCapRotate180( FALSE ),
   m_bCapStamp( FALSE ),
   m_bCapFullSlip( FALSE ),
   m_nPaperState( nStation != PTR_S_SLIP ? PAPER_OK : PAPER_EMPTY ),
   m_bLetterQuality( FALSE ),
   m_bPresent( FALSE ),
   m_nStation( nStation ),
   m_nLineChars( 0 ),
   m_nLineHeight( 0 ),
   m_nLineSpacing( 0 ),
   m_nLineWidth( 0 ),
   m_nLinesToPaperCut( 0 ),
   m_nSidewaysMaxChars( 0 ),
   m_nSidewaysMaxLines( 0 ),
   m_nLinesNearEndToEnd( 0 ),
   m_bBarcodeRotate0( FALSE ),
   m_bBarcodeRotateL90( FALSE ),
   m_bBarcodeRotateR90( FALSE ),
   m_bBarcodeRotate180( FALSE ),
   m_nMaxLines( 0 ),
   m_pService( NULL ),
   m_evPaperChange( ),
   m_nDesiredPaperState( PAPER_OK ),
   m_nColumns( 0 ),
   m_sdqData(4096, GetStationName(nStation) ),
   m_bWinRunnerSupport( FALSE ),
   m_nLeftPadChars( 0 ),
   m_nUpdates( 0 ),
   m_nHorizScale( 1 ),
   m_nVertScale( 1 ),
   m_bBold( FALSE ),
   m_nUnderline( 0 ),
   m_bReverse( FALSE ),
   m_bAlternateColor( FALSE ),
   m_bNotSHSW( FALSE ),
   m_psLCR( ),
   m_bPrintBuffer( FALSE ),
   m_cMaxHorizScale( 1 ),
   m_cMaxVertScale( 1 ),
   m_cMaxUnderline( 1 ),
   m_cReverseVideo( 0 )
   {
   ASSERT( m_nStation == PTR_S_JOURNAL
      || m_nStation == PTR_S_RECEIPT
      || m_nStation == PTR_S_SLIP );
   m_sStationName.LoadString( nNameId );
   }

long CPrinterStation::GetPropertyNumber( 
   long lPropIndex )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   long lResult = 0;
   STATION_PROPERTY_E ePidx = FindGenericPidx( lPropIndex, m_nStation );

   if ( m_bPresent != FALSE )
      {
      switch( ePidx )
         {                                                 
         case PIDXEmpty:            
            lResult = (m_nPaperState == PAPER_EMPTY);  
            ASSERT( lResult == FALSE || m_bCapEmptySensor != FALSE );
            break;
         case PIDXLetterQuality:    lResult = m_bLetterQuality;      break;
         case PIDXLineChars:        lResult = m_nLineChars;          break;
         case PIDXLineHeight:       lResult = m_nLineHeight;         break;
         case PIDXLineSpacing:      lResult = m_nLineSpacing;        break;
         case PIDXLineWidth:        lResult = m_nLineWidth;          break;
         case PIDXNearEnd:          // Can't be Near End unless sensor is present
            lResult = m_bCapNearEndSensor != FALSE && m_nPaperState != PAPER_OK;  
            break;
         case PIDXSidewaysMaxChars: lResult = m_nSidewaysMaxChars;   break;
         case PIDXSidewaysMaxLines: lResult = m_nSidewaysMaxLines;   break;
         case PIDXCap2Color:        lResult = m_bCap2Color;          break;
         case PIDXCapBold:          lResult = m_bCapBold;            break;
         case PIDXCapDhigh:         lResult = m_bCapDblHigh;         break;
         case PIDXCapDwide:         lResult = m_bCapDblWide;         break;
         case PIDXCapDwideDhigh:    lResult = m_bCapDblWideDblHigh;  break;
         case PIDXCapEmptySensor:   lResult = m_bCapEmptySensor;     break;
         case PIDXCapItalic:        lResult = m_bCapItalic;          break;
         case PIDXCapNearEndSensor: lResult = m_bCapNearEndSensor;   break;
         case PIDXCapUnderline:     lResult = m_bCapUnderline;       break;
         case PIDXCapPresent:       lResult = m_bPresent;            break;
         case PIDXCapBarCode:       lResult = m_bCapBarcode;         break;
         case PIDXCapBitmap:        lResult = m_bCapBitmap;          break;
         case PIDXCapLeft90:        lResult = m_bCapLeft90;          break;
         case PIDXCapRight90:       lResult = m_bCapRight90;         break;                                     
         case PIDXCapRotate180:     lResult = m_bCapRotate180;       break;
         case PIDXCapPapercut:      lResult = m_bCapPapercut;        break;
         case PIDXCapStamp:         lResult = m_bCapStamp;           break;
         case PIDXCapFullslip:      lResult = m_bCapFullSlip;        break;
         case PIDXLinesToPaperCut:  lResult = m_nLinesToPaperCut;    break;
         case PIDXLinesNearEndToEnd:lResult = m_nLinesNearEndToEnd;  break;
         case PIDXMaxLines:         lResult = m_nMaxLines;           break;
         default:
            ASSERT( FALSE );
            break;
         }
      }
   return lResult;
   }


void CPrinterStation::SetPropertyNumber(
   long lPropIndex, 
   long lProperty)
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   STATION_PROPERTY_E ePidx = FindGenericPidx( lPropIndex, m_nStation );
   if ( m_bPresent != FALSE )
      {
      switch( ePidx )
         {
         case PIDXLetterQuality:    
            m_bLetterQuality = lProperty;      
            break;
         case PIDXLineChars:        
            m_nLineChars = lProperty;          
            break;
         case PIDXLineHeight:       
            m_nLineHeight = lProperty;         
            break;
         case PIDXLineSpacing:      
            m_nLineSpacing = lProperty;        
            break;
         default:
            ASSERT( FALSE );
            break;
         }
      }
   return;
   }


CString CPrinterStation::GetPropertyString( 
   long lPropIndex )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   CString sResult;
   STATION_PROPERTY_E ePidx = FindGenericPidx( lPropIndex, m_nStation );

   if ( m_bPresent != FALSE )
      {
      switch( ePidx )
         {                                                 
         case PIDXBarCodeRotationList:
            sResult = BuildBarCodeRotationList();
            break;  
         case PIDXLineCharsList:         
            sResult = m_sLineCharsList;
            break;
         default:
            ASSERT( FALSE );
            break;
         }
      }
   return sResult;
   }


long CPrinterStation::ReadRegistry( 
	const COposRegistry& oRegistry,
   CmDataCapture& cmDc )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
	CString sRegName = oRegistry.GetClass() + _T('\\') + oRegistry.GetName();
	COposRegistry oStationRegistry( sRegName, m_sStationName, cmDc );
	if ( oStationRegistry.OpenKey() != FALSE 
      && oStationRegistry.ReadBool( _T("Present"), IDS_PRINTER_STATION_PRESENT_COMMENT, TRUE ) != FALSE)
		{
      m_bPresent = TRUE;
      m_bWinRunnerSupport = oStationRegistry.ReadBool( _T("WinRunnerSupport") );
      // Read Common Properties
      m_bCap2Color = oStationRegistry.ReadBool( _T("Cap2Color"), IDS_PRINTER_CAP_2_COLOR_COMMENT );
      m_bCapBold = oStationRegistry.ReadBool( _T("CapBold"), IDS_PRINTER_CAP_BOLD_COMMENT );
      m_bCapDblHigh = oStationRegistry.ReadBool( _T("CapDoubleHigh"), IDS_PRINTER_CAP_DBL_HIGH_COMMENT );
      m_bCapDblWide = oStationRegistry.ReadBool( _T("CapDoubleWide"), IDS_PRINTER_CAP_DBL_WIDE_COMMENT );
      m_bCapDblWideDblHigh = oStationRegistry.ReadBool( _T("CapDoubleWideHigh"), IDS_PRINTER_CAP_DBL_WIDEHIGH_COMMENT );
      m_bCapEmptySensor = oStationRegistry.ReadBool( _T("CapEmptySensor"), IDS_PRINTER_CAP_EMPTY_SENSOR_COMMENT );
      m_bCapItalic = oStationRegistry.ReadBool( _T("CapItalic"), IDS_PRINTER_CAP_ITALIC_COMMENT );
      m_bCapNearEndSensor = oStationRegistry.ReadBool( _T("CapNearEndSensor"), IDS_PRINTER_CAP_NEAR_END_SENSOR_COMMENT );
      m_bCapUnderline = oStationRegistry.ReadBool( _T("CapUnderline"), IDS_PRINTER_CAP_UNDERLINE_COMMENT );
      oStationRegistry.ReadString( _T("LineCharsList"), m_sLineCharsList,IDS_PRINTER_LINECHARSLIST_COMMENT, _T("40") );
      m_nLineChars = _ttoi( m_sLineCharsList );
      if ( m_nLineChars == 0 )
         m_bPresent = FALSE;
      m_nLineHeight = oStationRegistry.ReadInt( _T("LineHeight"), IDS_PRINTER_LINEHEIGHT_COMMENT, 9 );
      if ( m_nLineHeight <= 0 )
         m_bPresent = FALSE;
      m_nLineSpacing = oStationRegistry.ReadInt( _T("LineSpacing"), IDS_PRINTER_LINESPACING_COMMENT, 11 );
      if ( m_nLineSpacing <= 0 )
         m_bPresent = FALSE;
      if ( m_nLineSpacing <= m_nLineHeight )
         m_bPresent = FALSE;
      m_nLineWidth = oStationRegistry.ReadInt( _T("LineWidth"), IDS_PRINTER_LINEWIDTH_COMMENT, 400 );
      if ( m_nLineWidth <= 0 )
         m_bPresent = FALSE;
      m_bAutoLineFeed = oStationRegistry.ReadBool( _T("AutoLineFeed"), IDS_PRINTER_AUTO_LINE_FEED_COMMENT );
      }
   // Read Receipt & Slip Properties
   if ( m_bPresent != FALSE 
      && m_nStation != PTR_S_JOURNAL )
      {
      m_bCapBitmap = oStationRegistry.ReadBool( _T("CapBitmap"), IDS_PRINTER_CAP_BITMAP_COMMENT );        
      m_bCapLeft90 = oStationRegistry.ReadBool( _T("CapLeft90"), IDS_PRINTER_CAP_LEFT90_COMMENT );        
      m_bCapRight90 = oStationRegistry.ReadBool( _T("CapRight90"), IDS_PRINTER_CAP_RIGHT90_COMMENT );        
      m_bCapRotate180 = oStationRegistry.ReadBool( _T("CapRotate180"), IDS_PRINTER_CAP_ROTATE180_COMMENT );        
      ReadBarcodeRotationList( oStationRegistry );
      if ( m_bCapLeft90 != FALSE 
         || m_bCapRight90 != FALSE )
         {
         m_nSidewaysMaxChars = oStationRegistry.ReadInt( _T("SidewaysMaxChars"), IDS_PRINTER_SIDEWAYSMAXCHARS_COMMENT, 0 );
         m_nSidewaysMaxLines = oStationRegistry.ReadInt( _T("SidewaysMaxLines"), IDS_PRINTER_SIDEWAYSMAXLINES_COMMENT, 0 );
         }
      }
   // Read Receipt Properties
   if ( m_bPresent != FALSE 
      && m_nStation == PTR_S_RECEIPT )
      {
      m_bCapPapercut = oStationRegistry.ReadBool( _T("CapPapercut"), IDS_PRINTER_CAP_PAPERCUT_COMMENT );
      m_bCapStamp = oStationRegistry.ReadBool( _T("CapStamp"), IDS_PRINTER_CAP_STAMP_COMMENT );
      m_nLinesToPaperCut = oStationRegistry.ReadInt( _T("LinesToPaperCut"), IDS_PRINTER_LINESTOCUT_COMMENT, 0 );
      }
   // Read Slip Properties
   if ( m_bPresent != FALSE 
      && m_nStation == PTR_S_SLIP )
      {
      m_bCapFullSlip = oStationRegistry.ReadBool( _T("CapFullSlip"), IDS_PRINTER_CAP_FULLSLIP_COMMENT );
      if ( m_bCapNearEndSensor != FALSE 
         && m_bCapEmptySensor != FALSE)
         m_nLinesNearEndToEnd = oStationRegistry.ReadInt( _T("LinesNearEndToEnd"), IDS_PRINTER_LINESNEARENDTOEND_COMMENT, 0 );
      if ( m_bCapFullSlip == FALSE )
         {
         m_nMaxLines = oStationRegistry.ReadInt( _T("MaxLines"), IDS_PRINTER_MAXLINES_COMMENT, 0 );
         m_bCapFullSlip = ( m_nMaxLines == 0 );
         }
      }
   return m_bPresent ? OPOS_SUCCESS : OPOS_E_NOEXIST;
   }

void CPrinterStation::ReadBarcodeRotationList( 
   COposRegistry& oStationRegistry )
   {
   m_bCapBarcode = oStationRegistry.ReadBool( _T("CapBarcode"), IDS_PRINTER_CAP_BARCODE_COMMENT );
   if ( m_bCapBarcode != FALSE )
      {
      static LPCTSTR pszName = _T("BarCodeRotationList");
      CString sValidRotationList;
      oStationRegistry.ReadString( pszName, sValidRotationList );
      while ( sValidRotationList.IsEmpty() == FALSE )
         {
         int nIndex = sValidRotationList.Find( _T(",") );
         CString sTemp;
         if ( nIndex == -1 )
            {
            sTemp = sValidRotationList;
            sValidRotationList.Empty();
            }
         else
            {
            sTemp = sValidRotationList.Left( nIndex );
            sValidRotationList = sValidRotationList.Mid( nIndex + 1 );
            }
         if ( sTemp == _T("0") )
            m_bBarcodeRotate0 = TRUE;
         else if ( sTemp == _T("L90") )
            m_bBarcodeRotateL90 = TRUE;
         else if ( sTemp == _T("R90") )
            m_bBarcodeRotateR90 = TRUE;
         else if ( sTemp == _T("180") )
            m_bBarcodeRotate180 = TRUE;
         else
            {  
            // Data Capture
            }
         }
      CString sRotationList = BuildBarCodeRotationList();
      oStationRegistry.WriteRegString( pszName, sRotationList, IDS_PRINTER_BARCODEROTATION_COMMENT );
      m_bCapBarcode = !sRotationList.IsEmpty();
      }
   return;
   }


BOOL CPrinterStation::AddToComboBox( 
   CComboBox& comboStation ) const
   {
   if ( m_bPresent != FALSE )
      {
      comboStation.SetItemData( comboStation.AddString( m_sStationName ), m_nStation );
      }
   return m_bPresent;
   }


CString CPrinterStation::BuildBarCodeRotationList(
   void ) const 
   {
   CString sResult;

   if ( m_bBarcodeRotate0 != FALSE )
      sResult += _T(",0");
   if ( m_bBarcodeRotateL90 != FALSE )
      sResult += _T(",L90");
   if ( m_bBarcodeRotateR90 != FALSE )
      sResult += _T(",R90");
   if ( m_bBarcodeRotate180 != FALSE )
      sResult += _T(",180");
   if ( sResult.IsEmpty() == FALSE )
      sResult = sResult.Mid(1);
   return sResult;
   }


void CPrinterStation::SetPaperStates( 
   CComboBox& comboPaper ) const
   {
   comboPaper.EnableWindow( m_bPresent );
   if ( m_bPresent != FALSE )
      {
      int nIndex = comboPaper.AddString( _T("OK") );
      comboPaper.SetItemData( nIndex, PAPER_OK );
      if ( m_bCapNearEndSensor != FALSE )
         {
         nIndex = comboPaper.AddString( _T("Near Empty") );
         comboPaper.SetItemData( nIndex, PAPER_NEAR_END );
         }
      if ( m_bCapEmptySensor != FALSE )
         {
         nIndex = comboPaper.AddString( _T("Empty") );
         comboPaper.SetItemData( nIndex, PAPER_EMPTY );
         }
      }
   comboPaper.SetCurSel( 0 );
   if (  m_nStation == PTR_S_SLIP )
      comboPaper.SetCurSel( comboPaper.GetCount() - 1 );
   return;
   }


static long GetPaperStatusEvent( 
   long nStation,
   long nState )
   {
   static const struct 
      {
      long nStation;
      long nState;
      long nEvent;
      } MapStatusToEvent[] = 
      {
      PTR_S_JOURNAL,       PAPER_OK,         PTR_SUE_JRN_PAPEROK,
      PTR_S_JOURNAL,       PAPER_NEAR_END,   PTR_SUE_JRN_NEAREMPTY,
      PTR_S_JOURNAL,       PAPER_EMPTY,      PTR_SUE_JRN_EMPTY,
      PTR_S_RECEIPT,       PAPER_OK,         PTR_SUE_REC_PAPEROK,  
      PTR_S_RECEIPT,       PAPER_NEAR_END,   PTR_SUE_REC_NEAREMPTY,
      PTR_S_RECEIPT,       PAPER_EMPTY,      PTR_SUE_REC_EMPTY,    
      PTR_S_SLIP,          PAPER_OK,         PTR_SUE_SLP_PAPEROK,  
      PTR_S_SLIP,          PAPER_NEAR_END,   PTR_SUE_SLP_NEAREMPTY,
      PTR_S_SLIP,          PAPER_EMPTY,      PTR_SUE_SLP_EMPTY
      };
   long lResult = 0;

   for ( int nIndex = 0; nIndex < ARRAY_ELEMENT_COUNT( MapStatusToEvent ); nIndex++ )
      {
      if ( MapStatusToEvent[ nIndex ].nStation == nStation
         && MapStatusToEvent[ nIndex ].nState == nState )
         {
         lResult = MapStatusToEvent[ nIndex ].nEvent;
         break;
         }
      }
   ASSERT( lResult  != 0 );   // Must Find
   return lResult ;
   }


void CPrinterStation::SetPaperStatus( 
   long nState )
   {
   ASSERT( m_bPresent != FALSE );
   switch( nState )
      {
      case PAPER_OK:
      case PAPER_NEAR_END:
      case PAPER_EMPTY:
         if ( m_nPaperState != (DWORD)nState )
            {
            CSingleLock csAccess( &m_csAccess, TRUE );
            m_nPaperState = nState;
            if ( m_nDesiredPaperState == m_nPaperState )
               m_evPaperChange.SetEvent();
            csAccess.Unlock();
            m_pService->AddEvent( new COposStatusUpdateEvent( 
               GetPaperStatusEvent( m_nStation, nState ) ) ); 
            }
         break;
      default:
         ASSERT( FALSE );
         break;
      }
   return;
   }


long CPrinterStation::WaitForPaperState( 
   int   nPaperState,
   long  lTimeout )
   {
   CSingleLock csAccess( &m_csAccess, TRUE );
   long lResult = OPOS_SUCCESS;

   if ( m_nPaperState != (unsigned)nPaperState )
      {
      m_evPaperChange.ResetEvent();
      m_nDesiredPaperState = nPaperState;
      csAccess.Unlock();
      CSingleLock evPaperChange( &m_evPaperChange );
      if ( evPaperChange.Lock( lTimeout ) == FALSE )
         lResult = OPOS_E_TIMEOUT;
      }
   return lResult;
   }



long CPrinterStation::OnError( 
   long lResult,
   BOOL bAsync /* = TRUE */ )
   {
   long nRCEx = 0;
   switch( lResult )
      {
      case OPOS_EPTR_COVER_OPEN:
         nRCEx = OPOS_EPTR_COVER_OPEN;
         lResult = OPOS_E_EXTENDED;
         break;
      case OPOS_EPTR_REC_EMPTY:
         lResult = OPOS_E_EXTENDED;
         if ( m_nStation == PTR_S_RECEIPT )
            nRCEx = OPOS_EPTR_REC_EMPTY;
         else if ( m_nStation == PTR_S_JOURNAL )
            nRCEx = OPOS_EPTR_JRN_EMPTY;
         else if ( m_nStation == PTR_S_SLIP )
            nRCEx = OPOS_EPTR_SLP_EMPTY;
         else
            ASSERT( FALSE );
         break;
      case OPOS_E_NOHARDWARE:
      case OPOS_E_OFFLINE:
      case OPOS_E_FAILURE:
      case OPOS_E_TIMEOUT:
      case OPOS_SUCCESS:
      case OPOS_E_ILLEGAL:
         break;
#ifdef ULY_PRINTER
      case NCR_EPTR_HEAD_JAM:
      case NCR_EPTR_KNIFE_JAM:
      case NCR_EPTR_PAPER_JAM:
      case NCR_EPTR_PAPER_NOT_LOADED_JAM:
      case NCR_EPTR_COMMS_ERROR:
      case NCR_EPTR_PRINTER_OPEN:
      case NCR_EPTR_REPLACE_HEAD:
      case NCR_EPTR_SIDEWAYS_COMMS_ERROR:
      case NCR_EPTR_EJECT_JAM:
      case NCR_EPTR_BLACKMARK_ERROR:
         nRCEx  = lResult;
         lResult = OPOS_E_FAILURE;
         break;
#endif
      default:
         ASSERT( FALSE );
      }
//   m_pService->m_nErrorStation = m_nStation;
   if ( bAsync != FALSE )
      m_pService->AddEvent( new COposErrorEvent( 
         lResult | (m_nStation << 30), nRCEx, OPOS_EL_OUTPUT ) );
   else
      m_pService->SetRC( lResult, nRCEx );
   return lResult;
   }


long CPrinterStation::PrintRequestEx( 
   const CString& sText )
   {
   long lResult = OPOS_EPTR_REC_EMPTY;
   CSingleLock csAccess( &m_csAccess, TRUE );
   if ( m_nPaperState != PAPER_EMPTY )
      {
      lResult = OPOS_SUCCESS;
      m_sOutputText += sText;
      if ( m_bWinRunnerSupport != FALSE )
		   m_sdqData.AppendData((const BYTE*)(LPCTSTR)sText, sText.GetLength() + sizeof(TCHAR));
      }
   return lResult;
   }


long CPrinterStation::SetBitmap( 
   long     nBitmapIndex,
   LPCTSTR  sFileName, 
   long     lWidth, 
   long     lAlignment )
   {
   long lResult = OPOS_E_ILLEGAL;
   if ( (unsigned)(nBitmapIndex) < ARRAY_ELEMENT_COUNT( m_dibBitmaps ) )
      {
      m_dibBitmaps[ nBitmapIndex ].Initialize( sFileName, lWidth, lAlignment );
      lResult = OPOS_SUCCESS;
      }
   return lResult;
   }


long CPrinterStation::PrintBitmap( 
   CNullPrinter&  rPrinter, 
   LPCTSTR        sFileName, 
   long           lWidth, 
   long           lAlignment)
   {
   long lResult = OPOS_E_ILLEGAL;
   if ( m_bCapBitmap != FALSE )
      {
      lResult = OPOS_SUCCESS;
      if ( m_pService->IsAsyncMode() != FALSE )
         {
	      m_pService->AddRequest(  new CPrinterBitmapRequest( 
            rPrinter, rPrinter.GetNextId(), m_nStation, sFileName, lWidth, lAlignment ) );
         }
      else
         {
         lResult = rPrinter.SyncOutputBitmap( m_nStation, sFileName, lWidth,  lAlignment );
         lResult = OnError( lResult, FALSE );
         }
      }
   return lResult;
   }

#if 0
void CPrinterStation::DrawSelectedBitmap( 
   int            nBitmapId,
   CBitmapFrame&  staticBitmap )
   {
   if ( nSelectedItem != -1 )
      {
      m_aDibs[ nSelectedItem ]->Draw(dc, const CRect* rcDst=NULL, 
         const CRect* rcSrc=NULL,
		BOOL bUseDrawDib=TRUE, CPalette* pPal=NULL, BOOL bForeground=FALSE);

         
         
         rectWindow

      }
   }

#endif