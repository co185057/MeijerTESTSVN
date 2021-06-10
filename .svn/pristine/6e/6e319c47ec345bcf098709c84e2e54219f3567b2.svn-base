// SolutionMultiBitmap.cpp: implementation of the CSolutionMultiBitmap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionMultiBitmap.h"
#include "CustomerMHOptions.h"

#define T_ID  _T("CSolutionMultiBitmap")

CSolutionMultiBitmap::CSolutionMultiBitmap()
{

}

CSolutionMultiBitmap::~CSolutionMultiBitmap()
{

}

bool CSolutionMultiBitmap::IsEnabled()
{
	CString csEnabled = CCustomerMHOptions::instance()->OptionValue(_T("PrinterMultiBitmapEnabled"));
    csEnabled.TrimLeft();
    csEnabled.TrimRight();
    if( !csEnabled.Compare(_T("1")) )
    {
	    tbtrace(TM_INFO, _T("IsEnabled: Yes"));
        return true;
    }
    else
    {
        tbtrace(TM_INFO, _T("IsEnabled: No"));
        return false;
    }
}

bool CSolutionMultiBitmap::SyncRAP()
{
	CString csSyncRAP = CCustomerMHOptions::instance()->OptionValue(_T("PrinterMultiBitmapSyncRAP"));
    csSyncRAP.TrimLeft();
    csSyncRAP.TrimRight();
    if( !csSyncRAP.Compare(_T("1")) )
    {
	    tbtrace(TM_INFO, _T("SyncRAP: Yes"));
        return true;
    }
    else
    {
        tbtrace(TM_INFO, _T("SyncRAP: No"));
        return false;
    }
}

bool CSolutionMultiBitmap::Init()
{
    tbtraceIO( _T("CSolutionMultiBitmap::Init"));
    CSingleLock myLock(&m_critDoc);
    bool bRet = false;
    try
    {
        HRESULT hr = S_OK;
        _bstr_t bstrFile = 
            CCustomerMHOptions::instance()->OptionValue(_T("PrinterMultiBitmapDataFile"));    
        
        m_spDoc.CreateInstance(__uuidof(DOMDocument40));
        m_spDoc->async = VARIANT_FALSE;
        
        try
        {
            VARIANT_BOOL vt;
            hr = m_spDoc->raw_load(_variant_t(bstrFile), &vt);

            if( hr == S_FALSE )
            {
                // the file does not exist
                tbtrace(TM_INFO, _T("Data file does not exist. Creating a new %s file."), (LPCTSTR)bstrFile );
                CreateDoc();
            }
        }
        catch(_com_error & cex)
        {
            _ASSERT(FALSE);
            tbtrace(TM_ERROR, _T("A COM exception occured loading the data file. Error [0x%08X]: %s."), cex.Error(), cex.ErrorMessage());
            tbtrace(TM_INFO, _T("Creating a new %s file. All previous data (if any) will be cleared."), (LPCTSTR)bstrFile );
            CreateDoc();
        }
        bRet = true;
    }
    catch (_com_error & cex)
    {
        _ASSERT(FALSE);
        tbtrace(TM_ERROR, _T("A COM exception occured. Error [0x%08X]: %s."), cex.Error(), cex.ErrorMessage());
    }
    myLock.Unlock();
    return bRet;
}

void CSolutionMultiBitmap::Save()
{
    tbtraceIO( _T("CSolutionMultiBitmap::Save"));
    CSingleLock myLock(&m_critDoc);
    _bstr_t bstrFile = 
        CCustomerMHOptions::instance()->OptionValue(_T("PrinterMultiBitmapDataFile"));
    m_spDoc->save(bstrFile);
    tbtrace(TM_INFO, _T("Request file '%s' updated."), (LPCTSTR)bstrFile);
    myLock.Unlock();
}


void CSolutionMultiBitmap::CreateDoc()
{
    tbtraceIO( _T("CSolutionMultiBitmap::CreateDoc"));
    _bstr_t bstrFile = 
        CCustomerMHOptions::instance()->OptionValue(_T("PrinterMultiBitmapDataFile"));

    if( m_spDoc == NULL )
    {
        m_spDoc.CreateInstance(__uuidof(DOMDocument40));
        m_spDoc->async = VARIANT_FALSE;
    }

    MSXML2::IXMLDOMNodePtr spRoot = m_spDoc->createElement(MBMP);
    m_spDoc->appendChild(spRoot);
    m_spDoc->save(bstrFile);
}

