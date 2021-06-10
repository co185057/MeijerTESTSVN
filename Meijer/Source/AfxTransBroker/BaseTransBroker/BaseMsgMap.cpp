// MsgHandler.cpp: implementation of the CMsgHandler class.
//
//////////////////////////////////////////////////////////////////////
// POS320135 Work Request:76936 Name:Alex Bronola Date:April 3, 2017
// POS309602/310167 Work Request:79417 Name:Alex Bronola  Date:Feb. 24, 2017
// POS311584 Work Request:76936 Name:Alex Bronola  Date:January 23, 2017
// POS310167 Work Request:76936 Name: Alex Bronola   Date:Jan 04, 2017
// POS296302 Work Request:76936 Name: Alex Bronola   Date:Dec 15, 2016
//
#include "stdafx.h"
#include "atlbase.h"
#include "BaseMsgMap.h"
#include "TraceSystem.h"
#include "BaseTrxBroker.h"
#include "BaseMessages.h"
#include "CustomerTrxBroker.h"
#include "CustomerCurrentTransaction.h"
#include "CustomerTBConnectFL.h"
#include "CustomerMHOptions.h"
#include "CustomerTBSharedDefs.h"
#include "CurrentPosState.h"
#include "autoLock.h"

#define T_ID _T("BaseMsgMap")

const TCHAR *cTBDataEntryINI= _T("TBDataEntryINI");

CBaseMsgMap *CBaseMsgMap::m_pThis=NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseMsgMap::CBaseMsgMap()
:m_pCurrTrx(NULL),
m_pMsgDoc(NULL),
m_pmsgMapList(NULL),
m_hDelayedTriggerHandler(NULL),
m_csSavedLastPosState(_T("UNKNOWN")),
m_hUnhandledMessageWatchdog(NULL)
{
   m_pmmGeneric= new CtbMessageMap<CMsgMapNode>;
   
   m_pMsgDoc.CreateInstance(__uuidof(DOMDocument40));
   m_pMsgDoc->async = VARIANT_FALSE;
   m_pMsgDoc->validateOnParse = VARIANT_TRUE;
   
   InitializeCriticalSection(&m_csUnhandledMessageWatchdog);
   InitializeCriticalSection(&m_csDelayedTriggerHandler);

   CCustomerTrxBroker::instance()->TBSetProperty(_T("SavedLastPosState"), m_csSavedLastPosState, false);
   m_pThis=this;
}

CBaseMsgMap::~CBaseMsgMap()
{
   delete [] m_pmsgMapList;
   ClearUnhandledMessageWatchdog();
   ClearDelayedTriggerHandler();
   DeleteCriticalSection(&m_csUnhandledMessageWatchdog);
   DeleteCriticalSection(&m_csDelayedTriggerHandler);
   
   delete m_pmmGeneric;
}


void CBaseMsgMap::ClearUnhandledMessageWatchdog()
{
   CAutoLock lockThis(m_csUnhandledMessageWatchdog);
   if(m_hUnhandledMessageWatchdog )
   {
      tbtrace(TM_INFO, _T("ClearUnhandledMessageWatchdog"));
      if( !KillTimer(NULL, m_hUnhandledMessageWatchdog) )
      {
         tbtrace(TM_ERROR, _T("Failed to terminate the Unhandled Message Watchdog Timer"));
      }
      m_hUnhandledMessageWatchdog = NULL;
   }
}

void CBaseMsgMap::SetUnhandledMessageWatchdog()
{
   tbtraceIO(_T("SetUnhandledMessageWatchdog"));
   int iSeconds = _ttoi(CCustomerMHOptions::instance()->OptionValue(_T("UnhandledMessageTimeoutInSeconds")));
   if( !iSeconds )
   {
      tbtrace(TM_ERROR, _T("No timeout was specified for the Unhandled Message Timeout period. Defaulting to 5 seconds."));
      iSeconds = 5;
   }
   
   //
   // Set a new timer
   //
   CAutoLock lockThis(m_csUnhandledMessageWatchdog);
   m_hUnhandledMessageWatchdog = SetTimer(NULL, 0, iSeconds*1000, (TIMERPROC)UnhandledMessageWatchdogProc);
   if( !m_hUnhandledMessageWatchdog )
   {
      tbtrace(TM_ERROR, _T("Failed to create a new timer for the Unhandled Message Timeout wartchdog."));
   }
}

void CBaseMsgMap::Initialize()
{
   LookupHandlers();
   InitXMLDoc();
}

void CBaseMsgMap::Initialize(CMsgMapNode *pmap, int count)
{
   m_pmmGeneric->Initialize(pmap, count);
   m_pCurrTrx= CCustomerCurrentTransaction::instance();
   
#ifdef _DEBUG
   m_spMapUsage.CreateInstance(__uuidof(MSXML2::DOMDocument40));
   m_spMapUsage->async = VARIANT_FALSE;
   m_spMapUsage->validateOnParse = VARIANT_FALSE;
   VARIANT_BOOL vbRet = m_spMapUsage->load(_T("C:\\Scot\\Config\\MsgMapUsage.xml"));
   if( vbRet == VARIANT_FALSE )
   {
      m_spMapUsage->loadXML(m_pMsgDoc->xml);
   }
#endif 
   
}

