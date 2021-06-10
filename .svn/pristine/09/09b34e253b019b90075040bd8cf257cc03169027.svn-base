////////////////////////////////////////////////////////////////////////////
//
// printer.cpp
//
//   Implementation of IDispatch wrapper class for CPrinterWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Integration_Customer/Meijer/CURRENT_DEV/Platform-Core-Patch/core/SCOTAPP/dmx/printer.cpp $
//
// $Revision: 4 $
//
// $Date: 10/30/08 9:38a $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "printer.h"
#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMX")

static const struct 
   {
   long nResult;
   long nRcEx;
   long nDmError;
   }  g_mapOposError[] = 
   {
   OPOS_E_EXTENDED,     OPOS_EPTR_COVER_OPEN,   DM_PTR_ERR_COVER_OPEN,
   OPOS_E_EXTENDED,     OPOS_EPTR_REC_EMPTY,    DM_PTR_ERR_EMPTY,
   OPOS_E_EXTENDED,     OPOS_EPTR_JRN_EMPTY,    DM_PTR_ERR_EMPTY,
   OPOS_E_EXTENDED,     OPOS_EPTR_SLP_EMPTY,    DM_PTR_ERR_EMPTY,
   OPOS_E_NOHARDWARE,   0,                      DM_PTR_ERR_FAULT,
   OPOS_E_OFFLINE,      0,                      DM_PTR_ERR_FAULT,
   // +TAR 352251
   OPOS_E_FAILURE,      NCR_EPTR_RPM_NOT_READY, DM_PTR_ERR_RPM_NOT_READY,
   OPOS_E_FAILURE,      NCR_EPTR_CDS_DOOR_OPEN, DM_PTR_ERR_CDS_DOOR_OPEN,
   OPOS_E_FAILURE,      NCR_EPTR_RPM_MECH_ERROR,DM_PTR_ERR_RPM_MECH_ERROR,
   OPOS_E_FAILURE,      NCR_EPTR_RPM_PAPER_JAM, DM_PTR_ERR_RPM_PAPER_JAM,
   // -TAR 352251
   OPOS_E_FAILURE,      0,                      DM_PTR_ERR_FAULT,
   OPOS_E_TIMEOUT,      0,                      DM_PTR_ERR_FAULT,
   OPOS_E_BUSY,         0,                      DM_PTR_ERR_FAULT,
   };

static const struct 
   {
   long nStatusFromPrinter;
   long nDeviceClass;
   long nDmStatus;
   }  g_mapOposStatus[] = 
   {
   PTR_SUE_COVER_OPEN,        DMCLASS_PRINTER,     DM_PTR_COVER_OPEN,
   PTR_SUE_COVER_OK,          DMCLASS_PRINTER,     DM_PTR_COVER_CLOSED,
   PTR_SUE_JRN_EMPTY,         DMCLASS_JOURNAL,     DM_PTR_EMPTY,
   PTR_SUE_JRN_NEAREMPTY,     DMCLASS_JOURNAL,     DM_PTR_NEAREMPTY,
   PTR_SUE_JRN_PAPEROK,       DMCLASS_JOURNAL,     DM_PTR_PAPEROK,
   PTR_SUE_REC_EMPTY,         DMCLASS_RECEIPT,     DM_PTR_EMPTY,
   PTR_SUE_REC_NEAREMPTY,     DMCLASS_RECEIPT,     DM_PTR_NEAREMPTY,
   PTR_SUE_REC_PAPEROK,       DMCLASS_RECEIPT,     DM_PTR_PAPEROK,
   PTR_SUE_SLP_EMPTY,         DMCLASS_SLIP,        DM_PTR_EMPTY,
   PTR_SUE_SLP_NEAREMPTY,     DMCLASS_SLIP,        DM_PTR_NEAREMPTY,
   PTR_SUE_SLP_PAPEROK,       DMCLASS_SLIP,        DM_PTR_PAPEROK,
   };

/////////////////////////////////////////////////////////////////////////////
// CPrinterWrapper




