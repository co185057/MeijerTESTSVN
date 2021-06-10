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
//#include "..\SSCOMobile\SSFCommonMinimal.h"
#ifndef _CPPUNIT
#include "TraceObject.h"
#endif

#include "MobileCartParserBase.h"
#include "MobileConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMP_ID ID_CU               // Main Procedure
#define T_ID _T("CMobileCartParserBase")

// Event ID definitions
#define LONG_POLL_ID    1


IMPLEMENT_DYNCREATE(CMobileCartParserBase, CObject)// MFC Runtime class/object information

///////////////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////////////

/**  
Constructor for CMobileCartParserBase object
*/
CMobileCartParserBase::CMobileCartParserBase(): m_bInitialized(false)
{
    pCartDocument=NULL;
    pCartItemGroup=NULL;
    m_bSendCustomerIDAsScan=true;
}

///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////

/// Destructor
/**  
Destructor for CMobileCartParserBase object
*/
CMobileCartParserBase::~CMobileCartParserBase()
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
void CMobileCartParserBase::UnInitialize()
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
long CMobileCartParserBase::Initialize()
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

void CMobileCartParserBase::Reset()
{
    m_lTotalDue = 0;
    m_nTransactionNum = 0;
    m_bstrCartID = _T("");

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


void CMobileCartParserBase::GetItems(int &nValidItems, CString &str)
{      
    trace(L7, _T("+CMobileCartParserBase::GetItems"));
    long lQuantity;
    CString csQuantity;
    CString csWork;
    CString csTemp;
    CString csAttributes;
    CString csReScan;
    CString csScanType;
    CString csOrgBarcode;
    MSXML2::IXMLDOMNodePtr pNode;
    MSXML2::IXMLDOMNodePtr pItem;
    MSXML2::IXMLDOMNodeListPtr pItems;

    // Get items and add to map (LATER - For now just build CString and send)
    pItems = pCartItemGroup->selectNodes(_T("//ItemGroup/Item"));
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
                
                
                pNode = pItem->selectSingleNode(_T("RescanItem"));
                if (pNode != NULL)
                {
                    CString csRescan = (LPCTSTR)pNode->text;
                    long lRescan = _ttol(csRescan);
                    if (lRescan ==2)
                    {
                        bProcessItem = false;
                    }  
                }


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

                
                // Get quantity
                lQuantity = 1;
                
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
                
                
                
                // Add item to output string
                if (bProcessItem)
                {  
                    if (nValidItems > 0)
                    {
                        str += _T("^");
                    }
                    nValidItems++;
                    str += csItemCode + _T(":") + csQuantity + _T(":") + csAttributes + _T(":") + csAttributes + _T(":") + csAttributes + _T(":") + csScanType + _T(":") + csOrgBarcode;
                }
            }
            else
            {
                trace(L6, _T("No item code found in cart for item %d"), i);
            }
        }
    }
    trace(L7, _T("-CMobileCartParserBase::GetItems"));
} 
///  ProcessCart
/**
This method translates the externally received mobile cart into an internal format in order for the
items to be sold within a new transaction on this lane.
   @param[in] pCartDocument A pointer to the XML document containing the mobile cart.
   @param[out] retVal An indication as to whether or not the XML contained a valid cart and it was sent internally
*/
CString CMobileCartParserBase::ProcessCart(_bstr_t bstrCartXML)
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
            pItemGroup =  pTemp->cloneNode(VARIANT_TRUE);
            _bstr_t    bstrTest = pItemGroup->Getxml();
            trace(L7, _T("pItemGroup = \n%s"), (LPCTSTR) bstrTest);
            pCartItemGroup->appendChild(pItemGroup);
        }

        CString str = _T("NCR_QB|");
        
        // Add unique GUID
        str += (LPCTSTR)m_bstrCartID;
        str += _T("|");
        
        // If no cart we have no idea what this is
        if (pRescan)
        {
            CString csText = (LPCTSTR) pRescan->text;
            if (csText.Find(_T("true")) != -1)
            {
                trace(L6, _T("ProcessCart - Sending RescanRequired"));
                return MOBILE_RESCAN_REQUIRED;
            }
        }
        else
        {
//            m_bIsAuditRequired = false;
        }
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
                    str += csLoyaltyID +_T(":::::0:") + csLoyaltyID;
                    nValidItems++;
                }
            }
        }
        
        
        GetItems(nValidItems, str);


        // Add string terminator
        str += _T("|");
        trace(L7, _T("ProcessCart - Posting mobile cart message %s"), (LPCTSTR)str);
        
        
