// RewardObj.h : Declaration of the CRewardObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "rewardLineDetailsMsg.h"
#include "TBStream.h"

[export]
typedef enum
{
	BOTTOM_OF_RECEIPT = 1,
	ABOVE_REF_ENTRYID,
	BELOW_REF_ENTRYID,
	POS_NONE
} tRewardLinePosType;


// IRewardObj
[
	object,
	uuid(83398A76-DB40-412F-AB39-34EC9A187C59),
	dual,
	helpstring("IRewardObj Interface"),
	pointer_default(unique)
]
__interface IRewardObj : public IDispatch
{
	[propget, id(1), helpstring("property RewardValue")] HRESULT RewardValue([out, retval] LONG* pVal);
	[propput, id(1), helpstring("property RewardValue")] HRESULT RewardValue([in] LONG newVal);
	[propget, id(2), helpstring("property RewardPosition")] HRESULT RewardPosition([out, retval] tRewardLinePosType* pVal);
	[propput, id(2), helpstring("property RewardPosition")] HRESULT RewardPosition([in] tRewardLinePosType newVal);
	[propget, id(3), helpstring("property IsVoided")] HRESULT IsVoided([out, retval] BYTE* pVal);
	[propput, id(3), helpstring("property IsVoided")] HRESULT IsVoided([in] BYTE newVal);
	[propget, id(4), helpstring("property EntryId")] HRESULT EntryId([out, retval] LONG* pVal);
	[propput, id(4), helpstring("property EntryId")] HRESULT EntryId([in] LONG newVal);
	[propget, id(5), helpstring("property UpdateRewardTotal")] HRESULT UpdateRewardTotal([out, retval] BYTE* pVal);
	[propput, id(5), helpstring("property UpdateRewardTotal")] HRESULT UpdateRewardTotal([in] BYTE newVal);
	[propget, id(6), helpstring("property Description")] HRESULT Description([out, retval] BSTR* pVal);
	[propput, id(6), helpstring("property Description")] HRESULT Description([in] BSTR newVal);
	[propget, id(7), helpstring("property AssociatedEntryId")] HRESULT AssociatedEntryId([out, retval] LONG* pVal);
	[propput, id(7), helpstring("property AssociatedEntryId")] HRESULT AssociatedEntryId([in] LONG newVal);
	[propget, id(8), helpstring("property ShowValue")] HRESULT ShowValue([out, retval] SHORT* pVal);
	[propput, id(8), helpstring("property ShowValue")] HRESULT ShowValue([in] SHORT newVal);
};


// CRewardObj
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.RewardObj"),
	progid("tbControls.RewardObj.1"),
	version(1.0),
	uuid("CC493BBE-BBB2-4E5F-92E7-83170080B195"),
	helpstring("RewardObj Class"),
	support_error_info(IRewardObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CRewardObj : 
	public IRewardObj,
	public IPersistStreamInitImpl<CRewardObj>,
	public IOleControlImpl<CRewardObj>,
	public IOleObjectImpl<CRewardObj>,
	public IOleInPlaceActiveObjectImpl<CRewardObj>,
	public IViewObjectExImpl<CRewardObj>,
	public IOleInPlaceObjectWindowlessImpl<CRewardObj>,
	public IPersistStorageImpl<CRewardObj>,
	public ISpecifyPropertyPagesImpl<CRewardObj>,
	public IQuickActivateImpl<CRewardObj>,
	public IDataObjectImpl<CRewardObj>,
	public IProvideClassInfo2Impl<&__uuidof(CRewardObj), NULL>,
	public CComControl<CRewardObj>,
   public ITBStream
{
public:

	CRewardObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CRewardObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CRewardObj)
	CHAIN_MSG_MAP(CComControl<CRewardObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IRewardObj
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
		LPCTSTR pszText = _T("ATL 7.0 : RewardObj");
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

   CRewardLineDetailsMsg m_rewardMsg;
   STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
	STDMETHOD(ParseStream)(VARIANT i_varpBuf);
	STDMETHOD(ParseBuffer)(BYTE *pBuf);
	STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);

	STDMETHOD(get_RewardValue)(LONG* pVal);
	STDMETHOD(put_RewardValue)(LONG newVal);
	STDMETHOD(get_RewardPosition)(tRewardLinePosType* pVal);
	STDMETHOD(put_RewardPosition)(tRewardLinePosType newVal);
	STDMETHOD(get_IsVoided)(BYTE* pVal);
	STDMETHOD(put_IsVoided)(BYTE newVal);
	STDMETHOD(get_EntryId)(LONG* pVal);
	STDMETHOD(put_EntryId)(LONG newVal);
	STDMETHOD(get_UpdateRewardTotal)(BYTE* pVal);
	STDMETHOD(put_UpdateRewardTotal)(BYTE newVal);
	STDMETHOD(get_Description)(BSTR* pVal);
	STDMETHOD(put_Description)(BSTR newVal);
	STDMETHOD(get_AssociatedEntryId)(LONG* pVal);
	STDMETHOD(put_AssociatedEntryId)(LONG newVal);
	STDMETHOD(get_ShowValue)(SHORT* pVal);
	STDMETHOD(put_ShowValue)(SHORT newVal);
};

