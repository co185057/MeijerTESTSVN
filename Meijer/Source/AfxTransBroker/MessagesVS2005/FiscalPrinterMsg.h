// PrinterMsg.h: interface for the CPrinterMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FISCALPRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_)
#define AFX_FISCALPRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBMsgObj.h"

typedef enum {
   cInvalidFunction,
   cBeginFiscalReceipt,
   cPrintRecItem,
   cPrintRecTotal,
   cEndFiscalReceipt,
   cDirectIO,
   cDirectIO_Return,
   cBeginNonFiscal,
   cEndNonFiscal,
   cPrintNormal,
   cPrintRecItemAdjustment,
   cPrintRecMessage,
   cPrintRecSubtotal,
   cPrintRecSubtotalAdjustment,
   cPrintRecVoid,
   cPrintRecVoidItem,
   cPrintZReport,
   cBeginTraining,
   cEndTraining,
   cGetData,
   cGetData_Return,
   cGetDate,
   cGetDate_Return,
   cPrintRecNotPaid,
   cPrintRecRefund,
   cResetPrinter,
   cSetDate,
   cSetHeaderLine,
   cOutputComplete_Return
} enum_FPFunctions;


class CFiscalPrinterMsg : public CTBMsgObj
{
public:
    CFiscalPrinterMsg(TBDEVICEID devId=TB_DEV_FISCALPRINTER);
    virtual ~CFiscalPrinterMsg();

    virtual BYTE* Serialize(int &byteLength);
    virtual void Parse(BYTE *pBuf);

    // Data Input
    //    Group 1
    void SetDirectIO( long Command, long lInput, const CString &pString );
    void SetDirectIO_Return( long Command, const CString &Response );
    void SetBeginFiscalReceipt( bool PrintHeader );
    void SetEndFiscalReceipt( bool PrintHeader );
    void SetPrintRecItem( const CString &Description, const CURRENCY &Price, long Quantity,
                       long VatInfo, const CURRENCY &UnitPrice, const CString &UnitName);
    void SetPrintRecTotal( const CURRENCY &Total, const CURRENCY &Payment, const CString &Description);

    //    Group 2
	void SetBeginNonFiscal();
	void SetEndNonFiscal();
	void SetPrintNormal(long Station, const CString &Data);
	void SetPrintRecItemAdjustment(long AdjustmentType, const CString &Description, CURRENCY Amount, long VatInfo);
	void SetPrintRecMessage(const CString &Message);
	void SetPrintRecSubtotal(CURRENCY Amount);
	void SetPrintRecSubtotalAdjustment(long AdjustmentType, const CString &Description, CURRENCY Amount);
	void SetPrintRecVoid(const CString &Description);
	void SetPrintRecVoidItem(const CString &Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo);
	void SetPrintZReport();

    //    Group 3
    void SetBeginTraining();
    void SetEndTraining();
    void Set_GetData(long DataItem);
    void Set_GetData_Return(long DataItem, const CString ReturnData );
    void Set_GetDate();
    void Set_GetDate_Return(const CString ReturnDate );
    void SetPrintRecNotPaid(const CString Description, CURRENCY Amount );
    void SetPrintRecRefund(const CString Description, CURRENCY Amount, long VatInfo );
    void Set_ResetPrinter();
    void Set_SetDate(const CString Date);
    void Set_SetHeaderLine(long LineNumber, const CString Text, bool DoubleWidth);

    void Set_OutputComplete_Return();

    // Data Access
    //    Group 1
    enum_FPFunctions GetFunctionCode( ) const;
    bool GetDirectIO( long &Command, long &lInput, CString &pString ) const;
    bool GetDirectIO_Return( long &Command, CString &Response ) const;
    //bool GetGetPropertyNumber( long &lPropertyIndex ) const;
    //bool GetGetPropertyNumber_Return( long &lPropertyValue ) const;
    bool GetBeginFiscalReceipt( bool &PrintHeader ) const;
    bool GetEndFiscalReceipt( bool &PrintHeader ) const;
    bool GetPrintRecItem( CString &Description, CURRENCY &Price, long &Quantity,
                       long &VatInfo, CURRENCY &UnitPrice, CString &UnitName) const;
    bool GetPrintRecTotal( CURRENCY &Total, CURRENCY &Payment, CString &Description) const;

        //    Group 2
	bool GetBeginNonFiscal();
	bool GetEndNonFiscal();
	bool GetPrintNormal(long &Station, CString &Data);
	bool GetPrintRecItemAdjustment(long &AdjustmentType, CString &Description, CURRENCY &Amount, long &VatInfo);
	bool GetPrintRecMessage(CString &Message);
	bool GetPrintRecSubtotal(CURRENCY &Amount);
	bool GetPrintRecSubtotalAdjustment(long& AdjustmentType, CString &Description, CURRENCY &Amount);
	bool GetPrintRecVoid(CString &Description);
	bool GetPrintRecVoidItem(CString &Description, CURRENCY &Amount, long &Quantity, long &AdjustmentType, CURRENCY &Adjustment, long &VatInfo);
	bool GetPrintZReport();

    //    Group 3
    bool GetBeginTraining();
    bool GetEndTraining();
    bool Get_GetData(long &DataItem);
    bool Get_GetData_Return(long& DataItem, CString& ReturnData );
    bool Get_GetDate();
    bool Get_GetDate_Return(CString& ReturnDate );
    bool GetPrintRecNotPaid(CString& Description, CURRENCY& Amount );
    bool GetPrintRecRefund(CString& Description, CURRENCY& Amount, long& VatInfo );
    bool Get_ResetPrinter();
    bool Get_SetDate(CString& Date);
    bool Get_SetHeaderLine(long& LineNumber, CString& Text, bool& DoubleWidth);

    bool Get_OutputComplete_Return();

protected:
    virtual int getLength(void);

private:
    enum_FPFunctions    m_Function;
    bool m_bBoolVal;
    long m_lLongVal1;
    long m_lLongVal2;
    long m_lLongVal3;
    CURRENCY m_curMoney1;
    CURRENCY m_curMoney2;
    CString m_csStringVal1;
    CString m_csStringVal2;
};


#endif //AFX_FISCALPRINTERMSG_H__42B90DD7_51ED_4FA0_9193_6EBF52E75215__INCLUDED_