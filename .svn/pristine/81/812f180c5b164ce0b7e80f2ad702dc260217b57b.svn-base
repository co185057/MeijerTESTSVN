// TelnetClientIF.cpp : Implementation of CTelnetClientIF
#include "stdafx.h"
//lint +libh(TelnetClientSupport.h)
#include "TelnetClientSupport.h"
#include "TelnetClientIF.h"
#include "TelnetClientDefs.h"
#define REPORTINGKEY        _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting")                            
#define TELNET_TIMEOUT          _T("TelnetTimeout")
#define DEFAULT_TELNET_TIMEOUT   90
/////////////////////////////////////////////////////////////////////////////
// CTelnetClientIF

CTelnetClientIF::CTelnetClientIF() :
   m_pTelnetConnection(NULL),
   m_pfnCallback(NULL),
   m_bDisconnecting(false) // TAR 343530
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
}

CTelnetClientIF::~CTelnetClientIF()
{
}

STDMETHODIMP CTelnetClientIF::SetCallback(VARIANT pfnCallbackAddr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	_variant_t vtTemp;

	vtTemp.Attach(pfnCallbackAddr);
	m_pfnCallback = (FPEVENTHANDLER)vtTemp.byref; //lint !e611
	return S_OK;
}


STDMETHODIMP CTelnetClientIF::ReceiveString(BSTR *bstrInputString, long lCharCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VARIANT vString;
	_variant_t vtType;
	_variant_t vtLength;
	_bstr_t bString;

    HRESULT hr = S_OK;
    try
    {
        if (NULL != m_pTelnetConnection)
        {
	        vtType.intVal = 8; // vbString
	        vtLength.intVal = lCharCount;
	        hr = m_pTelnetConnection->Receive(vtType, vtLength, &vString);
            if (!FAILED(hr))
            {
                bString = vString;
                *bstrInputString = bString.copy();
            }
        }
        else
        {
            hr = E_FAIL;
        }
    }
    catch (_com_error )
    {
        hr = E_FAIL;                   
    }
    catch (...)
    {

        hr = E_FAIL;                   
    }
	return hr;
}

STDMETHODIMP CTelnetClientIF::TransmitString(BSTR bstrSendString)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    HRESULT hr = S_OK;
    short b;
    try
    {
        if (NULL != m_pTelnetConnection)
        {
	        hr = m_pTelnetConnection->Send(_variant_t((_bstr_t)bstrSendString), &b);
        }
        else
        {
            hr = E_FAIL;
        }
    }
    catch (_com_error )
    {
        hr = E_FAIL;                   
    }
    catch (...)
    {

        hr = E_FAIL;                   
    }
	return hr;
}

STDMETHODIMP CTelnetClientIF::Connect(BSTR bstrHostName, long lPortNumber)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	_variant_t vtHostname;
	_variant_t vtPort;
    ITelnetXComNotify *pNotify;
    LONG     lRC;
	HKEY     hKey;
    long  lTelnetTimeout= (long) DEFAULT_TELNET_TIMEOUT;
    m_bDisconnecting = false;      // TAR 343530

    HRESULT hr;

    try
    {
        hr = S_OK;  // Set default return code

		DWORD dwServerSize=sizeof(long);
        lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, REPORTINGKEY,
				 0, KEY_QUERY_VALUE, &hKey);
        if (ERROR_SUCCESS == lRC)
        {

            lRC = ::RegQueryValueEx(hKey, TELNET_TIMEOUT, NULL, NULL,
			      (BYTE *)&lTelnetTimeout, &dwServerSize);
            if (lRC != ERROR_SUCCESS)
            {
                lTelnetTimeout = (long) DEFAULT_TELNET_TIMEOUT;
            }
		    ::RegCloseKey(hKey);
        }

        if (NULL == m_pTelnetConnection)
        {
            hr = m_pTelnetConnection.CreateInstance(_T("Mabry.TelnetXCom"));

            if (!FAILED(hr))
            {
                // Set the notification pointer
                // This must succeed since it is using the this pointer and this object
                // implements the interface.
                VERIFY(SUCCEEDED(this->QueryInterface(IID_ITelnetXComNotify, (void **) &pNotify)));
                VERIFY(SUCCEEDED(m_pTelnetConnection->put_NotificationObject(pNotify)));
            }
        }
        if (!FAILED(hr))
        {
            // Created successfully or already there
	        vtPort.ChangeType(VT_INT);
	        vtPort.intVal = lPortNumber;
	        vtHostname = _bstr_t(bstrHostName);
            m_pTelnetConnection->put_Timeout((short)lTelnetTimeout);
	        hr = m_pTelnetConnection->Connect(vtHostname, vtPort);
            
        }

    }
    catch (_com_error )
    {
        hr = E_FAIL;                   
    }
    catch (...)
    {

        hr = E_FAIL;                   
    }

	return hr;
}

STDMETHODIMP CTelnetClientIF::Disconnect()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    m_bDisconnecting = true; // TAR 343530
    HRESULT hr = S_OK;
    try
    {
        if (NULL != m_pTelnetConnection)
        {
            hr = m_pTelnetConnection->Disconnect();
            m_pTelnetConnection = NULL; // TAR 343530
        }
        else
        {
            hr = E_FAIL;
        }
    }
    catch (_com_error )
    {
        hr = E_FAIL;                   
    }
    catch (...)
    {

        hr = E_FAIL;                   
    }
	return hr;
}

STDMETHODIMP CTelnetClientIF::Disconnected(ITelnetXCom * )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())


    if (NULL != m_pfnCallback)
    {
	    (*m_pfnCallback)(RPT_TELNETDISCONNECT, 0, 0);
    }

	return S_OK;
}
STDMETHODIMP CTelnetClientIF::Received(ITelnetXCom * , LONG BytesCount)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

    if (   (NULL != m_pfnCallback)
        && (!m_bDisconnecting) )  // TAR 343530
    {
	    (*m_pfnCallback)(RPT_TELNETRECEIVED, 0, BytesCount );
    }

	return S_OK;
}
