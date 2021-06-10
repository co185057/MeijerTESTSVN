//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHCashDrawer.h
/// \brief interface for the CBaseMHCashDrawer class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASEMHCashDrawer_H__INCLUDED_)
#define AFX_BASEMHCashDrawer_H__INCLUDED_

#include "MsgHandler.h"
#include "CashDrawerMsg.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHCashDrawer is an implementaion of CBaseMHCashDrawer
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHCashDrawer : public CMsgHandler
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CBaseMHCashDrawer();

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
    virtual TBSTATE ProcessMessage(CTBMsgObj *pMsg);

		//////////////////////////////////////////////////////////////////////
		///
		/// ClosePOSCashDrawer, EMPTY!!  implementation is TB and Hook specific
		///
		///	@param  CashDrawerStations  cdStation - Cash Drawer station to be closed
		///	@return TRUE if input messages sent, FALSE otherwise
		//////////////////////////////////////////////////////////////////////
    virtual void ClosePOSCashDrawer(CCashDrawerMsg::CashDrawerStations cdStation );

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCloseImmediate, This is called by TB to specify the handling of 
		///                     of CashDrawer messages from the POS
		///
		///	@param bool bNewCloseImmediate - true or false.
		//////////////////////////////////////////////////////////////////////
    virtual void SetCloseImmediate( bool bNewCloseImmediate );

protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHCashDrawer();

private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide Copy Const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHCashDrawer(const CBaseMHCashDrawer &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide Assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHCashDrawer & operator =(const CBaseMHCashDrawer &); // hide assignment

    bool    m_bCloseImmediate_A; ///< variable to flag close immediate drawer A
    bool    m_bCloseImmediate_B; ///< variable to flag close immediate drawer B                               
};

#endif 
