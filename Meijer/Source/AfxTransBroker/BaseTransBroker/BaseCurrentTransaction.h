//////////////////////////////////////////////////////////////////////////
///
/// \file BaseCurrentTransaction.h
/// \brief interface for the CBaseCurrentTransaction class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASECurrentTransaction_H__INCLUDED_)
#define AFX_BASECurrentTransaction_H__INCLUDED_

#include "BaseTBDefs.h"
#include "transbroker.h"
#include "tbInProgress.h"

#pragma warning (disable: 4786)
#pragma warning (disable: 4788)
#include <map>
using namespace std;

class CBaseCurrentTransaction;
typedef void (CBaseCurrentTransaction::*StateHandlerType)(PSMessageElement psMessage, TBRC* retval, LPARAM lparam);
typedef struct 
{
    long      State;
    StateHandlerType  pEntryFunction;
    StateHandlerType  pExitFunction;
} StateHandlerStruct;

typedef map<int , StateHandlerStruct> CMapStateToEntryFunction;

//////////////////////////////////////////////////////////////////////
///
/// CBaseCurrentTransaction is an implementaion of CBaseCurrentTransaction
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseCurrentTransaction
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CBaseCurrentTransaction();
		
    //////////////////////////////////////////////////////////////////////
		///
		/// ResetTransactionDetails, reset all base transaction data 
		///
		//////////////////////////////////////////////////////////////////////
    virtual void ResetTransactionDetails(void);
    
		//////////////////////////////////////////////////////////////////////
		///
		/// GetQuantityLimit, accessors for member variable
		///
		///	@return long quantity limit
		//////////////////////////////////////////////////////////////////////
    virtual long GetQuantityLimit(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetQuantityLimit - accessors for member variable
		///
		///	@param long quantity limit
		//////////////////////////////////////////////////////////////////////
    virtual void SetQuantityLimit(long lVal);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetTransactionLimit, accessors for member variable
		///
		///	@return long transaction limit
		//////////////////////////////////////////////////////////////////////
    virtual long GetTransactionLimit(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetTransactionLimit, accessors for member variable
		///
		///	@param long transaction limit
		//////////////////////////////////////////////////////////////////////
    virtual void SetTransactionLimit(long lVal);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentTransactionNumber, accessors for member variable
		///
		///	@return long current transaction number
		//////////////////////////////////////////////////////////////////////
    virtual long GetCurrentTransactionNumber(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCurrentTransactionNumber, accessors for member variable
		///
		///	@param long current transaction number
		//////////////////////////////////////////////////////////////////////
    virtual void SetCurrentTransactionNumber(long lVal);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetAssistModeStatus, accessors for member variable
		///
		///	@return long assist mode status
		//////////////////////////////////////////////////////////////////////
    virtual long GetAssistModeStatus(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetAssistModeStatus, accessors for member variable
		///
		///	@param long assist mode status
		//////////////////////////////////////////////////////////////////////
    virtual void SetAssistModeStatus(long lVal);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetLoyaltyCardNumber, from TBAPI, Fastlane requesting loyalty num
		///
		///	@param LPTSTR *pszLoyaltyCardNumber - will contain loyalty number
		///	@return TBRC
		//////////////////////////////////////////////////////////////////////
    virtual TBRC GetLoyaltyCardNumber(LPTSTR *pszLoyaltyCardNumber);

		//////////////////////////////////////////////////////////////////////
		///
		/// LoyaltyCardNumInTrx, accessors for member data, gets and sets data
		///
		///	@return CString loyalty card number
		//////////////////////////////////////////////////////////////////////
    virtual CString LoyaltyCardNumInTrx(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// LoyaltyCardNumInTrx, accessors for member data, gets and sets data
		///
		///	@param CString loyalty card number
		//////////////////////////////////////////////////////////////////////
    virtual void LoyaltyCardNumInTrx(CString csNewVal);
    
		//////////////////////////////////////////////////////////////////////
		///
		/// GetPreviousInProgressState, return the previous TBINPROGRESS TB state
		///
		///	@return long previous TB State
		//////////////////////////////////////////////////////////////////////
    long GetPreviousInProgressState(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressState - finds the long or TBINPROGRESS value of a 
		///                             TB state from its string name
		///
		///	@param LPCTSTR szStateName - contains the string TB state name
		///	@retrun long - value of the TB state
		//////////////////////////////////////////////////////////////////////
    long GetCurrentInProgressState(LPCTSTR szStateName);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressState - return the TBINPROGRESS TB state
		///
		///	@param long - current in progress TB State
		//////////////////////////////////////////////////////////////////////
    long GetCurrentInProgressState(void);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCurrentInProgressState - provides the ability to set the current TB
		///	                            TB state by passing the TB string name.
		///
		///	@param LPCTSTR szStateName - contains the string TB state name
		//////////////////////////////////////////////////////////////////////
    void SetCurrentInProgressState(LPCTSTR szStateName);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetCurrentInProgressState - sets the current TB InProgress state. If we 
		///                             are actually changing states, then the current
		///                             states exit routine will be called, and the new
		///                             states entry routine will be called. This also
		///                             copies the old state to the previous InProgress
		///                             state.
		///
		///	@param long nState - TBINPROGRESS is a long, so this is just the new state
		//////////////////////////////////////////////////////////////////////
    void SetCurrentInProgressState(long newState);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetCurrentInProgressStateName - given the key (the InProgress state num)
		///	                                this will find the string name of the state
		///	@param long key - TB state number
		///	@return LPCTSTR - the string representation of the state name
		//////////////////////////////////////////////////////////////////////
    LPCTSTR GetCurrentInProgressStateName(long key=0);

		//////////////////////////////////////////////////////////////////////
		///
		/// SetLanguageUsed, accessors for the member variable
		///
		///	@param LPCTSTR - language code
		//////////////////////////////////////////////////////////////////////
    void SetLanguageUsed(LPCTSTR szLangCode);

		//////////////////////////////////////////////////////////////////////
		///
		/// GetLanguageUsed, accessors for the member variable
		///
		///	@return CString - language code
		//////////////////////////////////////////////////////////////////////
    CString GetLanguageUsed(void);
    
		//////////////////////////////////////////////////////////////////////
		///
		/// DirtyTrx - accessors for the member variable. A Dirty Transaction flag is
		///				kept to know when something must be done to complete an empty trx.
		///			  A non-dirty trx requires no additional processing when ending the
		///			  trx. You would not enter a tender state in clean trx's, for example.
		///			  At the very least, trx's become dirty when items are sold. They
		///			  can also become dirty when loyalty cards are scanned.
		///
		///	@param BOOL - flag for dirty trx
		//////////////////////////////////////////////////////////////////////
		virtual void DirtyTrx(BOOL bNewVal);

		//////////////////////////////////////////////////////////////////////
		///
		/// DirtyTrx - accessors for the member variable. A Dirty Transaction flag is
		///				kept to know when something must be done to complete an empty trx.
		///			  A non-dirty trx requires no additional processing when ending the
		///			  trx. You would not enter a tender state in clean trx's, for example.
		///			  At the very least, trx's become dirty when items are sold. They
		///			  can also become dirty when loyalty cards are scanned.
		///
		///	@return BOOL - flag for dirty trx
		//////////////////////////////////////////////////////////////////////
		virtual BOOL DirtyTrx(void) const;

protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentTransaction();  // should not be called outside of this class (instance function)
    
    long   m_CurrentInProgressState;	///< variable for current in progress state
    long   m_PreviousInProgressState;	///< variable for previous in progress state
    
    CMapStateToEntryFunction m_StateEntryHandlerMap;	///< object for StateHandler Map
    
private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide copy const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentTransaction(CBaseCurrentTransaction &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentTransaction & operator =(const CBaseCurrentTransaction &); // hide assignment
    
    long m_lTransactionNumber;	///< variable for transaction number
    long m_lTrxLimit;           ///< variable Transaction limit
    long m_lQuantityLimit;      ///< variable for Quantity limit per trx - not used currently by FL
    long m_lAssistModeStatus;		///< variable for assist mode status
    CTBInProgress *m_pTBInProgress;	///< object for TBInProgress
    CString m_csLoyaltyCardNumInTrx;	///< variable for loyalty card number
    CString m_csLanguageUsed;		///< variable for language code
		BOOL m_bDirtyTrx;						///< variable to flag dirty trx

};

#endif // !defined(AFX_BASECurrentTransaction_H__INCLUDED_)

