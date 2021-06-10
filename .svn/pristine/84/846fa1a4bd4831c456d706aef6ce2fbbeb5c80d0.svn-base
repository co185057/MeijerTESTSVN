// DMPrinterUtilsBase.cpp: implementation of the CDMPrinterUtilsBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMPrinterUtilsBase.h"
#include <OposPtr.hi>
#include "CmFunctions.h"

#ifndef _CPPUNIT
#include "DllDefine.h"

#include "Common.h"
#include "CommonNCR.h"
#include "CommonServices.h"
#include "dmx\printer.h"

//SR672+
#include "DeviceErrorLookUpAPI.h" 
#include "OposPtr.h"
#include <OposPtr.hi>
//SR672-

#include "DMXAdapter.h" //SR700
#include "PrinterAdapter.h" //SR700

#else
#include "FakeCommon.h"
#include "FakePrinter.h"
#include "FakeDevMgr.h"
#endif


#define DEVICEID0 0
#define DEVICEID1 1

#define COMP_ID ID_DM							  // used by the trace() calls to identify this component
#define T_ID    _T("DMPrinterUtils")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// macro to handle exception processing using multiple catch{}'s
// pass return value if exception caught
#define PROCESS_EXCEPTIONS_EX(i,n)                           \
	catch(COleDispatchException * e)                            \
{                                                              \
  rc = dm.OleDispatchException(e,i,n,msg,__LINE__);                 \
  e->Delete();                                                 \
}                                                              \
	catch(COleException* e)                                     \
{                                                              \
  rc = -2;                                                     \
  dm.OleException(e,i,n,msg,__LINE__);                              \
  e->Delete();                                                 \
}                                                              \
	catch(CException* e)                                        \
{                                                              \
  rc = -2;                                                     \
  dm.Exception(e,i,n,msg,__LINE__);                                 \
  e->Delete();                                                 \
}                                                              \
	catch(...)                                                  \
{                                                              \
  rc = -2;                                                     \
  long le = GetLastError();                                    \
  ScotError(RETURN,SM_CAT_INTERNAL_INTERFACE, SCOTAPP_DMINTERFACE, _T("%s|%d"),msg,le);\
}
// Same but with DeviceId assumed to be zero
#define PROCESS_EXCEPTIONS(i) PROCESS_EXCEPTIONS_EX(i,0)



_TCHAR CDMPrinterUtilsBase::szTokenList[] = {BARCODETOK_START, BMPTOK_START, BMPTOK_END,_T('\0')};
static _TCHAR szWork[255];

PRNOBJECTTOKENS FindPrnTokenType(_TCHAR chTokenPrefix);

static long rc;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDMPrinterUtilsBase::CDMPrinterUtilsBase()
{
    
}

CDMPrinterUtilsBase::~CDMPrinterUtilsBase()
{
    
}

//////////////////////////////////////////////
// Printer functions
long CDMPrinterUtilsBase::ReceiptLine(const CString& csPrintLine) // print a line
{
    trace(L6,_T("+ReceiptLine"));
    dm.nDMLastReceiptFunction = 1;
    rc = ProcessPrintLine(DMCLASS_RECEIPT,csPrintLine);
    trace(L6,_T("-ReceiptLine 0x%x"),rc);
    return rc;
}

