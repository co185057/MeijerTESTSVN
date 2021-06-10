// ToneIndicator.h: interface for the CToneIndicator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TONEINDICATOR_H__BBDABC6E_B1B2_43EC_95A1_2117A55A720E__INCLUDED_)
#define AFX_TONEINDICATOR_H__BBDABC6E_B1B2_43EC_95A1_2117A55A720E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TBDevice.h"
#include "ToneIndicatorMsg.h"

class CToneIndicator : public CTBDevice  
{
public:
	CToneIndicator(int nPort, int nId,  CmDataCapture& cmDc);
	virtual ~CToneIndicator();

   	virtual CString GetDeviceModel( void ) const { return CString("ToneIndicator");}
	virtual CString GetDeviceVersion( void ) const{ return CString("1.3");} 
	virtual long Open( CTBService* pService, COposRegistry& oRegistry );
	virtual void Close( CTBService* pService );
	virtual void ProcessRawData( void* pData, DWORD dwByteLen );

    long Sound(long NumberOfCycles, long InterSoundWait);
    long SoundImmediate();
	void ClearOutput();
    long NotifyTB( CToneIndicatorMsg* msg );

private:
};

#endif // !defined(AFX_TONEINDICATOR_H__BBDABC6E_B1B2_43EC_95A1_2117A55A720E__INCLUDED_)
