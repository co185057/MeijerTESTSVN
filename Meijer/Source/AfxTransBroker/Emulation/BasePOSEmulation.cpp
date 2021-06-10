// CUJPOSEmulation.cpp: implementation of the CBasePOSEmulation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tbdefs.h"
#include "BasePOSEmulation.h"
#include "customerTrxBroker.h"
#include "TraceSystem.h"
#include "msgs.h"

#define COMP_ID ID_TB               // Base state component
#define T_ID    _T("CBasePOSEmulation") 


// where should these be defined? - ajkajk
#define TB_MONITOR_PIPE   _T("\\\\.\\PIPE\\MONITOR")
#define TB_ORIGINATE_PIPE _T("\\\\.\\PIPE\\ORIGINATE")

#define ORIG_PIPE_ID 1
#define MONITOR_PIPE_ID 2



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasePOSEmulation::CBasePOSEmulation()
{
   tbtraceIO( _T("CBasePOSEmulation"));
   // create connections to Pipe Server
   m_OriginatePipe = new CDuplexClientPipe;
   m_MonitorPipe = new CDuplexClientPipe;
}

CBasePOSEmulation::~CBasePOSEmulation()
{
   tbtraceIO(_T("~CBasePOSEmulation()"));
   m_OriginatePipe->Uninitialize();
   delete [] m_OriginatePipe;
   m_OriginatePipe = 0;
   m_MonitorPipe->Uninitialize();
   delete [] m_MonitorPipe;
   m_MonitorPipe = 0;
}

void CBasePOSEmulation::Initialize()
{
   tbtraceIO( _T("Initialize()"));
   OpenPipe(m_OriginatePipe, TB_ORIGINATE_PIPE, ORIG_PIPE_ID);
   OpenPipe(m_MonitorPipe, TB_MONITOR_PIPE, MONITOR_PIPE_ID );   
}

void CBasePOSEmulation::Uninitialize()
{
    tbtrace( TM_INFO, _T("+-Uninitialize()"));
}


/////////////////////////////////////////////////////////////
//
// Function - OpenPipe()
//
// Purpose - Open a pipe, monitor or originate 
//
/////////////////////////////////////////////////////////////
DWORD CBasePOSEmulation::OpenPipe(CDuplexClientPipe* clientpipe, 
                               LPCTSTR szPipeName, 
                               DWORD pipe_id)
{
    tbtraceIO(_T("OpenPipe"));
    DWORD status;
    
    clientpipe->SetPipeSink( (APipeSink*) this );    
    while (1)
    {
        status = clientpipe->Initialize( szPipeName, pipe_id, true );
        if (status == 0) break;
        if (status == ERROR_PIPE_BUSY)
        {
            tbtrace( TM_ML_INFO, _T("Pipe is busy %s, will retry"), szPipeName);
            Sleep( 5000);
            continue;
        }
        else return status;
    }
    return 0;
}


/////////////////////////////////////////////////////////////
//
// Function - ProcessRawHostData()
//
// Purpose - Process messages received from POS
//
/////////////////////////////////////////////////////////////
void CBasePOSEmulation::ProcessRawHostData( DWORD dwPipeID, void* pData, DWORD dwByteLength)
{
   tbtrace( TM_INFO, _T("+ProcessRawHostData"));
   
   if (dwPipeID != MONITOR_PIPE_ID)
   {
      tbtrace( TM_ERROR, _T("Invalid dwPipeID %d"), dwPipeID);
      return;
   }
   
   tRouteHeader* pHdr= (tRouteHeader*)pData;

   CTBMsgObj *pMsg;
   if ((pHdr->nVersion == VERSION_GENOBJ_BIN) || (pHdr->nVersion == VERSION_GENOBJ_XML))
   {
      pMsg = new CTBGenMsg();
   }
   else
      pMsg = CCustomerTrxBroker::instance()->CreateMessageObjectByMessageId((BYTE) pHdr->messageID);
   if(pMsg)
      pMsg->Parse((BYTE*)pData);
   
   if (pMsg)
      m_pTBConnection->ReceiveMsg(pMsg);
}



/////////////////////////////////////////////////////////////
//
// Function - PostHostMsg()
//
// Purpose - Post a message to POS
//
/////////////////////////////////////////////////////////////
void CBasePOSEmulation::PostHostMsg(CTBMsgObj * pMsg)
{
	int nLen;
	BYTE* pBuf= pMsg->Serialize(nLen);
	m_OriginatePipe->WriteData(pBuf, nLen);
	delete []pBuf;
}

