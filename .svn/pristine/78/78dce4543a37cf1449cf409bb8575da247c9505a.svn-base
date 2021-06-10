/*
-------------------------------------------------------------------------------

 File Name       : SigReceiptDataFile.cpp
 Description     : Definition for TB class : CSigReceiptDataFile
 
 Language        : MSVC++ 
 Operating System: Win32
 Author/Location : SCOT Team, RSG-Atlanta

 Copyright (C) NCR Corporation 1999

-------------------------------------------------------------------------------
 Date    | Changes Made                                     | Version | By
-------------------------------------------------------------------------------
 12/10/99  Original version                                   01.00.00  Yafu
-------------------------------------------------------------------------------
*/


#include "stdafx.h"
#include "common.h"
#include "devmgr.h"
#include "SigReceiptDataFile.h"
#include "wingdi.h"
#include "math.h"
#include "io.h"
#include "smstate.h"

#define COMP_ID ID_SM
#define T_ID _T("SigC")

#define EMPTY_STRING			((LPCTSTR)_T(""))

// define SigReceipt related path and file names
#define SIGRECEIPT_PATH			    _T("%DATA_DRIVE%\\SCOT\\DATA\\") 
#define SIGRECEIPT_FILE_NAME	  _T("S-RECEIPT.SCP") 
#define SIGRECEIPT_LOG_NAME		  _T("S-RECEIPT.SCP.LOG") 
#define SIGRECEIPT_BACKUP_NAME	_T("S-RECEIPT.SCP.BAK") 
#define SIGRECEIPT_BADFILE_NAME	_T("S-RECEIPT.SCP.BAD") 

#define SIGTOK              _T("\x06SignatureToken")

const CString  SCP_SLIP_DATA_TOKEN =_T("#T#") ;
const CString  SCP_SIG_DATA_TOKEN =_T("#S#") ;
const CString  SCP_FILE_END_TOKEN = _T("#E#");
const CString  SCP_RECEIPTTYPE_TOKEN = _T("#R#");

const signed long SCP_TOKEN_LENGTH=3 * sizeof(TCHAR);


//Allocate a two-dimentional array of BYTE to hold bitmap data
static BYTE arrayBitmapData[WINDOW_HEIGHT][bitmap_row_bytes(WINDOW_HEIGHT)];	// should be local to the class??


// Defines color indices a monochrome bitmap
typedef struct {
	RGBQUAD color1;						// black
	RGBQUAD color2;						// white
}COLORTABLE;

const CString TMP_BITMAP_FILE1 = CString(_T("S-RECEIPT_TMP1.BMP")) ;
const CString TMP_BITMAP_FILE2 = CString(_T("S-RECEIPT_TMP2.BMP")) ;

CSigReceiptDataFile::CSigReceiptDataFile()
		:m_csReceiptFilePath(SIGRECEIPT_PATH), m_csFileName(SIGRECEIPT_FILE_NAME)  
{
	GET_PARTITION_PATH(m_csReceiptFilePath, m_csReceiptFilePath.GetBuffer(_MAX_PATH));
	m_csReceiptFilePath.ReleaseBuffer();

	m_csFileName  = m_csReceiptFilePath + m_csFileName;
	m_hSReceiptFile = INVALID_HANDLE_VALUE ;
	m_bWrite = false ;
  m_ePreviousToken = SIGCAP_ERROR_TOKEN ;
  m_ePrePreviousToken = SIGCAP_ERROR_TOKEN ;
  m_csSlipData.Empty() ;
  m_csTempBitmapFile.Empty() ;
}


CSigReceiptDataFile::CSigReceiptDataFile(LPCTSTR szFileName)
		:m_csReceiptFilePath(SIGRECEIPT_PATH) 
{
	ASSERT(_tcsclen(szFileName));
	
	GET_PARTITION_PATH(m_csReceiptFilePath, m_csReceiptFilePath.GetBuffer(_MAX_PATH));
	m_csReceiptFilePath.ReleaseBuffer();

	m_csFileName  = szFileName;
	m_hSReceiptFile = INVALID_HANDLE_VALUE ;
	m_bWrite = false ;
  m_ePreviousToken = SIGCAP_ERROR_TOKEN ;
  m_ePrePreviousToken = SIGCAP_ERROR_TOKEN ;
  m_csSlipData.Empty() ;
  m_csTempBitmapFile.Empty() ;
}


CSigReceiptDataFile::~CSigReceiptDataFile()
{
	CloseFile();
}


HANDLE CSigReceiptDataFile::OpenFile(bool bWrite)
{
	if (m_hSReceiptFile ==INVALID_HANDLE_VALUE)  // the file is not opened successfully yet
	{
		DWORD dwAccessoMode, dwHowtoCreate ; 		

		m_bWrite = bWrite  ;
		dwAccessoMode= m_bWrite?GENERIC_WRITE:GENERIC_READ;
		dwHowtoCreate= m_bWrite?OPEN_ALWAYS:OPEN_EXISTING;
		m_hSReceiptFile = ::CreateFile(
			  m_csFileName,          // pointer to name of the file
			  dwAccessoMode,				// access (read-write) mode
			  0,						// share mode
			  NULL,						// pointer to security attributes
			  dwHowtoCreate,			// how to create
			  FILE_ATTRIBUTE_NORMAL,	// file attributes
			  NULL						// handle to file with attributes to copy
			);
		if (m_hSReceiptFile ==INVALID_HANDLE_VALUE)
		{
			ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
		}
		if(m_bWrite)
			::SetFilePointer(m_hSReceiptFile, 0, NULL, FILE_END) ;  // set to the end of file
	}

	return m_hSReceiptFile ;  // return the handle to the file
}

void CSigReceiptDataFile::CloseFile()
{
	if (m_hSReceiptFile !=INVALID_HANDLE_VALUE)   // file opened
	{
		::CloseHandle(m_hSReceiptFile) ;
		m_hSReceiptFile = INVALID_HANDLE_VALUE ;
	}
	return;
}

void CSigReceiptDataFile::SaveSigCapTransData(const SigCapTransData& TransData)
{
	ASSERT(m_bWrite) ;
	if (m_hSReceiptFile!=INVALID_HANDLE_VALUE && m_bWrite)		
	{   // make sure file has been created and it is write-operation
		SaveString(TransData.szTransNum) ;
		SaveLong(TransData.lLaneNum) ;
		SaveLong(TransData.lTransTotal) ;
		SaveString(TransData.szCreditCardNum) ;
		SaveString(TransData.szApprovalCd) ;
		SaveString(TransData.szCreditCardOwnerName) ;
		SaveTimeStamp(TransData.SigCapTimeStamp) ;	
	}
}