CPrinterWrapper::CPrinterWrapper(
   long nDeviceId )
   : 
   m_nPartialCutPercentage( 80 ),
	m_nDefaultCharacterSet( 0 ),  //.rvl added for Req 3.2.12.1 and 3.2.12.3
	m_nDefaultLineChars(40),	// RFC 367092
   m_bUseReceipt( TRUE ),
   m_bUseJournal( TRUE ),
   m_bUseSlip( TRUE ),
   m_nCurrentStation( 0 )
   {
   m_aStations.SetSize( DMCLASS_PRINTER );
   m_nDeviceId = nDeviceId;
   }


BOOL CPrinterWrapper::Create( 
   CWnd* pParentWnd, 
   UINT nID )
	{
   return TOPOSPOSPrinter<CGenericOposIF>::Create(NULL, 0, CRect(0,0,0,0), pParentWnd, nID );
   }


/////////////////////////////////////////////////////////////////////////////
// CPrinterWrapper properties


BOOL CPrinterWrapper::GetCapJrnPresent()
{
	BOOL result = m_bUseJournal;
   if ( result != FALSE )
      result = TOPOSPOSPrinter<CGenericOposIF>::GetCapJrnPresent();
	return result;
}


BOOL CPrinterWrapper::GetCapRecPresent()
{
	BOOL result = m_bUseReceipt; 
   if ( result != FALSE )
	   result = TOPOSPOSPrinter<CGenericOposIF>::GetCapRecPresent();
	return result;
}



BOOL CPrinterWrapper::GetCapSlpPresent()
   {
	BOOL result = m_bUseSlip;
   if ( result != FALSE )
	   result = TOPOSPOSPrinter<CGenericOposIF>::GetCapSlpPresent();
	return result;
   }

/////////////////////////////////////////////////////////////////////////////
// CPrinterWrapper operations


long CPrinterWrapper::ClearOutput()
   {
   long lResult = TOPOSPOSPrinter<CGenericOposIF>::ClearOutput();
   m_listOfRequests.RemoveAll( );
   m_nCurrentStation = 0;     // No station currently printing
   return lResult;
}


long CPrinterWrapper::PrintNormal(
   long     nStation, 
   CString  sData )
   {
   SetFreezeEvents( TRUE );
	long result = TOPOSPOSPrinter<CGenericOposIF>::PrintNormal( nStation, sData );
   if ( result == OPOS_SUCCESS 
      && GetAsyncMode() != FALSE )
      AddRequest( nStation, new CPrintTextRequest( sData ) );
   SetFreezeEvents( FALSE );
	return result;
   }


long CPrinterWrapper::CutPaper(
   long nPercentage)
   {
	long result = OPOS_E_ILLEGAL;
   long nFeedLines = GetRecLinesToPaperCut();
   if ( nFeedLines  == 0 )
      {     // do it now using OPOS method
      SetFreezeEvents( TRUE );
      result = TOPOSPOSPrinter<CGenericOposIF>::CutPaper( nPercentage );
      if ( result == OPOS_SUCCESS 
         && GetAsyncMode() != FALSE )
         AddRequest( PTR_S_RECEIPT, new CCutPaperRequest( nPercentage ) );
      SetFreezeEvents( FALSE );
      }
   else // if ( GetCapRecPapercut() != FALSE )
      {
      // Check if receipt station can perform a paper cut before sending the
      // escape sequence.  This makes "autoadvance" cutting consistent with
      // "now" cutting.
      // build escape sequence to cut paper which includes required 
      // number of CR/LFs
      CString strCutEscSeq;
      strCutEscSeq.Format( _T("\x1b|%ldlF\x1b|%ldP"), 
         nFeedLines , nPercentage );
      result = PrintNormal( PTR_S_RECEIPT, strCutEscSeq );
      }
	return result;
   }

long CPrinterWrapper::ChangePrintSide(
   long nPrintSide)
   {
	long result = OPOS_E_ILLEGAL;
   
    SetFreezeEvents( TRUE );
    result = TOPOSPOSPrinter<CGenericOposIF>::ChangePrintSide( nPrintSide );
    if ( result == OPOS_SUCCESS 
       && GetAsyncMode() != FALSE )
       AddRequest( PTR_S_SLIP, new CChangePrintSideRequest( nPrintSide ) );
    SetFreezeEvents( FALSE );
    
	return result;
   }

