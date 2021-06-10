#ifndef _ICONFIG_OBJECT_H
#define _ICONFIG_OBJECT_H

#include "ConfigObjectConstants.h"

const enum SCOT_LANGUAGE_TYPE;

class IConfigObject
{
public:
    IConfigObject() {}
    virtual ~IConfigObject() {}

    // State Section
    virtual bool GetfStateDualLanguage(void) const = 0;
    virtual bool GetfStateSingleLanguageButtonAtAttract(void) const = 0;
    virtual bool GetfStateSecurityDisableAllowed(void) const = 0;
    virtual bool GetfStateAttndModeAllowed(void) const = 0;
    virtual bool GetfStateLiveVideo(void) const = 0;
    virtual bool GetfStateDisableAudioInAttendantMode(void) = 0;
    virtual bool GetfStateSayPhrases(void) const = 0;
    virtual bool GetfStateAllowTBAudioControl(void) const = 0;
    virtual bool GetfStateSayPrices(void) const = 0;
    virtual bool GetfStateSayPricesDisableInAttendantMode(void) const = 0;
    
    // Operation Section
    virtual bool GetfOperationsInvertLightStates(void) const = 0;
    virtual bool GetfOperationsAllowMultiPick(void) const = 0;
    virtual bool GetfOperationsAllowFuzzySearch(void) const = 0;
    virtual bool GetfOperationsDisplayPickListItemCode(void) const = 0;
    virtual bool GetfOperationsSuppressAudioInMismatch(void) const = 0;
    virtual bool GetfOperationsAllowContainerBtnForMismatching(void) const = 0;
    virtual bool GetfOperationsAllowContainerBtn(void) const = 0;
    virtual bool GetfOperationsDisableUnexpIncContainerBtnAftFirstItem(void) const = 0;
    virtual bool GetfOperationsSuppressAudioInUnexpectedIncrease(void) const = 0;
    virtual bool GetfOperationsAllowContainerBtnInFinalization(void) const = 0;
    virtual bool GetfOperationsEnableUSSF(void) const = 0;
    virtual bool GetbOperationsUseFingerPrintDevice(void) const = 0;
    virtual bool GetfOperationsPrintVersion(void) const = 0;
    virtual bool GetfOperationsScaleMetric(void) const = 0;
    virtual bool GetfOperationsShowItemRemovedButtonOnRemoveItemViolationScreen(void) const = 0;
    virtual bool GetfOperationsAutoVoidUnderageRestrict(void) const = 0;
    virtual bool GetfOperationsAllowPickListSearchButton(void) const = 0;
    virtual int GetnOperationsOperatorIDMaxDigit(void) const = 0;
    virtual bool GetfOperationsOperatorNumericKeyPad(void) const = 0;
    virtual int GetnOperationsOperatorPWMaxDigit(void) const = 0;
    virtual int GetnOperationsGiftCardRedemption(void) const = 0;
    virtual int GetnOperationsTareContainer1(void) const = 0;
    virtual int GetnOperationsTareContainer2(void) const = 0;
    virtual CString GetcsOperationsOperatorBarcodeMask(void) const = 0;
    virtual bool GetfOperationsSendPOSOperatorBarcode(void) const = 0;
    virtual int GetnOperationsOperatorLoginBarcodeType(void) const = 0;
    virtual bool GetfOperationsAllowLeadingZerosInCode128LoginBarCode(void) const = 0;
    virtual CString GetcsOperationsDefaultSendPOSOperatorPassword(void) const = 0;
    virtual bool GetfOperationsAllowSkipBaggingBtn(void) const = 0;
    virtual bool GetfOperationsInterventionAtStartOfTrainingModeTransaction(void) const = 0;
    virtual bool GetfOperationsHopperSubstitutionIntervention(void) const = 0;
    virtual bool GetfOperationsTakeChangeBypassedIfNoBills(void) const = 0;
    virtual int GetnOperationsAllowDispenseInTrainingMode(void) const = 0;
    virtual bool GetfOperationsPrintReceiptOnCancelledTransaction(void) const = 0;
    virtual bool GetfOperationsUsePicklistXMLImageName(void) const = 0;
    virtual bool GetfOperationsEncodeOperatorIDPW(void) const = 0;
    
