///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCartAudit.CPP
//          Manages the mobile cart audit process
//
// AUTHOR:  TDonnan
// 
/////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

#include "stdafx.h"
#include "Common.h"
#include "TraceObject.h"
#include "MobileCartAudit.h"
#include "MobileCartAuditParser.h"
#include "MobileAuditScreenBuilder.h"
#include "MobileCartAuditDefs.h"
#include "FLTCPDefines.h"
#include "TCPConnectConstants.h"
#include "TCPConnectConstantsSSF.h"
#include "MobileConstantsSSF.h"
#include "SMState.h"

//#include "PSProcedures.h"

CMobileAuditScreenBuilder screenBuilder;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCartAudit")

static const UINT   nIntSizeLength  = 20;
static const int    nLeftMaskCount  = 4;
static const int    nRightMaskCount = 4;

// DLLIMPORT extern TraceObject     to; 
// DLLIMPORT extern CTCPWebConnect &tcpConnect;
// IMPLEMENT_DYNCREATE(CMobileCartAudit, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
/// Constructor
/**
Constructor for CMobileCartAudit object
*/
CMobileCartAudit::CMobileCartAudit()
{
    m_MobileAuditItemsQueue = new CQueuedActionsContainer();

    Reset();
}

///////////////////////////////////////////////////////////////////////////////////
/// Destructor
/**
Destructor for CMobileCartAudit object
*/
CMobileCartAudit::~CMobileCartAudit()
{
    
}

///////////////////////////////////////////////////////////////////////////////////
///  UnInitialize
/**
Uninitialize the object and free all resources.
*/
void CMobileCartAudit::UnInitialize()
{
    m_MobileAuditItemsQueue->Clear();
    delete m_MobileAuditItemsQueue;
    // CMobileCartAuditBase::UnInitialize();
}

