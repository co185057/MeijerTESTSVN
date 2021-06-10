// RecallListBase.cpp: implementation of the CRecallListBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ctime>
#include <algorithm>

#ifndef _CPPUNIT
#include "TraceObject.h"
#include "FastlaneUtils.h"
#endif

#include "RecallListBase.h"

#define COMP_ID ID_CU               // base state component
#define T_ID    _T("CRecallListBase")


#define QB_OUTFILEPREFIX _T("QB_")  //Beginning of output file name.
#define QB_OUTFILEEXTENSION _T(".txt")

/* This file is documented with comment fields that are usable by Doxygen */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///  Class constructor. 
CRecallListBase::CRecallListBase()
{
    Reset();
}

///Class destructor
CRecallListBase::~CRecallListBase()
{
    Reset();
}

/** 
* \brief Initialize reads configuration parameters from ScotSSFConfig.ini and saves the terminal number.
 
* 
* The terminal number is saved in a protected member variable.  
* The method also opens the ScotSSFConfig.ini from the system's config directory and
* reads these parameters from the [QueueBuster] section:\n
*   TransactionFileDirectoryOut  - the path of where to save the transaction data files.
*       (The method will append a "\" if none is provided.)\n
*
* \param csTerminalNumber The terminal number of the lane.
*/

void CRecallListBase::Initialize(CString csTerminalNumber)
{
    m_csTerminalNumber = csTerminalNumber;
    Reset();

}

/**
* \brief Reset clears the item queue and resets transaction variables. 
* 
*  Reset should be called after each transaction.
*/

void CRecallListBase::Reset()
{
    while ( !g_RecallItemQueue.empty() )
    {
        g_RecallItemQueue.pop();
    }
                
    m_bItemListLoaded = false;
    m_csListID = _T("");  //Unique ID of last list that was loaded
    m_csCurrentScanData = _T("");
    m_csCurrentScanDataLabel = _T("");
    m_nCurrentBarcodeType = 0;
    m_nCurrentBarcodeQuantity = 0;
    m_lCurrentUnitPrice = 0;
    m_csFileName = _T("");
    m_bMobileTransaction = false;
    m_nNumItemsTotal = 0;
    m_nNumListItems = 0;
    m_bPriceEmbedded = false;
    CTime t = CTime::GetCurrentTime();
    srand(time(NULL));

}

///Uninitialize calls reset to clear the item queue and reset internal variables.
void CRecallListBase::Uninitialize()
{
    Reset();
}

/**
* \brief Parses 2D transaction barcode
*
* LoadQBBarcodes parses a formatted string containing items for a transaction 
* and loads them into an internal queue.  The barcode string will be parsed according to the
* initial format indicator in the string by dedicated methods.  If the load succeeds, the data is
* saved to the output file for the transaction. \n
* Currently recognized format versions: \n
*    "NCR_QB" - Format one,  handled by Load_FMT_001() method.
*  
** \param csItemsBarcode text of the 2D barcode scan data containing list of items to sell.
*/
bool CRecallListBase::LoadQBBarcodes(const CString& csItemsBarcode)
{
    m_bItemListLoaded = false;
    m_nNumListItems = 0;
    trace(L6, _T("+CRecallListBase::LoadQBBarcodes"));
    
    if (csItemsBarcode.Find(QBFMT_001)==0)
    {
        m_bItemListLoaded = Load_FMT_001(csItemsBarcode);
    }

    trace(L6, _T("-CRecallListBase::LoadQBBarcodes"));
    if (m_bItemListLoaded)
    {
        m_bMobileTransaction = true;
        m_nNumListItems = g_RecallItemQueue.size();
//        SaveTransactionData(csItemsBarcode + _T("\r\n"));
    }

    return m_bItemListLoaded;
}

/**
* \brief Returns ScanData field for current item.
*
* GetScanData() returns the ScanData field for the current barcode, 
* which is the full barcode including barcode type indicator.\n
*/
CString CRecallListBase::GetScanData()
{
    return m_csCurrentScanData;
}

/**
* \brief Returns ScanDataLabel field for current item.
*
* Returns the ScanDataLabel field for the current barcode,  which is the barcode without the barcode type.\n
*/
CString CRecallListBase::GetScanDataLabel()
{
    return m_csCurrentScanDataLabel;
}

/**
* \brief Returns the OPOS code for the current barcode type.
*
* GetBarcodeType() the OPOS code for the current barcode type. \n
* Currently supported barcode types are:\n
*    SCAN_SDT_UPCA  (UPC-A)
*    SCAN_SDT_UPCE  (UPC-E)
*    SCAN_SDT_EAN8  (EAN-8)
*    SCAN_SDT_EAN13 (EAN-13)
*/ 
int CRecallListBase::GetBarcodeType()
{
    return m_nCurrentBarcodeType;
}

