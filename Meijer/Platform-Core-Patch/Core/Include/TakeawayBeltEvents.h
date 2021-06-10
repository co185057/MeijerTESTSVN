// Event notifications from application to Takeaway Belt

#ifndef __TAKEAWAYBELTEVENTS_H__
#define __TAKEAWAYBELTEVENTS_H__

enum ETakeawayBeltAppEvents
{
    tabEventInvalid             =  0x0000,
    tabEventStableGoodWeight	=  0x0001,
    tabEventStableBadWeight		=  0x0002,
    tabEventLightItemExpected	=  0x0003,
    tabEventScaleReadsZero		=  0x0004,
    tabEventItemScan			=  0x0005,
    tabEventReset				=  0x0006,
    tabEventSkipBagging         =  0x0007,
    tabEventFlushAndFinish      =  0x0008,
    tabEventHandleBagAreaBackup =  0x0009,
    tabEventAppRequestStopBelt  =  0x000A,
    tabEventAssistModeStart		=  0x000B,
    tabEventAssistModeStop		=  0x000C,
    tabEventQuantityItem		=  0x000D
};

// Status notifications from Takeaway Belt to application

enum ETakeawayBeltStatuses
{
    tabStatusScannerDisable		=  0x0400,
    tabStatusScannerEnable		=  0x0401,
    tabStatusItemMovedOffBelt	=  0x0402,
    tabStatusBagAreaClear		=  0x0410,
    tabStatusBagAreaBackup		=  0x0411,
    tabStatusSafetyCoverOpened  =  0x0420,
    tabStatusSafetyCoverClosed  =  0x0421,
    tabStatusSendScaleStatus	=  0x0440,
    tabStatusAmExitDone			=  0x0441
};

#endif // !__TAKEAWAYBELTEVENTS_H__
