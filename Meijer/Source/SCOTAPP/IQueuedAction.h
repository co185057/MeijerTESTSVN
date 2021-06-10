#ifndef _IQUEUED_ACTION_H
#define _IQUEUED_ACTION_H

#ifndef _CPPUNIT
#include "DllDefine.h"
#else
#include "TestMacros.h"
#endif // _CPPUNIT

class SMStateBase;
#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif
 IQueuedAction
{
public:
    IQueuedAction();
    virtual ~IQueuedAction();

    /**
     * Interface definition for actions to be taken by scotapp when the application
     * has returned to a state capable of executing the action (ScanAndBag, etc).
     * Return a new State to initiate a state transition or STATE_NULL to
     * allow processing of next action on the queue.
     */
    virtual SMStateBase* HandleQueuedAction(SMStateBase* pCurrentState) = 0;
    virtual CString GetVideoPath();
    virtual CString GetActionID();
    virtual CString GetActionString();
    virtual int GetReportingID();
    virtual void SetReportingID(int iValue);


protected:
    CString m_csActionString;   ///String data used by individual action classes
    CString m_csID;             ///Identifier for action
    int m_nReportingID;
};

#endif //_IQUEUED_ACTION_H
