#ifndef _CPIPECONNECTION_H
#define _CPIPECONNECTION_H

#include "IPipeConnection.h"

/*
#ifdef _DEBUG
    #import <GTOUD.dll> rename_namespace("GTO")
#else
    #import <GTOU.dll> rename_namespace("GTO")
#endif
*/
// SSCOADK-2499 Sample SSF will not compile stand-alone
//              Use HeaderGenerator project to build tlh/tli headers if necessary.
#include <GTOU.tlh>


class CPipeConnection : public IPipeConnection
{
public:
    CPipeConnection();
    virtual ~CPipeConnection();

    virtual HRESULT CreateInstance();
	virtual HRESULT Disconnect();
	virtual HRESULT SubscribeTo(_bstr_t strSubscriberId);
	virtual HRESULT ConnectEx(_bstr_t bsrDevices, long lReadPort, long lWritePort, _bstr_t messsageId);
	virtual HRESULT SetCallback(_variant_t vtCallback, _variant_t vtUser);

private:
	GTO::IGTConnectPtr m_PipeConnection;
};


#endif //_CPIPECONNECTION_H
