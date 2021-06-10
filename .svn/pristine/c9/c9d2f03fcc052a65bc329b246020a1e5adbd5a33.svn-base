#ifndef _QUEUED_ACTION_CONTAINER_H
#define _QUEUED_ACTION_CONTAINER_H

#include <deque>
using namespace std;

class IQueuedAction;
class SMStateBase;
/**
 * \brief Class to store queued action requests.
 *
 * \note  This class will store action requests in the order they come in,
 *        The request data can be retrieved and processed later when the
 *        system is not busy with a different item.
 */  
#ifdef _SCOTSSF_
class DLLIMPORT
#elif defined _SCOT
class DLLEXPORT
#else
class 
#endif    
 CQueuedActionsContainer
{
public:
    CQueuedActionsContainer();
    virtual ~CQueuedActionsContainer();

    virtual long GetSize(void) const;

    /**
     * Queue an action.
     * \param[in] Queued action to process later.
     */
    //lint -sem(CQueuedActionsContainer::Add, custodial(1))
    virtual void Add(IQueuedAction* pQueuedAction);

    /**
     * Queue an action, replace existing action with same ID.
     * \param[in] Queued action to process later.
     * \param[out] returns false if new intervention was created, true if replaced
     */
    //lint -sem(CQueuedActionsContainer::Add, custodial(1))
    virtual IQueuedAction *AddReplace(IQueuedAction* pQueuedAction);


    /**
     * Put action on front of queue
     * \param[in] Queued action to process later.
     * \param[out] returns false if new intervention was created, true if replaced
     */
    //lint -sem(CQueuedActionsContainer::Add, custodial(1))
    virtual void PushFront(IQueuedAction* pQueuedAction);


    /**
     * Put action on back of queue
     * \param[in] Queued action to process later.
     * \param[out] returns false if new intervention was created, true if replaced
     */
    //lint -sem(CQueuedActionsContainer::Add, custodial(1))
    virtual void PushBack(IQueuedAction* pQueuedAction);


    /**
     * \return the next Queued action or NULL if there are no stored
     *         Queued actions.
     *
     * \note The oldest queued action is always returned.
     */
    virtual IQueuedAction * GetNext(void);

    /**
     * \return the last Queued action or NULL if there are no stored
     *         Queued actions.
     *
     * \note The newest queued action is always returned.
     */
    virtual IQueuedAction * GetLast(void);


    /**
     * \return Pointer to a IQueuedAction that is the oldest
     *         Queued action or NULL if the queue is empty.
     *
     * \note The action is not removed from the queue.  
     */
    virtual IQueuedAction * const Peek(void) const;

     /**
     * \return Pointer to a IQueuedAction that is at the iIndex position on the
     *         queue.  Used to iterate through the queue to get information on
     *         existing actions.  Returns a null pointer if the index is out of range.
     *
     * \note The action is not removed from the queue.  
     */
    virtual IQueuedAction * const Peek(int iIndex) const;

     /**
     * \return Pointer to a IQueuedAction that had an ID = csID on the
     *         queue..  Used to iterate through the queue to get information on
     *         existing actions.  Returns a null pointer if the index is out of range.
     *
     * \note The action is not removed from the queue.  
     */
    virtual IQueuedAction * const Peek(CString csID) const;


     /**
     * \return Pointer to a IQueuedAction that had an ID = csID on the
     *         queue.  Removes action from the queue.
     *         Returns a null pointer if queue has no ID = csID
     *
     * \note The action is removed from the queue.  
     */
    virtual IQueuedAction * Get(CString csID);

     /**
     * \return Pointer to a IQueuedAction that is at the iIndex position on the
     *         queue.  Removes action from the queue.
     *         Returns a null pointer if the index is out of range.
     *
     * \note The action is removed from the queue.  
     */
    virtual IQueuedAction * Get(int iIndex);

    /**
     * Empty the container.
     */
    virtual void Clear(void) throw();

protected:
    
    // Prevent the container from being copied.
    //lint -esym(1526, CQueuedActionsContainer::CQueuedActionsContainer)
    CQueuedActionsContainer(const CQueuedActionsContainer &rhs);

    //lint -esym(1526, CQueuedActionsContainer::operator=)
    CQueuedActionsContainer & operator=(const CQueuedActionsContainer &rhs);

    deque<IQueuedAction *> m_actionQueue;

};

#endif // _QUEUED_ACTION_CONTAINER_H
