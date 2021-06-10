//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSecurityLogs.h
//
// TITLE: Class header for store mode Security Logs state
//
//
// AUTHOR:  Peter Denhoed
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMSECURITYLOGSSTATE
#define _SMSECURITYLOGSSTATE

#include "SMSmSecurityLogsBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmSecurityLogs : public SMSmSecurityLogsBase
{
public:
    SMSmSecurityLogs();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize(void);

protected:
    virtual SMStateBase  *PSButton1(void);
    virtual SMStateBase  *PSButton8(void);
    //virtual SMStateBase *RAParse(MessageElement* ); SSCOADK-5431

    DECLARE_DYNCREATE(SMSmSecurityLogs)// MFC Runtime class/object information
};

#endif