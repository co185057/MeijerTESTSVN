///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCartAuditParser.CPP
//          Manages xml message parsing for the mobile cart audit process
//
// AUTHOR:  TDonnan
// 
/////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Common.h"
// #include "TraceObject.h"
#include "ScanInfo.h"
#include "MobileCartAuditParser.h"
#include "XmlSerializer.h"
#include "MobileCartAuditDefs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCartAuditParser")

//DLLIMPORT extern CTCPWebConnect &tcpConnect;

//IMPLEMENT_DYNCREATE(CMobileCartAuditParser, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
/// Constructor
/**  
Constructor for CMobileCartAuditParser object
*/
CMobileCartAuditParser::CMobileCartAuditParser()
{
}

/////////////////////////////////////////////////////////////////////////////////////
/// Destructor
/**  
Destructor for CMobileCartAuditParser object
*/
CMobileCartAuditParser::~CMobileCartAuditParser()
{
}

///////////////////////////////////////////////////////////////////////////////////
/// UnInitialize
/**
Uninitialize the object and free all resources.
*/
void CMobileCartAuditParser::UnInitialize()
{
    // CMobileCartAuditParserBase::UnInitialize();
}

///////////////////////////////////////////////////////////////////////////////////
/// Initialize
/**
Initialize the object and allocate all required resources.
   @param[out] retVal Error code (Always zero for now)
*/
long CMobileCartAuditParser::Initialize()
{
    //return CMobileCartAuditParserBase::Initialize(csStoreNum, nTerminalNum);
    return 0L;
}

