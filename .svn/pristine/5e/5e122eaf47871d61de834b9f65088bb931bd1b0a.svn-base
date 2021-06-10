// WindowMsg.h : Declaration of the CWindowMsg
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>

// IWindowMsg
[
	object,
	uuid(13979CBF-F2A8-471D-BF62-72E7A19B88C7),
	dual,
	helpstring("IWindowMsg Interface"),
	pointer_default(unique)
]
__interface IWindowMsg : public IDispatch
{
};


// CWindowMsg
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.WindowMsg"),
	progid("tbControls.WindowMsg.1"),
	version(1.0),
	uuid("8BA51575-F22F-426D-A4D2-5D3301F086F4"),
	helpstring("WindowMsg Class"),
	support_error_info(IWindowMsg),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CWindowMsg : 
	public IWindowMsg,
	public IPersistStreamInitImpl<CWindowMsg>,
	public IOleControlImpl<CWindowMsg>,
	public IOleObjectImpl<CWindowMsg>,
	public IOleInPlaceActiveObjectImpl<CWindowMsg>,
	public IViewObjectExImpl<CWindowMsg>,
	public IOleInPlaceObjectWindowlessImpl<CWindowMsg>,
	public IPersistStorageImpl<CWindowMsg>,
	public ISpecifyPropertyPagesImpl<CWindowMsg>,
	public IQuickActivateImpl<CWindowMsg>,
	public IDataObjectImpl<CWindowMsg>,
	public IProvideClassInfo2Impl<&__uuidof(CWindowMsg), NULL>,
	public CComControl<CWindowMsg>
{
public:

	CWindowMsg()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CWindowMsg)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CWindowMsg)
	CHAIN_MSG_MAP(CComControl<CWindowMsg>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IWindowMsg
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
		LPCTSTR pszText = _T("ATL 7.0 : WindowMsg");
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
};

