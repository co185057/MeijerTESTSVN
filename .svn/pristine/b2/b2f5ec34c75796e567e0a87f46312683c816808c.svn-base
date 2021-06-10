/////////////////////////////////////////////////////////////////////
//
// OposPtr.h
//
//   POS Printer header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 95-12-08 OPOS Release 1.0                                     CRM
// 96-04-22 OPOS Release 1.1                                     CRM
//   Add CapCharacterSet values.
//   Add ErrorLevel values.
//   Add TransactionPrint Control values.
// 97-06-04 OPOS Release 1.2                                     CRM
//   Remove PTR_RP_NORMAL_ASYNC.
//   Add more barcode symbologies.
/////////////////////////////////////////////////////////////////////

#if !defined(OPOSPTR_H)
#define      OPOSPTR_H


#include "Opos.h"


/////////////////////////////////////////////////////////////////////
// Printer Station Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_S_JOURNAL        = 1;
const LONG PTR_S_RECEIPT        = 2;
const LONG PTR_S_SLIP           = 4;

const LONG PTR_S_JOURNAL_RECEIPT= PTR_S_JOURNAL | PTR_S_RECEIPT;
const LONG PTR_S_JOURNAL_SLIP   = PTR_S_JOURNAL | PTR_S_SLIP   ;
const LONG PTR_S_RECEIPT_SLIP   = PTR_S_RECEIPT | PTR_S_SLIP   ;


/////////////////////////////////////////////////////////////////////
// "CapCharacterSet" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_CCS_ALPHA        =   1;
const LONG PTR_CCS_ASCII        = 998;
const LONG PTR_CCS_KANA         =  10;
const LONG PTR_CCS_KANJI        =  11;


/////////////////////////////////////////////////////////////////////
// "CharacterSet" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_CS_ASCII         = 998;
const LONG PTR_CS_WINDOWS       = 999;


/////////////////////////////////////////////////////////////////////
// "ErrorLevel" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_EL_NONE          = 1;
const LONG PTR_EL_RECOVERABLE   = 2;
const LONG PTR_EL_FATAL         = 3;


/////////////////////////////////////////////////////////////////////
// "MapMode" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_MM_DOTS          = 1;
const LONG PTR_MM_TWIPS         = 2;
const LONG PTR_MM_ENGLISH       = 3;
const LONG PTR_MM_METRIC        = 4;


/////////////////////////////////////////////////////////////////////
// "CutPaper" Method Constant
/////////////////////////////////////////////////////////////////////

const LONG PTR_CP_FULLCUT       = 100;


/////////////////////////////////////////////////////////////////////
// "PrintBarCode" Method Constants:
/////////////////////////////////////////////////////////////////////

//   "Alignment" Parameter
//     Either the distance from the left-most print column to the start
//     of the bar code, or one of the following:

const LONG PTR_BC_LEFT          = -1;
const LONG PTR_BC_CENTER        = -2;
const LONG PTR_BC_RIGHT         = -3;

//   "TextPosition" Parameter

const LONG PTR_BC_TEXT_NONE     = -11;
const LONG PTR_BC_TEXT_ABOVE    = -12;
const LONG PTR_BC_TEXT_BELOW    = -13;

//   "Symbology" Parameter:

//     One dimensional symbologies
const LONG PTR_BCS_UPCA         = 101;  // Digits
const LONG PTR_BCS_UPCE         = 102;  // Digits
const LONG PTR_BCS_JAN8         = 103;  // = EAN 8
const LONG PTR_BCS_EAN8         = 103;  // = JAN 8 (added in 1.2)
const LONG PTR_BCS_JAN13        = 104;  // = EAN 13
const LONG PTR_BCS_EAN13        = 104;  // = JAN 13 (added in 1.2)
const LONG PTR_BCS_TF           = 105;  // (Discrete 2 of 5) Digits
const LONG PTR_BCS_ITF          = 106;  // (Interleaved 2 of 5) Digits
const LONG PTR_BCS_Codabar      = 107;  // Digits, -, $, :, /, ., +;
                                        //   4 start/stop characters
                                        //   (a, b, c, d)
const LONG PTR_BCS_Code39       = 108;  // Alpha, Digits, Space, -, .,
                                        //   $, /, +, %; start/stop (*)
                                        // Also has Full ASCII feature
const LONG PTR_BCS_Code93       = 109;  // Same characters as Code 39
const LONG PTR_BCS_Code128      = 110;  // 128 data characters
//        (The following were added in Release 1.2)
const LONG PTR_BCS_UPCA_S       = 111;  // UPC-A with supplemental
                                        //   barcode