bool CSolutionMultiBitmap::HaveFastLaneRequests()
{
	tbtraceIO( _T("CSolutionMultiBitmap::HaveFastLaneRequests"));
	return HaveRequests(true);
}

bool CSolutionMultiBitmap::HaveRAPRequests()
{
	tbtraceIO( _T("CSolutionMultiBitmap::HaveRAPRequests"));
    return HaveRequests(false);
}

bool CSolutionMultiBitmap::GetFastLaneRequest(MBMP_REQ & req)
{
    tbtraceIO( _T("CSolutionMultiBitmap::GetFastLaneRequest"));
    return GetRequest(req, true);
}


bool CSolutionMultiBitmap::GetRAPRequest(MBMP_REQ & req)
{
    tbtraceIO( _T("CSolutionMultiBitmap::GetRAPRequest"));
    return GetRequest(req, false);
}

bool CSolutionMultiBitmap::UpdateFastLaneRequest(long lIndex, long lStation, bool bSucceeded, CString csResult)
{
    tbtraceIO( _T("CSolutionMultiBitmap::UpdateFastLaneRequest"));
    return UpdateRequest(lIndex, lStation, bSucceeded, csResult, true);
}

bool CSolutionMultiBitmap::UpdateRAPRequest(long lIndex, long lStation, bool bSucceeded, CString csResult)
{
    tbtraceIO( _T("CSolutionMultiBitmap::UpdateRAPRequest"));
    return UpdateRequest(lIndex, lStation, bSucceeded, csResult, false);
}

bool CSolutionMultiBitmap::HaveRequests(bool bFastLane)
{
	tbtraceIO( _T("CSolutionMultiBitmap::HaveRequests"));
	CSingleLock myLock(&m_critDoc);

    long lSize = 0;

    if( m_spDoc != NULL )
    {
        LPCOLESTR pstr = bFastLane ? MBMP_QRY_PENDINGANDFAILED_FL : MBMP_QRY_PENDINGANDFAILED_RAP;
        MSXML2::IXMLDOMNodeListPtr spList = m_spDoc->selectNodes(pstr);
        if( spList != NULL )
        {
            lSize = spList->length;
        }
    }

	myLock.Unlock();
	return (lSize > 0);
}

bool CSolutionMultiBitmap::GetRequest(MBMP_REQ & req, bool bFastLane)
{
	CSingleLock myLock(&m_critDoc);
    bool bRet = false;

    long lSize = 0;

    if( m_spDoc != NULL )
    {
        // To ensure a best case scenario of updated bitmaps, we'll prioritize
        // on all the Pending bitmaps that have not yet been loaded, once we have
        // no more new ones left, we will process the prior failed attempts one 
        // at a time.
        LPCOLESTR pstr = bFastLane ? MBMP_QRY_PENDING_FL : MBMP_QRY_PENDING_RAP;
        MSXML2::IXMLDOMNodePtr spNode = m_spDoc->selectSingleNode(pstr);

        if( spNode == NULL )
        {
            // No new SetBitmap requests, let's try find one that has previously failed
            pstr = bFastLane ? MBMP_QRY_FAILED_FL : MBMP_QRY_FAILED_RAP;
            spNode = m_spDoc->selectSingleNode(pstr);
        }

        if( spNode != NULL )
        {
            // Get the parent node
            spNode = spNode->parentNode;
            req.lIndex = _ttol(GetAttr(spNode, MBMP_SETBMP_NUM));
            req.lStation = _ttol(GetAttr(spNode, MBMP_SETBMP_STN));
	        req.csFilename = GetAttr(spNode, MBMP_SETBMP_FILE);
            req.lWidth = _ttol(GetAttr(spNode, MBMP_SETBMP_WIDTH));
            req.lAlign = _ttol(GetAttr(spNode, MBMP_SETBMP_ALIGN));
            bRet = true;
        }
    }

	myLock.Unlock();
	return bRet;
}

