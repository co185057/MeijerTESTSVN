// POSPrinter.cpp: implementation of the CPOSPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "POSPrinter.h"
#include "TBService.h"
#include "TBOposIO.h"
#include "Resource.h"
#include "printerMsg.h"
#include "CmBstring.h"
#include "POSPrinterDirectIO.h"

//#define T_ID "CPOSPrinter"
//#define TM_MODULE TM_MODULE_4
//#include "traceSystem.h"

#include "SocketForTBDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPOSPrinter::CPOSPrinter(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_PTR,nId,nPort, cmDc),
					    m_HdrVersion(1)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::CPOSPrinter") );


    m_receiptBitmaps.RemoveAll();
    m_slipBitmaps.RemoveAll();

	//m_ReceiptBitmap1.m_strFileName.Empty();
	//m_ReceiptBitmap2.m_strFileName.Empty();
	//m_SlipBitmap1.m_strFileName.Empty();
	//m_SlipBitmap2.m_strFileName.Empty();
	m_bTransactionMode = FALSE;
	m_bRotateMode = FALSE;
}

CPOSPrinter::~CPOSPrinter()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::~CPOSPrinter") );


    int iKey;
    CPrinterBitmap * pBitmap = NULL;
    POSITION pos = m_receiptBitmaps.GetStartPosition();
    while( pos )
    {
        m_receiptBitmaps.GetNextAssoc(pos, iKey, pBitmap);
        if( pBitmap )
        {
            delete pBitmap;
            pBitmap = NULL;
        }
    }
    m_receiptBitmaps.RemoveAll();

    pBitmap = NULL;
    pos = m_slipBitmaps.GetStartPosition();
    while( pos )
    {
        m_slipBitmaps.GetNextAssoc(pos, iKey, pBitmap);
        if( pBitmap )
        {
            delete pBitmap;
            pBitmap = NULL;
        }
    }
    m_slipBitmaps.RemoveAll();
}

//
// Function:  Open
// Purpose:  Opens the POSPrinter and initializes operations based on the 
//           contents of the registry key for the device profile.
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
//          oRegistry - Registry class for accessing profile properties.
// Outputs:  Return code from Base Class
// Assumptions:  None
// Comments:  None
// 
long CPOSPrinter::Open( CTBService* pService, COposRegistry& oRegistry )
{
	CString sModel;
	int nSlipToggle;

	m_lOutputDelay = oRegistry.ReadInt( _T("OutputDelay"), 0, 500 );
	oRegistry.ReadString( _T("Model"), sModel, 0 , 
		_T("Axiom7156") );
	nSlipToggle = oRegistry.ReadInt( _T("SlipToggle"), 0, 0 ); 

    m_bPrinterMultiBitmapEnabled = oRegistry.ReadInt(_T("PrinterMultiBitmapEnabled"), 0, 0);
	m_HdrVersion= oRegistry.ReadInt( _T("tbControlsHdrVersion"), 0, 1);

    pService->InitializePOSPrinter( sModel, nSlipToggle );

	COposRegistry myRegistry = oRegistry;
	if ( myRegistry.OpenSubKey( _T("OE_Override")) )
	{
		pService->OverridePOSPrinter( myRegistry );
	}

	m_nDirectIO = oRegistry.ReadInt( _T("SendDirectIO"), 0, 0 );
	m_nTB5_NoNull = oRegistry.ReadInt( _T("TB5_NoNull"), 0, 0 );


	return CTBDevice::Open( pService, oRegistry );
}

// 
// Function: Close
// Purpose:  Frees resources related to an instance of a POSPrinter device
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
// Outputs:  None
// Assumptions:  None
// Comments:  None
//
void CPOSPrinter::Close( CTBService* pService )
{
	m_pTBService->UninitializePOSPrinter();

	CTBDevice::Close( pService);
}


//
// Function:  BeginInsertion
// Purpose:  Implementation of the OPOS BeginInsertion method
// Inputs:  Timeout - Not used in this implementation
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::BeginInsertion(long Timeout)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::BeginInsertion") );
	// The "device" is placed into insertion mode.
	// Return success to indicate a form was inserted.

	this->m_pTBService->SetSlipEmpty( FALSE );
	this->OnStatus( PTR_SUE_SLP_PAPEROK );

   // Notify the TB of beginInsertion event
   OPOSIO_PRINTER	 Msg={0};
	Msg.hdr.unit = PTR_S_SLIP;
	Msg.func = PTRFUNC_SLIPSTATUS;
   Msg.funcArg= SLIPSTATUS_BEGININSERTION;
   Msg.dataLen=1;
   Msg.Data[0]= 0;

	// Send the data to the TB
	this->NotifyTB( (UCHAR *)&Msg, sizeof(Msg) );
	return OPOS_SUCCESS;
}

