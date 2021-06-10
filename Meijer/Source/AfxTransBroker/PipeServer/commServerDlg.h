// commServerDlg.h : Declaration of the CcommServerDlg

#pragma once

#include "resource.h"       // main symbols
#include <atlhost.h>
#include "socketServer.h"


// CcommServerDlg

class CcommServerDlg : 
	public CAxDialogImpl<CcommServerDlg>
{
public:
	CcommServerDlg();

	~CcommServerDlg()
	{
	}

	enum { IDD = IDD_COMMSERVERDLG };

BEGIN_MSG_MAP(CcommServerDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WMU_CONNINSERT, OnListInsert)
	MESSAGE_HANDLER(WMU_CONNUPDATE, OnListUpdate)
	MESSAGE_HANDLER(WMU_CONNREMOVE, OnListRemove)
	MESSAGE_HANDLER(WMU_CONNCLOSEANDREMOVE, OnSocketResetAndRemove)
	MESSAGE_HANDLER(WMU_SETCOLOR,   OnListSetColor)
	//COMMAND_HANDLER(IDOK, BN_CLICKED, OnClickedOK)
	COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnClickedOK)
	CHAIN_MSG_MAP(CAxDialogImpl<CcommServerDlg>)
END_MSG_MAP()

// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		//EndDialog(wID);
		return 0;
	}
	void SetupListeningThreads();
private:
	void LoadRegistrySettings();
	void QueryKey(HKEY hKey);
	void SetupUI();
	void ShutDown();

public:
	LRESULT OnListInsert(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnListUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnListRemove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSocketResetAndRemove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnListSetColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void ListInsertItem(CConnectBase *pConn);
	void ListUpdateItem(CConnectBase *pConn);
	void ListRemoveItem(CConnectBase *pConn);
    void ListSetColor(COLORREF color);

    bool WaitForSubscriber(BYTE msID);
    static bool bWaitingForSubscriber;

	LRESULT OnEnChangeEdit1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnLvnItemchangedConnList(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

	static bool m_bSendAllToTB;
	static int m_nHeaderVersion;
	static int m_nTraceOnReceive;
	static long WINAPI CcommServerDlg::mainWriterThreadFn(void *);
	static CString m_csTraceFileName;
    static CString m_csWaitForSubscriber;
	HWND m_hList;
};


