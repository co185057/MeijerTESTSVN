// SolutionMsgMap.h: interface for the CSolutionMsgMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SolutionMsgMap_H__INCLUDED_)
#define AFX_SolutionMsgMap_H__INCLUDED_

#include "BaseMHDisplay.h"
#include "BaseMsgMap.h"

// The definition of tMsgMapDisplay is included before the definition
// of CSolutionMsgMap in order to insure that it is created with
// the same size as the tMsgMapGeneric object defined in tbMessageMap.h
// Defining the message map after CSolutionMsgMap results in a 
// different sized pointer, which causes problems in MsgHandler 
// processing.
class CSolutionMsgMap;
class CSolutionTrxBroker;

class CSolutionMsgMap : public CBaseMsgMap
{
public:
   virtual ~CSolutionMsgMap();

   virtual void Initialize();
   virtual TBSTATE ProcessMessage();
    
   virtual bool msgAPI_EnterItem(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAPI_EnterTender(TBSTATE &tbretState, LPARAM lparam);
   //virtual bool msgAPI_Finish(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAPI_CancelTransaction(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAPI_SuspendTransaction(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAPI_SetTrainingModeIn(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAPI_SetTrainingModeOut(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgTermDisconnected(TBSTATE &tbretState, LPARAM lparam);
   
   virtual bool msgMessage300 (TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgSendWeight(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgTenderAccepted(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgDetermineIfTenderResetNeeded(TBSTATE &tbretState, LPARAM lparam);
   virtual bool msgAssistModeTrxEnd( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgAddRewardLine( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgPreserveAssistModeState( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgVoidScannedItem( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgDontReturnTBNeedAsstIfInAsst( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgRestartFastlane( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgEBTSlipPrint( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgSendKeyIfNotInAssist( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgDetermineIfSeparateMediaOkToHandleNow( TBSTATE &tbretState, LPARAM lparam );
   virtual bool msgDataNeededWithCustomerScreen( TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgAssistModeTenderInProgress( TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgNeedMICR( TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgSayPhrase(TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgSetHookProperty(TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgAMCancelRebateReceipt(TBSTATE & tbretState, LPARAM lparam );
   virtual bool msgAddWeightLine(TBSTATE &tbretState, LPARAM lparam);
   bool msgMessageScreen(TBSTATE &tbretState, LPARAM lparam);
   void SetupMessageScreen(CString, int, LPCTSTR, bool, bool);

protected:
   CSolutionMsgMap();  // should not be called outside of this class (instance function)
   virtual void LookupHandlers();
    
private:
   CSolutionMsgMap(const CSolutionMsgMap &);                    // hide copy const
   CSolutionMsgMap & operator =(const CSolutionMsgMap &); // hide assignment
   


};

#endif

