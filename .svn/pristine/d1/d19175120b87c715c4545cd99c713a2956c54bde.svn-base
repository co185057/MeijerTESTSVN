#include "..\stdafx.h"
#include "printer.h"


/* virtual */
long CPrintTextRequest::Process( 
   CPrintStation& rStation ) const
   {  
   return rStation.OnPrintNormal( m_sData ); 
   }

/* virtual */
long CPrintBarcodeRequest::Process( 
   CPrintStation& rStation ) const
   {  
       long lconv;
       long rc;

       // +TAR 352038
       if(m_nBinaryConversion != -1)
       {
           // Store the current setting for binary conversion.
           lconv = rStation.GetPrinter().GetBinaryConversion(); // Save current.

           // Set to the desired value.
           rStation.GetPrinter().SetBinaryConversion(m_nBinaryConversion);
       }
       // -TAR 352038

       rc = rStation.OnPrintBarcode( m_sName, m_nType, m_nHeight, m_nWidth, 
                                     m_nAlign, m_nTextPosition ); 

       // +TAR 352038
       if(m_nBinaryConversion != -1)
       {
           // Restore the original setting.
           rStation.GetPrinter().SetBinaryConversion(lconv);
       }
       // -TAR 352038

      return rc;
   }


/* virtual */
long CPrintBitmapRequest::Process( 
   CPrintStation& rStation ) const
   {  
   return rStation.OnPrintBitmap( m_sName, m_nWidth, m_nAlign ); 
   }

/* virtual */
long CCutPaperRequest::Process( 
   CPrintStation& rStation ) const
   {  
   return rStation.OnCutPaper( m_nPercentage ); 
   }

/* virtual */
long CChangePrintSideRequest::Process( 
   CPrintStation& rStation ) const
   {  
   return rStation.OnChangePrintSide( m_nPrintSide ); 
   }


long CPrintStation::PrintNormal( 
   const CString& sData ) 
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   ASSERT( rPrinter.GetAsyncMode( ) != FALSE );
   long lResult = OnPrintNormal( sData );
   return lResult;
   }


long CPrintStation::PrintBarcode( 
   const CString&    sName, 
   long              nType, 
   long              nHeight, 
   long              nWidth,
   long              nAlign, 
   long              nTextPosition ) 
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   ASSERT( rPrinter.GetAsyncMode( ) != FALSE );
   long lResult = OnPrintBarcode( sName, nType, nHeight, nWidth, nAlign, nTextPosition );
   return lResult;
   }



long CPrintStation::PrintBitmap( 
   const CString& sName,
   long           nWidth, 
   long           nAlign ) 
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   ASSERT( rPrinter.GetAsyncMode( ) != FALSE );
   long lResult;
   // This code emulates the functionality of DM re: bitmaps
   if ( sName.IsEmpty() == FALSE )
      {
      lResult = OnPrintBitmap( sName, nWidth, nAlign );
      if( lResult  == OPOS_E_ILLEGAL 
         || rPrinter.GetResultCodeExtended() == OPOS_EPTR_TOOBIG )
         {     // print using printer width
         if ( nWidth == PTR_BM_ASIS )
            lResult = OnPrintBitmap( sName, GetLineWidth(), nAlign );
         }
      }
   else
      lResult = PrintNormal( _T("\x1b|1B\n") );
   return lResult;
   }

long CPrintStation::CutPaper( 
   long nPercentage )
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   ASSERT( rPrinter.GetAsyncMode( ) != FALSE );
   long lResult = OnCutPaper( nPercentage );
   return lResult;
   }

long CPrintStation::ChangePrintSide( 
   long nPrintSide )
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   ASSERT( rPrinter.GetAsyncMode( ) != FALSE );
   long lResult = OnChangePrintSide( nPrintSide );
   return lResult;
   }


long CPrintStation::RetryOutput( 
   void ) 
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   return rPrinter.RetryOutput( m_nStation );
   }


void CPrintStation::ClearOutput( void )
   {
   GetPrinter().ClearOutput();
   return;
   }

CPrinterWrapper& CPrintStation::GetPrinter( 
   void ) const 
   {
   // This may throw if the station is unavailable !
   if ( m_rPtr.IsAvailable() == FALSE )
      CDeviceManagerEx::ThrowDmException( DM_E_DEVICENOTAVAILABLE );
   return m_rPtr;
   }


/* virtual */
void CPrintStation::FireStatusEvent( 
   long nStatus )
   {
   GetPrinter().PostDmEventToApplication( DM_STATUS, GetDeviceClass(), 
      m_nDeviceId, nStatus );
   return;
   }


// All stations print strings - pass wrapper class and data
/* virtual */
long CPrintStation::OnPrintNormal( 
   const CString& sData ) const
   { 
   return GetPrinter().PrintNormal( m_nStation, sData ); 
   }

/* virtual */
long CPrintStation::OnPrintBarcode( 
   const CString& sName, 
   long           nType, 
   long           nHeight, 
   long           nWidth, 
   long           nAlign, 
   long           nTextPosition ) const
   { 
   return GetPrinter().PrintBarCode( m_nStation, 
      sName, nType, nHeight, nWidth, nAlign, nTextPosition ); 
   }

