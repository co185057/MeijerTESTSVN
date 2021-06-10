// TelnetXPropPage.h : Declaration of the CTelnetXPropPage

#ifndef __TELNETXPROPPAGE_H_
#define __TELNETXPROPPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_TelnetXPropPage;

/////////////////////////////////////////////////////////////////////////////
// CTelnetXPropPage
class ATL_NO_VTABLE CTelnetXPropPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTelnetXPropPage, &CLSID_TelnetXPropPage>,
	public IPropertyPageImpl<CTelnetXPropPage>,
	public CDialogImpl<CTelnetXPropPage>
{
public:

	int bLoading;


	CTelnetXPropPage() 
	{
		m_dwTitleID = IDS_TITLETelnetXPropPage;
		m_dwHelpFileID = IDS_HELPFILETelnetXPropPage;
		m_dwDocStringID = IDS_DOCSTRINGTelnetXPropPage;
	}

	enum {IDD = IDD_TELNETXPROPPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_TELNETXPROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTelnetXPropPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTelnetXPropPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTelnetXPropPage>)
	COMMAND_HANDLER(IDC_BLOCKINGC, BN_CLICKED, OnClickedBlockingc)
	COMMAND_HANDLER(IDC_BLOCKINGMODE, CBN_SELCHANGE, OnSelchangeBlockingmode)
	COMMAND_HANDLER(IDC_DEBUGMODE, BN_CLICKED, OnClickedDebugmode)
	COMMAND_HANDLER(IDC_HOST, EN_CHANGE, OnChangeHost)
	COMMAND_HANDLER(IDC_PORT, EN_CHANGE, OnChangePort)
	COMMAND_HANDLER(IDC_PROMPT, EN_CHANGE, OnChangePrompt)
	COMMAND_HANDLER(IDC_TIMEOUT, EN_CHANGE, OnChangeTimeout)
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
		HRESULT hr = IPropertyPageImpl<CTelnetXPropPage>::Activate(hWndParent, pRect, bModal );
		_TCHAR strbuff[MAX_PATH+1];
		BSTR bstr;
		short sVal;

		if( hr != S_OK || m_nObjects == 0)
			return hr;

		bLoading=TRUE;

		CComQIPtr<ITelnetX, &IID_ITelnetX> pTelnetX(m_ppUnk[0]);
		
		// let's read Host
		if FAILED(pTelnetX->get_Host(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_HOST,strbuff);
		SysFreeString(bstr);
		
		// let's read Port
		if FAILED(pTelnetX->get_Port(&sVal))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		wsprintf(strbuff,_T("%u"),sVal);
		SetDlgItemText(IDC_PORT,strbuff);

		// let's read Prompt
		if FAILED(pTelnetX->get_PromptString(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_PROMPT,strbuff);
		SysFreeString(bstr);
		
		
		// let's read TimeOut
		if FAILED(pTelnetX->get_Timeout(&sVal))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		wsprintf(strbuff,_T("%u"),sVal);
		SetDlgItemText(IDC_TIMEOUT,strbuff);

		// let's read DebugMode
		VARIANT_BOOL bval;
		if FAILED(pTelnetX->get_DebugMode(&bval))
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
			CheckDlgButton(IDC_DEBUGMODE, -1 );
			SetDlgItemText(IDC_DEBUGMODE,_T("On"));
		}
		else
		{
			CheckDlgButton(IDC_DEBUGMODE, 0 );
			SetDlgItemText(IDC_DEBUGMODE,_T("Off"));
		}

			// let's read Blocking
		TelnetXBlockingModesEnum bloc;
		if FAILED(pTelnetX->get_BlockingMode(&bloc))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		SendDlgItemMessage(IDC_BLOCKINGMODE,CB_RESETCONTENT,0,0);
		SendDlgItemMessage(IDC_BLOCKINGMODE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("True blocking"));
		SendDlgItemMessage(IDC_BLOCKINGMODE,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)_T("Pseudo blocking"));
		if (bloc==TrueBlocking)
			SendDlgItemMessage(IDC_BLOCKINGMODE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("True blocking"));
		else
			SendDlgItemMessage(IDC_BLOCKINGMODE,CB_SELECTSTRING,0,(LPARAM)(LPCTSTR)_T("Pseudo blocking"));
		
		// let's read Blocking
		if FAILED(pTelnetX->get_Blocking(&bval))
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
			CheckDlgButton(IDC_BLOCKINGC, -1 );
			SetDlgItemText(IDC_BLOCKINGC,_T("On"));
		}
		else
		{
			CheckDlgButton(IDC_BLOCKINGC, 0 );
			SetDlgItemText(IDC_BLOCKINGC,_T("Off"));
		}

		// let's set Version string
		if FAILED(pTelnetX->get_Version(&bstr))
		{
			CComPtr<IErrorInfo> pError;
			CComBSTR            strError;
			GetErrorInfo(0, &pError);
			pError->GetDescription(&strError);
			MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
			return E_FAIL;
		}
		lstrcpy(strbuff,OLE2T(bstr));
		SetDlgItemText(IDC_VERSIONSTRING,strbuff);
		SysFreeString(bstr);


		
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

			// let's store Host
			GetDlgItemText(IDC_HOST, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_Host(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Port
			GetDlgItemText(IDC_PORT, strbuff, sizeof(strbuff));
			sVal=(SHORT)atoi(strbuff);
			str = strbuff;
			if FAILED(pTelnetX->put_Port(sVal))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Prompt
			GetDlgItemText(IDC_PROMPT, strbuff, sizeof(strbuff));
			str = strbuff;
			if FAILED(pTelnetX->put_PromptString(str))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Timeout
			GetDlgItemText(IDC_TIMEOUT, strbuff, sizeof(strbuff));
			sVal=(SHORT)atoi(strbuff);
			str = strbuff;
			if FAILED(pTelnetX->put_Timeout(sVal))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store DebugMode
			sVal=(IsDlgButtonChecked(IDC_DEBUGMODE)?TRUE:FALSE);
			if FAILED(pTelnetX->put_DebugMode(sVal))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store BlockingMode
			TelnetXBlockingModesEnum bloc;
			if (!SendDlgItemMessage(IDC_BLOCKINGMODE,CB_GETCURSEL,0,0))
				bloc=TrueBlocking;
			else
				bloc=PseudoBlocking;
			if FAILED(pTelnetX->put_BlockingMode(bloc))
			{
				CComPtr<IErrorInfo> pError;
				CComBSTR            strError;
				GetErrorInfo(0, &pError);
				pError->GetDescription(&strError);
				MessageBox(OLE2T(strError), _T("Error"), MB_ICONEXCLAMATION);
				return E_FAIL;
			}

			// let's store Blocking
			sVal=(IsDlgButtonChecked(IDC_BLOCKINGC)?TRUE:FALSE);
			if FAILED(pTelnetX->put_Blocking(sVal))
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
	LRESULT OnClickedBlockingc(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (IsDlgButtonChecked(IDC_BLOCKINGC))
			SetDlgItemText(IDC_BLOCKINGC,_T("On"));
		else
			SetDlgItemText(IDC_BLOCKINGC,_T("Off"));
		
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnSelchangeBlockingmode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnClickedDebugmode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if (IsDlgButtonChecked(IDC_DEBUGMODE))
			SetDlgItemText(IDC_DEBUGMODE,_T("On"));
		else
			SetDlgItemText(IDC_DEBUGMODE,_T("Off"));
		
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeHost(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangePort(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangePrompt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
	LRESULT OnChangeTimeout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if ( !bLoading ) SetDirty(TRUE);
		return 0;
	}
};

#endif //__TELNETXPROPPAGE_H_
