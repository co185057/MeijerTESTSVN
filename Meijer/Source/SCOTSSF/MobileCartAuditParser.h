//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    MobileCartAuditParser.h
//
// TITLE:   Mobile cart audit session object
//
// AUTHOR:  TDonnan
//
//////////////////////////////////////////////////////////////////////////////////////////////////
// MobileCartAuditParser
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CMobileCartAuditParser
#define _CMobileCartAuditParser

//#include <atlbase.h>
//extern CComModule _Module;
//#include <atlcom.h>
//#include "comutil.h"

//#include "xmlHttpEvent.h"
// #include <GTOU.tlh>
//class CMobileCartAuditParserParser;
// #include "MobileCartAuditParserBase.h"

const static TCHAR szAuditSuccess[] = _T("Success");
const static TCHAR szAuditFailure[] = _T("Failure");

#ifdef _SCOTSSF_
class DLLEXPORT
#else
class DLLIMPORT
#endif
CMobileCartAuditParser  // : public CMobileCartAuditParserBase
{
public:

    CMobileCartAuditParser();

    ~CMobileCartAuditParser();

    virtual long Initialize();

    virtual void UnInitialize();

public:

    virtual BOOL ProcessTicketDocument( 
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtStoreNumber,
        _bstr_t & a_bstrtTerminalNumber,
        _bstr_t & a_bstrtLanguageId,
        _bstr_t & a_bstrtCartId,
        _bstr_t & a_bstrtLoyaltyId );

    virtual bstr_t CreateStartAuditRequest(
        _bstr_t a_bstrtApplicationId,
        _bstr_t a_bstrtLanguageCode,
        _bstr_t a_bstrtStoreNumber,
        _bstr_t a_bstrtTerminalNumber,
        _bstr_t a_bstrtCartNumber,
        _bstr_t a_bstrtLoyaltyId);

    virtual BOOL ProcessStartAuditResponse( 
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtSessionId,
        _bstr_t & a_bstrtResult,
        _bstr_t & a_bstrtResultMessage,
        _bstr_t & a_bstrtRescanType,
        _bstr_t & a_bstrtRescanStatus,
        _bstr_t & a_bstrtCustomerName,
        _bstr_t & a_bstrtNumberOfVisits,
        _bstr_t & a_bstrtFirstVisit,
        _bstr_t & a_bstrtCardNumber,
        _bstr_t & a_bstrtNumberOfItemsInTicket,
        _bstr_t & a_bstrtTotalTicketAmount,
        _bstr_t & a_bstrtNumItemsToRescan,
        _bstr_t & a_bstrtAmountToResscan,
        _bstr_t & a_bstrtRescanSkipAllowed,
        _bstr_t & a_bstrtRescanAbortAllowed );

    virtual bstr_t CreateAddAuditItemRequest(
        int     a_nQuantity,
        _bstr_t a_bstrtBarcode );

    virtual bstr_t CreateAddAuditItemRequest(
        _bstr_t a_bstrtScanInfo );

    virtual BOOL ProcessAddAuditItemResponse( 
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtResult,
        _bstr_t & a_bstrtResultMessage,
        _bstr_t & a_bstrtRescanType,
        _bstr_t & a_bstrtRescanStatus,
        _bstr_t & a_bstrtId,
        _bstr_t & a_bstrtQuantity,
        _bstr_t & a_bstrtBarcode,
        _bstr_t & a_bstrtDescription,
        _bstr_t & a_bstrtPrice,
        _bstr_t & a_bstrtRemainingItems,
        _bstr_t & a_bstrtRemainingAmount,
        _bstr_t & a_bstrtNumItemsScanned,   
        _bstr_t & a_bstrtScannedItemsAmount );

    virtual bstr_t CreateEndAuditRequest(
        _bstr_t a_bstrtAction );

    virtual BOOL ProcessEndAuditResponse( 
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtResult,
        _bstr_t & a_bstrtResultMessage,
        _bstr_t & a_bstrtRescanType,
        _bstr_t & a_bstrtRescanStatus );

    virtual _bstr_t CreateSwitchToFullAuditRequest(
        _bstr_t a_bstrtOnError );

    virtual BOOL ProcessSwitchToFullAuditResponse(
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtResult,
        _bstr_t & a_bstrtResultMessage,
        _bstr_t & a_bstrtRescanStatus);

    virtual bstr_t CreateContinueAfterErrorRequest(
        _bstr_t a_bstrtBarcode );

    virtual BOOL ProcessContinueAfterErrorResponse( 
        _bstr_t a_bstrtXml,
        _bstr_t & a_bstrtResult,
        _bstr_t & a_bstrtResultMessage,
        _bstr_t & a_bstrtRescanType,
        _bstr_t & a_bstrtRescanStatus );

    virtual BOOL ProcessScanInfo(
        _bstr_t a_bstrtScanInfoXml,
        _bstr_t & a_bstrtScanData,
        _bstr_t & a_bstrtScanDataLabel,
        long & lBarcodeType );

    //DECLARE_DYNCREATE(CMobileCartAuditParser)// MFC Runtime class/object information
};

#endif