//
// Function:  BeginRemoval
// Purpose:  Implementation of the OPOS BeginRemoval method
// Inputs:  Timeout - Not used in this implementation
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::BeginRemoval(long Timeout)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::BeginRemoval") );
	// The "device" is placed into form removal mode
	// Return success to indicate a form was removed

	this->m_pTBService->SetSlipEmpty( TRUE );
	this->OnStatus( PTR_SUE_SLP_EMPTY );
   
   // Notify the TB of beginRemoval event
   OPOSIO_PRINTER	 Msg={0};
	Msg.hdr.unit = PTR_S_SLIP;
	Msg.func = PTRFUNC_SLIPSTATUS;
   Msg.funcArg= SLIPSTATUS_BEGINREMOVAL;
   Msg.dataLen=1;
   Msg.Data[0]= 0;
	// Send the data to the TB
	this->NotifyTB( (UCHAR *)&Msg, sizeof(Msg) );

	return OPOS_SUCCESS;
}

//
// Function:  EndInsertion
// Purpose:  Implementation of the OPOS EndInsertion method
// Inputs:  None
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::EndInsertion()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::EndInsertion") );
	// The "device" is taken out of insertion mode
	// Return success to indicate that a form is present

   // Notify the TB of endInsertion event
    OPOSIO_PRINTER	 Msg={0};
	Msg.hdr.unit = PTR_S_SLIP;
	Msg.func = PTRFUNC_SLIPSTATUS;
   Msg.funcArg= SLIPSTATUS_ENDINSERTION;
   Msg.dataLen=1;
   Msg.Data[0]= 0;
	// Send the data to the TB
	this->NotifyTB( (UCHAR *)&Msg, sizeof(Msg) );

   return OPOS_SUCCESS;
}

//
// Function:  EndRemoval
// Purpose:  Implementation of the OPOS EndRemoval method
// Inputs:  None
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::EndRemoval()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::EndRemoval") );
	// The "device is taken out of removal mode
	// Return success to indicate the form is gone.

    // Notify TB of event
    OPOSIO_PRINTER	 Msg={0};
	Msg.hdr.unit = PTR_S_SLIP;
	Msg.func = PTRFUNC_SLIPSTATUS;
   Msg.funcArg= SLIPSTATUS_ENDREMOVAL;
   Msg.dataLen=1;
   Msg.Data[0]= 0;
	// Send the data to the TB
	this->NotifyTB( (UCHAR *)&Msg, sizeof(Msg) );

	return OPOS_SUCCESS;
}

//
// Function:  CutPaper
// Purpose:  Implementation of the OPOS CutPaper method
// Inputs:  Percentage - type of cut to be performed
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::CutPaper(long Percentage)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::CutPaper") );
	// Notify the TB
	BYTE	Buffer[PTR_DATA_OFFSET + 4];
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	long lDataLen = PTR_DATA_OFFSET + 4;

	// Pack the Data
	memset( Buffer, 0, lDataLen);
	pMsg->hdr.unit = PTR_S_RECEIPT;
	pMsg->func = PTRFUNC_CUTPAPER;
	pMsg->funcArg = Percentage;
	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( bAsync && !m_bTransactionMode )
	{
		// Need separate thread to handle Async notification
		// Set outputId
		m_pTBService->IncrementOutputId();
		// Initiate thread
		AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
	}
	
	return OPOS_SUCCESS;
}

