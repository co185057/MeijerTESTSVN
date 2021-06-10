// FiscalPrinterMsg.cpp: implementation of the CFiscalPrinterMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FiscalPrinterMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFiscalPrinterMsg::CFiscalPrinterMsg(TBDEVICEID devId) :
   CTBMsgObj(devId), m_Function( cInvalidFunction ),
   m_bBoolVal( false ), m_lLongVal1(0), m_lLongVal2(0),
   m_lLongVal3(0)
{
    m_csStringVal1.Empty();
    m_csStringVal2.Empty();
    m_curMoney1.int64 = 0;
    m_curMoney2.int64 = 0;
}

CFiscalPrinterMsg::~CFiscalPrinterMsg()
{
}

BYTE* CFiscalPrinterMsg::Serialize(int &byteLength)
{
    byteLength = 0;
    SerializeHeader(byteLength); // must be called first

    SerializeByte((BYTE)m_Function);
    SerializeByte(m_bBoolVal);
    SerializeULong((unsigned long)m_lLongVal1);
    SerializeULong((unsigned long)m_lLongVal2);
    SerializeULong((unsigned long)m_lLongVal3);
    SerializeBinaryBuffer((BYTE*)&m_curMoney1, sizeof(CURRENCY));
    SerializeBinaryBuffer((BYTE*)&m_curMoney2, sizeof(CURRENCY));
    SerializeL2String( m_csStringVal1 );
    SerializeL2String( m_csStringVal2 );
    SerializeExtensionData();

    return m_pBuf;
}

void CFiscalPrinterMsg::Parse(BYTE *pBuf)
{
    ParseHeader(pBuf);         // must be called first 

    m_Function = (enum_FPFunctions)ParseByte();
    m_bBoolVal = ParseByte()? true : false;
    m_lLongVal1 = (long)ParseULong();
    m_lLongVal2 = (long)ParseULong();
    m_lLongVal3 = (long)ParseULong();
    memcpy( &m_curMoney1, this->ParseBinaryBuffer( sizeof(CURRENCY)), sizeof(CURRENCY) );
    memcpy( &m_curMoney2, this->ParseBinaryBuffer( sizeof(CURRENCY)), sizeof(CURRENCY) );
    m_csStringVal1 = ParseL2String();
    m_csStringVal2 = ParseL2String();
	ParseExtensionData();
}

int CFiscalPrinterMsg::getLength(void)
{
    int nSize = 0;

    nSize += (int)sizeof(tRouteHeader);    // Size of the header
    nSize += (int)sizeof( BYTE );          // Function Code
    nSize += (int)sizeof( BYTE );          // Boolean value
    nSize += (int)(3 * sizeof(long));      // 3 long values
    nSize += (int)(2 * sizeof(CURRENCY));  // 2 Currency Values
    nSize += (2 + m_csStringVal1.GetLength());  // 1st String Value
    nSize += (2 + m_csStringVal2.GetLength());  // 2nd String Value
    nSize += GetLengthExtensionData();

    return nSize;
}


    // Data Input
void CFiscalPrinterMsg::SetDirectIO( long Command, long lInput, const CString &pString )
{
    this->m_Function = cDirectIO;
    this->m_lLongVal1 = Command;
    this->m_lLongVal2 = lInput;
    this->m_csStringVal1 = pString;
}

void CFiscalPrinterMsg::SetDirectIO_Return( long Command, const CString &Response )
{
    this->m_Function = cDirectIO_Return;
    this->m_lLongVal1 = Command;
    this->m_csStringVal1 = Response;
}

/*void CFiscalPrinterMsg::SetGetPropertyNumber( long lPropertyIndex )
{
    this->m_lLongVal1 = lPropertyIndex;
}

void CFiscalPrinterMsg::SetGetPropertyNumber_Return( long lPropertyValue )
{
    this->m_lLongVal1 = lPropertyValue;
}*/

void CFiscalPrinterMsg::SetBeginFiscalReceipt( bool PrintHeader )
{
    this->m_Function = cBeginFiscalReceipt;
    this->m_bBoolVal = PrintHeader;
}

