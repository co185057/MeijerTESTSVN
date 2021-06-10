//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    MobileCartBase.h
//
// TITLE:   Receives mobile cart from cloud and updates it as required
//
// AUTHOR:  Barbara Dawkins
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// MobileCartBase
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CMobileCartBase
#define _CMobileCartBase

#ifndef _CPPUNIT
#include <map>
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

#include "DllDefine.h"
#else
#include "FakeCommon.h"
#endif


#ifdef _SCOTSSF_
class DLLIMPORT
#else
class DLLEXPORT
#endif 
 CMobileCartBase  : public CObject
{
public:
    CMobileCartBase();
    ~CMobileCartBase();
    virtual long Initialize(CString csStoreNum, int nTerminalNum);
    virtual void UnInitialize();
    virtual int SendReadyForTransaction();
    virtual void NotReadyForTransaction();

    virtual int SendFinishTransaction(long lTotal, int nTransactionNumber);

    virtual _bstr_t ProcessCart(_bstr_t bstrCartData);
    virtual void SetSendFinishTransaction(bool bSendFinishTransaction);

protected:
    bool m_bInitialized;           // Initialization complete
    CCriticalSection m_CriticalRequest;
    CString m_csStoreNum;
    CString m_csTerminalNum;
    bool m_bSendFinishTransaction;


    DECLARE_DYNCREATE(CMobileCartBase)// MFC Runtime class/object information
};
#endif

