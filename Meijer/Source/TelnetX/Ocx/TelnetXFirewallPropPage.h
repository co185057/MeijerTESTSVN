// TelnetXFirewallPropPage.h : Declaration of the CTelnetXFirewallPropPage

#ifndef __TELNETXFIREWALLPROPPAGE_H_
#define __TELNETXFIREWALLPROPPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_TelnetXFirewallPropPage;

/////////////////////////////////////////////////////////////////////////////
// CTelnetXFirewallPropPage
class ATL_NO_VTABLE CTelnetXFirewallPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTelnetXFirewallPropPage, &CLSID_TelnetXFirewallPropPage>,
	public IPropertyPageImpl<CTelnetXFirewallPropPage>,
	public CDialogImpl<CTelnetXFirewallPropPage>
{
public:

	int bLoading;

	CTelnetXFirewallPropPage() 
	{
		m_dwTitleID = IDS_TITLETelnetXFirewallPropPage;
		m_dwHelpFileID = IDS_HELPFILETelnetXFirewallPropPage;
		m_dwDocStringID = IDS_DOCSTRINGTelnetXFirewallPropPage;
	}

	enum {IDD = IDD_TELNETXFIREWALLPROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_TELNETXFIREWALLPROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTelnetXFirewallPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTelnetXFirewallPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTelnetXFirewallPropPage>)
	COMMAND_HANDLER(IDC_FIREWALLHOST, EN_CHANGE, OnChangeFirewallhost)
	COMMAND_HANDLER(IDC_FIREWALLLOGONNAME, EN_CHANGE, OnChangeFirewalllogonname)
	COMMAND_HANDLER(IDC_FIREWALLPASSWORD, EN_CHANGE, OnChangeFirewallpassword)
	COMMAND_HANDLER(IDC_FIREWALLPORT, EN_CHANGE, OnChangeFirewallport)
	COMMAND_HANDLER(IDC_FIREWALLTYPE, CBN_SELCHANGE, OnSelchangeFirewalltype)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	void SetDirty(BOOL bDirty)
	{
		m_bDirty = bDirty;
		m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY | PROPPAGESTATUS_VALIDATE);
	}
	
	STDMETHOD(Activate)(HWND hWndParent, LPCRECT pRect, BOOL bModal )
	{
		USES_CONVERSION;
		HRESULT hr = IPropertyPageImpl<CTelnetXFirewallPropPage>::Activate(hWndParent, pRect, bModal );
		_TCHAR strbuff[MAX_PATH+1];
		BSTR bstr;
		short sVal;

		if( hr != S_OK || m_nObjects == 0)
			return hr;

		bLoading=TRUE;

		CComQIPtr<ITelnetX, &IID_ITelnetX> pTelnetX(m_ppUnk[0]);
		
		// let's read Host
		if FAILED(pTelnetX->get_FirewallHost(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_FIREWALLHOST,strbuff);
		SysFreeString(bstr);
		
		// let's read Port
		if FAILED(pTelnetX->get_FirewallPort(&sVal))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		wsprintf(strbuff,_T("%u"),sVal);
		SetDlgItemText(IDC_FIREWALLPORT,strbuff);

		// let's read LogonName
		if FAILED(pTelnetX->get_FirewallLogonName(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_FIREWALLLOGONNAME,strbuff);
		SysFreeString(bstr);
		
		// let's read Password
		if FAILED(pTelnetX->get_FirewallPassword(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_FIREWALLPASSWORD,strbuff);
		SysFreeString(bstr);
		
		FirewallTypeEnum fw;
		// let's read Blocking
		if FAILED(pTelnetX->get_FirewallType(&fw))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("None"));
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Socks 4"));
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Socks 5"));
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Proxy"));
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("SSL Proxy"));
		SendDlgItemMessage(IDC_FIREWALLTYPE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Pipe"));
		switch (fw)
		{
			case FirewallTypeNone:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("None"));
				break;
			case FirewallTypeSocks4:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Socks 4"));
				break;
			case FirewallTypeSocks5:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Socks 5"));
				break;
			case FirewallTypeProxy:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Proxy"));
				break;
			case FirewallTypeSSLProxy:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("SSL Proxy"));
				break;
			case FirewallTypePipe:
				SendDlgItemMessage(IDC_FIREWALLTYPE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Pipe"));
				break;
		}
		bLoading=FALSE;
		return S_OK;
	}
	
	STDMETHOD(Apply)(void)
	{
		USES_CONVERSION;
		ATLTRACE(_T("CFtpServXCtrlPropPageFirewall::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			_TCHAR strbuff[MAX_PATH+1];
			short sVal;

			CComQIPtr<ITelnetX, &IID_ITelnetX> pTelnetX(m_ppUnk[i]);
			CComBSTR str;

			// let's store Host
			GetDlgItemText(IDC_FIREWALLHOST, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_FirewallHost(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Port
			GetDlgItemText(IDC_FIREWALLPORT, strbuff, sizeof(strbuff));
			sVal=(SHORT)atoi(strbuff);
			str = strbuff;
			if FAILED(pTelnetX->put_FirewallPort(sVal))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}


			// let's store Logonname
			GetDlgItemText(IDC_FIREWALLLOGONNAME, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_FirewallLogonName(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Password
			GetDlgItemText(IDC_FIREWALLPASSWORD, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_FirewallPassword(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}
			
			// let's store Firewalltype
			FirewallTypeEnum fw;
			fw=(FirewallTypeEnum) SendDlgItemMessage(IDC_FIREWALLTYPE,CB_GETCURSEL,0,0);
			if FAILED(pTelnetX->put_FirewallType(fw))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}
		}
		SetDirty(FALSE);
		return S_OK;
	}

	LRESULT OnChangeFirewallhost(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeFirewalllogonname(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeFirewallpassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeFirewallport(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnSelchangeFirewalltype(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
};

#endif //__TELNETXFIREWALLPROPPAGE_H_
