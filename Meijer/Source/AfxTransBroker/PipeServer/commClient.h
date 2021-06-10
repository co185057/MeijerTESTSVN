// commClient.h : Declaration of the CcommClient
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
//#import "progid:commServer.commRouter.1" embedded_idl, no_namespace
#include "commrouter.h"

// IcommClient
[
	object,
	uuid(D2A928E8-1883-49FA-A7C6-01AD0C3F959B),
	dual,
	helpstring("IcommClient Interface"),
	pointer_default(unique)
]
__interface IcommClient : public IDispatch
{
	[id(1), helpstring("method Advise")] HRESULT Advise([in] LONG lID);
	[id(2), helpstring("method UnAdvise")] HRESULT UnAdvise(void);
	[id(3), helpstring("method Send")] HRESULT Send([in] LONG lDestination, [in] VARIANT data);
};


// CcommClient
[
	coclass,
	threading("apartment"),
	vi_progid("commServer.commClient"),
	progid("commServer.commClient.1"),
	version(1.0),
	uuid("11F1CD29-4399-4397-B663-2E310FDA0945"),
	helpstring("commClient Class"),
	registration_script("control.rgs"),
	event_receiver(com)
]
class ATL_NO_VTABLE CcommClient : 
	public IcommClient,
	public IPersistStreamInitImpl<CcommClient>,
	public IOleControlImpl<CcommClient>,
	public IOleObjectImpl<CcommClient>,
	public IOleInPlaceActiveObjectImpl<CcommClient>,
	public IViewObjectExImpl<CcommClient>,
	public IOleInPlaceObjectWindowlessImpl<CcommClient>,
	public IDataObjectImpl<CcommClient>,
	public IProvideClassInfo2Impl<&__uuidof(CcommClient), NULL>,
	public CComControl<CcommClient>
{
public:

	CcommClient()
	{
		m_bConnected = false;
		m_data = L"Not connected";
	}

	~CcommClient()
	{
		UnAdvise();
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CcommClient)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CcommClient)
	CHAIN_MSG_MAP(CComControl<CcommClient>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IcommClient
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
			USES_CONVERSION;
			LPCTSTR text = OLE2CT(m_data.bstrVal);
			RECT& rc = *(RECT*)di.prcBounds;
			Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
			SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
			TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			text, 
			lstrlen(text));
		return S_OK;

/*
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
		LPCTSTR pszText = _T("ATL 7.0 : commClient");
		TextOut(di.hdcDraw, 
			(rc.left + rc.right) / 2, 
			(rc.top + rc.bottom) / 2, 
			pszText, 
			lstrlen(pszText));

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
*/
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	HRESULT OnTransmit(VARIANT data);

	STDMETHOD(Advise)(LONG lID);
	STDMETHOD(UnAdvise)(void);
	STDMETHOD(Send)(LONG lDestination, VARIANT data);
private:
   // Data
   bool m_bConnected;
   CComPtr<IcommRouter> m_spIServ;
   CComVariant m_data;

};

