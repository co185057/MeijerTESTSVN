/////////////////////////////////////////////////////////////////////////////
//
// printer.h
//
//   Declaration of IDispatch wrapper class for CPrinterWrapper
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.2/Development/Core/Source/SCOTAPP/dmx/printer.h $
//
// $Revision: 6 $
//
// $Date: 3/03/08 8:26a $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __PRINTER_H__
#define __PRINTER_H__

#ifndef _CPPUNIT
#include "TOPOSPOSPrinter.h"
#include "OposPtr.h"	// RFC 367092
#else
class CTestingPrinter;
#endif

class CPrintStation;
class CPrintMapElement;

class CPrintRequest
   {
   public:
      virtual long Process( CPrintStation& rPrinter ) const = 0;
	  virtual ~CPrintRequest() {}
   };

class CPrintTextRequest : public CPrintRequest
   {
   public:
      CPrintTextRequest ( const CString& sData ) 
         : m_sData( sData )  {}
      virtual long Process( CPrintStation& rPrinter ) const;
   private:
      const CString m_sData;
   };

class CPrintBarcodeRequest : public CPrintRequest
   {
   public:
      CPrintBarcodeRequest( const CString& sName, long nType, 
         long nHeight, long nWidth, long nAlign, long  nTextPosition,
         long nBinaryConversion=-1 )
         : m_sName( sName ), m_nType( nType ), 
         m_nHeight( nHeight ), m_nWidth( nWidth ), m_nAlign( nAlign ), 
         m_nTextPosition( nTextPosition ), 
         m_nBinaryConversion( nBinaryConversion )  {}
      virtual long Process( CPrintStation& rPrinter ) const;
   private:
      const CString m_sName;
      const long m_nType;
      const long m_nHeight;
      const long m_nWidth;
      const long m_nAlign;
      const long m_nTextPosition;
      const long m_nBinaryConversion;       // TAR 352038
   };

class CPrintBitmapRequest : public CPrintRequest
   {
   public:
      CPrintBitmapRequest( const CString& sName, long nWidth, 
         long nAlign )
         : m_sName( sName ), m_nWidth( nWidth ), m_nAlign( nAlign ) {}
      virtual long Process( CPrintStation& rPrinter ) const;
   private:
      const CString m_sName;
      const long m_nWidth;
      const long m_nAlign;
   };

class CCutPaperRequest : public CPrintRequest
   {
   public:
      CCutPaperRequest( long nPercentage )
         : m_nPercentage( nPercentage ) {}
      virtual long Process( CPrintStation& rPrinter ) const;
   private:
      const long m_nPercentage;
   };
// RFQ 7477
class CChangePrintSideRequest : public CPrintRequest
   {
public:
    CChangePrintSideRequest(long nPrintSide)
        : m_nPrintSide( nPrintSide ) {}
      virtual long Process( CPrintStation& rPrinter ) const;
    private:
    const long m_nPrintSide;
   };
// RFQ 7477
class CSetRecLineCharRequest : public CPrintRequest
   {
   public:
      CSetRecLineCharRequest( long nLineChars )
         : m_nLineChars( nLineChars ) {}
      virtual long Process( CPrintStation& rPrinter ) const;
      virtual bool IsBlockingRequest() { return true; }
   private:
      const int m_nLineChars;
   };

class CPrinterWrapper : public TOPOSPOSPrinter<CGenericOposIF>
   {
   public:  
      static TClassPtr<CGenericOposIF> CreateDevice( HKEY hKey, long nDeviceId );
      CPrinterWrapper*  GetPrinter( void ) { return this; }
      // this method will throw if station is not present. 
      CPrintStation*    GetPrintStation( long nDeviceClass );
      long RetryOutput( long nStation );
      long PrintSynchronous(long nStation );
   public:  
      virtual void OnOposOutputCompleteEvent( long nOutputId );
      virtual void OnOposDirectIOEvent( long nEventId, long* plData, BSTR* pbstrData ); // RFQ 7477
      virtual void OnOposErrorEvent( long nResult, long nRcEx );  
      virtual void OnOposStatusEvent( long nStatus );
      virtual BOOL IsAvailable( void );
      virtual void ReadRegistry( HKEY hKey );
      virtual long DoOneTimeInitialization(void);
      void CreateStation( long nDeviceClass, long nIndex );
   public:
	   BOOL Create( CWnd* pParentWnd, UINT nID );
   // Attributes
   public:
	   BOOL GetCapJrnPresent();
	   BOOL GetCapRecPresent();
	   BOOL GetCapSlpPresent();
   // Operations
   public:
	   long ClearOutput();
	   long PrintNormal(long Station, CString Data);
	   long CutPaper(long Percentage);
       long ChangePrintSide(long nPrintSide); // RFQ 7477
	   long RotatePrint(long Station, long Rotation);
	   long PrintBarCode(long Station, LPCTSTR Data, long Symbology, long Height, long Width, long Alignment, long TextPosition);
	   long PrintBitmap(long Station, LPCTSTR FileName, long Width, long Alignment);
       long SaveSetLineCharRequest(long lStation, int nLineChars);
       long GetDefaultLineChars() {return m_nDefaultLineChars;};
   public:  // pure virtuals from CGenericOposIF
      virtual long GetDeviceClass( void ) const 
         { return DMCLASS_PRINTER; }
   protected:
      void AddRequest( long nStation, TClassPtr<CPrintRequest> pRequest );
      CPrinterWrapper( long nDeviceId );
      long GetDeviceClass( long nStation ) const;
   protected:
      BOOL     m_bUseReceipt; // read these from DMX registry
      BOOL     m_bUseJournal;
      BOOL     m_bUseSlip;
      long     m_nCurrentStation;
      long	   m_nPartialCutPercentage ;
      long     m_nDefaultCharacterSet;  //.rvl added for Req 3.2.12.1 and 3.2.12.3
      long     m_nDefaultLineChars;	// RFC 367092
      BOOL     m_bAsynchronousPrint;
      bool     m_bPrintSynchronously; 
      bool     m_bRetrying;
      CList<TClassPtr<CPrintMapElement>, TClassPtr<CPrintMapElement> > m_listOfRequests;
      CArray< TClassPtr<CPrintStation>, TClassPtr<CPrintStation> > m_aStations;
   };