long CPrinterWrapper::RotatePrint(long nStation, long Rotation)
   {
	long result = OPOS_E_ILLEGAL;
   // We don't support this at this time, we would need to set a flag
   // to indicate that we are in rotate mode, subsequent requests
   // are queued, but no output id is assigned. This is tricky
   ASSERT( FALSE );
#if 0
   result = TOPOSPOSPrinter<CGenericOposIF>::RotatePrint(nStation, Rotation);
#endif
	return result;
   }

long CPrinterWrapper::PrintBarCode(
   long     nStation, 
   LPCTSTR  sData, 
   long     nSymbology, 
   long     nHeight, 
   long     nWidth, 
   long     nAlign, 
   long     nTextPosition )
   {
   SetFreezeEvents( TRUE );
    long result = TOPOSPOSPrinter<CGenericOposIF>::PrintBarCode(
      nStation, sData, nSymbology, nHeight, nWidth, nAlign, nTextPosition );
   if ( result == OPOS_SUCCESS 
      && GetAsyncMode() != FALSE )
      // TAR 352038 - Store current binary conversion setting with
      //              CPrintBarcodeRequest object.
      AddRequest( nStation, new CPrintBarcodeRequest( sData, nSymbology, nHeight, 
         nWidth,nAlign, nTextPosition, GetBinaryConversion()) );
   SetFreezeEvents( FALSE );
    return result;
   }

long CPrinterWrapper::PrintBitmap(
   long     nStation, 
   LPCTSTR  sFileName, 
   long     nWidth, 
   long     nAlign )
   {
   SetFreezeEvents( TRUE );
	long result = TOPOSPOSPrinter<CGenericOposIF>::PrintBitmap( nStation, sFileName, nWidth, nAlign );
   if ( result == OPOS_SUCCESS 
      && GetAsyncMode() != FALSE )
      AddRequest( nStation, new CPrintBitmapRequest( sFileName, nWidth, nAlign ) );
   SetFreezeEvents( FALSE );
	return result;
   }

// TPB Additions

/* static */
TClassPtr<CGenericOposIF> CPrinterWrapper::CreateDevice( 
   HKEY hKey,
   long nDeviceId )
   {
   return new CPrinterWrapper( nDeviceId );
   }


void CPrinterWrapper::ReadRegistry( 
   HKEY hKey )
   {   
#if 0
   CString sData = ReadRegString( hKey, _T("PaperCutPartial") );
   sData.MakeUpper();
   m_bPaperCutPartial = sData.Left( 1 ) == _T("Y");
	// the actual default for this will be RecLinesToPaperCut property
   m_nPaperCutLinesToAdvance = _ttoi( ReadRegString( hKey, _T("PaperCutLinesToAdvance")) );
#endif
	//.rvl added for Req 3.2.12.1 and 3.2.12.3
 	m_nDefaultCharacterSet = _ttoi( ReadRegString( hKey, _T("DefaultCharacterSet") ) );
	m_nDefaultLineChars = _ttoi( ReadRegString( hKey, _T("DefaultRecLineChars")) );	// RFC 367092

   return;
   }


CPrintStation* CPrinterWrapper::GetPrintStation( 
   long nDeviceClass )
   {
   return m_aStations[ nDeviceClass ];
   }

void CPrinterWrapper::AddRequest(  
   long                       nStation,
   TClassPtr<CPrintRequest>   pRequest )
   {  
   // You can only print to one station at a time. I don't have time 
   // to make the station as printer concept perfect in this regard, 
   // so I'm limiting printing to one station at a time.
   ASSERT( m_nCurrentStation == 0
      || m_nCurrentStation == nStation );
   m_nCurrentStation = nStation;
   m_listOfRequests.AddTail( 
      new CPrintMapElement( nStation, GetOutputID(), pRequest ) ); 
   return;
   }


void CPrinterWrapper::CreateStation( 
   long nDeviceClass, 
   long nDeviceId )
   {
   TClassPtr<CPrintStation> pStation;
   switch( nDeviceClass )
      {
      case DMCLASS_JOURNAL:
         pStation = new CJournalStation( *this, nDeviceId  );
         break;
      case DMCLASS_RECEIPT:
         pStation = new CReceiptStation( *this, nDeviceId  );
         break;
      case DMCLASS_SLIP:
         pStation = new CSlipStation( *this, nDeviceId );
         break;
      default:
         ASSERT( FALSE );
      }
   m_aStations.SetAt( nDeviceClass, pStation );
   }