///////////////////////////////////////////////////////////////////////////////////
///  CreateStartAuditRequest
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
bstr_t CMobileCartAuditParser::CreateStartAuditRequest(
    _bstr_t a_bstrtApplicationId,
    _bstr_t a_bstrtLanguageCode,
    _bstr_t a_bstrtStoreNumber,
    _bstr_t a_bstrtTerminalNumber,
    _bstr_t a_bstrtCartNumber,
    _bstr_t a_bstrtLoyaltyId )
{
    trace(L6, _T("+CMobileCartAuditParser::CreateStartAuditRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(szEmpty);
    BOOL            bSuccess = FALSE;

    //TCHAR szStartAuditRequest[] = 
    //    _T("<StartAuditRequest>")\
    //    _T("<ApplicationId></ApplicationId>")\
    //    _T("<LanguageCode>EN</LanguageCode>")\
    //    _T("<StoreNumber>1</StoreNumber>")\
    //    _T("<TerminalNumber>1</TerminalNumber>")\
    //    _T("<TransactionNumber>123</TransactionNumber>")\
    //    _T("<CustomerCardNumber>1234567890</CustomerCardNumber>")\
    //    _T("</StartAuditRequest>");

    if ( eXsSuccess == xs.Create() )
    {
        xs.Serialize( szStartAuditRequest );
        xs.Serialize( szApplicationId,      a_bstrtApplicationId );
        xs.Serialize( szLanguageCode,       a_bstrtLanguageCode);
        xs.Serialize( szStoreNumber,        a_bstrtStoreNumber);
        xs.Serialize( szTerminalNumber,     a_bstrtTerminalNumber);
        xs.Serialize( szTransactionNumber,  a_bstrtCartNumber );
        xs.Serialize( szCustomerCardNumber, a_bstrtLoyaltyId);

        bstrtXml = xs.GetXml();

        #ifdef _CPPUNIT
            xs.Save(_T("StartAuditRequest.xml"));
        #endif

        bSuccess = TRUE;
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateStartAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}


///////////////////////////////////////////////////////////////////////////////////
///  ProcessTicketDocument
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
BOOL CMobileCartAuditParser::ProcessTicketDocument( 
    _bstr_t a_bstrtXml,
    _bstr_t & a_bstrtStoreNumber,
    _bstr_t & a_bstrtTerminalNumber,
    _bstr_t & a_bstrtLanguageId,
    _bstr_t & a_bstrtCartId,
    _bstr_t & a_bstrtLoyaltyId )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessTicketDocument()"));

    CXmlSerializer  xs;
    BOOL bSuccess = FALSE;

    /*
    <Document xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
        <DocumentVersion>1</DocumentVersion>
        <Header>
            <Origin>0</Origin>
            <CreateDate>2018-02-09</CreateDate>
            <StoreNumber>001</StoreNumber>
            <ScannerWallNumber>0</ScannerWallNumber>
            <TransactionNumber>5262</TransactionNumber>
            <SelfscannerNumber>0</SelfscannerNumber>
            <CardNumber>8675309986753099</CardNumber>
            <CustomerLanguage>EN</CustomerLanguage>
            <PersonnelNumber />
            <Timestamp>20180209140105</Timestamp>
        </Header>
        <ItemGroup />
        <Footer>
            <Origin>0</Origin>
            <NumberOfItems>0</NumberOfItems>
            <VoidedItems>0</VoidedItems>
            <PosNumber>61</PosNumber>
            <Rescan>0</Rescan>
            <ErrorScans>0</ErrorScans>
            <TotalAmountSelfscan>0</TotalAmountSelfscan>
            <LoyaltyPointBalance>0</LoyaltyPointBalance>
            <LoyaltyPointChange>0</LoyaltyPointChange>
            <Timestamp>20180209140240</Timestamp>
            <RescanRequired>true</RescanRequired>
        </Footer>
    </Document>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szDocument) )
        {
            xs.Deserialize(szHeader);
            xs.Deserialize(szStoreNumber,           a_bstrtStoreNumber  );
            xs.Deserialize(szTransactionNumber,     a_bstrtCartId       );
            xs.Deserialize(szCardNumber,            a_bstrtLoyaltyId    );
            xs.Deserialize(szCustomerLanguage,      a_bstrtLanguageId   );
            xs.UnnestCurrentNode();
            xs.Deserialize(szFooter);
            xs.Deserialize(szPosNumber,            a_bstrtTerminalNumber);

            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessTicketDocument() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}



///////////////////////////////////////////////////////////////////////////////////
///  ProcessStartAuditResponse
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
BOOL CMobileCartAuditParser::ProcessStartAuditResponse( 
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
    _bstr_t & a_bstrtAmountToRescan,
    _bstr_t & a_bstrtRescanSkipAllowed,
    _bstr_t & a_bstrtRescanAbortAllowed )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessStartAuditResponse()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    /*
    <StartAuditResponse>
        <DataNeeded>MobileTransactionAuditStart</DataNeeded>
        <SessionId>SessionGuid</SessionId>
        <Result>Success/Failure</Result>
        <ResultMessage>Success or text that describes the error</ResultMessage>
        <RescanType>FULL/PARTIAL</RescanType>
        <RescanStatus>MinRequirementsMet/PartialScanContinued,FullRescanAfterError</RescanStatus>
        <CustomerName>John Doe</CustomerName>
        <NumberOfVisits>0</NumberOfVisits>
        <FirstVisit>false</FirstVisit>
        <CardNumber>8675309986753099</CardNumber>
        <NumberOfItemsInTicket>5</NumberOfItemsInTicket>
        <TotalTicketAmount>5.00</TotalTicketAmount>
        <NumItemsToRescan>3</NumItemsToRescan>
        <AmountToResscan>3.00</AmountToResscan>
        <RescanSkipAllowed>false</RescanSkipAllowed>
        <RescanAbortAllowed>false</RescanAbortAllowed>
    </StartAuditResponse>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szStartAuditResponse) )
        {
            xs.Deserialize(szSessionId,             a_bstrtSessionId                );
            xs.Deserialize(szResult,                a_bstrtResult                   );
            xs.Deserialize(szResultMessage,         a_bstrtResultMessage            );
            xs.Deserialize(szRescanType,            a_bstrtRescanType               );
            xs.Deserialize(szRescanStatus,          a_bstrtRescanStatus             );
            xs.Deserialize(szCustomerName,          a_bstrtCustomerName             );
            xs.Deserialize(szNumberOfVisits,        a_bstrtNumberOfVisits           );
            xs.Deserialize(szFirstVisit,            a_bstrtFirstVisit               );
            xs.Deserialize(szCardNumber,            a_bstrtCardNumber               );
            xs.Deserialize(szNumberOfItemsInTicket, a_bstrtNumberOfItemsInTicket    );
            xs.Deserialize(szTotalTicketAmount,     a_bstrtTotalTicketAmount        );
            xs.Deserialize(szNumItemsToRescan,      a_bstrtNumItemsToRescan         );
            xs.Deserialize(szAmountToRescan,        a_bstrtAmountToRescan          );
            xs.Deserialize(szRescanSkipAllowed,     a_bstrtRescanSkipAllowed        );
            xs.Deserialize(szRescanAbortAllowed,    a_bstrtRescanAbortAllowed       );
            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessStartAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  CreateAddAuditItemRequest
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
bstr_t CMobileCartAuditParser::CreateAddAuditItemRequest(
    int     a_nQuantity,
    _bstr_t a_bstrtBarcode )
{
    trace(L6, _T("+CMobileCartAuditParser::CreateAddAuditItemRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(szEmpty);
    BOOL            bSuccess = FALSE;

    //<?xml version="1.0"?>
    //<AddAuditItem>
    //    <Id></Id>
    //    <Quantity></Quantity>
    //    <Barcode></Barcode>
    //    <Weight></Weight>
    //</AddAuditItem>

    if ( eXsSuccess == xs.Create() )
    {
        xs.Serialize( szAddAuditItem );
        // xs.Serialize( szId,      a_bstrtId );
        xs.Serialize( szQuantity,   a_nQuantity);
        xs.Serialize( szBarcode,    a_bstrtBarcode);
        // xs.Serialize( szWeight,  a_bstrtWeight);

        bstrtXml = xs.GetXml();

        #ifdef _CPPUNIT
            xs.Save(_T("AddAuditItemRequest.xml"));
        #endif

        bSuccess = TRUE;
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateAddAuditItemRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}

///////////////////////////////////////////////////////////////////////////////////
///  CreateAddAuditItemRequest
/**
This method is called by the application to initiate an audit session with above
store server through FLServices
*/
bstr_t CMobileCartAuditParser::CreateAddAuditItemRequest(
    _bstr_t a_bstrtScanInfo )
{
    trace(L6, _T("+CMobileCartAuditParser::CreateAddAuditItemRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(szEmpty);
    BOOL            bSuccess = FALSE;

    CScanInfo oScanInfo;

    if ( oScanInfo.Deserialize( a_bstrtScanInfo ) )
    {
        //<?xml version="1.0"?>
        //<AddAuditItem>
        //    <Id></Id>
        //    <Quantity></Quantity>
        //    <Barcode></Barcode>
        //    <Weight></Weight>
        //</AddAuditItem>

        if ( eXsSuccess == xs.Create() )
        {
            xs.Serialize( szAddAuditItem );
            // xs.Serialize( szId,          a_bstrtId );
            // xs.Serialize( szQuantity,    oScanInfo.Quantity());
            xs.Serialize( szQuantity,       1);
            xs.Serialize( szBarcode,        oScanInfo.BarcodeLabel() );
            // xs.Serialize( szWeight,      a_bstrtWeight);

            bstrtXml = xs.GetXml();

            #ifdef _CPPUNIT
                xs.Save(_T("AddAuditItemRequest.xml"));
            #endif

            bSuccess = TRUE;
        }
    }
    else
    {
        trace(L6, _T("-CMobileCartAuditParser::CreateAddAuditItemRequest() - oScanInfo.Deserialize() failed") );
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateAddAuditItemRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}


///////////////////////////////////////////////////////////////////////////////////
///  ProcessAddAuditItemResponse
/**
This method is called by the application to process add audit item response
*/
BOOL CMobileCartAuditParser::ProcessAddAuditItemResponse( 
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
    _bstr_t & a_bstrtScannedItemsAmount )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessAddAuditItemResponse()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    /*
    <AddAuditItemResponse>
        <Result></Result>
        <ResultMessage></ResultMessage>
        <RescanType></RescanType>
        <RescanStatus></RescanStatus>
        <Id></Id>
        <Quantity></Quantity>
        <Barcode></Barcode>
        <Description></Description>
        <Price></Price>
        <Weight></Weight>
        <RemainingItems></RemainingItems>
        <RemainingAmount></RemainingAmount>
        <NumItemsScanned></NumItemsScanned>
        <ScannedItemsAmount></ScannedItemsAmount>
    </AddAuditItemResponse>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szAddAuditItemResponse) )
        {
            xs.Deserialize(szResult,        a_bstrtResult           );
            xs.Deserialize(szResultMessage, a_bstrtResultMessage    );
            xs.Deserialize(szRescanType,    a_bstrtRescanType       );
            xs.Deserialize(szRescanStatus,  a_bstrtRescanStatus     );

            xs.Deserialize(szId,            a_bstrtId               );
            xs.Deserialize(szQuantity,      a_bstrtQuantity         );
            xs.Deserialize(szBarcode,       a_bstrtBarcode          );
            xs.Deserialize(szDescription,   a_bstrtDescription      );
            xs.Deserialize(szPrice,         a_bstrtPrice            );
            // xs.Deserialize(szWeight,     a_bstrtWeight           );

            // if we did not fail...
            //
            if ( _tcscmp( (TCHAR*) a_bstrtResult, szFailure) != 0 )
            {
                // get updated amount and count..
                //
                xs.Deserialize(szRemainingItems,    a_bstrtRemainingItems   );
                xs.Deserialize(szRemainingAmount,   a_bstrtRemainingAmount  );
            }

            xs.Deserialize(szNumItemsScanned,       a_bstrtNumItemsScanned  );
            xs.Deserialize(szScannedItemsAmount,    a_bstrtScannedItemsAmount);
        
            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessAddAuditItemResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  CreateEndAuditRequest
/**
This method is called by the application to initiate an end audit session with above
store server through FLServices
*/
bstr_t CMobileCartAuditParser::CreateEndAuditRequest(
    _bstr_t a_bstrtAction )
{
    trace(L6, _T("+CMobileCartAuditParser::CreateEndAuditRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(szEmpty);
    BOOL            bSuccess = FALSE;

    CScanInfo oScanInfo;

    //<?xml version="1.0"?>
    //<EndAudit>
    //    <Action>Send/Cancel/Skip/Abort/ErrorExit</Action>
    //</EndAudit>

    if ( eXsSuccess == xs.Create() )
    {
        xs.Serialize( szEndAudit );
        xs.Serialize( szAction, a_bstrtAction);

        bstrtXml = xs.GetXml();

        #ifdef _CPPUNIT
            xs.Save(_T("EndAuditRequest.xml"));
        #endif

        bSuccess = TRUE;
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateEndAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}


///////////////////////////////////////////////////////////////////////////////////
///  ProcessEndAuditResponse
/**
This method is called by the application to process the end audit response
*/
BOOL CMobileCartAuditParser::ProcessEndAuditResponse( 
    _bstr_t a_bstrtXml,
    _bstr_t & a_bstrtResult,
    _bstr_t & a_bstrtResultMessage,
    _bstr_t & a_bstrtRescanType,
    _bstr_t & a_bstrtRescanStatus )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessEndAuditResponse()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    /*
    <EndAuditResponse>
        <Result></Result>
        <ResultMessage></ResultMessage>
        <RescanType></RescanType>
        <RescanStatus></RescanStatus>
    </EndAuditResponse>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szEndAuditResponse) )
        {
            xs.Deserialize(szResult,        a_bstrtResult           );
            xs.Deserialize(szResultMessage, a_bstrtResultMessage    );
            xs.Deserialize(szRescanType,    a_bstrtRescanType       );
            xs.Deserialize(szRescanStatus,  a_bstrtRescanStatus     );

            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessEndAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}


///////////////////////////////////////////////////////////////////////////////////
///  CreateSwitchToFullAuditRequest
/**
This method is called by the application to create a Switch To Full Audit Request.
*/
bstr_t CMobileCartAuditParser::CreateSwitchToFullAuditRequest(
    _bstr_t a_bstrtOnError )
{
    trace(L6, _T("-CMobileCartAuditParser::CreateSwitchToFullAuditRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(L"");
    BOOL            bSuccess = FALSE;

    /*
    <!-- SwitchToFullAudit - SCO to FLServices -->
   <SwitchToFullAudit>
        <!--set on error to true if switching to full scan due to encountering and error condition-->
        <OnError>true/false</OnError> 
    </SwitchToFullAudit>
    */

    if ( eXsSuccess == xs.Create() )
    {
        xs.Serialize( szSwitchToFullAudit );
        xs.Serialize( szOnError, a_bstrtOnError);

        bstrtXml = xs.GetXml();

        #ifdef _CPPUNIT
            xs.Save(_T("SwitchToFullAudit.xml"));
        #endif

        bSuccess = TRUE;
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateSwitchToFullAuditRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessSwitchToFullAuditResponse
/**
This method is called by the application to process switch to full audit response
*/
BOOL CMobileCartAuditParser::ProcessSwitchToFullAuditResponse( 
    _bstr_t a_bstrtXml,
    _bstr_t & a_bstrtResult,
    _bstr_t & a_bstrtResultMessage,
    // _bstr_t & a_bstrtRescanType,
    _bstr_t & a_bstrtRescanStatus )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessSwitchToFullAuditResponse()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    /*
    <SwitchToFullAudit>
        <Result></Result>
        <ResultMessage></ResultMessage>
        <!-- <RescanType></RescanType> -->
        <RescanStatus></RescanStatus>
    </SwitchToFullAudit>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szSwitchToFullAuditResponse) )
        {
            xs.Deserialize(szResult,        a_bstrtResult           );
            xs.Deserialize(szResultMessage, a_bstrtResultMessage    );
            // xs.Deserialize(szRescanType, a_bstrtRescanType       );
            xs.Deserialize(szRescanStatus,  a_bstrtRescanStatus     );
        
            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessSwitchToFullAuditResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bSuccess;
}

///////////////////////////////////////////////////////////////////////////////////
///  CreateContinueAfterErrorRequest
/**
This method is called by the application to continue audit after scan error
*/
bstr_t CMobileCartAuditParser::CreateContinueAfterErrorRequest(
    _bstr_t a_bstrtBarcode )
{
    trace(L6, _T("+CMobileCartAuditParser::CreateContinueAfterErrorRequest()"));

    CXmlSerializer  xs;
    _bstr_t         bstrtXml(szEmpty);
    BOOL            bSuccess = FALSE;

    /*
    <ContinueAfterError>
        <Barcode></Barcode>
    </ContinueAfterError>
    */

    if ( eXsSuccess == xs.Create() )
    {
        xs.Serialize( szContinueAfterError );
        // xs.Serialize( szId,      a_bstrtId );
        xs.Serialize( szBarcode,    a_bstrtBarcode);
        // xs.Serialize( szWeight,  a_bstrtWeight);

        bstrtXml = xs.GetXml();

        #ifdef _CPPUNIT
            xs.Save(_T("ContinueAfterErrorRequest.xml"));
        #endif

        bSuccess = TRUE;
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::CreateContinueAfterErrorRequest() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure);

    return bstrtXml;
}

///////////////////////////////////////////////////////////////////////////////////
///  ProcessContinueAfterErrorResponse
/**
This method is called by the application to process add audit item response
*/
BOOL CMobileCartAuditParser::ProcessContinueAfterErrorResponse( 
    _bstr_t a_bstrtXml,
    _bstr_t & a_bstrtResult,
    _bstr_t & a_bstrtResultMessage,
    _bstr_t & a_bstrtRescanType,
    _bstr_t & a_bstrtRescanStatus )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessContinueAfterErrorResponse()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    /*
    <ContinueAfterErrorResponse>
        <Result></Result>
        <ResultMessage></ResultMessage>
        <RescanType></RescanType>
        <RescanStatus></RescanStatus>
    </ContinueAfterErrorResponse>
    */

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtXml ) )
    {
        if ( xs.Deserialize(szContinueAfterErrorResponse) )
        {
            xs.Deserialize(szResult,                a_bstrtResult           );
            xs.Deserialize(szResultMessage,         a_bstrtResultMessage    );
            xs.Deserialize(szRescanType,            a_bstrtRescanType       );
            xs.Deserialize(szRescanStatus,          a_bstrtRescanStatus     );

            bSuccess = TRUE;
        }
    }

    xs.Close();

    trace(L6, _T("-CMobileCartAuditParser::ProcessContinueAfterErrorResponse() - returns: %s"), bSuccess ? szAuditSuccess : szAuditFailure  );

    return bSuccess;
}

BOOL CMobileCartAuditParser::ProcessScanInfo(
    _bstr_t a_bstrtScanInfoXml,
    _bstr_t & a_bstrtScanData,
    _bstr_t & a_bstrtScanDataLabel,
    long & lBarcodeType )
{
    trace(L6, _T("+CMobileCartAuditParser::ProcessScanInfo()"));

    CXmlSerializer  xs;
    BOOL            bSuccess = FALSE;
    int nBarcodeType = 0;

    if ( eXsSuccess ==  xs.LoadXml( a_bstrtScanInfoXml ) )
    {
        if ( xs.Deserialize(_T("ScanInfo") ))
        {
            xs.Deserialize(szBarcodeType,         nBarcodeType            );
            xs.Deserialize(szBarcodeData,         a_bstrtScanData         );
            xs.Deserialize(szBarcodeLabel,        a_bstrtScanDataLabel    );
            lBarcodeType = (long) nBarcodeType;

            bSuccess = TRUE;
        }
    }
    return bSuccess;

}