// commClient.cpp : Implementation of CcommClient
#include "stdafx.h"
#include "commClient.h"


// CcommClient

STDMETHODIMP CcommClient::Advise(LONG lID)
{
	// TODO: Add your implementation code here
   HRESULT hr;
   hr = m_spIServ.CoCreateInstance(__uuidof(CcommRouter));
   if (SUCCEEDED(hr))
   {
      hr = __hook(&_IcommRouterEvents::Transfer, m_spIServ, CcommClient::OnTransmit);
   }
   if (hr == S_OK)
   {
      m_bConnected = true;
      m_data = L"Connected";
   }
   FireViewChange();
   return(hr);
}

STDMETHODIMP CcommClient::UnAdvise(void)
{
	// TODO: Add your implementation code here
   if (m_bConnected)
   {
      HRESULT hr = __unhook(&_IcommRouterEvents::Transfer, m_spIServ, CcommClient::OnTransmit);
      if (SUCCEEDED(hr))
      {
         m_spIServ.Release();
         m_bConnected = false;
      }
      return(hr);
   }
   return(S_OK);
}

HRESULT CcommClient::OnTransmit(VARIANT data)
{
   if (data.vt == VT_BSTR)
      m_data = data;
   //m_data.WriteToStream();
   FireViewChange();
   return(S_OK);
}

STDMETHODIMP CcommClient::Send(LONG lDestination, VARIANT data)
{
	// TODO: Add your implementation code here
   if (m_bConnected)
      m_spIServ->Send(1,data);
	return S_OK;
}
