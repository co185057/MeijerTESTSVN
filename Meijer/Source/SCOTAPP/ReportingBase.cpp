///////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:   ReportingBase.CPP
//
// TITLE:  Log data for FastLane 3.1 Reports.This module will attain all data associated 
//         with the Items, Transaction, Intervention, Tender, CurrencyAccepted, CurrencyDispensed 
//         and Signature entities. 
//
// Author: Tara Duncan
///////////////////////////////////////////////////////////////////////////////////////////
#include <stdafx.h>                 // MS MFC always needed first
//               // MS MFC template header

#ifndef _CPPUNIT
#include "Common.h"                 // MGV includes common to most
#include "CTransbroker.h"   
#include "SMState.h"
#include "Reporting.h"   
#include "CollectorBase.h"          // Track events 
#else
#include "FakeCommon.h"
#include "FakeSMStateBase.h"
#include "ReportingBase.h"
#define RPT_INVALIDPARM         11
#endif //_CPPUNIT


            
#define COMP_ID ID_RP               // Base state component
#define T_ID _T("CReportingBase")

IMPLEMENT_DYNCREATE(CReportingBase, CObject)// MFC Runtime class/object information
static CString csDummyClearingOperator = _T("0000"); //TAR 371932 & tar 371918

///////////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////////
CReportingBase::CReportingBase()
{ 
#ifndef _CPPUNIT
  InitData();
  m_bCashManagementInProgress = false;
#endif
}
///////////////////////////////////////////////////////////////////////////////
// Function:    InitData                                        
// Class:       CReportingBase                                              
// Description: is a public method that Initializes all constant variables needed
// Parameters:  NONE                                               
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void CReportingBase::InitData()
{
//  trace(L7, _T("+CReportingBase::InitData"));
  m_lItemCount          = 0;                    //default value for Item count
  m_lTransactionTotal   = 0;                    //default value for Transaction total
  m_csTransactionNumber = _T("");				//TAR410994
  m_bTransactionNumberSet = false;				//TAR410994
  m_bForceEnd           = false;                //default value for Forced End
  m_bItemAssoc          = false;                //default value for Item Association with Intervention
  m_bRapAssisted        = false;                //default value for RapAssited for Transacation End Entry
  m_bTransAssisted      = false;                //default value for TransAssited for Transaction End Entry
  m_bIsInterventionApproved = false;            //SR803 - default value for IsInterventionApproved for Intervention End Entry  
  m_bTelnetActive       = false;
  m_nDisposition        = RP_DISPOSITION_COMPLETE;//default value for TransDisposition for Transaction End Entry
  m_bOffline            = false;                //default value for Offline for Transaction End Entry  
  m_bInterventionEndInProgress = false;  // TAR 311902
  m_csTypeStart.Format(_T("%s=\"%s\" "), ATTTYPE, ATTSTART);
  m_csTypeEnd.Format(_T("%s=\"%s\" "), ATTTYPE, ATTEND);
  m_lOperatorLoginID = 1; // A unique per-transaction ID for operator login... reset it to 1
  m_bTransactionInProgress = false; //TAR 343053
  m_lEntryID = 1;
  m_bMobileShopper = false;
  // RFQ 7477
  //SOTF 5572
  ResetCurrencyList();
  // RFQ 7477
//  trace(L7, _T("-CReportingBase::InitData"));
}
///////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////
CReportingBase::~CReportingBase()
{
// RFQ 7477
	//SOTF 5572
	ResetCurrencyList();
}

