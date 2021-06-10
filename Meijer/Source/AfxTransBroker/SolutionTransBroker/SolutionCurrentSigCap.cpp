// SolutionCurrentSigCap.cpp: implementation of the CSolutionCurrentSigCap class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SolutionCurrentSigCap.h"
#include "TraceSystem.h"
#include "CustomerCurrentReceipt.h"

#define T_ID  _T("CSolutionCurrentSigCap")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSolutionCurrentSigCap::CSolutionCurrentSigCap()
{
}

CSolutionCurrentSigCap::~CSolutionCurrentSigCap()
{
    
}

void CSolutionCurrentSigCap::AddStoreReceiptFile( CFile* pFile )
{
	tbtraceIO( _T("AddStoreReceiptFile") );

	tbtrace( TM_INFO, _T("Adding File %s"), pFile->GetFileName() );

	m_lstSlips.AddTail( pFile );
}




HFILE CSolutionCurrentSigCap::GetSlipData(void)
{
	tbtraceIO( _T("GetSlipData") );
	HFILE hRet = NULL;
    const TCHAR cBitmapTokenStart = _T('\x05');
    const TCHAR cBitmapTokenEnd = _T('\x0F');

	if ( m_lstSlips.GetCount() )
	{
		CFile* pFile = (CFile*)m_lstSlips.GetHead();
		
		DWORD dwLen = pFile->SeekToEnd();
		pFile->SeekToBegin();
		BYTE* pBuf = new BYTE[dwLen+1];
		memset( pBuf, 0, dwLen+1 );
		pFile->Read( pBuf, dwLen );
		_bstr_t tBuf;
		tBuf = (char*) pBuf;
		CString csUnicode = (TCHAR*) tBuf;
		int nBitmap = csUnicode.Find( cBitmapTokenStart );
		if ( -1 != nBitmap )
		{
			int nEndBitMap = csUnicode.Find( cBitmapTokenEnd );
			CString csToken = _T("\x06SignatureToken");
			CString csTemp;

			csTemp = csUnicode.Left(nBitmap);
			csTemp += csToken;
			csTemp += csUnicode.Right( csUnicode.GetLength() - nEndBitMap - 1 );

			csUnicode = csTemp;
		}
		pFile->SetLength(0);
		pFile->Write(csUnicode, csUnicode.GetLength() * sizeof(TCHAR) );
		delete[] pBuf;

		hRet = pFile->m_hFile;
	}
	else
	{
		// All slips complete, reset the match data
		CCustomerCurrentReceipt::instance()->ResetStoreRecMatch();
	}

	return hRet;
}

TBRC CSolutionCurrentSigCap::ReleaseSlipData(HFILE hSlipData)
{
	tbtraceIO( _T("ReleaseSlipData") );

    if ( !m_lstSlips.IsEmpty() )
    {

        CFile* pFile = (CFile*)m_lstSlips.RemoveHead();

        tbtrace( TM_INFO, _T("Checking Release for file 0x%0X"), pFile->m_hFile );

        if ( hSlipData == pFile->m_hFile )
        {
		    try
		    {
			    CString csFile = pFile->GetFilePath();
			    pFile->Close();
			    pFile->Remove( csFile );
			    csFile.Replace(_T(".prt"), _T(".bmp") );
			    CFileStatus Status;
			    if ( pFile->GetStatus( csFile, Status ) )
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
            m_lstSlips.AddHead( pFile );
            tbtrace( TM_ERROR, _T("FastLane releasing unknown sigslip 0x%0X!!!"), hSlipData );
        }
    }
    else
    {
        tbtrace( TM_WARNING, _T("FastLane releasing slip 0x%0X when TB has no slips!!!"), hSlipData );
    }

    return TB_SUCCESS;
}

void CSolutionCurrentSigCap::GetSlipBitmap( BSTR* pData)
{
	if ( m_lstSlips.GetCount() )
	{
		CFile* pFile = (CFile*)m_lstSlips.GetHead();
		CString csName = pFile->GetFilePath();
		csName.Replace(_T(".prt"), _T(".bmp") );
		CFileStatus Status;
		if ( pFile->GetStatus( csName, Status ) )
		{
			CFile Bitmap;
			Bitmap.Open( csName, CFile::modeRead, NULL );
			DWORD dwLen = Bitmap.SeekToEnd();
			Bitmap.SeekToBegin();
			BYTE* pBuf = new BYTE[dwLen+1];
			memset(pBuf, 0, dwLen+1);
			Bitmap.Read( pBuf, dwLen);
			*pData = SysAllocStringByteLen( (const char*)pBuf, dwLen );
			Bitmap.Close();
			delete[] pBuf;
		}
		else
		{
			*pData = NULL;
		}
	}
	else
	{
		*pData = NULL;
	}
}

TBRC CSolutionCurrentSigCap::PreprocessSignatureData(LPCTSTR lpFileName,BSTR bstrSignatureData, LPCTSTR lpReceiptType, BOOL* bProcessData)
{
	tbtraceIO( _T("PreprocessSignatureData") );

	DWORD dwLen = SysStringByteLen( bstrSignatureData );
	if ( dwLen > 0 )
	{
		*bProcessData = true;
		CFile Bitmap;
	    long nFileMode = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone;
		Bitmap.Open( lpFileName, nFileMode, NULL );
		Bitmap.Write( bstrSignatureData, dwLen );
		Bitmap.Close();
	}

	return TB_SUCCESS;
}
