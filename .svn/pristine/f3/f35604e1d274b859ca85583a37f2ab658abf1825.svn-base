//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMSmSalesAndAssistanceReportsBase.CPP
//
// TITLE: Class implementation for Sales and Assistance Reports state
//
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header
#include "Common.h"                 // Common includes
#include "DMProcedures.h"
#include "SMSmSalesAndAssistanceReports.h"
#include "SMSmReportMenu.h"

#define COMP_ID ID_SM               // Base state component
#define T_ID _T("SmSalesAndAssistanceReports")

DEFINE_TIMESTAMP
IMPLEMENT_DYNCREATE(SMSmSalesAndAssistanceReportsBase, CObject) // MFC Runtime class/object information
//IMPLEMENT_STATE(SmSalesAndAssistanceReports)

//===================================================================
// constructor
//===================================================================
SMSmSalesAndAssistanceReportsBase::SMSmSalesAndAssistanceReportsBase(REPORTKIND rKind, 
																	 REPORTDAY  rDay)
{
  IMPLEMENT_TIMESTAMP
  pReportGen = NULL;
  SetReportDay(rDay);
  SetReportType(RP_SUMMARY); 
  SetReportKind(rKind);
}

//===================================================================
// Deliver
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::Deliver(REPORTKIND rKind,
														 REPORTDAY  rDay)
{
  pReportGen = NULL;
  SetReportDay(rDay);
  SetReportType(RP_SUMMARY); 
  SetReportKind(rKind);
  return this;
}

//===================================================================
// Initialize
///===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::Initialize(void)
{
   // Set frame
   ps.SetFrame(STORE_MODE_FRAME);

   // Display report title
   if (ReportKind == RP_SALES)
     ps.SetTitle(MSG_SALESREPORTS, PS_BLANK);
   else if (ReportKind == RP_ASSISTANCE)
     ps.SetTitle(MSG_ASSISTANCEREPORTS, PS_BLANK);
  // Lead thru is blank initially.
   ps.SetLeadthruText(PS_BLANK, PS_BLANK);

   // Initially some buttons are disabled for Today and Yesterday.
   // No Hourly or Summary button for previous 7 days report.
   if (ReportDay == RP_PREVIOUS7DAYS)
   {
     ps.SetButtonText(1, PS_NOTEXT);
     ps.SetButtonText(2, PS_NOTEXT);
   }
   else
   { 
	 ps.SMButton(IDC_BUTTON1, BTT_HOURLY,  false);
     ps.SMButton(IDC_BUTTON2, MSG_SUMMARY, false);
   }

   ps.SetButtonText(3, PS_NOTEXT);
   ps.SetButtonText(4, PS_NOTEXT);
   ps.SetButtonText(5, PS_NOTEXT);
   ps.SetButtonText(6, PS_NOTEXT);
   ps.SMButton(IDC_BUTTON7, BTT_PRINTREPORT, false);
   ps.SetButtonText(8, BTT_GOBACK);

   ps.SetBackground(BMP_SMLIST);
   ps.ShowFrame();
   ps.ShowSMTotal(false);

   // Show report, if there is data, set 'Hourly' and 'Print' button On.
   long rc = GenerateReport(ReportKind, ReportDay, ReportType);
   if (rc)
   {
     if (ReportDay != RP_PREVIOUS7DAYS)
       ps.SMButton(IDC_BUTTON1, BTT_HOURLY, true);
     ps.SMButton(IDC_BUTTON7, BTT_PRINTREPORT, true);
   }
   return STATE_NULL;
}


//===================================================================
// Button 1 - Hourly
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::PSButton1(void)
{
   ps.SMButton(IDC_BUTTON1, BTT_HOURLY,  false);
   ps.SMButton(IDC_BUTTON2, MSG_SUMMARY, true);
   GenerateReport(ReportKind, ReportDay, RP_HOURLY);
   return STATE_NULL;
}


//===================================================================
// Button 2 - Summary
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::PSButton2(void)
{
   ps.SMButton(IDC_BUTTON1, BTT_HOURLY,  true);
   ps.SMButton(IDC_BUTTON2, MSG_SUMMARY, false);   
   GenerateReport(ReportKind, ReportDay, RP_SUMMARY);
   return STATE_NULL;
}

