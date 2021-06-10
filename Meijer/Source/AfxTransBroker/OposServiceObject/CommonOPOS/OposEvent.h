/////////////////////////////////////////////////////////////////////////////
//
// OposEvent.h
//
// Declaration of Event related classes.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//
#ifndef OPOS_EVENT_H
#define OPOS_EVENT_H

// Classes declared in this file.
class COposEvent;
   class COposDataEvent;
   class COposStatusUpdateEvent;
   class COposErrorEvent;
   class COposOutputCompleteEvent;
   class COposDirectIOEvent;

class COposEventReceiver;
   class COposCheckHealth;

// Abstract Base Class for Events
class COposEvent
   {
   public:
      COposEvent(  ) { }
      virtual ~COposEvent() {}
      virtual void Dispatch( COposEventReceiver& pTarget)   = 0;  
      virtual BOOL IsDataEvent( void ) const { return FALSE; }
      virtual BOOL NeedsDataEventEnabled( void ) const { return FALSE; }
      virtual BOOL IsOutputEvent( void ) const { return FALSE; }
   private:
      COposEvent( const COposEvent& );             // Disable Copies
      COposEvent& operator=( const COposEvent& );  // Disable Copies
   };


// Concrete Data Event Class
class COposDataEvent : public COposEvent
   {
   public:
     COposDataEvent()
      : m_nResult(0), m_sData(NULL), m_bAutoDataEventEnabled(FALSE)
     {
     }
	  COposDataEvent( long nResult, LPCTSTR rData, long llen=0, BOOL bAutoDataEventEnabled = FALSE )
         : m_nResult( nResult ), m_bAutoDataEventEnabled(bAutoDataEventEnabled)
		{
			if (llen == 0) llen=_tcslen(rData) * sizeof(TCHAR); 
			m_sData = new BYTE[llen + sizeof(TCHAR)];
			memcpy(m_sData,rData,llen);
         
         TCHAR *pTchar= (TCHAR *) &(m_sData[llen]);  // point to the last TCHAR character
         *pTchar = (TCHAR) NULL;
		}
	  ~COposDataEvent()
	  {
		  delete [] m_sData;
	  }
      virtual void Dispatch( COposEventReceiver& pTarget );
      virtual BOOL IsDataEvent( void ) const { return TRUE; }
      virtual BOOL NeedsDataEventEnabled( void ) const { return (m_bAutoDataEventEnabled == FALSE); }
   protected:
      const long           m_nResult;
	  BOOL  m_bAutoDataEventEnabled;
      BYTE  *m_sData;
   private:
      COposDataEvent( const COposDataEvent& );             // Disable Copies
      COposDataEvent& operator=( const COposDataEvent& );  // Disable Copies
   };

// Concrete Status Update Event Class
class COposStatusUpdateEvent : public COposEvent
   {
   public:
      COposStatusUpdateEvent( long nResult )
         : m_nResult(nResult) { }
      virtual void Dispatch( COposEventReceiver& pTarget );
   protected:
      const long           m_nResult;
   private:
      COposStatusUpdateEvent( const COposStatusUpdateEvent& );    // Disable Copies
      COposStatusUpdateEvent& operator=( const COposStatusUpdateEvent& );
   };


// Concrete Error Event Class

class COposErrorEvent : public COposEvent
   {
   public:
      COposErrorEvent( long nResult, long nRCEx, long nLocus ) 
         : m_nRC( nResult ), m_nRCEx( nRCEx ), m_nLocus( nLocus ) { }
      virtual void Dispatch( COposEventReceiver& pTarget );
      virtual BOOL NeedsDataEventEnabled( void ) const 
         { return m_nLocus != OPOS_EL_OUTPUT; }
      virtual BOOL IsOutputEvent( void ) const 
         { return m_nLocus == OPOS_EL_OUTPUT; }
   protected:
      const long     m_nRC;
      const long     m_nRCEx;
      const long     m_nLocus;
   private:
      COposErrorEvent( const COposErrorEvent& );             // Disable Copies
      COposErrorEvent& operator=( const COposErrorEvent& );  // Disable Copies
   };


