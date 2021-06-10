//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHKeylock.h
/// \brief interface for the CBaseMHKeylock class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASEMHKEYLOCK_H__INCLUDED_)
#define AFX_BASEMHKEYLOCK_H__INCLUDED_

#include "MsgHandler.h"

#define R_N_POS   0
#define S_POS     1
#define L_POS     2
#define X_Ex_POS  3

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHKeylock is an implementaion of CBaseMHKeylock
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHKeylock : public CMsgHandler
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CBaseMHKeylock();

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentSecurityLevel, Called by the FastLane App so that the App can 
		///       determine the TB’s current security level.
		///
		///	@param long *plExpectedLevel - security level from TBKEYLOCK enum
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
   	virtual TBRC GetCurrentSecurityLevel(long *plCurrentLevel);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetExpectedSecurityLevel, Called by the FastLane application so that 
		///       it can learn what security level the TB currently wants to see.
		///
		///	@param long *plExpectedLevel - security level from TBKEYLOCK enum
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
		virtual TBRC GetExpectedSecurityLevel(long *plExpectedLevel);

    // This is required functionality for all MH classes
		//////////////////////////////////////////////////////////////////////
		///
		/// ProcessMessage, Process the message posted by TB. TB analyzes the message 
		///	       and returns the state of the host application, based on the presentation 
		///        data in the message. 
		///
		///	@param CTBMsgObj *pMsg - Pointer to the TB message
		///	@return TBSTATE - the current state of the host application
		//////////////////////////////////////////////////////////////////////
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsgObj);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendLockOff, Do a manager key-lock off. Not currently used in core app
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC SendLockOff(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendLockOn, Do a manager key-lock on. Not currently used in core app
		///
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC SendLockOn(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendLockPosition, Allows TB to send a specific keylock position to the POS.
		///       Base TB cannot implement this as it is POS-specific
		///
		///	@param int nPosition - keylock position to send to POS
		//////////////////////////////////////////////////////////////////////
    virtual void SendLockPosition(int nPosition);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetSecurityLevel, Called by the FastLane App when the security level 
		///       of the operator changes.
		///
		///	@param long lSecurityLevel - new security level from TBKEYLOCK enum
		/// @return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC SetSecurityLevel( long lSecurityLevel);
    
protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		//////////////////////////////////////////////////////////////////////
   	CBaseMHKeylock();
    
private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide Copy Const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHKeylock(const CBaseMHKeylock &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHKeylock & operator =(const CBaseMHKeylock &); // hide assignment
};

#endif
