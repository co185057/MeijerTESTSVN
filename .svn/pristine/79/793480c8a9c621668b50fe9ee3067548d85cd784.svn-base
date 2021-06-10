// CashDrawerObj.h : Declaration of the CCashDrawerObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "CashDrawerMsg.h"
#include "TBStream.h"
[export]
typedef enum { DRAWER_A         = 0x01,
				    DRAWER_B         = 0x02,
                DRAWER_UNDEFINED = 0x99
} CashDrawerStations;
[export]
typedef enum { CD_OPEN    = 0x01,
				    CD_CLOSED  = 0x02,
                CD_UNKNOWN = 0x99
} CashDrawerStates;

// ICashDrawerObj
[
	object,
	uuid(3FF844BD-E48C-4E60-B5AF-12827E1F9C21),
	dual,
	helpstring("ICashDrawerObj Interface"),
	pointer_default(unique)
]
__interface ICashDrawerObj : public IDispatch
{
	[propget, id(1), helpstring("property Station")] HRESULT Station([out, retval] CashDrawerStations* pVal);
	[propput, id(1), helpstring("property Station")] HRESULT Station([in] CashDrawerStations newVal);
	[propget, id(2), helpstring("property Status")] HRESULT Status([out, retval] CashDrawerStates* pVal);
	[propput, id(2), helpstring("property Status")] HRESULT Status([in] CashDrawerStates newVal);
};


// CCashDrawerObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.CashDrawerObj"),
	progid("tbControls.CashDrawerObj.1"),
	version(1.0),
	uuid("FF5BEFA5-8E50-4528-8F04-3340D66E6F80"),
	helpstring("CashDrawerObj Class"),
	support_error_info(ICashDrawerObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CCashDrawerObj : 
	public ICashDrawerObj,
	public IPersistStreamInitImpl<CCashDrawerObj>,
	public IOleControlImpl<CCashDrawerObj>,
	public IOleObjectImpl<CCashDrawerObj>,
	public IOleInPlaceActiveObjectImpl<CCashDrawerObj>,
	public IViewObjectExImpl<CCashDrawerObj>,
	public IOleInPlaceObjectWindowlessImpl<CCashDrawerObj>,
	public IPersistStorageImpl<CCashDrawerObj>,
	public ISpecifyPropertyPagesImpl<CCashDrawerObj>,
	public IQuickActivateImpl<CCashDrawerObj>,
	public IDataObjectImpl<CCashDrawerObj>,
	public CComControl<CCashDrawerObj>,
	public ITBStream
{
private:
	CCashDrawerMsg m_CashDrawerMsg;
public:

	CCashDrawerObj()
	{
	}

	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
		OLEMISC_INSIDEOUT | 
		OLEMISC_ACTIVATEWHENVISIBLE | 
		OLEMISC_SETCLIENTSITEFIRST
		)


		BEGIN_PROP_MAP(CCashDrawerObj)
			PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
			PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
			// Example entries
			// PROP_ENTRY("Property Description", dispid, clsid)
			// PROP_PAGE(CLSID_StockColorPage)
		END_PROP_MAP()


		BEGIN_MSG_MAP(CCashDrawerObj)
			CHAIN_MSG_MAP(CComControl<CCashDrawerObj>)
			DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
		// Handler prototypes:
		//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

	// ICashDrawerObj
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
		LPCTSTR pszText = _T("ATL 7.0 : CashDrawerObj");
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
	STDMETHOD(get_Station)(CashDrawerStations* pVal);
	STDMETHOD(put_Station)(CashDrawerStations newVal);
	STDMETHOD(get_Status)(CashDrawerStates* pVal);
	STDMETHOD(put_Status)(CashDrawerStates newVal);

	// ITBStream Methods
public:
	STDMETHOD(StreamOut)(LONG* cb, BYTE** pBuffer);
	STDMETHOD(ParseStream)(VARIANT i_varpBuf);
	STDMETHOD(ParseBuffer)(BYTE *pBuf);
	STDMETHOD(GetMessage)(LONG* o_pTBMsgObj);
};