//        mo.PostCU(MOBILECART_SALESTARTED, str);
        return str;
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
void CMobileCartParserBase::SendItemSold(ITEMDETAILS &itemDetails, bool bRecalledItem)
{
    trace(L6, _T("+SendItemSold()"));

    _bstr_t str= updateItemMap( itemDetails.lEntryID, 
                   itemDetails.fVoid, 
                   itemDetails.csItemCode,
                   itemDetails.lExtPrice, 
                   itemDetails.csDescription,
                   bRecalledItem);

    // TODO:: Send item message?

    trace(L6, _T("-SendItemSold()"));
}



void CMobileCartParserBase::SetTotalDue(long lTotal)
{
    m_lTotalDue = lTotal;
}

void CMobileCartParserBase::SetPOSTransactioNum(int nTransactionNumber)
{
    m_nTransactionNum = nTransactionNumber;
}

void CMobileCartParserBase::CopyNode(MSXML2::IXMLDOMDocumentPtr pDoc, MSXML2::IXMLDOMNodePtr pContainer, MSXML2::IXMLDOMNodePtr pNewContainer, LPCTSTR sNodeName, LPCTSTR sNodeValue)
{
    try
    {
        
        MSXML2::IXMLDOMNodePtr pNode = pContainer->selectSingleNode(sNodeName);
        if (pNode == NULL)
        {
            CString csNodeName(sNodeName);
            csNodeName.Replace(_T("//"), _T(""));
            pNode = pDoc->createElement((LPCTSTR) csNodeName);
            pNode->text = sNodeValue;
        }
        pNewContainer->appendChild(pNode);
    }
    catch (_com_error ce) 
    {
        HRESULT hr = ce.Error();
        CString csMesg = ce.ErrorMessage();
        CString csErrorCode;
        csErrorCode.Format(_T("%x"),hr);
        _bstr_t test = ce.Description();
        
        trace(L6, _T("CopyNode - HRESULT=%x, ErrorMessage=%s, Desc=%s"),
            hr, csMesg, test);
    }
    catch(...)
    {
        trace( L6, _T("-CopyNode Caught Unknown Exception"), -1);
    }
    
}


CString CMobileCartParserBase::UpdateNode(CString csXML, CString csNodeName, CString csNodeValue)
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

