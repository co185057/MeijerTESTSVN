// RemoteConsoleMgrPropPage.cpp : Implementation of the CRemoteConsoleMgrPropPage property page class.

#include "stdafx.h"
#include "RemoteConsoleMgr.h"
#include "RemoteConsoleMgrPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CRemoteConsoleMgrPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CRemoteConsoleMgrPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRemoteConsoleMgrPropPage, "REMOTECONSOLEM.RemoteConsoleMPropPage.1",
	0x8d054c09, 0x2417, 0x4014, 0xaa, 0x1c, 0x1a, 0x8c, 0xc5, 0xe0, 0x5e, 0x1d)



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
}



// CRemoteConsoleMgrPropPage::DoDataExchange - Moves data between page and properties

void CRemoteConsoleMgrPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CRemoteConsoleMgrPropPage message handlers
