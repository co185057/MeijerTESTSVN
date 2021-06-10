// CKeylockObj.h : Declaration of the CKeylockObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "KeylockMsg.h"
#include "TBStream.h"

// IKeylockObj
[
	object,
	uuid(83DBC6EC-6015-408C-9249-0FBA333CB7D7),
	dual,
	helpstring("IKeylockObj Interface"),
	pointer_default(unique)
]
__interface IKeylockObj : public IDispatch
{
   [propget, id(1), helpstring("property Position")] HRESULT Position([out, retval] BYTE* pVal);
   [propput, id(1), helpstring("property Position")] HRESULT Position([in] BYTE newVal);
};

// CKeylockObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.CKeylockObj"),
	progid("tbControls.CKeylockObj.1"),
	version(1.0),
	uuid("23439E29-A334-434B-963D-1AEC4A5A8A0B"),
	helpstring("CKeylockObj Class"),
	support_error_info(IKeylockObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CKeylockObj : 
	public IKeylockObj,
	public IPersistStreamInitImpl<CKeylockObj>,
	public IOleControlImpl<CKeylockObj>,
	public IOleObjectImpl<CKeylockObj>,
	public IOleInPlaceActiveObjectImpl<CKeylockObj>,
	public IViewObjectExImpl<CKeylockObj>,
	public IOleInPlaceObjectWindowlessImpl<CKeylockObj>,
	public IPersistStorageImpl<CKeylockObj>,
	public ISpecifyPropertyPagesImpl<CKeylockObj>,
	public IQuickActivateImpl<CKeylockObj>,
	public IDataObjectImpl<CKeylockObj>,
	public CComControl<CKeylockObj>,
	public ITBStream
{
public:

	CKeylockObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CKeylockObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CKeylockObj)
	CHAIN_MSG_MAP(CComControl<CKeylockObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IKeylockObj
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
		LPCTSTR pszText = _T("ATL 7.0 : CKeylockObj");
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
   CKeylockMsg m_KeylockDetailMsg;
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
   STDMETHOD(ParseStream)(VARIANT i_varpBuf);
   STDMETHOD(ParseBuffer)(BYTE* pBuffer);
   STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
   STDMETHOD(get_Position)(BYTE* pVal);
   STDMETHOD(put_Position)(BYTE newVal);
};

