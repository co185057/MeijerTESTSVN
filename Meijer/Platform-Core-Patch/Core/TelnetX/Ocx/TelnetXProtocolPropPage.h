// TelnetXProtocolPropPage.h : Declaration of the CTelnetXProtocolPropPage

#ifndef __TELNETXPROTOCOLPROPPAGE_H_
#define __TELNETXPROTOCOLPROPPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_TelnetXProtocolPropPage;

/////////////////////////////////////////////////////////////////////////////
// CTelnetXProtocolPropPage
class ATL_NO_VTABLE CTelnetXProtocolPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTelnetXProtocolPropPage, &CLSID_TelnetXProtocolPropPage>,
	public IPropertyPageImpl<CTelnetXProtocolPropPage>,
	public CDialogImpl<CTelnetXProtocolPropPage>
{
public:

	int bLoading;


	CTelnetXProtocolPropPage() 
	{
		m_dwTitleID = IDS_TITLETelnetXProtocolPropPage;
		m_dwHelpFileID = IDS_HELPFILETelnetXProtocolPropPage;
		m_dwDocStringID = IDS_DOCSTRINGTelnetXProtocolPropPage;
	}

	enum {IDD = IDD_TELNETXPROTOCOLPROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_TELNETXPROTOCOLPROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTelnetXProtocolPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTelnetXProtocolPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTelnetXProtocolPropPage>)
	COMMAND_HANDLER(IDC_AUTOLOGON, BN_CLICKED, OnClickedAutologon)
	COMMAND_HANDLER(IDC_COMMAND, EN_CHANGE, OnChangeCommand)
	COMMAND_HANDLER(IDC_LOGIN, EN_CHANGE, OnChangeLogin)
	COMMAND_HANDLER(IDC_PASSWORD, EN_CHANGE, OnChangePassword)
	COMMAND_HANDLER(IDC_PROTOCOL, CBN_SELCHANGE, OnSelchangeProtocol)
	COMMAND_HANDLER(IDC_ENCRYPTION1, BN_CLICKED, OnClickedEncryption1)
	COMMAND_HANDLER(IDC_ENCRYPTION2, BN_CLICKED, OnClickedEncryption2)
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
		HRESULT hr = IPropertyPageImpl<CTelnetXProtocolPropPage>::Activate(hWndParent, pRect, bModal );
		_TCHAR strbuff[MAX_PATH+1];
		BSTR bstr;

		if( hr != S_OK || m_nObjects == 0)
			return hr;

		bLoading=TRUE;

		CComQIPtr<ITelnetX, &IID_ITelnetX> pTelnetX(m_ppUnk[0]);
		
		// let's read Protocol
		ProtocolsEnum prot;
		if FAILED(pTelnetX->get_Protocol(&prot))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		SendDlgItemMessage(IDC_PROTOCOL,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(IDC_PROTOCOL,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Telnet"));
		SendDlgItemMessage(IDC_PROTOCOL,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("RLogin"));
		SendDlgItemMessage(IDC_PROTOCOL,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("RSH"));
		SendDlgItemMessage(IDC_PROTOCOL,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("RExec"));
		SendDlgItemMessage(IDC_PROTOCOL,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("SSH 1"));
		switch(prot)
		{
			case Telnet:
				SendDlgItemMessage(IDC_PROTOCOL,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Telnet"));
				break;
			case RLogin:
				SendDlgItemMessage(IDC_PROTOCOL,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("RLogin"));
				break;
			case RSH:
				SendDlgItemMessage(IDC_PROTOCOL,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("RSH"));
				break;
			case RExec:
				SendDlgItemMessage(IDC_PROTOCOL,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("RExec"));
				break;
			default:
				SendDlgItemMessage(IDC_PROTOCOL,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("SSH 1"));
				break;

		}

		// let's read autologon
		VARIANT_BOOL bval;
		if FAILED(pTelnetX->get_AutoLogon(&bval))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		if (bval)
		{
			CheckDlgButton(IDC_AUTOLOGON, -1 );
			SetDlgItemText(IDC_AUTOLOGON,_T("On"));
		}
		else
		{
			CheckDlgButton(IDC_AUTOLOGON, 0 );
			SetDlgItemText(IDC_AUTOLOGON,_T("Off"));
		}

		// let's read logonname
		if FAILED(pTelnetX->get_LogonName(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_LOGIN,strbuff);
		SysFreeString(bstr);
		
		// let's read logonpassword
		if FAILED(pTelnetX->get_LogonPassword(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_PASSWORD,strbuff);
		SysFreeString(bstr);
		
		// let's read logoncommand
		if FAILED(pTelnetX->get_LogonCommand(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_COMMAND,strbuff);
		SysFreeString(bstr);
		
		// let's read encryption
		EncryptionsEnum enc;
		if FAILED(pTelnetX->get_Encryption(&enc))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}

		if (enc=TripleDES)
			CheckDlgButton(IDC_ENCRYPTION1, -1 );
		else
			CheckDlgButton(IDC_ENCRYPTION2, -1 );
		
		bLoading=FALSE;
		return S_OK;
	}
		
	STDMETHOD(Apply)(void)
	{
		USES_CONVERSION;
		ATLTRACE(_T("CFtpServXCtlPropPage::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			_TCHAR strbuff[MAX_PATH+1];
			short sVal;

			CComQIPtr<ITelnetX, &IID_ITelnetX> pTelnetX(m_ppUnk[i]);
			CComBSTR str;


			// let's store ProtocolType
			ProtocolsEnum prot;
			prot=(ProtocolsEnum) SendDlgItemMessage(IDC_PROTOCOL,CB_GETCURSEL,0,0);
			if FAILED(pTelnetX->put_Protocol(prot))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store AutoLogon
			sVal=(IsDlgButtonChecked(IDC_AUTOLOGON)?TRUE:FALSE);
			if FAILED(pTelnetX->put_AutoLogon(sVal))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store LogonName
			GetDlgItemText(IDC_LOGIN, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_LogonName(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Password
			GetDlgItemText(IDC_PASSWORD, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_LogonPassword(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Command
			GetDlgItemText(IDC_COMMAND, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_LogonCommand(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Encryption
			EncryptionsEnum enc;
			enc=(EncryptionsEnum)IsDlgButtonChecked(IDC_ENCRYPTION1?TripleDES:SingleDES);
			if FAILED(pTelnetX->put_Encryption(enc))
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
	LRESULT OnClickedAutologon(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (IsDlgButtonChecked(IDC_AUTOLOGON))
			SetDlgItemText(IDC_AUTOLOGON,_T("On"));
		else
			SetDlgItemText(IDC_AUTOLOGON,_T("Off"));
		
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeCommand(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeLogin(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangePassword(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnSelchangeProtocol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnClickedEncryption1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnClickedEncryption2(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
};

#endif //__TELNETXPROTOCOLPROPPAGE_H_