long CDMPrinterUtilsBase::ProcessPrintLine(int nDeviceClass, const CString& csPrintLine)
{
    CString csWork = csPrintLine;
    int nPrintLineLen = csWork.GetLength();
    if (!nPrintLineLen)
    {
        // no work??? 
        return OPOS_SUCCESS;
    }
    long rc = OPOS_E_FAILURE;
    int nCurrentTokenStartPos, nCurrentTokenEndPos;
    nCurrentTokenStartPos = -1;
    nCurrentTokenEndPos = -1;
    bool bContinueParsing = false;
    
    TCHAR ch;
    for(nCurrentTokenStartPos = 0; nCurrentTokenStartPos < csWork.GetLength(); nCurrentTokenStartPos++)
    {
        ch = csWork.GetAt(nCurrentTokenStartPos);
        if(ch == _T('\x04') || ch == _T('\x05') || ch==_T('\x06'))
        {
            bContinueParsing = true;
            break;
        }
    }
    
    
    while (bContinueParsing)
    {
        
        rc = dm.PrintLine(nDeviceClass, csWork.Left(nCurrentTokenStartPos));
        csWork = csWork.Mid(nCurrentTokenStartPos); // include start token
        
        rc = ProcessPrintTokens(nDeviceClass, csWork);
        //nCurrentTokenStartPos = csWork.FindOneOf(szTokenList);
        bContinueParsing = false;
        for(nCurrentTokenStartPos = 0; nCurrentTokenStartPos < csWork.GetLength(); nCurrentTokenStartPos++)
        {
            ch = csWork.GetAt(nCurrentTokenStartPos);
            if(ch == _T('\x04') || ch == _T('\x05') || ch==_T('\x06'))
            {
                bContinueParsing = true;
                break;
            }
        }
        
    }
    
    
    // now print the last piece of normal data buffer
    if (!csWork.IsEmpty())
    {
        rc = dm.PrintLine(nDeviceClass, csWork);
    }

    
    CPrintStation* pPrinter = dm.GetPrintStation( nDeviceClass );
    CPrinterWrapper& cPrinterWrapper= pPrinter->GetPrinter();
    if ( cPrinterWrapper.GetAsyncMode() == FALSE )
    {
        if ( pPrinter != NULL && pPrinter->GetState() != OPOS_S_BUSY )
        {
            rc = pPrinter->PrintSynchronous();
            if ( rc == OPOS_SUCCESS)
            {
                mo.PostDM(DM_STATUS, nDeviceClass, pPrinter->m_nDeviceId, DM_PTR_REQUEST_COMPLETED, NULL);
            }
        }
    }


    return rc;
}