void CReportingBase::ResetCurrencyList(void)
{
	CReportingBase::m_SCurrencyList.clear();
// RFQ 7477
}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetTimeAsString                                        
// Class:       CReportingBase                                              
// Description: This method will be called by all other log methods to capture the current
//              Time.
// Parameters:  NONE
// Returns:     CString, the current formatted time                                         
///////////////////////////////////////////////////////////////////////////////////
CString CReportingBase::GetTimeAsString()
{
    CString  csRetCode;
    CString csTime;  
    SYSTEMTIME sTime;   
    GetSystemTime( &sTime); 
    csTime.Format(_T("\"%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%d\" "),
                sTime.wYear,
                sTime.wMonth,
                sTime.wDay,
                sTime.wHour,
                sTime.wMinute,
                sTime.wSecond,
                sTime.wMilliseconds);

    csRetCode = csTime;
    return  csRetCode;
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    TransactionStart                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures period business date for the point
//              of sale application and the store transaction identifier. The period
//              business date will correspond with the Reporting Section of the Scotopts.dat 
//              configuration file   
// Parameters:  csBusinessDate  -Business date passed by TB
// Returns:     long 
// Example:     <Transaction Time="2012-10-04T18:45:33.179" Station="1" BusinessDate="2012-10-04" 
//              StoreTransactionID="1" Type="Start" ID="1" />                                         
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::TransactionStart(CString csBusinessDate)
{
    trace(L7, _T("+CReportingBase::TransactionStart"));
    CString csBDate, csID,csStation,csTime;
    long lRetCode = 0;
    if(!m_bTransactionInProgress)
    {  
        m_bTransactionInProgress = true;
        m_csNameStart = TRANSACTION;
        m_csStart.Format(_T("<%s"), m_csNameStart);
        csBDate.Format(_T("%s=\"%s\" "),ATTBUSINESS,csBusinessDate);
        csStation.Format(_T("%s=\"%s\" "),ATTSTATION,m_csTerminalNumber);
        ID(RP_IDTYPE_TRANS,RP_IDOPERATION_INCREMENT);
        csID.Format(_T("%s=\"%d\"/>"),ATTIDID,ID(RP_IDTYPE_TRANS,RP_IDOPERATION_GET));
		if (m_csTerminalNumber == _T(""))
            trace(L5, _T("+m_csTerminalNumber is EMPTY"));
       csTime = GetTimeAsString();
       if (csTime.GetLength() > 3)
       {
            m_csTransactionID = csTime.Mid(1,csTime.GetLength()- 3);
       }
       else
       {
           m_csTransactionID = _T("");
       }
       if ((csBusinessDate == EMPTY_STRING))
       {
           	CTime ctCurrentTime;
            ctCurrentTime = CTime::GetCurrentTime();
	        CString csTimeBD;
	        csTimeBD.Format(_T("%.4d-%.2d-%.2d"),
		    ctCurrentTime.GetYear(),
		    ctCurrentTime.GetMonth(),
		    ctCurrentTime.GetDay());
            csBusinessDate = csTimeBD;
            csBDate.Format(_T("%s=\"%s\" "),ATTBUSINESS,csBusinessDate);

       }
	   //Attaching difference between local time and universal time in minutes into Transaction start record
			// START SRXXX CSV Reporting
		TIME_ZONE_INFORMATION stTimeZone;
		// Check for new business date (increment only)
		DWORD dwTZVal = GetTimeZoneInformation(&stTimeZone);
        if (dwTZVal == TIME_ZONE_ID_DAYLIGHT)
        {
			m_lBias = stTimeZone.Bias + stTimeZone.DaylightBias;
        }
        else
        {
            m_lBias = stTimeZone.Bias;
		    if (TIME_ZONE_ID_UNKNOWN == dwTZVal)
            {
				trace(L6, _T("-CReportingBase::TransactionStart Time zone id unknown"));
            }
        }
		CString csBias; 	csBias.Format(_T("%s=\"%d\" "), ATTBIAS, m_lBias);
			// END SRXXX CSV Reporting
			
																					// csBias  / SRXXX
       m_csRecord = m_csStart + csTime + csStation + csBDate + m_csTypeStart + csBias  + csID ;
       lRetCode =  WriteToCollector(m_csRecord);
       trace(L7, _T("-CReportingBase::TransactionStart"));
       return lRetCode;

     }
     else
     {
         trace(L7, _T("-CReportingBase::TransactionStart Not InProgress"));
     }
         return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    PickListItemSold                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures PickList items that are sold.  
//              The parameter list include the item UPC.
// Parameters:  strItemUPC      - Item UPC
// Returns:     long
// Example:     <PickListItemSold UPC="12345678" TimesSold="1" />
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::PickListItemSold( const CString& strItemUPC, int nTimesSold /*= 1*/ )
{
	trace( L7, _T("+CReportingBase::PickListItemSold") );
    
	long lRetCode = RPT_INVALIDPARM;

	CString strRecord( _T("") );
	strRecord.Format( _T("<%s %s=\"%s\" %s=\"%d\" %s=%s />"), PICKLISTITEMSOLD, ATTITEMUPC, strItemUPC, ATTTIMESSOLD, nTimesSold, ATTTIME, GetTimeAsString() );
    lRetCode = WriteToCollector( strRecord );
    
	trace( L7, _T("-CReportingBase::PickListItemSold<%s>"), strRecord );
    
	return lRetCode;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Function:    WriteToCollector                                        
// Class:       CReportingBase                                              
// Description: A private method asynchronously put a message on a queue;This method
//              is called by all other log methods to call WriteToReportData in the Collector object. 
//              WriteToReportData writes the line to c:\scot\report\rpt_Lanenum_mmddyyyyHHMMSS.dat
// Parameters:  csXmlFormatted  -Formated CString
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::WriteToCollector(CString csXmlFormatted)
{
    long lRetCode = RPT_INVALIDPARM;
#ifndef _CPPUNIT
    trace(L7, _T("+CReportingBase::WriteToCollector"));
    if(co.fOperationsLogDataForSystemReports && !TrainingModeObj.IsTMOn()) //Capture Reporting data 
    {
        // +387731 - Migrated changes from E5
        _bstr_t xmlString(csXmlFormatted) ;
        // -387731
        
        lRetCode = WriteReportData(xmlString.copy()); // cs185088 mem leak 2012.09.11
        trace(L7, _T("-CReportingBase::WriteToCollector<%s>"), csXmlFormatted);
        return lRetCode;
    }
    else 
#endif
        return lRetCode;
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    TBCreateReportEntry                                        
// Class:       CReportingBase                                              
// Description: This is a generic method that will create a generic xml string 
//              based on the parameters passed.  The purpose of the method is 
//              to allow a Transaction Broker developer to write events other 
//              than the predefined events to the report file when necessary. 
//              This method will call the WriteToCollector. The csRecordType will 
//              be the node that the Transaction Broker will be creating and the 
//              csOtherValues will be the value of the node surrounded by quotation marks.     
// Parameters:  csRecordType    -Record Type
//              csOtherValues   -Value of record type
//              Example:
//              CString  values;
//              values = "CustomerName=\"John Doe\" ZipCode=\"40590\" ID=\"5\"";
//              long a = TBCreateReportEntry ("Survey",  values);
//              values = "AssistanceRequired=\"No\", "PreviousExperience=\"Yes\" ID=\"5\"";
//              a = TBCreateReportEntry("Survey", values);
// Returns:     long
// Example:     <TB Time="2012-10-04T18:45:29.694" TBTestRecord="Number1" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::TBCreateReportEntry(CString csRecordType, CString csOtherValues)
{
    trace(L7, _T("+CReportingBase::TBCreateReportEntry"));
    CString csTime, csTBrecord;
    long  lRetCode = RPT_INVALIDPARM;
    m_csNameStart = csRecordType;
    m_csStart.Format(_T("<%s Time="), m_csNameStart);
    csTime = GetTimeAsString();
    csTBrecord.Format(_T(" %s />"),csOtherValues);
    m_csRecord = m_csStart + csTime + csTBrecord;
    lRetCode = WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::TBCreateReportEntry<%s>"), m_csRecord);
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    InterventionStart                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures the valid intervention type, 
//              intervention class and the item number associated with the intervention.  
//              Modification to SCOTDeferredOverrideClass will include a call to this method. 
//              The handle which will be the ID and the category of the 
//              Intervention being saved.   
// Parameters:  csDesc      - Description of Intervention being Captured
//              hlD         - Intervention ID
//              m_csType    - Type Immediate or delayed                                                 
// Returns:     long 
// Example:     <Intervention Time="2012-10-04T18:45:42.291" ID="2051358720" Type="Start" 
//              Class="CALLFORHELP" InterventionType="DELAYED" /> 
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::InterventionStart(CString csDesc, RAHANDLE hlD,DWORD dwType)
{
   trace(L7, _T("+CReportingBase::InterventionStart")); 
   CString csReportPrority, cshID, csClass,csInterType,csItemCount,csTime,csItemID;  
   long  lRetCode = RPT_INVALIDPARM;   
   if(csDesc == ATTSUSPEND)
   {
       trace(L7, _T("-CReportingBase::InterventionStart<RPT_INVALIDPARM><%s> SUSPENDs are Not included in rpt.xml"),csDesc ); 
       return RPT_INVALIDPARM;
   }
   else
   {

    m_csNameStart        = INTERVENTION;
    m_csStart.Format(_T("<%s"), m_csNameStart);
    csClass.Format(_T("%s=\"%s\" "),ATTCLASS,csDesc); 
    csItemCount.Format(_T("%s=\"%d\"/>"),ATTITEMCOUNT, m_lItemCount);
    csItemID.Format(_T("%s=\"%d\"/>"),ATTITEMID,ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_GET));
    cshID.Format(_T("%s=\"%d\" "),ATTIDID, hlD);
    csTime = GetTimeAsString();
    csReportPrority = (dwType == RA_PRIORITY_URGENT ? ATTIMMEDIATE :ATTDELAYED);
	//TAR 371932 & tar 371918+
	//m_csInterventionTerminalNumber = m_csTerminalNumber;
    //csStation.Format(_T("%s=\"%s\" "),ATTDEFSTATION,m_csInterventionTerminalNumber);
	//TAR 371932 & tar 371918-
    if (m_bItemAssoc)
    {
        //Item associated with Intervention
        csInterType.Format(_T("%s=\"%s\" "),ATTINTERVENTIONTYPE,csReportPrority);
        m_csRecord = m_csStart + csTime + cshID + m_csTypeStart + csClass + csInterType + csItemID;
        m_bItemAssoc =false;
   }
   else
   {
        //No item associated with Intervention
        csInterType.Format(_T("%s=\"%s\"/>"),ATTINTERVENTIONTYPE,csReportPrority);
        m_csRecord = m_csStart + csTime + cshID + m_csTypeStart + csClass + csInterType;
   }
    lRetCode = WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::InterventionStart<%s>"),m_csRecord ); 
    return lRetCode;
   }
  
}
///////////////////////////////////////////////////////////////////////////////
// Function:    InterventionEnd                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures the end time of the transaction 
//              and the valid intervention type, the station identifier where the 
//              intervention was cleared. The identifier of the operator will also be 
//              obtained and any pager events that was generated in the results of the 
//              current intervention. Modification to SCOTDeferredOverrideClass will include
//              a call to this method.     
// Parameters:  csDesc      -Description of Intervention being Captured
//              hlD         -Intervention ID
// Returns:     long
// Example:     <Intervention Time="2012-10-04T18:46:39.471" ID="2051358720" Type="End" Pager="TRUE" 
//              Trans="IN" Station="1" Operator="1" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::InterventionEnd(CString csDesc, RAHANDLE hlD)
{
   long  lRetCode = RPT_INVALIDPARM;
   trace(L7, _T("+CReportingBase::InterventionEnd")); 
   if (hlD == -1)
   {
       trace(L7, _T("-CReportingBase::InterventionStart<RPT_INVALIDPARM><%s> -1 ID Detected"),csDesc ); 
       return RPT_INVALIDPARM;
   }
   CString cshID,csOperator,csPagerEvent,csStation,csTime, csActive, csPager, csStatus, csTerminator, csIsApproved;  //SR803
   CString csClearingOperator;
   rp.SetInterventionEndInProgress(false);  // TAR 311902
   csTime = GetTimeAsString();
   m_csNameStart = INTERVENTION;
   m_csStart.Format(_T("<%s"), m_csNameStart);
   cshID.Format(_T("%s=\"%d\" "),ATTIDID, hlD);
   csClearingOperator = m_csOperatorNumber;
   if ((m_csInterventionTerminalNumber == _T("RAP")) && m_bTransactionInProgress)
   {
         m_bRapAssisted = true;
         // There should always be an operator number if clearing from RAP
        if (csClearingOperator == EMPTY_STRING)
        {
            csClearingOperator = m_csTenderOperator;
        }
   }
   else
   {
       m_csInterventionTerminalNumber = m_csTerminalNumber;
       if ((csClearingOperator != EMPTY_STRING) && m_bTransactionInProgress)
       {
           // If operator cleared it, this was an assisted transaction otherwise not
            m_bTransAssisted = true;
       }
   }
   //TAR 371932 & tar 371918+
   if (csClearingOperator == EMPTY_STRING )
   {
	   //+TAR409050
	   if ((csDesc == _T("VISUAL VERIFIED") || csDesc == _T("UNKNOWN")
		   //+SSCOP-3775
		   || csDesc == _T("UNKNOWN ITEM") || csDesc == _T("RECALLED ITEM") 
		   || csDesc == _T("TIME RESTRICTED") || csDesc == _T("CALLED FOR HELP")
           || csDesc == _T("QUANTITY RESTRICTED"))
		   //-SSCOP-3775
		   && (m_csTenderOperator != EMPTY_STRING))
	   {	
		   csClearingOperator = m_csTenderOperator;
		   if (m_bTransactionInProgress)
			   m_bTransAssisted = true;   
	   }
	   //-TAR409050
	   else
	   csClearingOperator = csDummyClearingOperator;
   }
   //TAR 371932 & tar 371918-
   if(csDesc == ATTSUSPEND)
   {
       trace(L7, _T("-CReportingBase::InterventionStart<RPT_INVALIDPARM><%s> SUSPENDs are Not included in rpt.xml"),csDesc ); 
       return RPT_INVALIDPARM;
   }
   else
   {
       csStation.Format(_T("%s=\"%s\" "),ATTSTATION,m_csInterventionTerminalNumber);
       csOperator.Format(_T("%s=\"%s\" "),ATTOPERATOR,csClearingOperator);
       csPager = (m_bpagerEvent ? ATTPAGERT:ATTPAGERF);
       csPagerEvent.Format(_T("%s=\"%s\" "),ATTPAGEREVENT,csPager);
       csStatus = (m_bTransactionInProgress ? ATTIN : ATTOUT);
       csActive.Format(_T("%s=\"%s\" "),ATTACTIVE,csStatus);
       csIsApproved.Format(_T("%s=\"%d\" "),ATTAPPROVED,m_bIsInterventionApproved);  //SR803
       csTerminator = _T("/>");
       m_csRecord = m_csStart + csTime + cshID + m_csTypeEnd + csPagerEvent + csActive + csStation + csIsApproved;  //SR803
       if (csClearingOperator != EMPTY_STRING)
       {
          m_csRecord +=  csOperator ;
       }
       m_csRecord += csTerminator;

       if (m_bItemAssoc)
       {
            m_bItemAssoc = false;
       }

       lRetCode= WriteToCollector(m_csRecord);
       trace(L7, _T("-CReportingBase::InterventionEnd<%s>"),m_csRecord ); 
// RFQ 7477
	     //391498+
       if (m_csInterventionTerminalNumber == _T("RAP"))
	   {
	      //send operator ID to report
	      rp.OperatorLogin(m_lOperatorLoginID, ATTIN, _T("RAP"));
	      m_lOperatorLoginID += 1;
	      rp.OperatorLogin(m_lOperatorLoginID, ATTOUT, _T("RAP"));
	   }
       //391498
// RFQ 7477
       m_csInterventionTerminalNumber = m_csTerminalNumber;
       return lRetCode;
   }
}

///////////////////////////////////////////////////////////////////////////////
// Function:    UnknownItemException                                        
// Class:       CReportingBase                                              
// Description: is a public method that calls the  method ID() to generate and 
//              or gets a item id for the current UnknownItemExceptionitem. 
// Parameters:  csItemSku       -Item SKU number


// Returns:     long
// Example:     <ItemException Time="2012-10-04T18:49:32.32" ExceptionType="UNKNOWNITEM" ID="1" SKU="4444" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::UnknownItemException(CString csItemSku)
{
    trace(L7, _T("+CReportingBase::ItemException")); 
    CString csSKU,csIDIE,csExcepType,csTime;
    long  lRetCode = RPT_INVALIDPARM;
    m_csNameStart = ITEMEXCEPTION;
    m_csStart.Format(_T("<%s"), m_csNameStart);
    ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_INCREMENT);
    csIDIE.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_GET));
	//TAR412798+
	if (csItemSku.GetLength() > 50 )
	{
	  csItemSku = csItemSku.Left(50);	
	}
	//TAR412798-
    csSKU.Format(_T("%s=\"%s\"/>"),ATTITEMSKU, csItemSku);
    csExcepType.Format(_T("%s=\"%s\" "),ATTEXCEPTYPE, ATTUNKNOWN);
    csTime = GetTimeAsString();
    m_csRecord = m_csStart + csTime + csExcepType + csIDIE + csSKU ;
    lRetCode=WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::ItemException<%s>"), m_csRecord); 
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    ItemException                                        
// Class:       CReportingBase                                              
// Description: is a public method that calls the  method ID() to generate and 
//              or gets a item id for the current item. The parameter list include
//              the description associated with the current item identifier, the item
//              SKU associated with the current item identifier and the item price associated 
//              with the current item identifier
// Parameters:  csItemDesc      -Description of Item being Captured
//              csItemSku       -Item Sku
//              lItemPrice      -Price associated with the item 
//              csObservedWts   -Observed weight
//              csExpected      -Expected weight                                                
// Returns:     long
// Example:     <ItemException Time="2012-10-04T19:33:53.413" ID="1" ExceptionType="MISMATCHWEIGHT" 
//              SKU="#A030000070604" Description="Oat Bran" Price="$1.00" ObservedWeight="8.00" Station="1" 
//              ExpectedWeight="0.00" />                                         
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::ItemException(CString csItemDesc, CString csItemSku, long lItemPrice,CString csObservedWts, CString csExpected)
{
    trace(L7, _T("+CReportingBase::ItemException")); 
    CString csSKU, csMisMatchDesc, csObserved, csExpectedWt, csItemPrice,csIDIE,csExcepType,csTime;
    long  lRetCode = RPT_INVALIDPARM;
    m_csNameStart = ITEMEXCEPTION;
    m_csStart.Format(_T("<%s"), m_csNameStart);
    csSKU.Format(_T("%s=\"%s\" "),ATTITEMSKU, csItemSku);
	CString csFormattedDesc =  EscapeText(csItemDesc); //TAR440565 
    csMisMatchDesc.Format(_T("%s=\"%s\" "),ATTMSMATCHDESC, csFormattedDesc);
    csObserved.Format(_T("%s=\"%s\" "),ATTOBSERVEDWEIGHT, csObservedWts);
    csExpectedWt.Format(_T("%s=\"%s\"/>"),ATTEXPECTEDWEIGHT, csExpected);
    csIDIE.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_GET));
    csItemPrice.Format(_T("%s=\"%s\" "),ATTITEMPRICE, ps.FmtDollar(lItemPrice,(enum PSProceduresBase::SCREEN_POSITION) 0, 
      ps.REPORTINGFORMAT));
    csExcepType.Format(_T("%s=\"%s\" "),ATTEXCEPTYPE, _T("MISMATCHWEIGHT"));
    csTime = GetTimeAsString();

    m_csRecord = m_csStart + csTime + csIDIE + csExcepType + csSKU + csMisMatchDesc + csItemPrice + csObserved + csExpectedWt ;
    lRetCode=WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::ItemException<%s>"), m_csRecord); 
    return lRetCode;
}

