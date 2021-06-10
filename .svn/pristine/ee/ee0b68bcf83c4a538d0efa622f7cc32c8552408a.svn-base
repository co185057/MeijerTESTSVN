// TotalObj.h : Declaration of the CTotalObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "totaldetailsmsg.h"
#include "TBStream.h"

// ITotalObj
[
	object,
	uuid(EB2C6ECD-2D7D-4626-B8FE-264738AA036E),
	dual,
	helpstring("ITotalObj Interface"),
	pointer_default(unique)
]
__interface ITotalObj : public IDispatch
{
   [propget, id(1), helpstring("property TaxDue")] HRESULT TaxDue([out, retval] BSTR* pVal);
   [propput, id(1), helpstring("property TaxDue")] HRESULT TaxDue([in] BSTR newVal);
   [propget, id(2), helpstring("property TotalDue")] HRESULT TotalDue([out, retval] BSTR* pVal);
   [propput, id(2), helpstring("property TotalDue")] HRESULT TotalDue([in] BSTR newVal);
   [propget, id(3), helpstring("property TotalDiscount")] HRESULT TotalDiscount([out, retval] BSTR* pVal);
   [propput, id(3), helpstring("property TotalDiscount")] HRESULT TotalDiscount([in] BSTR newVal);
   [propget, id(4), helpstring("property ErrorDescription")] HRESULT ErrorDescription([out, retval] BSTR* pVal);
   [propput, id(4), helpstring("property ErrorDescription")] HRESULT ErrorDescription([in] BSTR newVal);
   [propget, id(5), helpstring("property FoodStampAmount")] HRESULT FoodStampAmount([out, retval] BSTR* pVal);
   [propput, id(5), helpstring("property FoodStampAmount")] HRESULT FoodStampAmount([in] BSTR newVal);
   [propget, id(6), helpstring("property RewardPoints")] HRESULT RewardPoints([out, retval] LONG* pVal);
   [propput, id(6), helpstring("property RewardPoints")] HRESULT RewardPoints([in] LONG newVal);
};


// CTotalObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.TotalObj"),
	progid("tbControls.TotalObj.1"),
	version(1.0),
	uuid("E1354D9A-EBFF-479C-9EA2-2C41D6C2BB1E"),
	helpstring("TotalObj Class"),
	support_error_info(ITotalObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CTotalObj : 
	public ITotalObj,
	public IPersistStreamInitImpl<CTotalObj>,
	public IOleControlImpl<CTotalObj>,
	public IOleObjectImpl<CTotalObj>,
	public IOleInPlaceActiveObjectImpl<CTotalObj>,
	public IViewObjectExImpl<CTotalObj>,
	public IOleInPlaceObjectWindowlessImpl<CTotalObj>,
	public IPersistStorageImpl<CTotalObj>,
	public ISpecifyPropertyPagesImpl<CTotalObj>,
	public IQuickActivateImpl<CTotalObj>,
	public IDataObjectImpl<CTotalObj>,
	public CComControl<CTotalObj>,
	public ITBStream
{
public:

	CTotalObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CTotalObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CTotalObj)
	CHAIN_MSG_MAP(CComControl<CTotalObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ITotalObj
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
		LPCTSTR pszText = _T("ATL 7.0 : TotalObj");
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
private:
   CTotalDetailsMsg m_TotalDetailsMsg;
public:
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
   STDMETHOD(ParseStream)(VARIANT i_varpBuf);
   STDMETHOD(ParseBuffer)(BYTE* pBuffer);
   STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
   STDMETHOD(get_TaxDue)(BSTR* pVal);
   STDMETHOD(put_TaxDue)(BSTR newVal);
   STDMETHOD(get_TotalDue)(BSTR* pVal);
   STDMETHOD(put_TotalDue)(BSTR newVal);
   STDMETHOD(get_TotalDiscount)(BSTR* pVal);
   STDMETHOD(put_TotalDiscount)(BSTR newVal);
   STDMETHOD(get_ErrorDescription)(BSTR* pVal);
   STDMETHOD(put_ErrorDescription)(BSTR newVal);
   STDMETHOD(get_FoodStampAmount)(BSTR* pVal);
   STDMETHOD(put_FoodStampAmount)(BSTR newVal);
   STDMETHOD(get_RewardPoints)(LONG* pVal);
   STDMETHOD(put_RewardPoints)(LONG newVal);
};

