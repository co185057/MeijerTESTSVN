// Created by Microsoft (R) C/C++ Compiler Version 14.00.50727.42
//
// c:\fastlane\controls\dll - tbcontrols\windowmsg.mrg.h
// compiler-generated file created 10/23/06 at 15:22:47
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

// WindowMsg.h : Declaration of the CWindowMsg
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>

// IWindowMsg
[
	object,
	uuid(13979CBF-F2A8-471D-BF62-72E7A19B88C7),
	dual,
	helpstring("IWindowMsg Interface"),
	pointer_default(unique)
]
__interface IWindowMsg : public IDispatch
{
};


// CWindowMsg
[
	coclass,
	threading(apartment),
	vi_progid("tbControls.WindowMsg"),
	progid("tbControls.WindowMsg.1"),
	version(1.0),
	uuid("8BA51575-F22F-426D-A4D2-5D3301F086F4"),
	helpstring("WindowMsg Class"),
	support_error_info(IWindowMsg),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CWindowMsg : 
	public IWindowMsg,
	public IPersistStreamInitImpl<CWindowMsg>,
	public IOleControlImpl<CWindowMsg>,
	public IOleObjectImpl<CWindowMsg>,
	public IOleInPlaceActiveObjectImpl<CWindowMsg>,
	public IViewObjectExImpl<CWindowMsg>,
	public IOleInPlaceObjectWindowlessImpl<CWindowMsg>,
	public IPersistStorageImpl<CWindowMsg>,
	public ISpecifyPropertyPagesImpl<CWindowMsg>,
	public IQuickActivateImpl<CWindowMsg>,
	public IDataObjectImpl<CWindowMsg>,
	public IProvideClassInfo2Impl<&__uuidof(CWindowMsg), NULL>,
	public CComControl<CWindowMsg>
,
    /*+++ Added Baseclass */ public ISupportErrorInfo
{
public:

	CWindowMsg()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CWindowMsg)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CWindowMsg)
	CHAIN_MSG_MAP(CComControl<CWindowMsg>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IWindowMsg
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
		LPCTSTR pszText = _T("ATL 7.0 : WindowMsg");
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

	//+++ Start Injected Code For Attribute 'support_error_info'
#injected_line 21 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    virtual HRESULT STDMETHODCALLTYPE IWindowMsg::Invoke(
                /* [in] */ DISPID dispIdMember,
                /* [in] */ REFIID riid,
                /* [in] */ LCID lcid,
                /* [in] */ WORD wFlags,
                /* [out][in] */ DISPPARAMS *pDispParams,
                /* [out] */ VARIANT *pVarResult,
                /* [out] */ EXCEPINFO *pExcepInfo,
                /* [out] */ UINT *puArgErr) 
    {
        (void) riid;
        (void) dispIdMember;
        (void) lcid;
        (void) wFlags;
        (void) pExcepInfo;
        (void) puArgErr;
        HRESULT hr = S_OK;
        if (pDispParams == 0) {
            return DISP_E_BADVARTYPE;
        }
        if (pDispParams->cArgs > 0) {
            return DISP_E_BADPARAMCOUNT;
        }
        if (pVarResult != 0) {
            ::VariantInit(pVarResult);
        }
        return DISP_E_MEMBERNOTFOUND;
    }
    virtual HRESULT STDMETHODCALLTYPE IWindowMsg::GetIDsOfNames(
                /* [in] */ REFIID riid,
                /* [size_is][in] */ LPOLESTR *rgszNames,
                /* [in] */ UINT cNames,
                /* [in] */ LCID lcid,
                /* [size_is][out] */ DISPID *rgDispId) 
    {
        (void) riid;
        (void) rgszNames;
        (void) cNames;
        (void) lcid;
        (void) rgDispId;
        return DISP_E_UNKNOWNNAME;
    }
    HRESULT TypeInfoHelper(REFIID iidDisp, LCID /*lcid*/, ITypeInfo** ppTypeInfo) 
    {
        if (ppTypeInfo == NULL) {
            return E_POINTER;
        }
        *ppTypeInfo = NULL;
        TCHAR szModule1[_MAX_PATH];
        int nLen = ::GetModuleFileName(_AtlBaseModule.GetModuleInstance(), szModule1, _MAX_PATH);
        if (nLen == 0 || nLen == _MAX_PATH) {
            return E_FAIL;
        }
        USES_CONVERSION_EX;
        LPOLESTR pszModule = T2OLE_EX(szModule1, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
#ifndef _UNICODE
        if (pszModule == NULL) {
            return E_OUTOFMEMORY;
        }
#endif
        CComPtr<ITypeLib> spTypeLib;
        HRESULT hr = LoadTypeLib(pszModule, &spTypeLib);
        if (SUCCEEDED(hr)) {
            CComPtr<ITypeInfo> spTypeInfo;
            hr = spTypeLib->GetTypeInfoOfGuid(iidDisp, &spTypeInfo);
            if (SUCCEEDED(hr)) {
                *ppTypeInfo = spTypeInfo.Detach();
            }
        }
        return hr;
    }
    virtual HRESULT STDMETHODCALLTYPE IWindowMsg::GetTypeInfoCount(unsigned int*  pctinfo) 
    {
        if (pctinfo == NULL) {
            return E_POINTER;
        }
        CComPtr<ITypeInfo> spTypeInfo;
        *pctinfo = 
                       (SUCCEEDED(TypeInfoHelper(__uuidof(IWindowMsg), 0, &spTypeInfo))) ? 1 : 0;
        return S_OK;
    }
    virtual HRESULT STDMETHODCALLTYPE IWindowMsg::GetTypeInfo(unsigned int iTInfo, LCID lcid, ITypeInfo** ppTInfo) 
    {
        if (iTInfo != 0) {
            return DISP_E_BADINDEX;
        }
        return TypeInfoHelper(__uuidof(IWindowMsg), lcid, ppTInfo);
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static DWORD* GetOpCodes() 
    {
        static DWORD rgOps[] = 
            					{
            						0x70000000,
            0x30004000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x50000000,
            0x60000000,
            0x60000000,
            0x30014000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x50000000,
            0x60000000,
            0x30018000,
            0x80000001,
            0x50000000,
            0x60000000,
            0x60000000,
            0x1000c000,
            0x3000c000,
            0x50000000,
            0x20010000,
            0x30010000,
            0x80000002,
            0x50000000,
            0x3001c000,
            0x80000001,
            0x50000000,
            0x60000000,
            0x30020000,
            0x80000005,
            0x50000000,
            0x60000000,
            0x30024000,
            0x8000000a,
            0x50000000,
            0x8002c00c,
            0x60000000,
            0x20034000,
            0x30034000,
            0x50000000,
            0x60000000,
            0x20038000,
            0x30038000,
            0x8000000f,
            0x50000000,
            0x60000000,
            0x30040000,
            0x80000011,
            0x50000000,
            0x30048000,
            0x80000013,
            0x50000000,
            0x60000000,
            0x60000000,
            0x80050015,
            0x30058000,
            0x80000017,
            0x50000000,
            0x60000000,
            0x30060000,
            0x80000019,
            0x50000000,
            0x60000000,
            0x60000000,
            0x60000000,
            0x60000000,
            0
            					};
        return rgOps;
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static RGSDWORD* GetOpcodeDWORDVals() 
    {
        static RGSDWORD rgDWORDS[] = 
            					{
            						{0, 0}
            					};
        return rgDWORDS;
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static RGSBinary* GetOpcodeBinaryVals() 
    {

        static RGSBinary rgBinary[] = 
            					{
            						{0, 0}
            					};
        return rgBinary;
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static RGSStrings* GetOpcodeStringVals() 
    {
        static RGSStrings rgStrings[] = 
            					{
            						{_T(""),0 }, 
            {_T("tbControls.WindowMsg.1"),0 }, 
            {_T("%FriendlyName%"),1 }, 
            {_T("CLSID"),0 }, 
            {_T("{8BA51575-F22F-426D-A4D2-5D3301F086F4}"),0 }, 
            {_T("tbControls.WindowMsg"),0 }, 
            {_T("CurVer"),0 }, 
            {_T("ProgID"),0 }, 
            {_T("VersionIndependentProgID"),0 }, 
            {_T("%MODULETYPE%"),1 }, 
            {_T("%MODULE%"),1 }, 
            {_T("ThreadingModel"),0 }, 
            {_T("apartment"),0 }, 
            {_T("Control"),0 }, 
            {_T("ToolboxBitmap32"),0 }, 
            {_T("%MODULE%, 1"),1 }, 
            {_T("MiscStatus"),0 }, 
            {_T("0"),0 }, 
            {_T("1"),0 }, 
            {_T("131473"),0 }, 
            {_T("AppID"),0 }, 
            {_T("%APPID%"),1 }, 
            {_T("TypeLib"),0 }, 
            {_T("%MODULEGUID%"),1 }, 
            {_T("Version"),0 }, 
            {_T("1.0"),0 }, 
            {NULL, 0}
            					};
        return rgStrings;
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister) 
    {
        CRegistryVirtualMachine rvm;
        HRESULT hr;
        if (FAILED(hr = rvm.AddStandardReplacements()))
            return hr;
        rvm.AddReplacement(_T("FriendlyName"), GetObjectFriendlyName());
        return rvm.VMUpdateRegistry(GetOpCodes(), GetOpcodeStringVals(), GetOpcodeDWORDVals(), GetOpcodeBinaryVals(), bRegister);
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static const TCHAR* GetObjectFriendlyName() 
    {
        return _T("CWindowMsg Object");
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static const TCHAR* GetProgID() 
    {
        return _T("tbControls.WindowMsg.1");
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    static const TCHAR* GetVersionIndependentProgID() 
    {
        return _T("tbControls.WindowMsg");
    }
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"
    BEGIN_COM_MAP(CWindowMsg)
        COM_INTERFACE_ENTRY(IWindowMsg)
        COM_INTERFACE_ENTRY(IDispatch)
        COM_INTERFACE_ENTRY(IPersistStreamInit)
        COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
        COM_INTERFACE_ENTRY(IOleControl)
        COM_INTERFACE_ENTRY(IOleObject)
        COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
        COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceActiveObject)
        COM_INTERFACE_ENTRY(IViewObjectEx)
        COM_INTERFACE_ENTRY(IViewObject2)
        COM_INTERFACE_ENTRY(IViewObject)
        COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
        COM_INTERFACE_ENTRY(IOleInPlaceObject)
        COM_INTERFACE_ENTRY(IPersistStorage)
        COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
        COM_INTERFACE_ENTRY(IQuickActivate)
        COM_INTERFACE_ENTRY(IDataObject)
        COM_INTERFACE_ENTRY(IProvideClassInfo2)
        COM_INTERFACE_ENTRY(IProvideClassInfo)
        COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"

    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) 
        					
    {
        static const IID* rgErrorGuids[] = 
            						{
            							&__uuidof(IWindowMsg)
            						};
        for (int i=0; i < sizeof(rgErrorGuids) / sizeof(rgErrorGuids[0]); i++)
						{
            if (InlineIsEqualGUID(*rgErrorGuids[i],riid))
                return S_OK;
        }
        return S_FALSE;
    } 

	//--- End Injected Code For Attribute 'support_error_info'
};

//+++ Start Injected Code For Attribute 'support_error_info'
#injected_line 28 "c:\\fastlane\\controls\\dll - tbcontrols\\windowmsg.h"

				OBJECT_ENTRY_AUTO(__uuidof(CWindowMsg), CWindowMsg)
			
//--- End Injected Code For Attribute 'support_error_info'