/* virtual */
void CPrinterWrapper::OnOposStatusEvent( 
										long  nStatus )
{
	try{
		for ( int nIndex = 0; 
		nIndex < sizeof( g_mapOposStatus ) / sizeof( g_mapOposStatus[0] )
			&& nStatus != g_mapOposStatus[ nIndex ].nStatusFromPrinter; nIndex++ )
			continue;      
		ASSERT( nIndex < sizeof( g_mapOposStatus ) / sizeof( g_mapOposStatus[0] ) );
		nStatus =g_mapOposStatus[ nIndex ].nDmStatus;
		// The device class, DMCLASS_PRINTER, is used internal to DM. It indicates
		// that the status is not associated with any printer station.
		long nDeviceClass = g_mapOposStatus[ nIndex ].nDeviceClass;
		if ( nDeviceClass == DMCLASS_PRINTER )
        {   // TAR 223498/224782/225005 - prevent infinite loop
			for (nIndex = DMCLASS_RECEIPT; nIndex < m_aStations.GetSize(); nIndex++ )
			{
				if ( m_aStations[ nIndex  ] != NULL )
				{
					nDeviceClass = nIndex;
					break;
				}
			}
		}
		// it should get mapped. 
		ASSERT( nDeviceClass != DMCLASS_PRINTER );
		// Now get the device Id from the station 
		//RFC 356020
		//ASSERT( m_aStations[ nDeviceClass ] != NULL );  
		if (m_aStations[ nDeviceClass ])
		{
			m_aStations[ nDeviceClass ]->FireStatusEvent( nStatus  );
		}
		else
		{
			trace(L0, _T("Could not fire event for devclass:%d, status event:%d, nIndex:%d"), nDeviceClass ,nStatus, nIndex);
		}
	}
	catch(...)
	{
		ASSERT(0);
		TRACE(_T("Unexpected exception caught in CPrinterWrapper::OnOposStatusEvent()"));
		trace(L6, _T("Unexpected exception caught in CPrinterWrapper::OnOposStatusEvent()"));
	}
	return;
}


/* virtual */
void CPrinterWrapper::OnOposErrorEvent(
   long  nResult,
   long  nRcEx )
   {
   for ( int nIndex = 0; 
      nIndex < sizeof( g_mapOposError ) / sizeof( g_mapOposError[0] ); nIndex++ )
      {
      if ( nResult == g_mapOposError[ nIndex ].nResult )
         {
         if ( g_mapOposError[ nIndex ].nRcEx == 0 
            || g_mapOposError[ nIndex ].nRcEx == nRcEx )
            break;
         }
      }
   ASSERT( nIndex < sizeof( g_mapOposError ) / sizeof( g_mapOposError[0] ) );
   long nStatus = DM_ERROR_UNEXPECTED;
   if ( nIndex < sizeof( g_mapOposError ) / sizeof( g_mapOposError[0] ) )
      nStatus = g_mapOposError[ nIndex ].nDmError;
   else
      nStatus = MapOposErrorToDM( nResult );
   long nDeviceClass = GetDeviceClass( m_nCurrentStation );
   ASSERT( m_aStations[ nDeviceClass ] != NULL );
   long nDeviceId = m_aStations[ nDeviceClass ]->m_nDeviceId;
   if ( nStatus == DM_ERROR_UNEXPECTED )
      trace(L6, _T("DMX:OnOposErrorEvent??? PTR-")
      _T("%d (%d,%d) Posting DM_ERROR_UNEXPECTED"), nDeviceId, nResult, nRcEx );
   PostDmEventToApplication( DM_ERROR, nDeviceClass, nDeviceId, nStatus );
   if ( nDeviceClass == DMCLASS_SLIP )
      ClearOutput();
   return;
   }

