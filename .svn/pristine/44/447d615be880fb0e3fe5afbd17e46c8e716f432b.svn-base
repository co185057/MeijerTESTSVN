// CUJPOSEmulation.h: interface for the CBasePOSEmulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUJPOSEMULATION_H__INCLUDED_)
#define AFX_CUJPOSEMULATION_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ATBEmulationBase.h"
#include "DuplexClientPipe\DuplexClientPipe.h"
#include "TBMsgObj.h"

class CBasePOSEmulation : public ATBEmulationBase, public APipeSink
{
public:
   CBasePOSEmulation();
   virtual ~CBasePOSEmulation();
   
   virtual void Initialize();
   virtual void Uninitialize();
   virtual void PipeFailure( DWORD dwPipeID ){};
   virtual void PostHostMsg(CTBMsgObj * pMsg);
   virtual void ProcessRawHostData( DWORD dwPipeID, void* pData, DWORD dwByteLength);
   
protected:
   CDuplexClientPipe* m_OriginatePipe;
   CDuplexClientPipe* m_MonitorPipe;
   
private:
   
   DWORD OpenPipe(CDuplexClientPipe* pipe, 
                  LPCTSTR szPipeName, 
                  DWORD pipe_id);
   
};

#endif // !defined(AFX_HDJPOSEMULATION_H__INCLUDED_)
