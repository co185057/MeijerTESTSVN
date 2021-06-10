//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:    SMSmRunReportsBase.CPP
//
// TITLE:   Class implementation for Print Reports state
//
// AUTHOR:  Tara Duncan
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // Common includes
#include "SMSmRunReports.h"
#include "SMSmReportsMenu.h"
#else
#include "SMSmRunReportsBase.h"
#include "FakeSMStoreModeMenuBase.h"
#include "TestMacros.h"
#endif


#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmRunReportsBase")

IMPLEMENT_DYNCREATE(SMSmRunReportsBase, CObject)// MFC Runtime class/object information
DEFINE_TIMESTAMP
///////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////
SMSmRunReportsBase::SMSmRunReportsBase()
{
}
#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////
SMSmRunReportsBase::~SMSmRunReportsBase()
{
    if (m_csCurrentReport != EMPTY_STRING)
    {
		STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
        m_csCurrentReport = EMPTY_STRING;
    }
    m_csRPT = EMPTY_STRING;
}            
///////////////////////////////////////////////////////////////////////////////
// Function:    Initialize                                   
// Class:       SMSmRunReportsBase                                              
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:     STATE_NULL                                         
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmRunReportsBase::Initialize(void)
{
  TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_BEGIN, _T("AcknowledgeSig") );	// RFC 330050
  SetReportFileToPrint(m_csCurrentReport);
  printerUtils.ResetReceiptPrinter();
  ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);
  if(gpSCOTAuxDisp)
	gpSCOTAuxDisp->SetLanguage(SCOT_LANGUAGE_PRIMARY);
  ps.SetFrame(_T("SmRunReports")); //*** NewUI ***// 
  ps.SetTitle(MSG_REPORTCNT, PS_BLANK);

  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_RED,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
  // (*) SR898 ====================
  DMSetTriColorLight(DM_TRICOLORLIGHT_YELLOW,
                     DM_TRICOLORLIGHT_OFF,
                     DM_TRICOLORLIGHT_NORMALREQUEST);
                     
  if (m_csCurrentReport != EMPTY_STRING)
  {
    ps.SMButton(_T("SMButton1"), BTT_PRINTREPORT, true); //enable print report button
  }
  else
  {
    ps.SMButton(_T("SMButton1"), BTT_PRINTREPORT, false); //disable print report button
  }
  // TAR 236945 - Needed for Remote, this api will check option whether to display or not.
  ps.Weight(lDMScaleWeight);


  CString csDesc;
  csDesc.Format(ps.GetPSText(LTT_REPORTNAME, SCOT_LANGUAGE_PRIMARY), m_csRPT);           
  ps.SetLeadthruText(csDesc, _T(""));
  ps.ShowFrame();
  if (dm.GetPrinterPaperLow())
  {
	// (*) SR898 ====================
    DMSetTriColorLight(DM_TRICOLORLIGHT_GREEN,
                       DM_TRICOLORLIGHT_BLINK_1HZ,
                       DM_TRICOLORLIGHT_NORMALREQUEST);  
  }
  ps.Echo(PS_BLANK);
  ps.ShowTrainingModeOnScreen(TrainingModeObj.IsTMOn());  //Tar 228689, While lane in training, RAP store mode did not show in training

  ps.SMButton(_T("SMButton8"), BTT_GOBACK, true); //enable go back button always
