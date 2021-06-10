// ScaleObj.h : Declaration of the CScaleObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "ScaleMsg.h"

// IScaleObj
[
	object,
	uuid(EA8C9BF5-9DBF-486E-8C16-3673E72D4B1C),
	dual,
	helpstring("IScaleObj Interface"),
	pointer_default(unique)
]
__interface IScaleObj : public IDispatch
{
	[propget, id(1), helpstring("property Weight")] HRESULT Weight([out, retval] LONG* pVal);
	[propput, id(1), helpstring("property Weight")] HRESULT Weight([in] LONG newVal);
};


// CScaleObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.ScaleObj"),
	progid("tbControls.ScaleObj.1"),
	version(1.0),
	uuid("FB06D951-F350-413B-81CA-F613D234F339"),
	helpstring("ScaleObj Class"),
	support_error_info(IScaleObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CScaleObj : 
	public IScaleObj,
	public IPersistStreamInitImpl<CScaleObj>,
	public IOleControlImpl<CScaleObj>,
	public IOleObjectImpl<CScaleObj>,
	public IOleInPlaceActiveObjectImpl<CScaleObj>,
	public IViewObjectExImpl<CScaleObj>,
	public IOleInPlaceObjectWindowlessImpl<CScaleObj>,
	public IPersistStorageImpl<CScaleObj>,
	public ISpecifyPropertyPagesImpl<CScaleObj>,
	public IQuickActivateImpl<CScaleObj>,
	public IDataObjectImpl<CScaleObj>,
	public IProvideClassInfo2Impl<&__uuidof(CScaleObj), NULL>,
	public CComControl<CScaleObj>,
   public ITBStream

{
private:
	CScaleMsg m_ScaleMsg;
public:

	CScaleObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CScaleObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CScaleObj)
	CHAIN_MSG_MAP(CComControl<CScaleObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IScaleObj
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
		LPCTSTR pszText = _T("ATL 7.0 : ScaleObj");
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
	STDMETHOD(get_Weight)(LONG* pVal);
	STDMETHOD(put_Weight)(LONG newVal);
};

