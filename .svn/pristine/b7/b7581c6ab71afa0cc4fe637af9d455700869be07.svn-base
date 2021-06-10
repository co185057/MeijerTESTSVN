// TBDevice.h: interface for the CTBDevice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBDEVICE_H__2B5E45C3_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
#define AFX_TBDEVICE_H__2B5E45C3_37E8_11D5_9C83_0090276FD28D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ARawDataSink.h"

class CTBService;

class CTBDevice: public COposDevice//, public ARawDataSink  
{
public:
	CTBDevice(
		LPCTSTR        sClass,
		int            idsName,    // from the resource: "device name\nDescription"
	    int            nShareKey,  // Sharing key, for claim sem name
	    CmDataCapture& cmDc);

	virtual ~CTBDevice();

	// Implement ARawDataSink
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );
	virtual void CommunicationFailure( );

	// Overrides for COposDevice
	virtual long InteractiveHealthCheck( COposCheckHealth* pCheckHealth = NULL );
	virtual long InternalHealthCheck( void ); 
    virtual long ExternalHealthCheck( void );
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
    virtual long DirectIO( long nCommand, long* plData, CString& sData );

	static CTBDevice* CreateDevice( int nPort, COposRegistry& oRegistry,CmDataCapture& cmDc);
	static int GetDevicePort( COposRegistry& oRegistry, CmDataCapture& cmDc );

	// Event queueing methods
    void OnData( LPCTSTR sData, long lStatus = 0, long len=0 );
    void OnDirectIO( long lEventNumber, long lData = 0, LPCTSTR sData = NULL);
    void OnStatus( long lStatus );
    void OnOutput( long nOutputId );
    void OnInputError( long nRC, long nRCEx = 0 );
    int Send(const void *pBuf, int iLength);
	virtual void ReportPropertyNumber( long lProperty, long lValue );
protected:
	virtual bool CheckPropertyNumber( long lProperty );
	virtual long OpenDevice( void );
    virtual void CloseDevice( void );
	const int                m_nPort;
	CTBService*				 m_pTBService;
	int						 m_nTBDeviceId;
	CString				     m_sName;
	BOOL					 m_bAutoDataEventEnabled;
	CMapWordToPtr			 m_PropertyMap;
private:
	void StartSocketClient();
};  //lint !e1712 supress "Default contructor is NEVER used!"



#endif // !defined(AFX_TBDEVICE_H__2B5E45C3_37E8_11D5_9C83_0090276FD28D__INCLUDED_)
