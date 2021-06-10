// FiscalPrinter.cpp: implementation of the CFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiscalPrinter.h"
#include "TBService.h"
#include "CMBstring.h"

#include "SocketForTBDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFiscalPrinter::CFiscalPrinter(int nPort, int nId,  CmDataCapture& cmDc )
					   :CTBDevice(OPOS_CLASSKEY_FPTR,nId,nPort, cmDc)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::CFiscalPrinter") );
    m_hFiscalMessage = NULL;
    m_dwRespTimeout = 5000;
    m_RecMsg = NULL;
    m_RemainingPayment.int64 = 0;
}

CFiscalPrinter::~CFiscalPrinter()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::~CFiscalPrinter") );
    if ( NULL != m_hFiscalMessage )
    {
        CloseHandle( m_hFiscalMessage );
        m_hFiscalMessage = NULL;
    }
    if ( NULL != m_RecMsg )
    {
        delete m_RecMsg;
        m_RecMsg = NULL;
    }
}

//
// Function:  Open
// Purpose:  Opens the FiscalPrinter and initializes operations based on the 
//           contents of the registry key for the device profile.
// Inputs:  pService - pointer to CTBService object that handles the service
//                     operations for the device.
//          oRegistry - Registry class for accessing profile properties.
// Outputs:  Return code from Base Class
// Assumptions:  None
// Comments:  None
// 
long CFiscalPrinter::Open( CTBService* pService, COposRegistry& oRegistry )
{
    long lResult = OPOS_SUCCESS;

    // Initialize Fiscal Printer (Model 2214)
    bool bEuroStatus = false;
    bEuroStatus = oRegistry.ReadBool( _T("EuroStatus"), 0, FALSE )?true:false;

    m_dwRespTimeout = (DWORD)oRegistry.ReadInt( _T("RespTimeout"), 0, INFINITE);

    if ( pService->InitializeFiscalPrinter( bEuroStatus) != OPOS_SUCCESS)
    {
        return OPOS_E_FAILURE;
    }

    // Create the message event
    m_hFiscalMessage = CreateEvent(NULL, FALSE, FALSE, _T("FiscalPrinterMsg") );
    if ( NULL == m_hFiscalMessage )
    {
        lResult = OPOS_E_FAILURE;
    }
    else
    {
        lResult = CTBDevice::Open( pService, oRegistry );
    }
    return lResult;
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
void CFiscalPrinter::Close( CTBService* pService )
{

    if ( m_hFiscalMessage )
    {
        CloseHandle( m_hFiscalMessage );
        m_hFiscalMessage = NULL;
    }
	CTBDevice::Close( pService);
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
void CFiscalPrinter::ProcessRawData( void* pData, DWORD dwByteLen )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::ProcessRawData") );
    if (CSocketForTBDevice::GetSpecVersion() > 1) 
    {
        m_RecMsg = new CFiscalPrinterMsg();
        m_RecMsg->Parse((BYTE*)pData);
        SetEvent( m_hFiscalMessage );
    }
    else
    {
        m_cmDc.DCPrintf(TRACE_ALWAYS, _T("Invalid SPEC Version"));
    }
}

long CFiscalPrinter::BeginFiscalDocument(long DocumentAmount)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::BeginInsertion(long Timeout)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::BeginFiscalReceipt(BOOL PrintHeader)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::BeginFiscalReceipt") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetBeginFiscalReceipt( PrintHeader? true : false );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        if ( !this->m_pTBService->GetPropertyNumber( PIDXFptr_TrainingModeActive) )
        {
            this->m_pTBService->SetFiscalProperty( PIDXFptr_DayOpened, true );
        }
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::BeginFixedOutput(long Station, long DocumentType)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::BeginItemList(long VatID)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::BeginNonFiscal()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::BeginNonFiscal") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetBeginNonFiscal( );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        if ( !this->m_pTBService->GetPropertyNumber( PIDXFptr_TrainingModeActive) )
        {
            this->m_pTBService->SetFiscalProperty( PIDXFptr_DayOpened, true );
        }
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_NONFISCAL );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::BeginTraining()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::BeginTraining") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetBeginTraining( );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        this->m_pTBService->SetFiscalProperty( PIDXFptr_TrainingModeActive, true );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::ClearError()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::BeginRemoval(long Timeout)
{
    return OPOS_E_NOSERVICE;
}

