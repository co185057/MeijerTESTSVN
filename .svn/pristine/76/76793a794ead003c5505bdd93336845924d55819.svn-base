// ItemObj.h : Declaration of the CItemObj
#if !defined(ITEM_OBJ_H)
#define ITEM_OBJ_H

#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "ItemDetailsMsg.h"
#include "TBStream.h"

// IItemObj
[
	object,
	uuid(2DFDB3BF-5971-4251-AE7E-4173468732B0),
	dual,
	helpstring("IItemObj Interface"),
	pointer_default(unique)
]
__interface IItemObj : public IDispatch
{
   [propget, id(1), helpstring("property IsCoupon")] HRESULT IsCoupon([out, retval] BYTE* pVal);
   [propput, id(1), helpstring("property IsCoupon")] HRESULT IsCoupon([in] BYTE newVal);
   [propget, id(2), helpstring("property IsRestricted")] HRESULT IsRestricted([out, retval] BYTE* pVal);
   [propput, id(2), helpstring("property IsRestricted")] HRESULT IsRestricted([in] BYTE newVal);
   [propget, id(3), helpstring("property IsVoided")] HRESULT IsVoided([out, retval] BYTE* pVal);
   [propput, id(3), helpstring("property IsVoided")] HRESULT IsVoided([in] BYTE newVal);
   [propget, id(4), helpstring("property LinkedItem")] HRESULT LinkedItem([out, retval] BYTE* pVal);
   [propput, id(4), helpstring("property LinkedItem")] HRESULT LinkedItem([in] BYTE newVal);
   [propget, id(5), helpstring("property NeedPrice")] HRESULT NeedPrice([out, retval] BYTE* pVal);
   [propput, id(5), helpstring("property NeedPrice")] HRESULT NeedPrice([in] BYTE newVal);
   [propget, id(6), helpstring("property NeedQuantity")] HRESULT NeedQuantity([out, retval] BYTE* pVal);
   [propput, id(6), helpstring("property NeedQuantity")] HRESULT NeedQuantity([in] BYTE newVal);
   [propget, id(7), helpstring("property NeedWeight")] HRESULT NeedWeight([out, retval] BYTE* pVal);
   [propput, id(7), helpstring("property NeedWeight")] HRESULT NeedWeight([in] BYTE newVal);
   [propget, id(8), helpstring("property NotForSale")] HRESULT NotForSale([out, retval] BYTE* pVal);
   [propput, id(8), helpstring("property NotForSale")] HRESULT NotForSale([in] BYTE newVal);
   [propget, id(9), helpstring("property PIMItem")] HRESULT PIMItem([out, retval] BYTE* pVal);
   [propput, id(9), helpstring("property PIMItem")] HRESULT PIMItem([in] BYTE newVal);
   [propget, id(10), helpstring("property SecurityTag")] HRESULT SecurityTag([out, retval] BYTE* pVal);
   [propput, id(10), helpstring("property SecurityTag")] HRESULT SecurityTag([in] BYTE newVal);
   [propget, id(11), helpstring("property UnknownItem")] HRESULT UnknownItem([out, retval] BYTE* pVal);
   [propput, id(11), helpstring("property UnknownItem")] HRESULT UnknownItem([in] BYTE newVal);
   [propget, id(12), helpstring("property VisualVerify")] HRESULT VisualVerify([out, retval] BYTE* pVal);
   [propput, id(12), helpstring("property VisualVerify")] HRESULT VisualVerify([in] BYTE newVal);
   [propget, id(13), helpstring("property ErrorDescription")] HRESULT ErrorDescription([out, retval] BSTR* pVal);
   [propput, id(13), helpstring("property ErrorDescription")] HRESULT ErrorDescription([in] BSTR newVal);
   [propget, id(14), helpstring("property ExtendedPrice")] HRESULT ExtendedPrice([out, retval] BSTR* pVal);
   [propput, id(14), helpstring("property ExtendedPrice")] HRESULT ExtendedPrice([in] BSTR newVal);
   [propget, id(15), helpstring("property ItemCode"), bindable, defaultbind, displaybind] HRESULT ItemCode([out, retval] BSTR* pVal);
   [propput, id(15), helpstring("property ItemCode"), bindable, defaultbind, displaybind] HRESULT ItemCode([in] BSTR newVal);
   [propget, id(16), helpstring("property ItemDescription")] HRESULT ItemDescription([out, retval] BSTR* pVal);
   [propput, id(16), helpstring("property ItemDescription")] HRESULT ItemDescription([in] BSTR newVal);
   [propget, id(17), helpstring("property Quantity")] HRESULT Quantity([out, retval] BSTR* pVal);
   [propput, id(17), helpstring("property Quantity")] HRESULT Quantity([in] BSTR newVal);
   [propget, id(18), helpstring("property UnitPrice")] HRESULT UnitPrice([out, retval] BSTR* pVal);
   [propput, id(18), helpstring("property UnitPrice")] HRESULT UnitPrice([in] BSTR newVal);
   [propget, id(19), helpstring("property Weight")] HRESULT Weight([out, retval] BSTR* pVal);
   [propput, id(19), helpstring("property Weight")] HRESULT Weight([in] BSTR newVal);
   [propget, id(20), helpstring("property Department")] HRESULT Department([out, retval] LONG* pVal);
   [propput, id(20), helpstring("property Department")] HRESULT Department([in] LONG newVal);
   [propget, id(21), helpstring("property EntryID")] HRESULT EntryID([out, retval] LONG* pVal);
   [propput, id(21), helpstring("property EntryID")] HRESULT EntryID([in] LONG newVal);
   [propget, id(22), helpstring("property RestrictedAge")] HRESULT RestrictedAge([out, retval] LONG* pVal);
   [propput, id(22), helpstring("property RestrictedAge")] HRESULT RestrictedAge([in] LONG newVal);
   [propget, id(23), helpstring("property TareCode")] HRESULT TareCode([out, retval] LONG* pVal);
   [propput, id(23), helpstring("property TareCode")] HRESULT TareCode([in] LONG newVal);
   [propget, id(24), helpstring("property TaxExempt")] HRESULT TaxExempt([out, retval] BYTE* pVal);
   [propput, id(24), helpstring("property TaxExempt")] HRESULT TaxExempt([in] BYTE newVal);
   [propget, id(25), helpstring("property EndorsementRequired")] HRESULT EndorsementRequired([out, retval] BYTE* pVal);
   [propput, id(25), helpstring("property EndorsementRequired")] HRESULT EndorsementRequired([in] BYTE newVal);
	[id(26), helpstring("method Reset")] HRESULT Reset(void);
	[propget, id(27), helpstring("property UnitQuantity")] HRESULT UnitQuantity([out, retval] BSTR* pVal);
	[propput, id(27), helpstring("property UnitQuantity")] HRESULT UnitQuantity([in] BSTR newVal);
};

// CItemObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.ItemObj"),
	progid("tbControls.ItemObj.1"),
	version(1.0),
	uuid("C8A5701C-082A-4E05-A4B2-E53E08BC0EFB"),
	helpstring("ItemObj Class"),
	support_error_info(IItemObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CItemObj : 
	public IItemObj,
	public IPersistStreamInitImpl<CItemObj>,
	public IOleControlImpl<CItemObj>,
	public IOleObjectImpl<CItemObj>,
	public IOleInPlaceActiveObjectImpl<CItemObj>,
	public IViewObjectExImpl<CItemObj>,
	public IOleInPlaceObjectWindowlessImpl<CItemObj>,
	public IPersistStorageImpl<CItemObj>,
	public ISpecifyPropertyPagesImpl<CItemObj>,
	public IQuickActivateImpl<CItemObj>,
	public IDataObjectImpl<CItemObj>,
	public CComControl<CItemObj>,
	public ITBStream
{
public:

	CItemObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CItemObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CItemObj)
	CHAIN_MSG_MAP(CComControl<CItemObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IItemObj
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
		LPCTSTR pszText = _T("ATL 7.0 : ItemObj");
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
   CItemDetailsMsg m_ItemDetailsMsg;
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
   STDMETHOD(ParseStream)(VARIANT i_varpBuf);
   STDMETHOD(ParseBuffer)(BYTE* pBuffer);
   STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
   STDMETHOD(get_IsCoupon)(BYTE* pVal);
   STDMETHOD(put_IsCoupon)(BYTE newVal);
   STDMETHOD(get_IsRestricted)(BYTE* pVal);
   STDMETHOD(put_IsRestricted)(BYTE newVal);
   STDMETHOD(get_IsVoided)(BYTE* pVal);
   STDMETHOD(put_IsVoided)(BYTE newVal);
   STDMETHOD(get_LinkedItem)(BYTE* pVal);
   STDMETHOD(put_LinkedItem)(BYTE newVal);
   STDMETHOD(get_NeedPrice)(BYTE* pVal);
   STDMETHOD(put_NeedPrice)(BYTE newVal);
   STDMETHOD(get_NeedQuantity)(BYTE* pVal);
   STDMETHOD(put_NeedQuantity)(BYTE newVal);
   STDMETHOD(get_NeedWeight)(BYTE* pVal);
   STDMETHOD(put_NeedWeight)(BYTE newVal);
   STDMETHOD(get_NotForSale)(BYTE* pVal);
   STDMETHOD(put_NotForSale)(BYTE newVal);
   STDMETHOD(get_PIMItem)(BYTE* pVal);
   STDMETHOD(put_PIMItem)(BYTE newVal);
   STDMETHOD(get_SecurityTag)(BYTE* pVal);
   STDMETHOD(put_SecurityTag)(BYTE newVal);
   STDMETHOD(get_UnknownItem)(BYTE* pVal);
   STDMETHOD(put_UnknownItem)(BYTE newVal);
   STDMETHOD(get_VisualVerify)(BYTE* pVal);
   STDMETHOD(put_VisualVerify)(BYTE newVal);
   STDMETHOD(get_ErrorDescription)(BSTR* pVal);
   STDMETHOD(put_ErrorDescription)(BSTR newVal);
   STDMETHOD(get_ExtendedPrice)(BSTR* pVal);
   STDMETHOD(put_ExtendedPrice)(BSTR newVal);
   STDMETHOD(get_ItemCode)(BSTR* pVal);
   STDMETHOD(put_ItemCode)(BSTR newVal);
   STDMETHOD(get_ItemDescription)(BSTR* pVal);
   STDMETHOD(put_ItemDescription)(BSTR newVal);
   STDMETHOD(get_Quantity)(BSTR* pVal);
   STDMETHOD(put_Quantity)(BSTR newVal);
   STDMETHOD(get_UnitPrice)(BSTR* pVal);
   STDMETHOD(put_UnitPrice)(BSTR newVal);
   STDMETHOD(get_Weight)(BSTR* pVal);
   STDMETHOD(put_Weight)(BSTR newVal);
   STDMETHOD(get_Department)(LONG* pVal);
   STDMETHOD(put_Department)(LONG newVal);
   STDMETHOD(get_EntryID)(LONG* pVal);
   STDMETHOD(put_EntryID)(LONG newVal);
   STDMETHOD(get_RestrictedAge)(LONG* pVal);
   STDMETHOD(put_RestrictedAge)(LONG newVal);
   STDMETHOD(get_TareCode)(LONG* pVal);
   STDMETHOD(put_TareCode)(LONG newVal);
   STDMETHOD(get_TaxExempt)(BYTE* pVal);
   STDMETHOD(put_TaxExempt)(BYTE newVal);
   STDMETHOD(get_EndorsementRequired)(BYTE* pVal);
   STDMETHOD(put_EndorsementRequired)(BYTE newVal);
	STDMETHOD(Reset)(void);
	STDMETHOD(get_UnitQuantity)(BSTR* pVal);
	STDMETHOD(put_UnitQuantity)(BSTR newVal);
};

#endif