///////////////////////////////////////////////////////////////////////////////
// Function:     dispatchMsgHandler
// Input:        str -- input device message
// Output:       State to return to FastLane
// Assumptions:  Slip, journal, receipt, 2x20, and display can all be handled
//               the same.
// Comments:     All entries are looped through for matches.  Entries may be
//               for the specific text string passed in or for any message for
//               a specific state.
///////////////////////////////////////////////////////////////////////////////
TBSTATE CBaseMsgMap::dispatchMsgHandler(const CString &csMessage1)
{ 
   CString csMessage= csMessage1;
   long retState = TB_IGNORE;
   int numLookups = 0;
   CMsgInfo<CMsgMapNode> *pmi;
   CMsgMapNode *pEntry;
   
   m_bmsgHandled=false;
   pmi = NULL;
   csMessage.MakeLower();
   m_pmmGeneric->Lookup(csMessage, pmi);  // Find match to exact string first
   
   long tbInProgressState = m_pCurrTrx->GetCurrentInProgressState();
   
   while (numLookups < 2)
   {
      numLookups += 1;
      if (pmi)
      {  // We have found our message in the msgMap
         POSITION pos=pmi->msgStateMap.GetHeadPosition();
         while (pos) 
         {
            pEntry= pmi->msgStateMap.GetNext(pos);
            // Dispatch the handler and update the state
            if (pEntry->currentState == tbInProgressState)
            {
               bool bExactMatchHandled= false;
               long oldstate; 
               if( (m_bmsgHandled) )
               {
                  // thta means we handled it as a exact match.
                  bExactMatchHandled= true;
                  oldstate= retState;
               }
               retState = HandleMessageMatch(pEntry);
               m_bmsgHandled= (m_bmsgHandled || bExactMatchHandled);
               if(bExactMatchHandled) // regard less of the NAY match is handled or not, the tbstate remains.
               {
                  TBASSERT(! ((TB_IGNORE != retState) && (TB_IGNORE != oldstate)));
                  if( (TB_IGNORE == retState) && (TB_IGNORE != oldstate) )
                     retState= (long) oldstate;
               }
               
            }
            // Do not break.  Keep looking for more matches even if we
            // already found a match.  There may be multiple handling 
            // routines set up.
         }
      }
      
	  static CString csPrevPosMsg = _T("");
	  static int nPrevState = 0;
	  static long prevRetState = TB_IGNORE;

      // If no match so far and trying to match on a specific message (1st time through)
      // lets try a match with cSTATE_ANY
      if ((m_bmsgHandled == false) && (numLookups == 1))
      {                          
         if (pmi)
         {  // We have found our message in the msgMap
            POSITION pos=pmi->msgStateMap.GetHeadPosition();
			
            while (pos) 
            {
               pEntry= pmi->msgStateMap.GetNext(pos);
               // Dispatch the handler and update the state
               if (pEntry->currentState == cSTATE_ANY) 
               {
				   //+POS291204, 291641, 274880 - Prevent repeated handling of state any.
				   if(csPrevPosMsg != csMessage) 
					   // || (nPrevState != tbInProgressState && prevRetState == TB_IGNORE /*POS296302 - the fix was moved to customerMsgMap to fix here also the POS309602 & 310167*/))
				   {
                       // csPrevPosMsg = csMessage; //POS310167
					   prevRetState = retState;
                       tbtrace(TM_INFO, _T("Matched Message[%s] in State[cSTATE_ANY]"), (LPCTSTR)csMessage);
                       retState = HandleMessageMatch(pEntry);
				   }
				   else
				   {
					   tbtrace(TM_INFO, _T("Ignoring multiple handling for cSTATE_ANY."));
					   m_bmsgHandled = true;
				   }
               }
               // Do not break.  Keep looking for more matches even if we
               // already found a match.  There may be multiple handling 
               // routines set up.
            }
         }
      }
      
	  if(csPrevPosMsg != csMessage)
	  {
		  tbtrace(TM_INFO, _T("Saving new Message match."));
          nPrevState = tbInProgressState;
		  csPrevPosMsg = csMessage;
		  prevRetState = retState;
	  }

      // Check if we are supposed to do something for all messages ("-1")
      pmi = NULL;
      m_pmmGeneric->Lookup(cMSG_ANY, pmi);
   } // end of while loop
   
   return (TBSTATE) retState;
}

void CBaseMsgMap::TriggerMsgMap()
{
   // safe direct call into the msg handler - avoids posting to FL and TBProcessMsg handling
   tbtraceIO(_T("TriggerMsgMap"));
   
   TBSTATE rc= dispatchHelper(m_csSavedLastPosState);
   if (rc != TB_IGNORE) 
   {
      tbtrace(TM_INFO, _T("TriggerMsgMap Posting Return Code[%s]"), 
         CCustomerTrxBroker::instance()->GetTBSTATEName(rc));
      CCustomerTBScotMsg::instance()->SendReturnStateMsg(rc);
   }
}