void CSigReceiptDataFile::SaveString(const CString& Str)
{
	unsigned long lBytesWritten, lDataLen = (unsigned long)(Str.GetLength()* sizeof(TCHAR)) ;

  BOOL bResult = ::WriteFile(m_hSReceiptFile, &lDataLen , sizeof(lDataLen) , &lBytesWritten, NULL) ;
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
  bResult = ::WriteFile(m_hSReceiptFile, Str , lDataLen , &lBytesWritten, NULL) ; 
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
}

void CSigReceiptDataFile::SaveLong(long lData)
{
	unsigned long lBytesWritten,  lDataLen = sizeof(lData);

	BOOL bResult = WriteFile(m_hSReceiptFile, &lDataLen , sizeof(long) , &lBytesWritten, NULL) ; 
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
	bResult = WriteFile(m_hSReceiptFile, &lData , sizeof(long) , &lBytesWritten, NULL) ; 
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
}

void CSigReceiptDataFile::SaveTimeStamp(const CTime& tmTime)
{
	unsigned long lBytesWritten,  lDataLen = sizeof(CTime);

	BOOL bResult = WriteFile(m_hSReceiptFile, &lDataLen , sizeof(long) , &lBytesWritten, NULL) ; 
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
	bResult = WriteFile(m_hSReceiptFile, &tmTime , lDataLen , &lBytesWritten, NULL) ; 
  if(!bResult)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
}

bool CSigReceiptDataFile::SaveToken(const CString& strToken)
{
  unsigned long lBytesWritten ; 
  BOOL bResult = ::WriteFile(m_hSReceiptFile, strToken , SCP_TOKEN_LENGTH , &lBytesWritten, NULL) ; 
  if(!bResult)
  {
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
    return false; 
  }
  else 
    return true; 
}

bool CSigReceiptDataFile::ReadToken(CString& strToken)
{
  unsigned long lBytesRead ;
  bool bReturn ;

  BOOL bResult = ::ReadFile(m_hSReceiptFile, strToken.GetBuffer(SCP_TOKEN_LENGTH/sizeof(TCHAR)) , SCP_TOKEN_LENGTH , &lBytesRead, NULL) ; 
  if(!bResult)
  {
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
    bReturn = false;
  }
  else
  {
	  if ( SCP_TOKEN_LENGTH != lBytesRead)
      bReturn = false;
    else
       {
       strToken.ReleaseBuffer(SCP_TOKEN_LENGTH/sizeof(TCHAR)) ;
       bReturn = true;
       }
  }

  return bReturn;
}

SIGCAP_TOKEN_TYPE CSigReceiptDataFile::ParseToken(const CString& strToken) 
{
  SIGCAP_TOKEN_TYPE TokenType ;

  if (strToken.CompareNoCase(SCP_SLIP_DATA_TOKEN)==0)
    TokenType =SIGCAP_SLIP_TOKEN ;
  else if (strToken.CompareNoCase(SCP_SIG_DATA_TOKEN)==0)
    TokenType =SIGCAP_SIG_TOKEN ;
  else if (strToken.CompareNoCase(SCP_FILE_END_TOKEN)==0)
    TokenType =SIGCAP_FILE_END_TOKEN ;
  else if (strToken.CompareNoCase(SCP_RECEIPTTYPE_TOKEN)==0)
	TokenType =SIGCAP_RECEIPT_TYPE;
  else
  {
    ASSERT(0) ;
    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("!!!Signature receipt file format is wrong.  Token:%s"), strToken) ;
    TokenType =SIGCAP_ERROR_TOKEN ;
  }
  return TokenType ;
}

bool  CSigReceiptDataFile::ReadString(CString& Str)
{
	unsigned long lBytesRead,  lDataLength;
	int nSuccess = ReadFile(m_hSReceiptFile, &lDataLength , sizeof(lDataLength), &lBytesRead, NULL) ; 
  if(!nSuccess)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
  else
  {
	  if ( sizeof(lDataLength) != lBytesRead)
      nSuccess = 0 ;
    else
	  {
		  nSuccess = ReadFile(m_hSReceiptFile, Str.GetBuffer(lDataLength/sizeof(TCHAR)) , lDataLength, &lBytesRead, NULL) ; 
		  Str.ReleaseBuffer(lDataLength / sizeof(TCHAR)) ;
      if(!nSuccess)
		    ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
	  }
  }
  return (nSuccess !=0) ;
}

bool CSigReceiptDataFile::ReadLong(long& lData)
{
	unsigned long lBytesRead,  lDataLength;
	int nSuccess = ReadFile(m_hSReceiptFile, &lDataLength , sizeof(lDataLength), &lBytesRead, NULL) ; 
  if(!nSuccess)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
  else
  {
    nSuccess = ReadFile(m_hSReceiptFile, &lData , lDataLength, &lBytesRead, NULL) ;
    if(!nSuccess)
		  ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
  }
  return (nSuccess !=0) ;
}

bool CSigReceiptDataFile::ReadTimeStamp(CTime& tmTime)
{
	unsigned long lBytesRead,  lDataLength, lData;
	int nSuccess = ReadFile(m_hSReceiptFile, &lDataLength , sizeof(lDataLength), &lBytesRead, NULL) ; 
  if(!nSuccess)
		ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
  else
  {
	  nSuccess = ReadFile(m_hSReceiptFile, &lData , lDataLength, &lBytesRead, NULL) ;
    if(!nSuccess)
		  ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
    else
	    tmTime = CTime(lData) ;
  }
  return (nSuccess !=0) ;
}

void CSigReceiptDataFile::SaveSignatureData(BSTR pSigData)
{
	ASSERT(m_bWrite) ;
	if (m_hSReceiptFile!=NULL && m_bWrite)		// make sure file has been created and it is write operation
	{
		CString csSigData ;
		unsigned long nBytesWritten, lSigLength;
		BOOL bSuccess ;

    // save slipdata here to avoid sig data is missing if saving them separately in different functions.
    //SaveToken(SCP_SLIP_DATA_TOKEN) ;
    //SaveString(m_csSlipData);  // save slip data
    SaveToken(SCP_SIG_DATA_TOKEN) ;
		lSigLength = :: SysStringByteLen(pSigData) ;
		bSuccess= WriteFile(m_hSReceiptFile, &lSigLength , sizeof(lSigLength) , &nBytesWritten, NULL); 
    if(!bSuccess)
		  ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
		bSuccess = WriteFile(m_hSReceiptFile, pSigData , lSigLength, &nBytesWritten, NULL) ; 
    if(!bSuccess)
		  ScotError(RETURN, SM_CAT_FILE_IO, SCOTAPP_SM_TB_FILE, m_csFileName) ;
	}
}

