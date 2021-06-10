// MSRObj.h : Declaration of the CMSRObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "MSRMsg.h"

// IMSRObj
[
	object,
	uuid(BF8A79FF-565D-4BB2-9FCC-5110A7E21295),
	dual,
	helpstring("IMSRObj Interface"),
	pointer_default(unique)
]
__interface IMSRObj : public IDispatch
{
	[propget, id(1), helpstring("property Track1")] HRESULT Track1([out, retval] BSTR* pVal);
	[propget, id(2), helpstring("property Track2")] HRESULT Track2([out, retval] BSTR* pVal);
	[propget, id(3), helpstring("property Track3")] HRESULT Track3([out, retval] BSTR* pVal);
	[id(4), helpstring("method SetTrack123")] HRESULT SetTrack123(BYTE Track1Position, BYTE Track2Position, BYTE Track3Position, BSTR Track123);
};


// CMSRObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.MSRObj"),
	progid("tbControls.MSRObj.1"),
	version(1.0),
	uuid("155E348A-B10F-4DFB-90B6-4626518C9136"),
	helpstring("MSRObj Class"),
	support_error_info(IMSRObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMSRObj : 
	public IMSRObj,
	public IPersistStreamInitImpl<CMSRObj>,
	public IOleControlImpl<CMSRObj>,
	public IOleObjectImpl<CMSRObj>,
	public IOleInPlaceActiveObjectImpl<CMSRObj>,
	public IViewObjectExImpl<CMSRObj>,
	public IOleInPlaceObjectWindowlessImpl<CMSRObj>,
	public IPersistStorageImpl<CMSRObj>,
	public ISpecifyPropertyPagesImpl<CMSRObj>,
	public IQuickActivateImpl<CMSRObj>,
	public IDataObjectImpl<CMSRObj>,
	public IProvideClassInfo2Impl<&__uuidof(CMSRObj), NULL>,
	public CComControl<CMSRObj>,
   public ITBStream
{
private:
	CMSRMsg m_MSRMsg;
public:

	CMSRObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMSRObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMSRObj)
	CHAIN_MSG_MAP(CComControl<CMSRObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMSRObj
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
		LPCTSTR pszText = _T("ATL 7.0 : MSRObj");
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
	STDMETHOD(get_Track1)(BSTR* pVal);
	STDMETHOD(get_Track2)(BSTR* pVal);
	STDMETHOD(get_Track3)(BSTR* pVal);
	STDMETHOD(SetTrack123)(BYTE Track1Position, BYTE Track2Position, BYTE Track3Position, BSTR Track123);
};

