// tbConnect.h : Declaration of the CtbConnect
#if !defined (TBCONNECT_H)
#define TBCONNECT_H
#pragma once

#include "resource.h"       // main symbols
#include "ConnectSocket.h"
#include "ItemObj.h"
#include <atlctl.h>
#include "ScannerMsg.h"
#include "CashDrawerObj.h"
#include "TotalObj.h"
#include "TenderObj.h"
#include "RewardObj.h"
#include "ScannerObj.h"
#include "KeylockObj.h"
#include "KeyboardObj.h"
#include "MSRObj.h"
#include "MICRObj.h"
#include "PrinterObj.h"
#include "PinPadObj.h"
#include "ScaleObj.h"
#include "CustomObj.h"
#include "TBGenObj.h"

#define WM_RECEIVE_EVENT WM_USER + 101

[export]
typedef enum  {
	ReceiveEvent				= 0,
	ReceiveItemObjEvent			= 1,
	ReceiveCashDrawerObjEvent	= 2,
	ReceiveTotalObjEvent		= 3,
	ReceiveTenderObjEvent		= 4,
	ReceiveRewardObjEvent		= 5,
	ReceiveScannerObjEvent		= 6,
	ReceiveKeyboardObjEvent		= 7,
	ReceiveKeylockObjEvent		= 8,
	ReceiveMSRObjEvent			= 9,
	ReceiveMICRObjEvent			= 10,
	ReceivePrinterObjEvent		= 11,
	ReceivePinpadObjEvent		= 12,
	ReceiveScaleObjEvent		= 13,
	ReceiveCustomObjEvent		= 14,
	ReceiveTBGenObjEvent		= 15
} tbConnectReceiveEvent;

// ItbConnect
[
	object,
	uuid(A4D47620-0FA3-4C43-B3F8-0F726CE22E51),
	dual,
	helpstring("ItbConnect Interface"),
	pointer_default(unique)
]
__interface ItbConnect : public IDispatch
{
	[id(1), helpstring("Connect to TB"), helpcontext(1)] HRESULT Connect(void);
	[id(2), helpstring("method Send")] HRESULT Send(ITBStream * pObj);
	[id(3), helpstring("method ConnectAsTB")] HRESULT ConnectAsTB(void);
	[id(4), helpstring("method SetAck")] HRESULT SetAck([in] LONG nAck);
	[id(5), helpstring("method SubscribeTo")] HRESULT SubscribeTo([in] BSTR szCommaDelimHexDeviceIds);
	[id(6), helpstring("method Disconnect")] HRESULT Disconnect(void);
    [id(7), helpstring("method SetCallback")] HRESULT SetCallback([in] VARIANT vCallbackProc);
};


// _ItbConnectEvents
[
	uuid("F82352BA-F302-41C9-99FF-9E0930C2A3BC"),
	dispinterface,
	helpstring("_ItbConnectEvents Interface")
]
__interface _ItbConnectEvents
{
	[id(1),  helpstring("method OnReceive")] HRESULT OnReceive([in] BYTE* pBuf, [in] LONG length);
	[id(2),  helpstring("method Receive")] HRESULT Receive();
	[id(3),  helpstring("method ReceiveItemObj")] HRESULT ReceiveItemObj(IItemObj* i_pItemObj);
	[id(4),  helpstring("method ReceiveCashDrawerObj")] HRESULT ReceiveCashDrawerObj([in] ICashDrawerObj * pCashDrawer);
	[id(5),  helpstring("method ReceiveTotalObj")] HRESULT ReceiveTotalObj([in] ITotalObj * pTotal);
	[id(6),  helpstring("method ReceiveTenderObj")] HRESULT ReceiveTenderObj([in] ITenderObj * pTender);
	[id(7),  helpstring("method ReceiveRewardObj")] HRESULT ReceiveRewardObj([in] IRewardObj * pReward);
	[id(8),  helpstring("method ReceiveScannerObj")] HRESULT ReceiveScannerObj( IScannerObj * pScanner);
	[id(9),  helpstring("method ReceiveKeyboardObj")] HRESULT ReceiveKeyboardObj([in] IKeyboardObj * pKeyboard);
	[id(10), helpstring("method ReceiveKeylockObj")] HRESULT ReceiveKeylockObj([in] IKeylockObj * pKeylock);
	[id(11), helpstring("method ReceiveMSRObj")] HRESULT ReceiveMSRObj(IMSRObj* pMSRObj);
	[id(12), helpstring("method ReceiveMICRObj")] HRESULT ReceiveMICRObj(IMICRObj* pMICRObj);
	[id(13), helpstring("method ReceivePrinterObj")] HRESULT ReceivePrinterObj(IPrinterObj* pPrinterObj);
	[id(14), helpstring("method ReceivePinpadObj")] HRESULT ReceivePinpadObj(IPinpadObj* pPinpadMsg);
	[id(15), helpstring("method ReceiveScaleObj")] HRESULT ReceiveScaleObj(IScaleObj* pScaleObj);
	[id(16), helpstring("method ReceiveCustomObj")] HRESULT ReceiveCustomObj(ICustomObj* pCustom);
	[id(17), helpstring("method ReceiveTBGenObj")] HRESULT ReceiveTBGenObj([in] ITBGenObj * pGenObj);
};