bool CSigReceiptDataFile::FileAvailable()
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
    bool bReturn=false;
    hFind = FindFirstFile(m_csFileName, &fd);
    if(INVALID_HANDLE_VALUE != hFind)
    {
       FindClose(hFind); //must close handle!
       bReturn=true;
    }
    return bReturn;
}

bool CSigReceiptDataFile::ConvertSig5991toBitmap(BSTR pRawData, LPCTSTR csBitmapFileName) 
{
	BYTE *PA;  
	long tpoints;
	bool bSuccess = true; 

	tpoints = CalculatePointArraySize(pRawData);
	if(tpoints == 0)
	{
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR ,_T("Error Point Array Size = 0")) ;	
		return false;
	}
    //allocate points array
	PA = new BYTE [tpoints * 4];
	if(!PA)
	{
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR ,_T("Memory Allocation Error")) ;	
		return false;
	}
	try   //tar 269978
	{ 
	   tpoints = ConvertSig5991toPoints(pRawData, PA) ;
	}
	catch(...)
	{
	   traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR ,_T("Failure in ConvertSig5991toPoints to create %d points from pRawData"), tpoints) ;	
	   try
	   {
	      _bstr_t tmpbstr(pRawData, FALSE); // Allow approximation of data length
	      to.HexTrace(L2, _T("pRawData"), (LPCTSTR)pRawData,tmpbstr.length());
	   }
	   catch(...) {}
	   bSuccess = false;
	}
	if (bSuccess)
	{
	    bSuccess = ConvertSigPointsToBitmap(PA, tpoints, csBitmapFileName);
	}
	if(PA)
		delete [] PA;
	return bSuccess;
}

bool CSigReceiptDataFile::PrintSigReceiptFile()
{
  int i = 0;
  CString strToken;
  BOOL bBadFile=false ;
  CString csReceiptType;

  OpenFile(false) ;
  //	BOOL bResult = PrintTransData() ; //TAR#138918 removal transdata from report, replace with slip data
  SIGCAP_TOKEN_TYPE eCurrentToken ;
  while(ReadToken(strToken))
  {
    eCurrentToken = ParseToken(strToken) ;
    if (SIGCAP_ERROR_TOKEN!= eCurrentToken)
    {
      switch(eCurrentToken)
      {
      case SIGCAP_SLIP_TOKEN:
        if ((m_ePreviousToken == eCurrentToken) && (m_ePrePreviousToken == eCurrentToken))
        {
          ASSERT(0) ;
          ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_DATA_MISSING,_T("%s|%d"),_T("Signature"), ++i);
          traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR, _T("Signature data is missing from this signature receipt")) ;
          dm.ReceiptCut() ;	// Cut receipt
          traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO,
               _T("Index of sigcap receipt in this batch printing:%d"), i) ;
        }
        PrintSlipData();
		m_ePrePreviousToken = m_ePreviousToken ; 
        m_ePreviousToken = eCurrentToken ;
        break;
      case SIGCAP_SIG_TOKEN:
        i++ ;
        if (m_ePreviousToken == eCurrentToken)
        {
          ASSERT(0) ;
          ScotError(RETURN, SM_CAT_3rdPARTY_INTERFACE, SCOTAPP_SC_DATA_MISSING,_T("%s|%d"),_T("Signature"), i);
          traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR, _T("Slip data is missing from this signature receipt")) ;
        }
        PrintSignatureData((LPCTSTR)csReceiptType) ;
	    // dm.ReceiptCut() ;	// Cut receipt
        traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO,
               _T("Index of sigcap receipt in this batch printing:%d"), i) ;
		m_ePrePreviousToken = m_ePreviousToken ; 
        m_ePreviousToken = eCurrentToken ;
        break;
	  case SIGCAP_FILE_END_TOKEN:
          dm.ReceiptCut() ;
		  break;
	  case SIGCAP_RECEIPT_TYPE:
		  m_ePrePreviousToken = m_ePreviousToken ; 
		  m_ePreviousToken = eCurrentToken ;
		  ReadString(csReceiptType);
		  break;
	  default:
      ASSERT(0) ;
      bBadFile = true ;
      break;

      }
    }
    else
    {
      //ParseToken already logged error
      ASSERT(0) ;
      bBadFile = true ;
      break;
    }
  }
  CloseFile() ;
  m_ePreviousToken = SIGCAP_ERROR_TOKEN ;  //reset it
  if (!bBadFile)
  {
    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO,
               _T("Signature receipt printing is finished.  Total receipts: %d"), i) ;
//TAR 195179 & 191490    BackupSigReceiptFile() ;
  }
  else
  {
    SaveBadSigFile() ;
    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO, _T("Bad signature receipt file has been saved")) ;
  }
  return (!bBadFile) ;
}

// print Transaction related data
void CSigReceiptDataFile::SaveBadSigFile()
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;
	CString  csBadFileName ;
	csBadFileName = m_csReceiptFilePath + CString(SIGRECEIPT_BADFILE_NAME);
    hFind = FindFirstFile(csBadFileName, &fd);
    if (hFind != INVALID_HANDLE_VALUE)
	{
        FindClose(hFind); // must close handle
		CFile::Remove(csBadFileName) ;
	}
	CFile::Rename(m_csFileName, csBadFileName) ;
}

// print Transaction related data
BOOL CSigReceiptDataFile::PrintTransData()
{
	CString strBuffer ;
	long lBuffer ;
	CTime tmBuffer ;

	if (!ReadString(strBuffer))  // transction number ;
	{
		return false ;	// end of the file
	}
	else
	{	
		// transction number ;
		// dm.ReceiptLine(ps.GetPSText(MSG_SCP_TRANSACTIONNUM) + strBuffer + _T("\n"));

		// lane number
		ReadLong(lBuffer) ;   ;
		strBuffer.Format(ps.GetPSText(MSG_SCP_LANENUM), lBuffer) ;
		dm.ReceiptLine(strBuffer + _T("\n"));
		strBuffer.Empty() ;

		// total charge
		ReadLong(lBuffer) ;    
		strBuffer = FormatCurrency(lBuffer) ;
		dm.ReceiptLine(ps.GetPSText(MSG_SCP_TOTAL_CHARGE) + strBuffer + _T("\n"));
		dm.ReceiptLine(_T("----------------------------------------\n"));
		strBuffer.Empty() ;

		// Credit Card #
		ReadString(strBuffer) ;	
		dm.ReceiptLine(ps.GetPSText(MSG_SCP_CREDIT_CARDNUM) + strBuffer);
		dm.ReceiptLine(_T("\n")) ;

		// Approval Code
		ReadString(strBuffer) ;	
		// dm.ReceiptLine(ps.GetPSText(MSG_SCP_APPROVAL_CODE) + strBuffer + _T("\n"));

		// szCreditCardOwnerName
		ReadString(strBuffer) ;
		strBuffer.TrimLeft() ;
		if (!strBuffer.IsEmpty()) 
		{
			dm.ReceiptLine(ps.GetPSText(MSG_SCP_OWNER_NAME) + strBuffer + _T("\n") );
		}

		ReadTimeStamp(tmBuffer) ;	// TimeStamp ;
		strBuffer = tmBuffer.Format(_T("%c")) ;
		dm.ReceiptLine(strBuffer + _T("\n"));
		dm.ReceiptLine(_T("----------------------------------------\n"));
		return true ;
	}
}