//
// Function:  PrintBarCode
// Purpose:  Implementation of the OPOS PrintBarCode method
// Inputs:  Station - the print station
//          Symbology - the symbology of the barcode to be printed
//          Height - height of the barcode to be printed
//          Width - width of the barcode to be printed
//			Alignment - alignment of the barcode
//          TextPosition - Placement of readable character string
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::PrintBarCode") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;

    CmBString bstrData = Data;
	CString sASCII = bstrData.GetCString();
	CString sData = m_pTBService->ConvertASCII( sASCII );

	int nSendNull = 1;
	if ( m_nTB5_NoNull > 0 )
	{
		nSendNull = 0;
	}

	// Pack the Data
    long lBinLength = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData.GetLength() + nSendNull));
    Buffer = new BYTE[ lBinLength ];
	memset( Buffer, 0, lBinLength );
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = (BYTE)Station;
	pMsg->func = PTRFUNC_BARCODE;
	pMsg->funcArg = Symbology;
	pMsg->height = m_pTBService->ConvertMeasure( TRUE, Station, Height );
	pMsg->width = m_pTBService->ConvertMeasure( FALSE, Station, Width );
	if ( (Alignment == PTR_BM_LEFT) ||
		(Alignment == PTR_BM_CENTER) ||
		 (Alignment == PTR_BM_RIGHT) )
	{
		pMsg->alignment = Alignment;
	}
	else
	{
		pMsg->alignment = m_pTBService->ConvertMeasure( FALSE, Station, Alignment );
	}
	pMsg->textPosition = TextPosition;
	pMsg->dataLen = (sData.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( bAsync && !m_bTransactionMode )
	{
		// Need separate thread to handle Async notification
		// Set outputId
		m_pTBService->IncrementOutputId();
		// Initiate thread
		AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
	}

    delete[] Buffer;
	
	return OPOS_SUCCESS;
}

//
// Function:  PrintBitmap
// Purpose:  Implementation of the OPOS PrintBitmap method
// Inputs:  Station - the printer station
//          FileName - Name of Windows bitmap file
//          Width - printed width of the bitmap
//          Alignment - placement of the bitmap
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment)
{
   CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::PrintBitmap") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;
    CmBString bstrData = FileName;
	CString sData = bstrData.GetCString();

	int nSendNull = 1;
	if ( m_nTB5_NoNull > 0 )
	{
		nSendNull = 0;
	}

	// Pack the Data
    long lBinLength = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData.GetLength() + nSendNull));
    Buffer = new BYTE[lBinLength];
	memset( Buffer, 0, lBinLength);
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = (BYTE)Station;
	pMsg->func = PTRFUNC_BITMAP;
	if ( Width == PTR_BM_ASIS )
	{
		pMsg->width = Width;
	}
	else
	{
		pMsg->width = m_pTBService->ConvertMeasure( FALSE, Station, Width );
	}
	if ( (Alignment == PTR_BM_LEFT) ||
		(Alignment == PTR_BM_CENTER) ||
		 (Alignment == PTR_BM_RIGHT) )
	{
		pMsg->alignment = Alignment;
	}
	else
	{
		pMsg->alignment = m_pTBService->ConvertMeasure( FALSE, Station, Alignment );
	}
	pMsg->dataLen = (sData.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( bAsync && !m_bTransactionMode )
	{
		// Need separate thread to handle Async notification
		// Set outputId
		m_pTBService->IncrementOutputId();
		// Initiate thread
		AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
	}
	
    delete[] Buffer;

	return OPOS_SUCCESS;
}

//
// Function:  PrintImmediate
// Purpose:  Implementation of the OPOS PrintImmediate method
// Inputs:  Station - The printer station
//          Data - the text to be printed.
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::PrintImmediate(long Station, LPCTSTR Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::PrintImmediate") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;
    CmBString bstrData = Data;
	CString sASCII = bstrData.GetCString();
	CString sData = m_pTBService->ConvertASCII( sASCII );

	int nSendNull = 1;
	if ( m_nTB5_NoNull > 0 )
	{
		nSendNull = 0;
	}

	// Pack the Data
    long lBinLength = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData.GetLength() + nSendNull)); 
    Buffer = new BYTE[ lBinLength ];
	memset( Buffer, 0, lBinLength );
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = (BYTE)Station;
	pMsg->func = PTRFUNC_PRINT;
	pMsg->dataLen = (sData.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

    delete[] Buffer;

	return OPOS_SUCCESS;
}