const LONG PTR_BCS_UPCE_S       = 112;  // UPC-E with supplemental
                                        //   barcode
const LONG PTR_BCS_UPCD1        = 113;  // UPC-D1
const LONG PTR_BCS_UPCD2        = 114;  // UPC-D2
const LONG PTR_BCS_UPCD3        = 115;  // UPC-D3
const LONG PTR_BCS_UPCD4        = 116;  // UPC-D4
const LONG PTR_BCS_UPCD5        = 117;  // UPC-D5
const LONG PTR_BCS_EAN8_S       = 118;  // EAN 8 with supplemental
                                        //   barcode
const LONG PTR_BCS_EAN13_S      = 119;  // EAN 13 with supplemental
                                        //   barcode
const LONG PTR_BCS_EAN128       = 120;  // EAN 128
const LONG PTR_BCS_OCRA         = 121;  // OCR "A"
const LONG PTR_BCS_OCRB         = 122;  // OCR "B"


//     Two dimensional symbologies
const LONG PTR_BCS_PDF417       = 201;
const LONG PTR_BCS_MAXICODE     = 202;

//     Start of Printer-Specific bar code symbologies
const LONG PTR_BCS_OTHER        = 501;


/////////////////////////////////////////////////////////////////////
// "PrintBitmap" Method Constants:
/////////////////////////////////////////////////////////////////////

//   "Width" Parameter
//     Either bitmap width or:

const LONG PTR_BM_ASIS          = -11;  // One pixel per printer dot

//   "Alignment" Parameter
//     Either the distance from the left-most print column to the start
//     of the bitmap, or one of the following:

const LONG PTR_BM_LEFT          = -1;
const LONG PTR_BM_CENTER        = -2;
const LONG PTR_BM_RIGHT         = -3;


/////////////////////////////////////////////////////////////////////
// "RotatePrint" Method: "Rotation" Parameter Constants
// "RotateSpecial" Property Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_RP_NORMAL        = 0x0001;
const LONG PTR_RP_RIGHT90       = 0x0101;
const LONG PTR_RP_LEFT90        = 0x0102;
const LONG PTR_RP_ROTATE180     = 0x0103;


/////////////////////////////////////////////////////////////////////
// "SetLogo" Method: "Location" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_L_TOP            = 1;
const LONG PTR_L_BOTTOM         = 2;


/////////////////////////////////////////////////////////////////////
// "TransactionPrint" Method: "Control" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_TP_TRANSACTION   = 11;
const LONG PTR_TP_NORMAL        = 12;


/////////////////////////////////////////////////////////////////////
// "StatusUpdateEvent" Event: "Data" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG PTR_SUE_COVER_OPEN   =   11;
const LONG PTR_SUE_COVER_OK     =   12;

const LONG PTR_SUE_JRN_EMPTY    =   21;
const LONG PTR_SUE_JRN_NEAREMPTY=   22;
const LONG PTR_SUE_JRN_PAPEROK  =   23;

const LONG PTR_SUE_REC_EMPTY    =   24;
const LONG PTR_SUE_REC_NEAREMPTY=   25;
const LONG PTR_SUE_REC_PAPEROK  =   26;

const LONG PTR_SUE_SLP_EMPTY    =   27;
const LONG PTR_SUE_SLP_NEAREMPTY=   28;
const LONG PTR_SUE_SLP_PAPEROK  =   29;

const LONG PTR_SUE_IDLE         = 1001;


/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Printer
/////////////////////////////////////////////////////////////////////

const LONG OPOS_EPTR_COVER_OPEN = 1 + OPOSERREXT; // (Several)
const LONG OPOS_EPTR_JRN_EMPTY  = 2 + OPOSERREXT; // (Several)
const LONG OPOS_EPTR_REC_EMPTY  = 3 + OPOSERREXT; // (Several)
const LONG OPOS_EPTR_SLP_EMPTY  = 4 + OPOSERREXT; // (Several)
const LONG OPOS_EPTR_SLP_FORM   = 5 + OPOSERREXT; // EndRemoval
const LONG OPOS_EPTR_TOOBIG     = 6 + OPOSERREXT; // PrintBitmap
const LONG OPOS_EPTR_BADFORMAT  = 7 + OPOSERREXT; // PrintBitmap


#endif                  // !defined(OPOSPTR_H)