// Concrete Output Complete Event Class
class COposOutputCompleteEvent : public COposEvent
   {
   public:
      COposOutputCompleteEvent( long nOutputID )
         : m_nOutputID( nOutputID ) { }
      virtual void Dispatch( COposEventReceiver& pTarget );
      virtual BOOL IsOutputEvent( void ) const { return TRUE; }
   protected:
      const long m_nOutputID;         
   };

// Concrete DirectIO Event Class
class COposDirectIOEvent : public COposEvent
   {
   public:
      COposDirectIOEvent( long nEventNumber, long nData, const CString& sData )
         : m_nEventNumber( nEventNumber ), m_nData( nData ), m_sData(sData) 
      { 
      }
      virtual void Dispatch( COposEventReceiver& pTarget );
   protected:
      const long     m_nEventNumber;
      // These are non-constant because the receiver of the event may change 
      // the values.
      long           m_nData;
      CString	      m_sData;
   private:
      COposDirectIOEvent( const COposDirectIOEvent& );    // Disable Copies
      COposDirectIOEvent& operator=( const COposDirectIOEvent& );
   };


// Event Queue Mechanism 
class COposEventQueue : private CWnd
   {
	public:
		static CString CreateEventSemaphoreName( void );
   public:
      COposEventQueue( void );
      ~COposEventQueue( void );
      void AddEvent( COposEvent* pEvent, BOOL bAddAtHead = FALSE );
      void AddInputErrorEvent( long nRC, long nRCEx );
      void AbortAll( void );
      void BeginService( COposEventReceiver* pReceiver, CmDataCapture* pDc = NULL);
      void EndService( void );
      void EnableEvents( BOOL bEnableAny, BOOL bEnableData);
      void ClearInput( void );
      void ClearOutput( void );
      long GetDataCount( void );

   private:
      CList < COposEvent*, COposEvent* > m_qList;
      CCriticalSection  m_csAccess;       // Guard Access to m_qList
      const CString     m_sClassName;     // Window Class Name
      BOOL              m_bEnableAny;     // Fire Any only when TRUE
      BOOL              m_bEnableData;    // Fire Data Only when TRUE
      int               m_nDataEvents;    // Count Data Events in the queue
      CmDataCapture*    m_pDc;           // Data Capture Events
      COposEventReceiver* m_pReceiver;
// Generated message map functions
    //{{AFX_MSG(COposEventQueue)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    afx_msg LONG OnFireEvent( UINT wParam, LONG lParam );
// User message map functions.
    DECLARE_MESSAGE_MAP()
   private:
      COposEventQueue( const COposEventQueue& );    // Disable Copies
      COposEventQueue& operator=( const COposEventQueue& );

	};


// ABC for event receiver classes
class COposEventReceiver
   {
   public:
      COposEventReceiver( ) : m_qEvents( ) {}
      virtual void DispatchErrorEvent( long nRC, long nRCEx, long nLocus) = 0;
//      virtual void DispatchDataEvent( const CString& sData, long nResult ) = 0;
//antonio start
	  virtual void DispatchDataEvent( LPCTSTR sData, long nResult ) = 0;
//antonio end
      virtual void DispatchStatusUpdateEvent( long nResult ) = 0;
      virtual void DispatchDirectIOEvent( 
         long lEventNumber, long* plData, CString& sData ) = 0;
      virtual void DispatchOutputCompleteEvent( long nOutputID ) = 0;
      virtual ~COposEventReceiver()  {}
   protected:
      // Event Operations should be called from the derived classes only
      COposEventQueue      m_qEvents;
   private:
      COposEventReceiver( const COposEventReceiver& );    // Disable Copies
      COposEventReceiver& operator=( const COposEventReceiver& );
   };


#ifdef _OPOS_ENABLE_INLINES
#define OPOS_INLINE inline
#include "OposEvent.inl"
#endif

#endif
