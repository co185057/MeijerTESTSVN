// CustomerCurrentReceipt.cpp: implementation of the CCustomerCurrentReceipt class.
//
// POS23895 WORK REQUEST:16656 Name: Robert Susanto Date: November 22, 2011
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CustomerCurrentReceipt.h"
#include "CustomerTrxBroker.h"
#include "TraceSystem.h"
#include "CustomerMHOptions.h"
#include "CustomerMsgMap.h"

#define T_ID  _T("CCustomerCurrentReceipt")
#define MAIN_RECEIPT_FILE _T("c:\\scot\\data\\MainReceipt.SLP")
#define PREVIOUS_RECEIPT_FILE _T("c:\\scot\\data\\PreviousMainReceipt.SLP")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomerCurrentReceipt::CCustomerCurrentReceipt() :
    m_pMainReceiptFile(NULL),
    m_bGiftReceipt(false),
    m_pLastReceiptPrinted(NULL)  

{
}

CCustomerCurrentReceipt::~CCustomerCurrentReceipt()
{
    
}

bool CCustomerCurrentReceipt::FinishReceipt( bool bDiscard )
{
    tbtraceIO(_T("CCustomerCurrentReceipt::FinishReceipt") );

	// We got a cut - Finished with MainReceipt?
    CString csMain = CCustomerTrxBroker::instance()->TBGetProperty(_T("MainReceipt"));
	if (csMain == _T("0"))
	{    
		CCustomerTrxBroker::instance()->TBSetProperty(_T("MainReceipt"), _T("1"));
		m_pMainReceiptFile->Close();
        delete m_pMainReceiptFile;
        m_pMainReceiptFile = NULL;

		// Remove the previous receipt file FIRST!
		try
		{
			CFile::Remove(PREVIOUS_RECEIPT_FILE);
		}
		catch(CFileException *pe)
		{
			pe->Delete();
		}
		// THEN, Rename the MAINRECEIPT.SLP file FIRST!
		try
		{
			CFile::Rename(MAIN_RECEIPT_FILE, PREVIOUS_RECEIPT_FILE);
		}
		catch(CFileException *pe)
		{
			pe->Delete();
		}
	}

    //SOTF 7612
    bool bRetVal = CSolutionCurrentReceipt::FinishReceipt(bDiscard);

    m_bGiftReceipt = false;

    return bRetVal;

    //sotf

}