/* virtual */
void CPrinterWrapper::OnOposOutputCompleteEvent( 
   long  nOutputId )
   {
     try
     {
       trace(L6,_T("+CPrinterWrapper::OnOposOutputCompleteEvent(%d)"), nOutputId);
       ASSERT( m_listOfRequests.GetCount() != 0 );
       ASSERT( m_listOfRequests.GetHead( )->m_nOutputId == nOutputId );
       while ( m_listOfRequests.GetCount() != 0 
          && m_listOfRequests.GetHead( )->m_nOutputId <= nOutputId )
          m_listOfRequests.RemoveHead( );
       long nDeviceClass = GetDeviceClass( m_nCurrentStation );
       long nStatusType = 0;
       if ( m_listOfRequests.GetCount() == 0 )
          {
          // Now get the device Id from the station 
          ASSERT( m_aStations[ nDeviceClass ] != NULL );
          long nDeviceId = m_aStations[ nDeviceClass ]->m_nDeviceId;
          PostDmEventToApplication( DM_STATUS, nDeviceClass, nDeviceId,  
             DM_PTR_REQUEST_COMPLETED );
          m_nCurrentStation = 0;  // Printing is complete
          }
       return;
     }
     catch(...)
     {
       ASSERT(0);
       TRACE(_T("Unexpected exception caught in CPrinterWrapper::OnOposOutputCompleteEvent()"));
       trace(L6, _T("Unexpected exception caught in CPrinterWrapper::OnOposOutputCompleteEvent()"));
     }

   }

void CPrinterWrapper::OnOposDirectIOEvent( long nEventId, long* plData, BSTR* pbstrData )
{ 
    // nothing to do for now, done to get rid of the assertions for the F306 throat sensor
    return; 
}


long CPrinterWrapper::GetDeviceClass( 
   long nStation ) const
   {
   long lResult = DMCLASS_PRINTER;
   if ( nStation == PTR_S_JOURNAL )
      lResult = DMCLASS_JOURNAL;
   else if ( nStation == PTR_S_RECEIPT )
      lResult = DMCLASS_RECEIPT;
   else if ( nStation == PTR_S_SLIP )
      lResult = DMCLASS_SLIP;
   ASSERT( lResult != DMCLASS_PRINTER );
   ASSERT( m_aStations[ lResult ] != NULL );
   return lResult;
   }

/* virtual */
BOOL CPrinterWrapper::IsAvailable( 
   void ) 
   {
   BOOL bResult = CGenericOposIF::IsAvailable( );
   if ( bResult != FALSE )
      bResult = GetCapRecPresent() || GetCapJrnPresent() || GetCapSlpPresent();
   return bResult;
   }

long CPrinterWrapper::RetryOutput(
   long nStation )
   {
   SetAsyncMode( FALSE );
   long lResult = OPOS_SUCCESS;
   while ( m_listOfRequests.GetCount() != 0 )
      {
      ASSERT( m_listOfRequests.GetHead()->m_nStation  == nStation );
      long nPrintStation = m_listOfRequests.GetHead()->m_nStation;
      CPrintStation* pPrinter = m_aStations[ GetDeviceClass( nPrintStation ) ];
      lResult = m_listOfRequests.GetHead()->Process( *pPrinter );
      if ( lResult != OPOS_SUCCESS )
         break;
      m_listOfRequests.RemoveHead( );
      }
   SetAsyncMode( TRUE );
   return lResult;
   }


/* virtual */
long CPrinterWrapper::DoOneTimeInitialization(
   void )
   {
   // Put printer into AsyncMode - application relies on this
   SetAsyncMode( TRUE );
	SetMapMode(PTR_MM_DOTS);
   long nBarCodeWidth = 2;
	DirectIO(NCRDIO_PTR_SET_BARCODE_WIDTH, &nBarCodeWidth, NULL);
   // Make sure our BMP's print as nicely as possible - 4/27/00 - ncr_barn
   if ( GetCapRecPresent() != FALSE )
      SetRecLetterQuality(TRUE);
   if ( GetCapSlpPresent() != FALSE )
      SetSlpLetterQuality(TRUE);
   if ( m_nDefaultCharacterSet > 0 )
      SetCharacterSet( m_nDefaultCharacterSet );
   // +RFC 367092
   if (m_nDefaultLineChars > 0)	   
		SetRecLineChars(m_nDefaultLineChars);
   // -RFC 367092
   return OPOS_SUCCESS;
   }