bool CSolutionMultiBitmap::UpdateRequest(long lIndex, long lStation, bool bSucceeded, CString csResult, bool bFastLane)
{
    bool bRet = false;
    if( m_spDoc != NULL )
    {
        CSingleLock myLock(&m_critDoc);
        LPCOLESTR pstrStat = bSucceeded ? MBMP_UP_STAT_OK : MBMP_UP_STAT_FAIL;

        MSXML2::IXMLDOMNodePtr spHist;
        MSXML2::IXMLDOMNodePtr spUp;
        MSXML2::IXMLDOMNodePtr spNode;
        
        spNode = GetUpdateNode(lIndex, lStation, bFastLane);
        if( spNode != NULL )
        {
            // clean up the current history 
            spHist = ClearAndReturnUpdateHistory(lIndex, lStation, bFastLane);

            // copy the last update node to history
            spHist->appendChild(spNode->cloneNode(VARIANT_TRUE));

            // Update the current node with the new values
            COleDateTime oleDate = COleDateTime::GetCurrentTime();
            _variant_t vtDate(oleDate);
            vtDate.vt = VT_DATE;
            SetAttr(spNode, MBMP_UP_STAT, pstrStat);
            SetAttr(spNode, MBMP_UP_EXEC, vtDate);
            SetAttr(spNode, MBMP_UP_RES, (LPCTSTR)csResult);
            bRet = true;
            Save();
        }
        else
        {   
            tbtrace(TM_ERROR, _T("Failed to locate Request/Update record for updating."));
        }
        myLock.Unlock();
    }
    
	return bRet;
}

bool CSolutionMultiBitmap::AddRequest(long lIndex, long lStation, CString csFilePath,  
                                           long lWidth, long lAlign)
{
    tbtraceIO( _T("CSolutionMultiBitmap::AddRequest"));
	CSingleLock myLock(&m_critDoc);
    bool bRet = false;
	
    if( !csFilePath.IsEmpty() )
	{
        MSXML2::IXMLDOMNodePtr spNode = GetRequestNode(lIndex, lStation);
        if( spNode != NULL )
        {
            SetAttr(spNode, MBMP_SETBMP_FILE, (LPCTSTR)csFilePath);
            SetAttr(spNode, MBMP_SETBMP_WIDTH, lWidth);
            SetAttr(spNode, MBMP_SETBMP_ALIGN, lAlign);
            InitRequestUpdateSection(spNode);
            Save();
            tbtrace(TM_INFO, _T("Added SetBitmap request to queue. Details:[%s]"), RequestToString(spNode));
            bRet = true;
        }
        else
        {
            tbtrace(TM_ERROR, _T("Failed to create the SetBitmap request."));    
        }
		
	}
    else
    {
        tbtrace(TM_ERROR, _T("Attempted to add a request with no filename in queue. Ignoring..."));
    }
    myLock.Unlock();
	return bRet;
}

    
// If the history node doesn't exist create it
MSXML2::IXMLDOMNodePtr CSolutionMultiBitmap::ClearAndReturnUpdateHistory(long lIndex, long lStation, bool bFastLane)
{
    // Get the history node
    MSXML2::IXMLDOMNodePtr spReq = GetRequestNode(lIndex, lStation);
    MSXML2::IXMLDOMNodePtr spHist = spReq->selectSingleNode(MBMP_HIST);
    if( spHist == NULL )
    {
        spHist = m_spDoc->createElement(MBMP_HIST);
        spReq->appendChild(spHist);
    }
                
    // identify & remove the current history item
    LPCTSTR pszFmt = bFastLane ? MBMP_QRY_GETHISTUPD_FL : MBMP_QRY_GETHISTUPD_RAP;
    CString strQry;
    strQry.Format(pszFmt, lIndex, lStation);
    MSXML2::IXMLDOMNodePtr spUpd = m_spDoc->selectSingleNode((LPCTSTR)strQry);
    if( spUpd != NULL )
    {
        spHist->removeChild(spUpd);
    }
    return spHist;
}

