// RemoteConsoleMgrPpg.cpp : Implementation of the CRemoteConsoleMgrPropPage property page class.

#include "stdafx.h"
#include "RemoteConsoleMgr.h"
#include "RemoteConsoleMgrPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRemoteConsoleMgrPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CRemoteConsoleMgrPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRemoteConsoleMgrPropPage, "REMOTECONSOLEM.RemoteConsoleMPropPage.1",
	0x7d6f02b, 0x992e, 0x4d10, 0xb1, 0x34, 0x75, 0x67, 0xd8, 0x4e, 0xab, 0x53)



// CRemoteConsoleMgrPropPage::CRemoteConsoleMgrPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRemoteConsoleMgrPropPage

BOOL CRemoteConsoleMgrPropPage::CRemoteConsoleMgrPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_REMOTECONSOLEMGR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CRemoteConsoleMgrPropPage::CRemoteConsoleMgrPropPage - Constructor

CRemoteConsoleMgrPropPage::CRemoteConsoleMgrPropPage() :
	COlePropertyPage(IDD, IDS_REMOTECONSOLEMGR_PPG_CAPTION)
{
	SetHelpInfo(_T("Names to appear in the control"), _T("RemoteConsoleMgr.HLP"), 0);
}



// CRemoteConsoleMgrPropPage::DoDataExchange - Moves data between page and properties

void CRemoteConsoleMgrPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CRemoteConsoleMgrPropPage message handlers