///////////////////////////////////////////////////////////////////////////////
//Function:		ModeChange
//Class:		CReportingBase
//Description:	is a public method that captures:
//				Application Mode name: AttendantMode, TrainingMode,..
//				ModeChangeType: Enter or Exit
//				ModeChangeTime: System Time when the mode changed
//				Operator ID:	ID of the operator who enter/exit out the mode
//				InTransaction:	True or False.
//parameters:	ID				-Unique id's for enter/exit out the mode within the same transaction
//				csModeName		-Name of the mode
//				csChangeType	-Enter or Exit
//Returns:		long
//Example:		<ModeChange Time="2004-09-09T16:29:02.36" Station="1" StoreTransactionID="" ID="0" ModeName="ATTENDANT" TypeChange="ENTER" Operator="1" Trans="1"/>	

////////////////////////////////////////////////////////////////////////////////
long CReportingBase::ModeChange(int ID, CString csModeName, CString csChangeType)
{
	trace(L7, _T("+CReportingBase::ModeChange"));
	CString csTime, csStation, csModeChangeName, csModeChangeType, csOperator, csStatus, csActive, csModeChangeID;
    long  lRetCode = RPT_INVALIDPARM;
    m_csNameStart = MODECHANGE;
    m_csStart.Format(_T("<%s"), m_csNameStart);
	csTime = GetTimeAsString();
	csStation.Format(_T("%s=\"%s\" "),ATTSTATION,m_csTerminalNumber);
	csModeChangeID.Format(_T("%s=\"%d\" "),ATTMODEID, ID);
	csModeChangeName.Format(_T("%s=\"%s\" "),ATTMODENAME,csModeName);
	csModeChangeType.Format(_T("%s=\"%s\" "),ATTMODECHANGETYPE,csChangeType);
	csOperator.Format(_T("%s=\"%s\" "),ATTOPERATOR,m_csOperatorNumber);
	csStatus = (SMStateBase::fSAInTransaction ? ATTIN : ATTOUT);
    csActive.Format(_T("%s=\"%s\"/>"),ATTACTIVE,csStatus);


	m_csRecord = m_csStart + csTime + csStation + csModeChangeID + csModeChangeName
		+ csModeChangeType + csOperator + csActive;
    lRetCode=WriteToCollector(m_csRecord);

	trace(L7, _T("-CReportingBase::ModeChange<%s>"),m_csRecord );
	return lRetCode;
	
}
///////////////////////////////////////////////////////////////////////////////
//Function:		OperatorLogin
//Class:		CReportingBase
//Description:	is a public method that captures:
//				OperatorLogin
//				ModeChangeTime: System Time of the login/off
//				SignOn:	1 or 0 to indicate in/off
//				Application:	SCOTAPP, RAP, or UTILITY. Probably only SCOTAPP is called from ScotApp
//parameters:	ID				-Unique id's for sign on/offs within the same transaction
//				csSignOn		-1 or 0 to indicate in/off
//				csApplication	-SCOTAPP, RAP, or UTILITY. Probably only SCOTAPP is called from ScotApp
//Returns:		long
//Example:		<OperatorLogin Time =2005-10-22-T14:16:18.57 Operator=12 SignOn=1 Application=SCOTAPP />
////////////////////////////////////////////////////////////////////////////////
long CReportingBase::OperatorLogin(int ID, CString csSignOn, CString csApplication)
{
	trace(L7, _T("+CReportingBase::OperatorLogin"));
	CString csTime, csOperator, csOperatorLoginID, csSignOnText, csApplicationText;
    long  lRetCode = RPT_INVALIDPARM;
    m_csNameStart = OPERATORLOGIN;
    m_csStart.Format(_T("<%s"), m_csNameStart);
	csTime = GetTimeAsString();
	csOperatorLoginID.Format(_T("%s=\"%d\" "),ATTMODEID, ID);

// RFQ 7477
	//391498+
	if (m_csOperatorNumber == EMPTY_STRING )
    {
	  csOperator.Format(_T("%s=\"%s\" "),ATTOPERATOR,csDummyClearingOperator);
	  
    }
	else
	//391498-
	{
		csOperator.Format(_T("%s=\"%s\" "),ATTOPERATOR,m_csOperatorNumber);
	}
// RFQ 7477
	csSignOnText.Format(_T("%s=\"%s\" "),SIGNON,csSignOn);
	csApplicationText.Format(_T("%s=\"%s\"/>"),APPLICATION,csApplication);
	
	m_csRecord = m_csStart + csTime + csOperator + csOperatorLoginID + csSignOnText + csApplicationText;
    lRetCode=WriteToCollector(m_csRecord);

	trace(L7, _T("-CReportingBase::OperatorLogin<%s>"),m_csRecord );
	return lRetCode;
}
#endif //_CPPUNIT