CString CSigReceiptDataFile::FormatCurrency(long lTotal) 
{
	CString csCurrencyStr = _T(""), sFormatString, sWidth;
	long lLeft, lRight, lUnit=co.nLocalePriceCentUnit, lDecimal=co.nLocaleNoOfDigitsAfterDecimal;

	if (lDecimal>0) 	// The number of decimal digits (needed to know if we work cents or not)
	{			
		lLeft	 = lUnit != 0 ?abs(lTotal/lUnit):lTotal;
		lRight = lUnit != 0 ?abs(lTotal%lUnit):0;
		sFormatString =	lTotal >= 0 ?_T("%li.%0"):_T("-%li.%0");
		sWidth.Format(_T("%li"), lDecimal);
		sFormatString += sWidth;
		sFormatString += _T("li");
		csCurrencyStr.Format(sFormatString, lLeft, lRight);
	}
	else
		csCurrencyStr.Format (_T("%li"), lTotal);
	
	return csCurrencyStr ; 
}

void CSigReceiptDataFile::PrintSignatureData(LPCTSTR lpReceiptType)
{
	BOOL bResult ;
	CString  csBitmapName;
	unsigned long lDataLength , lBytesRead;
  long lResult ;
  BYTE *btString;
	BOOL bProcessData = FALSE; // Initialization added for TAR 316837
	
	bResult = ::ReadFile(m_hSReceiptFile, &lDataLength , sizeof(lDataLength), &lBytesRead, NULL) ; 
	btString = new BYTE[lDataLength];
	bResult = ::ReadFile(m_hSReceiptFile, btString , lDataLength, &lBytesRead, NULL) ;
	
	// flip bitmap file name since dm does not work with same file name with different contents
    if (m_csTempBitmapFile.CompareNoCase(TMP_BITMAP_FILE1) == 0)
       m_csTempBitmapFile=TMP_BITMAP_FILE2 ;
    else
       m_csTempBitmapFile=TMP_BITMAP_FILE1 ;

    csBitmapName = m_csReceiptFilePath + m_csTempBitmapFile ;

	// We need to pass a real BSTR here, not just cast it.  1/31/2000 - ncr_barn
    BSTR sigData = SysAllocStringByteLen((const char *)btString, lDataLength );

	long rc;
    rc = tb.PreprocessSignatureData((LPCTSTR)csBitmapName, sigData, lpReceiptType, &bProcessData);
	if(rc == TB_SUCCESS && bProcessData)
	{
		SysFreeString(sigData);
  	    CString csBuffer ;

		lResult = dm.ReceiptBitMap(csBitmapName);   // print the bitmap file
		 
		if (lResult!=0L) 
		{
			traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_WARNING , _T("Signature Bitmap file print failed, Return Code:%l"), lResult) ;
		}
	  CFile::Remove(csBitmapName) ;
	//TAR 195179 & 191490 start
		SMStateBase::m_bSignaturePrintingStatus = true;
	//TAR 195179 & 191490 end
	}
	else
	{
		// TAR 189420 RJF011902 need to check results to avoid deleting non-existent bitmap file
		if(ConvertSig5991toBitmap(sigData, csBitmapName))
		{
		  SysFreeString(sigData);

		  CString csBuffer ;

		  lResult = dm.ReceiptBitMap(csBitmapName);   // print the bitmap file
		  if (lResult!=0L) 
		  {
			traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_WARNING , _T("Signature Bitmap file print failed, Return Code:%l"), lResult) ;
		  }
		  CFile::Remove(csBitmapName) ;
	//TAR 195179 & 191490 start
		  SMStateBase::m_bSignaturePrintingStatus = true;
	//TAR 195179 & 191490 end
		}
		else
		{
		  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_WARNING ,_T("Bitmap not created, continuing print job")) ;	
		  SysFreeString(sigData);
		}  // End of Tar 189420 RF011902
	}
	delete btString;
}

// backup the sig-receipt file to log file and log file to backup file
void CSigReceiptDataFile::BackupSigReceiptFile()
{

    HANDLE hFind;
    WIN32_FIND_DATA fd;
	CString csLogFile = m_csReceiptFilePath + CString(SIGRECEIPT_LOG_NAME) ;
	CString csBackupFile = m_csReceiptFilePath + CString(SIGRECEIPT_BACKUP_NAME) ;

    hFind = FindFirstFile(csLogFile, &fd);
    if (hFind == INVALID_HANDLE_VALUE) // cannot find the file
	{
		CFile::Rename(m_csFileName, csLogFile) ;
	}
	else
	{
        FindClose(hFind); //must close handle
        if ((hFind = FindFirstFile(csBackupFile, &fd)) != INVALID_HANDLE_VALUE)
		{
            FindClose(hFind); //must close handle
			CFile::Remove(csBackupFile) ;
		}
		CFile::Rename(csLogFile, csBackupFile) ;
		CFile::Rename(m_csFileName, csLogFile) ;
	}
}

long CSigReceiptDataFile::CalculatePointArraySize(BSTR pRawData)
{
	SAFEARRAY *psa;
	VectorFromBstr(pRawData, &psa);   
   
	BYTE *pSigData;
	int signature_index = 0;
	long tpoints = 0;
	short stroke_vli;

	signature_index = (*psa).rgsabound[0].cElements;
	pSigData = new BYTE [signature_index];
	if(!pSigData)
	{
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR ,_T("Memory Allocation Error")) ;	
		return 0;
	}

	memcpy(pSigData, (*psa).pvData, signature_index);

    DWORD signature_length = signature_index;   
	DWORD i=3; //skip sig data header
    //calculate size needed for points array
	while (++i < signature_length)
	{
		stroke_vli  =   ((short)((pSigData[i])) & 0x000F) * 256;
		stroke_vli  +=  ((short)(pSigData[++i]) & 0x00FF) - 1;  

		i+=4;
		tpoints++;

		for (int j=0; j<((short)stroke_vli & 0x0FFF); j++)
		{
			if(!(j % 4))
				i+=2;
			else
				i++;
			tpoints++;
		} 
         
		tpoints++;
	}
	if(pSigData)
		delete [] pSigData;

	return tpoints;
}