TBSTATE CBaseMsgMap::dispatchHelper(const CString &csMessage)
{
   TBSTATE rc;
   
   if(csMessage.GetLength() == 0)
   {
      return TB_IGNORE; 
   }
   m_csSavedLastPosState = csMessage;
   CCustomerTrxBroker::instance()->TBSetProperty(_T("SavedLastPosState"), m_csSavedLastPosState, false);

   tbtrace(TM_INFO, _T("ProcessMessage POSState=[%s], CurTBState=[%s]"), m_csSavedLastPosState, 
      CCustomerTrxBroker::instance()->GetCurrentInProgressStateName());
   
   ClearUnhandledMessageWatchdog();
   
   rc= dispatchMsgHandler(m_csSavedLastPosState);
   
   // Message handlers return true if they handled the message; otherwise, 
   // call the unhandled processing routine.
   if (!m_bmsgHandled) {
      // not handled yet. Try to match on the screen group 
      _bstr_t csGroup = CCurrentPosState::instance()->GetValue(_T("Group"));
      if (csGroup.length() > 0) {
         tbtrace(TM_INFO, _T("Attempting a Group Match:[%s]"), (LPCTSTR)csGroup);
         rc= dispatchMsgHandler(CString((LPCTSTR)csGroup));
      }
   }
   
   // if still unhandled, call catchall
   if (!m_bmsgHandled) 
   {
      m_bmsgHandled=msgUnhandledMessage(rc, 0);
      if( !m_bmsgHandled )
      {
         tbtrace(TM_WARNING, _T("Unhandled Message :%s"), (LPCTSTR) m_csSavedLastPosState);
         SetUnhandledMessageWatchdog();
      }
   }
   tbtrace(TM_INFO, _T("dispatchHelper returning[%s]"), 
         CCustomerTrxBroker::instance()->GetTBSTATEName(rc));

#ifdef _DEBUG
   m_spMapUsage->save(OLESTR("C:\\Scot\\Config\\MsgMapUsage.xml"));
#endif
   return (TBSTATE) rc;
}



void CBaseMsgMap::ParseParamString(CString &csStr, LPCTSTR szStartDelim, LPCTSTR szStopDelim)
{
   int posStart= csStr.Find(szStartDelim);
   if (posStart >=0)
   {
      int posStop= csStr.Find(szStopDelim, posStart);
      if (posStop > 0) {
         int nStartDelimLen= _tcslen(szStartDelim);
         int nStopDelimLen= _tcslen(szStopDelim);
         // evaluate property
         CString csPropertyName= csStr.Mid(posStart + nStartDelimLen, 
                                          posStop - posStart - nStartDelimLen);
         CString csPropertyValue;
         if(CString(szStartDelim) == _T("[["))
         {
            // this is a double bracket "[[name]]" so it is a hook property
            // if it's wrapped in [[]] characters then fetch the hook property value
            csPropertyValue= CCustomerTrxBroker::instance()->TBGetProperty(csPropertyName,true);
         }
         else
         { 
            // It is wrapped in [] characters. could be a sting id or a local property value
            if (_istdigit(csPropertyName[0])) 
            {
               // this is a string id.  get the string.
               int nStringID= _ttoi((LPCTSTR)csPropertyName);
               csPropertyValue= CBaseMessages::instance()->GetStringValueFromID(nStringID);
            }
            else
            {
               // this is a local property value
               csPropertyValue= CCustomerTrxBroker::instance()->TBGetProperty(csPropertyName);
            }
         }
         if (csPropertyValue.IsEmpty())
             tbtrace(TM_WARNING, _T("Msg Map Property:%s Value is empty"), (LPCTSTR)csPropertyName);
         csStr = csStr.Left(posStart) + 
                 csPropertyValue +
                 csStr.Mid(posStop + nStopDelimLen);
         // recurse...
         ParseParamString(csStr, szStartDelim, szStopDelim);
      }
   }
   return;
}