///////////////////////////////////////////////////////////////////////////////
// Function:    TransactionEnd                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures is a public method that captures:
//              The total number of items associated with the transaction.
//              The total amount associated with the entire transaction.
//              TransDisposition-can be set to Cancel, Suspend or Complete 
//              TransOfflineFlag is set to true if the lane attempted to communicate 
//              with the security server during an offline state.
//              TransAssistedflag- True or False, 
//              RapAssisted - True or False   
// Parameters:  NONE                                             
// Returns:     long
// Example:     <Transaction Time="2012-10-04T18:51:47.712" NumberOfItems="1" Amount="$1.10" 
//              Disposition="3" OfflineFlag="0" AssistedFlag="1" RapAssitedFlag="0" Type="End" ID="1" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::TransactionEnd()
{
    trace(L7, _T("+CReportingBase::TransactionEnd"));
    CString csItemCount,csDisposition, csAmount,csOffline,csTransAssisted,csRapAssited,csID,csTime, csTransID;
	CString csAutoAdvUnexpInc, csAutoAdvMismatch, csTransAttendant, csLanguage;
    CString csMobileShopper, csNumberOfItemsMobileShopper;
 
    long lRetCode = RPT_INVALIDPARM;
    csTime = GetTimeAsString();
    if(m_bTransactionInProgress || m_bForceEnd)
    {
#ifndef _CPPUNIT
        if (!m_bTransactionInProgress )
        {
            rp.TransactionStart(m_csBusinessDate);
        }
        if (rp.m_pNewTransacationDOM == NULL)
        {
            //If you made it here you must not have Reporting turned on
            m_bTransactionInProgress = false;
            InitData();
            return lRetCode;
        }
        m_csNameStart = TRANSACTION;

		//+TAR410994
		csTransID.Format(_T("%s=\"%s\" "), ATTSTOREID,m_csTransactionNumber);
        if (m_csTransactionNumber ==_T(""))
            trace(L5, _T("+m_csTransactionNumber is EMPTY"));
		//-TAR410994
        m_csStart.Format(_T("<%s"), m_csNameStart);
        csID.Format(_T("%s=\"%d\"/>"),ATTIDID,ID(RP_IDTYPE_TRANS,RP_IDOPERATION_GET)); 
        csDisposition.Format(_T("%s=\"%d\" "),ATTDISPOSITION, m_nDisposition);
        csAmount.Format(_T("%s=\"%s\" "),ATTAMOUNT, ps.FmtDollar(m_lTransactionTotal,(enum PSProceduresBase::SCREEN_POSITION) 0, 
          ps.REPORTINGFORMAT));
        csItemCount.Format(_T("%s=\"%d\" "),ATTITEMCOUNT, m_lItemCount);
        csOffline.Format(_T("%s=\"%d\" "),ATTOFFLINE, m_bOffline);
        csTransAssisted.Format(_T("%s=\"%d\" "),ATTTRANSASSISTED, m_bTransAssisted);
        csRapAssited.Format(_T("%s=\"%d\" "),ATTRAPASSISTED, m_bRapAssisted);
		csTransAttendant.Format(_T("%s=\"%d\" "),ATTTRANSATTENDANT, co.IsAttendantModeOn());
        csAutoAdvUnexpInc.Format(_T("%s=\"%d\" "),AUTOADVUNEXPINC, SMStateBase::nAdvanceUnexpectedIncreaseCnt-1); // 3.2 patch B -RFC 257450
		csAutoAdvMismatch.Format(_T("%s=\"%d\" "),AUTOADVMISMATCH, SMStateBase::nAdvanceMismatchCnt-1); // 3.2 patch B -RFC 257449
#endif
        csMobileShopper.Format(_T("%s=\"%d\" "),ATTMOBILIESHOPPER, m_bMobileShopper);
        csNumberOfItemsMobileShopper.Format(_T("%s=\"%d\" "),ATTMOBILESHOPPERITEMCOUNT, SMStateBase::ms_lMobileShopperItemCount);
        if(!m_csLanguage.IsEmpty())
		{
			csLanguage.Format(_T("%s=\"%s\" "),ATTLANGUAGE, m_csLanguage);
		}
		else
		{
			csLanguage.Empty();
		}
        m_bTransactionCompleted = true;
        m_bTransactionInProgress = false;
		m_lAttendantEntryID = 0; // reset the unique per-transaction ID’s to 0
		m_lOperatorLoginID = 1; // A unique per-transaction ID for operator login... reset it to 1

        m_csRecord = m_csStart + csTime + csMobileShopper + csNumberOfItemsMobileShopper + csItemCount + csAmount + csDisposition + csOffline 
			+ csTransAssisted + csRapAssited + csTransAttendant + m_csTypeEnd + csAutoAdvUnexpInc
            + csAutoAdvMismatch + csLanguage + csTransID +csID;
        lRetCode=WriteToCollector(m_csRecord);
        //Clearing all ID for next transaction
        ID(RP_IDTYPE_ITEMS,RP_IDOPERATION_CLEARALL);
        ID(RP_IDTYPE_TRANS,RP_IDOPERATION_CLEARALL);
        ID(RP_IDTYPE_TENDER,RP_IDOPERATION_CLEARALL);
        ID(RP_IDTYPE_SIGNID,RP_IDOPERATION_CLEARALL);
        ID(RP_IDTYPE_CURRENCYA,RP_IDOPERATION_CLEARALL);
        ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_CLEARALL);
        m_bRapAssisted   = false;                //default value for RapAssited for Transacation End Entry
        m_bTransAssisted = false;                //default value for TransAssited for Transaction End Entry
        TransactionCompleted(m_csTerminalNumber);
		m_csLanguage.Empty();
        m_csTransactionID.Empty();
        InitData();  // DEM TAR 449618
    }
    trace(L7, _T("-CReportingBase::TransactionEnd<%s>"),m_csRecord );
    return lRetCode;
}

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    TenderStart                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures receive station, operator ID and tender type.
// Parameters:  nTenderType         -Type of tender being captured
// Returns:     long
// Example:     <Tender Time="2012-10-04T18:51:24.890" ID="1" Type="Start" TenderType="CASH" 
//              Operator="1" Station="1" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::TenderStart(int nTenderType)
{   
    trace(L7, _T("+CReportingBase::TenderStart"));
    CString csOperator, csTenderType,csID,csStation,csTime;
    long  lRetCode = RPT_INVALIDPARM;
    SCOT_LANGUAGE_TYPE customerLanguage; 
    csTime = GetTimeAsString();
    m_csNameStart = TENDERS;
    m_csStart.Format(_T("<%s"), m_csNameStart);
    ID(RP_IDTYPE_TENDER,RP_IDOPERATION_INCREMENT);
    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_TENDER,RP_IDOPERATION_GET));

    //Report tenders are always captured in Primary language
    customerLanguage = ps.GetLanguage();
	//+TAR 444373 - DO NOT CALL SetLanguage() to change the language code to get the right message.  We should 
	// pass in the language code to GetPSText()
    //ps.SetLanguage(SCOT_LANGUAGE_PRIMARY);

    CString csCat = EMPTY_STRING;
    switch(nTenderType)
    {
    case TB_TENDER_COUPON:
        csCat = ps.GetPSText(MSG_TB_TENDER_COUPON, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_OTHERPAYMENT:
        csCat = ps.GetPSText(MSG_TB_TENDER_OTHERPAYMENT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_NONE:
        csCat = ps.GetPSText(MSG_TB_TENDER_NONE, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_CASH:
        csCat = ps.GetPSText(MSG_TB_TENDER_CASH, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_CREDIT:
        csCat = ps.GetPSText(MSG_TB_TENDER_CREDIT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_DEBIT:
        csCat = ps.GetPSText(MSG_TB_TENDER_DEBIT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_LOYALTY:
        csCat = ps.GetPSText(MSG_TB_TENDER_LOYALTY, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_DEBITSAVING:
        csCat = ps.GetPSText(MSG_TB_TENDER_DEBITSAVING, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_DEBITCHECKING:
        csCat = ps.GetPSText(MSG_TB_TENDER_DEBITCHECKING, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_EBT:
        csCat = ps.GetPSText(MSG_TB_TENDER_EBT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_FOODSTAMPS:
        csCat = ps.GetPSText(MSG_TB_TENDER_FOODSTAMPS, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_GIFTCARD:
        csCat = ps.GetPSText(MSG_TB_TENDER_GIFTCARD, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_GENERIC:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_GENERIC, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_CREDIT:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_CREDIT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_DEBIT:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_DEBIT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_DEBIT_CASHBACK:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_DEBIT_CASHBACK, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_LOYALTY:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_LOYALTY, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_DEBITSAVING:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_DEBITSAVING, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_DEBITCHECKING:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_DEBITCHECKING, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_EBT:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_EBT, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_FOODSTAMPS:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_FOODSTAMPS, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_PIP_GIFTCARD:
        csCat = ps.GetPSText(MSG_TB_TENDER_PIP_GIFTCARD, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED1:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED1, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED2:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED2, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED3:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED3, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED4:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED4, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED5:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED5, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED6:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED6, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED7:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED7, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED8:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED8, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED9:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED9, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED10:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED10, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED11:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED11, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED12:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED12, SCOT_LANGUAGE_PRIMARY);  // TAR 248036 - Fix typo error
        break;
    case TB_TENDER_USERDEFINED13:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED13, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED14:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED14, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED15:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED15, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED16:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED16, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED17:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED17, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED18:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED18, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED19:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED19, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_USERDEFINED20:
        csCat = ps.GetPSText(MSG_TB_TENDER_USERDEFINED20, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_CHECK:     //RFC 356020 - Tender check at rap
	case TB_TENDER_CHECK_LANE://RFC 356020 	- Tender check at Lane // RFQ 7477
        csCat = ps.GetPSText(MSG_TB_TENDER_CHECK, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_CASH_RAP:
        csCat = ps.GetPSText(MSG_TB_TENDER_CASH_RAP, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_ASSISTTENDER:
        csCat = ps.GetPSText(MSG_TB_TENDER_ASSISTTENDER, SCOT_LANGUAGE_PRIMARY);
        break;
    case TB_TENDER_VOUCHERPAYMENT:
        csCat = ps.GetPSText(MSG_TB_TENDER_VOUCHERS, SCOT_LANGUAGE_PRIMARY);
        break;
        }

		//-TAR 444373

        csTenderType.Format(_T("%s=\"%s\" "),ATTTENDERTYPE, csCat);
        csStation.Format(_T("%s=\"%s\"/>"),ATTSTATION,m_csTenderTerminalNumber);
        csOperator.Format(_T("%s=\"%s\" "),ATTOPERATOR,m_csTenderOperator);
             
        if (m_csTenderOperator == EMPTY_STRING)
        {
            m_csRecord = m_csStart + csTime + csID + m_csTypeStart + csTenderType +  csStation;
        }
        else
        {
            m_csRecord = m_csStart + csTime + csID + m_csTypeStart + csTenderType + csOperator + csStation;
        }
    //ps.SetLanguage(customerLanguage);
    lRetCode=WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::TenderStart<%s>"), m_csRecord);
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    TenderEnd                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures the tender total.   
// Parameters:  ltendertotal        -Tender total being Captured
// Returns:     long
// Example:     <Tender Time="2012-10-04T18:51:25.972" ID="1" Type="End" Amount="$2.00" />                                         
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::TenderEnd (long ltendertotal)
{
    trace(L7, _T("+CReportingBase::TenderEnd"));
    CString csAmount,csID,csTime;
    long  lRetCode = 0;
    csTime = GetTimeAsString();
    m_csNameStart = TENDERS;
    m_csStart.Format(_T("<%s"),m_csNameStart);
    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_TENDER,RP_IDOPERATION_GET));
    csAmount.Format(_T("%s=\"%s\"/>"),ATTAMOUNT, ps.FmtDollar(ltendertotal,(enum PSProceduresBase::SCREEN_POSITION) 0,
      ps.REPORTINGFORMAT));
    m_csRecord = m_csStart + csTime + csID + m_csTypeEnd + csAmount;
    lRetCode=WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::TenderEnd<%s>"), m_csRecord);
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyAccepted                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, the currency type, i.e. note or coin 
//              The denomination of the currency being accepted.
//              The quantity of this denomination being dispensed.
// Parameters:  bBill           -Note or Coin
//              csDen           -Denomination of the NOte or Coin
//              nQuantity       -FastLane 3.1, only can except 1 at a time.
//              lAcceptedAmount -Amount being dispensed                                                
// Returns:     long
// Example:     <CurrencyAccepted Time="2012-10-04T18:51:21.395" ID="1" Note="1" Denomination="200" 
//              Quantity="1" Amount="$2.00" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyAccepted(bool bBill, CString csDen, int nQuantity, long lAcceptedAmount)
{
    trace(L7, _T("+CReportingBase::CurrencyAccepted"));
    CString csAmount, csCurrencyType,csquantity,csDenomination,csID,csTime;
// RFQ 7477
	//Meijer CQ 5572
	strtCurrency stCurrency;

	stCurrency.lDen = nQuantity ? lAcceptedAmount/nQuantity : 0;
	stCurrency.bBill = bBill;
    stCurrency.lQuantity = (long) nQuantity;
	stCurrency.bAccepted = true;	

	m_SCurrencyList.push_back(stCurrency);

	//end CQ
// RFQ 7477
    long lRetCode = 0;
    if (!IsCashManagementInProgress()) // TAR 450658
    {
        csTime = GetTimeAsString();
        m_csNameStart = CURRENCYA;
        m_csStart.Format(_T("<%s"), m_csNameStart); 
        ID(RP_IDTYPE_CURRENCYA,RP_IDOPERATION_INCREMENT);
        csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_CURRENCYA,RP_IDOPERATION_GET));
        csAmount.Format(_T("%s=\"%s\"/>"),ATTAMOUNT, ps.FmtDollar(lAcceptedAmount, (enum PSProceduresBase::SCREEN_POSITION) 0, 
            ps.REPORTINGFORMAT));
        csquantity.Format(_T("%s=\"%d\" "),ATTQUANTITY, nQuantity);
        csDenomination.Format(_T("%s=\"%s\" "),ATTDENOMINATION, csDen);
        csCurrencyType.Format(_T("%s=\"%d\" "),ATTCURRENCYTYPE,bBill);
        m_csRecord = m_csStart + csTime + csID + csCurrencyType + csDenomination + csquantity + csAmount;
        lRetCode=WriteToCollector(m_csRecord);
    }
    else
    {
        trace(L6, _T("CReportingBase::CurrencyAccepted CM in progress - Ignored"));
    }
    trace(L7, _T("-CReportingBase::CurrencyAccepted<%s>"), m_csRecord);
    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyDispensed                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, the currency type, i.e. note or coin 
//              The denomination of the currency being dispensed.
//              The quantity of this denomination being dispensed.
// Parameters:  bBill           -Note or Coin
//              csDen           -Denomination of the Note or Coin
//              lQuantity       -How many Notes or Coins being dispensed
//              lDispensedAmount-Amount being dispensed                                                
// Returns:     long
// Example:     <CurrencyDispensed Time="2012-10-04T18:51:27.894" ID="1" Note="0" Denomination="5" 
//              Quantity="1" Amount="$0.05" />                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyDispensed(bool bBill, CString csDen,long lQuantity, long lDispensedAmount)
{
    trace(L7, _T("+CReportingBase::CurrencyDispensed"));
    CString csAmount,csCurrencyType,csquantity,csDenomination,csID,csTime;
// RFQ 7477
	//Meijer CQ 5572
	strtCurrency stCurrency;

	stCurrency.lDen = lQuantity ? lDispensedAmount/lQuantity : 0;
	stCurrency.bBill = bBill;
    stCurrency.lQuantity = lQuantity;
	stCurrency.bAccepted = false;

	m_SCurrencyList.push_back(stCurrency);
	//end CQ
// RFQ 7477
    long lRetCode = 0;
    if (!IsCashManagementInProgress())  // TAR 450658
    {
        csTime = GetTimeAsString();
        m_csNameStart = CURRENCYD;
        m_csStart.Format(_T("<%s"), m_csNameStart);
        ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_INCREMENT);
        csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_GET));
        csAmount.Format(_T("%s=\"%s\"/>"),ATTAMOUNT, ps.FmtDollar(lDispensedAmount,(enum PSProceduresBase::SCREEN_POSITION) 0, 
          ps.REPORTINGFORMAT));
        csquantity.Format(_T("%s=\"%d\" "),ATTQUANTITY, lQuantity);// will only be 1, later we will option for multiple
        csDenomination.Format(_T("%s=\"%s\" "),ATTDENOMINATION, csDen);
        csCurrencyType.Format(_T("%s=\"%d\" "),ATTCURRENCYTYPE,bBill);
        m_csRecord = m_csStart + csTime + csID + csCurrencyType + csDenomination +  csquantity + csAmount;
        lRetCode=WriteToCollector(m_csRecord);
    }
    else
    {
        trace(L6, _T("CReportingBase::CurrencyDispensed CM in progress - Ignored"));
    }
    trace(L7, _T("-CReportingBase::CurrencyDispensed<%s>"), m_csRecord);
    return lRetCode;
}

// RFQ 7477
//+SOTF 5572
CString CReportingBase::GetGurrencyList(void)
{
	
	CString csCurrencyList(_T(""));	
	long lDollarBillAccepted =0, lDollarCoinAccepted =0;
	long lCurrencyListSize = m_SCurrencyList.size();
	long l;

	//is there anything to do?
	if(lCurrencyListSize == 0)
	{
		return csCurrencyList;
	}


	//setup the temporary working variables
	std::vector<strtCurrency> vecCurrencyAcceptedList;
	std::vector<strtCurrency> vecCurrencyDispensedList;

	vecCurrencyAcceptedList.clear();
	vecCurrencyDispensedList.clear();

	strtCurrency stCurrency;

	stCurrency.lDen = 0;
	stCurrency.bBill = false;
    stCurrency.lQuantity = 0;
	stCurrency.bAccepted = false;

    //commented out the insert and replaced with the loop
    //the insert function was causing a compiler crash only in release mode
    //fatal error C1001: INTERNAL COMPILER ERROR
    //vecCurrencyAcceptedList.insert(0, MAX_NUMBER_DENOMINATIONS, stCurrency);
	//vecCurrencyDispensedList.insert(0, MAX_NUMBER_DENOMINATIONS, stCurrency);

    for(int i=0; i< MAX_NUMBER_DENOMINATIONS; i++)
    {
        vecCurrencyAcceptedList.push_back(stCurrency);
        vecCurrencyDispensedList.push_back(stCurrency);
    }
	

	

	//loop through the media that has been individually reported
	//and add up the qtys.
	for(l=0; l < lCurrencyListSize; l++)
	{

		if(m_SCurrencyList[l].bAccepted)
		{
			switch(m_SCurrencyList[l].lDen)
			{
				case 1:
				{
					vecCurrencyAcceptedList[0].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[0].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 5:
				{
					vecCurrencyAcceptedList[1].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[1].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 10:
				{
					vecCurrencyAcceptedList[2].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[2].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 25:
				{
					vecCurrencyAcceptedList[3].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[3].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}

				//the $1.00 case is the only different one because there can a $1 bill
				//or coin accepted
				case 100:
				{
					if(m_SCurrencyList[l].bBill)
					{
						lDollarBillAccepted += m_SCurrencyList[l].lQuantity;

					}else
					{
						lDollarCoinAccepted += m_SCurrencyList[l].lQuantity;
					}
					break;
				}
				case 200:
				{
					vecCurrencyAcceptedList[4].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[4].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 500:
				{
					vecCurrencyAcceptedList[5].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[5].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 1000:
				{
					vecCurrencyAcceptedList[6].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[6].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 2000:
				{
					vecCurrencyAcceptedList[7].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[7].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 5000:
				{
					vecCurrencyAcceptedList[8].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[8].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 10000:
				{
					vecCurrencyAcceptedList[9].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyAcceptedList[9].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				default:
				{
					trace(L6, _T("CReporting::GetGurrencyList Invalid Media type<%d>"), m_SCurrencyList[l].lDen);

				}
		
			}

		}else
		{
			switch(m_SCurrencyList[l].lDen)
			{
				case 1:
				{
					vecCurrencyDispensedList[0].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[0].lQuantity += m_SCurrencyList[l].lQuantity;
					
					break;
				}
				case 5:
				{
					vecCurrencyDispensedList[1].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[1].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 10:
				{
					vecCurrencyDispensedList[2].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[2].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 25:
				{
					vecCurrencyDispensedList[3].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[3].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 100:
				{
					vecCurrencyDispensedList[4].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[4].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 500:
				{
					vecCurrencyDispensedList[5].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[5].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 1000:
				{
					vecCurrencyDispensedList[6].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[6].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}

				//currently Meijer only dispenses $10.00 bills.  Add support for $20, $50,
				//and $100 in case it changes in the future
				case 2000:
				{
					vecCurrencyDispensedList[7].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[7].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				case 5000:
				{
					vecCurrencyDispensedList[8].lDen = m_SCurrencyList[l].lDen;
					vecCurrencyDispensedList[8].lQuantity += m_SCurrencyList[l].lQuantity;
					break;
				}
				//case 10000:
				//{
				//	vecCurrencyDispensedList[9].lDen = m_SCurrencyList[l].lDen;
				//	vecCurrencyDispensedList[9].lQuantity += m_SCurrencyList[l].lQuantity;
				//	break;
				//}
				default:
				{
					trace(L6, _T("CReporting::GetGurrencyList Invalid Media type<%d>"), m_SCurrencyList[l].lDen);

				}



					

			}
		} //end if for accepted or dispensed

	} //end for

	CString csAccepted(_T(""));
	CString csDispensed(_T(""));
	CString csTempAcc;
	CString csTempDis;

	for(l=0; l< MAX_NUMBER_DENOMINATIONS;l++)
	{

		if(vecCurrencyAcceptedList[l].lQuantity > 0)
		{
			csTempAcc.Format(_T("<Accepted_%d Qty=\"%d\"/>"), vecCurrencyAcceptedList[l].lDen, vecCurrencyAcceptedList[l].lQuantity);
		
			csAccepted += csTempAcc;
		}
		
		//insert the $1 coins or bills in the 4th posistion
		if(l == DOLLAR_DENOMINATION_INDEX)
		{
			if(lDollarCoinAccepted)
			{
				csTempAcc.Format(_T("<Accepted_100c Qty=\"%d\"/>"), lDollarCoinAccepted);
				csAccepted += csTempAcc;
			}
				

			if(lDollarBillAccepted)
			{
				csTempAcc.Format(_T("<Accepted_100b Qty=\"%d\"/>"), lDollarBillAccepted);
			
				csAccepted += csTempAcc;	
			}
		}

		if(vecCurrencyDispensedList[l].lQuantity > 0)
		{
			csTempDis.Format(_T("<Dispensed_%d Qty=\"%d\"/>"),vecCurrencyDispensedList[l].lDen, vecCurrencyDispensedList[l].lQuantity) ;
	
			csDispensed += csTempDis;
		}

		

	}

	csCurrencyList = csAccepted + csDispensed;

	ResetCurrencyList();
	return csCurrencyList;
}

//-SOTF5572
// RFQ 7477
//+dp185016 support glory device
///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyCashManagement                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, 
//                The denomination value list
//                The change in count for each denomination
//                The User ID used in logging into store mode
//                The Cashier ID used in Cash Management Screen.
// Parameters:  caValueList     - denomination value list
//              caChangeInCount - change in count for each denomination value
//              csUserID        - store id used in logging into store mode.
//              csCashierID     - cashier id used in Cash Management Screen.
// Returns:     long
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyCashManagement( CArray<int,int>& caValueList, CArray<int,int>& caChangeInCount, CString csUserID, CString csCashierID )
{
    trace(L6, _T("+CReportingBase::CurrencyCashManagement"));

    CString csAmount,csCurrencyType,csquantity,csDenomination,csID,csTime,csDen;
    long lRetCode = 0;
    csTime = GetTimeAsString();

    for( int i = 0; i < caValueList.GetSize(); i++ )
    {
      bool bBill;
      long lID, lChangeInCount;

      (caValueList[i] < 0) ? bBill = false : bBill = true;
      lChangeInCount = caChangeInCount[i];

      if( lChangeInCount != 0 )
      {
        ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_INCREMENT);
        lID = ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_GET);

        csDen.Format( _T("%d"), abs(caValueList[i]));
        trace( L6, _T("Parameters passed. bBill : %d. Denomination : %s. Change in Count : %d."),
          bBill, csDen, lChangeInCount );

        long lRet = CurrencyCashManagement( csTime, lID, bBill, csDen, lChangeInCount, 
                        csUserID, csCashierID );
        ( lRet != 0 ) ? lRetCode = lRet : lRetCode;
      }
    }
    
    //add the lock box
    if( caChangeInCount[caChangeInCount.GetSize()-1] != 0 )
    {
      ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_INCREMENT);
      long lRet = CurrencyCashManagement( csTime, ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_GET), 
                      true, ATTLOCKBOX, caChangeInCount[caChangeInCount.GetSize()-1],
                      csUserID, csCashierID );
      ( lRet != 0 ) ? lRetCode = lRet : lRetCode;
    }
    
    trace(L6, _T("-CReportingBase::CurrencyCashManagement"));
    return lRetCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyCashManagement                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, 
//                The time the cash management was accessed.
//                The ID of this entry
//                Bill or Coin
//                The denomination value
//                The Change In Count for the denomination value
//                The User ID used in logging into store mode
//                The Cashier ID used in Cash Management Screen.
// Parameters:  csTime          - The time the cash management was accessed.
//              ID              - The ID of this entry
//              bBill           - Bill or Coin
//              csDen           - The denomination value
//              caChangeInCount - he Change In Count for the denomination value
//              csUserID        - The User ID used in logging into store mode
//              csCashierID     - The Cashier ID used in Cash Management Screen.
// Returns:     long
// Example:     <CurrencyCashManagement Time="2005-03-16T08:47:23.437" ID="1" 
//                  Note="0" Denomination="1" ChangeInCount="-40" UserId="1" CashierId="1" /> 
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyCashManagement(CString csTime,long ID, bool bBill, CString csDen,long lChangeInCount, CString csUserID, CString csCashierID)
{
    trace(L6, _T("+CurrencyCashManagement. Per element"));
    long lRetCode = 0;

    CString csChangeInCount, csCurrencyType, csDenomination, csID, csDeviceType;
    m_csNameStart = CURRENCYCASHMANAGEMENT;
    m_csStart.Format(_T("<%s"), m_csNameStart);

    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID);
    csCurrencyType.Format(_T("%s=\"%s\" "),ATTCURRENCYTYPE,
      (bBill) ? ATTIN : ATTOUT );
    csDenomination.Format(_T("%s=\"%s\" "),ATTDENOMINATION, csDen);
    csChangeInCount.Format( _T("%d"), lChangeInCount );

    if( lChangeInCount > 0 )
    {
      csChangeInCount = _T("+") + csChangeInCount;
    }

    csChangeInCount.Format(_T("%s=\"%s\" "),ATTCHANGEINCOUNT, (CString)csChangeInCount);

    csUserID.Format(_T("%s=\"%s\" "), ATTUSERID, csUserID );

    csCashierID.Format(_T("%s=\"%s\"/>"), ATTCASHIERID, csCashierID );
    
    //PW500001
    if (bBill == true)
        csDeviceType.Format(_T("%s=\"%d\" "),ATTDEVICETYPE, 10 ); // TAR 454004
    else
        csDeviceType.Format(_T("%s=\"%d\" "),ATTDEVICETYPE, 11 ); // TAR 454004
    
    m_csRecord = m_csStart + csTime + csID + csDeviceType + csDenomination +  
      csChangeInCount + csUserID + csCashierID;
    lRetCode = WriteToCollector(m_csRecord);
    trace(L6, _T("-CurrencyCashManagement. Per element.<%s>"), m_csRecord);
    return lRetCode;
}
//-dp185016

long CReportingBase::SaveSignatureFileData(CFile* cfPointer , BSTR pSigData, LPCTSTR lpReceiptType)
{

	trace(L7, _T("+CReportingBase::SaveSignatureFileData"));
    long  lRetCode = RPT_INVALIDPARM;
    UINT iCount;
    BSTR bOutString;
    _bstr_t *pbstrSignatureData;
    _bstr_t *pbstrSlipData;

    if(pSigData != NULL)
	{
		iCount = :: SysStringByteLen(pSigData); 
	}
	else
	{
		iCount = 0;
	}
    CString csFormattedSlipData,csSignatureData,csID,csTime;

    if (cfPointer != NULL)
    {   
		DWORD len;
    	TCHAR  *pBuff;
   
  		len = cfPointer->SeekToEnd();
   		cfPointer->SeekToBegin();
	  
		if (len)		 
		{
			  pBuff = new TCHAR [len+1];
			  if (pBuff)
			  {
				  memset(pBuff, 0, len+1);
				  UINT c = cfPointer->Read(pBuff, len);
				  if (c)
				  {
					  pBuff[len]  = 0;
					  CString csWholePrintData(pBuff, len/sizeof(TCHAR));
					  //Covert Hex to Ascii
					  HexToAscii((BSTR)pBuff, &bOutString, len);
					  pbstrSlipData = new _bstr_t(bOutString, false);
				  }
				  delete [] pBuff;
			  }
		  }
    }
    else
    {
        pbstrSlipData = new _bstr_t( _T(""));
    }

	
    if(iCount != 0)
	{
		//Covert Hex to Ascii
		HexToAscii(pSigData, &bOutString, iCount);
		pbstrSignatureData = new _bstr_t(bOutString, false);
		csSignatureData.Format(_T("%s=\"%s\" "),ATTSIGNATUREDATA, (TCHAR *)(*pbstrSignatureData));
		delete pbstrSignatureData;
	}
	else
	{
		csSignatureData.Format(_T("%s=\"\" "),ATTSIGNATUREDATA);
	}
    m_csNameStart = SIGNATURES;
    m_csStart.Format(_T("<%s"), m_csNameStart);
    ID(RP_IDTYPE_SIGNID,RP_IDOPERATION_INCREMENT);
    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_SIGNID,RP_IDOPERATION_GET));
    csFormattedSlipData.Format(_T("%s=\"%s\" "),ATTSLIPDATA, (TCHAR *)(*pbstrSlipData));
    csTime = GetTimeAsString();
   	CString csReceiptType;
	csReceiptType.Format(_T("%s=\"%s\"/>"),ATTRECEIPTTYPE, lpReceiptType);
	m_csRecord =  m_csStart + csTime +  csID  + csFormattedSlipData + csSignatureData + csReceiptType;
    lRetCode=WriteToCollector(m_csRecord);
    delete pbstrSlipData;
    trace(L7, _T("-CReportingBase::SaveSignatureFileData<%s>"), m_csRecord);
    return lRetCode;
	
}

///////////////////////////////////////////////////////////////////////////////
// Function:    SaveReceiptData                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, Slipdata, Signaturedata,and Receipt type required for the Signature report.
// Parameters:  hFile 
//              pSigData
//				lpReceiptType
// Returns:     long
// Example:       <Signature Time="2002-12-04T19:34:50.363" ID="1" 
//                  SlipData="2A2A2A2A2A20506C65617365207369676E206F6E207468697320736C697020202A2A2A2A0D0A0D
//                  0A54727820416D6F756E743A20242424242E24240D0A0D0A0D0A0D0A5369676E61747572653A0D0A0D0A0D0A
//                  065369676E6174757265546F6B656E0D0A0D0A2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
//                  2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D0D0A0D0A5468616E6B20796F7520666F72207573696E67204E43522053
//                  434F540D0A0D0A" SignatureData="01111101803D6F00CC01400C0198C40AC2B0280B0280B0282BF9C0AFA
//                  FEEE97C1E8F80EE7F5FD3D5F5015FD3F1FC7F805BF60551D17C41118C409C0B0A80B0280FFA009FB80EFAC0A
//                  FAC0AFAFC2F93A3C8016F77FA" />                                  
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::SaveReceiptData(LPCTSTR lpFileName , BSTR pSigData, LPCTSTR lpReceiptType)
{
    trace(L7, _T("+CReportingBase::SaveReceiptData"));
	CFile *pCFile = NULL;
	long lRetCode;

	if (lpFileName != NULL)
	{
		try
		{
			pCFile = new CFile(lpFileName, CFile::modeRead);
		}
      	catch(CFileException *e)      
		{
		  // may be an invalid file handle
		  e->Delete();
		  lRetCode = RPTERROR_CANTCREATEFILE;
		  trace(L7, _T("-CReportingBase::SaveReceiptData - lRetCode:<%d>"),lRetCode);
		  return lRetCode;
		}
		lRetCode = SaveSignatureFileData(pCFile, pSigData, lpReceiptType);
		if (pCFile != NULL)
		{
			delete pCFile;
		}
	}
	trace(L7, _T("-CReportingBase::SaveReceiptData - lRetCode:<%d>"),lRetCode);
	return lRetCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    Signatures                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, Slipdata, Signaturedata required for the Signature report.
// Parameters:  hFile 
//              pSigData
// Returns:     long
// Example:       <Signature Time="2002-12-04T19:34:50.363" ID="1" 
//                  SlipData="2A2A2A2A2A20506C65617365207369676E206F6E207468697320736C697020202A2A2A2A0D0A0D
//                  0A54727820416D6F756E743A20242424242E24240D0A0D0A0D0A0D0A5369676E61747572653A0D0A0D0A0D0A
//                  065369676E6174757265546F6B656E0D0A0D0A2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D
//                  2D2D2D2D2D2D2D2D2D2D2D2D2D2D2D0D0A0D0A5468616E6B20796F7520666F72207573696E67204E43522053
//                  434F540D0A0D0A" SignatureData="01111101803D6F00CC01400C0198C40AC2B0280B0280B0282BF9C0AFA
//                  FEEE97C1E8F80EE7F5FD3D5F5015FD3F1FC7F805BF60551D17C41118C409C0B0A80B0280FFA009FB80EFAC0A
//                  FAC0AFAFC2F93A3C8016F77FA" />                                  
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::Signatures(HFILE hFile , BSTR pSigData)
{
	trace(L7, _T("+CReportingBase::Signatures"));
	CFile *pCFile = NULL;
	long lRetCode;
	try
	{
		pCFile = new CFile((HFILE)hFile);  
    }
	catch(CFileException *e)      
    {
      // may be an invalid file handle
      e->Delete();
	  lRetCode = RPTERROR_CANTCREATEFILE;
	  trace(L7, _T("-CReportingBase::Signatures - lRetCode:<%d>"),lRetCode);
	  return lRetCode;
	}

    lRetCode = SaveSignatureFileData(pCFile, pSigData, _T("SIGNATURE"));
    if (pCFile != NULL)
    {
		delete pCFile;
    }
	trace(L7, _T("-CReportingBase::Signatures - lRetCode:<%d>"),lRetCode);
	return lRetCode;
}
#endif

///////////////////////////////////////////////////////////////////////////////
// Function:    ID                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures IDtype, IDOperation.
//              IDType-consist of Tender, CurrencyAccepted, CurrencyDispensed, Items.
//              IDOperation- consists of CLEARALL, INCREMENT and GET operations
// Parameters:  IDType-consist of Tender, CurrencyAccepted, CurrencyDispensed, Items, 
//              Transaction and Signature.
//              IDOperation- consists of CLEARALL, INCREMENT and GET operations
// Returns:     long                                         
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::ID(long lIDtype, long lIDOperation)
{
   trace(L7, _T("+CReportingBase::ID"));
   long *ID;
   static long lCurrencyAcceptedID  = 0;
   static long lCurrencyDispensedID = 0;
   static long lTenderID            = 0;
   static long lItemsID             = 0;
   static long lTransID             = 0;
   static long lSignID              = 0; 
   static long lItemsSold			= 0;
   switch(lIDtype)
    {
    case RP_IDTYPE_CURRENCYA:
         ID =&lCurrencyAcceptedID ;
         break;
    case RP_IDTYPE_CURRENCYD:
         ID = &lCurrencyDispensedID;
         break;
    case RP_IDTYPE_TENDER:
         ID = &lTenderID;
         break;
    case RP_IDTYPE_ITEMS:
         ID = &lItemsID;
         break;
    case RP_IDTYPE_TRANS:
         ID = &lTransID;
         break;
    case RP_IDTYPE_SIGNID:
         ID = &lSignID;
         break;
	case RP_IDTYPE_ITEMSSOLD:
		ID = &lItemsSold;
		break;
    }
    switch(lIDOperation)
    {
    case RP_IDOPERATION_INCREMENT:
         *ID += 1;              
         break;
    case RP_IDOPERATION_CLEARALL:
         *ID = 0;
         break;
    case RP_IDOPERATION_GET:  //GET only returns  so the return takes care of this case
         break;
    }
    trace(L7, _T("-CReportingBase::ID"));
    return *ID;
}//end of ID function

#ifndef _CPPUNIT
///////////////////////////////////////////////////////////////////////////////
// Function:    HexToAscii                                        
// Class:       CReportingBase                                              
// Description: This method is called by the Signature method to convert the 
//              SignatureData from HexToAscii
// Parameters:  bBuf
//              bOutString
//              iLength                                                
// Returns:     void                                         
///////////////////////////////////////////////////////////////////////////////////
void CReportingBase::HexToAscii(BSTR bBuf, BSTR *bOutString, int iLength)
{
    trace(L7, _T("+CReportingBase::HexToAscii"));
    BYTE cIn;
    BYTE cNibble;
	TCHAR cChar;
    BYTE *pCharBuf = (BYTE *)bBuf;
    CString outString =_T("");
    _bstr_t bstrOut;

    for (int i=0; i< iLength; i++)
    {
        cIn = pCharBuf[i];
        for (int j = 0; j<2; j++)
        {
            cNibble = (cIn & 0xF0) >> 4;
            if (cNibble < 0xa)
            {
                cNibble += 0x30;
            }
            else
            {
                cNibble += 0x37;
            }
			cChar = cNibble;
            outString += cChar;
            cIn <<= 4;
        }
    }

    bstrOut = (LPCTSTR)outString;
    *bOutString = bstrOut.copy();
    trace(L7, _T("-CReportingBase::HexToAscii"));
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Function:    RPBusinessDate                                        
// Class:       CReportingBase                                              
// Description: 
//              
// Parameters:  None
//              
//                                                              
// Returns:     void                                     
///////////////////////////////////////////////////////////////////////////////////
void CReportingBase::RPBusinessDate()
{

	trace(L7, _T("+CReportingBase::RPBusinessDate"));
	CTime ctCurrentTime;
    ctCurrentTime = CTime::GetCurrentTime();
	CString csTime;
	csTime.Format(_T("%.4d-%.2d-%.2d"),
		ctCurrentTime.GetYear(),
		ctCurrentTime.GetMonth(),
		ctCurrentTime.GetDay());
    switch (co.nOperationsEOD)
    {
    case OPTION_OPERATOR_CONTROL:
            if (rp.m_csBusinessDate == EMPTY_STRING)
            {
                if (co.csBusinessDate == EMPTY_STRING)
                {
                   rp.m_csBusinessDate = csTime;
                   co.csBusinessDate = (LPCTSTR)rp.m_csBusinessDate;
                }
                else 
                {
                   rp.m_csBusinessDate = co.csBusinessDate;
                }
                tb.SetBusinessDate((LPCTSTR)rp.m_csBusinessDate);
            }

            break;
     case OPTION_TB_CONTROL:
            if (rp.m_csBusinessDate == EMPTY_STRING)
            {
                GetCurrentTBBusinessDate();
            }
            break;
     case OPTION_LANE_CONTROL:
            if (   (rp.m_csBusinessDate == EMPTY_STRING)
                || !(rp.m_csBusinessDate == csTime))
            {
                rp.m_csBusinessDate = csTime;
                tb.SetBusinessDate((LPCTSTR)rp.m_csBusinessDate);
                co.csBusinessDate = (LPCTSTR)rp.m_csBusinessDate;
            }
         break;
    }
   trace(L7, _T("-CReportingBase::RPBusinessDate"));

}
///////////////////////////////////////////////////////////////////////////////
// Function:    GetCurrentTBBusinessDate                                        
// Class:       CReportingBase                                              
// Description: This method will return current TB Business Date 
//              Time.
// Parameters:  NONE
// Returns:                                             
///////////////////////////////////////////////////////////////////////////////////
void CReportingBase::GetCurrentTBBusinessDate()
{
    trace(L7, _T("+CReportingBase::GetCurrentTBBusinessDate"));
   //Get Current Business Date From TB
  	CTime ctCurrentTime;
    ctCurrentTime = CTime::GetCurrentTime();
	CString csTime;
	csTime.Format(_T("%.4d-%.2d-%.2d"),
		    ctCurrentTime.GetYear(),
		    ctCurrentTime.GetMonth(),
		    ctCurrentTime.GetDay());
  long tbReturnCode = tb.GetCurrentBusinessDate(rp.m_csBusinessDate);
  if (tbReturnCode != TB_SUCCESS)
    {
      rp.m_csBusinessDate = csTime;
	  trace(L1, _T("Error TBGetCurrentBusiness not implemented, please be advised!"));
	}
 
	trace(L7, _T("-rp.m_csBusinessDate<%s>"),rp.m_csBusinessDate);
    //Write Reporting EOD BusinessDate to the registry 
    HKEY hKey = NULL;
	long lResult = 0L;
    trace(L7, _T("Attempting to Put ReportEOD in registry"));
    lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\SCOT\\CurrentVersion\\SCOTAPP\\Reporting\\ReportEOD"),
				 0, KEY_SET_VALUE, &hKey);
	if (lResult != ERROR_SUCCESS)
    {
	    trace(L7, _T("Error opening registry key to store ReportEOD"));
	}	 
    lResult = ::RegSetValueEx(hKey,
                _T("CurrentDate"), 


                0, 
                REG_SZ,
                (LPBYTE)(LPCTSTR)rp.m_csBusinessDate,
                rp.m_csBusinessDate.GetLength() * sizeof(TCHAR)); // TAR 417750
	 if (lResult != ERROR_SUCCESS)
	    {
		trace(L7, _T("Error writing ReportEOD to registry key"));  
	    }
	::RegCloseKey(hKey);
    trace(L7, _T("-CReportingBase::GetCurrentTBBusinessDate"));

 }
void CReportingBase::SetOperator(const CString csId) 
{
    if (!csId.GetLength())
    {
        int i = 0;
    }
    m_csOperatorNumber = csId;
    m_csTenderOperator = csId;
	trace(L7, _T("CReporting::SetOperator, set Operator to %s"), m_csOperatorNumber ); //429111
}

void CReportingBase::ClearOperator() 
{
	if (!m_bInterventionEndInProgress) // TAR 311902
	{
		m_csOperatorNumber = EMPTY_STRING;
		trace(L7, _T("CReporting::ClearOperator, set Operator to %s EMPTY_STRING"), EMPTY_STRING ); //429111
	}
	else
	{
		trace(L7, _T("CReporting::ClearOperator, operator was not cleared")); //429111
	}
}

void CReportingBase::SetInterventionEndInProgress(bool bValue)
{
  trace(L7, _T("m_bInterventionEndInProgress is set to %d"), m_bInterventionEndInProgress );  //429111
  m_bInterventionEndInProgress = bValue;  // TAR 311902
}


///////////////////////////////////////////////////////////////////////////////
// Function:    RAPDispenseChange                                       
// Class:       CReportingBase                                              
// Description: is a public method that captures, the Change Due RAP should dispense
// Parameters:  lDispensedAmount-Amount being dispensed                                                
// Returns:     long
// Example:     <RAPDispenseChange Time="2012-10-04T18:51:27.894" ID="1"  Amount="$0.05" TenderType="Cash" Operator="1"/>                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::RAPDispenseChange(long lDispensedAmount )
{
    trace(L7, _T("+CReportingBase::RAPDispenseChange"));
    CString csAmount, csOperator, csID, csTime, csTenderType;
    long lRetCode = 0;
    csTime = GetTimeAsString();
    m_csNameStart = RAPDISPENSECHANGE;
    m_csStart.Format(_T("<%s "), m_csNameStart);
    csID.Format(_T("%s=\"%d\" "),ATTIDID,1);
    csAmount.Format(_T("%s=\"-%s\" "),ATTAMOUNT, ps.FmtDollar(lDispensedAmount,(enum PSProceduresBase::SCREEN_POSITION) 0, 
      ps.REPORTINGFORMAT));

	csTenderType.Format(_T("%s=\"%s\" "),ATTTENDERTYPE, ps.GetPSText(MSG_TB_TENDER_CASH_RAP));
	csOperator.Format(_T("%s=\"%s\"/>"),ATTOPERATOR,m_csTenderOperator);

    m_csRecord = m_csStart + csTime + csID + csAmount + csTenderType + csOperator;
    lRetCode=WriteToCollector(m_csRecord);
    trace(L7, _T("-CReportingBase::RAPDispenseChange<%s>"), m_csRecord);
    return lRetCode;
}


///////////////////////////////////////////////////////////////////////////////
// Function:    LogDeviceError                                       
// Class:       CReportingBase                                              
// Description: is a public method that logs device errors
// Parameters:  Device error name, and device error type                                                
// Returns:     void
// Example:     "<DeviceError Time="2012-10-04T18:51:27.894" Station="1" StoreTransactionID="1"  Device="Cash Acceptor" ErrorType="Cass Out"/>                                          
///////////////////////////////////////////////////////////////////////////////////
long CReportingBase::LogDeviceError (CString csDeviceErrorName, CString csDeviceErrorType)
{
    trace(L6, _T("+CReportingBase::LogDeviceError"));
    CString csTime, csStation, csTransID, csDevice, csErrorType;
    long lRetCode = 0;
	csTime = GetTimeAsString();
    m_csNameStart = LOGDEVICEERROR;
    m_csStart.Format(_T("<%s "), m_csNameStart);

    csStation.Format(_T("%s=\"%s\" "),ATTSTATION,m_csTerminalNumber);
    csTransID.Format(_T("%s=\"%s\" "), ATTSTOREID,m_csTransactionNumber);
	csDevice.Format(_T("%s=\"%s\" "),ATTDEVICE,csDeviceErrorName);
	csErrorType.Format(_T("%s=\"%s\"/>"),ATTERRORTYPE,csDeviceErrorType);

    m_csRecord = m_csStart + csTime + csStation + csTransID + csDevice + csErrorType ;
    lRetCode=WriteToCollector(m_csRecord);
    trace(L6, _T("-CReportingBase::LogDeviceError<%s>"), m_csRecord);
    return lRetCode;
}

///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyCashManagement                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, 
//                The denomination value list
//                The change in count for each denomination
//                The balance count for each denomination
//                The User ID used in logging into store mode
//                The Cashier ID used in Cash Management Screen.
// Parameters:  cash lists for change count, balance count and denomination 
//              csUserID        - store id used in logging into store mode.
//              csCashierID     - cashier id used in Cash Management Screen.
// Returns:     long
//PW500001/////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyCashManagement( CArray<CString,CString>& caCashAcceptorDenList, 
        CArray<int,int>& caCashAcceptorCountBalanceList,
        CArray<int,int>& caCashAcceptorCountChangeList,
        CArray<CString,CString>& caCashDispenserDenList, 
        CArray<int,int>& caCashDispenserCountBalanceList,
        CArray<int,int>& caCashDispenserCountChangeList,
        CArray<CString,CString>& caCoinAcceptorDenList, 
        CArray<int,int>& caCoinAcceptorCountBalanceList,
        CArray<int,int>& caCoinAcceptorCountChangeList,
        CArray<CString,CString>& caCoinDispenserDenList, 
        CArray<int,int>& caCoinDispenserCountBalanceList,
        CArray<int,int>& caCoinDispenserCountChangeList,
        CString csUserID, CString csCashierID )
{
    //For nDeviceType 0 records, we display them at "BILL ACCEPTOR" section. 
    //For nDeviceType 1 records, we display them at "BILL DISPENSER" section. 
    //For nDeviceType 2 records, we display them at "COIN ACCEPTOR" section. 
    //For nDeviceType 3 records, we display them at "COIN DISPENSER" section. 
    //For nDeviceType 10 records, we display them at "BILL" section for BILL RECYCLER. //454004
    //For nDeviceType 11 records, we display them at "COIN" section for COIN RECYCLER.

	int nDeviceType = 0;
	long lRetVal = 0;
	
    //CurrencyCashManagement( nDeviceType, caDenomList, caChangeInCount, caBalanceCount, csUserID, csCashierID );

	nDeviceType = 0;

	lRetVal = CurrencyCashManagement( nDeviceType, caCashAcceptorDenList, caCashAcceptorCountChangeList, caCashAcceptorCountBalanceList, csUserID, csCashierID );

	nDeviceType = 1;

	lRetVal = CurrencyCashManagement( nDeviceType, caCashDispenserDenList, caCashDispenserCountChangeList, caCashDispenserCountBalanceList, csUserID, csCashierID );
	
	nDeviceType = 2;

	lRetVal = CurrencyCashManagement( nDeviceType, caCoinAcceptorDenList, caCoinAcceptorCountChangeList, caCoinAcceptorCountBalanceList, csUserID, csCashierID );

	nDeviceType = 3;

	lRetVal = CurrencyCashManagement( nDeviceType, caCoinDispenserDenList, caCoinDispenserCountChangeList, caCoinDispenserCountBalanceList, csUserID, csCashierID );

	return 1;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyCashManagement                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, 
//                The denomination value list
//                The change in count for each denomination
//                The balance count for each denomination
//                The User ID used in logging into store mode
//                The Cashier ID used in Cash Management Screen.
// Parameters:  device type for counts to be appropriated to
//              cash list for change count, balance count and denomination 
//              csUserID        - store id used in logging into store mode.
//              csCashierID     - cashier id used in Cash Management Screen.
// Returns:     long
//PW500001/////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyCashManagement( int nDeviceType, CArray<CString,CString>& caDenomList, CArray<int,int>& caChangeInCount, CArray<int,int>& caBalanceCount, CString csUserID, CString csCashierID )
{
	trace(L6, _T("+CReportingBase::CurrencyCashManagement"));

	CString csAmount,csCurrencyType,csquantity,csDenomination,csID,csTime,csDen = "";

	long lRetCode = 0;

	int iTermNumber = 0;

	csTime = GetTimeAsString();

	iTermNumber = _ttoi(m_csTerminalNumber);

    for( int i = 0; i < caDenomList.GetSize(); i++ )
    {
		long lID, lChangeInCount, lBalanceInCount = 0;

		lChangeInCount = caChangeInCount[i];
		lBalanceInCount = caBalanceCount[i];

		if( lChangeInCount != 0 )
		{
			ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_INCREMENT);
			lID = ID(RP_IDTYPE_CURRENCYD,RP_IDOPERATION_GET);

			csDen.Format( _T("%s"), caDenomList[i]);

			lRetCode = CurrencyCashManagement( csTime, lID, nDeviceType, csDen, lBalanceInCount, lChangeInCount, csUserID, csCashierID, iTermNumber );
		}
	}

    trace(L6, _T("-CReportingBase::CurrencyCashManagement"));

    return lRetCode;
}
///////////////////////////////////////////////////////////////////////////////
// Function:    CurrencyCashManagement                                        
// Class:       CReportingBase                                              
// Description: is a public method that captures, 
//                The time the cash management was accessed.
//                The ID of this entry
//                Device
//                The denomination value
//                The Change In Count for the denomination value
//                The Balance Count for the denomination value
//                The User ID used in logging into store mode
//                The Cashier ID used in Cash Management Screen.
// Parameters:  csTime          - The time the cash management was accessed.
//              ID              - The ID of this entry
//              nDevice         - Cash or coin device
//              csDen           - The denomination value
//              caChangeInCount - he Change In Count for the denomination value
//              caBalanceInCount- he Balance Count for the denomination value
//              csUserID        - The User ID used in logging into store mode
//              csCashierID     - The Cashier ID used in Cash Management Screen.
// Returns:     long
//PW500001/////////////////////////////////////////////////////////////////////////////////
long CReportingBase::CurrencyCashManagement(CString csTime, long ID, int nDeviceType, CString csDen, long lBalanceInCount, long lChangeInCount, CString csUserID, CString csCashierID, int nTerminalNumber )
{
    trace(L6, _T("+CurrencyCashManagement. Per element"));
    long lRetCode = 0;

    CString csChangeInCount, csDeviceType, csDenomination, csID, csBalanceInCount, csTerminalNumber = _T("");

    m_csNameStart = CURRENCYCASHMANAGEMENT;
    m_csStart.Format(_T("<%s"), m_csNameStart);

    csTerminalNumber.Format( _T("%s=\"%d\" "), ATTSTATION, nTerminalNumber );

    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID);
    csDeviceType.Format(_T("%s=\"%d\" "),ATTDEVICETYPE, nDeviceType );
    csDenomination.Format(_T("%s=\"%s\" "),ATTDENOMINATION, csDen);
    csChangeInCount.Format( _T("%d"), lChangeInCount );
    csBalanceInCount.Format( _T("%d"), lBalanceInCount );

    if( lChangeInCount > 0 )
		csChangeInCount = _T("+") + csChangeInCount;

    csChangeInCount.Format(_T("%s=\"%s\" "),ATTCHANGEINCOUNT, (CString)csChangeInCount);

    if( lBalanceInCount > 0 )
		csBalanceInCount = _T("+") + csBalanceInCount;

    csBalanceInCount.Format(_T("%s=\"%s\" "),ATTBALANCEINCOUNT, (CString)csBalanceInCount);

    csUserID.Format(_T("%s=\"%s\" "), ATTUSERID, csUserID );

    csCashierID.Format(_T("%s=\"%s\"/>"), ATTCASHIERID, csCashierID );
    
//    m_csRecord = m_csStart + csTime + csID + csCurrencyType + csDenomination +  
//      csChangeInCount + csUserID + csCashierID;

    m_csRecord = m_csStart + csTime + csID + csDeviceType + csDenomination +  
      csChangeInCount + csBalanceInCount + csUserID + csCashierID;

    lRetCode=WriteToCollector(m_csRecord);
 
	trace(L6, _T("-CurrencyCashManagement. Per element.<%s>"), m_csRecord);

    return lRetCode;
}


long CReportingBase::LogItemDetails()
{
    trace(L6, _T("+CReportingBase::LogItemDetails"));
    CString csTime, csID,csCode,csDescription,csDepartment,csExtPrice,
		csUnitPrice,csPriceRqd,csQtySold,csDealQty,csQtyRqd,csQtyConfirmed,csQtyLimitExceeded,
		csWgtRqd,csWgtSold,csCoupon,csTareRqd,csNotFound,csNotForSale,csVisVerify,csRestricted, 
		csRestrictedAge,csVoid,csLinked,csCrateAnswer,csSecurityBaggingRqd,csSecuritySubChkRqd,csSecurityTag,
		csZeroWeight,csPickList;
	
    long lRetCode = 0;
	//+TAR410994
	if (!m_bTransactionNumberSet)
    {
		 m_bTransactionNumberSet = true;
         int nTransNumber = TBGetCurrentTransactionNumber();
         trace(L7, _T("TransactionNumber set to %d"), nTransNumber);
         m_csTransactionNumber .Format(_T("%d"), nTransNumber);
    }
	//-TAR410994
	csTime = GetTimeAsString();
    m_csNameStart = LOGITEMDETAILS;
    m_csStart.Format(_T("<%s "), m_csNameStart);
	
	CString csFormattedDesc =  EscapeText(io.d.csDescription); //TAR440565 
	ID(RP_IDTYPE_ITEMSSOLD,RP_IDOPERATION_INCREMENT);
    csID.Format(_T("%s=\"%d\" "),ATTIDID,ID(RP_IDTYPE_ITEMSSOLD,RP_IDOPERATION_GET));
	
	csCode.Format(_T("%s=\"%s\" "),ATTCODE,io.d.csItemCode);
	csDescription.Format(_T("%s=\"%s\" "),ATTDESCRIPTION,csFormattedDesc);
	csDepartment.Format(_T("%s=\"%d\" "),ATTDEPARTMENT,io.d.lDepartment);
	csExtPrice.Format(_T("%s=\"%d\" "),ATTEXTPRICE,io.d.lExtPrice);
	csUnitPrice.Format(_T("%s=\"%d\" "),ATTUNITPRICE,io.d.lUnitPrice);
	csPriceRqd.Format(_T("%s=\"%s\" "),ATTPRICERQD,io.d.fPriceRequired ? _T("1") : _T("0"));
	csQtySold.Format(_T("%s=\"%d\" "),ATTQTYSOLD, io.d.lQtySold);
	csDealQty.Format(_T("%s=\"%d\" "),ATTDEALQTY,io.d.lDealQty);
	csQtyRqd.Format(_T("%s=\"%s\" "),ATTQTYRQD, io.d.fQuantityRequired ? _T("1") : _T("0"));
	csQtyConfirmed.Format(_T("%s=\"%s\" "),ATTQTYCONFIRMED, io.d.fQuantityConfirmed ? _T("1") : _T("0"));
	csQtyLimitExceeded.Format(_T("%s=\"%s\" "),ATTQTYLIMITEXCEEDED, io.d.fQuantityLimitExceeded ? _T("1") : _T("0"));
	csWgtRqd.Format(_T("%s=\"%s\" "),ATTWGTRQD, io.d.fWeightRequired ? _T("1") : _T("0"));
	csWgtSold.Format(_T("%s=\"%d\" "),ATTWGTSOLD, io.d.lWgtSold);
	csCoupon.Format(_T("%s=\"%s\" "),ATTCOUPON, io.d.fCoupon ? _T("1") : _T("0"));
	csTareRqd.Format(_T("%s=\"%s\" "),ATTTARERQD, io.d.fTareRequired ? _T("1") : _T("0"));
	csNotFound.Format(_T("%s=\"%s\" "),ATTNOTFOUND, io.d.fNotFound ? _T("1") : _T("0"));
	csNotForSale.Format(_T("%s=\"%s\" "),ATTNOTFORSALE, io.d.fNotForSale ? _T("1") : _T("0"));
	csVisVerify.Format(_T("%s=\"%s\" "),ATTVISVERIFY, io.d.fVisualVerify ? _T("1") : _T("0"));
	csRestricted.Format(_T("%s=\"%s\" "),ATTRESTRICTED, io.d.fRestricted ? _T("1") : _T("0"));
	csRestrictedAge.Format(_T("%s=\"%d\" "),ATTRESTRICTEDAGE, io.d.nRestrictedAge);
	csVoid.Format(_T("%s=\"%s\" "),ATTVOID, io.d.fVoid ? _T("1") : _T("0"));
	csLinked.Format(_T("%s=\"%s\" "),ATTLINKED, io.d.bLinkedItem ? _T("1") : _T("0"));
	csCrateAnswer.Format(_T("%s=\"%d\" "),ATTCRATEANSWER, io.d.nCrateableScreenAnswer);
	csSecurityBaggingRqd.Format(_T("%s=\"%d\" "),ATTSECURITYBAGGINGRQD, io.d.lRequireSecBagging);
	csSecuritySubChkRqd.Format(_T("%s=\"%d\" "),ATTSECURITYSUBCHKRQD,io.d.lRequireSecSubstChk);
	csSecurityTag.Format(_T("%s=\"%s\" "),ATTSECURITYTAG, io.d.bSecurityTag ? _T("1") : _T("0"));
	csZeroWeight.Format(_T("%s=\"%s\" "),ATTZEROWEIGHT, io.d.bZeroWeightItem? _T("1") : _T("0"));
	csPickList.Format(_T("%s=\"%s\"/>"),ATTPICKLIST, io.d.bPickListItem ? _T("1") : _T("0"));
	
	  
    m_csRecord = m_csStart + csTime + csID + csCode + csDescription + csDepartment + csExtPrice +
		csUnitPrice + csPriceRqd + csQtySold + csDealQty + csQtyRqd + csQtyConfirmed + csQtyLimitExceeded +
		csWgtRqd + csWgtSold + csCoupon + csTareRqd + csNotFound + csNotForSale + csVisVerify + csRestricted  + 
		csRestrictedAge + csVoid + csLinked + csCrateAnswer + csSecurityBaggingRqd + csSecuritySubChkRqd + csSecurityTag +
		csZeroWeight + csPickList;
	
    lRetCode=WriteToCollector(m_csRecord);
    trace(L6, _T("-CReportingBase::LogItemDetails %s"), m_csRecord);
    return lRetCode;
}



void CReportingBase::SetLanguageName(CString& csLanguage)
{
	m_csLanguage = csLanguage;
	return;
}

//+TAR 343053
bool CReportingBase::IsTransactionInProgress(void)
{
	return m_bTransactionInProgress;
}

//TAR429111
bool CReportingBase::IsInterventionEndInProgress(void)
{
	return m_bInterventionEndInProgress;
}

CString CReportingBase::EscapeText(const CString &text)
{
    CString retValue;
    _TCHAR c;
    for (unsigned i=0; i<text.GetLength(); i++)
    {
        c = text.GetAt(i);
        switch(c)
        {
            case '\'' :
                retValue += _T("&") XML_SQUOTE _T(";");
                break;
            case '>' :
                retValue += _T("&") XML_GT _T(";");
                break;
            case '<' :
                retValue += _T("&") XML_LT _T(";");
                break;
            case '&' :
                retValue += _T("&") XML_AMP _T(";");
                break;
            case '"' :									//TAR440565
                retValue += _T("&") XML_QUOT _T(";");
                break;
            default :
                retValue += c;
                break;
        }
    }

    return retValue;
}
#endif //_CPPUNIT

//-TAR 343053
///////////////////////////////////////////////////////////////////////////////////
//End of Reporting 