//===================================================================
// Button 7 - Print
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::PSButton7(void)
{
   ps.SMButton(IDC_BUTTON7, BTT_PRINTREPORT, false);
   PrintReport();
   return STATE_NULL;
}

//===================================================================
// Button 8 - Go back to Report Menu.
//===================================================================
SMStateBase  *SMSmSalesAndAssistanceReportsBase::PSButton8(void)
{
  delete pReportGen;
  pReportGen = NULL;
  CREATE_AND_DISPENSE(SmReportMenu)(ReportKind);
}

//===================================================================
// UnInitialize  - Fix for TAR 201738
//===================================================================
void SMSmSalesAndAssistanceReportsBase::UnInitialize(void)
{
    if (pReportGen)
    {
	   delete pReportGen;
       pReportGen = NULL;
    }
	SMStoreModeStateBase::UnInitialize();
}

//===================================================================
// GenerateReport
//===================================================================
long SMSmSalesAndAssistanceReportsBase::GenerateReport(REPORTKIND kind,
				                          REPORTDAY day, 
					                      REPORTTYPES type)
{
   long rc = 0;
   ps.Echo(MSG_RP_PROCESSING);
   if (!pReportGen)
      pReportGen = new ReportGeneratorBase();

   SetLeadThru(day, type);
   rc = pReportGen->GenerateReport(kind, day, type);

   // Save report day and type requested.
   if (rc)
   {
     SetReportDay(day);
     SetReportType(type);
     ps.SMButton(IDC_BUTTON7, BTT_PRINTREPORT, true);
   }
   ps.Echo(PS_BLANK);
   return rc;
}

//===================================================================
// Set lead thru text..
//===================================================================
void SMSmSalesAndAssistanceReportsBase::SetLeadThru(REPORTDAY day,
 				       REPORTTYPES type)
{
   PSTEXT text;
   if (day == RP_TODAY)
   {
     if (type == RP_SUMMARY)
       text = MSG_RP_TODAYSUMMARY;
     else
       text = MSG_RP_TODAYHOURLY;
   }
   else if (day == RP_YESTERDAY)
   {
     if (type == RP_SUMMARY)
       text = MSG_RP_YESTERDAYSUMMARY;
     else
       text = MSG_RP_YESTERDAYHOURLY;
   }
   else
     text = MSG_RP_PREVIOUS7DAYSSUMMARY;
   ps.SetLeadthruText(text, PS_BLANK);
}

//===================================================================
// Set type of report.
//===================================================================
void SMSmSalesAndAssistanceReportsBase::SetReportType(REPORTTYPES type)
{
   ReportType = type;
}  

//===================================================================
// Set kind of report.
//===================================================================
void SMSmSalesAndAssistanceReportsBase::SetReportKind(REPORTKIND kind)
{
   ReportKind = kind;
}  

//===================================================================
// Set day to generate report.
//===================================================================
void SMSmSalesAndAssistanceReportsBase::SetReportDay(REPORTDAY day)
{
   ReportDay = day;
}  

//===================================================================
// PrintReport
//===================================================================
long SMSmSalesAndAssistanceReportsBase::PrintReport()
{
   long rc = 0;
//Fiscal Printer start
	if(co.fOperationsTBControlsPrinter)
	{
		CString csFileToPrint=pReportGen->GetNameOfFileToPrint();
		int nFileToPrintLength = csFileToPrint.GetLength();
		LPTSTR szFileToPrint=csFileToPrint.GetBuffer(nFileToPrintLength);
		trace(L6, _T("+tb.Print, file name=%s"), szFileToPrint);
		rc = tb.Print(szFileToPrint);
		csFileToPrint.ReleaseBuffer(); //408230
		trace(L6, _T("-tb.Print"));
		return rc;
	}
//Fiscal Printer end
   if (!pReportGen)
      pReportGen = new ReportGeneratorBase();

   DWORD	len;
   char	 *pBuff=NULL;
    
   HFILE hFile = pReportGen->GetReportPrint();
   if (hFile!=NULL)
   {
	   try {

		  CFile	cFile(hFile);

		  len = cFile.SeekToEnd();
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
				}
			 }
		  }
		  pReportGen->ReleaseReportPrint();
	   }//try
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
			   
			   CString csPrintData(pBuff + iPos, iDataLength);
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
