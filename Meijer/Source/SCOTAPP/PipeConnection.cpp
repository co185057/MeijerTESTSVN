#include "stdafx.h"
#include "PipeConnection.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPipeConnection::CPipeConnection()
{
    m_PipeConnection = NULL;
}

CPipeConnection::~CPipeConnection()
{
    if (m_PipeConnection)
    {
        m_PipeConnection.Release();
        m_PipeConnection = NULL;
    }
}

HRESULT CPipeConnection::CreateInstance()
{
    return m_PipeConnection.CreateInstance(__uuidof(GTO::CGTConnect));
}

HRESULT CPipeConnection::Disconnect()
{
	return m_PipeConnection->Disconnect();
}

HRESULT CPipeConnection::SubscribeTo(_bstr_t strSubscriberId)
{
	return m_PipeConnection->SubscribeTo(strSubscriberId);
}

HRESULT CPipeConnection::ConnectEx(_bstr_t bstrDevices, long lReadPort, long lWritePort, _bstr_t messsageId)
{
	return m_PipeConnection->ConnectEx(bstrDevices, lReadPort, lWritePort, messsageId);
}

HRESULT CPipeConnection::SetCallback(_variant_t vtCallback, _variant_t vtUser)
{
	return m_PipeConnection->SetCallback(vtCallback, vtUser);
}

