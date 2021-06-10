// FiscalPrinter.h: interface for the CPOSPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FISCALPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
#define AFX_FISCALPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"
#include "FiscalPrinterMsg.h"

class CFiscalPrinter : public CTBDevice  
{
public:
	CFiscalPrinter(int nPort, int nId,  CmDataCapture& cmDc );
	virtual ~CFiscalPrinter();
	virtual CString GetDeviceModel( void ) const { return CString("FiscalPrinter");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );
    virtual long DirectIO( long nCommand, long* plData, CString& sData );

	// Operations
	long BeginFiscalDocument(long DocumentAmount);
	long BeginInsertion(long Timeout);
	long BeginFiscalReceipt(BOOL PrintHeader);
	long BeginFixedOutput(long Station, long DocumentType);
	long BeginItemList(long VatID);
	long BeginNonFiscal();
	long BeginTraining();
	long ClearError();
	long BeginRemoval(long Timeout);
	long EndFiscalDocument();
	long EndFiscalReceipt(BOOL PrintHeader);
	long EndFixedOutput();
	long EndInsertion();
	long EndItemList();
	long EndNonFiscal();
	long EndRemoval();
	long EndTraining();
	long GetData(long DataItem, long FAR* OptArgs, BSTR FAR* Data);
	long GetDate(BSTR FAR* Date);
	long GetTotalizer(long VatID, long OptArgs, BSTR FAR* Data);
	long GetVatEntry(long VatID, long OptArgs, long FAR* VatRate);
	long PrintDuplicateReceipt();
	long PrintNormal(long Station, LPCTSTR Data);
	long PrintFiscalDocumentLine(LPCTSTR DocumentLine);
	long PrintFixedOutput(long DocumentType, long LineNumber, LPCTSTR Data);
	long PrintPowerLossReport();
	long PrintRecItem(LPCTSTR Description, CURRENCY Price, long Quantity, long VatInfo, CURRENCY UnitPrice, LPCTSTR UnitName);
	long PrintPeriodicTotalsReport(LPCTSTR Date1, LPCTSTR Date2);
	long PrintRecItemAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount, long VatInfo);
	long PrintRecMessage(LPCTSTR Message);
	long PrintRecNotPaid(LPCTSTR Description, CURRENCY Amount);
	long PrintRecSubtotal(CURRENCY Amount);
	long PrintRecRefund(LPCTSTR Description, CURRENCY Amount, long VatInfo);
	long PrintRecTotal(CURRENCY Total, CURRENCY Payment, LPCTSTR Description);
	long PrintRecSubtotalAdjustment(long AdjustmentType, LPCTSTR Description, CURRENCY Amount);
	long PrintRecVoid(LPCTSTR Description);
	long PrintRecVoidItem(LPCTSTR Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo);
	long PrintReport(long ReportType, LPCTSTR StartNum, LPCTSTR EndNum);
	long PrintXReport();
	long ResetPrinter();
	long SetDate(LPCTSTR Date);
	long PrintZReport();
	long SetStoreFiscalID(LPCTSTR ID);
	long SetTrailerLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth);
	long SetVatTable();
	long SetHeaderLine(long LineNumber, LPCTSTR Text, BOOL DoubleWidth);
	long SetPOSID(LPCTSTR POSID, LPCTSTR CashierID);
	long SetVatValue(long VatID, LPCTSTR VatValue);
	long VerifyItem(LPCTSTR ItemName, long VatID);
	BOOL CheckEntry(short nOptions, BSTR FAR* pnStn);
    //long GetPropertyNumber( long lPropIndex );

private:
    long NotifyTB( CFiscalPrinterMsg* pMessage);
    long WaitForResponse( );

    HANDLE m_hFiscalMessage;
    DWORD  m_dwRespTimeout;
    CFiscalPrinterMsg* m_RecMsg;
    CURRENCY m_RemainingPayment;


};  //lint !e1712 supress "Default contructor is NEVER used!"

#endif // !defined(AFX_FISCALPRINTER_H__6C23E762_E548_4E40_9B31_F06F6ED237DF__INCLUDED_)