//  COleVariant vReturn, vIndex = (long)UI::ScrollTop;
 // m_pAppPSX->SendCommand( _T("SMReportReceipt"), _T("Scroll"), vReturn, 1, vIndex );
  //TAR 234371 
  m_pAppPSX->SetControlProperty(_T("SMReportReceipt"), UI::PROPERTYREDRAW, VARIANT_TRUE);  //TAR#295071
  trace(L6, _T("SMSmRunReportsBase::Initialize- REDRAW is set to TRUE") );   

  return STATE_NULL;  // stay in SmSmReportMenuBase state
}
///////////////////////////////////////////////////////////////////////////////
// Function:    ReleaseReportPrint                                   
// Class:       SMSmRunReportsBase                                              
// Description: Close Current Print File
// Parameters:          
//              
//              
//              
//              
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmRunReportsBase::ReleaseReportPrint()
{
  ClosePrintFile();   
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetReportPrint                                   
// Class:       SMSmRunReportsBase                                              
// Description: Return Current Report File to Print
// Parameters:          
//              
//              
//              
//              
// Returns:     HFILE                                         
///////////////////////////////////////////////////////////////////////////////////
HFILE SMSmRunReportsBase::GetReportPrint()
{
   CString csSourceFileName;
   int nOpenMode = CFile::modeRead| CFile::shareDenyNone| CFile::typeText;
   CFileException fe;
   HFILE hFile;

   //+TAR247967 - changing all upper case MFC Macro TRY/CATCH to C++ try/catch 
   try
   {
	   m_csfTmp.Open(m_csCurrentReport,nOpenMode);
	   hFile  = (HFILE)m_csfTmp.m_hFile;
   }
   catch( CFileException* e )
   {
	   csSourceFileName.Format(_T("File Error:%s could not be opened. Cause:%s \n"), m_csCurrentReport, e->m_cause);
	   TRACE(_T("%s"), csSourceFileName);      // debug output window
	   trace(L0, _T("%s"), csSourceFileName); // log file
#ifdef _DEBUG_
			    afxDump << _T("File Error:") << test << _T("could not be opened,cause: ") << e->m_cause << _T("\n");
#endif
				e->Delete();
   }
	//TAR247967

   return hFile;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    OpenPrintFile                                   
// Class:       SMSmRunReportsBase                                              
// Description: Opens Print File  
// Parameters:          
//              
//              
//              
//              
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long SMSmRunReportsBase::OpenPrintFile(void)
{
    long lRetCode = 0;
    CFileException fe;
    if (filePrint.m_hFile == CFile::hFileNull)
    {
       // Open does not throw exceptions
       filePrint.Open(ReportFileToPrint(), CFile::modeCreate | CFile::modeNoTruncate |
                      CFile::shareDenyNone | CFile::modeReadWrite, &fe);
       if (fe.m_cause  != CFileException::none)
       {
         // eo.FileError(RETURN,"Opening tracking file", fe);
       }
   }
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PrintReport                                   
// Class:       SMSmRunReportsBase                                              
// Description: Prints current report file  
// Parameters:          
//              
//              
//              
//              
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long SMSmRunReportsBase::PrintReport()
{
    long rc =0;
    if(co.fOperationsTBControlsPrinter)
	{
		CString csFileToPrint=GetNameOfFileToPrint();
		int nFileToPrintLength = csFileToPrint.GetLength();
		LPTSTR szFileToPrint=csFileToPrint.GetBuffer(nFileToPrintLength);
		trace(L6, _T("+tb.Print, file name=%s"), szFileToPrint);
		rc = tb.Print(szFileToPrint);
		csFileToPrint.ReleaseBuffer();  //408230
		trace(L6, _T("-tb.Print"));
		return rc;
	}
   HFILE hFile = GetReportPrint();
   if (hFile!=NULL)
   {
     DWORD	len;
	 TCHAR	*pBuff;
	   try
       {
#if _MFC_VER < 0x0700
            CFile cFileRP( hFile );
#else
            CFile cFileRP( (HANDLE)hFile );
#endif
		    
            len = (DWORD)cFileRP.SeekToEnd();
		    cFileRP.SeekToBegin();
		    if (len)
            {
			 pBuff = new TCHAR [len+1];
			 if (pBuff)
			 {
				memset(pBuff, 0, len+1);
				UINT c = cFileRP.Read(pBuff, len);
				if (c)
				{
				   pBuff[len]  = 0;
				}
			 }
		  }
          ReleaseReportPrint();
	   }
	   catch(CFileException *e)
	   {
		   // may be an invalid file handle
		   _TCHAR szError[255] = {NULL};
		   e->GetErrorMessage( szError, 255 );
		   ScotError(RETURN,SM_CAT_INTERNAL_ERROR, SCOTAPP_BS_RECEIPT,_T("%d|%s"),hFile, szError);
		   e->Delete();
		   if(pBuff)
			   delete [] pBuff;
		   return rc;
	   }
	   //parse contents into seperate jobs
       int iPos=0;
	   int iFnd;
	   
	   _TCHAR sCut[20];
	   wsprintf(sCut,_T("\x1B|50fP"));
	   
	   if(len)
	   {
		   do
		   {
			   iFnd = -1;
			   for(DWORD i=iPos;i<len;i++)
			   {
				   if(!memcmp(pBuff + i,sCut,_tcsclen(sCut)))
				   {
					   iFnd=i;
					   break;
				   }
			   }
			   int iDataLength;
			   if(iFnd==-1)
				   iDataLength = len - iPos;
			   else
				   iDataLength = iFnd - iPos;
			   
			   CString csPrintData(pBuff + iPos, (iDataLength/sizeof(TCHAR)));
			   iPos = iFnd + _tcsclen(sCut) + 1;
			   
			   DMReceiptLine(csPrintData);
			   DMReceiptCut();

		   } while(iFnd != -1);
	   }
	   if(pBuff)
		   delete [] pBuff;
   }
   return rc;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    ClosePrintFile                                   
// Class:       SMSmRunReportsBase                                              
// Description: Closes Print File  
// Parameters:          
//              
//              
//              
//              
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void SMSmRunReportsBase::ClosePrintFile(void)
{
    m_csfTmp.Close();
	STATE(StoreModeState)::SCOTDeleteFile(m_csCurrentReport);   //TAR 348946 - Delete Existing Report File
    m_csCurrentReport = EMPTY_STRING;

    ps.SMButton(_T("SMButton1"), BTT_PRINTREPORT, false); //disable print report button
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetNameOfFileToPrint                                  
// Class:       SMSmRunReportsBase                                              
// Description: Calls ReportFileTo Print 
// Parameters:          
//              
//              
//              
//              
// Returns:     CString                                         
///////////////////////////////////////////////////////////////////////////////////
CString SMSmRunReportsBase::GetNameOfFileToPrint()
{
  return ReportFileToPrint();
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PSButton1                                   
// Class:       SMSmRunReportsBase                                              
// Description: Calls PrintReport  
// Parameters:          
//              
//              
//              
//              
// Returns:                                              
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmRunReportsBase::PSButton1(void) 
{
  ps.SMButton(_T("SMButton1"), BTT_PRINTREPORT, false); //disable print report button
  //TAR378451+
//SSCOP-137 (migrate fix of TAR459459)  if(m_csPrinterCharSet != _T("437"))//TAR 410168
//SSCOP-137 (migrate fix of TAR459459)    SendNewEscSeq2Printer(); 
  PrintReport();
//SSCOP-137 (migrate fix of TAR459459)  if(m_csPrinterCharSet != _T("437"))//TAR 410168
//SSCOP-137 (migrate fix of TAR459459)    SendOrigEscSeq2Printer();
  //TAR378451-
  return STATE_NULL;
}
// ///////////////////////////////////////////////////////////////////////////////
// Function:    Button 8 - Go-back                                  
// Class:       SMSmRunReportsBase                                              
// Description: 
// Parameters:          
//              
//              
//              
//              
// Returns:                                              
///////////////////////////////////////////////////////////////////////////////////
SMStateBase  *SMSmRunReportsBase::PSButton8(void)
{    

   RETURNSTATE(SmReportsMenu);
}
////////////////////////////////////////
// NewUI
SMStateBase  *SMSmRunReportsBase::PSReceiptUp(void)
{ 
  //The scrolling of the receipt is done by action (defined in xml)
	return STATE_NULL;
}
#endif
SMStateBase  *SMSmRunReportsBase::PSReceiptDown(void)
{
  // NextGen //
  if (0 != nPSLastParm)
  {
	  CString csItemId;
	  csItemId.Format(_T("%d"), nPSLastParm);
	  ps.GetPSXObject()->SetSelectedReceiptItem(_T("SMReportReceipt"), csItemId);
  }

  //The scrolling of the receipt is done by action (defined in xml)
	return STATE_NULL;
}
// End NewUI
#ifndef _CPPUNIT
//Tar 397105
void SMSmRunReportsBase::DMReceiptLine(const CString& csPrintLine)
{
#ifdef _UNICODE
	char* pBuf;
	int nLen;
	BOOL bDef;

	unsigned int nCodePage = _ttoi(m_csPrinterCharSet);
	trace(L6, _T("Default Character Set is %s"), m_csPrinterCharSet);

	if ( nCodePage != 0 )
	{
//SSCOP-137 (migrate fix of TAR459459)		if ( m_csPrinterCharSet == _T("852") )
//SSCOP-137 (migrate fix of TAR459459)		{
			trace( L6, _T("Attempting conversion to code page %d"), nCodePage );
			nLen = WideCharToMultiByte( nCodePage, 0, csPrintLine, csPrintLine.GetLength(), NULL, 0,
			   NULL, &bDef );
			trace( L6, _T("Conversion needs %d bytes: %d"), nLen, ::GetLastError() );

			if ( nLen )
			{
				int nRealLen;
				pBuf = new char[nLen+1];
				memset((void*)pBuf, 0, nLen+1 );
				nRealLen = WideCharToMultiByte( nCodePage, 0, csPrintLine, csPrintLine.GetLength(), pBuf, nLen+1,
				   NULL, &bDef );

				trace( L6, _T("Conversion returns %d bytes: %d"), nLen, ::GetLastError() );
				if ( nRealLen )
				{
					CString csANSI( pBuf, nRealLen );
					dm.ReceiptLine(csANSI);
					delete[] pBuf;
					return;
				}
				else
				{
					delete[] pBuf;
					return;
				}
			}
//SSCOP-137 (migrate fix of TAR459459)		}
//SSCOP-137 (migrate fix of TAR459459)		else
//SSCOP-137 (migrate fix of TAR459459)		{
//SSCOP-137 (migrate fix of TAR459459)			dm.ReceiptLine(csPrintLine);
//SSCOP-137 (migrate fix of TAR459459)			return;
//SSCOP-137 (migrate fix of TAR459459)		}

	}
#endif

	dm.ReceiptLine(csPrintLine);
}

// + RFC 330050
void SMSmRunReportsBase::UnInitialize(void)
{
	TellApplicationStateToSecurity(KEY_AC_SET_APP_STATE_END, _T("SmRunReports") );	
}
// - RFC 330050

#endif


// End of SMSMRunReportBase
// ///////////////////////////////////////////////////////////////////////////////