//****************************************************************************************
//  TB appends bitmap or barcode token to the receipt file and SCOT App instructs
//  printer to print those objects. If any paramter is set to zero, SCOT App uses appropriate 
//  default values.
//
// Bitmap TokenFormat:
//  ____________________________________________________________
// |			  |	       |        |              |            |
// |  BMPTOKSTART |  WIDTH |  ALIGN |   BMPNAME    |  BMPTOKEND | 
// |_____________ |________|________|______________|____________|
//
// Note that the bitmap name should include the complete path to print the bitmap
//
//
// Barcode TokenFormat:
//  ____________________________________________________________________________________
// |			 |	    |        |        | print numbers    |             |            |
// |  BCTOKSTART | TYPE | HEIGHT |  ALIGN |   at the bottom  | BarcodeData |  BCTOKEND  | 
// |_____________|______|________|________|__0 or 1__________|_____________|____________|
//
//////////////////////////////////////////////////////////////////////////////////////////////
long CDMPrinterUtilsBase::ProcessPrintTokens(int nDeviceClass, CString& CurrentPrintLine)
{
    PRNOBJECTTOKENS CurrentTokenType;
    int nTokenEndPos = -1;
    long rc = OPOS_E_FAILURE;
    
    CurrentTokenType = FindPrnTokenType(CurrentPrintLine[0]);
    //	process tokens
    CString csWork = CurrentPrintLine.Mid(1);
    int nPrintLineLength = csWork.GetLength();
    switch (CurrentTokenType)
    {
    case PRN_BITMAPTOK:
        {
            nTokenEndPos = 0;
            LPTSTR szPrintLine = csWork.GetBuffer(nPrintLineLength);
            while ((nTokenEndPos < nPrintLineLength) && 
                (szPrintLine[nTokenEndPos] != BMPTOK_END))
            {
                ++nTokenEndPos;
            }
            if ((nTokenEndPos == nPrintLineLength) || (szPrintLine[nTokenEndPos] != BMPTOK_END))
            {
                nTokenEndPos = -1;
            }
            long nAlignment,nWidth;
            CString csBmpName,csPrintLineWithoutToken;
            if (nTokenEndPos != -1)
            {
                _TCHAR szField1[sizeof(long)+1] = {NULL};
                _TCHAR szField2[sizeof(long)+1] = {NULL};
                _TCHAR szField3[500] = {NULL};
                int nLongSize = sizeof(long);
                int nField3Size = (nTokenEndPos - 2*nLongSize);
                int nTokenLen = nField3Size + 2*nLongSize + sizeof(_TCHAR); // no starting token prefix
                int nPrnLnWithoutTokenLen = nPrintLineLength - nTokenLen;
                LPTSTR szField4 = (LPTSTR ) new _TCHAR[nPrnLnWithoutTokenLen];
                memcpy(szField1, szPrintLine, nLongSize * sizeof(_TCHAR));
                memcpy(szField2, szPrintLine+nLongSize, nLongSize * sizeof(_TCHAR));
                memcpy(szField3, szPrintLine+2*nLongSize, nField3Size * sizeof(_TCHAR));
                memcpy(szField4, szPrintLine+nTokenLen, nPrnLnWithoutTokenLen * sizeof(_TCHAR));
                CString csTmpRestOfTheLine(szField4, nPrnLnWithoutTokenLen);
                delete [] szField4;
                nWidth     = _ttol(szField1);
                nAlignment = _ttol(szField2);
                
                if ((nAlignment != DM_ALIGN_LEFT) && 
                    (nAlignment != DM_ALIGN_RIGHT) && 
                    (nAlignment != DM_ALIGN_CENTER))
                {
                    nAlignment = DM_ALIGN_CENTER;
                }
                csBmpName = szField3;
                csPrintLineWithoutToken = csTmpRestOfTheLine; // rest of the print line without token
            }
            CurrentPrintLine.ReleaseBuffer();
            // if (!csBmpName.IsEmpty()) // if bmp is null, DM uses PrinterSetBitmap value
            {
                // ****** DM control currently doesn't allow to set width of a bitmap ****** ///
                ASSERT(csBmpName.GetLength());
                rc = PrintBitmap(csBmpName, nWidth, nAlignment,nDeviceClass);
                
                if (rc == OPOS_E_NOEXIST)   //SR672
                    
                {
                    trace(L6, _T("Print Error: PrintBitmap: bitmap:%s doesnt exist"), csBmpName);
                }
            }
            CurrentPrintLine = csPrintLineWithoutToken;
            break;
        }
    case PRN_BARCODETOK:
        {
            long nType, nHeight, nAlignment;
            bool bPrintNumbers;
            CString csBarcodeData,csPrintLineWithoutToken;
            
            // find bar code token's end
            nTokenEndPos = 0;
            LPTSTR szPrintLine = csWork.GetBuffer(nPrintLineLength);
            while ((nTokenEndPos < nPrintLineLength) && 
                (szPrintLine[nTokenEndPos] != BARCODETOK_END))
            {
                ++nTokenEndPos;
            }
            if ((nTokenEndPos == nPrintLineLength) || (szPrintLine[nTokenEndPos] != BARCODETOK_END))
            {
                nTokenEndPos = -1;
            }
            
            // if token end is found, process bar code data
            if (nTokenEndPos != -1)
            {
                _TCHAR szField1[sizeof(long)+1] = {NULL};
                _TCHAR szField2[sizeof(long)+1] = {NULL};
                _TCHAR szField3[sizeof(long)+1] = {NULL};
                _TCHAR cField4 = DM_DA_NOT_CONFIGURED;
                int nLongSize = sizeof(long);
                int nField5Size = (nTokenEndPos - 3*nLongSize - 1);
                int nTokenLen = 3*nLongSize + 1 + nField5Size + 1; // no starting token prefix
                int nPrnLnWithoutTokenLen = nPrintLineLength - nTokenLen;
                LPTSTR szField5 = (LPTSTR ) new _TCHAR[nField5Size+1];
                LPTSTR szField6 = (LPTSTR ) new _TCHAR[nPrnLnWithoutTokenLen];
                memset(szField5,0,(nField5Size+1)*sizeof(_TCHAR));
                memset(szField6,0,nPrnLnWithoutTokenLen*sizeof(_TCHAR));
                
                // extract buffers for fields
                memcpy(szField1, szPrintLine, nLongSize*sizeof(_TCHAR));
                memcpy(szField2, szPrintLine+nLongSize, nLongSize*sizeof(_TCHAR));
                memcpy(szField3, szPrintLine+2*nLongSize, nLongSize*sizeof(_TCHAR));
                cField4 = *(szPrintLine+3*nLongSize);
                memcpy(szField5, szPrintLine+3*nLongSize+1, nField5Size*sizeof(_TCHAR));
                memcpy(szField6, szPrintLine+nTokenLen, nPrnLnWithoutTokenLen*sizeof(_TCHAR));
                CString csTmpBCData(szField5, nField5Size);
                CString csTmpRestOfTheLine(szField6, nPrnLnWithoutTokenLen);
                delete [] szField5;
                delete [] szField6;
                
                // save extracted field's value
                nType      = _ttol(szField1);
                nHeight    = _ttol(szField2);
                nAlignment = _ttol(szField3);
                if ((nAlignment != DM_ALIGN_LEFT) && 
                    (nAlignment != DM_ALIGN_RIGHT) && 
                    (nAlignment != DM_ALIGN_CENTER))
                {
                    nAlignment = DM_ALIGN_CENTER;
                }
                bPrintNumbers = (cField4 == DM_DA_LOADED) ? 1:0 ;
                csBarcodeData = csTmpBCData;
                csPrintLineWithoutToken = csTmpRestOfTheLine; // rest of the print line without current barcode token
                // test data
                int nTmpLen;
                nTmpLen = csBarcodeData.GetLength();
                nTmpLen = csPrintLineWithoutToken.GetLength();
            }
            CurrentPrintLine.ReleaseBuffer();
            if (!csBarcodeData.IsEmpty())
            {
                int nDataLen = csBarcodeData.GetLength();
                co.csOperationsReceiptBarcodeDataTextToBinaryDecoder.MakeUpper();
                if(co.csOperationsReceiptBarcodeDataTextToBinaryDecoder == _T("BASE64"))
                {
                    //need to convert to ANSI for Base64 Decoding
                    int myANSIDataSize = 0;
                    LPTSTR myp = csBarcodeData.GetBuffer(nDataLen);
#ifdef UNICODE
                    char *myANSIdata = ConvertLPWSTRToLPSTR(myp,&myANSIDataSize);
#else  //For CPPUnit test
                    char *myANSIdata = new char[nDataLen+1];
                    myANSIDataSize = nDataLen;
                    strcpy(myANSIdata, myp);
#endif
                    
                    //prepare a buffer for the decoded data and decode it
                    int test= sizeof(TCHAR);
                    int nDecodedSize=4*nDataLen*sizeof(TCHAR);
                    BYTE *buffer = (BYTE *)malloc(nDecodedSize);
                    memset(buffer,0,nDecodedSize);
                    Base64Decode(myANSIdata,myANSIDataSize,buffer,&nDecodedSize);

                    delete [] myANSIdata;

                    myp = csBarcodeData.GetBufferSetLength(nDecodedSize);

                    for(int i=0;i<nDecodedSize;i++)
                    {
                        myp[i] = (TCHAR) buffer[i];
                    }
                    csBarcodeData.ReleaseBuffer(nDecodedSize);

                    free(buffer);

                }
                rc = PrintBarcode(csBarcodeData,nType, nHeight, nAlignment,bPrintNumbers, nDeviceClass);
                
                if (rc == CTL_E_OUTOFMEMORY)
                {
                    trace(L6, _T("Print Error: PrintBarcode: out of memory error from DM"));
                }
            }
            CurrentPrintLine = csPrintLineWithoutToken;
            break;
        }
    case PRN_SETPROPTOK:
        {
            CString csProperty, csValue, csPrintLineWithoutToken;
			// find setproperty token's end
			nTokenEndPos = 0;
			LPTSTR szPrintLine = csWork.GetBuffer(nPrintLineLength);
			while ((nTokenEndPos < nPrintLineLength) && 
			       (szPrintLine[nTokenEndPos] != BARCODETOK_END))
			{
				++nTokenEndPos;
			}
			if ((nTokenEndPos == nPrintLineLength) || (szPrintLine[nTokenEndPos] != BARCODETOK_END))
			{
				nTokenEndPos = -1;
			}

			// if token end is found, process bar code data
			if (nTokenEndPos != -1)
			{
                int nPrnLnWithoutTokenLen = nPrintLineLength - nTokenEndPos;
				LPTSTR szField = (LPTSTR ) new _TCHAR[(nTokenEndPos + 1)*sizeof(_TCHAR)];
                LPTSTR szRestofString = (LPTSTR ) new _TCHAR[nPrnLnWithoutTokenLen*sizeof(_TCHAR)];;
				memset(szField,0,(nTokenEndPos)*sizeof(_TCHAR));

				// extract buffers for fields
				memcpy(szField, szPrintLine, nTokenEndPos*sizeof(_TCHAR));
                memcpy(szRestofString, szPrintLine+nTokenEndPos+1, nPrnLnWithoutTokenLen*sizeof(_TCHAR));

				CString csPropSet(szField, nTokenEndPos);
                CString csTmpRestOfTheLine(szRestofString, nPrnLnWithoutTokenLen);
				delete [] szField;
                delete [] szRestofString;

                ProcessSetProperty(csPropSet, nDeviceClass);



                csPrintLineWithoutToken = csTmpRestOfTheLine;

            }
            
            CurrentPrintLine.ReleaseBuffer();
            CurrentPrintLine = csPrintLineWithoutToken;
            break;

        }

    case PRN_NOTOKEN:
    default:
        {
            ASSERT(0);
            break;
        }
    }
    // test line to see the remaining length
    int ntmp = CurrentPrintLine.GetLength();
    return rc;
}

