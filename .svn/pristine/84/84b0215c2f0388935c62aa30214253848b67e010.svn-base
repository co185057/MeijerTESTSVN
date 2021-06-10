// commRouter.h : Declaration of the CcommRouter

#pragma once
#include "resource.h"       // main symbols
#include "commServerDlg.h"

// IcommRouter
[
	object,
	uuid("D5A3FBE0-CF00-402D-A1BA-909553229B90"),
	dual,	helpstring("IcommRouter Interface"),
	pointer_default(unique)
]
__interface IcommRouter : IDispatch
{
	[id(1), helpstring("method Send")] HRESULT Send([in] LONG lDestination, [in] VARIANT data);
};


// _IcommRouterEvents
[
	dispinterface,
	uuid("280883D0-7A04-44D4-A69F-DFC91EBB0AC5"),
	helpstring("_IcommRouterEvents Interface")
]
__interface _IcommRouterEvents
{
	[id(1), helpstring("method Transfer")] HRESULT Transfer([in] VARIANT data);
};


// CcommRouter

[
	coclass,
	threading(free),
	event_source(com),
	vi_progid("commServer.commRouter"),
	progid("commServer.commRouter.1"),
	version(1.0),
	uuid("E9CA2411-4ACA-4165-9037-876C3101D42F"),
	helpstring("commRouter Class")
]
class ATL_NO_VTABLE CcommRouter : 
	public IcommRouter
{
public:
	CcommRouter()
	{
	}

	__event __interface _IcommRouterEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_CLASSFACTORY_SINGLETON(CcommRouter);

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	static CcommServerDlg* m_pDlg;

	STDMETHOD(Send)(LONG lDestination, VARIANT data);
};