CString CMobileCartParserBase::UpdateNode(CString csXML, CString sNodeName, long lNodeValue)
{
    CString csValue;
    csValue.Format(_T("%d"), lNodeValue);
    return UpdateNode(csXML, sNodeName,  csValue);
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


_bstr_t CMobileCartParserBase::GetFinishRequest()
{
    //UploadTicket

    trace(L6, _T("+GetFinishRequest()"));
    _bstr_t bstrReturn = _T("");
    _bstr_t bstrNewCart = _T("");
    if( pCartDocument != NULL)
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

        MSXML2::IXMLDOMNodePtr pHeader = pCartDocument->selectSingleNode(_T("//Header"));

        CString csHeaderXML = (LPCTSTR) pHeader->Getxml();
        csHeaderXML = UpdateNode(csHeaderXML, _T("Timestamp"), csTime);
        csHeaderXML = UpdateNode(csHeaderXML, _T("Origin"), MOBILE_ORIGIN_POS);

        MSXML2::IXMLDOMNodePtr pItemList = pCartItemGroup->selectSingleNode(_T("//ItemGroup"));

        bstrNewCart = _T("<Document>") + csHeaderXML + _T("<ItemGroup>");
        
        MSXML2::IXMLDOMNodePtr pNode;
        MSXML2::IXMLDOMNodePtr pItem;
        MSXML2::IXMLDOMNodeListPtr pItems;
        
        // Get items and add to map (LATER - For now just build CString and send)
        pItems = pCartItemGroup->selectNodes(_T("//ItemGroup/Item"));
        if (pItems != NULL)
        {
            int count = pItems->length;
            int i;
            
            for (i = 0; i < count; i++)
            {
                pItem = pItems->item[i];
                bool bProcessItem = true;
                CString csItem = (LPCTSTR) pItem->Getxml();
                csItem = UpdateNode(csItem, _T("Origin"), MOBILE_ORIGIN_POS);
                bstrNewCart += (LPCTSTR) csItem;
            }
        }
        bstrNewCart = bstrNewCart + _T("</ItemGroup>");
        bstrNewCart = bstrNewCart + csFooterXML + _T("</Document>");

        bstrReturn = GetPrettyXML(bstrNewCart);

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
_bstr_t CMobileCartParserBase::updateItemMap(long id, 
                                         bool bVoid,
                                         const CString &upc,
                                         long lextendedPrice, 
                                         const CString &sDesc,
                                         bool bRecalledItem)
{
    _bstr_t str = _T("");

    //TODO::Save item message for retrieval as correctly formatted string later.
/*
    if (bVoid)
    {
        // remove from list 
        ItemSoldMap::iterator iter= m_itemsMap.find(id);
        if (iter != m_itemsMap.end())
        { 
            m_itemsMap.erase(iter);
        }
    }
    else
    {
        // add to list
        gto::IGTGenObjPtr pNewItem(__uuidof(gto::CGTGenObj));  // smart pointer. will remain in map until unreferenced
        pNewItem->IsNamedElementsXML = true;
        pNewItem->IsXMLOutput = true;

        pNewItem->Name=_T("item");
        pNewItem->SetValue(_T("entryId"), id);
        pNewItem->SetValue(_T("upc"), (LPCTSTR) upc);
        pNewItem->SetValue(_T("description"), (LPCTSTR) sDesc);
        pNewItem->SetValue(_T("price"), lPrice);

        pNewItem->SetValue(_T("isVoid"), false);
        pNewItem->SetValue(_T("quantity"), lQuantity);
        pNewItem->SetValue(_T("weight"), lWeight);
        pNewItem->SetValue(_T("extendedPrice"), lextendedPrice);
        
        m_itemsMap[id]= pNewItem;
        str = pNewItem->xml;
    }
*/
    return str;
}

_bstr_t CMobileCartParserBase::serializeItemMap() 
{
    _bstr_t str = _T("<ItemGroup></ItemGroup>");
    /*
    for (ItemSoldMap::iterator iter= m_itemsMap.begin(); 
        iter != m_itemsMap.end();
        iter++) {
            str +=(*iter).second->xml;
        }
    str+= _T("</items>");
    */
    return str;
}

void CMobileCartParserBase::SetSendCustomerIDAsScan(bool bSendCustomerIDAsScan)
{
    m_bSendCustomerIDAsScan = bSendCustomerIDAsScan;
}

bool CMobileCartParserBase::SetCustomerIDFilter(int nIndex, CString csFilter)
{
    trace(L6, _T("CMobileCartParserBase::SetCustomerIDFilter  %d, %s"), nIndex, csFilter);
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

bool CMobileCartParserBase::IsValidCustomerID(CString csCustomerID)
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

bool CMobileCartParserBase::IsFilterMatch(CString csFilterString, CString csCompareString)
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
// Method:      void CMobileCartParserBase::AddBarCodeConversionOptionToMap
//
// Purpose:     The method add the Mobile Barcode Conversion option-value pair in the map
//
// Parameters:  LPCTSTR szKey   - the key which is MobileShopperBarcodeConversion Barcode Type 
//              CString csValue - the barcode prefix
//
// Returns:     void
//
/////////////////////////////////////////////////////////////////////////////////////
void CMobileCartParserBase::AddBarCodeConversionOptionToMap(LPCTSTR szKey, CString csValue)
{
    trace(L6, _T("+CMobileCartParserBase::AddBarCodeConversionOptionToMap()"));

    ca_map[szKey] = csValue;

    trace(L6, _T("-CMobileCartParserBase::AddBarCodeConversionOptionToMap() BarCodeType = %s, BarcodePrefix = %s"), szKey, csValue);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Method:      bool CMobileCartParserBase::ClearBarCodeConversionOptionMap
//
// Purpose:     The method removes all entries of the map (empties it)
//
// Parameters:  void
//
// Returns:     BOOL - returns true if map is empty, false if not
//
/////////////////////////////////////////////////////////////////////////////////////
bool CMobileCartParserBase::ClearBarCodeConversionOptionMap()
{
    bool fRet; 

    trace(L6, _T("+CMobileCartParserBase::ClearBarCodeConversionOptionMap()"));

    ca_map.RemoveAll();

    fRet = ca_map.IsEmpty();

    trace(L6, _T("-CMobileCartParserBase::ClearBarCodeConversionOptionMap() returns = %d"), fRet);

    return fRet;
}