long CFiscalPrinter::EndFiscalDocument()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::EndFiscalReceipt(BOOL PrintHeader)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::EndFiscalReceipt") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetEndFiscalReceipt( PrintHeader ? true : false );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_MONITOR );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::EndFixedOutput()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::EndInsertion()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::EndItemList()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::EndNonFiscal()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::EndNonFiscal") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetEndNonFiscal( );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_MONITOR );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::EndRemoval()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::EndTraining()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::EndTraining") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetEndTraining( );

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        this->m_pTBService->SetFiscalProperty(PIDXFptr_TrainingModeActive, false);
    }

    return SetRC( lResult );
}

long CFiscalPrinter::GetData(long DataItem, long FAR* OptArgs, BSTR FAR* Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::GetData") );

    // Created a Fiscal Printer Message 
    CFiscalPrinterMsg  FPMsg;
    CString csEmpty;
    csEmpty.Empty();

    FPMsg.Set_GetData(DataItem );

    // Send it to the TB
    long lResult = this->NotifyTB( &FPMsg );

    if ( (OPOS_SUCCESS == lResult) && (NULL != m_RecMsg) )
    {
        // Process the response
        long nRetItem;
        CString csResp;

        if ( m_RecMsg->Get_GetData_Return( nRetItem, csResp ) )
        {
            if ( nRetItem == DataItem )
            {
        		*Data = csResp.AllocSysString();
            }
            else
            {
                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("DataItem in response does not match!"));
                lResult = OPOS_E_FAILURE;
            }
        }
        else
        {
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Response does not match request!"));
            lResult = OPOS_E_FAILURE;
        }

        // Cleanup the message
        delete this->m_RecMsg;
        m_RecMsg = NULL;
    }

    return SetRC( lResult );
}

long CFiscalPrinter::GetDate(BSTR FAR* Date)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::GetDate") );

    // Created a Fiscal Printer Message 
    CFiscalPrinterMsg  FPMsg;
    CString csEmpty;
    csEmpty.Empty();

    FPMsg.Set_GetDate();

    // Send it to the TB
    long lResult = this->NotifyTB( &FPMsg );

    if ( (OPOS_SUCCESS == lResult) && (NULL != m_RecMsg) )
    {
        // Process the response
        CString csResp;

        if ( m_RecMsg->Get_GetDate_Return( csResp ) )
        {
        	*Date = csResp.AllocSysString();
        }
        else
        {
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Response does not match request!"));
            lResult = OPOS_E_FAILURE;
        }

        // Cleanup the message
        delete this->m_RecMsg;
        m_RecMsg = NULL;
    }

    return SetRC( lResult );
}