long CDMPrinterUtilsBase::PrintBitmap(CString  csBitmap, 
                             long     lWidth,
                             long     lAlign, 
                             int      nDeviceClass)
{
    trace(L6,_T("+PrintBitmap %s, lAlign=%d, nDeviceClass=%d, (lWidth=%d not used)"), 
        csBitmap, lAlign, nDeviceClass, lWidth);
    return dm.PrintBitmap(csBitmap, lAlign, nDeviceClass);

}

long CDMPrinterUtilsBase::PrintBarcode(
                                CString  csBarcode,
                                long     lBCType, 
                                long     lBCHeight, 
                                long     lBCAlign, 
                                bool     fPrintNumbers, 
                                int      nDeviceClass )
{
    trace(L6,_T("+PrintBarcode %s"), csBarcode);
   	//TAR 211913 BZ: for use of printing 128-C type barcodes
    CString csEncodedBarcode;
   	bool bBinaryConversion = false; 
    long lBinaryConversion = 0;	//defaut to OPOS_BC_NONE with no conversion
    long nWidth = 2;
    //TAR 211913 end 
    
    _TCHAR msg[] = T_ID _T("::PrintBarcode");
    rc = dm.Enable( nDeviceClass, DEVICEID0, true );
    if ( rc == 0)
    {
        try
        {
            CPrintStation* pPrinter = dm.GetPrintStation( nDeviceClass );
            if (lBCType < 100)  //Tar 237528, only do mapping if it is not defined in OPosPtr.h
                lBCType = MapDmBarcodeTypeToOpos( lBCType );
            lBCAlign = MapDmBarcodeAlignToOpos( lBCAlign );
            
            lBCHeight *= pPrinter->GetLineHeight();
            nWidth = 20 * pPrinter->GetLineWidth() / pPrinter->GetLineChars(); 
            
            //++SSCOADK-2504 Check if a Barcode Print Width is defined for this type
            for(int i=0;i<co.caBarcodePrintWidth.GetSize();i++)
            {
                SBmpPrintWidths &ptr = co.caBarcodePrintWidth.ElementAt(i);
                if(lBCType == ptr.nBarcodeType)
                {
                    if(ptr.nPrintWidth > -1)
                    {
                        nWidth=ptr.nPrintWidth;
                        trace(L6,_T("using scotopts defined width=%d for barcode type=%d"), nWidth,lBCType);
                        break;
                    }
                }
            }
            
            //TAR 211913 BZ: encode 128-C type barcodes as required by OPOS binary conversion
            //SSCOADK-968
            if( csBarcode.Find(_T("[OPOS_BC_DECIMAL]")) == 0)
            {
                lBinaryConversion = pPrinter->m_rPtr.GetBinaryConversion();	
                pPrinter->m_rPtr.SetBinaryConversion(2);	//Set BinaryConversion to OPOS_BC_DECIMAL
                bBinaryConversion = true;
                csBarcode.Replace(_T("[OPOS_BC_DECIMAL]"), _T(""));
            }
            else if (csBarcode.Find(_T("[OPOS_BC_NIBBLE]")) == 0)
            {
                lBinaryConversion = pPrinter->m_rPtr.GetBinaryConversion();	
                pPrinter->m_rPtr.SetBinaryConversion(1);	//Set BinaryConversion to OPOS_BC_NIBBLE
                bBinaryConversion = true;
                csBarcode.Replace(_T("[OPOS_BC_NIBBLE]"), _T(""));
            }
            else if (csBarcode.Find(_T("[BASE64]")) == 0)
            {
                lBinaryConversion = pPrinter->m_rPtr.GetBinaryConversion();
                pPrinter->m_rPtr.SetBinaryConversion(1);
                bBinaryConversion = true;
                csBarcode.Replace(_T("[BASE64]"), _T(""));
                DecodeBase64String(csBarcode);
                ConvertBinaryToOposAscii(csBarcode, OPOS_BC_NIBBLE);

            }
            else if ( ((lBCType == 110 ) && (csBarcode.Left(2) != _T("{A")))
                && ((lBCType == 110 ) && (csBarcode.Left(2) != _T("{B"))) )
            {
                if(co.fOperationsEncodeDataAsPackedBCD)
                {
                    csEncodedBarcode = GetEncoded128CBarcode(csBarcode);
                    csBarcode=csEncodedBarcode;
                }
                else
                {
                    if(csBarcode.Left(2) == _T("{C"))
                    {
                      csBarcode =  csBarcode.Mid(2);
                      trace(L7, _T("prefix {C removed"));
                    }
                    ConvertBinaryToOposAscii(csBarcode, OPOS_BC_NIBBLE);
                }
                if (!csBarcode.IsEmpty()){
                    lBinaryConversion = pPrinter->m_rPtr.GetBinaryConversion();	
                    pPrinter->m_rPtr.SetBinaryConversion(1);	//Set BinaryConversion to OPOS_BC_NIBBLE
                    bBinaryConversion = true;
                }
            } 
            trace(L7, _T("pPrinter->PrintBarcode: csBarcode=%s lBCType=%d lBCHeight=%d nWidth=%d lBCAlign=%d"), 
                csBarcode, lBCType, lBCHeight, nWidth, lBCAlign);
            //TAR 211913 end
            
            rc = pPrinter->PrintBarcode(
                csBarcode, lBCType, 
                lBCHeight, nWidth,  // barcode height, width (zero width causes DMx to scale like DM)
                lBCAlign, fPrintNumbers ? PTR_BC_TEXT_BELOW : PTR_BC_TEXT_NONE ); 
            //TAR 211913 BZ: restore the original value of BinaryConversion
            if (bBinaryConversion) {
                
                pPrinter->m_rPtr.SetBinaryConversion(lBinaryConversion);
            }
            //TAR 211913 end
            //CDeviceManagerEx::ThrowDmException( pPrinter );
            //m_devmgr.ThrowDmException( pPrinter ); 
        }
        PROCESS_EXCEPTIONS(nDeviceClass);
    }
    trace(L6,_T("-PrintBarcode returned 0x%x : csBarcode=%s lBCType=%d lBCHeight=%d nWidth=%d lBCAlign=%d"),
        rc, csBarcode, lBCType, lBCHeight, nWidth, lBCAlign); 
    return rc;
}