    // AutoRun Section
    virtual void SetfAutoRunOn(bool bAutorun) = 0;
    virtual bool GetfAutoRunOn(void) const = 0;
    virtual void SetfAutoRunAssistModeOn(bool bAutoRunAssistModeOn) = 0;
    virtual bool GetfAutoRunAssistModeOn(void) const = 0;
    virtual bool GetfAutoRunBypassBagging(void) const = 0;
    
    // TimeOut Section
    virtual int getTimeOutScannerScaleOnAttract() = 0;
    virtual int getTimeOutItemTransactionResult() = 0;
    virtual int getTimeOutPeriodicSecurityAudio() = 0;
    virtual int getTimeOutDisplayIncreaseMisMatchScreenTimer() = 0;
    
    //Tender Section
    virtual bool GetfTenderShowUseCoupons(void) const = 0;
    virtual bool GetfTenderEBTFSAllowed(void) const = 0;
    virtual bool GetfTenderOtherPayment(void) const = 0;
    virtual bool GetfTenderCashAllowed(void) const = 0;
    virtual bool GetfTenderCreditAllowed(void) const = 0;
    virtual bool GetfTenderDebitAllowed(void) const = 0;
    virtual bool GetfTenderCreditNeedsPIN(void) const = 0;
    virtual bool GetfTenderDebitNeedsPIN(void) const = 0;
    virtual bool GetfTenderCreditCashBack(void) const = 0;
    virtual bool GetfTenderDebitCashBack(void) const = 0;
    virtual bool GetfTenderEBTFSNeedsPIN(void) const = 0;
    virtual bool GetfTenderEBTCBAllowed(void) const = 0;
    virtual bool GetfTenderEBTCBNeedsPIN(void) const = 0;
    virtual bool GetfTenderEBTCBCashBack(void) const = 0;
    virtual bool GetfTenderAllowCancelWithPartialEBTTender(void) const = 0;
    virtual bool GetfTenderSubmitFSTtotalIfMorethanTrxTotal(void) const = 0;
    virtual bool GetfTenderIsMotorizedMSR(void) const = 0;
    virtual bool GetfTenderDispenseChangeAtLaneForRemoteTendering(void) const = 0;

    // Locale
    virtual CString GetcsLocaleStoreName() const =0;

    //Tracing Section
    virtual int GetnTracingCurrentLevel(void) const = 0;

    virtual int             GetNumberOfLanguagesSupported(void) const = 0;
    virtual int             GetmsTraceToFlush() = 0;
    virtual int             GetnCMCustomerMessage(void) const = 0;
    virtual void            InitPickListAssistOptions() = 0;
    virtual void            InitCustomerBagAllowed(bool bIsTAB) = 0;
    virtual bool            GetfUserExitAllEnabled(void) const = 0;
    virtual bool            GetfUserExit00Enabled(void) const = 0;
    virtual bool            IsShowVolumeControl(void) const = 0;
    virtual bool            GetfCMRewardInfo() const = 0;
    virtual long            Initialize(const int) = 0;
    virtual CString         GetcsPicklistImagePath(void) const = 0;
    virtual CString         GetcsDataEntryINIFile() const = 0;
    virtual CString         GetcsPrimaryLanguageCode(void) = 0;
    virtual CString         GetcsTemplateAssistMode() const = 0;
    virtual CString         GetcsDataXMLFile() const = 0;
    virtual CString         GetcsDataTareFile() const = 0;
    virtual SLanguageInfo   Getlanguage(int index) const = 0;
    virtual TenderButton    GetTenderButton(SCOT_LANGUAGE_TYPE language, int nButtonIndex) const = 0;
    
};

#endif // _ICONFIG_OBJECT_H
