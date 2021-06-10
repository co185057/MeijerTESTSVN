#ifndef _CONFIG_OBJECT_CONSTANTS_H
#define _CONFIG_OBJECT_CONSTANTS_H

#define MAX_TENDER_CASHBACK_VALUES      11
#define MAX_TENDER_EBT_CASHBACK_VALUES  11
#define MAX_PRINTOBJECTS    2
#define MAXBUTTONNUM  15     //New Finalization

typedef    struct 
    {
        int nTenderType;
        CString csTenderText;
        CString csTenderBitmap;
        bool bCashBack;
        bool bConfirmTotal;
        bool bNeedPin;
        bool bIntegratedMSR;
        bool bCaptureSignature;
        bool bVerifySignature;
        int nVerifySignatureOverAmount;
        HBITMAP hTenderBitmap;
        int  nAssistedTender; // prompts at RAP to use Assistmode to complete this tender as of now
        int  nButtonNum;      // NewUI - stores button number
        int  nTenderLimit;    // trx total is checked against this limit and then intervention follows(=0,always intervention, <0 no intervention, -1 is default value)
        bool bCashBackIfVoided; //RFC 275377 MIGRATED-BM185018
        int nPrintReceiptTrxOverAmount; //SR832, holds the value of PrintReceiptTrxOverAmount in SCOTTEND file
        CString csAnimation;
    }TenderButton;

typedef struct
{
   TCHAR szCode[5];          //Two letter ISO 639 code for language or 4 character language code
   CString csName;           //Button for language on Attract screen
   CString csMessageFile;    //Customer Text message file
   CString csTareFile;       //Tare description file
   CString csSecMessageFile; //Security message file (ie. SASIActione)
   CString csTenderFile;     //Tender file
   CString csPriceSoundConfigFile;  //Price Sound Config File
   CString csButtonBitmap;   //Bitmap filename used for the language button in the lang selection screen
} SLanguageInfo;

const enum SCOT_LANGUAGE_TYPE
{
   SCOT_LANGUAGE_PRIMARY,
   SCOT_LANGUAGE_SECONDARY,
   SCOT_LANGUAGE_THIRD,
   SCOT_LANGUAGE_FOURTH,
   SCOT_LANGUAGE_FIFTH,
   SCOT_LANGUAGE_SIXTH,

   SCOT_LANGUAGE_MAX  // If adding in more language choices, add them before the max entry
};

typedef struct 
{
    CString csBmpName;
    long    nBmpWidth;
    long    nAlignment;
    float   scalingFactor;           //TAR 271776
} SBmpPrintObject;

//SSCOADK-2504 Load Barcode Print Widths
typedef struct
{
    int nBarcodeType;
    int nPrintWidth;
} SBmpPrintWidths;


#endif // _CONFIG_OBJECT_CONSTANTS_H