PRNOBJECTTOKENS CDMPrinterUtilsBase::FindPrnTokenType(_TCHAR chTokenPrefix)
{
	PRNOBJECTTOKENS nTokenType = PRN_NOTOKEN;
	_TCHAR cTmp = (_TCHAR) BARCODETOK_START;
	cTmp = (_TCHAR) BMPTOK_START;
	if (chTokenPrefix == (_TCHAR) BMPTOK_START)
	{
		nTokenType = PRN_BITMAPTOK;
	}
	else if (chTokenPrefix == (_TCHAR) BARCODETOK_START)
	{
		nTokenType = PRN_BARCODETOK;
	}
    else if (chTokenPrefix == (_TCHAR) SETPROPTOK_START)
    {
        nTokenType = PRN_SETPROPTOK;
    }
	ASSERT(nTokenType != PRN_NOTOKEN);
	return nTokenType;
}

void CDMPrinterUtilsBase::ProcessSetProperty(CString csPropSet, long nDeviceClass)
{
    trace(L6, _T("+DMProcedures::ProcessSetProperty - csPropSet = %s,  nDeviceClass = %d"), csPropSet, nDeviceClass);
    int posFirstComma = csPropSet.Find(_T(','));
    CString csPropertyNum = csPropSet.Mid(0,posFirstComma);
    CString csPropertyValue = csPropSet.Mid(posFirstComma+1);
    long lPropertyNum = _ttol(csPropertyNum);
    CPrintStation* pPrinter = dm.GetPrintStation( nDeviceClass );
    long lPropertyValue = 0;
    long ltest = PIDXPtr_RecLineChars;

    switch (lPropertyNum)
    {
    case PIDXPtr_RecLineChars:
        {
            int nPropertyValue = _ttoi(csPropertyValue);
            if (nPropertyValue > 0)
            {
                trace(L6, _T("Setting RecLineChars to %d"), nPropertyValue);
                CPrinterWrapper& cPrinterWrapper= pPrinter->GetPrinter();
                cPrinterWrapper.SaveSetLineCharRequest(DMCLASS_RECEIPT, nPropertyValue);
            }
            else
            {
                trace(L6, _T("Invalid RecLineChar value"));
            }
        }
        break;
    default:
        trace(L6, _T("Unsupported property change"));
        break;
    }
    trace(L6, _T("-DMProcedures::ProcessSetProperty"));
}
//TAR 211913 end

