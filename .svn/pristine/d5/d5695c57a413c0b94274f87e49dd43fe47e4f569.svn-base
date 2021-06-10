///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCartParser.CPP
//          Manages the mobile cart
//
// AUTHOR:  Barbara Dawkins
// 
/////////////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////////////////////////////////

#define _WIN32_WINNT 0x0400
#include "stdafx.h"
#include "Common.h"

#include "MobileCartParser.h"
#include "MobileConstants.h"
#include "MobileConstantsSSF.h"
#include "QueuedActionRecallItem.h"
#include "QueuedActionRescanItem.h"
#include "MobileCartAudit.h"
extern CMobileCartAudit mobileAudit;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCartParser")
CMobileCartParser myCMobileCartParser;
DLLEXPORT CMobileCartParser &mobileCartParser = myCMobileCartParser;


IMPLEMENT_DYNCREATE(CMobileCartParser, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////

/**  
Constructor for CMobileCartParser object
*/
CMobileCartParser::CMobileCartParser()
{
    pCartDocument=NULL;
    pCartItemGroup = NULL;
    m_bSendCustomerIDAsScan=true;
    m_nRussiaEgaisItems = 0;
    new CQueuedActionsContainer();
}

///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////

/// Destructor
/**  
Destructor for CMobileCartParser object
*/
CMobileCartParser::~CMobileCartParser()
{
    try 
    {
        if (pCartDocument)
        {
            pCartDocument.Release();
            pCartDocument=NULL;
        }
        if (pCartItemGroup)
        {
            pCartItemGroup.Release();
            pCartItemGroup = NULL;
        }
    }
    catch(...)
    {
        //Don't trace anything on exit,  datacapture object may not be there.
    }

}

///////////////////////////////////////////////////////////////////////////////////
// UnInitialize for shutdown
///////////////////////////////////////////////////////////////////////////////////

/// UnInitialize
/**
Uninitialize the object and free all resources.
*/
void CMobileCartParser::UnInitialize()
{
    trace(L6, _T("+UnInitialize()"));
    if (m_bInitialized)
    {
        try 
        {
            caIDFilters.RemoveAll();
            (void) ClearBarCodeConversionOptionMap();
            Reset();
        }
        catch(...)
        {
            trace( L6, _T("-UnInitialize() : exception"));
        }
        m_bInitialized = false;
    }
}

///////////////////////////////////////////////////////////////////////////////////
// Initialize
///////////////////////////////////////////////////////////////////////////////////

/// Initialize
/**
Initialize the object and allocate all required resources.
   @param[out] retVal Error code (Always zero for now)
*/
long CMobileCartParser::Initialize()
{
    long rc = 0;
    trace(L6, _T("+Initialize()"));
    Reset();
    
    m_bInitialized = true;
    
    trace(L6, _T("-Initialize() = %d"), rc);
    
    caIDFilters.RemoveAll();
    (void) ClearBarCodeConversionOptionMap();
    
    //Load optional CustomerID filters
    bool bEndOfFiltersList = false;
    int nIndex=1;
    CString csOptionFormat = _T("MobileCustIDFilter%d");
    CString csOptionName;
    CString csFilterDefault(_T("NOT_FOUND"));
    while (!bEndOfFiltersList)
    {
        csOptionName.Format(csOptionFormat, nIndex);
        CString csFilter = cr.GetStringOption(_T("Operations"), csOptionName, csFilterDefault ,_T("Scotopts"));
        
        if (csFilter.Compare(csFilterDefault) == 0)
        {
            bEndOfFiltersList = true;
        }
        else
        {
            SetCustomerIDFilter(nIndex, csFilter);
        }
        nIndex++;
    }
    
    
    //Load optional MobileShopperBarcodeConversions
    bEndOfFiltersList = false;
    nIndex = 1;
    csOptionFormat = _T("MobileShopperBarcodeConversion%d");
    
    while (!bEndOfFiltersList)
    {
        csOptionName.Format(csOptionFormat, nIndex);
        CString csOptionValue = cr.GetStringOption(_T("Operations"), csOptionName, csFilterDefault ,_T("Scotopts"));
        
        
        if (csOptionValue.Compare(csFilterDefault) == 0)
        {
            bEndOfFiltersList = true;
        }
        else
        {
            int nPos = csOptionValue.Find(_T(","));
            if (nPos != -1 && nIndex > 0)
            {
                CString csBarCodeType = csOptionValue.Mid(0,nPos);
                csBarCodeType.TrimLeft();
                csBarCodeType.TrimRight();
                CString csBarCodePrefix = csOptionValue.Mid(nPos+1);
                
                AddBarCodeConversionOptionToMap(csBarCodeType, csBarCodePrefix);
            }
        }
        nIndex++;
    }
    
    return rc;
    
}

void CMobileCartParser::Reset()
{
    m_lTotalDue = 0;
    m_nTransactionNum = 0;
    m_bstrCartID = _T("");
    m_lVoidedItems = 0;
    m_lNumberOfItems = 0;
    m_nRussiaEgaisItems = 0;

    try 
    {
        if (pCartDocument)
        {
            pCartDocument.Release();
            pCartDocument=NULL;
        }
        if (pCartItemGroup)
        {
            pCartItemGroup.Release();
            pCartItemGroup = NULL;
        }
    }
    catch(...)
    {
        trace( L6, _T("-CMobileCartParserBase:Reset() : exception"));
    }
}


void CMobileCartParser::GetItems(int &nValidItems)
{      
    trace(L7, _T("+CMobileCartParser::GetItems"));
    CString csWork;
    CString csTemp;
    CString csAttributes;
    CString csReScan;
    CString csScanType;
    CString csOrgBarcode;
    MSXML2::IXMLDOMNodePtr pNode;
    MSXML2::IXMLDOMNodePtr pItem;
    MSXML2::IXMLDOMNodePtr pItemGroup;
    MSXML2::IXMLDOMNodeListPtr pItems;

    // Get items and add to map (LATER - For now just build CString and send)
    pItemGroup = pCartDocument->selectSingleNode(_T("//ItemGroup"));
    pItems = pCartDocument->selectNodes(_T("//ItemGroup/Item"));
    if (pItems != NULL)
    {
        int count = pItems->length;
        int i;
        
        for (i = 0; i < count; i++)
        {
            pItem = pItems->item[i];
            bool bProcessItem = true;
            
            // Get item code
            pNode = pItem->selectSingleNode(_T("Barcode"));
            if (pNode != NULL)
            {
                CString csItemCode((LPCTSTR)pNode->text);

                csOrgBarcode = csItemCode;
                
                long lRescanCode = GetRescanCode(pItem);
                bProcessItem = (lRescanCode != 2) && !IsEgaisItem(pItem);


                // Get barcode type and modify UPC according to configuration. Since scanners can be configured to
                // prepend/postpend characters to the UPC, this will perform the same action to the UPC values
                // sent from the server. This will allow SSCO to process the items as though they were scanned locally.
                pNode = pItem->selectSingleNode(_T("BarcodeType"));
                if (pNode != NULL)  // && (LPCTSTR)pNode->text != NULL
                {
                    csScanType = (LPCTSTR) pNode->text;
                    CString csFormatBarcode;

                    CString csValue;
                    bool bResult = ca_map.Lookup(csScanType,csValue);


                    //*********************************
                    if (bResult) // Found Barcode Type in the Barcode Conversion Map
                    {
                        csFormatBarcode = csValue;
                    }
                    //**********************************

                    if (!csFormatBarcode.IsEmpty())
                    {
                        CString csNewItemCode;
                        csNewItemCode.Format(csFormatBarcode,csItemCode);
                        csItemCode = csNewItemCode;
                    }
                }
                else //No barcode type, treat it as key in barcode
                {
                    csScanType = _T("0");
                }

                
                // Get extended price
                csWork = _T("");
                pNode = pItem->selectSingleNode(_T("NettoPrice"));
                if (pNode != NULL)
                {
                    csWork = (LPCTSTR)pNode->text;
                    csWork.TrimLeft();
                    csWork.TrimRight();
                    int nPos = csWork.Find(_T("."));
                    if (nPos > -1)
                    {
                        // If decimal is included - use up to at most 2 digits
                        csWork = csWork.Mid(0, nPos) + csWork.Mid(nPos + 1, 2);
                        csWork.TrimLeft(_T("0"));
                    }
                }
                csAttributes = csWork;
                
                CString csID;
                pNode = pItem->selectSingleNode(_T("PositionNumber"));
                if (pNode != NULL)
                {
                    csID = (LPCTSTR)pNode->text;
                    csID.TrimLeft();
                    csID.TrimRight();
                }
                
                long lOrigin = 0;
                pNode = pItem->selectSingleNode(_T("Origin"));
                if (pNode != NULL)
                {
                    CString csOrigin = (LPCTSTR)pNode->text;
                    lOrigin = _ttol(csOrigin);
                }

                // Add item to recall queue
                if (bProcessItem)
                {  
                    nValidItems++;
                    CQueuedActionRecallItem* pAction = 
                           new CQueuedActionRecallItem(csID, csOrgBarcode, csItemCode, _ttol(csScanType));
                    pAction->SetRescanItem(lRescanCode);
                    pAction->SetOrigin(lOrigin);

                    recallList.QueueMobileItem(pAction);

                }
            }
            else
            {
                trace(L6, _T("No item code found in cart for item %d"), i);
            }
            pItemGroup->removeChild(pItem);

        }
    }
    trace(L7, _T("-CMobileCartParser::GetItems"));
} 


long CMobileCartParser::GetRescanCode(MSXML2::IXMLDOMNodePtr pItemNode)
{
    long lRescan = 0;
    MSXML2::IXMLDOMNodePtr pNode;
    pNode = pItemNode->selectSingleNode(_T("RescanItem"));
    if (pNode != NULL)
    {
        CString csRescan = (LPCTSTR)pNode->text;
        lRescan = _ttol(csRescan);  
    }
    return lRescan;
}

/* look for Egais Item indicator 
      <Attribute>
         <Name>EgaisItem</Name>
         <Value>true</Value>
      </Attribute>
*/
bool CMobileCartParser::IsEgaisItem(MSXML2::IXMLDOMNodePtr pItemNode)
{
    MSXML2::IXMLDOMNodeListPtr pAttributes;
    MSXML2::IXMLDOMNodePtr pAttribute;
    MSXML2::IXMLDOMNodePtr pName;
    MSXML2::IXMLDOMNodePtr pValue;
    bool bEgaisItem = false;

    pAttributes = pItemNode->selectNodes(_T("AttributeGroup/Attribute"));
    if (pAttributes != NULL)
    {
        int count = pAttributes->length;
        int i;
        
        for (i = 0; i < count; i++)
        {
            pAttribute = pAttributes->item[i];
            if (pAttribute)
            {
                pName = pAttribute->selectSingleNode(_T("Name"));
                CString csName = (LPCTSTR)pName->text;
                if (csName.Find(_T("EgaisItem")) == 0)
                {
                    pValue = pAttribute->selectSingleNode(_T("Value"));
                    if (pValue)
                    {
                        CString csValue = (LPCTSTR)pValue->text;
                        if (csValue.CompareNoCase(_T("true")) == 0)
                        {
                            bEgaisItem = true;
                            m_nRussiaEgaisItems++;
                        }
                    }
                }
            }
        }
    }
    
    return bEgaisItem;
}


///  ProcessCart
/**
This method translates the externally received mobile cart into an internal format in order for the
items to be sold within a new transaction on this lane.
   @param[in] pCartDocument A pointer to the XML document containing the mobile cart.
   @param[out] retVal An indication as to whether or not the XML contained a valid cart and it was sent internally
*/
CString CMobileCartParser::ProcessCart(_bstr_t bstrCartXML)
{

    try
    {
        HRESULT hr = pCartDocument.CreateInstance(__uuidof(MSXML2::DOMDocument60));
        //Make sure that the DOM document object was successfully created
        if (FAILED(hr))
        {
            trace( L1, _T("ProcessCart - HRESULT=%x, Unable to create DOMDocument object"), hr);
            return _T("");
        }
        trace(L7, _T("ProcessCart processing xml string: %s"), (LPCTSTR) bstrCartXML);
        VARIANT_BOOL bResult = pCartDocument->loadXML(bstrCartXML);
        
#ifdef _DEBUG
        //TODO:  Delete this DEBUG section
        try
        {
            CTime ctTime = CTime::GetCurrentTime();
            CString csTime = ctTime.Format(_T("%Y%m%d%H%M%S"));
            
            CString csFileName;
            csFileName.Format(_T("c:\\temp\\Ticket%s_Received.xml"), csTime);
            CFile cFileSave(csFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);//modeNoTruncate
            int nTmp = (int)cFileSave.SeekToEnd();
            cFileSave.Write((LPCTSTR) bstrCartXML, bstrCartXML.length() * sizeof(_TCHAR));//TAR 392046 - double the size of data to handle UNICODE if the receipt file is set to ANSI
            cFileSave.Close();
        }
        catch(...)
        {
            trace(L6, _T("Unable to save debug copy of downloaded ticket"));
        }
#endif 


        if( bResult != VARIANT_TRUE )
        {
            MSXML2::IXMLDOMParseError* parseError = pCartDocument->GetparseError();
            _bstr_t reason = parseError->Getreason();
            int filePos = parseError->Getfilepos();
            trace(L1, _T("ProcessCart - Unable to load xml, bResult=%x"),  bResult);
            return _T("");
        } 
        MSXML2::IXMLDOMNodePtr pCartID = pCartDocument->selectSingleNode(_T("//Header/TransactionNumber"));
        if (pCartID == NULL)
        {
            trace( L1, _T("ProcessCart - Unable to retrieve transaction number"), hr);
            return _T("");
        }
        m_bstrCartID = pCartID->text;
        MSXML2::IXMLDOMNodePtr pRescan = pCartDocument->selectSingleNode(_T("//Footer//RescanRequired"));
        MSXML2::IXMLDOMNodePtr pTemp = pCartDocument->selectSingleNode(_T("//ItemGroup"));
        MSXML2::IXMLDOMNodePtr pItemGroup;
 
        pCartItemGroup = pCartDocument->createDocumentFragment();

        
        if (pTemp)
        {
            pItemGroup =  pCartDocument->createElement(_T("ItemGroup"));
            _bstr_t    bstrTest = pItemGroup->Getxml();
            trace(L6, _T("pItemGroup = \n%s"), (LPCTSTR) bstrTest);
            pCartItemGroup->appendChild(pItemGroup);
        }
                

        if (pRescan)
        {
            CString csText = (LPCTSTR) pRescan->text;
            if (csText.Find(_T("true")) != -1)
            {
                trace(L6, _T("ProcessCart - Sending RescanRequired"));
                return MOBILE_RESCAN_REQUIRED;
            }
        }


        // If no cart we have no idea what this is
        if (pCartItemGroup == NULL)
        {
            trace(L6, _T("ProcessCart - No <cart> element found"));
            return _T("");
        }
        
        int nValidItems = 0;
        // Obtain loyalty card number
        if (m_bSendCustomerIDAsScan)
        {
            MSXML2::IXMLDOMNodePtr pCardNumber = pCartDocument->selectSingleNode(_T("//Header//CardNumber"));
            if (pCardNumber != NULL)
            {
                CString csLoyaltyID((LPCTSTR)pCardNumber->text);
                if (IsValidCustomerID(csLoyaltyID))
                {
                    nValidItems++;

                    CQueuedActionRecallItem* pAction = 
                           new CQueuedActionRecallItem(_T("LOYALTY"), csLoyaltyID, csLoyaltyID, 0);
                    pAction->SetIsLoyaltyID(true);
                    recallList.QueueMobileItem(pAction);

                }
            }
        }
        
        
        GetItems(nValidItems);




        return _T("START_MOBILE");
    }
    catch (_com_error ce) 
    {
        HRESULT hr = ce.Error();
        CString csMesg = ce.ErrorMessage();
        CString csErrorCode;
        csErrorCode.Format(_T("%x"),hr);
        _bstr_t test = ce.Description();
        
        trace(L6, _T("ProcessCart - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
        hr, csMesg, test);
    }
    catch(...)
    {
        trace( L6, _T("-ProcessCart Caught Unknown Exception"), -1);
    }
    return _T("");

}

///////////////////////////////////////////////////////////////////////////////////
// Function:                                     
// Class:       SendItemSold                                         
// Description: 
// Parameters:  None       
//             
// Comments:    N/A
//              
//              
// Returns:     N/A                                        
///////////////////////////////////////////////////////////////////////////////////
void CMobileCartParser::SendItemSold(ItemDetails &itemDetails, bool bRecalledItem)
{
    CMobileCartParserBase::SendItemSold(itemDetails, bRecalledItem);
}


void CMobileCartParser::SetTotalDue(long lTotal)
{
    CMobileCartParserBase::SetTotalDue(lTotal);
}

void CMobileCartParser::SetPOSTransactioNum(int nTransactionNumber)
{
    CMobileCartParserBase::SetPOSTransactioNum(nTransactionNumber);
}

_bstr_t GetPrettyXML(_bstr_t bstrXML)

{
       MSXML2::IMXWriterPtr writer;
       writer.CreateInstance(__uuidof(MSXML2::MXXMLWriter));
       writer->indent = VARIANT_TRUE;
       writer->omitXMLDeclaration = VARIANT_TRUE;

       MSXML2::ISAXXMLReaderPtr reader;
       reader.CreateInstance(__uuidof(MSXML2::SAXXMLReader));
 
       MSXML2::ISAXContentHandlerPtr handler = writer;

       COleVariant oleString = bstrXML;
       reader->putContentHandler(handler);
       reader->parse(oleString);

       return writer->output;

}

_bstr_t CMobileCartParser::GetFinishRequest()
{
    //UploadTicket

    trace(L6, _T("+GetFinishRequest()"));
    _bstr_t bstrReturn = _T("");
    _bstr_t bstrNewCart = _T("");
    if( pCartDocument != NULL)  //Finish XML will not be built and sent unless we have received a mobile cart
    {
        try
        {
        CString csValue, csTransactionNum, csTotalDue;
        CTime ctTime = CTime::GetCurrentTime();
        CString csTime = ctTime.Format(_T("%Y%m%d%H%M%S"));


        MSXML2::IXMLDOMNodePtr pFooter = pCartDocument->selectSingleNode(_T("//Footer"));

        csTransactionNum.Format(_T("%d"), m_nTransactionNum);
        csTotalDue.Format(_T("%d"), m_lTotalDue);

        CString csFooterXML = (LPCTSTR) pFooter->Getxml();
        csFooterXML = UpdateNode(csFooterXML, _T("Origin"), MOBILE_ORIGIN_POS);
        csFooterXML = UpdateNode(csFooterXML, _T("Timestamp"), csTime);
        csFooterXML = UpdateNode(csFooterXML, _T("PosTransactionNumber"), csTransactionNum);
        csFooterXML = UpdateNode(csFooterXML, _T("TotalAmountPos"), csTotalDue);
        csFooterXML = UpdateNode(csFooterXML, _T("VoidedItems"), m_lVoidedItems);
        csFooterXML = UpdateNode(csFooterXML, _T("NumberOfItems"), m_lNumberOfItems);

        MSXML2::IXMLDOMNodePtr pHeader = pCartDocument->selectSingleNode(_T("//Header"));

        CString csHeaderXML = (LPCTSTR) pHeader->Getxml();
        csHeaderXML = UpdateNode(csHeaderXML, _T("Timestamp"), csTime);
        csHeaderXML = UpdateNode(csHeaderXML, _T("Origin"), MOBILE_ORIGIN_POS);

        bstrNewCart = _T("<Document>") + csHeaderXML;
        
        bstrNewCart = bstrNewCart + buildItemList();

        csFooterXML = UpdateNode(csFooterXML, _T("VoidedItems"), m_lVoidedItems);
        csFooterXML = UpdateNode(csFooterXML, _T("NumberOfItems"), m_lNumberOfItems);

		MSXML2::IXMLDOMNodePtr pAttributes = pCartDocument->selectSingleNode(_T("//AttributeGroup"));
		if(NULL != pAttributes)
		{
			CString csAttributeGroupXML = (LPCTSTR) pAttributes->Getxml();
			bstrNewCart = bstrNewCart + csAttributeGroupXML;
		}


        bstrNewCart = bstrNewCart + csFooterXML + _T("</Document>");
        
        bstrReturn = GetPrettyXML(bstrNewCart);
        
#ifdef _DEBUG
        //TODO:  Delete this DEBUG section
        try
        {
            CString csFileName;
            csFileName.Format(_T("c:\\temp\\Ticket%s_Upload.xml"), csTime);
            CFile cFileSave(csFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);//modeNoTruncate
            int nTmp = (int)cFileSave.SeekToEnd();
            cFileSave.Write((LPCTSTR) bstrReturn, bstrReturn.length() * sizeof(_TCHAR));//TAR 392046 - double the size of data to handle UNICODE if the receipt file is set to ANSI
            cFileSave.Close();
        }
        catch(...)
        {
            trace(L6, _T("Unable to save debug copy of downloaded ticket"));
        }
#endif 

        trace(L7, _T("Finish request = \n%s"), (LPCTSTR) bstrReturn);
        }
        catch (_com_error ce) 
        {
            HRESULT hr = ce.Error();
            CString csMesg = ce.ErrorMessage();
            CString csErrorCode;
            csErrorCode.Format(_T("%x"),hr);
            _bstr_t test = ce.Description();
            
            trace(L6, _T("GetFinishRequest - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
                hr, csMesg, test);
        }
        catch(...)
        {
            trace( L6, _T("-GetFinishRequest Caught Unknown Exception"), -1);
        }

        
    }
    trace(L6, _T("-GetFinishRequest()"));
    return bstrReturn;
}


///////////////////////////////////////////////////////////////////////////////////
// Function:    updateItemMap                                 
// Class:       MobileCart                                         
// Description: 
// Parameters:  Item details       
//             
// Comments:    N/A
//              
//              
// Returns:     The XML string                                        
///////////////////////////////////////////////////////////////////////////////////
_bstr_t CMobileCartParser::updateItemMap(long id, 
                                         bool bVoid,
                                         const CString &upc,
                                         long lextendedPrice, 
                                         const CString &sDesc,
                                         bool bRecalledItem)
{
    return CMobileCartParserBase::updateItemMap(id, bVoid, upc, 
        lextendedPrice, sDesc, bRecalledItem);
}


_bstr_t CMobileCartParser::serializeItemMap() 
{
    return CMobileCartParserBase::serializeItemMap();
}

CString CMobileCartParser::UpdateNode(CString csXML, CString csNodeName, CString csNodeValue)
{
    CString csBegin, csEnd;
    csBegin.Format(_T("<%s>"), csNodeName);
    csEnd.Format(_T("</%s>"), csNodeName);

    CString csReturn;
    int nPos = csXML.Find(csBegin);
    if (nPos != -1)
    {
        csReturn = csXML.Mid(0, nPos + csBegin.GetLength());
        csReturn += csNodeValue;
        nPos = csXML.Find(csEnd);
        csReturn += csXML.Mid(nPos);
    }
    else
    {

        int nPos = csXML.ReverseFind(_T('<'));
        csReturn = csXML.Mid(0, nPos) + csBegin + csNodeValue + csEnd + csXML.Mid(nPos);
    }
        
    return csReturn;
        
}

CString CMobileCartParser::UpdateNode(CString csXML, CString sNodeName, long lNodeValue)
{
    CString csValue;
    csValue.Format(_T("%d"), lNodeValue);
    return UpdateNode(csXML, sNodeName,  csValue);
}

void CMobileCartParser::SetSendCustomerIDAsScan(bool bSendCustomerIDAsScan)
{
    m_bSendCustomerIDAsScan = bSendCustomerIDAsScan;
}

bool CMobileCartParser::SetCustomerIDFilter(int nIndex, CString csFilter)
{
    trace(L6, _T("CMobileCartParser::SetCustomerIDFilter  %d, %s"), nIndex, csFilter);
    CString csFilterType;
    CString csFilterString;
    CString csError = _T("");
    bool bGoodFilter = false;
    int nPos = csFilter.Find(_T(","));
    if (nPos != -1 && nIndex > 0)
    {
        csFilterType = csFilter.Mid(0,nPos);
        csFilterType.TrimLeft();
        csFilterType.TrimRight();
        csFilterType.MakeUpper();
        csFilterString = csFilter.Mid(nPos+1);
        try
        {
            LPMOBILEIDFILTER newFilter = new MOBILEIDFILTER;
            newFilter->csFilterType = csFilterType;
            newFilter->csFilterString = csFilterString;
            caIDFilters.SetAtGrow(nIndex-1, newFilter);
            bGoodFilter = true;
        }
        catch( CException *e)
        {
            TCHAR   szCause[255];
            e->GetErrorMessage(szCause, 255);

            trace(L6, _T("Exception creating MobileFilter %s"), szCause);
        }
    }
    else
    {
        trace(L6, _T("Invalid filter"));
    }

    return bGoodFilter;

}

bool CMobileCartParser::IsValidCustomerID(CString csCustomerID)
{
    int nFilters = caIDFilters.GetSize();
    bool bMatched = nFilters?false:true;

    for (int nIndex=0; (nIndex < nFilters) && !bMatched; nIndex++)
    {	
        CString csFilter = caIDFilters.GetAt(nIndex)->csFilterString;
        CString csFilterType = caIDFilters.GetAt(nIndex)->csFilterType;
        bMatched = IsFilterMatch(csFilter, csCustomerID);

        if (csFilterType.Compare(_T("E")) == 0)
        {
            if (bMatched)
            {
                return false;  //Matched exclusion pattern
            }
        }
        else if (csFilterType.Compare(_T("I")) != 0)
        {
            return false;  //I dont know what this pattern type is
        }
    }
    return bMatched;
}

bool CMobileCartParser::IsFilterMatch(CString csFilterString, CString csCompareString)
{
    
    int nFilterLen = csFilterString.GetLength();
    bool bVariableLength = false;
    bool bMatched = false;
    
    if ( csFilterString.GetAt(nFilterLen - 1) == _T('+') )
        bVariableLength = true;
    
    if ( (nFilterLen == 1) && csFilterString.GetAt(0) == _T('*'))
    {
        bMatched = true;
        
    }
    else if ((csCompareString.GetLength() == csFilterString.GetLength())||
        (bVariableLength && (csCompareString.GetLength() >= csFilterString.GetLength())) )
        
    {	// first check if lengths match
        int pos = 0;
        while(pos < csFilterString.GetLength())
        {	// compare each character
            _TCHAR strChar = csFilterString.GetAt(pos);
            // ignore x and c
            if((strChar != _T('x')) && (strChar != _T('c')) && (strChar != _T('+')))
            {
                if(strChar != csCompareString.GetAt(pos))
                {	// doesn't match so go to next coupon format
                    break;
                }
            }
            pos++;
        } 
        if(pos == csFilterString.GetLength())
            bMatched = true;
    }
    return bMatched;
    
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      void CMobileCartParser::AddBarCodeConversionOptionToMap
//
// Purpose:     The method add the Mobile Barcode Conversion option-value pair in the map
//
// Parameters:  LPCTSTR szKey   - the key which is MobileShopperBarcodeConversion Barcode Type 
//              CString csValue - the barcode prefix
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void CMobileCartParser::AddBarCodeConversionOptionToMap(LPCTSTR szKey, CString csValue)
{
    trace(L6, _T("+CMobileCartParser::AddBarCodeConversionOptionToMap()"));

    ca_map[szKey] = csValue;

    trace(L6, _T("-CMobileCartParser::AddBarCodeConversionOptionToMap() BarCodeType = %s, BarcodePrefix = %s"), szKey, csValue);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool CMobileCartParser::ClearBarCodeConversionOptionMap
//
// Purpose:     The method removes all entries of the map (empties it)
//
// Parameters:  void
//
// Returns:     BOOL - returns true if map is empty, false if not
//
/////////////////////////////////////////////////////////////////////////////////////
bool CMobileCartParser::ClearBarCodeConversionOptionMap()
{
    bool fRet; 

    trace(L6, _T("+CMobileCartParser::ClearBarCodeConversionOptionMap()"));

    ca_map.RemoveAll();

    fRet = ca_map.IsEmpty();

    trace(L6, _T("-CMobileCartParser::ClearBarCodeConversionOptionMap() returns = %d"), fRet);

    return fRet;
}

///////////////////////////////////////////////////////////////////////////////////
// Function:    buildItemList                                 
// Class:       MobileCart                                         
// Description: 
// Parameters:  Item details       
//             
// Comments:    N/A
//              
//              
// Returns:                                            
///////////////////////////////////////////////////////////////////////////////////
_bstr_t CMobileCartParser::buildItemList()
{
    CTime ctTime = CTime::GetCurrentTime();
    CString csTime = ctTime.Format(_T("%Y%m%d%H%M%S"));
    MSXML2::IXMLDOMNodePtr pItemList = pCartItemGroup->selectSingleNode(_T("//ItemGroup"));
    //MSXML2::IXMLDOMNodePtr pOldItemList = pCartDocument->selectSingleNode(_T("//ItemGroup"));

    _bstr_t bstrItemGroup = _T("<ItemGroup>");
    _bstr_t bstrAuditItems = _T("");
    m_lNumberOfItems = 0;
    m_lVoidedItems = 0;
    int nItemCount = recallList.GetNumSoldItems();
    int nPositionNum = 1;
    for (int i = 0; i<nItemCount; i++)
    {
        IXMLDOMNodePtr pItemNode;
        CQueuedActionRecallItem* pActionItem = (CQueuedActionRecallItem*) recallList.GetNextSoldItem(true);
        
        if (pActionItem)
        {
            long lQty = pActionItem->GetQty() < 2 ? 1 : pActionItem->GetQty();
            bool bProcess = true;

            if (pActionItem->GetItemProperty() == MOBILE_ITEMPROP_VOIDED)
            {
                if (pActionItem->GetOrigin() == MOBILE_ORIGIN_POS)
                {
                    bProcess = false; //Revision doesn't care about non-mobile items that are voided
                }
                else
                {
                    m_lVoidedItems += lQty;
                }
            }
            if (bProcess)
            {
                m_lNumberOfItems += lQty;
                
                for (int j = 1; j<=lQty && bProcess; j++)
                {
                    CString csItem = _T("<Item></Item>");
                    csItem = UpdateNode(csItem, _T("Origin"), MOBILE_ORIGIN_POS);        
                    csItem = UpdateNode(csItem, _T("PositionNumber"), nPositionNum);
                    csItem = UpdateNode(csItem, _T("Barcode"), pActionItem->GetScanDataLabel());
                    csItem = UpdateNode(csItem, _T("Description"), pActionItem->GetDescription());
                    csItem = UpdateNode(csItem, _T("NettoPrice"), pActionItem->GetNetPrice());
                    csItem = UpdateNode(csItem, _T("Timestamp"), csTime);
                    csItem = UpdateNode(csItem, _T("ItemProperty"), pActionItem->GetItemProperty());
                    
                    bstrItemGroup += (LPCTSTR)csItem;
                    nPositionNum++;
                }
            }
            delete pActionItem;
            pActionItem = NULL;
        }
    }

    int nAuditItems = GetAuditItemList(nPositionNum, bstrAuditItems );
    m_lNumberOfItems += nAuditItems;
    bstrItemGroup += bstrAuditItems;
    bstrItemGroup += _T("</ItemGroup>");

    

    trace(L6, _T("-CMobileCartParser::buildItemList()"));

    return bstrItemGroup;
}

int CMobileCartParser::GetAuditItemList(int & nPositionNum,  bstr_t & bstrAuditItems )
{
    int nAuditItems = mobileAudit.GetNumberOfAuditFailureItems();
    CTime ctTime = CTime::GetCurrentTime();
    CString csTime = ctTime.Format(_T("%Y%m%d%H%M%S"));
    
    for (int i = 0; i<nAuditItems; i++)
    {
        IXMLDOMNodePtr pItemNode;
        CQueuedActionRescanItem* pActionItem = (CQueuedActionRescanItem*) mobileAudit.GetNextAuditFailureItem();
        
        if (pActionItem)
        {
            CString csFailureReason = pActionItem->GetResponseErrorString();
            //if ((csFailureReason.Compare(_T("")) == 0) || (csFailureReason.Compare(_T("")) == 0))
            long lQty = pActionItem->GetQty() < 2 ? 1 : pActionItem->GetQty();
            
            
            for (int j = 1; j<=lQty; j++)
            {
                CString csItem = _T("<Item></Item>");
                csItem = UpdateNode(csItem, _T("Origin"), pActionItem->GetOrigin());        
                csItem = UpdateNode(csItem, _T("PositionNumber"), nPositionNum);
                csItem = UpdateNode(csItem, _T("Barcode"), pActionItem->GetScanDataLabel());
                csItem = UpdateNode(csItem, _T("Description"), pActionItem->GetDescription());
                csItem = UpdateNode(csItem, _T("NettoPrice"), pActionItem->GetNetPrice());
                csItem = UpdateNode(csItem, _T("Timestamp"), csTime);
                csItem = UpdateNode(csItem, _T("ItemProperty"), pActionItem->GetItemProperty());
                csItem = UpdateNode(csItem, _T("RescanItem"), pActionItem->GetRescanItem());
                
                bstrAuditItems += (LPCTSTR)csItem;
                nPositionNum++;
            }
        }
        delete pActionItem;
        pActionItem = NULL;
    }
    return nAuditItems;
}

