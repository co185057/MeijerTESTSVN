// SolutionCurrentReceipt.cpp: implementation of the CSolutionCurrentReceipt class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "afxmt.h"
#include "afxcoll.h"
#include "SolutionCurrentReceipt.h"
#include "CustomerMHOptions.h"
#include "TraceSystem.h"
#include "ReturnStateMsg.h"
#include "CustomerTBScotMsg.h"
#include "CustomerTrxBroker.h"
#include "CustomerMsgMap.h"
#include "CustomerCurrentSlip.h"
#include "CustomerCurrentSigCap.h"
#include "CustomerCurrentCashDrawer.h"

#define T_ID  _T("CSolutionCurrentReceipt")

#define RECEIPT_FILE _T("%s\\Receipt%s.prt")
// Assumes only one bitmap per receipt.
#define RECEIPT_BITMAP _T("%s\\Receipt%s.bmp")
#define RECEIPT_FILE_SEARCHSTRING		_T("%s\\Receipt*.prt")
#define RECEIPT_BITMAP_SEARCHSTRING     _T("%s\\Receipt*.bmp")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionCurrentReceipt::CSolutionCurrentReceipt() :
    m_nPrintRequest(0),
    m_pReceiptFile(NULL),
    m_bIsPrinting(false),
    m_bPrintWhenFinished(false),
    m_bInProgressOverride(false),
	m_bIsStoreReceipt( false )
{
    m_csReceiptDir = CCustomerTrxBroker::instance()->TBGetProperty(_T("ReceiptFilePath"));
    if ( m_csReceiptDir.IsEmpty() )
    {
        m_csReceiptDir = _T("C:\\SCOT\\Data\\Receipts");
    }
	m_csReceiptDir.TrimLeft();
	m_csReceiptDir.TrimRight();
}

CSolutionCurrentReceipt::~CSolutionCurrentReceipt()
{
    // Clean up but don't delete receipt files
    PurgeAllReceipts(false);
	if ( m_pReceiptFile )
	{
		if ( IsInProgress() )
		{
			m_pReceiptFile->Close();
		}
		else
		{
			CString csFile = m_pReceiptFile->GetFilePath();
			m_pReceiptFile->Close();
			m_pReceiptFile->Remove( csFile );
		}
		delete m_pReceiptFile;
		m_pReceiptFile = NULL;
	}

}