// Convert SignatureData from 5991 format to Points. result will be saved in to PA array.  It returns the number of points
// The origin of the source is SignatureLane control in RAP

// The VC++ 6.0 Optimizer causes this following function to screw up some of the point
// bits.  I have spent hours trying to figure out where, and am tired of it.
// Disabling optimizations for this function fixes the problem and since this
// is not a high-volume function (only called at EOD when printing sigs), it should be
// okay if it runs a few milliseconds slower.  1/31/2000 - ncr_barn

#pragma optimize( _T(""), off )

long CSigReceiptDataFile::ConvertSig5991toPoints(BSTR pRawData, BYTE* PA) 
{
   // cut and paste from GetSignatureFrom5991 in 5991comm.cpp; my code is marked with YXIE
	SAFEARRAY *psa;
   
	VectorFromBstr(pRawData, &psa);   
   
	BYTE *signature_data;
	int signature_index = 0;				// possibly optimize and clean later ...
	int pa_index = 0;
	long tpoints = 0;

   // set it up so that we can use the exact same code as in 5991comm.cpp later
	signature_index = (*psa).rgsabound[0].cElements;
	signature_data = new BYTE [signature_index];
	if(!signature_data)
	{
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR ,_T("Memory Allocation Error")) ;	
		return 0;
	}

	memcpy(&signature_data[0], (*psa).pvData, signature_index);
	//
	// Parse compressed data into (x,y) pairs .... algorithm from 5991 Interface Document
	//

   // Other variables for parsing ....
	BYTE sighead[4];
	BYTE shift, error_adj, work, x_msb, x_lsb, y_msb, y_lsb;
	DWORD signature_length = signature_index;
	short	stroke_head, stroke_vli;
	char	x1,y1;
	short   x,y;
	DWORD i=0;

	sighead[0] = signature_data[i];
	sighead[1] = signature_data[++i];
	sighead[2] = signature_data[++i];
	sighead[3] = signature_data[++i];

	shift = 3 - (sighead[3] & 0x0F);
	if (shift)
		error_adj = 0x01 << (shift - 1);
	else
		error_adj = 0;

	while (++i < signature_length)
	{
		stroke_head =   ((short)(signature_data[i]))  & 0x00F0;
		stroke_vli  =   ((short)((signature_data[i])) & 0x000F) * 256;
		stroke_vli  +=  ((short)(signature_data[++i]) & 0x00FF) - 1;  // the number of offsets following start point

		// get start y co-ord
		y_lsb = signature_data[++i];
		y_msb = signature_data[++i];

		// get start x co-ord
		x_lsb = signature_data[++i];
		x_msb = signature_data[++i];

		y = (y_msb << 8) + y_lsb;
		x = (x_msb << 8) + x_lsb;

		PA[pa_index] = x_lsb;
		pa_index++;
		PA[pa_index] = x_msb;
		pa_index++;
		PA[pa_index] = y_lsb;
		pa_index++;
		PA[pa_index] = y_msb;
		pa_index++;

		tpoints++;

		for (int j=0; j<((short)stroke_vli & 0x0FFF); j++)
		{
			switch (j % 4)
			{
				// Bit pattern : |yyyyyxxx|xx------|
				case 0:
					work = signature_data[++i] & 0xF8;
					y1 = ((char)work) >> 3;
					work = signature_data[i] << 5;
					work |= (signature_data[++i] & 0xC0) >> 3;
					x1 = ((char)work) >> 3;
					break;

				// Bit pattern : |--yyyyyx|xxxx----|
				case 1:
					work = (signature_data[i] & 0xFE) << 2;
					y1 = ((char)work) >> 3;
					work = signature_data[i] << 7;
					work |= (signature_data[++i] & 0xF0) >> 1;
					x1 = ((char)work) >> 3;
					break;

				// Bit pattern : |----yyyy|yxxxxx--|
				case 2:
					work = signature_data[i] << 4;
					work |= (signature_data[++i] & 0x80) >> 4;
					y1 = ((char)work) >> 3;
					work = (signature_data[i] & 0xFE) << 1;
					x1 = ((char)work) >> 3;
					break;

				// Bit patterm : |------yy|yyyxxxxx|
				case 3:
					work = signature_data[i] << 6;
					work |= (signature_data[++i] & 0xE0) >> 2;
					y1 = ((char)work) >> 3;
					work = signature_data[i] << 3;
					x1 = ((char)work) >> 3;
					break;
			}

			x1 = (x1 << shift) | error_adj;
			y1 = (y1 << shift) | error_adj;

			x += x1;
			y += y1;

			// End Point from previous pt.
			PA[pa_index] = (BYTE)(x & 0x00FF);
			pa_index++;
			PA[pa_index] = (BYTE)((x & 0xFF00) >> 8);
			pa_index++;
			PA[pa_index] = (BYTE)(y & 0x00FF);
			pa_index++;
			PA[pa_index] = (BYTE)((y & 0xFF00) >> 8);
			pa_index++;

			tpoints++;

		} // End Stroke

		PA[pa_index] = 0xFF;
		pa_index++;
		PA[pa_index] = 0xFF;
		pa_index++;
		PA[pa_index] = 0xFF;
		pa_index++;
		PA[pa_index] = 0xFF;
		pa_index++;

		tpoints++;

	} // End Signature ....
	if(signature_data)
		delete [] signature_data;

	return tpoints ;
}

#pragma optimize( _T(""), on )


