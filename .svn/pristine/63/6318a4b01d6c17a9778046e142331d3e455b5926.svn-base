// ATBEmulationBase.h: interface for the ATBEmulationBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATBEMULATIONBASE_H__50A934BF_06A4_4611_8F8A_F7AED86BD2ED__INCLUDED_)
#define AFX_ATBEMULATIONBASE_H__50A934BF_06A4_4611_8F8A_F7AED86BD2ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AMessageSink.h"

class ATBEmulationBase  
{
public:
  ATBEmulationBase(){};
  virtual ~ATBEmulationBase(){};

	virtual void Initialize() = 0;
	virtual void Uninitialize() = 0;

	virtual void PostHostMsg(CTBMsgObj * pMsg) = 0;

    void SetTBConnection(AMessageSink* pMsgSink){m_pTBConnection = pMsgSink;};

protected:
	AMessageSink* m_pTBConnection;
};

#endif // !defined(AFX_ATBEMULATIONBASE_H__50A934BF_06A4_4611_8F8A_F7AED86BD2ED__INCLUDED_)