HFILE CSolutionCurrentReceipt::GetReceiptData(void)
{
    tbtraceIO(_T("GetReceiptData") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    HFILE hRet = NULL;
    CFile* pFile = NULL;

    if ( !m_lstReceipts.IsEmpty() )
    {
        pFile = (CFile*)m_lstReceipts.GetHead();
        if ( pFile )
        {
            m_bIsPrinting = true;
            hRet = pFile->m_hFile;
        }
    }
    tbtrace(TM_INFO, _T("File 0x%0X was returned"), hRet );
    myLock.Unlock();
    return hRet;
}

TBRC CSolutionCurrentReceipt::ReleaseReceiptData(HFILE hReceiptData)
{
    tbtraceIO( _T("ReleaseReceiptData()") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();
    CFile* pFile = NULL;

    //BEGIN TAR# 375786
    //Check that m_lstReceipts is not empty before making the call to RemoveHead()
    if ( !m_lstReceipts.IsEmpty() )
    {
       pFile = (CFile*)m_lstReceipts.RemoveHead();
    }
    else
    {
       tbtrace( TM_ERROR, _T("FastLane receipt list is null!!! No receipt file to release."));
       return TB_SUCCESS;
    }
    //END TAR# 375786

    tbtrace( TM_INFO, _T("Checking Release for file 0x%0X"), pFile->m_hFile );

    if ( hReceiptData == pFile->m_hFile )
    {
		try
		{
			m_bIsPrinting = false;
			CString csFile = pFile->GetFilePath();
			pFile->Close();
			pFile->Remove( csFile );
			csFile.TrimRight( _T("prt") );
			//csFile = csFile.Left( csFile.GetLength() - 3 );
			csFile += _T("bmp");
			CFileStatus tStat;
			if ( pFile->GetStatus( csFile, tStat ) )
			{
				pFile->Remove( csFile );
			}
			delete pFile;
		}
		catch( CFileException* e )
		{
            TCHAR szError[255];
            szError[0] = 0;
            e->GetErrorMessage(szError, 255);
            tbtrace( TM_ERROR, _T("Failed to discard active receipt file (%s) %d:%s"),
                e->m_strFileName, e->m_cause, szError );
            e->Delete();
		}
    }
    else
    {
        m_lstReceipts.AddHead( pFile );
        tbtrace( TM_ERROR, _T("FastLane releasing unknown receipt 0x%0X!!!"), hReceiptData );
    }

    if ( !m_lstReceipts.IsEmpty() )
    {
        // There is more to print
        CCustomerTBScotMsg::instance()->SendReturnStateMsg( TB_COMPLETEPRINTANDCUT, NULL, NULL );
		m_bIsPrinting = true;
    }
    else
    {
        if ( IsInProgress() )
        {
            // A receipt is being produced, print it immedidately when finished
            SetPrintWhenFinished( true );
        }
        else
        {
            // All receipts are finished!
			SetPrintWhenFinished( false );
	        CString  csCurTBState = CCustomerTrxBroker::instance()->GetCurrentInProgressStateName();
            tbtrace( TM_INFO, _T("All receipts done in state '%s'"), csCurTBState );
            if ( -1 != csCurTBState.Find(_T("ASSISTMODE_TRXEND")) )
            {
                TBSTATE tbRet = TB_TRXEND;
                CCustomerMsgMap::instance()->msgAssistModeTrxEnd( tbRet, NULL );
                if ( tbRet != TB_IGNORE )
                {
                    CCustomerTBScotMsg::instance()->SendReturnStateMsg( tbRet, NULL, NULL );
                }
            }
        }
    }

    myLock.Unlock();
    return TB_SUCCESS;
}



bool CSolutionCurrentReceipt::FinishReceipt( bool bDiscard )
{
    tbtraceIO(_T("FinishReceipt") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    if ( bDiscard )
    {
        try
        {
            CString csFile = this->m_pReceiptFile->GetFilePath();
            m_pReceiptFile->Close();
            m_pReceiptFile->Remove( csFile );
            delete m_pReceiptFile;
            m_pReceiptFile = NULL;
        }
        catch( CFileException* e )
        {
            TCHAR szError[255];
            szError[0] = 0;
            e->GetErrorMessage(szError, 255);
            tbtrace( TM_ERROR, _T("Failed to discard active receipt file (%s) %d:%s"),
                e->m_strFileName, e->m_cause, szError );
            e->Delete();
        }
        SetPrintWhenFinished(false);
        m_bIsStoreReceipt = false;
    }
    else
    {
        if ( m_bIsStoreReceipt )
        {
            CCustomerCurrentSigCap::instance()->AddStoreReceiptFile( m_pReceiptFile );
            m_pReceiptFile = NULL;
            if ( GetPrintWhenFinished())
            {
                SetPrintWhenFinished(false);
                tbtrace(TM_ERROR,_T("PrintWhenFinished found a StoreReceipt!!"));
            }
            m_bIsStoreReceipt = false;
			m_nPrintRequest++;
        }
        else
        {
            m_lstReceipts.AddTail( m_pReceiptFile );
            m_pReceiptFile = NULL;
            m_nPrintRequest++;

            if ( GetPrintWhenFinished() )
            {
                // If not printing, then make this receipt print
                // If we are already printing, this receipt will be handled in ReleaseReceiptData
                // or in TBEndorseCompleted
				SetPrintWhenFinished( false );
                if ( !IsPrinting() && !CCustomerCurrentSlip::instance()->IsSlipPrinting() )
                {
                    m_bIsPrinting = true;
                    CReturnStateMsg *pStateMsg = new CReturnStateMsg(TB_COMPLETEPRINTANDCUT); // delete done in ReleaseMessageLParam()
                    CCustomerTBScotMsg::instance()->SendScotMsg( pStateMsg);
                }
            }
        }
    }
    myLock.Unlock();
    return true;
}

CFile* CSolutionCurrentReceipt::GetNewReceiptFile()
{
    tbtraceIO(_T("GetNewReceiptFile") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    CheckReceiptDir();

    long nFileMode = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone;
    CFile* pNewFile = new CFile();
    CString csFileName;

	m_csActiveIndex = GenerateIndex( );

    csFileName.Format( RECEIPT_FILE, m_csReceiptDir, m_csActiveIndex );
	tbtrace(TM_INFO, _T("New Receipt is %s"), csFileName );
    try
    {
        pNewFile->Open( csFileName, nFileMode);
        pNewFile->SetLength(0);
    }
    catch( CFileException* e)
    {
        TCHAR szError[255];
        szError[0] = 0;
        e->GetErrorMessage(szError, 255);
        tbtrace( TM_ERROR, _T("Failed to open receipt file (%s) %d:%s"),
            e->m_strFileName, e->m_cause, szError );
        e->Delete();
    }

    myLock.Unlock();
    return pNewFile;
}

CString CSolutionCurrentReceipt::GenerateIndex( )
{
	CString csRet = "";
	TCHAR strTick[20];

	DWORD dwTick = GetTickCount();
	_ultot( dwTick, strTick, 16 );

	csRet.Format(_T("%d%s"), m_nPrintRequest, strTick );

	return csRet;
}

void CSolutionCurrentReceipt::CheckReceiptDir()
{
    // NOTE:  No need to lock.  This is an internal private method.
    tbtraceIO(_T("CheckReceiptDir()"));
    CFileStatus rStat;
    BOOL bDir = FALSE;
    if ( CFile::GetStatus( m_csReceiptDir, rStat ) )
    {
        if ( ! (rStat.m_attribute & CFile::Attribute::directory) )
        {
            // The Dir is a File!  Remove it!
            CFile::Remove( m_csReceiptDir );
            bDir = CreateDirectory( m_csReceiptDir, NULL );
        }
        else
        {
            bDir = TRUE;
        }
    }
    else
    {
        bDir = CreateDirectory( m_csReceiptDir, NULL );
    }

    if ( !bDir )
    {
        tbtrace( TM_ERROR, _T("Failed to create receipt directory: 0x%0X"), GetLastError() );
        TCHAR* pBuf = NULL;
        DWORD nSize = GetTempPath(0, NULL);
        pBuf = new TCHAR(nSize + 1);
        memset( pBuf, 0, (nSize+1)*sizeof(TCHAR));
        GetTempPath(nSize, pBuf);
        // directory creation failed
        m_csReceiptDir = pBuf;
        delete[] pBuf;
    }
}


bool CSolutionCurrentReceipt::WriteToReceipt( CString csReceiptData )
{
    tbtraceIO(_T("WriteToReceipt") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    if ( !m_pReceiptFile )
    {
        // Need to open new Receipt File
        m_pReceiptFile = GetNewReceiptFile();
    }

    try
    {
        CheckForStoreReceipt( csReceiptData );
        _bstr_t bstrData = csReceiptData;
        char* sData = (char*)bstrData;
        m_pReceiptFile->Write( sData, csReceiptData.GetLength() );
    }
    catch( CFileException* e )
    {
        TCHAR szError[255];
        szError[0] = 0;
        e->GetErrorMessage(szError, 255);
        tbtrace( TM_ERROR, _T("Failed to write to receipt file (%s) %d:%s"),
            e->m_strFileName, e->m_cause, szError );
        e->Delete();
    }

    myLock.Unlock();
    return true;
}

bool CSolutionCurrentReceipt::IsPrinting()
{
    tbtraceIO(_T("IsPrinting") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    bool bRet = m_bIsPrinting;

    myLock.Unlock();
    return bRet;
}

void CSolutionCurrentReceipt::SetIsPrinting( bool bVal )
{
    tbtraceIO(_T("IsPrinting") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    m_bIsPrinting = bVal;

    myLock.Unlock();
}


bool CSolutionCurrentReceipt::IsInProgress()
{
    tbtraceIO(_T("IsInProgress") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    bool bRet;
    if ( m_pReceiptFile != NULL )
    {
        if ( !m_bInProgressOverride )
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
    }
    else
    {
        bRet = false;
    }
    myLock.Unlock();
    return bRet;
}

bool CSolutionCurrentReceipt::PurgeAllReceipts( bool bRemoveFiles )
{
    tbtraceIO(_T("PurgeAllReceipts") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    while ( !m_lstReceipts.IsEmpty() )
    {
        CFile* pFile = (CFile*)m_lstReceipts.RemoveHead();
        CString csFile = pFile->GetFilePath();
        pFile->Close();
        if ( bRemoveFiles )
        {
            pFile->Remove( csFile );
        }
        delete pFile;
    }

    myLock.Unlock();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/*
	This function will collect all saved receipts for Printing. This is called at 
	FastLane startUp to print all spooled receipts.
*/
////////////////////////////////////////////////////////////////////////////////
void CSolutionCurrentReceipt::PrintAllSavedReceipts( )
{
    tbtraceIO(_T("CSolutionCurrentReceipt::PrintAllSavedReceipts") );
	
	CheckReceiptDir();

    CSingleLock myLock( &m_Crit );
    myLock.Lock();
	try
	{
		CString strRcptFildWildCard;
		CString strRcptFileName;

		strRcptFildWildCard.Format(RECEIPT_FILE_SEARCHSTRING, m_csReceiptDir);

		CFileFind	finder;
		DWORD		nReceiptLen = 0;
		HFILE		hFile = NULL; 
		CFile		*pRcptFile;
        CString     strActiveFile;

        if ( m_pReceiptFile )
        {
            strActiveFile = m_pReceiptFile->GetFilePath();
        }
	
		// Find all the files in the receipt files directory
		BOOL bFound = finder.FindFile(strRcptFildWildCard);
		while (bFound)
		{
			bFound			= finder.FindNextFile();
			strRcptFileName = finder.GetFilePath();
            
            if ( strRcptFileName != strActiveFile )
            {
                pRcptFile = new CFile();
                if(!pRcptFile->Open(strRcptFileName, CFile::modeRead))
                   continue;

                m_lstReceipts.AddTail( pRcptFile );
            }
		}
	}
    catch( CFileException* e )
    {
        TCHAR szError[255];
        szError[0] = 0;
        e->GetErrorMessage(szError, 255);
        e->Delete();
    }
    myLock.Unlock();
    return;
}

int CSolutionCurrentReceipt::GetReceiptFileCount()
{
    tbtraceIO(_T("GetReceiptFileCount") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    int nRet = m_lstReceipts.GetCount();

    myLock.Unlock();
    return nRet;
}


void CSolutionCurrentReceipt::SetPrintWhenFinished( bool bVal )
{
    tbtraceIO(_T("SetPrintWhenFinished") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    m_bPrintWhenFinished = bVal;

    myLock.Unlock();
}

bool CSolutionCurrentReceipt::GetPrintWhenFinished( )
{
    return m_bPrintWhenFinished;
}

void CSolutionCurrentReceipt::OverrideInProgress( bool bVal )
{
    tbtraceIO(_T("OverrideInProgress") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    m_bInProgressOverride = bVal;

    myLock.Unlock();
}

CString CSolutionCurrentReceipt::PerserveBitmap( CString csFile )
{
	tbtraceIO( _T("PreserveBitmap") );
	CString csNewFile;
	csNewFile.Format( RECEIPT_BITMAP, m_csReceiptDir, m_csActiveIndex );

	if ( !CopyFile( csFile, csNewFile, false ) )
	{
		tbtrace( TM_ERROR, _T("Bitmap copy failed! Err=%d"), GetLastError() );
	}

	return csNewFile;
}

void CSolutionCurrentReceipt::AddStoreRecMatch( CString csData )
{
    CString csMatch = csData;
    csMatch.TrimLeft();
    csMatch.TrimRight();

    if ( !csData.IsEmpty())
    {
        m_lstStoreMatch.AddHead( csData );
    }
}

void CSolutionCurrentReceipt::ResetStoreRecMatch( )
{
    m_lstStoreMatch.RemoveAll();
}

void CSolutionCurrentReceipt::CheckForStoreReceipt( CString csData )
{
        //if we know that the slip should be printed at the rap then
    //don't set the m_bIsStoreReceipt flag
    /*CCustomerTrxBroker *pTrx = CCustomerTrxBroker::instance();	//commented for SSCOI-48605
	long lVal = _ttol(pTrx->TBGetProperty(_T("SignPaperForm"),true));
    if(lVal != 0)
    {
       
        m_bIsStoreReceipt = false;
        return;
    }*/
    if ( !m_bIsStoreReceipt)
    {
        bool bFound = false;
        for( POSITION pPOS = m_lstStoreMatch.GetHeadPosition();
             (NULL != pPOS) && !bFound;  )
        {
			CString csMatch = m_lstStoreMatch.GetNext(pPOS);
            if ( -1 != csData.Find( csMatch ) )
            {
                bFound = true;
                m_bIsStoreReceipt = true;
            }
        }
    }
}