//
// Function:  PrintNormal
// Purpose:  Implementation of the OPOS PrintNormal method
// Inputs:  Station - The printer station
//          Data - the text to be printed.
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::PrintNormal(long Station, LPCTSTR Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::PrintNormal") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;
    CmBString bstrData = Data;
	CString sASCII = bstrData.GetCString();
	CString sData = m_pTBService->ConvertASCII( sASCII );

	int nSendNull = 1;
	if ( m_nTB5_NoNull > 0 )
	{
		nSendNull = 0;
	}

	// Pack the Data
    long lBinLength = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData.GetLength() + nSendNull));
    Buffer = new BYTE[lBinLength];
	memset( Buffer, 0, lBinLength);
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = (BYTE)Station;
	pMsg->func = PTRFUNC_PRINT;
	pMsg->dataLen = (sData.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( bAsync && !m_bTransactionMode )
	{
		// Need separate thread to handle Async notification
		// Set outputId
		m_pTBService->IncrementOutputId();
		// Initiate thread
		AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
	}
	
    delete[] Buffer;

	return OPOS_SUCCESS;
}

//
// Function:  PrintImmediate
// Purpose:  Implementation of the OPOS PrintImmediate method
// Inputs:  Stations - The printer stations
//          Data1 - the text to be printed to station 1
//          Data2 - the text to be printed to station 2
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::PrintTwoNormal(long Stations, LPCTSTR Data1, LPCTSTR Data2)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::PrintTwoNormal") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;
    CmBString bstrData1 = Data1;
	CString sASCII = bstrData1.GetCString();
	CString sData1 = m_pTBService->ConvertASCII( sASCII );
    CmBString bstrData2 = Data2;
	sASCII = bstrData2.GetCString();
	CString sData2 = m_pTBService->ConvertASCII( sASCII );
	BYTE	Unit1;
	BYTE	Unit2;

	int nSendNull = 1;
	if ( m_nTB5_NoNull > 0 )
	{
		nSendNull = 0;
	}

	// Pack the Data
	switch ( Stations )
	{
	case PTR_S_JOURNAL_RECEIPT:
		Unit1 = PTR_S_JOURNAL;
		Unit2 = PTR_S_RECEIPT;
		break;

	case PTR_S_JOURNAL_SLIP:
		Unit1 = PTR_S_JOURNAL;
		Unit2 = PTR_S_SLIP;
		break;

	case PTR_S_RECEIPT_SLIP:
		Unit1 = PTR_S_RECEIPT;
		Unit2 = PTR_S_SLIP;
		break;

	default:
		return OPOS_E_ILLEGAL;
	}

    if ( sData1.GetLength() > sData2.GetLength() )
    {
        lDataLen = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData1.GetLength() + nSendNull));
        Buffer = new BYTE[ lDataLen ];
    }
    else
    {
        lDataLen = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData2.GetLength() + nSendNull));
        Buffer = new BYTE[ lDataLen ];
    }
	memset( Buffer, 0, lDataLen );
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;

	pMsg->hdr.unit = Unit1;
	pMsg->func = PTRFUNC_PRINT;
	pMsg->dataLen = (sData1.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData1, sData1.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );
    memset( Buffer, 0, lDataLen );

    pMsg->hdr.unit = Unit2;
	pMsg->dataLen = (sData2.GetLength() + nSendNull)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData2, sData2.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	this->NotifyTB( Buffer, lDataLen );

	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( bAsync && !m_bTransactionMode )
	{
		// Need separate thread to handle Async notification
		// Set outputId
		m_pTBService->IncrementOutputId();
		// Initiate thread
		AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
	}
	
    delete[] Buffer;

	return OPOS_SUCCESS;
}

//
// Function:  RotatePrint
// Purpose:  Implementation of the OPOS RotatePrint method
// Inputs:  Station - The printer station
//          Rotation - direction of rotation.
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::RotatePrint(long Station, long Rotation)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::RotatePrint") );
	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( (Rotation == PTR_RP_NORMAL) && m_bRotateMode )
	{
		if ( bAsync && !m_bTransactionMode )
		{
			// Need separate thread to handle Async notification
			// Set outputId
			m_pTBService->IncrementOutputId();
			// Initiate thread
			AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
		}
		m_bRotateMode = FALSE;
	}
	else
	{
		if ( (Rotation != PTR_RP_NORMAL) &&
			 (Rotation != PTR_RP_ROTATE180) )
		{
			m_bRotateMode = TRUE;
		}
	}

	return OPOS_SUCCESS;
}