// Fetch the requested node, if it doesn't exist, create it and initialize all the attributes
MSXML2::IXMLDOMNodePtr CSolutionMultiBitmap::GetRequestNode(long lIndex, long lStation)
{
    tbtraceIO( _T("CSolutionMultiBitmap::GetRequestNode"));
    if( m_spDoc != NULL )
    {
        CSingleLock myLock(&m_critDoc);
        CString strQry;
        strQry.Format(MBMP_QRY_GETREQ, lIndex, lStation);
        MSXML2::IXMLDOMNodePtr spNode = m_spDoc->selectSingleNode((LPCTSTR)strQry);
        if( spNode == NULL )
        {
           // request for this slot/station doesn't exist so create it
            spNode = m_spDoc->createElement(MBMP_SETBMP);
            SetAttr(spNode, MBMP_SETBMP_NUM, lIndex);
            SetAttr(spNode, MBMP_SETBMP_STN, lStation);
            SetAttr(spNode, MBMP_SETBMP_FILE);
            SetAttr(spNode, MBMP_SETBMP_WIDTH);
            SetAttr(spNode, MBMP_SETBMP_ALIGN);
            m_spDoc->firstChild->appendChild(spNode);
        }
        myLock.Unlock();
        return spNode;
    }
    else
    {
        return NULL;
    }
}

MSXML2::IXMLDOMNodePtr CSolutionMultiBitmap::GetUpdateNode(long lIndex, long lStation, bool bFastLane)
{
    MSXML2::IXMLDOMNodePtr spNode = NULL;
    if( m_spDoc != NULL )
    {
        CString strQry;
        LPCTSTR pszQuery = bFastLane ? MBMP_QRY_GETREQUPD_FL : MBMP_QRY_GETREQUPD_RAP;
        strQry.Format(pszQuery, lIndex, lStation);
        spNode = m_spDoc->selectSingleNode((LPCTSTR)strQry);
    }
    return spNode;
}

void CSolutionMultiBitmap::InitRequestUpdateSection(MSXML2::IXMLDOMNodePtr spNode)
{
    tbtraceIO( _T("CSolutionMultiBitmap::InitRequestUpdateSection"));
    if( m_spDoc != NULL )
    {
        // clean up the current history
        MSXML2::IXMLDOMNodePtr spHist;
        spHist = spNode->selectSingleNode(MBMP_HIST);
        if( spHist == NULL )
        {
            spHist = m_spDoc->createElement(MBMP_HIST);
            spNode->appendChild(spHist);
        }
        else
        {
            while( spHist->firstChild != NULL )
            {
                spHist->removeChild(spHist->firstChild);
            }
        }


        // copy the current update nodes to history & remove
        MSXML2::IXMLDOMNodeListPtr spList;
        MSXML2::IXMLDOMNodePtr spUp;
        spList = spNode->selectNodes(MBMP_UP);
        spList->reset();
        while( (spUp = spList->nextNode()) != NULL)
        {
            spHist->appendChild(spUp->cloneNode(VARIANT_TRUE));
            spNode->removeChild(spUp);
        }

        // Create new values
        COleDateTime oleDate = COleDateTime::GetCurrentTime();
        _variant_t vtDate(oleDate);
        vtDate.vt = VT_DATE;

       
        // Create the FL update node
        spUp = m_spDoc->createElement(MBMP_UP);
        SetAttr(spUp, MBMP_UP_PRN, MBMP_UP_PRN_FL);
        SetAttr(spUp, MBMP_UP_STAT, MBMP_UP_STAT_PEND);
        SetAttr(spUp, MBMP_UP_REQ, vtDate);
        SetAttr(spUp, MBMP_UP_EXEC);
        SetAttr(spUp, MBMP_UP_RES);
        spNode->appendChild(spUp);

        if( SyncRAP() )
        {
            // Create the RAP update node
            spUp = m_spDoc->createElement(MBMP_UP);
            SetAttr(spUp, MBMP_UP_PRN, MBMP_UP_PRN_RAP);
            SetAttr(spUp, MBMP_UP_STAT, MBMP_UP_STAT_PEND);
            SetAttr(spUp, MBMP_UP_REQ, vtDate);
            SetAttr(spUp, MBMP_UP_EXEC);
            SetAttr(spUp, MBMP_UP_RES);
            spNode->appendChild(spUp);
        }
    }
}


