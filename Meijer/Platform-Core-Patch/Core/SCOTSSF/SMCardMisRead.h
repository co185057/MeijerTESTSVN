//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMCardMisRead.h
//
// TITLE: Class header for mis-read card State
//
//
// AUTHOR:    Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CARDMISREADSTATE
#define _CARDMISREADSTATE

#include "SMCardMisReadBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
 SMCardMisRead : public SMCardMisReadBase
{
public:
	SMCardMisRead(LPTSTR, PSTEXT Title = LTT_CARDMISREAD);
	SMCardMisRead();
	SMStateBase *Deliver(LPTSTR, PSTEXT Title = LTT_CARDMISREAD);
	SMCardMisRead(PSTEXT, PSTEXT Title = LTT_CARDMISREAD);
	SMStateBase *Deliver(PSTEXT, PSTEXT Title = LTT_CARDMISREAD);

  DECLARE_DYNCREATE(SMCardMisRead)// MFC Runtime class/object information
};

#endif