//
// Function:  SetBitmap
// Purpose:  Implementation of the OPOS SetBitmap method
// Inputs:  BitmapNumber - number to be assigned to this bitmap
//          Station - the printer station
//          FileName - Name of Windows bitmap file
//          Width - printed width of the bitmap
//          Alignment - placement of the bitmap
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::SetBitmap(long BitmapNumber, long Station, LPCTSTR FileName, long Width, long Alignment)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::SetBitmap") );
	long lResult;

    if( !m_bPrinterMultiBitmapEnabled )
    {
	    if ( (BitmapNumber != 1) && (BitmapNumber != 2) )
	    {
            m_cmDc.DCPrintf(TRACE_ALWAYS, 
                _T("Invalid Bitmap index of %d specified. Expected value ")
                _T("1 or 2 since MultiBitmap support not enabled."), BitmapNumber);
		    return OPOS_E_ILLEGAL;
	    }
    }

    if ( (Station != PTR_S_RECEIPT) && (Station != PTR_S_SLIP) )
	{
        m_cmDc.DCPrintf( TRACE_ALWAYS, 
                _T("Invalid Station value of %d specified."), Station);
		return OPOS_E_ILLEGAL;
	}

    CPrinterBitmapMap * pBitmapMap = 
                    (Station == PTR_S_RECEIPT) ? &m_receiptBitmaps : &m_slipBitmaps;
    
    // If no bitmap exists in that slot, create one
    CPrinterBitmap * pBitmap;
    if( !pBitmapMap->Lookup(BitmapNumber, pBitmap) )
    {
        pBitmap = new CPrinterBitmap;
        pBitmap->m_strFileName.Empty();
        pBitmapMap->SetAt(BitmapNumber, pBitmap);
        pBitmapMap->Lookup(BitmapNumber, pBitmap);
    }


    CmBString bstrFileName = FileName;
    CString csFileName = bstrFileName.GetCString();
	if ( csFileName.GetLength() == 0 )
	{
		pBitmap->m_strFileName.Empty();
	}
	else
	{
		pBitmap->m_strFileName = csFileName;
	}
	pBitmap->m_lStation = Station;
	pBitmap->m_lWidth = Width;
	pBitmap->m_lAlignment = Alignment;
    pBitmap->m_BitmapNumber = m_bPrinterMultiBitmapEnabled ? BitmapNumber : 1;
	lResult = this->NotifyTBBitmap( pBitmap );
	
	return lResult;
}

// 
// Function:  NotifyTBBitmap
// Purpose:  Sends a SetBitmap message to the TB
// Inputs: Bitmap - data defining the bitmap.
// Outputs:  OPOS_SUCCESS
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::NotifyTBBitmap( CPrinterBitmap* Bitmap )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::NotifyTBBitmap") );
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = PTR_DATA_OFFSET + ((Bitmap->m_strFileName.GetLength() + 1)*sizeof(TCHAR));

	// Pack the Data
    Buffer = new BYTE[lDataLen];
	memset( Buffer, 0, lDataLen );
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = (BYTE)(Bitmap->m_lStation);
	pMsg->func = PTRFUNC_SETBITMAP;
	pMsg->funcArg = Bitmap->m_BitmapNumber;
	if ( Bitmap->m_lWidth == PTR_BM_ASIS )
	{
		pMsg->width = Bitmap->m_lWidth;
	}
	else
	{
		pMsg->width = m_pTBService->ConvertMeasure( FALSE, Bitmap->m_lStation, Bitmap->m_lWidth );
	}
	if ( (Bitmap->m_lAlignment == PTR_BM_LEFT) ||
		(Bitmap->m_lAlignment == PTR_BM_CENTER) ||
		 (Bitmap->m_lAlignment == PTR_BM_RIGHT) )
	{
		pMsg->alignment = Bitmap->m_lAlignment;
	}
	else
	{
		pMsg->alignment = m_pTBService->ConvertMeasure( FALSE, Bitmap->m_lStation, Bitmap->m_lAlignment );
	}
	pMsg->dataLen = (Bitmap->m_strFileName.GetLength() + 1) * sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)Bitmap->m_strFileName, Bitmap->m_strFileName.GetLength() * sizeof(TCHAR) );

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

    delete[] Buffer;

	return OPOS_SUCCESS;
}