class CPrintStation
   {
   public:
      CPrintStation( CPrinterWrapper& rPtr, long nDeviceId, long nStation )
         : m_rPtr( rPtr ), m_nStation( nStation ), m_nDeviceId( nDeviceId )
         { 
         ASSERT( nDeviceId == 0 );
         }
// +RFC 367092 made all these below functions Virtual
      virtual long PrintNormal( const CString& sData );
      virtual long PrintBarcode( const CString& sName, long nType, long nHeight, long nWidth,
         long nAlign, long nTextPosition );
     virtual long PrintBitmap( const CString& sName, long nWidth, long nAlign );
     virtual long CutPaper( long nPercentage );
     virtual long RetryOutput( void );
     virtual long PrintSynchronous( void );
     virtual void ClearOutput( void );
     virtual long GetResultCode( void ) const;  //Error Handing
     virtual long GetResultCodeExtended( void ) const;
     virtual long CheckHealth( long lLevel );
// -RFC 367092 made all these below functions Virtual

      BOOL IsAvailable( void ) const 
         { return GetPrinter().IsAvailable(); }
      virtual long BeginInsertion( long lTimeout )
         { /*ASSERT( FALSE );*/ return OPOS_E_ILLEGAL; }
      virtual long BeginRemoval( long lTimeout )
         { /*ASSERT( FALSE );*/ return OPOS_E_ILLEGAL; }
      virtual long BeginRemovalForced( long lTimeout ) // RFQ 7477
         { ASSERT( FALSE ); return OPOS_E_ILLEGAL; }
      virtual long EndInsertion( void )
         { /*ASSERT( FALSE );*/ return OPOS_E_ILLEGAL; }
      virtual long EndRemoval( void )
         { /*ASSERT( FALSE );*/ return OPOS_E_ILLEGAL; }
      long GetState( void ) 
         { return GetPrinter().GetState( ); }
   public:
	   virtual long SetBitmap( long nNumber, LPCTSTR sFileName, long nWidth, long nAlignment);
      virtual long GetLineWidth( void ) const = 0;
      virtual long GetLineHeight( void ) const = 0;
      virtual long GetLineChars( void ) const = 0;
	  virtual void SetLineChars( int lineChars)  const
	  {  GetPrinter().SetRecLineChars(lineChars); }     //tar 266885
      virtual bool GetPaperEmpty( void ) const = 0;
      virtual bool GetPaperNearEmpty( void ) const = 0;
      virtual void FireStatusEvent( long nStatus );
	  long ChangePrintSide( long nPrintSide ); // RFQ 7477
   public:
      CPrinterWrapper& GetPrinter( void ) const;
   protected:
      // All stations print strings - pass wrapper class and data
      virtual long OnPrintNormal( const CString& sData ) const;
      virtual long OnPrintBarcode( const CString& sName, long nType, long nHeight, 
         long nWidth, long nAlign, long nTextPosition ) const;
      virtual long OnPrintBitmap( const CString& sData, long nWidth, long nAlign ) const;
      virtual long OnCutPaper( long nPercentage ) const;
      virtual long OnChangePrintSide( long nPrintSide) const; // RFQ 7477
      virtual long GetDeviceClass( void ) const = 0;
   public:
      const long        m_nDeviceId;
	  //TAR 211913 BZ: move it here to access binary conversion property
      CPrinterWrapper&  m_rPtr;		
   private:
      //CPrinterWrapper&  m_rPtr;	//TAR 211913 BZ
	   const long        m_nStation;
   private:
      friend long CPrintTextRequest::Process(CPrintStation& rStation ) const;
      friend long CPrintBarcodeRequest::Process(CPrintStation& rStation ) const;
      friend long CPrintBitmapRequest::Process(CPrintStation& rStation ) const;
      friend long CCutPaperRequest::Process(CPrintStation& rStation ) const;
      friend long CChangePrintSideRequest::Process(CPrintStation& rStation ) const; // RFQ 7477
      friend long CSetRecLineCharRequest::Process(CPrintStation& rStation) const;
      friend class CTestingPrinter;
   };


