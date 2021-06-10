//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateHTBase.cpp
//
// TITLE: Hard Totals Functions
//
//
// AUTHOR:  Scott Barnhart
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first

#include "Common.h"                 // MGV common includes

#include "SMState.h"                // MGV abstract base state

#include "HTProcedures.h"           // HT procedures
extern HTProcedures ht;             // global HT object
// AKR extern DLLEXPORT CTransBroker tb;   // global TB object

#define COMP_ID ID_HT               // base state component
#define T_ID    _T("HTBase")

//////////////////////////////////////////////////////////////////////////////////////////////////
// additional Initialization
long SMStateBase::HTInitialize(void)
{
  long rc = 0;
  trace(L6,_T("+Init"));
	
  try
  {
      rc = ht.Initialize();
  }
  catch (...)
  {
      rc = -1;
  }
	
  trace(L6,_T("-Init %d"),rc);
  return rc;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
long SMStateBase::HTUnInitialize(void)// before SCOT
{
  long rc = 0;
  trace(L6,_T("+UnInit"));
  rc = ht.UnInitialize();
  trace(L6,_T("-UnInit %d"),rc);
  return rc;
}

SMStateBase *SMStateBase::HTParse(MessageElement *me, TBSTATE nTBCMOSState)
{
  SMStateBase *sUser = STATE_NULL;
  trace(L6,_T("+HTParse"));
  CString fileName;
  PFILE_IO_INFO pInfo = (PFILE_IO_INFO) me->lParam;
  ASSERT(pInfo != NULL);

  switch (nTBCMOSState)
  {
    case TB_CMOS_DOSOPEN:
      memcpy(fileName.GetBuffer(me->nLength+1), me->pData, me->nLength);
      fileName.ReleaseBuffer(me->nLength+1);
      fileName.SetAt(me->nLength, 0); // Null Terminate;

      pInfo->retCode = pInfo->handle = ht.DosOpen(fileName, pInfo->OpenInfo.flags);
      break;

    case TB_CMOS_DOSCLOSE:
      pInfo->retCode = ht.DosClose(pInfo->handle);
      break;

    case TB_CMOS_DOSREAD:
      pInfo->retCode = ht.DosRead(pInfo->handle, me->pData, me->nLength);
      break;

    case TB_CMOS_DOSWRITE:
      pInfo->retCode = ht.DosWrite(pInfo->handle, me->pData, me->nLength);
      break;

    case TB_CMOS_DOSSEEK:
      pInfo->retCode = ht.DosSeek(pInfo->handle, 
                                  pInfo->SeekInfo.offset, 
                                  pInfo->SeekInfo.sMode);
      break;

    case TB_CMOS_PURGE:
      pInfo->retCode = ht.PurgeAll();
      break;

    default:
      pInfo->retCode = -1; // Unknown error
  }

  pInfo->bRequest = false;  // This is now a response block, not a request block
  TBProcessMessage(me);    // Have TB send the response to the host app.

  trace(L6,_T("-HTParse"));
  return sUser;
}