long CFiscalPrinter::GetTotalizer(long VatID, long OptArgs, BSTR FAR* Data)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::GetVatEntry(long VatID, long OptArgs, long FAR* VatRate)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintDuplicateReceipt()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintNormal(long Station, LPCTSTR Data)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintNormal") );
    CmBString bstrData = Data;
	CString sData = bstrData.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintNormal( Station, sData );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintFiscalDocumentLine(LPCTSTR DocumentLine)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintFixedOutput(long DocumentType, long LineNumber, LPCTSTR Data)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintPowerLossReport()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintRecItem(LPCTSTR Description, CURRENCY Price, long Quantity, long VatInfo, CURRENCY UnitPrice, LPCTSTR UnitName)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecItem") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();
    CmBString bstrUnitName = UnitName;
	CString sUnitName = bstrUnitName.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecItem( sDescription, Price, Quantity, VatInfo, UnitPrice, sUnitName );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintPeriodicTotalsReport(LPCTSTR Date1, LPCTSTR Date2)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintRecItemAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount, long VatInfo)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecItemAdjustment") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecItemAdjustment( AdjustmentType, sDescription, Amount, VatInfo );

    long lResult = NotifyTB( &FPMsg );

   	if( OPOS_SUCCESS == lResult )
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecMessage(LPCTSTR Message)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecMessage") );
    CmBString bstrMessage = Message;
	CString sMessage = bstrMessage.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecMessage( sMessage );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecNotPaid(LPCTSTR Description, CURRENCY Amount)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecNotPaid") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecNotPaid( sDescription, Amount );

    long lResult = NotifyTB( &FPMsg );

	if( lResult == OPOS_SUCCESS )
	{
		m_RemainingPayment.Hi = m_RemainingPayment.Hi - Amount.Hi; 
	}

	if ( m_RemainingPayment.Hi == 0)
	{
		if( lResult == OPOS_SUCCESS )
		{
            this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_ENDING );
			m_RemainingPayment.Hi = 0;
		}
	}
	else
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_TOTAL );
	}

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecSubtotal(CURRENCY Amount)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecSubtotal") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecSubtotal( Amount );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecRefund(LPCTSTR Description, CURRENCY Amount, long VatInfo)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecRefund") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecRefund( sDescription, Amount, VatInfo );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecTotal(CURRENCY Total, CURRENCY Payment, LPCTSTR Description)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecTotal") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecTotal( Total, Payment, sDescription );

    long lResult = NotifyTB( &FPMsg );

    // The following code was extracted from the OPOS Source
    if ( OPOS_SUCCESS == lResult )
    {
        long lState = this->m_pTBService->GetPropertyNumber( PIDXFptr_PrinterState );
        if ( FPTR_PS_FISCAL_RECEIPT == lState )
        {
            if ((Payment.Hi == 0) && (Total.Hi > 0))
            {
			    m_RemainingPayment.Hi = Total.Hi;
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_TOTAL);
			    return SetRC(lResult);
		    }

		    if(Payment.Hi < Total.Hi ) 
		    {
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_TOTAL);
			    m_RemainingPayment.Hi = Total.Hi-Payment.Hi;
			    return SetRC(lResult);
		    }

		    if ( Payment.Hi >= Total.Hi)
		    {
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_ENDING);
			    return SetRC(lResult);
		    }
        }
        if ( FPTR_PS_FISCAL_RECEIPT_TOTAL == lState )
        {
		    if (Payment.Hi == 0)
		    {
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_TOTAL);
			    return SetRC(lResult);
		    }

			m_RemainingPayment.Hi = m_RemainingPayment.Hi-Payment.Hi;

		    if ( (m_RemainingPayment.Hi < 0) || (m_RemainingPayment.Hi == 0))
		    {
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_ENDING);
				m_RemainingPayment.Hi = 0;
			    return SetRC(lResult);
		    }
            else
            {
                this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT_TOTAL);
                return SetRC(lResult);
            }
        }
    }

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecSubtotalAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecSubtotalAdjustment") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecSubtotalAdjustment(AdjustmentType, sDescription, Amount);

    long lResult = NotifyTB( &FPMsg );

   	if( OPOS_SUCCESS == lResult )
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecVoid(LPCTSTR Description)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecVoid") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecVoid(sDescription );

    long lResult = NotifyTB( &FPMsg );

   	if( OPOS_SUCCESS == lResult )
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_MONITOR );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::PrintRecVoidItem(LPCTSTR Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintRecVoidItem") );
    CmBString bstrDescription = Description;
	CString sDescription = bstrDescription.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintRecVoidItem(sDescription, Amount, Quantity, AdjustmentType, Adjustment, VatInfo);

    long lResult = NotifyTB( &FPMsg );

   	if( OPOS_SUCCESS == lResult )
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_FISCAL_RECEIPT );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::PrintReport(long ReportType, LPCTSTR StartNum, LPCTSTR EndNum)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::PrintXReport()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::ResetPrinter()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::ResetPrinter") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.Set_ResetPrinter();

    long lResult = NotifyTB( &FPMsg );

   	if( OPOS_SUCCESS == lResult )
	{
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_MONITOR );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::SetDate(LPCTSTR Date)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::SetDate") );
    CmBString bstrDate = Date;
	CString sDate = bstrDate.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.Set_SetDate( sDate );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::PrintZReport()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::PrintZReport") );

    CFiscalPrinterMsg FPMsg;

    FPMsg.SetPrintZReport();

    long lResult = NotifyTB( &FPMsg );

    if ( OPOS_SUCCESS == lResult )
    {
        this->m_pTBService->SetFiscalProperty( PIDXFptr_DayOpened, false );
        this->m_pTBService->SetFiscalProperty( PIDXFptr_PrinterState, FPTR_PS_MONITOR );
    }

    return SetRC( lResult );
}

long CFiscalPrinter::SetStoreFiscalID(LPCTSTR ID)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::SetTrailerLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::SetVatTable()
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::SetHeaderLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::SetHeaderLine") );
    CmBString bstrText = Text;
	CString sText = bstrText.GetCString();

    CFiscalPrinterMsg FPMsg;

    FPMsg.Set_SetHeaderLine( LineNumber, sText, DoubleWidth ? true : false );

    long lResult = NotifyTB( &FPMsg );

    return SetRC( lResult );
}

