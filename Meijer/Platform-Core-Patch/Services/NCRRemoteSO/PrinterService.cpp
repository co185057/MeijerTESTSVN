// PrinterService.cpp: implementation of the CPrinterService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NCRRemoteSO.h"
#include "PrinterService.h"
#include "PrinterDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long IsValidSymbology(
   long lSymbology )
   {
   long lResult = OPOS_E_ILLEGAL;
   if ( lSymbology >= PTR_BCS_UPCA
      && lSymbology <= PTR_BCS_OCRB )
      lResult = OPOS_SUCCESS;
   else if ( lSymbology == PTR_BCS_PDF417  
      || lSymbology == PTR_BCS_MAXICODE      
      || lSymbology == PTR_BCS_OTHER )
      lResult = OPOS_SUCCESS;
   return lResult;
   }

long IsValidStation( 
   long  lStation,
   BOOL  bAllowJournal = TRUE )
   {
   long lResult = OPOS_E_ILLEGAL;
   if ( lStation == PTR_S_RECEIPT 
      || lStation == PTR_S_SLIP )
      lResult = OPOS_SUCCESS;
   else if ( lStation == PTR_S_JOURNAL
      && bAllowJournal != FALSE )
      lResult = OPOS_SUCCESS;
   return lResult;
   }




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrinterService::CPrinterService(
   void )
   : COposService( _T("PtrSO") )
   {

   }

/* virtual */
void CPrinterService::InitializeProperties(
   void )
   {
   COposService::InitializeProperties();
   return;
   }


