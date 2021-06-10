#pragma once

// RemoteConsoleMgrPropPage.h : Declaration of the CRemoteConsoleMgrPropPage property page class.


// CRemoteConsoleMgrPropPage : See RemoteConsoleMgrPropPage.cpp.cpp for implementation.

class CRemoteConsoleMgrPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRemoteConsoleMgrPropPage)
	DECLARE_OLECREATE_EX(CRemoteConsoleMgrPropPage)

// Constructor
public:
	CRemoteConsoleMgrPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_REMOTECONSOLEMGR };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

