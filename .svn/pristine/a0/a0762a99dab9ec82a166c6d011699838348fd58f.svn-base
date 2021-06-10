// RecallList.cpp: implementation of the CRecallList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include <ctime>
//#include <algorithm>

//UploadTicket Port +
#include "TraceObject.h"
#include "FastlaneUtils.h"
#include "RecallList.h"
#include "MobileConstantsSSF.h"
//UploadTicket Port - 


//UploadTicket Port - Entire file is heavily modified 
#define COMP_ID ID_CU               // base state component
#define T_ID    _T("CRecallList")
CRecallList myCRecallList;
DLLEXPORT CRecallList &recallList = myCRecallList;

DLLIMPORT extern ItemObject io;   //UploadTicket Port 


//#define QB_OUTFILEPREFIX _T("QB_")  //Beginning of output file name.
//#define QB_OUTFILEEXTENSION _T(".txt")

/* This file is documented with comment fields that are usable by Doxygen */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

///  Class constructor. 
CRecallList::CRecallList()
{
    m_MobileRecallQueue = new CQueuedActionsContainer();
    m_MobileItemSoldList = new CQueuedActionsContainer();
    pCurrentItem=NULL;
    Reset();
}

///Class destructor
CRecallList::~CRecallList()
{
    Reset();
    delete m_MobileRecallQueue;
    m_MobileRecallQueue=NULL;
    delete m_MobileItemSoldList;
    m_MobileItemSoldList=NULL;
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

void CRecallList::Initialize(CString csTerminalNumber)
{
    CRecallListBase::Initialize(csTerminalNumber);
    Reset();
}


/**
* \brief Reset clears the item queue and resets transaction variables. 
* 
*  Reset should be called after each transaction.
*/

void CRecallList::Reset()
{
    if (pCurrentItem)
    {
        delete pCurrentItem;
        pCurrentItem=NULL;
    }
    m_MobileRecallQueue->Clear();
    m_MobileItemSoldList->Clear();
    
    m_csCurrentScanData = _T("");
    m_csCurrentScanDataLabel = _T("");
    m_csCurrentDescription = _T("");
    m_nCurrentBarcodeType = 0;
    m_nCurrentBarcodeQuantity = 0;
    m_bMobileTransaction = false;
    m_nNumItemsTotal = 0;
    m_nNumListItems = 0;
    CTime t = CTime::GetCurrentTime();
    srand(time(NULL));
}

///Uninitialize calls reset to clear the item queue and reset internal variables.
void CRecallList::Uninitialize()
{
    CRecallListBase::Uninitialize();
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
bool CRecallList::LoadQBBarcodes(const CString& csItemsBarcode)
{
    return true;  //Do nothing
}


/**
* \brief Returns ScanData field for current item.
*
* GetScanData() returns the ScanData field for the current barcode, 
* which is the full barcode including barcode type indicator.\n
*/
CString CRecallList::GetScanData()
{
    return m_csCurrentScanData;
}

/**
* \brief Returns ScanDataLabel field for current item.
*
* Returns the ScanDataLabel field for the current barcode,  which is the barcode without the barcode type.\n
*/
CString CRecallList::GetScanDataLabel()
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
int CRecallList::GetBarcodeType()
{
    return m_nCurrentBarcodeType;
}

/**
* \brief Returns the quantity of the current barcode.
*
*/ 
int CRecallList::GetBarcodeQuantity()
{
    return m_nCurrentBarcodeQuantity;
}

long CRecallList::GetUnitPrice()
{
    return CRecallListBase::GetUnitPrice();
}

long CRecallList::GetExtendedPrice()
{
    return CRecallListBase::GetExtendedPrice();
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
bool CRecallList::ParseNextItem(bool bPopItem)
{
    bool bReturn = false;
    m_nCurrentBarcodeType = 0;
    m_csCurrentScanData = _T("");
    m_csCurrentScanDataLabel = _T("");

    if (m_MobileRecallQueue->GetSize() == 0 )
    {
        return false;
    }
    
    if (bPopItem)
    {
        pCurrentItem = (CQueuedActionRecallItem*) m_MobileRecallQueue->GetNext();
    }
    else
    {
        pCurrentItem = (CQueuedActionRecallItem*) m_MobileRecallQueue->Peek();
    }

    if (pCurrentItem)
    {
        m_nCurrentBarcodeType = pCurrentItem->GetBarcodeType();
        m_csCurrentScanData = pCurrentItem->GetScanData();
        m_csCurrentScanDataLabel = pCurrentItem->GetScanDataLabel();
        m_nCurrentBarcodeQuantity = 1;  //Currently no quantities sent by server
        m_csCurrentDescription = pCurrentItem->GetDescription();
        bReturn = true;
    }


   // bReturn = DecodeBarcode(csBarcode);
    if (bPopItem)
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
bool CRecallList::DecodeBarcode(CString csBarcode)
{
    return CRecallListBase::DecodeBarcode(csBarcode);
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
bool CRecallList::SplitString(CString csList, CString csDelim, CStringArray* pcsaOutputArray)
{
    return CRecallListBase::SplitString(csList, csDelim, pcsaOutputArray);
}



bool CRecallList::Load_FMT_001(CString csItemsBarcode)
{
    return CRecallListBase::Load_FMT_001(csItemsBarcode);
}

bool CRecallList::IsMobileTransaction()
{
    return m_bMobileTransaction;
}

void CRecallList::QueueMobileItem(IQueuedAction* pMobileItem)
{
    m_MobileRecallQueue->AddReplace(pMobileItem);
}

void CRecallList::QueueMobileItemSold(CString csEntryID, CString csTrimmedItemCode)
{
    CQueuedActionRecallItem* pItem = pCurrentItem;
    if (pCurrentItem)
    {
        pCurrentItem->SetActionID(csEntryID);
        pCurrentItem->SetDescription(io.d.csDescription);
        pCurrentItem->SetNetPrice(io.d.lExtPrice);
        pCurrentItem->SetAgeRestricted(io.d.nRestrictedAge != 0);
        m_MobileItemSoldList->AddReplace(pCurrentItem);
        pCurrentItem = NULL;
    }
    else
    {     
        CQueuedActionRecallItem* pNewItem = 
            new CQueuedActionRecallItem(csEntryID, csTrimmedItemCode, io.csOrigCode, io.cScanType);
        CQueuedActionRecallItem* pOldItem = (CQueuedActionRecallItem*) m_MobileItemSoldList->Peek(csEntryID);
        if (pOldItem)
        {
            pNewItem->SetOrigin(pOldItem->GetOrigin());
        }
        else
        {
            pNewItem->SetOrigin(MOBILE_ORIGIN_POS);
        }
        pNewItem->SetDescription(io.d.csDescription);
        pNewItem->SetNetPrice(io.d.lExtPrice);
        pNewItem->SetAgeRestricted(io.d.nRestrictedAge != 0);
        pNewItem->SetQty(io.d.lQtySold);
        if (io.d.fVoid)
        {
            pNewItem->SetItemProperty(MOBILE_ITEMPROP_VOIDED);
        }
        else
        {
            pNewItem->SetItemProperty(MOBILE_ITEMPROP_ADDITIONALSALE);
        }
        
        m_MobileItemSoldList->AddReplace(pNewItem);    
    }
}

IQueuedAction* CRecallList::GetNextSoldItem(bool bPopItem)
{
    return m_MobileItemSoldList->GetNext();
}

int CRecallList::GetNumRecallItems() 
{ 
    int nSize=0;
    if (m_MobileRecallQueue)
    {
       nSize=m_MobileRecallQueue->GetSize();
    }
    return nSize;
}

int CRecallList::GetNumSoldItems()
{
    int nSize=0;
    if (m_MobileItemSoldList)
    {
       nSize = m_MobileItemSoldList->GetSize();
    }
    return nSize;
}