long CPrinterService::BeginInsertion(
   long lTimeout )
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->BeginInsertion( lTimeout, &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::BeginRemoval(
   long lTimeout )
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->BeginRemoval( lTimeout, &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::CutPaper(
   long lPercentage )
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS 
      && (DWORD)lPercentage > 100 )
      lResult = OPOS_E_ILLEGAL;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->CutPaper( lPercentage, &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::EndInsertion(
   void)
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->EndInsertion( &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::EndRemoval(
   void)
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->EndRemoval( &nRcEx );
   return SetRC( lResult, nRcEx );
   }



long CPrinterService::PrintBarCode(
   long     lStation, 
   LPCTSTR  sData, 
   long     lSymbology, 
   long     lHeight, 
   long     lWidth, 
   long     lAlignment, 
   long     lTextPosition ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation, FALSE );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidSymbology( lSymbology );
   if ( lResult == OPOS_SUCCESS )
      {
      if ( lAlignment != PTR_BC_LEFT
         && lAlignment != PTR_BC_CENTER
         && lAlignment != PTR_BC_RIGHT	)
         lResult = OPOS_E_ILLEGAL;
      else if ( lTextPosition != PTR_BC_TEXT_NONE
         && lTextPosition != PTR_BC_TEXT_ABOVE
         && lTextPosition != PTR_BC_TEXT_BELOW )
         lResult = OPOS_E_ILLEGAL;
      // Empty barcode?
      else if ( *sData == 0 
         || lHeight <= 0 
         || lWidth <= 0 )
         lResult = OPOS_E_ILLEGAL;
      }
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->PrintBarCode( lStation, ConvertAsciiToBinary( sData ), 
         lSymbology, lHeight, lWidth, lAlignment, lTextPosition, &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::PrintBitmap(
   long     lStation, 
   LPCTSTR  sFileName, 
   long     lWidth, 
   long     lAlignment ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation, FALSE );
   if ( lResult == OPOS_SUCCESS )
      {
      if ( lWidth <= 0 
         && lWidth != PTR_BM_ASIS )
         lResult = OPOS_E_ILLEGAL;
      else if ( lAlignment != PTR_BM_LEFT	
         && lAlignment != PTR_BM_CENTER
         && lAlignment != PTR_BM_RIGHT	)
         lResult = OPOS_E_ILLEGAL;
      else if ( *sFileName == 0 )
         lResult = OPOS_E_ILLEGAL;
      }
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->PrintBitmap( lStation, sFileName, lWidth, lAlignment, &nRcEx );
   return SetRC( lResult, nRcEx );
   }


long CPrinterService::PrintImmediate(
   long     lStation, 
   LPCTSTR  sData) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation );
   if ( lResult == OPOS_SUCCESS 
      && *sData == 0 )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->PrintImmediate( lStation, 
         ConvertAsciiToBinary( sData ), &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::PrintNormal(
   long				lStation, 
   const CString&	sData ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation );
   if ( lResult == OPOS_SUCCESS 
      && *sData == 0 )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->PrintNormal( lStation, 
         ConvertAsciiToBinary( sData ), &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::PrintTwoNormal(
   long     lStations, 
   LPCTSTR  sData1, 
   LPCTSTR  sData2) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS 
      && lStations != PTR_S_JOURNAL_RECEIPT
      && lStations != PTR_S_JOURNAL_SLIP   
      && lStations != PTR_S_RECEIPT_SLIP )
      lResult = OPOS_E_ILLEGAL;
   if ( lResult == OPOS_SUCCESS )
      {
      if ( *sData1 == 0 
         || *sData2 == 0 )
         lResult = OPOS_E_ILLEGAL;
      }
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->PrintTwoNormal( lStations, 
         ConvertAsciiToBinary( sData1 ), 
         ConvertAsciiToBinary( sData2 ), &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::RotatePrint(
   long  lStation, 
   long  lRotation ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation, FALSE );
   if ( lResult == OPOS_SUCCESS 
      && lRotation != PTR_RP_RIGHT90
      && lRotation != PTR_RP_LEFT90
      && lRotation != PTR_RP_ROTATE180
      && lRotation != PTR_RP_NORMAL )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->RotatePrint( lStation, lRotation, &nRcEx );
   return SetRC( lResult, nRcEx );
   }
    
                                 
long CPrinterService::SetBitmap(
   long     lBitmapNumber, 
   long     lStation, 
   LPCTSTR  sFileName, 
   long     lWidth, 
   long     lAlignment ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation, FALSE );
    
   // Remove validation at this level, it will be 
   // performed at the defice driver level. 
   /* 
   if ( lResult == OPOS_SUCCESS )
      {
      if ( lBitmapNumber != 1
         && lBitmapNumber != 2 )
         lResult = OPOS_E_ILLEGAL;
      else if ( lWidth <= 0 
         && lWidth != PTR_BM_ASIS )
         lResult = OPOS_E_ILLEGAL;
      else if ( lAlignment != PTR_BM_LEFT	
         && lAlignment != PTR_BM_CENTER
         && lAlignment != PTR_BM_RIGHT	)
         lResult = OPOS_E_ILLEGAL;
      else if ( *sFileName == 0 )
         lResult = OPOS_E_ILLEGAL;
      }
   */
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->SetBitmap( lBitmapNumber, lStation, sFileName, lWidth, lAlignment, &nRcEx );
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::SetLogo(
   long     lLocation, 
   LPCTSTR  sData ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS 
      && lLocation != PTR_L_TOP
      && lLocation != PTR_L_BOTTOM )
      lResult = OPOS_E_ILLEGAL;
   else if ( *sData == 0 )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->SetLogo( lLocation, ConvertAsciiToBinary( sData ), &nRcEx );
   return SetRC( lResult, nRcEx );
   } 

long CPrinterService::TransactionPrint(
   long  lStation, 
   long  lControl ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation );
   if ( lResult == OPOS_SUCCESS 
      && lControl != PTR_TP_TRANSACTION
      && lControl != PTR_TP_NORMAL )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = OPOS_E_NOHARDWARE;     // We just don't support this
   return SetRC( lResult, nRcEx );
   }

