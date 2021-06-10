/////////////////////////////////////////////////////////////////////////////
//
// GenericCO.h
//
// Declaration of IGenericCO class.
//
// Copyright (c) 1997 NCR.  All rights reserved.
//
// Date     Modification                                          Author
//---------|---------------------------------------------------|-------------
// 97/05/19 Initial version.                                      T. Burk
//

// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IGenericCO wrapper class

class IGenericCO : public COleDispatchDriver
   {
   public:
	   IGenericCO() {}		// Calls COleDispatchDriver default constructor
//	   IGenericCO(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
//	   IGenericCO(const IGenericCO& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
   public:
	   CString GetControlObjectDescription();
	   long GetControlObjectVersion();
// Operations
   public:
      void SODirectIO(long lEventNumber, long* plData, CString& sData);
      void SOError(long lRC, long lRCEx, long lLocus, long* pResponse);
      void SOOutputComplete(long lOutputID);
      void SOStatusUpdate(long lData);
	   void SOData(long lStatus);
      BOOL Connect(LPDISPATCH lpDispatch, CmDataCapture& cmDC, BOOL bAutoRelease = FALSE);
      void Disconnect( void );
   private:
      IGenericCO( const IGenericCO& ); // Disable
      IGenericCO& operator=( const IGenericCO& );
      IConnection m_IConnect;
   };
