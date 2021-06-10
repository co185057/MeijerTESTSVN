//////////////////////////////////////////////////////////////////////////
///
/// \file BaseMHMicr.h
/// \brief interface for the CBaseMHMicr class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEMHMICR_H__INCLUDED_)
#define AFX_BASEMHMICR_H__INCLUDED_

#include "MsgHandler.h"

//////////////////////////////////////////////////////////////////////
///
/// CBaseMHMicr is an implementaion of CBaseMHMicr
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseMHMicr : public CMsgHandler  
{
public:
	//////////////////////////////////////////////////////////////////////
	///
	/// Destructor
	///
	//////////////////////////////////////////////////////////////////////
	virtual ~CBaseMHMicr();

	//////////////////////////////////////////////////////////////////////
	///
	/// EnterMICR, This function is called by the FastLane App to report 
	///       information retrieved from the MICR document. If MICR data could 
	///       not be retrieved, the parameter szRawData and other string parameters 
	///       can be an empty string and the value of the long parameters is zero.
	///
	/// @param LPCTSTR szRawData - The raw MICR data from the MICR reader device
	/// @param LPCTSTR szTransitNumber - The bank’s transit (routing) number
	///	@param LPCTSTR szAccountNumber - The customer’s account number
	///	@param LPCTSTR szBankNumber - The bank’s ABA bank number
	///	@param LPCTSTR szSerialNumber - The sequence (check) number of the check
	///	@param LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
	/// @param long lType - The type of check:
	///		<ls>       1  personal check
	///		<ls>       2  Business or Commercial check
	///		<ls>       99 Unknown check type
	///	@param long lCountryCode - The country code of the check:
	///		<ls>       1  United States check
	///		<ls>       2  Canadian check (Canada)
	///		<ls>       3  Mexican check (Mexico)
	///		<ls>      99  Unknown check origination
	///	@return TBRC - acknowledge the request
	//////////////////////////////////////////////////////////////////////
    virtual TBRC EnterMICR( LPCTSTR szRawData, LPCTSTR szTransitNumber, LPCTSTR szAccountNumber, 
                            LPCTSTR szBankNumber, LPCTSTR szSerialNumber, LPCTSTR szEPC, 
                            long lType, long lCountryCode);

	//////////////////////////////////////////////////////////////////////
	///
	/// GetMICRPrompt, This function is called by the FastLane App when it 
	///       receives a TB_NEEDMICR notification from the TB. It retrieves 
	///       information requested by TB about the MICR document.
	///
	///	@param LPTSTR *ppszMICRPrompt - A pointer to a string pointer that is allocated
	///       by TB. A prompt to be displayed to the operator to insert the MICR document.
	///	@return TBRC - acknowledge the request
	//////////////////////////////////////////////////////////////////////
    virtual TBRC GetMICRPrompt( LPTSTR *ppszMICRPrompt);
    
	//////////////////////////////////////////////////////////////////////
	///
	/// IsMICR, accessor functions for member variable
	///
	///	@return bool m_bMICR flag
	//////////////////////////////////////////////////////////////////////
	virtual bool IsMICR( void);
    
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
	/// SetMICR - accessor functions for member variable
	///
	///	@param bool MICR
	//////////////////////////////////////////////////////////////////////
	virtual void SetMICR( bool b);

	//////////////////////////////////////////////////////////////////////
	///
	/// EnterMICRWithType, This function is called by the FastLane App to report 
	///       information retrieved from the MICR document. If MICR data could 
	///       not be retrieved, the parameter szRawData and other string parameters 
	///       can be an empty string and the value of the long parameters is zero.
	///
    ///	@param TBTENDERTYPE nTenderType - Tender Type
	/// @param LPCTSTR szRawData - The raw MICR data from the MICR reader device
	/// @param LPCTSTR szTransitNumber - The bank’s transit (routing) number
	///	@param LPCTSTR szAccountNumber - The customer’s account number
	///	@param LPCTSTR szBankNumber - The bank’s ABA bank number
	///	@param LPCTSTR szSerialNumber - The sequence (check) number of the check
	///	@param LPCTSTR szEPC - The Extended Processing Code (EPC) on the check
	/// @param long lType - The type of check:
	///		<ls>       1  personal check
	///		<ls>       2  Business or Commercial check
	///		<ls>       99 Unknown check type
	///	@param long lCountryCode - The country code of the check:
	///		<ls>       1  United States check
	///		<ls>       2  Canadian check (Canada)
	///		<ls>       3  Mexican check (Mexico)
	///		<ls>      99  Unknown check origination
	///	@return TBRC - acknowledge the request
	//////////////////////////////////////////////////////////////////////
    virtual TBRC EnterMICRWithType(TBTENDERTYPE nTenderType, LPCTSTR szRawData, LPCTSTR szTransitNumber, 
                            LPCTSTR szAccountNumber, LPCTSTR szBankNumber, LPCTSTR szSerialNumber, 
                            LPCTSTR szEPC, long lType, long lCountryCode);

protected:
	//////////////////////////////////////////////////////////////////////
	///
	/// Constructor
	///
	//////////////////////////////////////////////////////////////////////
	CBaseMHMicr();

private:
	//////////////////////////////////////////////////////////////////////
	///
	/// Hide Copy const
	///
	//////////////////////////////////////////////////////////////////////
    CBaseMHMicr(const CBaseMHMicr &);                    // hide copy const

	//////////////////////////////////////////////////////////////////////
	///
	/// Hide Assignment
	///
	//////////////////////////////////////////////////////////////////////
    CBaseMHMicr & operator =(const CBaseMHMicr &); // hide assignment

    bool m_bMICR;	///< Variable for MICR flag
};

#endif