/**
* \brief Returns the quantity of the current barcode.
*
*/ 
int CRecallListBase::GetBarcodeQuantity()
{
    return m_nCurrentBarcodeQuantity;
}

long CRecallListBase::GetUnitPrice()
{
    return m_lCurrentUnitPrice;
}

long CRecallListBase::GetExtendedPrice()
{
    return m_lCurrentExtendedPrice;
}


/**
* \brief Parses item at top of queue into ScanData, ScanDataLabel, and barcode type fields.
*
* ParseNextItem retrieves item from item queue and calls DecodeBarcode() to parse item. 
* It then optionally pops the item from the queue.\n
* \param bool bPopItem indicates whether or not item should popped from the item queue when parsed. 
* This option is set to false for functions like loyalty pre-lookup,  which checks the item but leaves it
* in place for the item sale later.
* \return boolean true if item exists and can be parsed.\n
*                 false otherwise.
*/
///Returns true after calling DecodeBarcode on item on top of item queue,  pops item if requested. Returns false if the queue is empty.
bool CRecallListBase::ParseNextItem(bool bPopItem)
{
    bool bReturn;
    m_nCurrentBarcodeType = 0;
    m_lCurrentUnitPrice = 0;
    m_csCurrentScanData = _T("");
    m_csCurrentScanDataLabel = _T("");

    if (g_RecallItemQueue.empty() )
    {
        return false;
    }
    
    CString csBarcode = g_RecallItemQueue.front();
    if (bPopItem)
    {
        g_RecallItemQueue.pop();
    }
    bReturn = DecodeBarcode(csBarcode);
    if (bReturn && bPopItem)
    {
        if (m_nCurrentBarcodeQuantity)
        {
            m_nNumItemsTotal += m_nCurrentBarcodeQuantity;
        }
        else
        {
            m_nNumItemsTotal++;
        }
    }

    return bReturn;

}

/**
* \brief Populates the ScanData, ScanDataLabel, and BarcodeType fields. 
*
* DecodeBarcode() \n
* \param csBarcode Individual item barcode from the item queue.
* The expected format is scan code:quantity:extended price:unit price:attributes
* Attributes recognized:
*   e = Price Embedded
*/
///
bool CRecallListBase::DecodeBarcode(CString csBarcode)
{
    CString csWork = csBarcode;
    CString csTemp;

    trace(L6, _T("RecallListBase::DecodeBarcode %s"), csBarcode);

    m_nCurrentBarcodeType = 0;
    m_nCurrentBarcodeQuantity = 1;
    m_lCurrentUnitPrice = 0;
    m_lCurrentExtendedPrice = 0;
    m_bPriceEmbedded = false;

    int nPos = csWork.Find(_T(":"));

    // Check if barcode specified
    if (nPos > -1)
    {
        m_csCurrentScanData = csWork.Mid(0, nPos); //Parse barcode 

        csWork = csWork.Mid(nPos+1);

        // Check if quantity specified
        nPos = csWork.Find(_T(":"));

        if (nPos > -1)
        {
            csTemp = csWork.Mid(0, nPos);
            if (csTemp.GetLength() > 0)
            {
                m_nCurrentBarcodeQuantity = _ttoi(csTemp); 
            }

            csWork = csWork.Mid(nPos + 1);

            // Check if extended price specified
            nPos = csWork.Find(_T(":"));

            if (nPos > -1)
            {
                csTemp = csWork.Mid(0, nPos);
                if (csTemp.GetLength() > 0)
                {
                    m_lCurrentExtendedPrice = _ttoi(csTemp);
                }

                csWork = csWork.Mid(nPos + 1);

                
                // Check if unit price specified
                nPos = csWork.Find(_T(":"));

                if (nPos > -1)
                {
                    csTemp = csWork.Mid(0, nPos);
                    if (csTemp.GetLength() > 0)
                    {
                        m_lCurrentUnitPrice = _ttoi(csTemp);
                    }

                    csWork = csWork.Mid(nPos + 1);


                    // Check if item attributes specified
                    nPos = csWork.Find(_T(":"));

                    if (nPos > -1)
                    {
                        csTemp = csWork.Mid(0, nPos);
                        if (csTemp.Find(_T("e")) > -1)
                        {
                            trace(L6, _T("CRecallListBase::DecodeBarcode - Price embedded item detected %s"), (LPCTSTR)m_csCurrentScanData);
                            m_bPriceEmbedded = true;
                        }
                        csWork = csWork.Mid(nPos + 1);

                        // Check if scan type specified
                        nPos = csWork.Find(_T(":"));

                        if(nPos > -1)
                        {
                            csTemp = csWork.Mid(0, nPos);
                            if(csTemp.GetLength() > 0)
                            {
                                m_nCurrentBarcodeType = _ttoi(csTemp);
                            }
                            csWork = csWork.Mid(nPos + 1);

                            // Check if original  Barcode specified
                            nPos = csWork.Find(_T(":"));
                            if(nPos > -1)
                            {
                                csTemp = csWork.Mid(0, nPos);
                                if(csTemp.GetLength() > 0)
                                {
                                    m_csCurrentScanDataLabel = csTemp;
                                }
                            }
                            else if (csWork.GetLength() > 0)
                            {
                                m_csCurrentScanDataLabel = csWork;
                            }
                        }
                    }

                }
            }
        }
    }
    else
    {
        m_csCurrentScanData = csWork;
    }

    trace(L6, _T("RecallListBase::DecodeBarcode m_csCurrentScanData = %s m_csCurrentScanDataLabel = %s m_nCurrentBarcodeType = %s"), GetScanData(), GetScanDataLabel(), GetBarcodeType());

     
    return true;
}