///////////////////////////////////////////////////////////////////////////////
// Function:     HandleMessageMatch
///////////////////////////////////////////////////////////////////////////////
long CBaseMsgMap::HandleMessageMatch(CMsgMapNode *pEntry)
{
   USES_CONVERSION;
   ASSERT(m_pCurrTrx != NULL);  // ensure this is not NULL
   TBSTATE retState = TB_IGNORE;
   
   // a Trace command in the MsgMap would be parsed and traced here.
   if( pEntry->csTrace.GetLength() > 0 )
   {
      CString csNewTrace = pEntry->csTrace;
      ParseParamString(csNewTrace, _T("[["), _T("]]"));
      ParseParamString(csNewTrace, _T("["), _T("]"));
      tbtrace(TM_INFO, _T("MsgMapTrace: %s"), (LPCTSTR)csNewTrace);
   }

   // If a match is found, set the msg handled flag in case no 
   // msghandler is defined.  The msghandler may override and
   // return false if it does not want to handle it.
   m_bmsgHandled = true;
   // Set the new progress state, if specified, before calling
   // the handler
   if (pEntry->newProgressState > -1)
   {
      m_pCurrTrx->SetCurrentInProgressState(pEntry->newProgressState);
   }
   // Set the new tb return state, if specified, before
   // calling the handler
   if ((pEntry->tbretState > -1))
   {
      retState = pEntry->tbretState;
   }
   
   MsgHandlerPairList & handlers = pEntry->msgHandlers;
   MsgHandlerPairListIterator iter = NULL;
   for( iter = handlers.begin(); iter < handlers.end(); iter++)
   {
      CString csNewLparam((LPCTSTR) iter->second);
      ParseParamString(csNewLparam, _T("[["), _T("]]"));
      ParseParamString(csNewLparam, _T("["), _T("]"));

      // Process the message through the defined handler
      m_bmsgHandled = (this->*(tMethodPointer)iter->first)
         (retState,
         (LPARAM)(LPCTSTR)csNewLparam);
   }
#ifdef _DEBUG
   {
      _bstr_t bstrScreen = pEntry->pmsg;
      _bstr_t bstrState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
      try
      {
         _bstr_t bstrNewState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
         
         _bstr_t bstrQuery = OLESTR("//MapEntry[(TBCurrentInProgress = '") + bstrState + OLESTR("' or TBCurrentInProgress = 'cSTATE_ANY')");
         if( bstrScreen.length() )
         {
            bstrQuery += OLESTR(" and (POSMessage = '") + bstrScreen + OLESTR("')");
         }
         if( bstrNewState.length() )
         {
            bstrQuery += OLESTR(" and (TBNewInProgress = '") + bstrNewState + OLESTR("' or TBNewInProgress = 'NO_STATE_CHANGE')");
         }
         bstrQuery += OLESTR("]");
         
         MSXML2::IXMLDOMNodePtr spNode = m_spMapUsage->selectSingleNode(bstrQuery);
         if( spNode != NULL ) 
         {
            MSXML2::IXMLDOMAttributePtr spAttr = spNode->attributes->getNamedItem(OLESTR("Hits"));
            if( spAttr == NULL )
            {
               spAttr = m_spMapUsage->createAttribute(OLESTR("Hits"));
               spAttr->value = OLESTR("1");
               spNode->attributes->setNamedItem(spAttr);
            }
            else
            {
               long lHits = _ttol(OLE2T(spAttr->value.bstrVal));
               
               lHits++;
               CString strHits;
               strHits.Format(_T("%d"), lHits);
               spAttr->value = _bstr_t(strHits);
            }
         }
         else
         {
            tbtrace(TM_WARNING, _T("HandleMessageMatch: Didn't find usage map entry to update hit count."));
         }
      }
      catch(_com_error & cex)
      {
         tbtrace(TM_WARNING, _T("HandleMessageMatch: Failed to increase the state usage hit count: %s"), cex.ErrorMessage());
      }
      
   }
#endif
   
   return retState;
}


bool CBaseMsgMap::msgUnhandledMessage(TBSTATE &tbretState, LPARAM lparam )
{
   tbtrace(TM_INFO, _T("msgUnhandledMessage"));
   tbretState = TB_IGNORE; // default to ignore
   return false;
}

VOID CALLBACK UnhandledMessageWatchdogProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
   tbtraceIO(_T("UnhandledMessageWatchdogProc"));
   int nRetCode= CCustomerMHOptions::instance()->IntOptionValue(CString(_T("UnhandledReturnCode")));
   if ((nRetCode != 0) && (nRetCode != TB_IGNORE))
       CCustomerTBScotMsg::instance()->SendReturnStateMsg(nRetCode);
   CBaseMsgMap::m_pThis->ClearUnhandledMessageWatchdog();
}

VOID CALLBACK DelayedTriggerHandlerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
   tbtraceIO(_T("DelayedTriggerHandlerProc"));
   CBaseMsgMap::m_pThis->ClearDelayedTriggerHandler();
   CBaseMsgMap::m_pThis->TriggerMsgMap();
}

void CBaseMsgMap::SetDelayedTriggerHandler(DWORD dwDelay)
{
   tbtrace(TM_INFO, _T("SetDelayedTriggerHandler(%d) milliseconds."), dwDelay);
   
   CAutoLock lockThis(m_csDelayedTriggerHandler);
   m_hDelayedTriggerHandler = SetTimer(NULL, 0, dwDelay, (TIMERPROC)DelayedTriggerHandlerProc);
   if( !m_hDelayedTriggerHandler )
   {
      tbtrace(TM_ERROR, _T("Failed to create a new timer for the Saved Screen Change handler."));
   }
}

void CBaseMsgMap::ClearDelayedTriggerHandler()
{
   CAutoLock lockThis(m_csDelayedTriggerHandler);
   if( m_hDelayedTriggerHandler )
   {
      tbtraceIO(_T("ClearDelayedTriggerHandler"));
      if( !KillTimer(NULL, m_hDelayedTriggerHandler) )
      {
         tbtrace(TM_ERROR, _T("Failed to terminate the DelayTrigger handler."));
      }
      m_hDelayedTriggerHandler = NULL;
   }
}

