// PrinterObj.h : Declaration of the CPrinterObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "PrinterMsg.h"


// IPrinterObj
[
	object,
	uuid(098D843A-FDDC-4708-9174-23134176A419),
	dual,
	helpstring("IPrinterObj Interface"),
	pointer_default(unique)
]
__interface IPrinterObj : public IDispatch
{
	[propget, id(1), helpstring("property Function")] HRESULT Function([out, retval] BYTE* pVal);
	[propput, id(1), helpstring("property Function")] HRESULT Function([in] BYTE newVal);
	[propget, id(2), helpstring("property Arguments")] HRESULT Arguments([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("property Arguments")] HRESULT Arguments([in] BSTR newVal);
	[propget, id(3), helpstring("property Height")] HRESULT Height([out, retval] SHORT* pVal);
	[propput, id(3), helpstring("property Height")] HRESULT Height([in] SHORT newVal);
	[propget, id(4), helpstring("property Width")] HRESULT Width([out, retval] SHORT* pVal);
	[propput, id(4), helpstring("property Width")] HRESULT Width([in] SHORT newVal);
	[propget, id(5), helpstring("property Alignment")] HRESULT Alignment([out, retval] BYTE* pVal);
	[propput, id(5), helpstring("property Alignment")] HRESULT Alignment([in] BYTE newVal);
	[propget, id(6), helpstring("property TextPosition")] HRESULT TextPosition([out, retval] SHORT* pVal);
	[propput, id(6), helpstring("property TextPosition")] HRESULT TextPosition([in] SHORT newVal);
	[propget, id(7), helpstring("property StationId")] HRESULT StationId([out, retval] BYTE* pVal);
	[propput, id(7), helpstring("property StationId")] HRESULT StationId([in] BYTE newVal);
	[propget, id(8), helpstring("property Data")] HRESULT Data([out, retval] BSTR* pVal);
	[propput, id(8), helpstring("property Data")] HRESULT Data([in] BSTR newVal);
	[propget, id(9), helpstring("property BarcodeType")] HRESULT BarcodeType([out, retval] BYTE* pVal);
	[propput, id(9), helpstring("property BarcodeType")] HRESULT BarcodeType([in] BYTE newVal);
	[propget, id(10), helpstring("property BarcodeCharacterPrint")] HRESULT BarcodeCharacterPrint([out, retval] BYTE* pVal);
	[propput, id(10), helpstring("property BarcodeCharacterPrint")] HRESULT BarcodeCharacterPrint([in] BYTE newVal);
	[propget, id(11), helpstring("property BitmapLogoNumber")] HRESULT BitmapLogoNumber([out, retval] BYTE* pVal);
	[propput, id(11), helpstring("property BitmapLogoNumber")] HRESULT BitmapLogoNumber([in] BYTE newVal);
};


// CPrinterObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.PrinterObj"),
	progid("tbControls.PrinterObj.1"),
	version(1.0),
	uuid("C2D1E10E-3DB1-4FE7-8160-FBEAB907B533"),
	helpstring("PrinterObj Class"),
	support_error_info(IPrinterObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CPrinterObj : 
	public IPrinterObj,
	public IPersistStreamInitImpl<CPrinterObj>,
	public IOleControlImpl<CPrinterObj>,
	public IOleObjectImpl<CPrinterObj>,
	public IOleInPlaceActiveObjectImpl<CPrinterObj>,
	public IViewObjectExImpl<CPrinterObj>,
	public IOleInPlaceObjectWindowlessImpl<CPrinterObj>,
	public IPersistStorageImpl<CPrinterObj>,
	public ISpecifyPropertyPagesImpl<CPrinterObj>,
	public IQuickActivateImpl<CPrinterObj>,
	public IDataObjectImpl<CPrinterObj>,
	public IProvideClassInfo2Impl<&__uuidof(CPrinterObj), NULL>,
	public CComControl<CPrinterObj>,
   public ITBStream

{
private:
	CPrinterMsg m_PrinterMsg;
public:

	CPrinterObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CPrinterObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CPrinterObj)
	CHAIN_MSG_MAP(CComControl<CPrinterObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPrinterObj
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
		RECT& rc = *(RECT*)di.prcBounds;
		// Set Clip region to the rectangle specified by di.prcBounds
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("ATL 7.0 : PrinterObj");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
	STDMETHOD(ParseStream)(VARIANT i_varpBuf);
	STDMETHOD(ParseBuffer)(BYTE *pBuf);
	STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
	STDMETHOD(get_Function)(BYTE* pVal);
	STDMETHOD(put_Function)(BYTE newVal);
	STDMETHOD(get_Arguments)(BSTR* pVal);
	STDMETHOD(put_Arguments)(BSTR newVal);
	STDMETHOD(get_Height)(SHORT* pVal);
	STDMETHOD(put_Height)(SHORT newVal);
	STDMETHOD(get_Width)(SHORT* pVal);
	STDMETHOD(put_Width)(SHORT newVal);
	STDMETHOD(get_Alignment)(BYTE* pVal);
	STDMETHOD(put_Alignment)(BYTE newVal);
	STDMETHOD(get_TextPosition)(SHORT* pVal);
	STDMETHOD(put_TextPosition)(SHORT newVal);
	STDMETHOD(get_StationId)(BYTE* pVal);
	STDMETHOD(put_StationId)(BYTE newVal);
	STDMETHOD(get_Data)(BSTR* pVal);
	STDMETHOD(put_Data)(BSTR newVal);
	STDMETHOD(get_BarcodeType)(BYTE* pVal);
	STDMETHOD(put_BarcodeType)(BYTE newVal);
	STDMETHOD(get_BarcodeCharacterPrint)(BYTE* pVal);
	STDMETHOD(put_BarcodeCharacterPrint)(BYTE newVal);
	STDMETHOD(get_BitmapLogoNumber)(BYTE* pVal);
	STDMETHOD(put_BitmapLogoNumber)(BYTE newVal);
};