///////////////////////////////////////////////////////////////////////////////////
///  Initialize
/**
This method is called by the application to initialize this object.
*/
BOOL CMobileCartAudit::Initialize( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::Initialize()"));

    CMobileCartAuditParser  oParser;

    Reset();

    BOOL bSuccess = oParser.ProcessTicketDocument(
        a_bstrtXml,
        m_bstrtStoreNumber,
        m_bstrtTerminalNumber,
        m_bstrtLanguageId,
        m_bstrtCartId,
        m_bstrtLoyaltyId );

    trace(L6, _T("-CMobileCartAudit::Initialize() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
    //return CMobileCartAuditBase::Initialize(csStoreNum, nTerminalNum);
}

///////////////////////////////////////////////////////////////////////////////////
///  Reset
/**
This method is called to reset object data.
*/
void CMobileCartAudit::Reset()
{
    m_bstrtStoreNumber              = szEmpty;
    m_bstrtTerminalNumber           = szZero;
    m_bstrtLanguageId               = szDefaultLanguageId;
    m_bstrtCartId                   = szEmpty;
    m_bstrtLoyaltyId                = szEmpty;
    m_bstrtSessionId                = szEmpty;
    m_bstrtResult                   = szEmpty;
    m_bstrtResultMessage            = szEmpty;
    m_bstrtRescanType               = szEmpty;
    m_bstrtRescanStatus             = szEmpty;

    m_bstrtCustomerName             = szDefaultCustomerName;
    m_bstrtNumberOfVisits           = szZero;
    m_bstrtFirstVisit               = szFalse;
    m_bstrtCardNumber               = szEmpty;

    m_bstrtNumberOfItemsInTicket    = szEmpty;
    m_bstrtTotalTicketAmount        = szEmpty;
    m_bstrtNumItemsToRescan         = szEmpty;
    m_bstrtAmountToRescan           = szEmpty;
    m_bstrtRescanSkipAllowed        = szEmpty;
    m_bstrtRescanAbortAllowed       = szEmpty;

    m_bstrtItemId                   = szEmpty;
    m_bstrtItemQuantity             = szEmpty;
    m_bstrtItemBarcode              = szEmpty;
    m_bstrtItemDescription          = szEmpty;
    m_bstrtItemPrice                = szEmpty;
    m_bstrtRemainingItems           = szEmpty;
    m_bstrtRemainingAmount          = szEmpty;
    m_bstrtNumItemsScanned          = szEmpty;  
    m_bstrtScannedItemsAmount       = szEmpty;

    m_bstrDataNeededSection         = szEmpty;
    m_bstrDataNeededViewString      = szEmpty;
    m_bUnknownItem                  = FALSE;
    m_MobileAuditItemsQueue->Clear();
}

///////////////////////////////////////////////////////////////////////////////////
///  TraceObjectState
/**
This method is called by the application to build a trace string for this object's
current state.
*/
_bstr_t CMobileCartAudit::TraceObjectState()
{
    CString csTraceLine = _T("MobileCartAudit state:\n");
    
    csTraceLine = csTraceLine + _T("m_bstrtStoreNumber           = ") + (LPCTSTR)m_bstrtStoreNumber + _T("\r\n");     
    csTraceLine = csTraceLine + _T("m_bstrtTerminalNumber        = ") + (LPCTSTR)m_bstrtTerminalNumber + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrtLanguageId            = ") + (LPCTSTR)m_bstrtLanguageId + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrtCartId                = ") + (LPCTSTR)m_bstrtCartId + _T("\r\n");

    #if defined ( _DEBUG )
        csTraceLine = csTraceLine + _T("m_bstrtLoyaltyId            = ") + (LPCTSTR)m_bstrtLoyaltyId + _T("\r\n");
    #else
        csTraceLine = csTraceLine + _T("m_bstrtLoyaltyId            = ") + (LPCTSTR)GetMaskedLoyaltyId() + _T("\r\n");
    #endif

    csTraceLine = csTraceLine + _T("m_bstrtLoyaltyId             = ") + (LPCTSTR)m_bstrtLoyaltyId + _T("\r\n");             
    csTraceLine = csTraceLine + _T("m_bstrtSessionId             = ") + (LPCTSTR)m_bstrtSessionId + _T("\r\n");             
    csTraceLine = csTraceLine + _T("m_bstrtResult                = ") + (LPCTSTR)m_bstrtResult + _T("\r\n");                
    csTraceLine = csTraceLine + _T("m_bstrtResultMessage         = ") + (LPCTSTR)m_bstrtResultMessage + _T("\r\n");        
    csTraceLine = csTraceLine + _T("m_bstrtRescanType            = ") + (LPCTSTR)m_bstrtRescanType + _T("\r\n");           
    csTraceLine = csTraceLine + _T("m_bstrtRescanStatus          = ") + (LPCTSTR)m_bstrtRescanStatus + _T("\r\n");          
    csTraceLine = csTraceLine + _T("m_bstrtCustomerName          = ") + (LPCTSTR)m_bstrtCustomerName + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrtNumberOfVisits        = ") + (LPCTSTR)m_bstrtNumberOfVisits + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrtFirstVisit            = ") + (LPCTSTR)m_bstrtFirstVisit + _T("\r\n");

    #if defined ( _DEBUG )
        csTraceLine = csTraceLine + _T("m_bstrtCardNumber           = ") + (LPCTSTR)m_bstrtCardNumber + _T("\r\n");
    #else
        csTraceLine = csTraceLine + _T("m_bstrtCardNumber           = ") + (LPCTSTR)GetMaskedCardNumber() + _T("\r\n");
    #endif

    csTraceLine = csTraceLine + _T("m_bstrtNumberOfItemsInTicket = ") + (LPCTSTR)m_bstrtNumberOfItemsInTicket + _T("\r\n"); 
    csTraceLine = csTraceLine + _T("m_bstrtTotalTicketAmount     = ") + (LPCTSTR)m_bstrtTotalTicketAmount + _T("\r\n");     
    csTraceLine = csTraceLine + _T("m_bstrtNumItemsToRescan      = ") + (LPCTSTR)m_bstrtNumItemsToRescan + _T("\r\n");      
    csTraceLine = csTraceLine + _T("m_bstrtAmountToRescan        = ") + (LPCTSTR)m_bstrtAmountToRescan + _T("\r\n");       
    csTraceLine = csTraceLine + _T("m_bstrtRescanSkipAllowed     = ") + (LPCTSTR)m_bstrtRescanSkipAllowed + _T("\r\n");     
    csTraceLine = csTraceLine + _T("m_bstrtRescanAbortAllowed    = ") + (LPCTSTR)m_bstrtRescanAbortAllowed + _T("\r\n");    
    csTraceLine = csTraceLine + _T("m_bstrtItemId                = ") + (LPCTSTR)m_bstrtItemId + _T("\r\n");                
    csTraceLine = csTraceLine + _T("m_bstrtItemQuantity          = ") + (LPCTSTR)m_bstrtItemQuantity + _T("\r\n");          
    csTraceLine = csTraceLine + _T("m_bstrtItemBarcode           = ") + (LPCTSTR)m_bstrtItemBarcode + _T("\r\n");           
    csTraceLine = csTraceLine + _T("m_bstrtItemDescription       = ") + (LPCTSTR)m_bstrtItemDescription + _T("\r\n");      
    csTraceLine = csTraceLine + _T("m_bstrtItemPrice             = ") + (LPCTSTR)m_bstrtItemPrice + _T("\r\n");             
    csTraceLine = csTraceLine + _T("m_bstrtRemainingItems        = ") + (LPCTSTR)m_bstrtRemainingItems + _T("\r\n");        
    csTraceLine = csTraceLine + _T("m_bstrtRemainingAmount       = ") + (LPCTSTR)m_bstrtRemainingAmount + _T("\r\n");       
    csTraceLine = csTraceLine + _T("m_bstrtNumItemsScanned       = ") + (LPCTSTR)m_bstrtNumItemsScanned + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrtScannedItemsAmount    = ") + (LPCTSTR)m_bstrtScannedItemsAmount + _T("\r\n");
    csTraceLine = csTraceLine + _T("m_bstrDataNeededSection      = ") + (LPCTSTR)m_bstrDataNeededSection + _T("\r\n");      
    csTraceLine = csTraceLine + _T("m_bstrDataNeededViewString   = ") + (LPCTSTR)m_bstrDataNeededViewString + _T("\r\n"); 
    
    return (_bstr_t) csTraceLine;
}