void CFiscalPrinterMsg::SetEndFiscalReceipt( bool PrintHeader )
{
    this->m_Function = cEndFiscalReceipt;
    this->m_bBoolVal = PrintHeader;
}

void CFiscalPrinterMsg::SetPrintRecItem( const CString &Description, const CURRENCY &Price, long Quantity,
                       long VatInfo, const CURRENCY &UnitPrice, const CString &UnitName)
{
    this->m_Function = cPrintRecItem;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Price;
    this->m_lLongVal1 = Quantity;
    this->m_lLongVal2 = VatInfo;
    this->m_curMoney2 = UnitPrice;
    this->m_csStringVal2 = UnitName;
}

void CFiscalPrinterMsg::SetPrintRecTotal( const CURRENCY &Total, const CURRENCY &Payment, const CString &Description)
{
    this->m_Function = cPrintRecTotal;
    this->m_curMoney1 = Total;
    this->m_curMoney2 = Payment;
    this->m_csStringVal1 = Description;
}

    //    Group 2
void CFiscalPrinterMsg::SetBeginNonFiscal()
{
    this->m_Function = cBeginNonFiscal;
}

void CFiscalPrinterMsg::SetEndNonFiscal()
{
    this->m_Function = cEndNonFiscal;
}

void CFiscalPrinterMsg::SetPrintNormal(long Station, const CString &Data)
{
    this->m_Function = cPrintNormal;
    this->m_lLongVal1 = Station;
    this->m_csStringVal1 = Data;
}

void CFiscalPrinterMsg::SetPrintRecItemAdjustment(long AdjustmentType, const CString &Description, CURRENCY Amount, long VatInfo)
{
    this->m_Function = cPrintRecItemAdjustment;
    this->m_lLongVal1 = AdjustmentType;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Amount;
    this->m_lLongVal2 = VatInfo;
}

void CFiscalPrinterMsg::SetPrintRecMessage(const CString &Message)
{
    this->m_Function = cPrintRecMessage;
    this->m_csStringVal1 = Message;
}

void CFiscalPrinterMsg::SetPrintRecSubtotal(CURRENCY Amount)
{
    this->m_Function = cPrintRecSubtotal;
    this->m_curMoney1 = Amount;
}

void CFiscalPrinterMsg::SetPrintRecSubtotalAdjustment(long AdjustmentType, const CString &Description, CURRENCY Amount)
{
    this->m_Function = cPrintRecSubtotalAdjustment;
    this->m_lLongVal1 = AdjustmentType;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Amount;
}

void CFiscalPrinterMsg::SetPrintRecVoid(const CString &Description)
{
    this->m_Function = cPrintRecVoid;
    this->m_csStringVal1 = Description;
}

void CFiscalPrinterMsg::SetPrintRecVoidItem(const CString &Description, CURRENCY Amount, long Quantity, long AdjustmentType, CURRENCY Adjustment, long VatInfo)
{
    this->m_Function = cPrintRecVoidItem;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Amount;
    this->m_lLongVal1 = Quantity;
    this->m_lLongVal2 = AdjustmentType;
    this->m_curMoney2 = Adjustment;
    this->m_lLongVal3 = VatInfo;
}

void CFiscalPrinterMsg::SetPrintZReport()
{
    this->m_Function = cPrintZReport;
}

    //    Group 3
void CFiscalPrinterMsg::SetBeginTraining()
{
    this->m_Function = cBeginTraining;
}

void CFiscalPrinterMsg::SetEndTraining()
{
    this->m_Function = cEndTraining;
}

void CFiscalPrinterMsg::Set_GetData(long DataItem)
{
    this->m_Function = cGetData;
    this->m_lLongVal1 = DataItem;
}

void CFiscalPrinterMsg::Set_GetData_Return(long DataItem, const CString ReturnData )
{
    this->m_Function = cGetData_Return;
    this->m_lLongVal1 = DataItem;
    this->m_csStringVal1 = ReturnData;
}

void CFiscalPrinterMsg::Set_GetDate()
{
    this->m_Function = cGetDate;
}