//
// Function:  SetLogo
// Purpose:  Implementation of the OPOS SetLogo method
// Inputs:  Location - The location for the logo
//          Data - the text for the logo
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  TB is always notified as if this message came from the RECEIPT
//               printer.  It actaully applies to all stations on the device.
// Comments:  None
//
long CPOSPrinter::SetLogo(long Location, LPCTSTR Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::SetLogo") );

	if ( (Location != PTR_L_TOP) && (Location != PTR_L_BOTTOM) )
	{
		return OPOS_E_ILLEGAL;
	}

    CmBString bstrData = Data;
	CString sASCII = bstrData.GetCString();
	CString sData = m_pTBService->ConvertASCII( sASCII );

    if ( Location == PTR_L_TOP )
	{
		m_strTopLogo = sData;
	}
	else
	{
		m_strBottomLogo = sData;
	}
	
	long lResult = this->NotifyTBLogo( Location, (LPCTSTR)sData );

	return lResult;
}

// 
// Function:  NotifyTBLogo
// Purpose:  Sends a SetLogo message to the TB
// Inputs: Location - the location for the logo
//         strData - data defining the logo.
// Outputs:  OPOS_SUCCESS
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::NotifyTBLogo( long Location, LPCTSTR strData )
{
	// Notify the TB
	BYTE*	Buffer;
	long lDataLen = 0;
	CString sData = strData;

	// Pack the Data
    long lBinLength = PTR_DATA_OFFSET + sizeof(TCHAR)*(sData.GetLength() + 1);
    Buffer = new BYTE[lBinLength];
	memset( Buffer, 0, lBinLength);
	OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
	pMsg->hdr.unit = PTR_S_RECEIPT;
	pMsg->func = PTRFUNC_SETLOGO;
	pMsg->funcArg = Location;
	pMsg->dataLen = (sData.GetLength() + 1)*sizeof(TCHAR);
	memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
	lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

	// Send the data to the TB
	this->NotifyTB( Buffer, lDataLen );

    delete[] Buffer;
	return OPOS_SUCCESS;
}

//
// Function:  TransactionPrint
// Purpose:  Implementation of the OPOS TransactionPrint method
// Inputs:  Station - The printer station
//          Control - Transaction control
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::TransactionPrint(long Station, long Control)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::TransactionPrint") );
	BOOL bAsync;

	bAsync = m_pTBService->IsAsync();

	if ( (Control == PTR_TP_NORMAL) && m_bTransactionMode )
	{
		if ( bAsync )
		{
			// Need separate thread to handle Async notification
			// Set outputId
			m_pTBService->IncrementOutputId();
			// Initiate thread
			AfxBeginThread( CPOSPrinter::OutputCompleteProc, (void*) this, 0,0,0,NULL );
		}
		m_bTransactionMode = FALSE;
	}
	else
	{
		if ( Control == PTR_TP_TRANSACTION )
		{
			m_bTransactionMode = TRUE;
		}
	}

	return OPOS_SUCCESS;
}

//
// Function:  ValidateData
// Purpose:  Implementation of the OPOS ValidateData method
// Inputs:  Station - The printer station
//          Data - the text to be validated
// Outputs:  Always OPOS_SUCCESS to keep the calling app happy
// Assumptions:  None
// Comments:  None
//
long CPOSPrinter::ValidateData(long Station, LPCTSTR Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::ValidateData") );
	// Always return a validation success to keep the App happy.
	return OPOS_SUCCESS;
}

//
// Function:  OutputCompleteProc
// Purpose: Thread to handle the setting of the output complete event after
//          a configured delay for async IO
// Inputs: lpParam - pointer to the CPOSPrinter instance that started the thread.
// Outptus:   0
// Assumptions:  None
// Comments:  None
//
UINT CPOSPrinter::OutputCompleteProc( void* lpParam )
{
	CPOSPrinter* pThis = (CPOSPrinter*) lpParam;
    CDataCapEntryExit DCE( pThis->m_cmDc, _T("CPOSPrinter::OutputCompleteProc") );

	long lOutputId = pThis->m_pTBService->GetOutputId();

	Sleep( pThis->m_lOutputDelay );

	pThis->OnOutput( lOutputId );

	return 0;
}

