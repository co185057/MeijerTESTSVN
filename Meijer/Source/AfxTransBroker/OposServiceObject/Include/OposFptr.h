/////////////////////////////////////////////////////////////////////
//
// OposFptr.h
//
//   Fiscal Printer header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 98-03-06 OPOS Release 1.3                                     PDU
//
/////////////////////////////////////////////////////////////////////

#if !defined(OPOSFPTR_H)
#define      OPOSFPTR_H


#include "Opos.h"


/////////////////////////////////////////////////////////////////////
// Fiscal Printer Station Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_S_JOURNAL                   = 1;
const LONG FPTR_S_RECEIPT                   = 2;
const LONG FPTR_S_SLIP                      = 4;

const LONG FPTR_S_JOURNAL_RECEIPT = FPTR_S_JOURNAL | FPTR_S_RECEIPT;


/////////////////////////////////////////////////////////////////////
// "CountryCode" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_CC_BRAZIL                   =  1;
const LONG FPTR_CC_GREECE                   =  2;
const LONG FPTR_CC_HUNGARY                  =  3;
const LONG FPTR_CC_ITALY                    =  4;
const LONG FPTR_CC_POLAND                   =  5;
const LONG FPTR_CC_TURKEY                   =  6;


/////////////////////////////////////////////////////////////////////
// "ErrorLevel" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_EL_NONE                     =  1;
const LONG FPTR_EL_RECOVERABLE              =  2;
const LONG FPTR_EL_FATAL                    =  3;
const LONG FPTR_EL_BLOCKED                  =  4;


/////////////////////////////////////////////////////////////////////
// "ErrorState", "PrinterState" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_PS_MONITOR                  =  1;
const LONG FPTR_PS_FISCAL_RECEIPT           =  2;
const LONG FPTR_PS_FISCAL_RECEIPT_TOTAL     =  3;
const LONG FPTR_PS_FISCAL_RECEIPT_ENDING    =  4;
const LONG FPTR_PS_FISCAL_DOCUMENT          =  5;
const LONG FPTR_PS_FIXED_OUTPUT             =  6;
const LONG FPTR_PS_ITEM_LIST                =  7;
const LONG FPTR_PS_LOCKED                   =  8;
const LONG FPTR_PS_NONFISCAL                =  9;
const LONG FPTR_PS_REPORT                   = 10;


/////////////////////////////////////////////////////////////////////
// "SlipSelection" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_SS_FULL_LENGTH              =  1;
const LONG FPTR_SS_VALIDATION               =  2;


/////////////////////////////////////////////////////////////////////
// "GetData" Method Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_GD_CURRENT_TOTAL            =  1;
const LONG FPTR_GD_DAILY_TOTAL              =  2;
const LONG FPTR_GD_RECEIPT_NUMBER           =  3;
const LONG FPTR_GD_REFUND                   =  4;
const LONG FPTR_GD_NOT_PAID                 =  5;
const LONG FPTR_GD_MID_VOID                 =  6;
const LONG FPTR_GD_Z_REPORT                 =  7;
const LONG FPTR_GD_GRAND_TOTAL              =  8;
const LONG FPTR_GD_PRINTER_ID               =  9;
const LONG FPTR_GD_FIRMWARE                 = 10;
const LONG FPTR_GD_RESTART                  = 11;


/////////////////////////////////////////////////////////////////////
// "AdjustmentType" arguments in diverse methods
/////////////////////////////////////////////////////////////////////

 const LONG FPTR_AT_AMOUNT_DISCOUNT          =  1;
const LONG FPTR_AT_AMOUNT_SURCHARGE         =  2;
const LONG FPTR_AT_PERCENTAGE_DISCOUNT      =  3;
const LONG FPTR_AT_PERCENTAGE_SURCHARGE     =  4;


/////////////////////////////////////////////////////////////////////
// "ReportType" argument in "PrintReport" method
/////////////////////////////////////////////////////////////////////

const LONG FPTR_RT_ORDINAL                  =  1;
const LONG FPTR_RT_DATE                     =  2;


/////////////////////////////////////////////////////////////////////
// "StatusUpdateEvent" Event: "Data" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG FPTR_SUE_COVER_OPEN              =  11;
const LONG FPTR_SUE_COVER_OK                =  12;

const LONG FPTR_SUE_JRN_EMPTY               =  21;
const LONG FPTR_SUE_JRN_NEAREMPTY           =  22;
const LONG FPTR_SUE_JRN_PAPEROK             =  23;

const LONG FPTR_SUE_REC_EMPTY               =  24;
const LONG FPTR_SUE_REC_NEAREMPTY           =  25;
const LONG FPTR_SUE_REC_PAPEROK             =  26;

const LONG FPTR_SUE_SLP_EMPTY               =  27;
const LONG FPTR_SUE_SLP_NEAREMPTY           =  28;
const LONG FPTR_SUE_SLP_PAPEROK             =  29;

const LONG FPTR_SUE_IDLE                    =1001;


/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Fiscal Printer
/////////////////////////////////////////////////////////////////////

const LONG OPOS_EFPTR_COVER_OPEN = 1 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_JRN_EMPTY  = 2 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_REC_EMPTY  = 3 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_SLP_EMPTY  = 4 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_SLP_FORM   = 5 + OPOSERREXT; // EndRemoval
const LONG OPOS_EFPTR_MISSING_DEVICES            =
            6 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_WRONG_STATE                =
            7 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_TECHNICAL_ASSISTANCE       =
            8 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_CLOCK_ERROR                =
            9 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_FISCAL_MEMORY_FULL         =
            10 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_FISCAL_MEMORY_DISCONNECTED =
            11 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_FISCAL_TOTALS_ERROR        =
            12 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_ITEM_QUANTITY          =
            13 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_ITEM_AMOUNT            =
            14 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_ITEM_DESCRIPTION       =
            15 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_RECEIPT_TOTAL_OVERFLOW     =
            16 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_VAT                    =
            17 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_PRICE                  =
            18 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BAD_DATE                   =
            19 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_NEGATIVE_TOTAL             =
            20 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_WORD_NOT_ALLOWED           =
            21 + OPOSERREXT; // (Several)
const LONG OPOS_EFPTR_BUSYWAIT           =
            22 + OPOSERREXT; // (Several)

////////////////////////////////////////////////////////
//////////// DIRECTIO PARAMETER 1 VALUES ///////////////
////////////////////////////////////////////////////////
const long OPOS_FPTR_DI_RAWDATA			= 0;	// used by application
const long OPOS_FPTR_DI_FISCALDOCTYPE	= 1;	// used by application
const long OPOS_FPTR_PRN_FISCALREC      = 2;
const long OPOS_FPTR_PRN_OUTFISREC      = 3; 
const long OPOS_FPTR_PRN_FISCALREQUEST  = 4;    

const long OPOS_FPTR_DI_LINEDISPLAYON	 = 11;	//used by LineDisplay Control
const long OPOS_FPTR_DI_LINEDISPLAYOFF	 = 12;  //used by LineDisplay Control

// used by DirectIO to set FiscalDocType
const long OPOS_FPTR_FO_CLASS1			= 0;
const long OPOS_FPTR_FO_CLASS3			= 1;
const long OPOS_FPTR_FO_CLASS2_INVOICE	= 2;
const long OPOS_FPTR_FO_CLASS2_RECEIPT	= 3;
const long OPOS_FPTR_FO_CLASS2_BOLLA	= 4;

#endif                  // !defined(OPOSFPTR_H)