bool CSigReceiptDataFile::ConvertSigPointsToBitmap(BYTE* Points, long lPointCount, LPCTSTR BitmapFileName)
{
	long minX=DEGITIZER_WIDTH,  maxX=0;		// initialization
	long minY=DEGITIZER_HEIGHT, maxY=0;		// initizlization 

	long bmWidth;							// bitmap width 
	long bmHeight;							// bitmap height 
	long sigWidth;							// signature bounding width
	long sigHeight;							// signature bounding height

	long x1, x2, y1, y2;					// coordinates
	long x, y;

	BYTE x_lsb, x_msb, y_lsb, y_msb;	

	long totalPts=lPointCount;

    traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("Point Counts: %ld"), lPointCount) ;

	CFile *f;	
	try 
	{
		f = new CFile(BitmapFileName, CFile::modeCreate | CFile::modeWrite ); // open a file
	}
	catch( CFileException* e )
	{
		ASSERT(0) ;
		e->Delete();
		traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("new CFile exception")) ;
		return false;
	}


	// Parse signature for bounds
	for (int i = 0; i < totalPts; i++)
	{
		x_lsb = *(Points+(i*4));
		x_msb = *(Points+(i*4+1));
		y_lsb = *(Points+(i*4+2));
		y_msb = *(Points+(i*4+3));

 		if ((x_msb == 255) && (x_lsb == 255))
		{
			// Pen-Lifted ... get next start point
			if (!(i==totalPts - 1))
			{
				// should do nothing, otherwise it will skip the starting point of next stroke, which
				// could be a boundary point
			}	
		}
		else
		{
			x = (x_msb*256 + x_lsb);
			y = (y_msb*256 + y_lsb);
			
			minX = min(x, minX);
			maxX = max(x, maxX);
			minY = min(y, minY);
			maxY = max(y, maxY);	
		}
	}

	// Get signature width and height
	sigWidth = maxX - minX + 1;
	sigHeight = maxY - minY + 1;

	// Use signature bounds to change bitmap size so that it fits in the window. 
	// If it hits the left or right side first, then use the new ratio to calculate 
	// the bitmap height. Else, use the new ratio to calculate the bitmap width. 
	// Note that aspect ratio remains the same.
	if (sigHeight * WINDOW_WIDTH / sigWidth <= WINDOW_HEIGHT) 
	{
		bmWidth = WINDOW_WIDTH;
		bmHeight = sigHeight * WINDOW_WIDTH / sigWidth;
	}
	else
	{
		bmHeight = WINDOW_HEIGHT;
		bmWidth = sigWidth * WINDOW_HEIGHT / sigHeight;
	}

	// Get ratios for resizing (x, y) coordinate so that it fits in bitmap boundary
	double x_adj = (double)(bmWidth - 1)/(double)(sigWidth - 1);
	double y_adj = (double)(bmHeight -1)/(double)(sigHeight - 1);	
	
	// Pad bitmap's pixel width to 4-byte boundary	
	DWORD dwBytes = (DWORD)bmWidth/32;
	if (((DWORD)bmWidth)%32) 
		dwBytes++;
	dwBytes *= 4;
	// traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , "bmHeight: %d, dwBytes: %d.", bmHeight, dwBytes) ;
	// Initialize all bits in the bitmap data block to 1 as white
	for (i=0; i < bmHeight; i++)
	{
		for (int j=0; j <= (int)dwBytes - 1; j++) 
			arrayBitmapData[i][j] = 0xff;
	}

	// Parse the signature data and plot points in the bitmap data 
	// Get first point
	x_lsb = *Points;
	x_msb = *(Points+1);
	y_lsb = *(Points+2);
	y_msb = *(Points+3);

// We have to flip the Y values because this code was always drawing sigs
// from devices that were out of the OPOS Spec with regard to the Y origin.
// Now that we have fixed the Topaz device and are using the 5992, which is 
// correctly setting the Y axis origin, we have to flip the y Axis in this 
// bitmap creation code, which is what the NEW_Y_VAL macro does.  The old
// code is left in the next comment so it can be used for reference.
// ncr_barn - 01/28/2000

#define NEW_Y_VAL1(YMSB, YLSB) ((long)(((YMSB*256 + YLSB) - minY)*y_adj))   //topaz
#define NEW_Y_VAL2(YMSB, YLSB) ((long)((maxY - (YMSB*256 + YLSB))*y_adj))    //5992


	x1 = (long)(((x_msb*256 + x_lsb) - minX)*x_adj);

	y1 = NEW_Y_VAL2(y_msb, y_lsb);

	// Read through rest of the points
	for (i = 1; i < totalPts; i++)
	{
		x_lsb = *(Points+(i*4));
		x_msb = *(Points+(i*4+1));
		y_lsb = *(Points+(i*4+2));
		y_msb = *(Points+(i*4+3));
	
		while (   (i < totalPts)  // TAR 267666 - handle back-to-back FFFF for X values
		       && (x_msb == 255) && (x_lsb == 255) )
		{
			// Pen-Lifted ... get next start pt ....
			if (!(i==totalPts - 1))
			{
				i++;
				
				x_lsb = *(Points+(i*4));
				x_msb = *(Points+(i*4+1));
				y_lsb = *(Points+(i*4+2));
				y_msb = *(Points+(i*4+3));
	
				x1 = (long)(((x_msb*256 + x_lsb) - minX)*x_adj);
		
				y1 = NEW_Y_VAL2(y_msb, y_lsb);
			}
			else
			{
				// Signature ends with FFFF for X values; force loop end
				i++;
			}
		}
		if (i < totalPts)
		{
			x2 = (long)(((x_msb*256 + x_lsb) - minX)*x_adj);
	    
	         y2 = NEW_Y_VAL2(y_msb, y_lsb);

				
			// Draw a line to connect points (x1, y1) and (x2, y2)
			ConnectTwoPoints(x1, y1, x2, y2);

			x1 = x2;
			y1 = y2;
		}
	}

	BITMAPFILEHEADER	bm_filehdr;
	BITMAPINFOHEADER	bm_infohdr;
	COLORTABLE			colors;		

	// Fill in all bitmap header info and create the bitmap from the signature
	bm_filehdr.bfType = 0x4d42;			// File type as "BM"
	bm_filehdr.bfSize = dwBytes*bmHeight + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(COLORTABLE); // File size in bytes
	bm_filehdr.bfReserved1 = 0;			// Must be 0
	bm_filehdr.bfReserved2 = 0;			// Must be 0
	bm_filehdr.bfOffBits = bm_filehdr.bfSize - dwBytes*bmHeight; // Offset 
		
	bm_infohdr.biSize = sizeof(BITMAPINFOHEADER);
	bm_infohdr.biWidth = bmWidth;		// The width of the bitmap, in pixels
	bm_infohdr.biHeight = bmHeight;		// The height of the bitmap, in pixels
	bm_infohdr.biPlanes = 1;			// Set to 1 as required
	bm_infohdr.biBitCount = 1;			// 1 bit per pixel for mono bitmap
	bm_infohdr.biCompression = 0;		// Uncompressed format (for top-down bitmaps)
	bm_infohdr.biSizeImage = 0;			// Set to 0 for uncompressed format
	bm_infohdr.biXPelsPerMeter = 0;		// Horizontal resolution, in pixels per meter
	bm_infohdr.biYPelsPerMeter = 0;		// Vertical resolution, in pixels per meter
	bm_infohdr.biClrUsed = 0;			// Set to 0 to use monochrome color indices
	bm_infohdr.biClrImportant= 0;		// All colors used are important

	colors.color1.rgbBlue = 0;			// Black 
	colors.color1.rgbGreen = 0;
	colors.color1.rgbRed = 0;
	colors.color2.rgbBlue = 0xff;		// White 
	colors.color2.rgbGreen =0xff;
	colors.color2.rgbRed = 0xff;
	if (f)
	{
		try									
		{
			// Write header info to the file
			f->Write(&bm_filehdr, sizeof(BITMAPFILEHEADER));
			f->Write(&bm_infohdr, sizeof(BITMAPINFOHEADER));
			f->Write(&colors, sizeof(COLORTABLE));			
			// Write bitmap data to the file
			for (i =0 ; i<=bmHeight-1; i++)		
				f->Write(arrayBitmapData[i], (int)dwBytes);
		}
		catch( CFileException* e )
		{
			ASSERT(0) ;
			e->Delete();
			traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , _T("Write CFile exception"));
		}	
		
		f->Close() ;
		delete f;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