/**
* \brief split strings into a CStringArray based on a given delimitor.
*
* Internal helper method that splits a string to an Output Array using the supplied delimiter\n
* \param csList The string to split
* \param csDelim The delimiter
* \param pcsaOutputArray 
* \returns true if the operation succeeded.
*/
bool CRecallListBase::SplitString(CString csList, CString csDelim, CStringArray* pcsaOutputArray)
{
    int iPosition, iLen;
    while(!csList.IsEmpty())
    {
        iPosition = csList.Find(csDelim);
        if(iPosition == -1)
        {
            iPosition = csList.GetLength();
            iLen = iPosition;
        }
        else
            iLen = iPosition + 1;
        pcsaOutputArray->Add(csList.Left(iPosition));
        csList.Delete(0, iLen);
    }
     if(pcsaOutputArray->GetSize() > 0)
         return true;
     else
         return false;
}



/**
* \brief Parses a barcode with the "NCR_QB" format version.
*
* Parses a string with the "NCR_QB" format version into the internal item queue. \n\n
* Format:\n
* FormatVersionIndictor|UniqueID|BarcodeList|ExtraData....  \n
* The FormatVersionIndicator will be NCR_QB\n
* The UniqueID should be an alphanumeric string used to uniquely identify the transaction barcode,  
* such as a timestamp.  It will be used to prevent double-scans of the barcode during the same 
* transaction.  The last ID scanned will be remembered,  and the user will be unable to run 
* the same transaction barcode back-to-back within a transaction.  The identifier should not 
* contain "|" characters,  as all text between the first two "|" will be used as the ID.\n
* The Barcode list will contain a ^-separated list of EAN-13, EAN-8, UPC-A, or UPC-E barcodes which include the NCR-format barcode type identifiers. \n
*   UPC-A:  "A", or 0x41 \n
*   UPC-E:  "E", or 0x45 \n
*   EAN-8:  "FF", or 0x46, 0x46  \n
*   EAN-13:  "F", or 0x46 \n\n
* Example: \n
* NCR_QB|0004996953|A030000070604^A036200000502:4^A086414001161|Other Text... \n
* \param csItemsBarcode string containing barcode data to be parsed.
*/
bool CRecallListBase::Load_FMT_001(CString csItemsBarcode)
{
    bool bItemListLoaded = false;
    CString csItemList = _T("");
    CString csListID = _T("");
    int nPOS = csItemsBarcode.Find(_T("|"));
    if (nPOS > -1)
    {
        csListID = csItemsBarcode.Mid(nPOS + 1);
        
        nPOS = csListID.Find(_T("|"));
        if (nPOS > -1)
        {
            csItemList = csListID.Mid(nPOS + 1);
            csListID = csListID.Left(nPOS);
        }
        if (csListID != m_csListID)
        {
            nPOS = csItemList.Find(_T("|"));
            if (nPOS > -1)
            {
                csItemList = csItemList.Left(nPOS);
            }
        }
        else
        {
            csItemList = _T("");  //Don't load item list if we've double-scanned
        }
        
    }
    
    if (!csItemList.IsEmpty())
    {
        CStringArray csaItemData;
        SplitString(csItemList,_T("^"),&csaItemData);
        
        if(csaItemData.GetSize() >= 1)
        {

            for (int i = 0; i<csaItemData.GetSize(); i++)
            {
                CString csItem = csaItemData[i];
                csItem.TrimLeft();
                csItem.TrimRight();
                g_RecallItemQueue.push(csItem);
            }
            m_csListID = csListID;
            bItemListLoaded = true;

        }
    }
    return bItemListLoaded;
}

bool CRecallListBase::IsMobileTransaction()
{
    return m_bMobileTransaction;
}