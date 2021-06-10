// PinpadObj.h : Declaration of the CPinpadObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "PinPadMsg.h"

[export]
typedef enum { TT_CREDIT        = 0x02,
                TT_DEBIT         = 0x03,
                TT_LOYALTY       = 0x04,
                TT_DEBIT_SAVINGS = 0x05,
                TT_EBT           = 0x07,
                TT_GIFT          = 0x09,
                TT_UNDEFINED     = 0x99
} TransactionTypes;

[export]
typedef enum { PE_MASTER_SESSION = 0x00,
                PE_DUKPT          = 0x01,
                PE_UNDEFINED      = 0x99
} PinEncodingList;

[export]
typedef enum { PB_ANSI      = 0x00,
                PB_DIEBOLD   = 0x01,
                PB_UNDEFINED = 0x99
} PinBlockFormats;


// IPinpadObj
[
	object,
	uuid(1A737844-D845-4E97-AD01-A3FFBAD13839),
	dual,
	helpstring("IPinpadObj Interface"),
	pointer_default(unique)
]
__interface IPinpadObj : public IDispatch
{
	[propget, id(1), helpstring("property PrimaryAccountNumber")] HRESULT PrimaryAccountNumber([out, retval] BSTR* pVal);
	[propput, id(1), helpstring("property PrimaryAccountNumber")] HRESULT PrimaryAccountNumber([in] BSTR newVal);
	[propget, id(2), helpstring("property TransactionType")] HRESULT TransactionType([out, retval] TransactionTypes* pVal);
	[propput, id(2), helpstring("property TransactionType")] HRESULT TransactionType([in] TransactionTypes newVal);
	[propget, id(3), helpstring("property PinEncoding")] HRESULT PinEncoding([out, retval] PinEncodingList* pVal);
	[propput, id(3), helpstring("property PinEncoding")] HRESULT PinEncoding([in] PinEncodingList newVal);
};


// CPinpadObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.PinpadObj"),
	progid("tbControls.PinpadObj.1"),
	version(1.0),
	uuid("AD4AE7F5-390C-4916-AC73-224FFBF7AC63"),
	helpstring("PinpadObj Class"),
	support_error_info(IPinpadObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CPinpadObj : 
	public IPinpadObj,
	public IPersistStreamInitImpl<CPinpadObj>,
	public IOleControlImpl<CPinpadObj>,
	public IOleObjectImpl<CPinpadObj>,
	public IOleInPlaceActiveObjectImpl<CPinpadObj>,
	public IViewObjectExImpl<CPinpadObj>,
	public IOleInPlaceObjectWindowlessImpl<CPinpadObj>,
	public IPersistStorageImpl<CPinpadObj>,
	public ISpecifyPropertyPagesImpl<CPinpadObj>,
	public IQuickActivateImpl<CPinpadObj>,
	public IDataObjectImpl<CPinpadObj>,
	public IProvideClassInfo2Impl<&__uuidof(CPinpadObj), NULL>,
	public CComControl<CPinpadObj>,
   public ITBStream
{
private:
	CPinpadMsg m_PinpadMsg;

public:

	CPinpadObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CPinpadObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CPinpadObj)
	CHAIN_MSG_MAP(CComControl<CPinpadObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPinpadObj
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
		LPCTSTR pszText = _T("ATL 7.0 : PinpadObj");
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
	STDMETHOD(get_PrimaryAccountNumber)(BSTR* pVal);
	STDMETHOD(put_PrimaryAccountNumber)(BSTR newVal);
	STDMETHOD(get_TransactionType)(TransactionTypes* pVal);
	STDMETHOD(put_TransactionType)(TransactionTypes newVal);
	STDMETHOD(get_PinEncoding)(PinEncodingList* pVal);
	STDMETHOD(put_PinEncoding)(PinEncodingList newVal);
};

