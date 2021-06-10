// ScannerObj.h : Declaration of the CScannerObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "ScannerMsg.h"
#include "TBStream.h"

[export]
typedef enum { BT_UPCA      = 0x01,
                  BT_UPCA_SUP  = 0X02,
                  BT_UPCE      = 0x03,
                  BT_UPCE_SUP  = 0x04,
                  BT_UPCD1     = 0x05,
                  BT_UPCD2     = 0x06,
                  BT_UPCD3     = 0x07,
                  BT_UPCD4     = 0x08,
                  BT_UPCD5     = 0x09,
                  BT_EAN8      = 0x0A,
                  BT_EAN8_SUP  = 0x0B,
                  BT_EAN13     = 0x0C,
                  BT_EAN13_SUP = 0x0D,
                  BT_EAN128    = 0x0E,
                  BT_STD25     = 0x0F,
                  BT_CODABAR   = 0x10,
                  BT_CODE39    = 0x11,
                  BT_CODE93    = 0x12,
                  BT_CODE128   = 0x13,
                  BT_OCRA      = 0x14,
                  BT_OCRB      = 0x14,
                  BT_GS1DATABAR = 0x20,
                  BT_GS1DATABAR_E = 0x21,
                  BT_PDF417    = 0x40,
                  BT_MAXICODE  = 0x41,
                  BT_UNKNOWN   = 0x99
   } BarcodeTypes;
   
// IScannerObj
[
	object,
	uuid(BA1AC205-4D3E-43C3-99D0-C1A3D749BF93),
	dual,
	helpstring("IScannerObj Interface"),
	pointer_default(unique)
]
__interface IScannerObj : public IDispatch
{
	[propget, id(1), helpstring("property BarcodeType")] HRESULT BarcodeType([out, retval] BarcodeTypes* pVal);
	[propput, id(1), helpstring("property BarcodeType")] HRESULT BarcodeType([in] BarcodeTypes newVal);
	[propget, id(2), helpstring("property Data")] HRESULT Data([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("property Data")] HRESULT Data([in] BSTR newVal);
};


// CScannerObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.ScannerObj"),
	progid("tbControls.ScannerObj.1"),
	version(1.0),
	uuid("2735E4A9-F209-4534-B7B9-E60D7D663A02"),
	helpstring("ScannerObj Class"),
	support_error_info(IScannerObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CScannerObj : 
	public IScannerObj,
	public IPersistStreamInitImpl<CScannerObj>,
	public IOleControlImpl<CScannerObj>,
	public IOleObjectImpl<CScannerObj>,
	public IOleInPlaceActiveObjectImpl<CScannerObj>,
	public IViewObjectExImpl<CScannerObj>,
	public IOleInPlaceObjectWindowlessImpl<CScannerObj>,
	public IPersistStorageImpl<CScannerObj>,
	public ISpecifyPropertyPagesImpl<CScannerObj>,
	public IQuickActivateImpl<CScannerObj>,
	public IDataObjectImpl<CScannerObj>,
	public IProvideClassInfo2Impl<&__uuidof(CScannerObj), NULL>,
	public CComControl<CScannerObj>,
   public ITBStream
{
public:

	CScannerObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CScannerObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CScannerObj)
	CHAIN_MSG_MAP(CComControl<CScannerObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IScannerObj
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
		LPCTSTR pszText = _T("ATL 7.0 : ScannerObj");
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
protected:
   CScannerMsg m_scannerMsg;
public:
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
	STDMETHOD(ParseStream)(VARIANT i_varpBuf);
	STDMETHOD(ParseBuffer)(BYTE *pBuf);
	STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
	STDMETHOD(get_BarcodeType)(BarcodeTypes* pVal);
	STDMETHOD(put_BarcodeType)(BarcodeTypes newVal);
	STDMETHOD(get_Data)(BSTR* pVal);
	STDMETHOD(put_Data)(BSTR newVal);
};