void CSolutionMultiBitmap::SetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName, LPCOLESTR strValue)
{
    MSXML2::IXMLDOMAttributePtr spAttr;
    spAttr = spNode->attributes->getNamedItem(strName);
    if( spAttr == NULL )
    {
        // create if it doesn't exist
        spAttr = m_spDoc->createAttribute(strName);
        spAttr->value = strValue;
        spNode->attributes->setNamedItem(spAttr);
    }
    else
    {
        spAttr->value = strValue;
    }
}


void CSolutionMultiBitmap::SetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName, _variant_t vtValue)
{
    MSXML2::IXMLDOMAttributePtr spAttr;
    spAttr = spNode->attributes->getNamedItem(strName);
    if( spAttr == NULL )
    {
        // create if it doesn't exist
        spAttr = m_spDoc->createAttribute(strName);
        spAttr->value = vtValue;
        spNode->attributes->setNamedItem(spAttr);
    }
    else
    {
        spAttr->value = vtValue;
    }
}


CString CSolutionMultiBitmap::GetAttr(MSXML2::IXMLDOMNodePtr spNode, LPCOLESTR strName)
{
    MSXML2::IXMLDOMAttributePtr spAttr;
    spAttr = spNode->attributes->getNamedItem(strName);
    if( spAttr )
    {
        return (LPCTSTR)_bstr_t(spAttr->value.bstrVal);
    }
    else
    {
        return _T("");
    }
}

CString CSolutionMultiBitmap::RequestToString(MSXML2::IXMLDOMNodePtr spNode)
{
    CString str;
    str.Format(_T("SetBitmap(Index:'%s', Station:'%s', File:'%s', Width:'%s', Alignment:'%s')"),
        GetAttr(spNode, MBMP_SETBMP_NUM),
        GetAttr(spNode, MBMP_SETBMP_STN),
        GetAttr(spNode, MBMP_SETBMP_FILE),
        GetAttr(spNode, MBMP_SETBMP_WIDTH),
        GetAttr(spNode, MBMP_SETBMP_ALIGN));
    return str;
}

//************************************************************************
// 
// Function:   	AddRecentUpdatedBitmaps
// 
// Purpose:		Add recent updated bitmaps.
// 
// Parameters:	None
// 
// Returns:		None
// 
// Notes:		Called when RAP connects to LANE.
//
//************************************************************************ 

void CSolutionMultiBitmap::AddRecentUpdatedBitmaps()
{
	tbtraceIO( _T("CSolutionMultiBitmap::AddRecentUpdatedBitmaps"));

	if( m_spDoc != NULL )
	{
		CSingleLock myLock(&m_critDoc);

		MSXML2::IXMLDOMNodeListPtr spNodeList;
		MSXML2::IXMLDOMNodePtr spNode;
		std::vector <COleDateTime> vData;
		std::vector <MBMP_REQ> vMBMP;
		COleDateTime cdtRecent;
		MBMP_REQ mbmp;
		CString csQry;
		int nCount;

		// get date/time requested for all updates.
		csQry.Format(_T("%s"), MBMP_QRY_GETUPDREQ_FL);
		spNodeList = m_spDoc->selectNodes((LPCTSTR)csQry);
		spNodeList->reset();
		while( (spNode = spNodeList->nextNode() ) != NULL )
		{
			_variant_t vt = spNode->GetnodeValue();
            COleDateTime dt(vt);
			if(dt.GetStatus()==COleDateTime::valid)
			{
				vData.push_back(dt);
			}
		}

		// get recent date/time
		cdtRecent = vData.at(0);
		for ( nCount=1; nCount<vData.size(); nCount++ )
		{
			if ( cdtRecent < vData.at(nCount) )
			{
				cdtRecent = vData.at(nCount);
			}
		}

		// get values needed to AddRequest for recent bitmaps.
        csQry.Format(_T("%s/%s"), MBMP, MBMP_SETBMP);
        spNodeList = m_spDoc->selectNodes((LPCTSTR)csQry);
		spNodeList->reset();
		nCount = 0;
		while( (spNode = spNodeList->nextNode() ) != NULL )
		{
			COleDateTimeSpan span = cdtRecent - vData.at(nCount);
			double nMin = span.GetTotalMinutes();
			if( span.GetTotalMinutes() <= 10 )
			{
				mbmp.lIndex = _ttol(GetAttr(spNode, MBMP_SETBMP_NUM));
				mbmp.lStation = _ttol(GetAttr(spNode, MBMP_SETBMP_STN));
				mbmp.csFilename = GetAttr(spNode, MBMP_SETBMP_FILE);
				mbmp.lWidth = _ttol(GetAttr(spNode, MBMP_SETBMP_WIDTH));
				mbmp.lAlign = _ttol(GetAttr(spNode, MBMP_SETBMP_ALIGN));
				vMBMP.push_back( mbmp );
			}
			nCount++;
		}

		// AddRequest to all bitmaps that are recent.
		for ( nCount=0; nCount<vMBMP.size(); nCount++ )
		{
			mbmp = vMBMP.at(nCount);
			int iRet = AddRequest(mbmp.lIndex,mbmp.lStation,mbmp.csFilename,mbmp.lWidth,mbmp.lAlign);
			if ( iRet == -1 )
			{
				tbtrace(TM_ERROR, _T("Failed to add SetBitmap request to the queue."));
			}
		}

		myLock.Unlock();
	}
}

