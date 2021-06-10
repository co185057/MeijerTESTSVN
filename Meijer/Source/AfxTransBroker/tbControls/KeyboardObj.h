// KeyboardObj.h : Declaration of the CKeyboardObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "KeyboardMsg.h"
#include "TBStream.h"

[export]
typedef enum { KEY_PRESSED_NONE  = 0x00,
                SHIFT_KEY_PESSED  = 0x01,
		        CTRL_KEY_PRESSED  = 0X02,
		        ALT_KEY_PRESSED   = 0X04
} CombinationKeyFlags;

[export]
typedef enum { 
		KEY_DOWN     = 0x0000,
		EXTENDED_KEY = 0x0001,
		KEY_UP       = 0x0002
} KeyEventType;

// IKeyboardObj
[
	object,
	uuid(6E77AFE9-47D1-494F-9506-97B889C8ABE7),
	dual,
	helpstring("IKeyboardObj Interface"),
	pointer_default(unique)
]
__interface IKeyboardObj : public IDispatch
{
	[propget, id(1), helpstring("property KeyCode")] HRESULT KeyCode([out, retval] LONG* pVal);
	[propput, id(1), helpstring("property KeyCode")] HRESULT KeyCode([in] LONG newVal);
	[propget, id(2), helpstring("property KeyEvent")] HRESULT KeyEvent([out, retval] KeyEventType* pVal);
	[propput, id(2), helpstring("property KeyEvent")] HRESULT KeyEvent([in] KeyEventType newVal);
	[propget, id(3), helpstring("property CombinationKeys")] HRESULT CombinationKeys([out, retval] CombinationKeyFlags* pVal);
	[propput, id(3), helpstring("property CombinationKeys")] HRESULT CombinationKeys([in] CombinationKeyFlags newVal);
};

// CKeyboardObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.KeyboardObj"),
	progid("tbControls.KeyboardObj.1"),
	version(1.0),
	uuid("080D4835-9415-4279-B2B7-8DC4E716AB0D"),
	helpstring("KeyboardObj Class"),
	support_error_info(IKeyboardObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CKeyboardObj : 
	public IKeyboardObj,
	public IPersistStreamInitImpl<CKeyboardObj>,
	public IOleControlImpl<CKeyboardObj>,
	public IOleObjectImpl<CKeyboardObj>,
	public IOleInPlaceActiveObjectImpl<CKeyboardObj>,
	public IViewObjectExImpl<CKeyboardObj>,
	public IOleInPlaceObjectWindowlessImpl<CKeyboardObj>,
	public IPersistStorageImpl<CKeyboardObj>,
	public ISpecifyPropertyPagesImpl<CKeyboardObj>,
	public IQuickActivateImpl<CKeyboardObj>,
	public IDataObjectImpl<CKeyboardObj>,
	public CComControl<CKeyboardObj>,
   public ITBStream
{
private:
	CKeyboardMsg m_KeyboardMsg;
public:

	CKeyboardObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CKeyboardObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CKeyboardObj)
	CHAIN_MSG_MAP(CComControl<CKeyboardObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IKeyboardObj
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
		LPCTSTR pszText = _T("ATL 7.0 : KeyboardObj");
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
	STDMETHOD(get_KeyCode)(LONG* pVal);
	STDMETHOD(put_KeyCode)(LONG newVal);
	STDMETHOD(get_KeyEvent)(KeyEventType* pVal);
	STDMETHOD(put_KeyEvent)(KeyEventType newVal);
	STDMETHOD(get_CombinationKeys)(CombinationKeyFlags* pVal);
	STDMETHOD(put_CombinationKeys)(CombinationKeyFlags newVal);
};