// 
// Function:  NotifyTB
// Purpose:  Sends a message to the TB
// Inputs: pBuffer - buffer containing the message
//         lBufferLen - length of the data to be sent
// Outputs:  OPOS_SUCCESS
// Assumptions:  None
// Comments:  None
// 
long CPOSPrinter::NotifyTB( BYTE* pBuffer, long lBufferLen )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::NotifyTB") );

   int iMessageSpecVersion = CSocketForTBDevice::GetSpecVersion();

   if (CSocketForTBDevice::GetSpecVersion() > 1) 
   {
      OPOSIO_PRINTER	 *pOld= (OPOSIO_PRINTER *) pBuffer;

      CPrinterMsg msg(TB_DEV_PRINTERRECEIPT);
      msg.Function((BYTE)pOld->func);
      msg.Height((unsigned short)pOld->height);
      msg.Width((unsigned short)pOld->width);
      msg.Alignment((BYTE)pOld->alignment);
      msg.BarcodeCharacterPrint( (BYTE)pOld->textPosition );
      msg.StationId( (BYTE)pOld->hdr.unit );
      msg.BarcodeType( (BYTE)pOld->funcArg );
      msg.BitmapLogoNumber( (BYTE) pOld->funcArg );
      CmBString bstrData = (LPCTSTR)&pOld->Data;
	  CString sData = bstrData.GetCString();
      
      msg.Data( sData);
      
	  msg.SetMsgVersion(m_HdrVersion);
      
      int iMsgSize;
      BYTE *pBuf = msg.Serialize( iMsgSize );


	   int iSent = Send(pBuf, iMsgSize);
      delete [] pBuf;
	   if (iSent == SOCKET_ERROR)
      {
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
	   }
   }
   else
   {
      OPOSIO_PRINTER* pMsg2 = (OPOSIO_PRINTER*) pBuffer;
	   pMsg2->hdr.deviceID = TBMSG_PRINTER;
	   pMsg2->hdr.port = m_nPort;

	   long SocketMsgLen = lBufferLen + sizeof(tSpecHeaderV01);
	   BYTE *SocketBuffer= new BYTE [SocketMsgLen];

	   OPOSIO_PRINTER_SOCKET* pMsg = (OPOSIO_PRINTER_SOCKET *) SocketBuffer;
	   pMsg->Spec.messageID = TBMSG_PRINTER;
	   pMsg->Spec.nLen = SocketMsgLen;
	   pMsg->Spec.nVersion = 1;
	   
	   memmove( pMsg->Data, pMsg2, lBufferLen );

      if (SOCKET_ERROR == Send((BYTE *)SocketBuffer, SocketMsgLen)) {
		   m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
      }
      delete [] SocketBuffer;
   }
   return OPOS_SUCCESS;
}

//
// Function:  ProcessRawData
// Purpose:  Process messages sent from the TB
// Inputs:  pData - buffer containing the message data
//          dwByteLen - the length of the message data
// Outputs:  None
// Assumptions:  the only expected message is the POLL message
// Comments: None
//
void CPOSPrinter::ProcessRawData( void* pData, DWORD dwByteLen )
{
    BYTE byMsgId;

    if( CSocketForTBDevice::GetSpecVersion() > 1 )
    {
        CPrinterMsg prMsg;
		prMsg.Parse((BYTE*)pData);
        byMsgId = prMsg.GetTBMsgId();
    }
    else
    {
        OPOSIO_HDR* pHdr = (OPOSIO_HDR*) pData;
        byMsgId = pHdr->deviceID;
    }

    if( byMsgId == 0xFF )
    {
		// This is a poll message
		
        // Send Bitmap Settings for Receipt printer
        int iKey;
        CPrinterBitmap * pBitmap = NULL;
        POSITION pos = m_receiptBitmaps.GetStartPosition();
        while( pos )
        {
            m_receiptBitmaps.GetNextAssoc(pos, iKey, pBitmap);
            if( !m_bPrinterMultiBitmapEnabled && iKey != 1 && iKey != 2 )
            {
                m_cmDc.DCPrintf(TRACE_ALWAYS, 
                    _T("Invalid Bitmap index of %d specified. Expected value ")
                    _T("1 or 2 since MultiBitmap support is not enabled."), iKey);
            }
            else
            {
                if( !pBitmap->m_strFileName.IsEmpty() )
                {
                    this->NotifyTBBitmap(pBitmap);
                }
            }
        }

        pBitmap = NULL;
        // Send Bitmap Settings for Slip printer
        pos = m_slipBitmaps.GetStartPosition();
        while( pos )
        {
            m_slipBitmaps.GetNextAssoc(pos, iKey, pBitmap);
            if( !m_bPrinterMultiBitmapEnabled && iKey != 1 && iKey != 2 )
            {
                m_cmDc.DCPrintf(TRACE_ALWAYS, 
                    _T("Invalid Bitmap index of %d specified. Expected value ")
                    _T("1 or 2 since MultiBitmap support is not enabled."), iKey);
            }
            else
            {
                if( !pBitmap->m_strFileName.IsEmpty() )
                {
                    this->NotifyTBBitmap(pBitmap);
                }
            }
        }
		
        // Send Logo Settings
		if ( !m_strTopLogo.IsEmpty() )
		{
			this->NotifyTBLogo( PTR_L_TOP, (LPCTSTR)m_strTopLogo );
		}
		if ( !m_strBottomLogo.IsEmpty() )
		{
			this->NotifyTBLogo( PTR_L_TOP, (LPCTSTR)m_strBottomLogo );
		}
	}
	else
	{
		m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Printer Got ProcessRawData 0x%0X"), byMsgId );
	}
}




