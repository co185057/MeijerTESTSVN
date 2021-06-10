// TBGenObj.h : Declaration of the CTBGenObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBGenMsg.h"
#include "TBStream.h"

[export]
typedef enum { 
    RESET_FIELD_DATA = 0,
    RESET_XML_DOC  = 1
} ResetOptions;

// ITBGenObj
[
	object,
	uuid(5D8CF3FE-492B-445E-A6D9-B673BC610D55),
	dual,
	helpstring("ITBGenObj Interface"),
	pointer_default(unique)
]
__interface ITBGenObj : public IDispatch
{
	[id(1), helpstring("method SetValue")] HRESULT SetValue([in] BSTR Name, [in] VARIANT vValue);
	[id(2), helpstring("method GetValue")] HRESULT GetValue([in] BSTR Name, [out,retval] VARIANT* vValue);
	[id(3), helpstring("method Reset")] HRESULT Reset([in] ResetOptions option);
	[propget, id(4), helpstring("property xml")] HRESULT xml([out, retval] BSTR* pVal);
	[propput, id(4), helpstring("property xml")] HRESULT xml([in] BSTR newVal);
	[propget, id(5), helpstring("property MsgId")] HRESULT MsgId([out, retval] BYTE* pVal);
	[propput, id(5), helpstring("property MsgId")] HRESULT MsgId([in] BYTE newVal);
	[propget, id(6), helpstring("property IsXMLOutput")] HRESULT IsXMLOutput([out, retval] BYTE* pVal);
	[propput, id(6), helpstring("property IsXMLOutput")] HRESULT IsXMLOutput([in] BYTE newVal);
	[propget, id(7), helpstring("property Validate")] HRESULT Validate([out, retval] BYTE* pVal);
	[propput, id(7), helpstring("property Validate")] HRESULT Validate([in] BYTE newVal);
    [propget, id(8), helpstring("property Name")] HRESULT Name([out, retval] BSTR* pVal);
    [propput, id(8), helpstring("property Name")] HRESULT Name([in] BSTR newVal);
};


// CTBGenObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.TBGenObj"),
	progid("tbControls.TBGenObj.1"),
	version(1.0),
	uuid("E3052DB2-D582-4A89-A354-AD9705641442"),
	helpstring("TBGenObj Class"),
	support_error_info(ITBGenObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CTBGenObj : 
	public ITBGenObj,
	public IPersistStreamInitImpl<CTBGenObj>,
	public IOleControlImpl<CTBGenObj>,
	public IOleObjectImpl<CTBGenObj>,
	public IOleInPlaceActiveObjectImpl<CTBGenObj>,
	public IViewObjectExImpl<CTBGenObj>,
	public IOleInPlaceObjectWindowlessImpl<CTBGenObj>,
	public IPersistStorageImpl<CTBGenObj>,
	public ISpecifyPropertyPagesImpl<CTBGenObj>,
	public IQuickActivateImpl<CTBGenObj>,
	public IDataObjectImpl<CTBGenObj>,
	public IProvideClassInfo2Impl<&__uuidof(CTBGenObj), NULL>,
	public CComControl<CTBGenObj>,
   public ITBStream
{
public:

	CTBGenObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CTBGenObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CTBGenObj)
	CHAIN_MSG_MAP(CComControl<CTBGenObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ITBGenObj
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
		LPCTSTR pszText = _T("ATL 7.0 : TBGenObj");
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

	STDMETHOD(SetValue)(BSTR Name, VARIANT vValue);
	STDMETHOD(GetValue)(BSTR Name, VARIANT* vValue);
	CTBGenMsg m_tbGenMsg;
	STDMETHOD(Reset)(ResetOptions option);
	STDMETHOD(get_xml)(BSTR* pVal);
	STDMETHOD(put_xml)(BSTR newVal);
	STDMETHOD(get_MsgId)(BYTE* pVal);
	STDMETHOD(put_MsgId)(BYTE newVal);
	STDMETHOD(get_IsXMLOutput)(BYTE* pVal);
	STDMETHOD(put_IsXMLOutput)(BYTE newVal);
	STDMETHOD(get_Validate)(BYTE* pVal);
	STDMETHOD(put_Validate)(BYTE newVal);
    STDMETHOD(get_Name)(BSTR* pVal);
    STDMETHOD(put_Name)(BSTR newVal);
};