// CDevMgrCtrl::ConnectTwoPoints
// 
// Fills in all points between (x, y) and (x1, y1) using some mathematical functions so 
// that (x,y) and (x1, y1) can be connected logically.
//
void CSigReceiptDataFile::ConnectTwoPoints(long x, long y, long x1, long y1)
{
	int i,j,delt,dir;
	double slope,calc;

	if(x == x1 && y == y1) return;
	
	if(x == x1) 
	{
		for(j = min(y,y1); j <= max(y,y1); j++)
			PlotPointInBitmapData(x,j);
		return;
	}
	else if(y == y1) 
	{
		for(i = min(x,x1); i <= max(x,x1); i++)
			PlotPointInBitmapData(i,y);
		return;
	}

	if(y1 > y) {dir = 1;}		// Draw from lower to upper
	else {dir = -1;}			// Draw from upper to lower

	// Calculate the slope using the current point
	slope = (double)abs(y1-y)/(double)abs(x1-x);

	// Go through as many points as the value in the slope
	for(i = min(x,x1); i <= max(x,x1); i++) 
	{	
		calc=abs(i-x)*slope;

		if(fabs(calc-floor(calc)) >= fabs(ceil(calc)-calc)) 
		{
			delt = (int)ceil(calc);		// round up
		}
		else {delt = (int)floor(calc);}	// round down
		PlotPointInBitmapData(i,y+(delt*dir));
	}

	if(x1 > x) {dir = 1;}		// Draw from left to right
	else {dir = -1;}			// Draw from right to left	

 	// Calculate the slope using the current point
	slope = (double)abs(x1-x)/(double)abs(y1-y);

 	// Go through as many points as the value in the slope
	for(i = min(y,y1); i <= max(y,y1); i++) 
	{
		calc=abs(i-y)*slope;

		if(fabs(calc-floor(calc)) >= fabs(ceil(calc)-calc)) 
		{
			delt = (int)ceil(calc);			// round up
		}
		else {delt = (int)floor(calc);}		// round down
		PlotPointInBitmapData(x + (delt*dir),i);
	}
}


void CSigReceiptDataFile::PlotPointInBitmapData(long x, long y)
{
	_TUCHAR c;			// Temporary for value to AND in
	long xcord, xofs;

	xcord = x/8;	
	xofs =  x%8;	

	// Determine the value usef to plot a point in the bitmap data 
	c = 1 << (7 - xofs);

	// Take the above value, flip the bits, and AND it into the correct place
	// in the bitmap data block. 
//  traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_INFO , "y: %d, xcord: %d.", y, xcord) ;
  ASSERT(y>=0 && xcord>=0) ; // prevent programming error from happening
  if (y<0)
    arrayBitmapData[0][xcord] &= ~c;  //make sure the is not negative
  else
    arrayBitmapData[y][xcord] &= ~c;
}

void CSigReceiptDataFile::SaveSigCapSlipData(HFILE hFile, BSTR pSignatureData)
{
  CString csSlipData;
  try
  {
      if (hFile!=NULL)
	  {
          DWORD	len;
          LPTSTR pBuff;

#if _MFC_VER < 0x0700
          CFile	cFile( hFile );
#else
          CFile	cFile( (HANDLE)hFile );  
#endif

          len = (DWORD)cFile.SeekToEnd();
          cFile.SeekToBegin();

          if (len)
          {
              pBuff = new _TCHAR [len+1];
              if (pBuff)
              {
                  memset(pBuff, 0, len+1);
                  UINT c = cFile.Read(pBuff, len);
                  if (c)
                  {
                      pBuff[len]  = 0;
                      CString csWholePrintData(pBuff, len);
                      csSlipData = csWholePrintData;
                  }
                  delete [] pBuff;
              }
          }
      }
  }
  catch(CFileException *e)
  {
      // may be an invalid file handle
      _TCHAR szError[255] = {NULL};
      e->GetErrorMessage( szError, 255 );
      ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%d|%s"),hFile, szError);
      e->Delete();
  }
  // need to delay slip data saving to avoid sig data missing because of state transition
  ProcessSlipData(csSlipData, pSignatureData,_T("SIGNATURE"));  // modified because ProcessSlipData function was modified

  m_csSlipData = csSlipData ;

}




void CSigReceiptDataFile::ProcessSlipData(CString& csSlipData, BSTR pSignatureData, LPCTSTR lpReceiptType)
{
  int nCurrentTokenStartPos = -1;
  //unsigned long nBytesWritten, lSigLength;

  if (m_hSReceiptFile!=NULL && m_bWrite)		// make sure file has been created and it is write operation
  {
	  //tar 239361++
	  nCurrentTokenStartPos = GetSigTokenStartPos(csSlipData);
	  //tar 239361++
	  SaveToken(SCP_RECEIPTTYPE_TOKEN) ;
	  CString csReceiptType = lpReceiptType;
	  SaveString(csReceiptType);
				  
	  if(nCurrentTokenStartPos>-1)   //Found Signature Token
	  {
 
		if(csSlipData.Left(nCurrentTokenStartPos))    //save data before SigToken
		{   
			SaveToken(SCP_SLIP_DATA_TOKEN) ;
			SaveString(csSlipData.Left(nCurrentTokenStartPos));  // save slip data
		}    

		SaveSignatureData(pSignatureData);

		if(csSlipData.Mid(nCurrentTokenStartPos+15))
		{
			SaveToken(SCP_SLIP_DATA_TOKEN) ;
			SaveString((csSlipData.Mid(nCurrentTokenStartPos+15))); // save slip data
		}
		SaveToken(SCP_FILE_END_TOKEN);
	  }
	  else    // Not found token, print signature after slip data
	  { 
			SaveToken(SCP_SLIP_DATA_TOKEN) ;
			long lSigLength =::SysStringByteLen(pSignatureData);
			if(lSigLength!=0)		
			{                         
				CString csBuffer;
				csBuffer = ps.GetPSText(MSG_SCP_SIGNATURE) ;
				SaveString(csSlipData + csBuffer);  // save slip data
				SaveSignatureData(pSignatureData);
				SaveToken(SCP_SLIP_DATA_TOKEN) ;
                SaveString(_T("\n\n----------------------------------------\n\n"));

			}
			else                        // no signature data
			{
				SaveString(csSlipData);
			}
			SaveToken(SCP_FILE_END_TOKEN);
	  }
  }

  else 
  { 
	 traceNew(TRACE_FIXED_HEADER, TRACE_LEVEL_ERROR, _T("Error: slip receipt is not created. Can not save data to it")) ;
  }
  return;
}


