///////////////////////////////////////////////////////////////////////////////////////////
//
//
// TITLE:   MobileCartAuditDefs.h
//          This module contains common definitions across mobile audit subsystem
//
// AUTHOR:  TDonnan
// 
/////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _MOBILE_CART_AUDIT_DEFS_H
#define _MOBILE_CART_AUDIT_DEFS_H

const TCHAR szStartAuditRequest[]           = _T("StartAuditRequest");
const TCHAR szApplicationId[]               = _T("ApplicationId");
const TCHAR szLanguageCode[]                = _T("LanguageCode");
const TCHAR szStoreNumber[]                 = _T("StoreNumber");
const TCHAR szTerminalNumber[]              = _T("TerminalNumber");
const TCHAR szTransactionNumber[]           = _T("TransactionNumber");
const TCHAR szCustomerCardNumber[]          = _T("CustomerCardNumber");
const TCHAR szDocument[]                    = _T("Document");
const TCHAR szHeader[]                      = _T("Header");
const TCHAR szCardNumber[]                  = _T("CardNumber");
const TCHAR szCustomerLanguage[]            = _T("CustomerLanguage");
const TCHAR szFooter[]                      = _T("Footer");
const TCHAR szPosNumber[]                   = _T("PosNumber");
const TCHAR szStartAuditResponse[]          = _T("StartAuditResponse");
const TCHAR szSessionId[]                   = _T("SessionId");
const TCHAR szResult[]                      = _T("Result");
const TCHAR szResultMessage[]               = _T("ResultMessage");
const TCHAR szRescanType[]                  = _T("RescanType");
const TCHAR szRescanStatus[]                = _T("RescanStatus");
const TCHAR szCustomerName[]                = _T("CustomerName");
const TCHAR szNumberOfVisits[]              = _T("NumberOfVisits");
const TCHAR szFirstVisit[]                  = _T("FirstVisit");
const TCHAR szNumberOfItemsInTicket[]       = _T("NumberOfItemsInTicket");
const TCHAR szTotalTicketAmount[]           = _T("TotalTicketAmount");
const TCHAR szNumItemsToRescan[]            = _T("NumItemsToRescan");
const TCHAR szAmountToRescan[]              = _T("AmountToResscan");
const TCHAR szRescanSkipAllowed[]           = _T("RescanSkipAllowed");
const TCHAR szRescanAbortAllowed[]          = _T("RescanAbortAllowed");
const TCHAR szAddAuditItem[]                = _T("AddAuditItem");
const TCHAR szQuantity[]                    = _T("Quantity");
const TCHAR szBarcode[]                     = _T("Barcode");
const TCHAR szAddAuditItemResponse[]        = _T("AddAuditItemResponse");
const TCHAR szId[]                          = _T("Id");
const TCHAR szDescription[]                 = _T("Description");
const TCHAR szPrice[]                       = _T("Price");
const TCHAR szRemainingItems[]              = _T("RemainingItems");
const TCHAR szRemainingAmount[]             = _T("RemainingAmount");
const TCHAR szNumItemsScanned[]             = _T("NumItemsScanned");
const TCHAR szScannedItemsAmount[]          = _T("ScannedItemsAmount");
const TCHAR szEndAudit[]                    = _T("EndAudit");
const TCHAR szAction[]                      = _T("Action");
const TCHAR szActionSend[]                  = _T("Send");
const TCHAR szActionCancel[]                = _T("Cancel");
const TCHAR szActionSkip[]                  = _T("Skip");
const TCHAR szActionAbort[]                 = _T("Abort");
const TCHAR szActionErrorExit[]             = _T("ErrorExit");
const TCHAR szActionSSCOAbortAudit[]        = _T("SSCOAbortAudit");
const TCHAR szEndAuditResponse[]            = _T("EndAuditResponse");
const TCHAR szSwitchToFullAudit[]           = _T("SwitchToFullAudit");
const TCHAR szOnError[]                     = _T("OnError");
const TCHAR szSwitchToFullAuditResponse[]   = _T("SwitchToFullAuditResponse");
const TCHAR szContinueAfterError[]          = _T("ContinueAfterError");
const TCHAR szContinueAfterErrorResponse[]  = _T("ContinueAfterErrorResponse");
const TCHAR szDefaultLanguageId[]           = _T("en");
const TCHAR szDefaultCustomerName[]         = _T("Unknown Customer");
const TCHAR szEmpty[]                       = _T("");
const TCHAR szZero[]                        = _T("0");
const TCHAR szFalse[]                       = _T("False");
const TCHAR szTrue[]                        = _T("True");
const TCHAR szFailure[]                     = _T("Failure");
const TCHAR szSuccess[]                     = _T("Success");
const TCHAR szItemNotFound[]                = _T("ItemNotFound");
const TCHAR szPartialRescanFailed[]         = _T("PartialRescanFailed");
const TCHAR szUnknown[]                     = _T("UNKNOWN");
const TCHAR szButtonScanData[]              = _T("SCANDATA");
const TCHAR szButtonFinishMobileCartAudit[] = _T("cFinishMobileCartAudit");
const TCHAR szButtonContinueAfterError[]    = _T("cContinueAfterError");
const TCHAR szButtonContinueFullRescanErr[]    = _T("cContinueAfterFullRescanError");
const TCHAR szButtonFullAudit[]             = _T("cFullAudit");
const TCHAR szButtonExitAudit[]             = _T("cExitAudit");
const TCHAR szButtonReturnToAudit[]         = _T("cReturnToAudit");
const TCHAR szButtonAbortAudit[]            = _T("cAbortAudit");
const TCHAR szFullRescanAfterError[]        = _T("FullRescanAfterError");
const TCHAR szBarcodeType[]                 = _T("BarcodeType");
const TCHAR szBarcodeData[]                 = _T("BarcodeData");
const TCHAR szBarcodeLabel[]                = _T("BarcodeLabel");
const TCHAR szGetHHScanner[]                = _T("cGetHHScanner");
const TCHAR szReleaseHHScanner[]            = _T("cReleaseHHScanner");
#endif