long CPrinterService::ValidateData(
   long     lStation, 
   LPCTSTR  sData ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   if ( lResult == OPOS_SUCCESS )
      lResult = IsValidStation( lStation );
   if ( lResult == OPOS_SUCCESS
      && *sData == 0 )
      lResult = OPOS_E_ILLEGAL;
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->ValidateData( lStation, ConvertAsciiToBinary( sData ), &nRcEx );
   return SetRC( lResult, nRcEx );
   }


// CPrinterService::GetPropertyNumber
//
// Purpose: Called by TService.

long CPrinterService::GetPropertyNumber( 
   long lPidx )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("CPrinterService::GetPropertyNumber") );
   long lResult = 0;
   switch( lPidx )
      {     // these are saved at the device
      case PIDX_ResultCode:           
      case PIDX_Claimed:              
      case PIDX_DataEventEnabled:     
      case PIDX_DeviceEnabled:        
      case PIDX_DataCount:
      case PIDX_BinaryConversion:
      case PIDX_AutoDisable:
      case PIDX_FreezeEvents:         
      case PIDX_ResultCodeExtended:   
//      case PIDX_OutputID:
      case PIDX_ServiceObjectVersion:
         lResult = COposService::GetPropertyNumber( lPidx );
         break;
      default:
         lResult = m_pDevice->GetPropertyNumber( lPidx );
         break;
      }
   return lResult;
   }


// CPrinterService::GetPropertyString
//
// Purpose: Get common string property by OPOS index ( PIDX ).

CString CPrinterService::GetPropertyString( 
   long lPidx ) 
   {
   CString sResult;
   if ( CheckMethodValid( ) == OPOS_SUCCESS )
      {
      switch( lPidx )
         {
//         case PIDX_DeviceDescription:        
//         case PIDX_DeviceName:               
//         case PIDX_ServiceObjectDescription:
         case PIDXPtr_CharacterSetList:
         case PIDXPtr_JrnLineCharsList:
         case PIDXPtr_RecLineCharsList:
         case PIDXPtr_SlpLineCharsList:
         case PIDXPtr_ErrorString:
         case PIDXPtr_FontTypefaceList:
         case PIDXPtr_RecBarCodeRotationList:
         case PIDXPtr_SlpBarCodeRotationList:
            sResult = m_pDevice->GetPropertyString( lPidx );
            break;
         default:
            sResult = COposService::GetPropertyString( lPidx );
            break;
         }
      }
   m_cmDc.DCPrintf( TRACE_OUTPUT_LEVEL1, 
      _T("PropertyIndex %d, Value <%s>"), lPidx, (LPCTSTR)sResult );
   return sResult;
   }

// CPrinterService::SetPropertyNumber
//
// Purpose: Called by TService.

void CPrinterService::SetPropertyNumber( 
   long lPidx,
   long lValue )
   {
   CDataCapEntryExit DCE( m_cmDc, _T("CPrinterService::SetPropertyNumber") );
   long lResult = CheckMethodValid( );
   if ( lResult == OPOS_SUCCESS )
      {
      switch( lPidx )
         {
         case PIDX_DataEventEnabled:
         case PIDX_BinaryConversion:
         case PIDX_AutoDisable:
         case PIDX_FreezeEvents:
            COposService::SetPropertyNumber( lPidx, lValue );
            break;
         default:
            m_pDevice->SetPropertyNumber(lPidx, lValue );
            break;
         }
      }
   return;
   }


// Called by CPOSPrinterSO
#if 0
long CPrinterService::ClearOutput(
   void ) 
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_CLAIMED );
   if ( lResult == OPOS_SUCCESS )
      m_pDevice->ClearOutput( );
   return SetRC( lResult );
   }
#endif

long CPrinterService::CheckHealth(
   long lLevel )
   {
   long lResult = CheckMethodValid( OPOS_REQUIRE_ENABLED );
   long nRcEx = 0;
   if ( lResult == OPOS_SUCCESS )
      lResult = m_pDevice->CheckHealth( lLevel, &nRcEx );
   return SetRC( lResult, nRcEx );
   }