bool CCustomerCurrentReceipt::WriteToReceipt( CString csReceiptData )
{
    tbtraceIO(_T("CCustomerCurrentReceipt::WriteToReceipt") );

	// If this is a bitmap, ensure MainReceipt Property is set to "0".
    const LPCTSTR szBitmap = _T("\x1B|1B");
    if ( 0 == csReceiptData.Compare( szBitmap ) )
    {
		CString csMain = CCustomerTrxBroker::instance()->TBGetProperty(_T("MainReceipt"));
		if (csMain == _T("1"))
		{
			CCustomerTrxBroker::instance()->TBSetProperty(_T("MainReceipt"), _T("2"));
		}
	}

	// If we're in MainReceipt, need to open the file.
    CString csMain = CCustomerTrxBroker::instance()->TBGetProperty(_T("MainReceipt"));
	if ((csMain == _T("0")) || (csMain == _T("2")))
	{  
		if (csMain == _T("2"))
		{
			CCustomerTrxBroker::instance()->TBSetProperty(_T("MainReceipt"), _T("3"));
		}

		//m_pMainReceiptFile.Open();
		if ( !m_pMainReceiptFile )
		{
			// Need to open new Receipt File
			//m_pMainReceiptFile = GetNewReceiptFile();
			try
			{
				m_pMainReceiptFile = new CFile();
				long nFileMode = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone;
				m_pMainReceiptFile->Open(MAIN_RECEIPT_FILE, nFileMode);
				m_pMainReceiptFile->SetLength(0);
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
		}

		try
		{
			CheckForStoreReceipt( csReceiptData );
			_bstr_t bstrData = csReceiptData;
			char* sData = (char*)bstrData;
			m_pMainReceiptFile->Write( sData, csReceiptData.GetLength() );
		}
		catch( CFileException* e )
		{
			TCHAR szError[255];
			szError[0] = 0;
			e->GetErrorMessage(szError, 255);
			tbtrace( TM_ERROR, _T("Failed to write to main receipt file (%s) %d:%s"),
				e->m_strFileName, e->m_cause, szError );
			e->Delete();
		}
	}

	return CSolutionCurrentReceipt::WriteToReceipt(csReceiptData);

}


//SOTF 7612
void CCustomerCurrentReceipt::CheckForStoreReceipt( CString csData )
{
    CString csRecData = csData;
    CSolutionCurrentReceipt::CheckForStoreReceipt(  csData );

    
    //check if this is the second gift card receipt
    if(csRecData.Find(CCustomerMHOptions::instance()->OptionValue(_T("GiftCardBalanceReceipt"))) != -1)
    {
        
        m_bGiftReceipt = true;
    }

    if(m_bGiftReceipt)
    {
        m_bIsStoreReceipt = false;
    }
    

}
//sotf

//+SOTF 8362
HFILE CCustomerCurrentReceipt::GetReceiptData(void)
{
    tbtraceIO(_T("GetReceiptData") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();

    HFILE hRet = NULL;
    CFile* pFile = NULL;

    if ( !m_lstReceipts.IsEmpty() )
    { 
        pFile = (CFile*)m_lstReceipts.GetHead();

        //is the current retrieve file the same as the last file sent to SSCO
        if((m_pLastReceiptPrinted != NULL) && (m_pLastReceiptPrinted == pFile))
        {
           tbtrace( TM_ERROR, _T("GetReceiptData() Receipt requested previously sent.  Not sending file to applicaiton") );

            pFile = (CFile*)m_lstReceipts.RemoveHead();
            try
		    {
			    m_bIsPrinting = false;
			    CString csFile = pFile->GetFilePath();
			    pFile->Close();
			    pFile->Remove( csFile );
			    csFile.TrimRight( _T("prt") );
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

            if ( !m_lstReceipts.IsEmpty() )
            { 
                pFile = (CFile*)m_lstReceipts.GetHead(); 
            }else
            {
                pFile = NULL;
            }

            //POS23895 - TB signalling the base to get the receipt based from the context received from ACS.
            //There is a possibility that when ACS sending the context, the receipt data is still being processed, e.g. gift receipt.
            //And it will result with the receipt not being printed as it returned the NULL file handle, to prevent this situation we need to do the checking in here
            //so that it will resend the TB_COMPLETEPRINTANDCUT after TB finished processing the receipt. - rrs
            if( IsInProgress() )
            {
                // A receipt is being produced, print it immedidately when finished
                SetPrintWhenFinished( true );
                tbtrace(TM_INFO, _T("Flag the TB to resend the TB_COMPLETEPRINTANDCUT after finished processing receipt"));
            }
            //e POS23895
           
        }
            

        if ( pFile )
        {
            m_bIsPrinting = true;
            hRet = pFile->m_hFile;
        }
    
    }else if( IsInProgress() )
    {
        // A receipt is being produced, print it immedidately when finished
        SetPrintWhenFinished( true );
    }
    m_pLastReceiptPrinted = pFile;

    tbtrace(TM_INFO, _T("File 0x%0X was returned"), hRet );
    myLock.Unlock();
    return hRet;
}


TBRC CCustomerCurrentReceipt::ReleaseReceiptData(HFILE hReceiptData)
{
 tbtraceIO( _T("ReleaseReceiptData()") );
    CSingleLock myLock( &m_Crit );
    myLock.Lock();
    CFile* pFile = NULL;

    //BEGIN TAR# 375786
    //Check that m_lstReceipts is not empty before making the call to RemoveHead()
    if ( !m_lstReceipts.IsEmpty() )
    {
       pFile = (CFile*)m_lstReceipts.GetHead(); 
       
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
        pFile = (CFile*)m_lstReceipts.RemoveHead();

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


//-SOTF 8362