///////////////////////////////
long CDMPrinterUtilsBase::ReceiptCut()		  // cut the paper
{
    trace(L6,_T("+ReceiptCut"));
    _TCHAR msg[] = T_ID _T("::ReceiptCut");
    if ( dm.nDMLastReceiptFunction < 10 )
        dm.nDMLastReceiptFunction += 10;
    rc = dm.Enable(DMCLASS_RECEIPT, DEVICEID0, true);
    if ( rc == 0 )
    {
        try
        {
            // 1.1 Printer SO was always doing an advance, even though
            // we sent CUT_NOW.  For 2.0, CUT_NOW really does a CUT NOW,
            // so we need to send an explicit AUTOADVANCE.
            // 03/01/2000 - ncr_barn - TAR# 133943
            CPrintStation* pPrinter = dm.GetPrintStation( DMCLASS_RECEIPT );

            if ( co.fOperationsEnablePartialCut )
            {
                pPrinter->CutPaper( 80 );
            }
            else
            {
                pPrinter->CutPaper( 100 );
            }

            CDeviceManagerEx::ThrowDmException( pPrinter );

                     
            CPrinterWrapper& cPrinterWrapper= pPrinter->GetPrinter();
            if ( cPrinterWrapper.GetAsyncMode() == FALSE )
            {
                if ( pPrinter != NULL && pPrinter->GetState() != OPOS_S_BUSY )
                {
                    rc = pPrinter->PrintSynchronous();
                    
                    if ( rc == OPOS_SUCCESS)
                    {
                        mo.PostDM(DM_STATUS, DMCLASS_RECEIPT, pPrinter->m_nDeviceId, DM_PTR_REQUEST_COMPLETED, NULL);
                    }
                }
            }

        }
        PROCESS_EXCEPTIONS(DMCLASS_RECEIPT );	
    }
    trace(L6,_T("-ReceiptCut 0x%x"),rc);
    return rc;
}