///////////////////////////////////////////////////////////////////////////////////
///  MaskValue
/**
class helper - returns mask copy of passed in parameter.
*/
_bstr_t CMobileCartAudit::MaskValue( const _bstr_t & a_bstrtValue ) const
{
    _bstr_t bstrtMaskValue( a_bstrtValue );

    UINT    nValueLen = SysStringLen( a_bstrtValue );

    if ( nValueLen > nLeftMaskCount && 
         nValueLen > nRightMaskCount )
    {
        CString csMaskedValue( (LPCTSTR)a_bstrtValue );

        for ( int i = 0; i < nValueLen; i++ )
        {
            if ( i < nLeftMaskCount || i > (nValueLen - nRightMaskCount - 1) )
            {
                csMaskedValue.SetAt(i,_T('*'));
            }
        }

        bstrtMaskValue = csMaskedValue;
    }
    return bstrtMaskValue;
}

///////////////////////////////////////////////////////////////////////////////////
///  GetMaskedLoyaltyId
/**
returns the masked LoyaltyId
*/
_bstr_t CMobileCartAudit::GetMaskedLoyaltyId() const
{
    return MaskValue( m_bstrtLoyaltyId );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetMaskedCardNumber
/**
returns the masked card number
*/
_bstr_t CMobileCartAudit::GetMaskedCardNumber() const
{
    return MaskValue( m_bstrtCardNumber );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetFirstVisit
/**
returns the first visit flag as a BOOL
*/
BOOL CMobileCartAudit::GetFirstVisit() const
{
    BOOL bIsFirstVisit = FALSE;

    if (_tcsicmp( m_bstrtFirstVisit, szTrue ) == 0 )
    {
        bIsFirstVisit = TRUE;
    }
    return bIsFirstVisit;
}

///////////////////////////////////////////////////////////////////////////////////
///  GetFirstVisit
/**
returns the number of visits as a LONG
*/
LONG CMobileCartAudit::GetNumberOfVisits() const
{
    LONG lValue = 0;

    if ( SysStringLen( m_bstrtNumberOfVisits ) > 0 )
    {
        lValue = _ttol( m_bstrtNumberOfVisits );
    }
    return lValue;
}

///////////////////////////////////////////////////////////////////////////////////
///  BstrMoneyToLong
/**
Class helper method used to convert bstrt repesenting money to a long.
*/
LONG CMobileCartAudit::BstrMoneyToLong( const _bstr_t & a_bstrValue ) const
{
    LONG lValue = 0;

    if ( SysStringLen( m_bstrtRemainingAmount ) > 0 )
    {
        CString csValue((TCHAR*)a_bstrValue);

        csValue.Remove(_T('.'));
        
        lValue = _ttol( csValue );
    }
    return lValue;
}

///////////////////////////////////////////////////////////////////////////////////
///  GetTotalTicketAmount
/**
returns the total ticket amount to rescan as a long.
*/
LONG CMobileCartAudit::GetTotalTicketAmount() const
{
    return BstrMoneyToLong( m_bstrtTotalTicketAmount );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetItemPrice
/**
returns the total ticket amount to rescan as a long.
*/
LONG CMobileCartAudit::GetItemPrice() const
{
    return BstrMoneyToLong( m_bstrtItemPrice );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetAmountToRescan
/**
returns the amount to rescan a long.
*/
LONG CMobileCartAudit::GetAmountToRescan() const
{
    return BstrMoneyToLong( m_bstrtAmountToRescan );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetRemainingAmount
/**
returns the scanned items amount as a long.
*/
LONG    CMobileCartAudit::GetScannedItemsAmount() const
{
    return BstrMoneyToLong( m_bstrtScannedItemsAmount );
}

///////////////////////////////////////////////////////////////////////////////////
///  GetRemainingAmount
/**
returns tne remaining amount as a long.
*/
LONG CMobileCartAudit::GetRemainingAmount() const
{
    return BstrMoneyToLong( m_bstrtRemainingAmount );
}

///////////////////////////////////////////////////////////////////////////////////
///  SendStartAuditRequest
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
BOOL CMobileCartAudit::SendStartAuditRequest()
{
    trace(L6, _T("+CMobileCartAudit::SendStartAuditRequest()"));

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    /*
    TCHAR szStartAuditRequest[] = 
        _T("<StartAuditRequest>")\
        _T("<ApplicationId></ApplicationId>")\
        _T("<LanguageCode>EN</LanguageCode>")\
        _T("<StoreNumber>1</StoreNumber>")\
        _T("<TerminalNumber>1</TerminalNumber>")\
        _T("<TransactionNumber>123</TransactionNumber>")\
        _T("<CustomerCardNumber>1234567890</CustomerCardNumber>")\
        _T("</StartAuditRequest>");
    */
    m_bstrDataNeededSection         = szEmpty;
    m_bstrDataNeededViewString      = szEmpty;

    bstrtXml = oParser.CreateStartAuditRequest(
        szUnknown,
        m_bstrtLanguageId,
        m_bstrtStoreNumber,
        m_bstrtTerminalNumber,
        m_bstrtCartId,
        m_bstrtLoyaltyId);

    // char * szTest = bstrtXml;

    long len = SysStringLen(bstrtXml);

    int nSizeNeeded = WideCharToMultiByte(CP_UTF8, 0, bstrtXml, len, NULL, 0, NULL, NULL );

    char *szUTF8 = new char [nSizeNeeded + 1];

    memset(szUTF8, 0, nSizeNeeded + 1);

    WideCharToMultiByte(CP_UTF8, 0, bstrtXml, len, szUTF8, nSizeNeeded, NULL, NULL );
    
    if ( len > 0 )
    {
        /*
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_START_AUDIT, 
            (BYTE*)(char*) bstrtXml ) == FALSE;
        */
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_START_AUDIT, 
            (BYTE*) szUTF8 ) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendStartAuditRequest() - failed to create XML"));
    }

    delete [] szUTF8;

    trace(L6, _T("-CMobileCartAudit::SendStartAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendExitAuditRequest
/**
This method is called by the application to send an exit audit request to 
FLService.
*/
BOOL CMobileCartAudit::SendExitAuditRequest()
{
    BOOL bSuccess = FALSE;

    bSuccess = SendEndAuditRequest( szActionSSCOAbortAudit);

    //Just setup screen data and exit for now
    screenBuilder.GetExitAuditScreenData(szEmpty, 
                                         szEmpty,
                                         m_bstrDataNeededSection, 
                                         m_bstrDataNeededViewString);
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessStartAuditResponse
/**
This method is called by the application to process the Start Audit response
from FLService.
*/
BOOL CMobileCartAudit::ProcessStartAuditResponse( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::ProcessStartAuditResponse()"));

    CMobileCartAuditParser  oParser;

    BOOL bSuccess = oParser.ProcessStartAuditResponse(
        a_bstrtXml,
        m_bstrtSessionId,
        m_bstrtResult,
        m_bstrtResultMessage,
        m_bstrtRescanType,
        m_bstrtRescanStatus,
        m_bstrtCustomerName,
        m_bstrtNumberOfVisits,
        m_bstrtFirstVisit,    
        m_bstrtCardNumber,    
        m_bstrtNumberOfItemsInTicket,
        m_bstrtTotalTicketAmount,
        m_bstrtNumItemsToRescan,
        m_bstrtAmountToRescan,
        m_bstrtRescanSkipAllowed,
        m_bstrtRescanAbortAllowed );

    m_bstrtRemainingItems = m_bstrtNumItemsToRescan;
    m_bstrtRemainingAmount = m_bstrtAmountToRescan;

    screenBuilder.GetPartialAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);

    trace(L6, _T("-CMobileCartAudit::ProcessStartAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure  );

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendAddAuditItemRequest
/**
This method is called by the application to send a rescan audit item request to 
FLService.
*/
BOOL CMobileCartAudit::SendAddAuditItemRequest( int a_nQuantity, const _bstr_t & a_bstrtBarcode )
{
    trace(L6, _T("+CMobileCartAudit::SendAddAuditItemRequest()"));

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    m_bstrtResult           = szEmpty;
    m_bstrtResultMessage    = szEmpty;
    m_bstrtItemId           = szEmpty;
    m_bstrtItemQuantity     = szEmpty;
    m_bstrtItemDescription  = szEmpty;
    m_bstrtItemPrice        = szEmpty;

    bstrtXml = oParser.CreateAddAuditItemRequest( a_nQuantity, a_bstrtBarcode );

    long len = SysStringLen(bstrtXml);
    
    if ( len > 0 )
    {
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_ADD_AUDIT_ITEM_REQUEST, 
            (BYTE*) (char*) bstrtXml ) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendAddAuditItemRequest() - failed to create XML"));
    }

    trace(L6, _T("-CMobileCartAudit::SendAddAuditItemRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendAddAuditItemRequest
/**
This method is called by the application to send a rescan audit item request to 
FLService.
*/
BOOL CMobileCartAudit::SendAddAuditItemRequest( const _bstr_t & a_bstrtScanInfo )
{
    trace(L6, _T("+CMobileCartAudit::SendAddAuditItemRequest()"));

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    m_bstrtResult           = szEmpty;
    m_bstrtResultMessage    = szEmpty;
    m_bstrtItemId           = szEmpty;
    m_bstrtItemQuantity     = szEmpty;
    m_bstrtItemDescription  = szEmpty;
    m_bstrtItemPrice        = szEmpty;

    m_bstrLastScanInfo = a_bstrtScanInfo;

    bstrtXml = oParser.CreateAddAuditItemRequest( a_bstrtScanInfo );

    long len = SysStringLen(bstrtXml);
    
    if ( len > 0 )
    {
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_ADD_AUDIT_ITEM_REQUEST, 
            (BYTE*) (char*) bstrtXml ) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendAddAuditItemRequest() - failed to create XML"));
    }

    trace(L6, _T("-CMobileCartAudit::SendAddAuditItemRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessAddAuditItemResponse
/**
This method is called by the application to process the audit item response
returned by FLService.
*/
BOOL CMobileCartAudit::ProcessAddAuditItemResponse( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::ProcessAddAuditItemResponse()"));

    CMobileCartAuditParser  oParser;

    BOOL bSuccess = oParser.ProcessAddAuditItemResponse(
        a_bstrtXml,
        m_bstrtResult,
        m_bstrtResultMessage,
        m_bstrtRescanType,
        m_bstrtRescanStatus,
        m_bstrtItemId,
        m_bstrtItemQuantity,
        m_bstrtItemBarcode,
        m_bstrtItemDescription,
        m_bstrtItemPrice,
        m_bstrtRemainingItems,
        m_bstrtRemainingAmount,
        m_bstrtNumItemsScanned,
        m_bstrtScannedItemsAmount );

    if (_tcsicmp((LPCTSTR) m_bstrtResult, szFailure ) == 0 )
    {

        // if item is unknown (not found) to server
        //
        m_bUnknownItem = FALSE;
        if (_tcsicmp((LPCTSTR) m_bstrtResultMessage, szItemNotFound) == 0 )
        {
            m_bUnknownItem = TRUE;
        }

        if (_tcsicmp((LPCTSTR) m_bstrtRescanStatus,  szFullRescanAfterError) == 0)
        {
            screenBuilder.GetFullAuditErrorScreenData(
                m_bstrtRescanStatus, 
                m_bstrtResult, 
                m_bstrDataNeededSection, 
                m_bstrDataNeededViewString);
        }
        else
        {
            screenBuilder.GetPartialAuditErrorScreenData(
                m_bstrtRescanStatus, 
                m_bstrtResult, 
                m_bstrDataNeededSection, 
                m_bstrDataNeededViewString);  
            if (!m_bUnknownItem)
                QueueFailedAuditItem();
        }
    }
    else
    {
        
        screenBuilder.GetPartialAuditScreenData(
            m_bstrtRescanStatus, 
            m_bstrtResult, 
            m_bstrDataNeededSection, 
            m_bstrDataNeededViewString);
    }

    trace(L6, _T("-CMobileCartAudit::ProcessAddAuditItemResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendEndAuditRequest
/**
This method is called by the application to send a rescan audit item request to 
FLService. 
where action can be: <Action>Send/Cancel/Skip/Abort/ErrorExit</Action>
*/
BOOL CMobileCartAudit::SendEndAuditRequest( const _bstr_t & a_bstrtAction )
{
    trace(L6, _T("+CMobileCartAudit::SendEndAuditRequest(%s)"), (TCHAR*)a_bstrtAction );

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    m_bstrtResult           = szEmpty;
    m_bstrtResultMessage    = szEmpty;

    bstrtXml = oParser.CreateEndAuditRequest( a_bstrtAction );

    long len = SysStringLen(bstrtXml);
    
    if ( len > 0 )
    {
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 

            TCP_MOBILECARTMSG_AUDIT_END, 
            (BYTE*) (char*) bstrtXml ) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendEndAuditRequest() - failed to create XML"));
    }

    trace(L6, _T("-CMobileCartAudit::SendEndAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessEndAuditResponse
/**
This method is called by the application to process the end audit response returned
by FLService.
*/
BOOL CMobileCartAudit::ProcessEndAuditResponse( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::ProcessEndAuditResponse()") );

    CMobileCartAuditParser  oParser;

    BOOL bSuccess = oParser.ProcessEndAuditResponse(
        a_bstrtXml,
        m_bstrtResult,
        m_bstrtResultMessage,
        m_bstrtRescanType,
        m_bstrtRescanStatus );

    screenBuilder.GetEndAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);

    trace(L6, _T("-CMobileCartAudit::ProcessEndAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);


    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendSwitchToFullAuditRequest
/**
This method is called by the application to send a switch from partial to full audit
request to FLService.
*/
BOOL CMobileCartAudit::SendSwitchToFullAuditRequest( const _bstr_t & a_bstrtOnErrorBool )
{
    trace(L6, _T("+CMobileCartAudit::SendSwitchToFullAuditRequest()"));

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    bstrtXml = oParser.CreateSwitchToFullAuditRequest( a_bstrtOnErrorBool );

    long len = SysStringLen(bstrtXml);

    if ( len > 0 )
    {
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_SWITCH_TO_FULL_AUDIT, 
            (BYTE*)(char*) bstrtXml) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendSwitchToFullAuditRequest() - failed to create XML"));
    }

    trace(L6, _T("-CMobileCartAudit::SendSwitchToFullAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessSwitchToFullAuditResponse
/**
This method is called by the application to process the switch to full audit response
from FLService
*/
BOOL CMobileCartAudit::ProcessSwitchToFullAuditResponse( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::ProcessSwitchToFullAuditResponse()") );

    CMobileCartAuditParser  oParser;

    BOOL bSuccess = oParser.ProcessSwitchToFullAuditResponse(
        a_bstrtXml,
        m_bstrtResult,
        m_bstrtResultMessage,
        // m_bstrtRescanType,
        m_bstrtRescanStatus );

    trace(L6, _T("-CMobileCartAudit::ProcessSwitchToFullAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    screenBuilder.GetFullAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  SendContinueAfterErrorRequest
/**
This method is called by the application to send a continue after scan error request
to FLService
*/
BOOL CMobileCartAudit::SendContinueAfterErrorRequest( const _bstr_t & a_bstrtBarcode )
{
    trace(L6, _T("+CMobileCartAudit::SendContinueAfterErrorRequest()") );

    BOOL                    bSuccess = FALSE;
    CMobileCartAuditParser  oParser;
    bstr_t                  bstrtXml;

    m_bstrtResult           = szEmpty;
    m_bstrtResultMessage    = szEmpty;
    m_bstrtRescanType       = szEmpty;
    m_bstrtRescanStatus     = szEmpty;

    bstrtXml = oParser.CreateContinueAfterErrorRequest( a_bstrtBarcode );

    long len = SysStringLen(bstrtXml);
    
    if ( len > 0 )
    {
        bSuccess = tcpConnect.SendBytes(
            TCP_CONNECTION_1, 
            len + 1, 
            MSG_FORMAT_XML, 
            MSG_FMT1, 
            1, 
            TCP_MOBILECARTMSG_CONTINUE_AFTER_ERROR, 
            (BYTE*) (char*) bstrtXml ) == FALSE;
    }
    else
    {
        trace(L6, _T("   CMobileCartAudit::SendContinueAfterErrorRequest() - failed to create XML"));
    }

    trace(L6, _T("-CMobileCartAudit::SendContinueAfterErrorRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  ContinueAfterFullRescanError
/**
This method is called by the application to restore the full rescan screen after
an error on an item scan
*/
BOOL CMobileCartAudit::SendContinueAfterFullRescanError(void)
{
    trace(L6, _T("+CMobileCartAudit::SendContinueAfterFullRescanError()") );

    BOOL bSuccess = TRUE;

    screenBuilder.GetFullAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);

    return bSuccess;

}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessContinueAfterErrorResponse
/**
This method is called by the application to process the ContinueAfterError response
from FLService.
*/
BOOL CMobileCartAudit::ProcessContinueAfterErrorResponse( const _bstr_t & a_bstrtXml )
{
    trace(L6, _T("+CMobileCartAudit::ProcessContinueAfterErrorResponse()") );

    CMobileCartAuditParser  oParser;

    BOOL bSuccess = oParser.ProcessContinueAfterErrorResponse(
        a_bstrtXml,
        m_bstrtResult,
        m_bstrtResultMessage,
        m_bstrtRescanType,
        m_bstrtRescanStatus );

    //    
    if ( m_bUnknownItem == FALSE && _tcsicmp( szSuccess, m_bstrtResult ) == 0 )
    {
        int nRemainingItems = _ttoi( m_bstrtRemainingItems );

        if ( nRemainingItems > 0 )
        {
            TCHAR string[nIntSizeLength];

            nRemainingItems--;

            m_bstrtRemainingItems = _itot(nRemainingItems, string, 10 );  
        }
    }

    m_bUnknownItem = FALSE;

    screenBuilder.GetPartialAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);
    
    trace(L6, _T("-CMobileCartAudit::ProcessContinueAfterErrorResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);
    
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  RequestAbortAudit
/**
This method is called by the application to abort the audit.
*/
BOOL CMobileCartAudit::RequestAbortAudit()
{
    trace(L6, _T("+CMobileCartAudit::RequestAbortAudit()") );
    BOOL bSuccess = TRUE;

    screenBuilder.GetAbortAuditSureScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);

    trace(L6, _T("-CMobileCartAudit::RequestAbortAudit() returning %d"), bSuccess );
    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  RequestAbortAudit
/**
This method is called by the application to return to the partial audit screen.
*/
BOOL CMobileCartAudit::ReturnToPartialAuditScreen()
{
    BOOL bSuccess = TRUE;

    screenBuilder.GetPartialAuditScreenData(
        m_bstrtRescanStatus, 
        m_bstrtResult, 
        m_bstrDataNeededSection, 
        m_bstrDataNeededViewString);

    return bSuccess;
}

void CMobileCartAudit::QueueFailedAuditItem()
{
    trace(L6, _T("+CMobileCartAudit::QueueFailedAuditItem()"));

    int nQueuedItems = m_MobileAuditItemsQueue->GetSize();
    CString csID;
    csID.Format(_T("AuditFailure_%d"), nQueuedItems+1);
    long lBarcodeType;
    _bstr_t bstrScanData;
    _bstr_t bstrScanDataLabel;
    
    CMobileCartAuditParser  oParser;
    if (oParser.ProcessScanInfo(m_bstrLastScanInfo, bstrScanData, bstrScanDataLabel, lBarcodeType) == TRUE)
    {
        CQueuedActionRescanItem* pRescanErrorItem = 
            new CQueuedActionRescanItem(csID, (LPCTSTR)bstrScanDataLabel, 
                                       (LPCTSTR)bstrScanData,
                                       (LPCTSTR)m_bstrtItemBarcode,
                                       (LPCTSTR)m_bstrtResultMessage,
                                       lBarcodeType);
        pRescanErrorItem->SetRescanItem(MOBILE_RESCANITEM_POSONLY);
        pRescanErrorItem->SetOrigin(MOBILE_ORIGIN_TERMINAL_RESCAN);
        pRescanErrorItem->SetDescription((LPCTSTR)m_bstrtItemDescription);
        pRescanErrorItem->SetNetPrice(GetItemPrice());
        m_MobileAuditItemsQueue->AddReplace(pRescanErrorItem);
    }
    else
    {
        trace(L6, _T("CMobileCartAudit::QueueFailedAuditItem() failed to queue item"));
    }
    trace(L6, _T("CMobileCartAudit::QueueFailedAuditItem()"));
}

IQueuedAction* CMobileCartAudit::GetNextAuditFailureItem()
{
    trace(L6, _T("+CMobileCartAudit::GetNextAuditFailureItem()"));
    IQueuedAction* pItem = NULL;
    if (m_MobileAuditItemsQueue)
    {
        pItem = m_MobileAuditItemsQueue->GetNext();
    }
    trace(L6, _T("-CMobileCartAudit::GetNextAuditFailureItem()"));
    return pItem;
    
}

int CMobileCartAudit::GetNumberOfAuditFailureItems()
{
    trace(L6, _T("+CMobileCartAudit::GetNumberOfAuditFailureItems()"));
    int nReturn=0;
    if (m_MobileAuditItemsQueue)
    {
        nReturn = m_MobileAuditItemsQueue->GetSize();
    }
    trace(L6, _T("+CMobileCartAudit::GetNumberOfAuditFailureItems()"));
    return nReturn;
}

void CMobileCartAudit::ClearAuditFailureItems()
{
    trace(L6, _T("+CMobileCartAudit::ClearAuditFailureItems()"));
    int nReturn=0;
    if (m_MobileAuditItemsQueue)
    {
        m_MobileAuditItemsQueue->Clear();
    }
    trace(L6, _T("+CMobileCartAudit::ClearAuditFailureItems()")); 
}