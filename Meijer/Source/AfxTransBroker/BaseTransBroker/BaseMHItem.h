//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHItem.h
/// \brief interface for the CBaseMHItem class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASEMITEM_H__INCLUDED_)
#define AFX_BASEMITEM_H__INCLUDED_

#include "MsgHandler.h"
#include "RewardLineDetailsMsg.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHItem is an implementaion of CBaseMHItem
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHItem  : public CMsgHandler
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
		virtual ~CBaseMHItem();

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
		/// EnterDOB, Inform host application the Customer’s Date of Birth if needed
		///       Base level traces parameter values. It is up to Customer level to 
		///       implement functionality
		///
		///	@param int nYear - valid year of customer DOB
		///	@param int nMonth - valid month of customer DOB
		///	@param int nDay - valid day of customer DOB
		///	@param int nCustomerAge - calculated customer age
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC EnterDOB(int nYear, int nMonth, int nDay, int nCustomerAge);

		//////////////////////////////////////////////////////////////////////
		///
		/// EnterItem, Called by FL to sell or void an item. Base level traces parameter
		///       values. It is up to Customer level to implement functionality
		///
		///	@param LPCTSTR szItemCode - item code as keyed in or extracted from barcode if 
		///					                    items is scanned
		///	@param BOOL fVoid - TRUE:void item, FALSE:sell item
		///	@param BOOL fOverride - TRUE: use override if needed; FALSE: no override
		///	@param long nQuantity - Quantity of the items to be entered into the transaction. 
		///                   For non-quantity-required items, use the value 0.
		///	@param long nWeight - Weight associated with each item
		///	@param long nTareCode - Tare code associated with each item. TB calculates the 
		///                   weight associated with this tare code automatically.
		///	@param long lPrice - Price of each item.
		///	@param BOOL fQuantityConfirmed - Used if nQuantity is above confirmation limit.
		///	@param PSEnterItemDetails psMoreDetails - structure containing lUnitPrice, scan type
		///                                     and multi-label barcode data
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
		virtual TBRC EnterItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetItemDetails, FL requests TB to populate a structure with pertinent
		///       item details. This will be called with FL receives TB_ITEMSOLD, or 
		///       any other item sale related state event (TB_NEEDQUANTITY, TB_NEEDWEIGHT, etc)
		///       This function will not reset the item details anymore - the exact 
		///       timing of that is customer specific!
		///
		///	@param PSItemDetails psItemDetails - populate this struct with the item details
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC GetItemDetails(PSItemDetails psDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetItemExtendedDetails, Get extended detailed information on the last 
		///       item in the transaction. After confirming that the TB supports 
		///       this function, with TBIsAllowed, the application should call 
		///       this API when it receives TB_ITEMSOLD or any other item sale 
		///       related state event from TB.
		///       This function will not reset the item details anymore - the exact 
		///       timing of that is customer specific!
		///
		///	@param PSItemExtendedDetails psItemExtendedDetails - populate this struct with the details
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC GetItemExtendedDetails(PSItemExtendedDetails psDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetRewardLineDetails, When TB returns TB_REWARDLINE, FL calls this
		///       function to collect the details of that reward line.
		///
		///	@param PSRewardLineDetails psRewardLineDetails - the reward line details struct
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC GetRewardLineDetails(PSRewardLineDetails psRewardLineDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// ResetRewardLineDetails, resets all members of the reward line details struct.
		///
		//////////////////////////////////////////////////////////////////////
    virtual void ResetRewardLineDetails(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetRewardLineDetails, copies a RewardLineDetailsMsg into this classes
		///       member variable to be passed to FL when requested
		///	@param CRewardLineDetailsMsg * pRewardDetails - the struct to copy
		//////////////////////////////////////////////////////////////////////
    virtual void SetRewardLineDetails( CRewardLineDetailsMsg *pRewardDetails);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemEntry, ### ADD API DOC ###
		///
		///	@param LPARAM item map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemEntry(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemVoid, ### ADD API DOC ###
		///
		///	@param LPARAM variable from message map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemVoid(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemQty, ### ADD API DOC ###
		///
		///	@param LPARAM variable from message map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemQty(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemWeight, ### ADD API DOC ###
		///
		///	@param LPARA variable from message map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemWeight(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemScan, ### ADD API DOC ###
		///
		///	@param LPARAM variable from message map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemScan(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// HandleItemKeyed, ### ADD API DOC ###
		///
		///	@param LPARAM variable from message map
		///	@return TBSTATE
		//////////////////////////////////////////////////////////////////////
		virtual TBSTATE HandleItemKeyed(LPARAM lparam);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendQuantity, Inform host application of the quantity entered by the 
		///			customer. The value comes in through the EnterItem API. Base logic
		///		  will be to key in the quantity as key strokes, and press <Enter>.
		///
		///	@param long lQuantity - the item quantity keyed in
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC SendQuantity(long lQuantity);

		//////////////////////////////////////////////////////////////////////
		///
		/// SendWeight, Inform host application of the weight currently on scale.
		///       This is only called while in assist mode when FL detects weight on 
		///       the scale and the last return was TB_NEEDWEIGHT. This will need to
		///       be implemented at the customer specific level.
		///
		///	@param long lWeight - the weight on the scale
		///	@return TBRC - acknowledge the request
		//////////////////////////////////////////////////////////////////////
    virtual TBRC SendWeight(long lWeight);

protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
		CBaseMHItem();

		//////////////////////////////////////////////////////////////////////
		///
		/// SaveToCurrentItem, ### ADD API DOC ###
		///
		///	@param LPCTSTR - szItemCode
		///	@param BOOL - void
		///	@param BOOL - override
		///	@param long - quantity
		///	@param long - weight
		///	@param long - tare code
		///	@param long - price
		///	@param BOOL - quantity confirmed
		///	@param PSEnterItemDetails - item details object
		//////////////////////////////////////////////////////////////////////
		void SaveToCurrentItem(LPCTSTR szItemCode, const BOOL fVoid, const BOOL fOverride, 
                           const long nQuantity, const long nWeight, const long nTareCode,
                           const long lPrice, const BOOL fQuantityConfirmed, 
                           PSEnterItemDetails psMoreDetails);

private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide copy const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHItem(const CBaseMHItem &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseMHItem & operator =(const CBaseMHItem &); // hide assignment

    SRewardLineDetails m_RewardLineDetails; ///< object to store Reward line
};

#endif 