BOOL CSigReceiptDataFile::PrintSlipData()
{
	CString strBuffer;

	if (ReadString(strBuffer))  // slip data string ;
	{
		//B TAR264477 
		//Don't print the unicode byte order mark
		if (strBuffer.Left(1) == 0xFEFF) //unicode file byte-order-mark
		{
			strBuffer = strBuffer.Mid(1);
		}
		//E TAR264477

		dm.ReceiptLine(strBuffer + _T("\n"));
		strBuffer.Empty();
        return TRUE;
	}
    else return FALSE;
}

//////////////////////////////////////////////////////
// other utilities
///////////////////////////////////////////////////////


BOOL CSigReceiptDataFile::EnterSignaturesFromFile(CString csFileName)
{
    BOOL bRetVal;
	CStdioFile stdioReportFile;
    CString csOutputFileName;
    CString csInputString;
    bool bKeepGoing = true;
    int spaceIndex;
    CString csIndex;
    CString csData;
    CString csHexSlip;
    CString csHexSignature;
    BSTR    pData;
	BYTE   *btString;
    int     iSigLength;
	unsigned int uiOpenMode;
	bool bSlipPending = false;
	CString csType = _T("SIGNATURE");
	bool bOKtoStop = true; // TAR 272622 - Handle null file properly

#ifdef UNICODE
	uiOpenMode = CFile::modeRead | CFile::typeBinary;
#else
	uiOpenMode = CFile::modeRead | CFile::typeText;
#endif
    try
    {
        OpenFile(true) ;
        csOutputFileName =  csFileName;
        if( TRUE == stdioReportFile.Open( csOutputFileName, uiOpenMode ) ) 
        {
            while (bKeepGoing)
            {
                if (TRUE == stdioReportFile.ReadString(csInputString))
                {
                    spaceIndex = csInputString.Find(_T(" "));
	                csData = csInputString.Mid(spaceIndex+1 );
                    csIndex = csInputString.Left(spaceIndex);
                   
					if (_T("ST") == csIndex) // Slip Type
                    {
						if(bSlipPending)
						{
							ProcessSlipData(csHexSlip, NULL, csType);
							csType=csData;
							bSlipPending = false; 
						}
						else
						{
							csType=csData;
						}
						bOKtoStop = false; // Expecting slip data next, end of file should not occur at this time
                    }
                   	else if (_T("SL") == csIndex) // Slip data
					{
						iSigLength = AsciiToHex(csData, btString);
						if (NULL != btString)
						{
                            //+RFC 368222
                            bool bUnicode = SMStateBase::IsReceiptUnicodeData ((unsigned char*)btString, iSigLength);
                            if ( bUnicode )
                            {
                                CString csTemp((LPCWSTR)btString, (iSigLength/sizeof(wchar_t)));
                                csHexSlip = csTemp;
                            }
                            else
                            {
                                CString csTemp((LPCSTR)btString, (iSigLength/sizeof(char)));
                                csHexSlip = csTemp;
                            }
                            //-RFC 368222

							delete [] btString;
						}
						bSlipPending = true;
						bOKtoStop = true; // end of file can occur now, but pending slip is processed before exit 

					}
					else if (_T("SI") == csIndex) // Signature data
					{
						iSigLength = AsciiToHex(csData, btString);
                        pData = SysAllocStringByteLen((const char *)btString, iSigLength );
                        ProcessSlipData(csHexSlip, pData, csType);
						delete [] btString;
                        SysFreeString(pData);
						bSlipPending = false;
						bOKtoStop = true;  // end of file can occur at this time
					}
					
                }
                else
                {
                    bKeepGoing = false;
					if(bSlipPending)
					{
						ProcessSlipData(csHexSlip, NULL, csType);
					}
					if(bOKtoStop) 
					{
						bRetVal = TRUE; 
					}
					else
					{
						bRetVal = FALSE; 
					}
                }
              
            }
            stdioReportFile.Close();
        }
        else
        {
            // Error occurred opening file
            bRetVal = FALSE;
        }
        CloseFile();
        return (bRetVal);
    }
    catch (...)
    {
        return FALSE;                   
    }
}

// Companion to HexToAscii method in Reporting.cpp
long CSigReceiptDataFile::AsciiToHex(CString csInString, BYTE *&btOutString)
{
    int iStringLength = csInString.GetLength();
    int iOutLength = 0;
    BYTE *pBuff;
    _TCHAR cNextChar;
	BYTE cNextByte;
    BYTE cHexChar;

	btOutString = NULL;
    if (iStringLength)
    {
        // If the string contains data, begin the translation.
        // Allocate a buffer long enough to hold the data
        pBuff = new BYTE[iStringLength];
        if (pBuff)
        {
            // Go through each character in the buffer
            cHexChar = 0;
            for (int i = 0; i < iStringLength; i++)
            {
                cNextChar = csInString.GetAt(i);
				cNextByte = (BYTE)(cNextChar & 0xFF);
                if (cNextByte < 0x40)  // if less than A
                {
                    cHexChar |= (cNextByte - 0x30);
                }
                else
                {
                    cHexChar |= (cNextByte - 0x37);
                }
                if (i % 2)
                {
                    // If this is the second nibble, save it
                    pBuff[iOutLength] = cHexChar;
                    cHexChar = 0;
                    iOutLength++;
                }
                else
                {
                    // If this is the first nibble, shift it
                    cHexChar <<= 4;
                }

            }
            // Add a null to the end
            pBuff[iOutLength]  = 0;
			btOutString = pBuff;
        }

    }
    return iOutLength;

}

//tar 239361 Need algorithm to search for signature token that can handle
//null characters in string
int CSigReceiptDataFile::GetSigTokenStartPos(const CString& csSlipData) const
{
    int nTokenStartPos = -1;
    int nIndex = 0;
    int nTokenLen = (int) _tcslen(SIGTOK);
    int nMax = csSlipData.GetLength() - nTokenLen;
    
    while(nIndex <= nMax)
    {
        if (csSlipData.GetAt(nIndex) == SIGTOK[0])
        {
            if(csSlipData.Mid(nIndex, nTokenLen) == SIGTOK)
            {
                nTokenStartPos = nIndex;
                break;
            }
        }
        nIndex++;
    }

    return nTokenStartPos;
}