void CPOSPrinter::ReportPropertyNumber( long lProperty, long lValue )
{
	CDataCapEntryExit DCE( m_cmDc, _T("CPOSPrinter::ReportPropertyNumber") );
	if ( CheckPropertyNumber( lProperty ) )
	{
		// Notify the TB
		BYTE*	Buffer;
		long lDataLen = 0;
		CString sData;
		sData.Format(_T("%d,%d"), lProperty, lValue );

		// Pack the Data
		long lBinLength = PTR_DATA_OFFSET + (sizeof(TCHAR)*(sData.GetLength()));
		Buffer = new BYTE[lBinLength];
		memset( Buffer, 0, lBinLength);
		OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
		pMsg->func = PTRFUNC_PROPCHANGE;
		pMsg->dataLen = (sData.GetLength())*sizeof(TCHAR);
		memmove( pMsg->Data, (LPCTSTR)sData, sData.GetLength()*sizeof(TCHAR) );
		lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

		// Send the data to the TB
		this->NotifyTB( Buffer, lDataLen );

		delete[] Buffer;
	}
}

// COposDevice::DirectIO
//
// Purpose: Common DirectIO code - log an event


long CPOSPrinter::DirectIO(
   long     nCommand,
   long*    plData,
   CString& sData)
{
	CDataCapEntryExit DCE( m_cmDc, _T("CTBDevice::DirectIO") );

	CmBString bstrData = sData;
	CString sASCII = bstrData.GetCString();
    
    if( !plData )
    {
	    m_cmDc.DoDCDataPrintf( TRACE_ALWAYS, sASCII, (sASCII.GetLength()*sizeof(TCHAR)), 
		        _T("*ERROR: DirectIO Command: 0x%X, plData: << null pointer >>.  Ignoring command."), nCommand);
        return SetRC( OPOS_SUCCESS );
    }

	m_cmDc.DoDCDataPrintf( TRACE_ALWAYS, sASCII, (sASCII.GetLength()*sizeof(TCHAR)), 
		_T("*DirectIO Command: 0x%X, plData: 0x%X"), nCommand, *plData );

	if ( m_nDirectIO > 0 )
	{
		// Pack the Data
		long lBinLength = PTR_DATA_OFFSET + (3*sizeof(long) + sASCII.GetLength()*sizeof(TCHAR));
		BYTE* Buffer = new BYTE[lBinLength];
		memset( Buffer, 0, lBinLength);
		OPOSIO_PRINTER*	pMsg = (OPOSIO_PRINTER*) Buffer;
		pMsg->func = PTRFUNC_DIRECTIO;

		OPOSIO_DIRECTIO* pDio = (OPOSIO_DIRECTIO*)pMsg->Data;

		pDio->command = nCommand;
		pDio->lData = *plData;
		pDio->dataLen = sASCII.GetLength()*sizeof(TCHAR);
		memcpy( pDio->Data, (LPCTSTR)sASCII, pDio->dataLen );

		pMsg->dataLen = (3*sizeof(long) + sASCII.GetLength()*sizeof(TCHAR));
		long lDataLen = PTR_DATA_OFFSET + pMsg->dataLen;

		// Send the data to the TB
		this->NotifyTB( Buffer, lDataLen );
	}


	return SetRC( OPOS_SUCCESS );
}

