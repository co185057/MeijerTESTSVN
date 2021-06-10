// MICRObj.h : Declaration of the CMICRObj
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "TBStream.h"
#include "MicrMsg.h"

[export]
typedef enum { CHECK_PERSONAL = 0x01,
                  CHECK_BUSINESS = 0x02,
                  CHECK_UNKNOWN  = 0x99
   } CheckTypes;

[export]
typedef enum { CC_US  = 0x01,
                  CC_CAN = 0x02,
                  CC_MEX = 0x03,
                  CC_UNK = 0x99
   } CountryCodes;

   // IMICRObj
[
	object,
	uuid(C6F93406-B84F-45D3-BFB2-BB422C48D68F),
	dual,
	helpstring("IMICRObj Interface"),
	pointer_default(unique)
]
__interface IMICRObj : public IDispatch
{
	[propget, id(1), helpstring("property TransitNumber")] HRESULT TransitNumber([out, retval] BSTR* pVal);
	[propput, id(1), helpstring("property TransitNumber")] HRESULT TransitNumber([in] BSTR newVal);
	[propget, id(2), helpstring("property AccountNumber")] HRESULT AccountNumber([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("property AccountNumber")] HRESULT AccountNumber([in] BSTR newVal);
	[propget, id(3), helpstring("property BankNumber")] HRESULT BankNumber([out, retval] BSTR* pVal);
	[propput, id(3), helpstring("property BankNumber")] HRESULT BankNumber([in] BSTR newVal);
	[propget, id(4), helpstring("property SerialNumber")] HRESULT SerialNumber([out, retval] BSTR* pVal);
	[propput, id(4), helpstring("property SerialNumber")] HRESULT SerialNumber([in] BSTR newVal);
	[propget, id(5), helpstring("property CheckType")] HRESULT CheckType([out, retval] CheckTypes* pVal);
	[propput, id(5), helpstring("property CheckType")] HRESULT CheckType([in] CheckTypes newVal);
	[propget, id(6), helpstring("property CountryCode")] HRESULT CountryCode([out, retval] CountryCodes* pVal);
	[propput, id(6), helpstring("property CountryCode")] HRESULT CountryCode([in] CountryCodes newVal);
	[propget, id(7), helpstring("property Data")] HRESULT Data([out, retval] BSTR* pVal);
	[propput, id(7), helpstring("property Data")] HRESULT Data([in] BSTR newVal);
};


// CMICRObj
[
	coclass,
	threading("apartment"),
	vi_progid("tbControls.MICRObj"),
	progid("tbControls.MICRObj.1"),
	version(1.0),
	uuid("B5CF4A4A-4EB0-402B-BA77-7CB8E13DEEC4"),
	helpstring("MICRObj Class"),
	support_error_info(IMICRObj),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMICRObj : 
	public IMICRObj,
	public IPersistStreamInitImpl<CMICRObj>,
	public IOleControlImpl<CMICRObj>,
	public IOleObjectImpl<CMICRObj>,
	public IOleInPlaceActiveObjectImpl<CMICRObj>,
	public IViewObjectExImpl<CMICRObj>,
	public IOleInPlaceObjectWindowlessImpl<CMICRObj>,
	public IPersistStorageImpl<CMICRObj>,
	public ISpecifyPropertyPagesImpl<CMICRObj>,
	public IQuickActivateImpl<CMICRObj>,
	public IDataObjectImpl<CMICRObj>,
	public IProvideClassInfo2Impl<&__uuidof(CMICRObj), NULL>,
	public CComControl<CMICRObj>,
   public ITBStream
{
private:
	CMicrMsg m_MICRMsg;
public:

	CMICRObj()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMICRObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMICRObj)
	CHAIN_MSG_MAP(CComControl<CMICRObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMICRObj
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
		LPCTSTR pszText = _T("ATL 7.0 : MICRObj");
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
	STDMETHOD(get_TransitNumber)(BSTR* pVal);
	STDMETHOD(put_TransitNumber)(BSTR newVal);
	STDMETHOD(get_AccountNumber)(BSTR* pVal);
	STDMETHOD(put_AccountNumber)(BSTR newVal);
	STDMETHOD(get_BankNumber)(BSTR* pVal);
	STDMETHOD(put_BankNumber)(BSTR newVal);
	STDMETHOD(get_SerialNumber)(BSTR* pVal);
	STDMETHOD(put_SerialNumber)(BSTR newVal);
	STDMETHOD(get_CheckType)(CheckTypes* pVal);
	STDMETHOD(put_CheckType)(CheckTypes newVal);
	STDMETHOD(get_CountryCode)(CountryCodes* pVal);
	STDMETHOD(put_CountryCode)(CountryCodes newVal);
	STDMETHOD(get_Data)(BSTR* pVal);
	STDMETHOD(put_Data)(BSTR newVal);
};

