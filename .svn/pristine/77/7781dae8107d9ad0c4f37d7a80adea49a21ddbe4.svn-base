// CustomerMHFiscalPrinter.h: interface for the CCustomerMHFiscalPrinter class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_CustomerMHFiscalPrinter_H__INCLUDED_)
#define AFX_CustomerMHFiscalPrinter_H__INCLUDED_

#include "BaseMHFiscalPrinter.h"
#include "CustomerKeyDefines.h"
#include "SingleInstance.h"
#include "FiscalPrinterMsg.h"

class CCustomerMHFiscalPrinter  : public CBaseMHFiscalPrinter, 
                             public CSingleInstance<CCustomerMHFiscalPrinter>
{
    friend class CSingleInstance<CCustomerMHFiscalPrinter>;

public:
	virtual ~CCustomerMHFiscalPrinter();

protected:
	CCustomerMHFiscalPrinter();

    // Message Handlers
    virtual TBSTATE HandleDirectIO( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleBeginFiscalReceipt( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleEndFiscalReceipt( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecItem( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecTotal( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleBeginNonFiscal( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleEndNonFiscal( CFiscalPrinterMsg* pMsg);
    virtual TBSTATE HandlePrintNormal( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecItemAdjustment( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecMessage( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecSubtotal( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecSubtotalAdjustment( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecVoid( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecVoidItem( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintZReport( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleBeginTraining( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleEndTraining( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleGetData( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleGetDate( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecNotPaid( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandlePrintRecRefund( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleResetPrinter( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleSetDate( CFiscalPrinterMsg* pMsg );
    virtual TBSTATE HandleSetHeaderLine( CFiscalPrinterMsg* pMsg );


private:
    CCustomerMHFiscalPrinter(CCustomerMHFiscalPrinter &);                    // hide copy const
    CCustomerMHFiscalPrinter & operator =(const CCustomerMHFiscalPrinter &); // hide assignment
};

#endif 