void CFiscalPrinterMsg::Set_GetDate_Return(const CString ReturnDate )
{
    this->m_Function = cGetDate_Return;
    this->m_csStringVal1 = ReturnDate;
}

void CFiscalPrinterMsg::SetPrintRecNotPaid(const CString Description, CURRENCY Amount )
{
    this->m_Function = cPrintRecNotPaid;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Amount;
}

void CFiscalPrinterMsg::SetPrintRecRefund(const CString Description, CURRENCY Amount, long VatInfo )
{
    this->m_Function = cPrintRecRefund;
    this->m_csStringVal1 = Description;
    this->m_curMoney1 = Amount;
    this->m_lLongVal1 = VatInfo;
}

void CFiscalPrinterMsg::Set_ResetPrinter()
{
    this->m_Function = cResetPrinter;
}

void CFiscalPrinterMsg::Set_SetDate(const CString Date)
{
    this->m_Function = cSetDate;
    this->m_csStringVal1 = Date;
}

void CFiscalPrinterMsg::Set_SetHeaderLine(long LineNumber, const CString Text, bool DoubleWidth)
{
    this->m_Function = cSetHeaderLine;
    this->m_lLongVal1 = LineNumber;
    this->m_csStringVal1 = Text;
    this->m_bBoolVal = DoubleWidth;
}

void CFiscalPrinterMsg::Set_OutputComplete_Return()
{
    this->m_Function = cOutputComplete_Return;
}

    // Data Access