/* virtual */
long CPrintStation::OnPrintBitmap( 
   const CString& sData,
   long           nWidth, 
   long           nAlign ) const
   { 
   return GetPrinter().PrintBitmap( m_nStation, sData, nWidth, nAlign ); 
   }

/* virtual */
long CPrintStation::OnCutPaper( 
   long nPercentage ) const
   { 
   return GetPrinter().CutPaper( nPercentage ); 
   }

/* virtual */
long CPrintStation::OnChangePrintSide( 
   long nPrintSide ) const
   { 
   return GetPrinter().ChangePrintSide( nPrintSide );
   }


long CPrintStation::GetResultCodeExtended( 
   void ) const
   {
   return GetPrinter().GetResultCodeExtended( ); 
   }

long CPrintStation::GetResultCode( 
   void ) const
   {
   return GetPrinter().GetResultCodeExtended( ); 
   }

long CPrintStation::CheckHealth( 
   long lLevel )
   {
   return GetPrinter().CheckHealth( lLevel ); 
   }


/* virtual */
long CPrintStation::SetBitmap( 
   long     nNumber, 
   LPCTSTR  sFileName, 
   long     nWidth, 
   long     nAlignment)
   {
   CPrinterWrapper& rPrinter = GetPrinter();
   long lResult = rPrinter.SetBitmap( nNumber, m_nStation, sFileName, nWidth, nAlignment);
   if( lResult  == OPOS_E_ILLEGAL 
      || rPrinter.GetResultCodeExtended() == OPOS_EPTR_TOOBIG )
      {     // print using printer width
      if ( nWidth == PTR_BM_ASIS )
         lResult = rPrinter.SetBitmap( nNumber, m_nStation, 
            sFileName, GetLineWidth(), nAlignment);
      }
   return lResult;
   }

// CSlipStation::BeginInsertion
//
// Purpose: Application always wants the event after this call. 
//          If the slip was already inserted, the event may not have made it
//          to the app. So if we find that the slip is already inserted, 
//          we need to fire an event immediately to make up for the one that
//          was discarded. 
// Note:    If the timeout is non-zero, then the SO may queue up an event anyway. 
//          so there will be two events in the queue. This is not a problem
//          because the second event will be discarded. 

/* virtual */
long CSlipStation::BeginInsertion( 
   long lTimeout )
   { 
   long lResult = OPOS_SUCCESS;
   CPrinterWrapper& rPrinter = GetPrinter();
   if ( GetPaperEmpty() != FALSE )        // If slip not present, open jaws
      lResult = rPrinter.BeginInsertion( lTimeout );
   m_bOkToFireSlipInserted = TRUE;   // Allow event to be fired to the application
   if ( lResult == OPOS_SUCCESS )
      FireStatusEvent( DM_PTR_PAPEROK );
   return lResult; 
   }

// CSlipStation::BeginRemoval
//
// Purpose: Application always wants the event after this call. 
//          If the slip was already removed, the event may not have made it
//          to the app. So if we find that the slip is gone, 
//          we need to fire an event immediately to make up for the one that
//          was discarded. 
// Note:    Same as above. 

/* virtual */
long CSlipStation::BeginRemoval( 
   long lTimeout )
   {
   long lResult = OPOS_SUCCESS;
   CPrinterWrapper& rPrinter = GetPrinter();
   if ( GetPaperEmpty() == FALSE )        // If slip present, open the jaws
      lResult = rPrinter.BeginRemoval( lTimeout );
   m_bOkToFireSlipRemoved = TRUE;    // Allow event to be fired
   if ( lResult == OPOS_SUCCESS 
      && GetPaperEmpty() != FALSE )  // Printer lies, Tar#????? Check again
      FireStatusEvent( DM_PTR_EMPTY );
   return lResult; 
   }


// CSlipStation::BeginRemovalForced
//
// Purpose: Similiar to BeginRemoval except that it will ignore the
//          current status of the Paper tray.  This is a workaround in case
//          the printer is holding a slip inside the printer passed the 
//          sensor and won't eject it.  
// Note:    Same as above. 

/* virtual */
long CSlipStation::BeginRemovalForced( 
   long lTimeout )
   {
   long lResult = OPOS_SUCCESS;
   CPrinterWrapper& rPrinter = GetPrinter();
   lResult = rPrinter.BeginRemoval( lTimeout );
   m_bOkToFireSlipRemoved = TRUE;    // Allow event to be fired
   if ( lResult == OPOS_SUCCESS 
      && GetPaperEmpty() != FALSE )  // Printer lies, Tar#????? Check again
      FireStatusEvent( DM_PTR_EMPTY );
   return lResult; 
   }



// CSlipStation::FireStatusEvent
//
// Purpose: Filter out paper inserted/removed event when the application is not
//          expecting them.
//          We suppress slip insertion removal events unless the application is
//          ready for them.

/* virtual */
void CSlipStation::FireStatusEvent( 
   long nStatus )
   {
   BOOL bResult = true;
   if ( nStatus == DM_PTR_PAPEROK )
      {
      bResult = m_bOkToFireSlipInserted;
      m_bOkToFireSlipInserted = false;
      }
   else if ( nStatus == DM_PTR_EMPTY )
      {
      bResult = m_bOkToFireSlipRemoved;
      m_bOkToFireSlipRemoved = false;
      }
   if ( bResult != false )
      CPrintStation::FireStatusEvent( nStatus );
   return ;
   }