void CBaseMsgMap::InitXMLDoc()
{
   MSXML2::IXMLDOMParseErrorPtr pError;
   MSXML2::IXMLDOMElementPtr ElementPtr;
   MSXML2::IXMLDOMElementPtr subElementPtr;
   
   CString csMsgMapFile = CCustomerMHOptions::instance()->OptionValue(_T("MsgMapXML"));
   CCustomerTrxBroker *pTB= CCustomerTrxBroker::instance();
   
   _bstr_t strResult = _T("");
   _bstr_t strFile = csMsgMapFile;
   
   HRESULT hr = m_pMsgDoc->load(strFile);
   pError = m_pMsgDoc->parseError;
   ASSERT(pError->errorCode == S_OK);
   if (pError->errorCode != S_OK)
   {
      strResult = _bstr_t("Validation failed on ") + strFile +
         _bstr_t("\n=====================") +
         _bstr_t("\nReason: ") + _bstr_t(pError->Getreason()) +
         _bstr_t("\nSource: ") + _bstr_t(pError->GetsrcText()) +
         _bstr_t("\nLine: ") + _bstr_t(pError->Getline());
      tbtrace(TM_ERROR, strResult);
      return;
   }
   
   const _bstr_t cMAP_ENTRY = _bstr_t(OLESTR("/MapEntries/MapEntry"));
   MSXML2::IXMLDOMNodeListPtr spList= m_pMsgDoc->selectNodes(cMAP_ENTRY);
   
   int indx=0;
   delete [] m_pmsgMapList;
   int count= spList->length;
   m_pmsgMapList= new CMsgMapNode[count];
   
   while( (ElementPtr = spList->nextNode()) != NULL) {
      
      subElementPtr= ElementPtr->selectSingleNode((_T("TBCurrentInProgress")));
      ASSERT(subElementPtr != NULL);
      ASSERT(indx < count);
      if (subElementPtr) {
         _bstr_t bstrCurState = subElementPtr->text;
         m_pmsgMapList[indx].currentState= pTB->GetCurrentInProgressState(bstrCurState);
         if( !bstrCurState.length() ) bstrCurState = OLESTR("");

         subElementPtr= ElementPtr->selectSingleNode(_T("POSMessage"));
         ASSERT(subElementPtr!= NULL);
         CopyString(m_pmsgMapList[indx].pmsg, subElementPtr->text);
         
         // the Trace node is optional
         subElementPtr = ElementPtr->selectSingleNode(_T("Trace"));
         if( subElementPtr != NULL )
         {
            m_pmsgMapList[indx].csTrace = (LPCTSTR)subElementPtr->text;
         }

         subElementPtr= ElementPtr->selectSingleNode(_T("TBNewInProgress"));
         ASSERT(subElementPtr!= NULL);
         _bstr_t bstrNewState = subElementPtr->text;
         m_pmsgMapList[indx].newProgressState= pTB->GetCurrentInProgressState(bstrNewState);
         if( !bstrNewState.length() ) bstrNewState = OLESTR("");
         
         subElementPtr= ElementPtr->selectSingleNode(_T("TBReturnCode"));
         ASSERT(subElementPtr!= NULL);
         _bstr_t bstrRetState = subElementPtr->text;
         m_pmsgMapList[indx].tbretState= (TBSTATE) pTB->GetTBSTATEFromName(bstrRetState);
         if( !bstrRetState.length() ) bstrRetState = OLESTR("");
         
         tbtrace(TM_INFO, _T("Loading Message Map Entry %s / %s / %s / %s at address 0x%08X..."), 
            (LPCTSTR)bstrCurState, m_pmsgMapList[indx].pmsg, (LPCTSTR)bstrNewState, (LPCTSTR)bstrRetState, &m_pmsgMapList[indx]);

         // Go through each pair of TriggerMethod & MethodParam nodes
         MSXML2::IXMLDOMNodeListPtr spList = ElementPtr->selectNodes(OLESTR("TriggerMethod|MethodParam"));
         MSXML2::IXMLDOMNodePtr spNode = NULL;
         spList->reset();
         MsgHandlerPair pair(NULL, NULL);

         while( (spNode = spList->nextNode()) != NULL )
         {
             if( spNode->nodeName == _bstr_t(OLESTR("TriggerMethod")) )
             {
                // if the last paid had not been submitted yet, submit and reset
                if( pair.first )
                {
                    m_pmsgMapList[indx].msgHandlers.push_back(pair);
                    pair.first = NULL;
                    pair.second = NULL;
                }
                
                pair.first = (tMethodPointer) GetMethodPointer(spNode->text);
                tbtrace(TM_INFO, _T("    Adding TriggerMethod %s with address 0x%08X"), (LPCTSTR)spNode->text, pair.first);
             }
             else if( spNode->nodeName == _bstr_t(OLESTR("MethodParam")) )
             {
                 if( pair.second != NULL )
                 {
                    tbtrace(TM_ERROR, _T("Two MethodParam entries have been detected in a row. Discarding second entry."));
                 }
                 else if( pair.first == NULL )
                 {
                     tbtrace(TM_ERROR, _T("MethodParam specified with no TriggerMethod. Discarding this entry."));
                 }
                 else if( spNode->text.length() )
                 {
                     tbtrace(TM_INFO, _T("    Adding MethodParam %s "), (LPCTSTR)spNode->text);
                     CopyString((LPTSTR &)pair.second, spNode->text);
                     m_pmsgMapList[indx].msgHandlers.push_back(pair);
                     pair.first = NULL;
                     pair.second = NULL;
                 }
             }
         }
        // if the last paid had not been submitted yet, submit
        // we're here because the last handler only had a TriggerMethod
        if( pair.first )
        {
            m_pmsgMapList[indx].msgHandlers.push_back(pair);
            pair.first = NULL;
            pair.second = NULL;
        }
      }
      
      indx++;
   }
   Initialize((CMsgMapNode *)m_pmsgMapList, count);
   
}