bool CFiscalPrinterMsg::GetDirectIO( long &Command, long &lInput, CString &pString ) const
{
    if ( cDirectIO != this->m_Function )
    {
        return false;
    }

    Command = this->m_lLongVal1;
    lInput = this->m_lLongVal2;
    pString = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::GetDirectIO_Return( long &Command, CString &Response ) const
{
    if ( cDirectIO_Return != this->m_Function )
    {
        return false;
    }

    Command = this->m_lLongVal1;
    Response = this->m_csStringVal1;

    return true;
}


bool CFiscalPrinterMsg::GetBeginFiscalReceipt( bool &PrintHeader ) const
{
    if ( cBeginFiscalReceipt != this->m_Function )
    {
        return false;
    }

    PrintHeader = this->m_bBoolVal;

    return true;
}

bool CFiscalPrinterMsg::GetEndFiscalReceipt( bool &PrintHeader ) const
{
    if ( cEndFiscalReceipt != this->m_Function )
    {
        return false;
    }

    PrintHeader = this->m_bBoolVal;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecItem( CString &Description, CURRENCY &Price, long &Quantity,
                       long &VatInfo, CURRENCY &UnitPrice, CString &UnitName) const
{
    if ( cPrintRecItem != this->m_Function )
    {
        return false;
    }

    Description = this->m_csStringVal1;
    Price = this->m_curMoney1;
    Quantity = this->m_lLongVal1;
    VatInfo = this->m_lLongVal2;
    UnitPrice = this->m_curMoney2;
    UnitName = this->m_csStringVal2;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecTotal( CURRENCY &Total, CURRENCY &Payment, CString &Description) const
{
    if ( cPrintRecTotal != this->m_Function )
    {
        return false;
    }

    Total = this->m_curMoney1;
    Payment = this->m_curMoney2;
    Description = this->m_csStringVal1;

    return true;
}

enum_FPFunctions CFiscalPrinterMsg::GetFunctionCode( ) const
{
    return m_Function;
}

        //    Group 2
bool CFiscalPrinterMsg::GetBeginNonFiscal()
{
    if ( cBeginNonFiscal != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::GetEndNonFiscal()
{
    if ( cEndNonFiscal != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::GetPrintNormal(long &Station, CString &Data)
{
    if ( cPrintNormal != this->m_Function )
    {
        return false;
    }

    Station = this->m_lLongVal1;
    Data = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecItemAdjustment(long &AdjustmentType, CString &Description, CURRENCY &Amount, long &VatInfo)
{
    if ( cPrintRecItemAdjustment != this->m_Function )
    {
        return false;
    }

    AdjustmentType = this->m_lLongVal1;
    Description = this->m_csStringVal1;
    Amount = this->m_curMoney1;
    VatInfo = this->m_lLongVal2;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecMessage(CString &Message)
{
    if ( cPrintRecMessage != this->m_Function )
    {
        return false;
    }

    Message = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecSubtotal(CURRENCY &Amount)
{
    if ( cPrintRecSubtotal != this->m_Function )
    {
        return false;
    }

    Amount = this->m_curMoney1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecSubtotalAdjustment(long& AdjustmentType, CString &Description, CURRENCY &Amount)
{
    if ( cPrintRecSubtotalAdjustment != this->m_Function )
    {
        return false;
    }

    AdjustmentType = this->m_lLongVal1;
    Description = this->m_csStringVal1;
    Amount = this->m_curMoney1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecVoid(CString &Description)
{
    if ( cPrintRecVoid != this->m_Function )
    {
        return false;
    }

    Description = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecVoidItem(CString &Description, CURRENCY &Amount, long &Quantity, long &AdjustmentType, CURRENCY &Adjustment, long &VatInfo)
{
    if ( cPrintRecVoidItem != this->m_Function )
    {
        return false;
    }

    Description = this->m_csStringVal1;
    Amount = this->m_curMoney1;
    Quantity = this->m_lLongVal1;
    AdjustmentType = this->m_lLongVal2;
    Adjustment = this->m_curMoney2;
    VatInfo = this->m_lLongVal3;

    return true;
}

bool CFiscalPrinterMsg::GetPrintZReport()
{
    if ( cPrintZReport != this->m_Function )
    {
        return false;
    }

    return true;
}

    //    Group 3
bool CFiscalPrinterMsg::GetBeginTraining()
{
    if ( cBeginTraining != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::GetEndTraining()
{
    if ( cEndTraining != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::Get_GetData(long &DataItem)
{
    if ( cGetData != this->m_Function )
    {
        return false;
    }

    DataItem = this->m_lLongVal1;

    return true;
}

bool CFiscalPrinterMsg::Get_GetData_Return(long& DataItem, CString& ReturnData )
{
    if ( cGetData_Return != this->m_Function )
    {
        return false;
    }

    DataItem = this->m_lLongVal1;
    ReturnData = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::Get_GetDate()
{
    if ( cGetDate != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::Get_GetDate_Return(CString& ReturnDate )
{
    if ( cGetDate_Return != this->m_Function )
    {
        return false;
    }

    ReturnDate = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecNotPaid(CString& Description, CURRENCY& Amount )
{
    if ( cPrintRecNotPaid != this->m_Function )
    {
        return false;
    }

    Description = this->m_csStringVal1;
    Amount = this->m_curMoney1;

    return true;
}

bool CFiscalPrinterMsg::GetPrintRecRefund(CString& Description, CURRENCY& Amount, long& VatInfo )
{
    if ( cPrintRecRefund != this->m_Function )
    {
        return false;
    }

    Description = this->m_csStringVal1;
    Amount = this->m_curMoney1;
    VatInfo = this->m_lLongVal1;

    return true;
}

bool CFiscalPrinterMsg::Get_ResetPrinter()
{
    if ( cResetPrinter != this->m_Function )
    {
        return false;
    }

    return true;
}

bool CFiscalPrinterMsg::Get_SetDate(CString& Date)
{
    if ( cSetDate != this->m_Function )
    {
        return false;
    }

    Date = this->m_csStringVal1;

    return true;
}

bool CFiscalPrinterMsg::Get_SetHeaderLine(long& LineNumber, CString& Text, bool& DoubleWidth)
{
    if ( cSetHeaderLine != this->m_Function )
    {
        return false;
    }

    LineNumber = this->m_lLongVal1;
    Text = this->m_csStringVal1;
    DoubleWidth = this->m_bBoolVal;

    return true;
}

bool CFiscalPrinterMsg::Get_OutputComplete_Return()
{
    if ( cOutputComplete_Return != this->m_Function )
    {
        return false;
    }

    return true;
}