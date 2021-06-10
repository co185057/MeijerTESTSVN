#ifndef _IPIPECONNECTION_H
#define _IPIPECONNECTION_H


class IPipeConnection
{
public:
    IPipeConnection() {}
    virtual ~IPipeConnection() {}

    virtual HRESULT CreateInstance() = 0;
	virtual HRESULT Disconnect() = 0;
	virtual HRESULT SubscribeTo(_bstr_t strSubscriberId) = 0;
	virtual HRESULT ConnectEx(_bstr_t bsrDevices, long lReadPort, long lWritePort, _bstr_t messsageId) =0;
	virtual HRESULT SetCallback(_variant_t vtCallback, _variant_t vtUser)=0;
};


#endif // _IPIPECONNECTION_H