tMethodPointer CBaseMsgMap::GetMethodPointer(LPCTSTR szMethodName)
{
   if ((szMethodName == NULL) || (szMethodName[0]==NULL))
      return NULL;
   
   CMapNameToMethodPointer::iterator myIterator;
   myIterator= m_FunctionLookup.find(szMethodName);
   if(myIterator != this->m_FunctionLookup.end())
   {
      return myIterator->second;
   }
   return NULL;
}

bool CBaseMsgMap::msgSetFLState(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgSetFLState"));
   CCustomerTrxBroker *pTB= CCustomerTrxBroker::instance();
   CTBInProgress *pInProg= CTBInProgress::instance();
   pTB->SetFLState((LPCTSTR) lparam);
   return true;
}

bool CBaseMsgMap::msgSendReturnStatusMsgs(TBSTATE &tbretState, LPARAM lparam)
{
   LPTSTR pszMsgs=NULL;
   
   tbtraceIO(_T("msgSendReturnStatusMsgs"));
   
   CopyString(pszMsgs, (LPCTSTR)lparam);
   ASSERT(pszMsgs && *pszMsgs);
   
   CCustomerTrxBroker & trxBroker = *CCustomerTrxBroker::instance(); 
   TCHAR * pszMsg = _tcstok(pszMsgs, _T(";"));
   while( pszMsg )
   {
      TBSTATE tbrc = (TBSTATE) trxBroker.GetTBSTATEFromName(pszMsg);
      tbtrace(TM_INFO, _T("SendReturnStatusMsg(%s)"), pszMsg);
      CCustomerTBScotMsg::instance()->SendReturnStateMsg(tbrc);
      
      pszMsg = _tcstok(NULL, _T(";"));
   }
   delete [] pszMsgs;
   return true;
}

bool CBaseMsgMap::msgMessageScreen(TBSTATE &tbretState, LPARAM lparam )
{
   tbtraceIO(_T("msgMessageScreen"));
   
   LPCTSTR pszMsg = (LPCTSTR)lparam;
   ASSERT(pszMsg && *pszMsg);
   
   // expecting the following parameters
   // 
   // 1) Screen Text ID  --> (indirect lookup from property if enclosed in [ ]
   // 2) Leadthru Text ID --> (indirect lookup from property if enclosed in [ ]
   // 3) Audio file to play --> (name only, no path expected)
   // 4) Need cancel button --> '1' = yes  anything else = no
   // 5) Need go back to Scan And Bag --> '1' = yes anything else = no
   LPTSTR pszParams = NULL;
   CopyString(pszParams, pszMsg);
   
   CString strScreenText = _T(""); 
   CString strLeadThru = _T("");
   CString strAudio = _T("");
   CString strCancel = _T("");
   CString strGoBack = _T("");
   
   LPCTSTR psz;
   
   // Get all the variables
   psz = _tcstok(pszParams, _T(";"));
   if( psz )
   {
      strScreenText = psz;
      psz = _tcstok(NULL, _T(";"));
      if( psz )
      {
         strLeadThru = psz;
         psz = _tcstok(NULL, _T(";"));
         if( psz )
         {
            strAudio = psz;
            psz = _tcstok(NULL, _T(";"));
            if( psz )
            {
               strCancel = psz;
               strCancel.TrimLeft();
               strCancel.TrimRight();
               
               psz = _tcstok(NULL, _T(";"));
               if( psz )
               {
                  strGoBack = psz;
                  strGoBack.TrimLeft();
                  strGoBack.TrimRight();
               }
            }
         }
      }
   }

   tbtrace(TM_INFO, _T("ScreenText='%s'; LeadThru='%s'; Audio='%s'; CancelButton='%s'; BackToScanAndBag='%s'"),
			strScreenText, strLeadThru, strAudio, strCancel, strGoBack);

   
   SetupMessageScreen(_ttoi(strScreenText), _ttoi(strLeadThru), strAudio, strCancel == _T("1"), strGoBack == _T("1"));
   
   delete [] pszParams;
   
   return true;
}

void CBaseMsgMap::SetupMessageScreen(int nScreenTextID, int nLeadThruTextID, LPCTSTR strWaveFile, bool bNeedCancel, bool bNeedGoBack)
{
   CBaseMessages *pMessages = CBaseMessages::instance();
   CCustomerTrxBroker *pCurrTrx = CCustomerTrxBroker::instance();
   CCustomerTBConnectFL &tbConnectFL = *CCustomerTBConnectFL::instance();
   
   SMessageInfo msgMessageObj;
   memset(&msgMessageObj,0,sizeof(SMessageInfo));
   
   CString csWaveFile = strWaveFile;
   CString csScreenText = pMessages->GetStringValueFromID(nScreenTextID);
   CString csLeadThruText = pMessages->GetStringValueFromID(nLeadThruTextID);
   msgMessageObj.szLeadThruText    = (LPTSTR)(LPCTSTR)csLeadThruText;
   msgMessageObj.szScreenText      = (LPTSTR)(LPCTSTR)csScreenText;
   msgMessageObj.szPlayWave        = (LPTSTR)(LPCTSTR)csWaveFile;
   msgMessageObj.bNeedItemCancel   = bNeedCancel;
   msgMessageObj.bGoBackScanAndBag = bNeedGoBack;
   tbConnectFL.SetMessageInfo(&msgMessageObj);
}