// CtbConnect
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.tbConnect"),
	progid("tbControls.tbConnect.1"),
	version(1.0),
	uuid("FEA44A8A-E5BC-4BB5-A2AC-34C7A0B29703"),
	helpstring("tbConnect Class"),
	event_source(com),
	support_error_info(ItbConnect),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CtbConnect : 
	public ItbConnect,
	public IPersistStreamInitImpl<CtbConnect>,
	public IOleControlImpl<CtbConnect>,
	public IOleObjectImpl<CtbConnect>,
	public IOleInPlaceActiveObjectImpl<CtbConnect>,
	public IViewObjectExImpl<CtbConnect>,
	public IOleInPlaceObjectWindowlessImpl<CtbConnect>,
	public IPersistStorageImpl<CtbConnect>,
	public ISpecifyPropertyPagesImpl<CtbConnect>,
	public IQuickActivateImpl<CtbConnect>,
	public IDataObjectImpl<CtbConnect>,
	public CComControl<CtbConnect>
{
public:

	CtbConnect()
		: m_bAck(false)
		, m_csServerName(_T(""))
		, m_csDeviceIDs(_T(""))
		, m_nPortNumberRead(0)
		, m_nPortNumberWrite(0)
		, m_nTBPortNumberRead(0)
		, m_nTBPortNumberWrite(0)
		, m_pfnEventCallbackProc(NULL)
	{
	}

	DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	//	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE | 
		OLEMISC_INSIDEOUT | 
		OLEMISC_ACTIVATEWHENVISIBLE | 
		OLEMISC_SETCLIENTSITEFIRST
		)


		BEGIN_PROP_MAP(CtbConnect)
			PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
			PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
			// Example entries
			// PROP_ENTRY("Property Description", dispid, clsid)
			// PROP_PAGE(CLSID_StockColorPage)
		END_PROP_MAP()

		BEGIN_MSG_MAP(CtbConnect)
			MESSAGE_HANDLER(WM_RECEIVE_EVENT, OnReceiveEvent)
			CHAIN_MSG_MAP(CComControl<CtbConnect>)
			DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
		// Handler prototypes:
		//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
		//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


		// IViewObjectEx
		DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)


	// ItbConnect
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
		LPCTSTR pszText = _T("ATL 7.0 : tbConnect");
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
		CoInitialize(NULL );
		m_critSectReceiveEvent.Init();
		return S_OK;
	}

	void FinalRelease() 
	{
		if (m_hWnd)
			::DestroyWindow(m_hWnd);
		m_critSectReceiveEvent.Term();
		CoUninitialize();
	}
	virtual HWND CreateControlWindow(HWND hWndParent, RECT &rcPos)
	{
		return Create(hWndParent, rcPos, NULL, WS_CHILD, 0);
	}

	// CMyControl is derived from CComControl
	STDMETHOD(SetClientSite)(IOleClientSite *pClientSite)
	{
		if (pClientSite)
		{
			RECT rc = {0,0,0,0};
			// Don't have access to the container's window so just use the
			// desktop.  Window will be resized correctly during in-place
			// activation.
			HWND hWnd = CreateControlWindow(::GetDesktopWindow(), rc);
			_ASSERT (hWnd);
		}
		return IOleObjectImpl<CtbConnect>::SetClientSite (pClientSite);
	}   
	HRESULT InPlaceActivate(LONG iVerb, const RECT* prcPosRect)
	{      // Get the container's window.
		_ASSERT (m_spClientSite);
		LPOLEINPLACESITE pInPlaceSite = NULL;
		HRESULT hr = m_spClientSite->QueryInterface(IID_IOleInPlaceSite,
			(void**)&pInPlaceSite);
		_ASSERT (SUCCEEDED (hr) && pInPlaceSite);
		HWND hParent = NULL;
		hr = pInPlaceSite->GetWindow (&hParent);
		_ASSERT (SUCCEEDED (hr) && hParent);
		pInPlaceSite->Release ();
		// Set container window as our parent window
		SetParent (hParent);
		return CComControlBase::InPlaceActivate(iVerb, prcPosRect);
	} 

private:
	bool m_bAck;
	CString m_csServerName;
	CString m_csDeviceIDs;
	int m_nPortNumberRead;
	int m_nPortNumberWrite;
	int m_nTBPortNumberRead;
	int m_nTBPortNumberWrite;
	CConnectSocket m_ReadSocket;
	CConnectSocket m_WriteSocket;
	CComCriticalSection m_critSectReceiveEvent;

protected:
	typedef void (WINAPI *LPEVENTCALLBACKPROC)(tbConnectReceiveEvent tbcre, IDispatch * pObj);
	LPEVENTCALLBACKPROC m_pfnEventCallbackProc;

private:
	int StartReadSocket(bool bConnectAsTB =false);
	int StartWriteSocket(bool bConnectAsTB =false);
	void StartReadThread(void);
private:
	static long WINAPI ReceiveThreadFn(LPVOID lparam);
	void LoadRegistrySettings(void);
	__event __interface _ItbConnectEvents;
public:
	STDMETHOD(Connect)(void);
	STDMETHOD(Send)(ITBStream * pObj);
	STDMETHOD(ConnectAsTB)(void);
	LRESULT OnReceiveEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	STDMETHOD(SetAck)(LONG nAck);
	STDMETHOD(SubscribeTo)(BSTR szCommaDelimHexDeviceIds);
	STDMETHOD(Disconnect)(void);
	STDMETHOD(SetCallback)(VARIANT vCallProc);
};

#endif