//TAR 211913 BZ: this routine is used to encode 128-c type barcodes as required by OPOS binary conversion
//This method assumes the barcode is sent as an ASCII representation of BCD string,  as has been sent by
// the TB Emulator for many years.  The barcodes previously could be sent to the Printer as-is,  but newer
// Printer SO's do not accept the data in that format. (It was an SCER).  This routine therefore creates a
// OPOS_BC_NIBBLE string by converting each ASCII char back into NIBBLE format.
CString CDMPrinterUtilsBase::GetEncoded128CBarcode(CString csBarcodeData)
{
	trace(L7,_T("+GetEncoded128CBarcode %s"), csBarcodeData);

	long lBarcodeDataLength;	//The number of characters embedded in the barcode
	CString csEncodedData = _T("");

	csEncodedData = csBarcodeData;
	//remove the leading characters "{C" - this is just for barcode type
	csEncodedData =  csEncodedData.Mid(2);
	lBarcodeDataLength = csEncodedData.GetLength();

    if (lBarcodeDataLength%2 != 0)
	{
		trace(L6,_T("-GetEncoded128CBarcode: original barcode is not in pairs %s"), csBarcodeData);
		return csBarcodeData;
	}

	TCHAR *pData = new TCHAR[lBarcodeDataLength + 1];
    memset(pData,0,(lBarcodeDataLength + 1) * sizeof(_TCHAR));


	//parse the original barcode data in pairs, each pair takes two characters
	//and translate the pair to the printable format required by OPOS binary conversion
	for (int i=0; i<lBarcodeDataLength/2; i++)
	{
		TCHAR a[8] = {0};
        _stprintf(a, _T("%02x"), _ttoi(csEncodedData.Mid(i*2, 2)));
		pData[i*2]=a[0];
		pData[i*2+1]=a[1];
	}
	//replace unprintable characters according to ASCII table
	for (i=0; i<lBarcodeDataLength;i++)	
	{
		if (pData[i]>='A' && pData[i]<='F')
		{
			pData[i] -= 7;
		}
		if (pData[i]>='a' && pData[i]<='f')
		{
			pData[i] -= 39;
		}
	}
    pData[lBarcodeDataLength] = '\0';
    
	csEncodedData = pData;	//done with encoding 
	delete [] pData;

    if (csEncodedData.GetLength() != (csBarcodeData.GetLength() - 2))
	{
		trace(L6,_T("-GetEncoded128CBarcode: barcode was mistakenly truncated, new barcode is %s"), csEncodedData);
	}
	else
		trace(L7,_T("-GetEncoded128CBarcode: barcode %s was successfully encoded."), csEncodedData);

	return csEncodedData;
}

