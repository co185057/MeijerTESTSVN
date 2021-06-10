// CustomerMsgMap.h: interface for the CCustomerMsgMap class. 
//
// CHANGE HISTORY:
//
// POS74992 Work Request:58657 Name:Robert Susanto Date:August 29, 2013
// POS68773/58095 Work Request:52635 Name:Robert Susanto Date:June 28, 2013
// POS36319 Work Request:50864 Name:Robert Susanto Date:June 20, 2012
// POS11024 Work Request:16632 Name:Sandy Reedy Date:July 20, 2011
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_CustomerMsgMap_H__INCLUDED_)
#define AFX_CustomerMsgMap_H__INCLUDED_

#include "BaseMHDisplay.h"
#include "SingleInstance.h"
#include "SolutionMsgMap.h"

// The definition of tMsgMapDisplay is included before the definition
// of CCustomerMsgMap in order to insure that it is created with
// the same size as the tMsgMapGeneric object defined in tbMessageMap.h
// Defining the message map after CCustomerMsgMap results in a 
// different sized pointer, which causes problems in MsgHandler 
// processing.
class CCustomerMsgMap;
class CCustomerTrxBroker;


/***********************************************************************
/*  Change History                                            
/*
/*  POS:11273 Work Request:11273 Name:Robert Susanto Date:February 22 2011  
/*
/*
/************************************************************************/


//class CCustomerMsgMap : public CMsgMapHandler, public CSingleInstance<CCustomerMsgMap>
class CCustomerMsgMap : public CSolutionMsgMap, public CSingleInstance<CCustomerMsgMap>
{
   friend class CSingleInstance<CCustomerMsgMap>;

public:
   virtual ~CCustomerMsgMap();
   virtual TBSTATE dispatchHelper(const CString &csMessage);
   void resetToLastPOSState(void);

   virtual TBSTATE ProcessMessage();
   virtual bool msgMessageScreen(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgPriceDiscrepancy( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgPriceEmbed( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgCMTenderingAmount(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgFSChange( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgFormatPriceInquiry(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgEnterTenderedAmount(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgOnRebateItem(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgRestartFastlane( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgScannedItemInSMFinish( TBSTATE &tbretState, LPARAM lparam ); 			
   virtual bool HasDrWatsonErrorOccurred(void);
   virtual bool GetProcessList(CArray<CString, CString> &csProcessNames);
   virtual bool msgAssistModeTrxEnd( TBSTATE &tbretState, LPARAM lparam );

   virtual void LookupHandlers();
   virtual bool msgCashDrawerSetCloseImmediate( TBSTATE &tbretState, LPARAM lparam ); //TAR359317			
   virtual bool msgAssistModeExit( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgOnGiftReceipt( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgSendSequenceIfRemote( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgVoidRewardLine( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgSendSequenceWithWaits( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgAddRewardLine( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgDecideIfTenderAccepted(TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgResetToLastPOSState(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgReturnCodeHandler(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgHandleWicBalanceExceeded(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgDataNeeded(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgDetectAndHandleNewRestrictedItem(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgCheckIfCouponToBeCollectedAndDisplay(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgImmediateOrDelayedIntervention(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgFormatPromoNumber (TBSTATE &tbretState, LPARAM lparam);							// SOTF6400 - Digital Couponing
   virtual bool msgDynamicCustomerDigitalCouponMessageScreen (TBSTATE &tbretState, LPARAM lparam);	// SOTF6400 - Digital Couponing
   virtual bool msgGiftPhonePriceEntrySetup(TBSTATE &tbretState, LPARAM lparam); // SOTF 7042 reloadable mall card
   virtual bool msgResendDOB(TBSTATE &tbretState, LPARAM lparam);	// SOTF7261
   virtual bool msgMEPAvailable(TBSTATE &tbretState, LPARAM lparam); //SOTF7695
   virtual bool msgGiftPhonePriceRetry(TBSTATE &tbretState, LPARAM lparam); //SOTF7701

   // pos:11273 - rrs
   virtual bool msgCheckedIfDataIsScannedSequence(TBSTATE &tbretState, LPARAM lparam);
   // e - pos:11273 - rrs
   virtual bool msgBagAloneScreen (TBSTATE &tbretState, LPARAM lparam);	//POS11024 Mobile Shopper

   virtual bool msgCheckForMediaCollection(TBSTATE &tbretState, LPARAM lparam);  //POS36319   

   virtual bool msgBuildGreenDotDataNeeded( TBSTATE &tbretState, LPARAM lparam ); //POS68773

   virtual bool msgPurgeAllReceipts( TBSTATE &tbretState, LPARAM lparam ); //POS74992

   virtual bool msgEASGenericDelayed(TBSTATE &tbretState, LPARAM lparam);	//CRD 185620
   static CString csEASItemDescription;	//CRD 185620
	virtual bool msgMEPMobileAvailable(TBSTATE &tbRetState, LPARAM lparam);	//CRD 231416
   //virtual bool msgMobileGenericDelayed(TBSTATE &tbretState, LPARAM lparam);	//CRD 223409
	virtual bool msgSendFLNotifyCancelAlert(TBSTATE &tbRetState, LPARAM lparam);
	virtual bool msgAccountLookupFailedIntervention(TBSTATE &tbRetState, LPARAM lparam);	// CRD 554916
protected:
   CCustomerMsgMap();  // should not be called outside of this class (instance function)
    
private:
   CCustomerMsgMap(const CCustomerMsgMap &);                    // hide copy const
   CCustomerMsgMap & operator =(const CCustomerMsgMap &); // hide assignment
   CString m_csCurrPOSState;
   CString m_csPrevPOSState;
   MSXML2::IXMLDOMDocumentPtr m_xmlPOSDisplay;
};

#endif