long CFiscalPrinter::SetPOSID(LPCTSTR POSID, LPCTSTR CashierID)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::SetVatValue(long VatID, LPCTSTR VatValue)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

long CFiscalPrinter::VerifyItem(LPCTSTR ItemName, long VatID)
{
    // Not supported in this release
    return SetRC( OPOS_E_ILLEGAL );
}

BOOL CFiscalPrinter::CheckEntry(short nOptions, BSTR FAR* pnStn)
{
    return false;
}

long CFiscalPrinter::DirectIO(
   long     nCommand,
   long*    plData,
   CString& sData)
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::DirectIO") );

    // Created a Fiscal Printer Message 
    CFiscalPrinterMsg  msgDirectIO;
    CString csEmpty;
    csEmpty.Empty();

    // plData as Input is either a long or not used
    if ( plData )
    {
        msgDirectIO.SetDirectIO(nCommand, *plData, sData );
    }
    else
    {
        msgDirectIO.SetDirectIO(nCommand, 0, sData );
    }

    // Send it to the TB
    long lResult = this->NotifyTB( &msgDirectIO );

    if ( (OPOS_SUCCESS == lResult) && (NULL != m_RecMsg) )
    {
        // Process the response
        long nRetCmd;
        CString csResp;

        if ( m_RecMsg->GetDirectIO_Return( nRetCmd, csResp ) )
        {
            if ( nRetCmd == nCommand )
            {
                switch( nRetCmd )
                {
                case OPOS_FPTR_DI_RAWDATA:
                    {
                        // pData get the return string
            		    LPTSTR p = csResp.GetBuffer(csResp.GetLength());
		                plData = (long*) p;
                        lResult = OPOS_SUCCESS;
                    }
                    break;

                case OPOS_FPTR_PRN_FISCALREQUEST:
                    // pString gets the return string
		            sData = csResp;
		            lResult = OPOS_SUCCESS;
                    break;

                case OPOS_FPTR_DI_FISCALDOCTYPE:
                case OPOS_FPTR_PRN_FISCALREC:
                case OPOS_FPTR_PRN_OUTFISREC:
                case OPOS_FPTR_DI_LINEDISPLAYON:
                case OPOS_FPTR_DI_LINEDISPLAYOFF:
                default:
                    lResult = OPOS_SUCCESS;
                    break;
                }
            }
            else
            {
                m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Command in response does not match!"));
                lResult = OPOS_E_FAILURE;
            }
        }
        else
        {
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Response does not match request!"));
            lResult = OPOS_E_FAILURE;
        }

        // Cleanup the message
        delete this->m_RecMsg;
        m_RecMsg = NULL;
    }

    return SetRC( lResult );
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
long CFiscalPrinter::NotifyTB( CFiscalPrinterMsg* msg )
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::NotifyTB") );
    long lReturn = OPOS_SUCCESS;

    if (CSocketForTBDevice::GetSpecVersion() > 1) 
    {
        int iMsgSize = 0;
        BYTE *pBuf = msg->Serialize( iMsgSize );

        ResetEvent(m_hFiscalMessage);
        int iSent = Send(pBuf, iMsgSize);
        delete [] pBuf;
        if (iSent == SOCKET_ERROR)
        {
            lReturn = OPOS_E_FAILURE;
            m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed to write to PipeServer Socket"));
        }
        else
        {
            lReturn = WaitForResponse();
        }
    }
    else
    {
        lReturn = OPOS_E_ILLEGAL;
    }
    return lReturn;
}

long CFiscalPrinter::WaitForResponse()
{
    CDataCapEntryExit DCE( m_cmDc, _T("CFiscalPrinter::WaitForResponse") );
    long lResult = OPOS_E_FAILURE;

    DWORD dwWait = WaitForSingleObject( m_hFiscalMessage, m_dwRespTimeout);
    switch ( dwWait )
    {
    case WAIT_OBJECT_0:
        lResult = OPOS_SUCCESS;
        break;

    case WAIT_TIMEOUT:
        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Timed out waiting for response"));
        lResult = OPOS_E_TIMEOUT;
        break;

    default:
        m_cmDc.DCPrintf( TRACE_ALWAYS, _T("Failed waiting for response: dwWait=%d, Err=%d"),
            dwWait, GetLastError());
        lResult = OPOS_E_FAILURE;
        break;
    }

    return lResult;
}