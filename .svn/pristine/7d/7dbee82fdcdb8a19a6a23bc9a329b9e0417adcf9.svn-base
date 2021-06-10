//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmCashManagementEx.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SMCASHMANAGEMENTEX
#define _SMCASHMANAGEMENTEX

#include "SMSmCashManagementExBase.h"

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
  SMSmCashManagementEx : public SMSmCashManagementExBase
{
public:
    SMSmCashManagementEx();
    virtual SMStateBase  *Initialize(void);
    virtual void UnInitialize();
    virtual SMStateBase *DMAcceptor(void); 
    virtual SMStateBase *PSParse( const MessageElement* );
    virtual SMStateBase  *PSButton8(void);
    virtual SMStateBase *PSEnterKey(void);
    virtual SMStateBase  *PSHotKey(CString &);
    virtual SMStateBase  *PSShiftKey(void);
virtual SMStateBase * createLevel1Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth, 
        bool displayHTML=false);
    virtual SMStateBase * createLevel2Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
    virtual SMStateBase * createLevel3Message( LPCTSTR szMsg,
        PSTEXT pstMsg,
        PSTEXT prompt,
        long   value,
        long   device,
        bool   checkHealth,
        bool displayHTML=false);
protected:
    virtual SMStateBase *DMParse( const MessageElement* );
    virtual long DMCashAcceptorAccept(void);
    virtual SMStateBase *HandleScannerData( const MessageElement* );

    DECLARE_DYNCREATE(SMSmCashManagementEx)// MFC Runtime class/object information
};

#endif