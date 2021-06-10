#include "StdAfx.h"

#ifndef _CPPUNIT
#include "DLLDefine.h"
#else
#include "TestMacros.h"
#endif

#include "QueuedActionsContainer.h"

#include "SMStateBase.h"
#include "IQueuedAction.h"


#define COMP_ID ID_SM
#define T_ID _T("QAContainer")

/**
  *  Queue for holding delayed actions,  such as delayed message screen created
  *  during Generic Delayed intervention processing.  Queue is checked on entry
  *  to ScanAndBag.
  */
CQueuedActionsContainer::CQueuedActionsContainer()
{

}

CQueuedActionsContainer::~CQueuedActionsContainer()
{
    CQueuedActionsContainer::Clear();
}

long CQueuedActionsContainer::GetSize(void) const
{
    return m_actionQueue.size();
}

void CQueuedActionsContainer::Add(
                            IQueuedAction *pQueuedAction)
{
    trace(L6, _T("CQueuedActionsContainer::Add"));
    if(pQueuedAction)
    {
        m_actionQueue.push_back(const_cast<IQueuedAction  *>(pQueuedAction));
    }
}

/*
 *  AddReplace:  Replace an existing action with matching ID if it is found,  otherwise
 *               add the action to the end of the queue.
 */
IQueuedAction *CQueuedActionsContainer::AddReplace(
                            IQueuedAction *pQueuedAction)
{
    trace(L6, _T("CQueuedActionsContainer::AddReplace"));
    IQueuedAction* oldCopy = NULL;
    if(pQueuedAction)
    {


 //       std::deque<IQueuedAction *>::iterator it = m_actionQueue.begin();

        std::deque<int>::size_type sz = m_actionQueue.size();
        for (unsigned i=0; i<sz && !oldCopy; i++)
        {
            IQueuedAction* pExistingAction = NULL;
            pExistingAction = m_actionQueue[i];
            CString csExistingActionID = pExistingAction->GetActionID();
            CString csNewActionID = pQueuedAction->GetActionID();
            trace(L6, _T("csExistingActionID = %s, csNewActionID = %s"), csExistingActionID, csNewActionID);
            if (csExistingActionID.Compare(csNewActionID) == 0)
            {
                trace(L6, _T("Found older action"));
                m_actionQueue[i] = pQueuedAction;
                oldCopy = pExistingAction;
            }
        }
        if (!oldCopy)
        {
            m_actionQueue.push_back(const_cast<IQueuedAction  *>(pQueuedAction));
        }
    }
    return oldCopy;
}

void CQueuedActionsContainer::PushFront(IQueuedAction* pQueuedAction)
{
    trace(L6, _T("CQueuedActionsContainer::PushFront"));
    if(pQueuedAction)
    {
        m_actionQueue.push_front(const_cast<IQueuedAction  *>(pQueuedAction));
    }
}

void CQueuedActionsContainer::PushBack(IQueuedAction* pQueuedAction)
{
    trace(L6, _T("CQueuedActionsContainer::PushBack"));
    if(pQueuedAction)
    {
        m_actionQueue.push_back(const_cast<IQueuedAction  *>(pQueuedAction));
    }
}

IQueuedAction * CQueuedActionsContainer::GetNext(void)
{
    IQueuedAction *pAction = NULL;

    if(m_actionQueue.size() > 0)
    {
        pAction = m_actionQueue.front();
        m_actionQueue.pop_front();
    }

    return pAction;
}


IQueuedAction * CQueuedActionsContainer::GetLast(void)
{
    IQueuedAction *pAction = NULL;

    if(m_actionQueue.size() > 0)
    {
        pAction = m_actionQueue.back();
        m_actionQueue.pop_back();
    }

    return pAction;
}

IQueuedAction * const CQueuedActionsContainer::Peek(void) const
{
    IQueuedAction *pAction = NULL;

    if(m_actionQueue.size() > 0)
    {
        pAction = m_actionQueue.front();
    }

    return pAction;
}

IQueuedAction * const CQueuedActionsContainer::Peek(int iIndex) const
{
    std::deque<int>::size_type sz = m_actionQueue.size();
    IQueuedAction* pAction = NULL;
    if (iIndex < sz)
    {
        pAction = m_actionQueue[iIndex];
    }  
    return pAction;
}

IQueuedAction * const CQueuedActionsContainer::Peek(CString csID) const
{
    IQueuedAction* pAction = NULL;

    std::deque<int>::size_type sz = m_actionQueue.size();
    for (unsigned i=0; i<sz ; i++)
    {
        IQueuedAction* pExistingAction = NULL;
        pExistingAction = m_actionQueue[i];
        CString csExistingActionID = pExistingAction->GetActionID();
        if (csExistingActionID.Compare(csID) == 0)
        {
            pAction = Peek(i);
            break;
        }
    }

    return pAction;
}

IQueuedAction * CQueuedActionsContainer::Get(int iIndex)
{
    std::deque<int>::size_type sz = m_actionQueue.size();
    IQueuedAction* pAction = NULL;
    if (iIndex >= 0 && iIndex < sz)
    {
        pAction = m_actionQueue[iIndex];
        m_actionQueue.erase(m_actionQueue.begin() + iIndex);
    }  
    return pAction;
}

IQueuedAction * CQueuedActionsContainer::Get(CString csID)
{
    IQueuedAction* pAction = NULL;

    std::deque<int>::size_type sz = m_actionQueue.size();
    for (unsigned i=0; i<sz ; i++)
    {
        IQueuedAction* pExistingAction = NULL;
        pExistingAction = m_actionQueue[i];
        CString csExistingActionID = pExistingAction->GetActionID();
        if (csExistingActionID.Compare(csID) == 0)
        {
            pAction = Get(i);
            break;
        }
    }

    return pAction;
}

void CQueuedActionsContainer::Clear(void) throw()
{
    try
    {
        IQueuedAction *pAction = GetNext();

        while(pAction)
        {
            delete pAction;
            pAction = GetNext();
        }

    } catch(...) {
    }
}


