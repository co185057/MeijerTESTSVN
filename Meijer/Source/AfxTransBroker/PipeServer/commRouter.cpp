// commRouter.cpp : Implementation of CcommRouter

#include "stdafx.h"
#include "commRouter.h"

CcommServerDlg *CcommRouter::m_pDlg=NULL;


// CcommRouter


STDMETHODIMP CcommRouter::Send(LONG lDestination, VARIANT data)
{
	// TODO: Add your implementation code here
	_IcommRouterEvents_Transfer(data);
	return S_OK;
}