class CJournalStation : public CPrintStation
   {
   public:
      CJournalStation( CPrinterWrapper& rPtr, long nDeviceId )
         : CPrintStation( rPtr, nDeviceId, PTR_S_JOURNAL ) {}
   public:
      virtual long GetLineWidth( void ) const 
         { return GetPrinter().GetJrnLineWidth(); }
	  virtual long SetBitmap( long nNumber, long nStation, LPCTSTR sFileName, long nWidth, long nAlignment)
         { return OPOS_E_ILLEGAL; }
      virtual long GetLineHeight( void ) const 
         {  return GetPrinter().GetJrnLineHeight(); }
      virtual long GetLineChars( void ) const 
         {  return GetPrinter().GetJrnLineChars(); }
      virtual bool GetPaperEmpty( void ) const 
         { return !!GetPrinter().GetJrnEmpty(); }
      virtual bool GetPaperNearEmpty( void ) const 
         { return !!GetPrinter().GetJrnNearEnd(); }
	  virtual void SetLineChars( int lineChars)  const
      {    GetPrinter().SetJrnLineChars(lineChars); }     //tar 266885

   protected:
      virtual long OnPrintBarcode( const CString& sName, long nType, long nHeight, 
         long nWidth, long nAlign, long nTextPosition ) const  
         { return OPOS_E_ILLEGAL; }
      virtual long OnPrintBitmap( const CString& sData, long nWidth, 
         long nAlign ) const
         { return OPOS_E_ILLEGAL; }
      virtual long OnCutPaper( long nPercentage ) const
         { return OPOS_E_ILLEGAL; }
      virtual long GetDeviceClass( void ) const { return DMCLASS_JOURNAL; }
   };


class CReceiptStation : public CPrintStation
   {
   public:
      CReceiptStation( CPrinterWrapper& rPtr, long nDeviceId )
         : CPrintStation( rPtr, nDeviceId, PTR_S_RECEIPT ) {}
   public:
      virtual long GetLineWidth( void ) const 
         { return GetPrinter().GetRecLineWidth(); }
      virtual long GetLineHeight( void ) const 
         {  return GetPrinter().GetRecLineHeight(); }
      virtual long GetLineChars( void ) const 
         {  return GetPrinter().GetRecLineChars(); }
      virtual bool GetPaperEmpty( void ) const 
         { return !!GetPrinter().GetRecEmpty(); }
      virtual bool GetPaperNearEmpty( void ) const 
         { return !!GetPrinter().GetRecNearEnd(); }
   protected:
      virtual long GetDeviceClass( void ) const { return DMCLASS_RECEIPT; }
   protected:
   };


class CSlipStation : public CPrintStation
   {
   public:
      CSlipStation( CPrinterWrapper& rPtr, long nDeviceId )
         : CPrintStation( rPtr, nDeviceId, PTR_S_SLIP ),
         m_bOkToFireSlipInserted(false),
         m_bOkToFireSlipRemoved(false) {}
   public:
      virtual long GetLineWidth( void ) const 
         { return GetPrinter().GetSlpLineWidth(); }
      virtual long GetLineHeight( void ) const 
         {  return GetPrinter().GetSlpLineHeight(); }
      virtual long GetLineChars( void ) const 
         {  return GetPrinter().GetSlpLineChars(); }
      virtual bool GetPaperEmpty( void ) const 
         { return !!GetPrinter().GetSlpEmpty(); }
      virtual bool GetPaperNearEmpty( void ) const 
         { return !!GetPrinter().GetSlpNearEnd(); }
      virtual long BeginInsertion( long lTimeout );
      virtual long BeginRemoval( long lTimeout );
      virtual long BeginRemovalForced( long lTimeout ); // RFQ 7477
      virtual long EndInsertion( void )
         { return GetPrinter().EndInsertion( );}
      virtual long EndRemoval( void )
         { return GetPrinter().EndRemoval( );}
      virtual void FireStatusEvent( long nStatus );
	  virtual void SetLineChars( int lineChars)  const
      {   GetPrinter().SetSlpLineChars(lineChars); }     //tar 266885
   protected:
      virtual long GetDeviceClass( void ) const { return DMCLASS_SLIP; }
   private:
      bool m_bOkToFireSlipInserted;
      bool m_bOkToFireSlipRemoved;
   };

class CPrintMapElement
   {
   private:
      CPrintMapElement( long nStation, long nOutputId, 
         TClassPtr<CPrintRequest> pRequest )
         : m_nStation( nStation ), m_nOutputId( nOutputId ), 
         m_pRequest( pRequest ) { }
      long Process( CPrintStation& rStation ) const
         { return m_pRequest->Process( rStation ); }
   private:
      const long m_nOutputId;
      const long m_nStation;
      TClassPtr<CPrintRequest> m_pRequest;
   private:
      friend class CPrinterWrapper;
   };

#endif // __PRINTER_H__