long CDMPrinterUtilsBase::MapDmBarcodeTypeToOpos(long BarcodeType ) 
{
    switch(BarcodeType)
    {
    case DM_BCS_UPCA:
        BarcodeType = PTR_BCS_UPCA;
        break;
    case DM_BCS_UPCE:
        BarcodeType = PTR_BCS_UPCE;
        break;
    case DM_BCS_JAN8:
        BarcodeType = PTR_BCS_JAN8;
        break;
    case DM_BCS_JAN13:
        BarcodeType = PTR_BCS_JAN13;
        break;
    case DM_BCS_ITF:
        BarcodeType = PTR_BCS_ITF;
        break;
    case DM_BCS_CODABAR:
        BarcodeType = PTR_BCS_Codabar;
        break;
    case DM_BCS_CODE39:
        BarcodeType = PTR_BCS_Code39;
        break;
 	case DM_BCS_CODE128:	//BZ 06/12/02 TAR 204658: Recognize Code 128 barcodes
        BarcodeType = PTR_BCS_Code128;
        break;
    default:
        ASSERT( FALSE );
          // Throw an error to the application indicating invalid parameter.
        CDeviceManagerEx::ThrowDmException( DM_E_INVALIDPARAMETER );
        break;
    }
    return BarcodeType;
}

long CDMPrinterUtilsBase::MapDmBarcodeAlignToOpos(long nAlign )
{
    switch(nAlign)
      {
      case DM_ALIGN_LEFT:
         nAlign = PTR_BC_LEFT;
         break;
      case DM_ALIGN_RIGHT:
         nAlign = PTR_BC_RIGHT;
         break;
      case DM_ALIGN_CENTER:
         nAlign = PTR_BC_CENTER;
         break;
      default:
         ASSERT( FALSE );
         // Throw an error to the application indicating invalid parameter.
         CDeviceManagerEx::ThrowDmException( DM_E_INVALIDPARAMETER );
         break;
      }
    return nAlign;
}


char* CDMPrinterUtilsBase::ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn, int* sizeout)
{
  LPSTR pszOut = NULL;
  *sizeout=0;
  if (lpwszStrIn != NULL)
  {
	int nInputStrLen = wcslen (lpwszStrIn);

	// Double NULL Termination
	int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
	pszOut = new char [nOutputStrLen];

	if (pszOut)
	{
	  memset (pszOut, 0x00, nOutputStrLen);
	  WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
	}
    *sizeout=nOutputStrLen-2;
  }
  return pszOut;
}

    
void CDMPrinterUtilsBase::DecodeBase64String(CString &csEncodedData)
{
    int nDataLen = csEncodedData.GetLength();
    //need to convert to ANSI for Base64 Decoding
    int myANSIDataSize = 0;
    LPTSTR myp = csEncodedData.GetBuffer(nDataLen);
#ifdef UNICODE
    char *myANSIdata = ConvertLPWSTRToLPSTR(myp,&myANSIDataSize);
#else  //For CPPUnit test
    char *myANSIdata = new char[nDataLen+1];
    myANSIDataSize = nDataLen;
    strcpy(myANSIdata, myp);
#endif
    
    //prepare a buffer for the decoded data and decode it
    int test= sizeof(TCHAR);
    int nDecodedSize=4*nDataLen*sizeof(TCHAR);
    BYTE *buffer = (BYTE *)malloc(nDecodedSize);
    memset(buffer,0,nDecodedSize);
    Base64Decode(myANSIdata,myANSIDataSize,buffer,&nDecodedSize);
    delete [] myANSIdata;
    
    //put results in a CString (use this constructor since the data could have NULLs in it if 128C data)
    CString myData((TCHAR *)buffer,nDecodedSize/sizeof(TCHAR));
    csEncodedData=myData;
    free(buffer);
}

void CDMPrinterUtilsBase::ResetReceiptPrinter()
{
    CPrintStation* pPrinter = dm.GetPrintStation( DMCLASS_RECEIPT );
    if ( pPrinter != NULL)
    {
        CPrinterWrapper& cPrinterWrapper= pPrinter->GetPrinter();
        cPrinterWrapper.SetRecLineChars(cPrinterWrapper.GetDefaultLineChars());
    }
}