bool CBaseMsgMap::msgDataNeeded(TBSTATE &tbretState, LPARAM lparam)
{
   CString strINISectionHeader;
   CString strDescID, strView;
   
   tbtraceIO(_T("msgDataNeeded"));
   
   CString csStr= (LPCTSTR)lparam;
   int posStop= csStr.Find(_T(";"));
   if (posStop >= 0) 
   {
      strINISectionHeader= csStr.Left(posStop);
	  int posStart=posStop +1;
	  posStop= csStr.Find(_T(";"), posStart);
	  if (posStop >= 0)
	  {
         strDescID= csStr.Mid(posStart, 
                              posStop - posStart);
		 if (csStr.GetLength() > posStop)
	         strView = csStr.Mid(posStop + 1);
	  }
	  else
         strDescID= csStr.Mid(posStart);
   }
   else
      strINISectionHeader= csStr;


   CCustomerTrxBroker *pCurTrx = CCustomerTrxBroker::instance();
   CBaseMessages *pMessages = CBaseMessages::instance();
   
   CString csView(_T(""));
   SDataNeededView MyDataNeeded={0};
   
   MyDataNeeded.szSection = strINISectionHeader.GetBuffer(0);

   CString csDataEntryFileName;
   if (strINISectionHeader.GetLength()>0)
   {
		csDataEntryFileName = CCustomerMHOptions::instance()->OptionValue(cTBDataEntryINI);
		MyDataNeeded.szIniFile = csDataEntryFileName.GetBuffer(0);
   }
   CString csDesc;
   if(_ttoi(strDescID) != 0) 
   {
   		csDesc= pMessages->GetStringValueFromID(_ttoi(strDescID)).GetBuffer(0);
		MyDataNeeded.szDescription= csDesc.GetBuffer(0);
   }
   else
      MyDataNeeded.szDescription= _T(" ");
   
   MyDataNeeded.szView = strView.GetBuffer(0);
   tbtrace(TM_INFO, _T("DataNeeded Section:[%s], Desc:[%s], szView:[%s]"),
	   MyDataNeeded.szSection, MyDataNeeded.szDescription, MyDataNeeded.szView);
   CCustomerTBConnectFL::instance()->SetDataNeededView( &MyDataNeeded);
   
   return true;
}

bool CBaseMsgMap::msgAddLineToRAP(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAddLineToRAP"));
   
   LPCTSTR pszMsg = (LPCTSTR)lparam;
   ASSERT(pszMsg && *pszMsg);
   
   CString strMsg = pszMsg;
   
   tbtrace(TM_INFO, _T("SendRapDisplayMsg: %s"), (LPCTSTR)strMsg);   

   CCustomerTBScotMsg::instance()->SendRAPDisplayMsg(strMsg);
   
   return true;
}

bool CBaseMsgMap::msgSendSequence(TBSTATE &tbretState, LPARAM lparam)
{
    tbtrace(TM_ENTRY, _T("msgSendSequence:%s"), (LPCTSTR)lparam);
    CCustomerTBConnectPOS::instance()->SendSequence(CString((LPCTSTR) lparam));
    return true;
}

void CBaseMsgMap::AddHandlerLookup(LPCTSTR methodname, tMethodPointer methodPtr)
{
   tbtrace(TM_INFO, _T("AddHandlerLookup:%s"), methodname);
   m_FunctionLookup[methodname]= methodPtr;
}

TBSTATE CBaseMsgMap::ProcessMessage(CTBMsgObj *pMsgObj)
{
   CCustomerTrxBroker & Trx = *CCustomerTrxBroker::instance();
   CCurrentPosState &PosState = *CCurrentPosState::instance();
   _bstr_t csCurPosStateBstr= PosState.GetValue(_T("PosState"));
   CString csCurPosState = (LPCTSTR)csCurPosStateBstr;
   
   if (m_csSavedLastPosState == csCurPosState)
   {
	  // SASA: we should just be looking at the variant boolean representation
      _bstr_t sVal= PosState.GetValue(_T("IgnoreDuplicate"));
	  CString strVal = (LPCTSTR)_bstr_t(PosState.GetValue(_T("IgnoreDuplicate")));
	  strVal.MakeUpper();
	  
      if( (_T("1") == strVal) || (_T("TRUE") == strVal) )
      {
         tbtrace(TM_INFO, _T("Duplicate POSState:%s ignored"), m_csSavedLastPosState);
         return TB_IGNORE;
      }
   }
   // get the new POSState Trigger delay value if any.
   _bstr_t sVal= PosState.GetValue(_T("TriggerDelay"));
   int lnewDelay = _ttoi((LPCTSTR) sVal); 
   if (m_hDelayedTriggerHandler) 
   {  // there is a state waiting to trigger!
      ClearDelayedTriggerHandler(); // first clear it
      // if the new state is the same, then we can ignore/clear the waiting state and continue.
      // if the new state is different, we must trigger the old state first, then the new one.
      if (m_csSavedLastPosState != csCurPosState)
      {
         tbtrace(TM_WARNING, _T("Forcing early Trigger for:[%s] because of new different POSState"), m_csSavedLastPosState);
         TBSTATE rc= dispatchHelper(m_csSavedLastPosState); // trigger the old state now!

         // now process the new one
         if( lnewDelay > 0 )
         {
            // the timer is now responsible for calling TriggerMsgMap.  No delays will occur there.
            SetDelayedTriggerHandler(lnewDelay);
         }    
         else
         {
            // the new POSState does not have a trigger delay.  trigger it now 
            TBSTATE newState= dispatchHelper(csCurPosState);
            if (newState != TB_IGNORE) 
            {
               if (rc == TB_IGNORE) // if the oldstate is TB_IGNORE, just return the new state.
                  rc = newState;
               else
               {  // we have to return 2 non TB_IGNORE states. 
                  // return the old state to keep the order and post the new one.  
                  tbtrace(TM_WARNING, _T("ProcessMessage Posting New State Code[%s]"), 
                           CCustomerTrxBroker::instance()->GetTBSTATEName(newState));
                  CCustomerTBScotMsg::instance()->SendReturnStateMsg(newState);
               }
            }
         }
         return rc; // we must return the old state first unless it is a TB_IGNORE
      }
   }

   if( lnewDelay > 0 )
   {
      SetDelayedTriggerHandler(lnewDelay);
      // the timer is now responsible for calling TriggerMsgMap.  No delays will occur there.
      return TB_IGNORE;
   }    
   return dispatchHelper(csCurPosState);
}

