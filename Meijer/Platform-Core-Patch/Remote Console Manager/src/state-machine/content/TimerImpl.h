#pragma once
#include "XMLContent.h"
#include "Timer.h"

namespace STATE_MACHINE_NAMESPACE
{

class CTimerImpl
    : public CXMLContent
    , public STATE_MACHINE_NAMESPACE::ITimer
    , public CPoolObject<CTimerImpl>
{
public:

    CTimerImpl(int);
    virtual ~CTimerImpl(void);

    virtual int GetId() const;

    virtual const _TCHAR *GetComment();

    virtual const _TCHAR *GetName();

    virtual const _TCHAR *GetTimeoutName();

    virtual int GetTimeoutDuration();

    virtual void SetTimeoutDuration(int);

    virtual int GetTimerHandle();

    virtual void SetTimerHandle(int);

    virtual bool IsPeriodic();

    virtual void SetPeriodic(bool);

//  virtual TIMER_CALLBACK GetCallback();

//  virtual void SetCallback(TIMER_CALLBACK);

    DECLARE_STRING_PROPERTY(Name);
    DECLARE_STRING_PROPERTY(Comment);

public: 

    StringType GetXMLOutput(CXMLBuilder *pXMLBuilder=NULL);

private:

    CTimerImpl(CTimerImpl &);                     // Hide the Copy construcor
    CTimerImpl &operator =(const CTimerImpl &);   // Hide the assignment operator

    int nId;

//  TIMER_CALLBACK m_pCallback;

    int m_nTimeoutDuration;

    int m_hTimer;

    bool m_bPeriodic;
};

};