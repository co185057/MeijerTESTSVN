// CustomObj.h : Declaration of the CCustomObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "CustomerDefinedMsg.h"

// ICustomObj
[
	object,
	uuid(94ED1368-53C2-44FA-8641-951D31DEE2B5),
	dual,
	helpstring("ICustomObj Interface"),
	pointer_default(unique)
]
__interface ICustomObj : public IDispatch
{
	[propget, id(1), helpstring("property CustomerData")] HRESULT CustomerData([out, retval] VARIANT* pVal);
	[propput, id(1), helpstring("property CustomerData")] HRESULT CustomerData([in] VARIANT newVal);
};


// CCustomObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.CustomObj"),
	progid("tbControls.CustomObj.1"),
	version(1.0),
	uuid("CF2B0DA9-5461-4544-8494-001C909E095C"),
	helpstring("CustomObj Class"),
	support_error_info(ICustomObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CCustomObj : 
	public ICustomObj,
	public IPersistStreamInitImpl<CCustomObj>,
	public IOleControlImpl<CCustomObj>,
	public IOleObjectImpl<CCustomObj>,
	public IOleInPlaceActiveObjectImpl<CCustomObj>,
	public IViewObjectExImpl<CCustomObj>,
	public IOleInPlaceObjectWindowlessImpl<CCustomObj>,
	public IPersistStorageImpl<CCustomObj>,
	public ISpecifyPropertyPagesImpl<CCustomObj>,
	public IQuickActivateImpl<CCustomObj>,
	public IDataObjectImpl<CCustomObj>,
	public IProvideClassInfo2Impl<&__uuidof(CCustomObj), NULL>,
	public CComControl<CCustomObj>,
   public ITBStream
{
private:
	CCustomerDefinedMsg m_CustomMsg;
public:

	CCustomObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CCustomObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CCustomObj)
	CHAIN_MSG_MAP(CComControl<CCustomObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ICustomObj
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
		LPCTSTR pszText = _T("ATL 7.0 : CustomObj");
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
	STDMETHOD(get_CustomerData)(VARIANT* pVal);
	STDMETHOD(put_CustomerData)(VARIANT newVal);
};