// msgAPI_Finish()
// is called by the message map as a FLAPI handler for TBFinish.
// 
bool CBaseMsgMap::msgAPI_Finish(TBSTATE &tbretState, LPARAM lparam)
{
   tbtraceIO(_T("msgAPI_Finish"));
   CCustomerCurrentTransaction::instance()->ResetTransactionDetails();
   return true;
}

bool CBaseMsgMap::msgSetProperty(TBSTATE &tbretState, LPARAM lparam)
{
    tbtrace(TM_ENTRY, _T("msgSetProperty:%s"), (LPCTSTR)lparam);

	LPCTSTR psz;
	CString strProperty;
	CString strValue, strType;
	bool bType;
   
	// Get all the variables
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);
   
	LPTSTR pszParams = NULL;
	CopyString(pszParams, pszMsg);
   
	psz = _tcstok(pszParams, _T(";"));
	if( psz )
	{
      strProperty = psz;
      psz = _tcstok(NULL, _T(";"));
      if( psz )
      {
         strValue = psz;
         psz = _tcstok(NULL, _T(";"));
         if( psz )
		 {
			 strType = psz;
             bType = (_ttoi((LPCTSTR)psz) ? true : false);
		 }
      }
	}
	delete [] pszParams;

    CCustomerTrxBroker::instance()->TBSetProperty(strProperty, strValue, bType);
    return true;
}

//We have a lot of logics in SSF and TB that are dependent on POSState, so adding a new POSState
//would have a chance to break those logics. So, this is a provision to restore back the known POSState.
//This is very useful after triggering something from non-dynaframe POSState.
//Just be careful not to abuse it.
bool CBaseMsgMap::msgOverridePOSState(TBSTATE &tbretState, LPARAM lparam)
{
    tbtrace(TM_ENTRY, _T("msgOverridePOSState:%s"), (LPCTSTR)lparam);
	
	
	LPCTSTR pszMsg = (LPCTSTR)lparam;
	ASSERT(pszMsg && *pszMsg);

	m_csSavedLastPosState = pszMsg;
	CCustomerTrxBroker::instance()->TBSetProperty(_T("SavedLastPosState"), m_csSavedLastPosState, false);
    CCustomerTrxBroker::instance()->TBSetProperty(_T("__POSState"), m_csSavedLastPosState, true);
	
    return true;
}

void CBaseMsgMap::LookupHandlers()
{
   AddHandlerLookup(_T("msgMessageScreen"),        (tMethodPointer) &CBaseMsgMap::msgMessageScreen);
   AddHandlerLookup(_T("msgSetFLState"),           (tMethodPointer) &CBaseMsgMap::msgSetFLState);
   AddHandlerLookup(_T("msgSendReturnStatusMsgs"), (tMethodPointer) &CBaseMsgMap::msgSendReturnStatusMsgs);
   AddHandlerLookup(_T("msgDataNeeded"),           (tMethodPointer) &CBaseMsgMap::msgDataNeeded);
   AddHandlerLookup(_T("msgAddLineToRAP"),         (tMethodPointer) &CBaseMsgMap::msgAddLineToRAP);
   AddHandlerLookup(_T("msgSendSequence"),	       (tMethodPointer) &CBaseMsgMap::msgSendSequence);
   AddHandlerLookup(_T("msgAPI_Finish"),           (tMethodPointer) &CBaseMsgMap::msgAPI_Finish);
   AddHandlerLookup(_T("msgSetProperty"),          (tMethodPointer) &CBaseMsgMap::msgSetProperty);
   AddHandlerLookup(_T("msgOverridePOSState"),     (tMethodPointer) &CBaseMsgMap::msgOverridePOSState);
}