//
// Definition of class constants
//
LPCOLESTR CSolutionMultiBitmap::MBMP = OLESTR("MultiBitmapData");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP = OLESTR("SetBitmap");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP_NUM = OLESTR("BitmapNumber");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP_STN = OLESTR("Station");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP_FILE = OLESTR("Filename");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP_WIDTH = OLESTR("Width");
LPCOLESTR CSolutionMultiBitmap::MBMP_SETBMP_ALIGN = OLESTR("Alignment");
LPCOLESTR CSolutionMultiBitmap::MBMP_HIST = OLESTR("History");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP = OLESTR("Update");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_PRN = OLESTR("Printer");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_PRN_FL = OLESTR("FL");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_PRN_RAP = OLESTR("RAP");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_STAT = OLESTR("Status");  
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_STAT_PEND = OLESTR("Pending");  
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_STAT_OK = OLESTR("Updated");  
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_STAT_FAIL = OLESTR("Failed");  
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_REQ = OLESTR("Requested");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_EXEC = OLESTR("Executed");
LPCOLESTR CSolutionMultiBitmap::MBMP_UP_RES = OLESTR("Result");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_PENDINGANDFAILED_FL = 
    _T("/MultiBitmapData/SetBitmap/Update[(@Status='Pending' or @Status='Failed') and @Printer='FL']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_PENDINGANDFAILED_RAP = 
    _T("/MultiBitmapData/SetBitmap/Update[(@Status='Pending' or @Status='Failed') and @Printer='RAP']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_PENDING_FL = 
    _T("/MultiBitmapData/SetBitmap/Update[@Status='Pending' and @Printer='FL']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_PENDING_RAP = 
    _T("/MultiBitmapData/SetBitmap/Update[@Status='Pending' and @Printer='RAP']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_FAILED_FL = 
    _T("/MultiBitmapData/SetBitmap/Update[@Status='Failed' and @Printer='FL']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_FAILED_RAP = 
    _T("/MultiBitmapData/SetBitmap/Update[@Status='Failed' and @Printer='RAP']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETREQ = 
    _T("/MultiBitmapData/SetBitmap[@BitmapNumber='%d' and @Station='%d']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETREQUPD_FL = 
    _T("/MultiBitmapData/SetBitmap[@BitmapNumber='%d' and @Station='%d']/Update[@Printer='FL']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETREQUPD_RAP = 
    _T("/MultiBitmapData/SetBitmap[@BitmapNumber='%d' and @Station='%d']/Update[@Printer='RAP']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETHISTUPD_FL = 
    _T("/MultiBitmapData/SetBitmap[@BitmapNumber='%d' and @Station='%d']/History/Update[@Printer='FL']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETHISTUPD_RAP = 
    _T("/MultiBitmapData/SetBitmap[@BitmapNumber='%d' and @Station='%d']/History/Update[@Printer='RAP']");

LPCTSTR CSolutionMultiBitmap::MBMP_QRY_GETUPDREQ_FL = 
    _T("/MultiBitmapData/SetBitmap/Update[@Printer='FL']/@Requested");