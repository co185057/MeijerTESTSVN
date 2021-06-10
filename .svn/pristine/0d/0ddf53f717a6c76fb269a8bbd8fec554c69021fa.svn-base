// TenderObj.h : Declaration of the CTenderObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "tenderdetailsmsg.h"
#include "TBStream.h"

// ITenderObj
[
	object,
	uuid(5520ED1C-ABB0-4C35-8DF0-6598F441051F),
	dual,
	helpstring("ITenderObj Interface"),
	pointer_default(unique)
]
__interface ITenderObj : public IDispatch
{
   [propget, id(1), helpstring("property Description")] HRESULT Description([out, retval] BSTR* pVal);
   [propput, id(1), helpstring("property Description")] HRESULT Description([in] BSTR newVal);
   [propget, id(2), helpstring("property Type")] HRESULT Type([out, retval] LONG* pVal);
   [propput, id(2), helpstring("property Type")] HRESULT Type([in] LONG newVal);
   [propget, id(3), helpstring("property Amount")] HRESULT Amount([out, retval] BSTR* pVal);
   [propput, id(3), helpstring("property Amount")] HRESULT Amount([in] BSTR newVal);
   [propget, id(4), helpstring("property Balance")] HRESULT Balance([out, retval] BSTR* pVal);
   [propput, id(4), helpstring("property Balance")] HRESULT Balance([in] BSTR newVal);
   [propget, id(5), helpstring("property ChangeDue")] HRESULT ChangeDue([out, retval] BSTR* pVal);
   [propput, id(5), helpstring("property ChangeDue")] HRESULT ChangeDue([in] BSTR newVal);
   [propget, id(6), helpstring("property Accepted")] HRESULT Accepted([out, retval] BYTE* pVal);
   [propput, id(6), helpstring("property Accepted")] HRESULT Accepted([in] BYTE newVal);
   [propget, id(7), helpstring("property Voided")] HRESULT Voided([out, retval] BYTE* pVal);
   [propput, id(7), helpstring("property Voided")] HRESULT Voided([in] BYTE newVal);
   [propget, id(8), helpstring("property CardRemainingBalance")] HRESULT CardRemainingBalance([out, retval] BSTR* pVal);
   [propput, id(8), helpstring("property CardRemainingBalance")] HRESULT CardRemainingBalance([in] BSTR newVal);
   [propget, id(9), helpstring("property ErrorDescription")] HRESULT ErrorDescription([out, retval] BSTR* pVal);
   [propput, id(9), helpstring("property ErrorDescription")] HRESULT ErrorDescription([in] BSTR newVal);
};


// CTenderObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.TenderObj"),
	progid("tbControls.TenderObj.1"),
	version(1.0),
	uuid("79D27133-06DF-46F9-8B5D-526D85332082"),
	helpstring("TenderObj Class"),
	support_error_info(ITenderObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CTenderObj : 
	public ITenderObj,
	public IPersistStreamInitImpl<CTenderObj>,
	public IOleControlImpl<CTenderObj>,
	public IOleObjectImpl<CTenderObj>,
	public IOleInPlaceActiveObjectImpl<CTenderObj>,
	public IViewObjectExImpl<CTenderObj>,
	public IOleInPlaceObjectWindowlessImpl<CTenderObj>,
	public IPersistStorageImpl<CTenderObj>,
	public ISpecifyPropertyPagesImpl<CTenderObj>,
	public IQuickActivateImpl<CTenderObj>,
	public IDataObjectImpl<CTenderObj>,
	public CComControl<CTenderObj>,
	public ITBStream
{
public:

	CTenderObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CTenderObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CTenderObj)
	CHAIN_MSG_MAP(CComControl<CTenderObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ITenderObj
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
		LPCTSTR pszText = _T("ATL 7.0 : TenderObj");
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
   CTenderDetailsMsg m_TenderDetailsMsg;

   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
   STDMETHOD(ParseStream)(VARIANT i_varpBuf);
   STDMETHOD(ParseBuffer)(BYTE* pBuffer);
   STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
   STDMETHOD(get_Description)(BSTR* pVal);
   STDMETHOD(put_Description)(BSTR newVal);
   STDMETHOD(get_Type)(LONG* pVal);
   STDMETHOD(put_Type)(LONG newVal);
   STDMETHOD(get_Amount)(BSTR* pVal);
   STDMETHOD(put_Amount)(BSTR newVal);
   STDMETHOD(get_Balance)(BSTR* pVal);
   STDMETHOD(put_Balance)(BSTR newVal);
   STDMETHOD(get_ChangeDue)(BSTR* pVal);
   STDMETHOD(put_ChangeDue)(BSTR newVal);
   STDMETHOD(get_Accepted)(BYTE* pVal);
   STDMETHOD(put_Accepted)(BYTE newVal);
   STDMETHOD(get_Voided)(BYTE* pVal);
   STDMETHOD(put_Voided)(BYTE newVal);
   STDMETHOD(get_CardRemainingBalance)(BSTR* pVal);
   STDMETHOD(put_CardRemainingBalance)(BSTR newVal);
   STDMETHOD(get_ErrorDescription)(BSTR* pVal);
   STDMETHOD(put_ErrorDescription)(BSTR newVal);
};

