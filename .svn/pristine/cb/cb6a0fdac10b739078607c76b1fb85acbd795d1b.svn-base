// PrinterObj.cpp : Implementation of CPrinterObj
#include "stdafx.h"
#include "PrinterObj.h"
#include "safearrayVariant.h"
#include "ImplementMacros.h"

#pragma warning(disable:4311) 
IMPLEMENT_TBSTREAM(CPrinterObj, m_PrinterMsg)
#pragma warning(default:4311) 

// CPrinterObj

STDMETHODIMP CPrinterObj::get_Function(BYTE* pVal)
{
	*pVal= m_PrinterMsg.Function();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Function(BYTE newVal)
{
	m_PrinterMsg.Function(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_Arguments(BSTR* pVal)
{
	CComBSTR csVal(m_PrinterMsg.Arguments());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Arguments(BSTR newVal)
{
	m_PrinterMsg.Arguments(CString(newVal));
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_Height(SHORT* pVal)
{
	*pVal= m_PrinterMsg.Height();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Height(SHORT newVal)
{
	m_PrinterMsg.Height(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_Width(SHORT* pVal)
{
	*pVal= m_PrinterMsg.Width();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Width(SHORT newVal)
{
	m_PrinterMsg.Width(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_Alignment(BYTE* pVal)
{
	*pVal= m_PrinterMsg.Alignment();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Alignment(BYTE newVal)
{
	m_PrinterMsg.Alignment(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_TextPosition(SHORT* pVal)
{
	*pVal= m_PrinterMsg.TextPosition();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_TextPosition(SHORT newVal)
{
	m_PrinterMsg.TextPosition(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_StationId(BYTE* pVal)
{
	*pVal= m_PrinterMsg.StationId();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_StationId(BYTE newVal)
{
	m_PrinterMsg.StationId(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_Data(BSTR* pVal)
{
	CComBSTR csVal(m_PrinterMsg.Data());
	csVal.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_Data(BSTR newVal)
{
	m_PrinterMsg.Data(CString(newVal));
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_BarcodeType(BYTE* pVal)
{
	*pVal= m_PrinterMsg.BarcodeType();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_BarcodeType(BYTE newVal)
{
	m_PrinterMsg.BarcodeType(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_BarcodeCharacterPrint(BYTE* pVal)
{
	*pVal= m_PrinterMsg.BarcodeCharacterPrint();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_BarcodeCharacterPrint(BYTE newVal)
{
	m_PrinterMsg.BarcodeCharacterPrint(newVal);
	return S_OK;
}

STDMETHODIMP CPrinterObj::get_BitmapLogoNumber(BYTE* pVal)
{
	*pVal= m_PrinterMsg.BitmapLogoNumber();
	return S_OK;
}

STDMETHODIMP CPrinterObj::put_BitmapLogoNumber(BYTE newVal)
{
	m_PrinterMsg.BitmapLogoNumber(newVal);
	return S_OK;
}
