#ifndef _PSPROCEDURES_CONSTANTS_H
#define _PSPROCEDURES_CONSTANTS_H

// weight / scale status
#define PS_SCALE_UNSTABLE   -1
#define PS_SCALE_UNDERZERO  -2
#define PS_SCALE_OVERWEIGHT -3

#define MAXTARE 100
#define MAXCATEGORIES 9

#ifndef TB_BASE 
#define TB_BASE 11000           // TB defined string IDs start with 11000
#endif

#ifndef SCUD_BASE
#define SCUD_BASE 9000            // SC.ot CU.stomer D.efined string constants or IDs start with 5000
#endif

#ifndef BEST_BASE
#define BEST_BASE 5000            // SC.ot CU.stomer D.efined base entry state IDs start with 5000
#endif

const TCHAR TAB_FAV[] = _T("Favorites");
const TCHAR TAB_CATEGORY[] = _T("Category");
const TCHAR SUBCATEGORY_ITEM[] = _T("SubCategoryItem");

const TCHAR XML_PICKLISTFILE[] = _T("PickList.xml");
const TCHAR XML_CATEGORY[] = _T("//PickList//Categories//Category");
const TCHAR XML_ITEM[] = _T("//PickList//Items//Item");
const TCHAR XML_ID[] = _T("Id");
const TCHAR XML_LANGUAGE[] = _T("Languages//Language");
const TCHAR XML_DEFAULT_LANGUAGE[] = _T("//PickList//Parameters//DefaultLanguage");
const TCHAR XML_DESCRIPTION[] = _T("Description");
const TCHAR XML_LCID[] = _T("LCID");
const TCHAR XML_UPC[] = _T("UPC");
const TCHAR XML_ISVISIBLE[] = _T("IsVisible");
const TCHAR XML_ALPHABETIC[] = _T("Alphabetic");
const TCHAR XML_PICTURE[] = _T("Pictures//Picture");
const TCHAR XML_RESOLUTION[] = _T("Resolution");
const TCHAR XML_ISQUANTITY[] = _T("IsQuantity");
const TCHAR XML_ISPOPULAR[] = _T("IsPopular");
const TCHAR XML_CATEGORIES[] = _T("Categories");
const TCHAR XML_SORTINGSTRING[] = _T("SortingString");
const TCHAR XML_DEPARTMENT[] = _T("Department");
const TCHAR XML_TRUE[] = _T("True");
const TCHAR XML_FALSE[] = _T("False");
const TCHAR XML_ISZEROWEIGHT[] = _T("IsZeroWeightItem"); //Japan RFC#1 (RFQ 893) 
const TCHAR XML_SUBCATEGORIES[] = _T("SubCategories");
const TCHAR XML_SCREENTITLE[] = _T("ScreenTitle");
const TCHAR XML_INSTRUCTIONS[] = _T("Instructions");
const TCHAR XML_ISQUICKPICKITEM[] = _T("IsQuickPickItem");
const TCHAR XML_INCATEGORIES[] = _T("InCategories");
const TCHAR XML_PICTUREID[] = _T("ID");

    
// string size limits
#define MAX_DIGITS            72    // used by EchoInput() to limit length
#define MAX_DIGITS_UPC        15    // upc codes to EAN-13
#define MAX_DIGITS_PIN        10    // pin numbers, let DM limit length
#define MAX_DIGITS_CASHBACK   3     // 0 to 999 dollars cash back
#define MAX_DIGITS_ITEMCODE   72    // needed for supplemental barcode
#define MAX_DIGITS_QUANTITY   3     // 1-999 quantity
// BeginLocaleCode
// #define MAX_DIGITS_DATE       6  // date format, such as mm/dd/yy
// #define MAX_DIGITS_YEAR       2  // yy or yyyy
// EndLocaleCode
#define MAX_DIGITS_CPNAMT     4     // $0.01 - $99.99 coupon amount
#define MAX_DIGITS_CASHAMT    9    // $0.01 - $9999999.99
#define MAX_DIGITS_ACCTNUM    16    // credit debit card max
#define MIN_DIGITS_ACCTNUM    13    // credit debit card min

// this is used to mark non-item or canceled item lines
// on the receipt
#define INFO_ITEM _T('+')                // info related the this item purchase
#define VOID_ITEM _T('-')                // this item has been voided
#define LINK_ITEM _T('#')                // this is a item linked to the prev. item
#define BMP_EXT _T("BMP")
//Personalization picklist
#define TOKEN_SEPARATOR _T(",")           // a separator for the list of UPC code 
                                         // from the personalization server

const enum ITEMTYPE
{
   TYPE_ITEM = 1,
   TYPE_CATEGORY,
   TYPE_SUBCATEGORY
};
    
    // background BMPs for frames
#define BMPS int
const enum BMPSLIST
{
  BMP_ATTRACT1 = 1,               // 1
  BMP_WELCOME,                    // 2
  BMP_ITEMENTRY,                  // 3
  BMP_CPNENTRY,                   // 4
  BMP_CANTSCAN,                   // 5
  BMP_HELP,                       // 6
  BMP_SETASIDE,                   // 7
  BMP_PUTONSCALE,                 // 8
  BMP_RESETSCALE,                 // 9
  BMP_ATMKEY,                     // 10
  BMP_ENTERITEM,                  // 1
  BMP_UNKNOWNITEM,                // 2
  BMP_QTYCONFIRM,                 // 3
  BMP_CPNNOITEM,                          // 4
  BMP_TIMEOUT,                    // 5
  BMP_CANCELITEM,                 // 6
  BMP_TRANSABORT,                 // 7
  BMP_WAITFORAPP,                 // 8
  BMP_FINISHPAY,                  // 9
  BMP_CPNSETASIDE,                // 20
  BMP_CPNDEPOSIT,                 // 1
  BMP_CASHINSERT,                 // 2
  BMP_CARDINSERT,                 // 3
  BMP_CARDPROCESSING,             // 4
  BMP_CPNVALUE,                   // 5
  BMP_TAKECHANGE,                 // 6
  BMP_TAKECARD,                   // 7
  BMP_TAKECASH,                   // 8
  BMP_TAKERECEIPT,                // 9
  BMP_THANKYOU,                   // 30
  BMP_CREDITDEBIT,                // 1
  BMP_CONFIRMDEBITAMT,            // 2
  BMP_SIGNCHARGE,                 // 3
  BMP_SIGNCONFIRM,                // 4
  BMP_WAITFORHELP,                // 5
  BMP_USEMESPARE1,                // 6 spare
  BMP_USEMESPARE2,                // 7 spare
  BMP_OUTOFSERVICE_NOECHO,        // 8 
  BMP_LISTTARES,                  // 9
  BMP_ERRMSGLEVEL1,               // 40
  BMP_ERRMSGLEVEL2,               // 1
  BMP_ERRMSGLEVEL3,               // 2
  BMP_PINENTER,                   // 3
  BMP_DEBITAMT,                   // 4
  BMP_OUTOFSERVICE,               // 5
  BMP_STOREABORT,                 // 6
  BMP_SMBLANK,                              // 7
  BMP_SMDEFAULT,                  // 8
  BMP_SMPAYMENT,                  // 9
  BMP_SMLIST,                     // 50
  BMP_ASSISTMODE,                 // 1
  BMP_PLASTICBAG,                 // 2
  BMP_TYPEITEMDESC,               // 3
  BMP_SELECTFROMLIST,             // 4
  BMP_FINISHPAYSWIPE,             // 5
  BMP_CARDSWIPE,                  // 6
  BMP_TAKELOYALTYCARD,            // 7
  BMP_INSERTCARD,                          // 8
  BMP_ENTERAMOUNT,                        // 9
  BMP_CMUSSCALEOVERLAY,           // 60 
  BMP_CMMETRICSCALEOVERLAY,       // 1
  BMP_REQUESTSIGNATURE,                  // 2
  BMP_ACKNOWLEDGESIGNATURE,            // 3
  BMP_DROPOFFCOUPONS,             // 4   
  BMP_BENEFITTYPE,                        // 5
  BMP_SIGNANDWAIT,                // 6
  BMP_CARDMISREAD,                // 7
  BMP_SMUSSCALEOVERLAY,           // 8 
  BMP_SMMETRICSCALEOVERLAY,       // 9
  BMP_PINENTER_NOECHO,            // 70
  BMP_PINENTER_NOECHO_SIGDEV,     // 1
  BMP_FINISHPAYSWIPE_SIGDEV,      // 2
  BMP_CARDSWIPE_SIGDEV,           // 3
  BMP_CM_BLANK_SCALEOVERLAY,      // 4
  BMP_SM_BLANK_SCALEOVERLAY,      // 5
  BMP_CM_TRAININGMODE_SCALEOVERLAY,            // 6
  BMP_SM_TRAININGMODE_SCALEOVERLAY,            // 7
  BMP_CM_TRAININGMODE_SCALEOVERLAY_METRIC,    // 8
  BMP_SM_TRAININGMODE_SCALEOVERLAY_METRIC,    //     9
  BMP_GRAPHICALSELECT,                        //    80 
  BMP_FINISHPAYPIP,                            //     1
  BMP_PINPADENTRY,                            //     2
  BMP_FINISHCARDPIP,                        //     3
  BMP_SLIDECARD7401,                        //     4
  BMP_SLIDECARDPAYMENTDEVICE,                //     5
  BMP_SENSORMATIC,                            //     6
  BMP_KEYINCODE,                            //     7
  BMP_WELCOME_REWARD,                        //     8
  BMP_ITEMENTRY_REWARD,                        //     9
  BMP_SELECT_PAYMENT_TYPE,                    //    90
  BMP_CHECKBASKETREMINDER,                    //   1
  BMP_USDCASHINSERT,                        //     2        
  BMP_CDNCASHINSERT,                         //     3        
  BMP_GBPeCASHINSERT,                        //     4        
  BMP_GBPsCASHINSERT,                        //     5        
  BMP_GBPcCASHINSERT,                        //     6        
  BMP_GBPrCASHINSERT,                        //     7        
  BMP_DEMCASHINSERT,                        //     8        
  BMP_NLGCASHINSERT,                        //     9        
  BMP_EURCASHINSERT,                        // 100     
  BMP_BIZERBAPUTONSCALE,                    // 101
  BMP_SCANCARD,                             // 102
  BMP_WAITFORASSISTANCE,                    // 103
  BMP_ENTERJOBINFO,                            //104
  BMP_BAGITEM,                                //105
  BMP_BAGITEMANDEAS,                        //106
  BMP_WAITFORASSISTANCE_VOIDITEM,           //107
  BMP_SECVIOLATIONRETURNINGITEMS,            //108
  BMP_ASSISTANCENEEDED,                        //109
  BMP_ASSISTANCENEEDEDITEMDETAILS,            //110
  BMP_SECVIOLATIONREMOVEITEM,                //111
  BMP_SECVIOLATIONREMOVEUNEXPECTEDITEM,        //112
  BMP_PLACEITEMONBELT,                      // 113
  BMP_SECVIOLATIONREMOVEUNEXPECTEDITEMFROMBELT,    // 114
  BMP_SETASIDE_BELT,                        // 115
  BMP_PUTONSCALE_BELT,                      // 116
  BMP_RESETSCALE_BELT,                      // 117
  BMP_TRANSPORT_ITEM,                        // 118
  BMP_SECVIOLATIONBAGAREABACKUP,            // 119
  BMP_ASSISTEDTENDER,                       // 120
  BMP_AUDCASHINSERT,                        // 121
  BMP_NZDCASHINSERT,                        // 122
  BMP_TAKECHANGE_BELT,                      // 123
  BMP_TAKECASH_BELT,                        // 124
  BMP_MARS_BillCassetteOut,                 // 125
  BMP_MARS_BillAcceptComError,              // 126
  BMP_MARS_BillAcceptJam,                   // 127
  BMP_5633_BillRejectBinOut,                // 128
  BMP_5633_CurrencyCassetteOut,             // 129
  BMP_BillDispPickFailure,                  // 130
  BMP_BillDispPickSensorJam,                // 131
  BMP_5633_BillDispExitJam,                 // 132
  BMP_5635_BillDispExitJam,                 // 133
  BMP_BillDispTopJam,                       // 134
  BMP_BillDispDiverterJam,                  // 135
  BMP_BillDispFeedJam,                      // 136
  BMP_5635_BillDispLVDTJam,                 // 137
  BMP_CoinAcceptComError,                   // 138
  BMP_PinPadComError,                       // 139
  BMP_K580_PrinterCabDoorOpen,              // 140
  BMP_K590_PrinterCabDoorOpen,              // 141
  BMP_K580_PrinterPlatenDoorOpen,           // 142
  BMP_K590_PrinterPlatenDoorOpen2x,         // 143
  BMP_K590_PrinterPlatenDoorOpen3x,         // 144
  BMP_K580_PresenterPaperJam,               // 145
  BMP_K590_PresenterPaperJam,               // 146
  BMP_K580_PrinterMechPaperJam,             // 147
  BMP_K590_PrinterMechPaperJam,             // 148
  BMP_MCRCardJam,                           // 149
  BMP_CASHFLOW_CASSOUT,                     // 150
  BMP_CASHFLOW_COMERROR,                    // 151
  BMP_CASHFLOW_BILLJAM,                     // 152
  BMP_CASHFLOW_ACCEPTOPEN,                  // 153
  BMP_CASHFLOW_CASSFULL,                    // 154
  BMP_TEC306_PAPEROUT,                      // 155
  BMP_TEC306_COVOPEN,                       // 156
  BMP_TEC306_PAPERJAM,                      // 157
  BMP_TEC306_PAPERJAMPRE,                   // 158
  BMP_LANGUAGE1,                            // 159
  BMP_LANGUAGE2,                            // 160
  BMP_LANGUAGE3,                            // 161
  BMP_LANGUAGE4,                            // 162    
  BMP_LANGUAGE5,                            // 163
  BMP_LANGUAGE6,                            // 164
  BMP_ARDAC_CASSOUT,                        // 165
  BMP_ARDAC_COMERROR,                       // 166
  BMP_ARDAC_BILLJAM,                        // 167
     BMP_BILLRECYCLER_HARDWARE_ERROR_11,        //168
     BMP_BILLRECYCLER_HARDWARE_ERROR_12,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_13,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_14,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_15,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_16,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_17,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_18,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_21,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_22,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_24,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_25,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_26,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_27,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_28,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_31,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_32,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_41,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_42,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_43,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_44,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_45,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_46,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_47,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_48,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_49,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_51,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_52,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_53,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_57,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_58,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_59,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_61,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_62,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_71,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_72,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_73,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_75,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_76,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_81,   
     BMP_BILLRECYCLER_HARDWARE_ERROR_82,        
     BMP_BILLRECYCLER_HARDWARE_ERROR_83,   
     BMP_COINRECYCLER_HARDWARE_ERROR_11,   
     BMP_COINRECYCLER_HARDWARE_ERROR_12,   
     BMP_COINRECYCLER_HARDWARE_ERROR_14,   
     BMP_COINRECYCLER_HARDWARE_ERROR_15,   
     BMP_COINRECYCLER_HARDWARE_ERROR_16,   
     BMP_COINRECYCLER_HARDWARE_ERROR_17,   
     BMP_COINRECYCLER_HARDWARE_ERROR_19,   
     BMP_COINRECYCLER_HARDWARE_ERROR_21,   
     BMP_COINRECYCLER_HARDWARE_ERROR_22,   
     BMP_COINRECYCLER_HARDWARE_ERROR_23,   
     BMP_COINRECYCLER_HARDWARE_ERROR_24,   
     BMP_COINRECYCLER_HARDWARE_ERROR_26,   
     BMP_COINRECYCLER_HARDWARE_ERROR_31,   
     BMP_COINRECYCLER_HARDWARE_ERROR_32,   
     BMP_COINRECYCLER_HARDWARE_ERROR_41,   
     BMP_COINRECYCLER_HARDWARE_ERROR_43,   
     BMP_COINRECYCLER_HARDWARE_ERROR_44,   
     BMP_COINRECYCLER_HARDWARE_ERROR_50,   
     BMP_COINRECYCLER_HARDWARE_ERROR_51,   
     BMP_COINRECYCLER_HARDWARE_ERROR_52,   
     BMP_COINRECYCLER_HARDWARE_ERROR_53,   
     BMP_COINRECYCLER_HARDWARE_ERROR_54,   
     BMP_COINRECYCLER_HARDWARE_ERROR_55,   
     BMP_COINRECYCLER_HARDWARE_ERROR_56,   
     BMP_COINRECYCLER_HARDWARE_ERROR_57,   
     BMP_COINRECYCLER_HARDWARE_ERROR_58,   
     BMP_COINRECYCLER_HARDWARE_ERROR_59,   
     BMP_COINRECYCLER_HARDWARE_ERROR_61,   
     BMP_COINRECYCLER_HARDWARE_ERROR_62,   
     BMP_COINRECYCLER_HARDWARE_ERROR_71,   
     BMP_COINRECYCLER_HARDWARE_ERROR_72,   
     BMP_COINRECYCLER_HARDWARE_ERROR_81,   
     BMP_COINRECYCLER_HARDWARE_ERROR_82,   
     BMP_COINRECYCLER_HARDWARE_ERROR_83,       //243 
     BMP_COIN_HOPPER_JAM,                      //244
     BMP_PINPADERROR_R34,                      //tar 310198

  BMP_COUNTER                     // counter of BMPs to load
};
    
     
 /*@@@INFOPOINT    
    follows the same numbering pattern as the background images above. 
    Used by the SetMascotBitmap method.
*/
const enum MASCOTBMPSLIST
{
      BMP_MASCOT_ATTRACT1 = 1,               // 1
      BMP_MASCOT_WELCOME,                    // 2
      BMP_MASCOT_ITEMENTRY,                  // 3
      BMP_MASCOT_CPNENTRY,                   // 4
      BMP_MASCOT_CANTSCAN,                   // 5
      BMP_MASCOT_HELP,                       // 6
      BMP_MASCOT_SETASIDE,                   // 7
      BMP_MASCOT_PUTONSCALE,                 // 8
      BMP_MASCOT_RESETSCALE,                 // 9
      BMP_MASCOT_ATMKEY,                     // 10
      BMP_MASCOT_ENTERITEM,                  // 1
      BMP_MASCOT_UNKNOWNITEM,                // 2
      BMP_MASCOT_QTYCONFIRM,                 // 3
      BMP_MASCOT_CPNNOITEM,                        // 4
      BMP_MASCOT_TIMEOUT,                    // 5
      BMP_MASCOT_CANCELITEM,                 // 6
      BMP_MASCOT_TRANSABORT,                 // 7
      BMP_MASCOT_WAITFORAPP,                 // 8
      BMP_MASCOT_FINISHPAY,                  // 9
      BMP_MASCOT_CPNSETASIDE,                // 20
      BMP_MASCOT_CPNDEPOSIT,                 // 1
      BMP_MASCOT_CASHINSERT,                 // 2
      BMP_MASCOT_CARDINSERT,                 // 3
      BMP_MASCOT_CARDPROCESSING,             // 4
      BMP_MASCOT_CPNVALUE,                   // 5
      BMP_MASCOT_TAKECHANGE,                 // 6
      BMP_MASCOT_TAKECARD,                   // 7
      BMP_MASCOT_TAKECASH,                   // 8
      BMP_MASCOT_TAKERECEIPT,                // 9
      BMP_MASCOT_THANKYOU,                   // 30
      BMP_MASCOT_CREDITDEBIT,                // 1
      BMP_MASCOT_CONFIRMDEBITAMT,            // 2
      BMP_MASCOT_SIGNCHARGE,                 // 3
      BMP_MASCOT_SIGNCONFIRM,                // 4
      BMP_MASCOT_WAITFORHELP,                // 5
      BMP_MASCOT_USEMESPARE1,                // 6 spare
      BMP_MASCOT_USEMESPARE2,                // 7 spare
      BMP_MASCOT_USEMESPARE3,                // 8 spare
      BMP_MASCOT_LISTTARES,                  // 9
      BMP_MASCOT_ERRMSGLEVEL1,               // 40
      BMP_MASCOT_ERRMSGLEVEL2,               // 1
      BMP_MASCOT_ERRMSGLEVEL3,               // 2
      BMP_MASCOT_PINENTER,                   // 3
      BMP_MASCOT_DEBITAMT,                   // 4
      BMP_MASCOT_OUTOFSERVICE,               // 5
      BMP_MASCOT_STOREABORT,                 // 6
      BMP_MASCOT_ITEMRESHELF,                // 7
      BMP_MASCOT_SMDEFAULT,                  // 8
      BMP_MASCOT_SMPAYMENT,                  // 9
      BMP_MASCOT_SMLIST,                     // 50
      BMP_MASCOT_ASSISTMODE,                 // 1
      BMP_MASCOT_PLASTICBAG,                 // 2
      BMP_MASCOT_TYPEITEMDESC,               // 3
      BMP_MASCOT_SELECTFROMLIST,             // 4
      BMP_MASCOT_FINISHPAYSWIPE,             // 5
      BMP_MASCOT_CARDSWIPE,                  // 6
      BMP_MASCOT_TAKELOYALTYCARD,            // 7
      BMP_MASCOT_INSERTCARD,                 // 8
      BMP_MASCOT_ENTERAMOUNT,                 // 9
      BMP_MASCOT_USSCALEOVERLAY,             // 60 
      BMP_MASCOT_METRICSCALEOVERLAY,         // 1
      BMP_MASCOT_REQUESTSIGNATURE,             // 2
      BMP_MASCOT_ACKNOWLEDGESIGNATURE,         // 3
      BMP_MASCOT_DROPOFFCOUPONS,             // 4  
      BMP_MASCOT_COUNTER                     // counter of BMPs to load
};

//@@@INFOPOINT

// each literal text is assigned a number which represents
// the line number in a text file of messages. The file can
// be edited to suit the customers needs, or to change languages

#define PSTEXT int
const enum PSTEXTMSG
{                                   // number indicates line in SCOTMSGS.DAT
      PS_NOTEXT,
      SCALE_LB,                            // 1
      SCALE_KG,                            // 2
      FTU_BEGIN,                        // 3        // NOT USED - TAR# 247554
      FTU_SCANBAG,                        // 4        // NOT USED - TAR# 247554
      FTU_BUYPROD,                        // 5        // NOT USED - TAR# 247554
      FTU_BUYHEAVY,                        // 6        // NOT USED - TAR# 247554
      FTU_CANCELITEMS,                    // 7        // NOT USED - TAR# 247554
      FTU_PAYCASH,                        // 8        // NOT USED - TAR# 247554
      FTU_PAYATM,                        // 9        // NOT USED - TAR# 247554
      FTU_PAYCREDIT,                    // 10        // NOT USED - TAR# 247554
      TXT_STOREAPPNEEDED,                // 1        // NOT USED - TAR# 247554
      TXT_QTYEACH,                        // 2
      TXT_WGTEACH,                        // 3
      TXT_QUANTITY,                        // 4        // NOT USED - TAR# 247554
      TXT_ITEMCODE,                        // 5
      MSG_CHARGEAMT,                    // 6
      TXT_COUPON,                        // 7
      TXT_TOTAL,                        // 8
      TXT_TAX,                            // 9
      TXT_PIN,                            // 20        // NOT USED - TAR# 247554
      TXT_DAMOUNT,                        // 1        // NOT USED - TAR# 247554
      TXT_EAMOUNT,                        // 2
      TXT_PAMOUNT,                        // 3
      TXT_CANCELED,                        // 4
      TXT_BIRTHDATE,                    // 5
      TXT_ACCTNUM,                        // 6        // NOT USED - TAR# 247554
      TXT_EXPIRYDATE,                    // 7        // NOT USED - TAR# 247554
      TXT_CANCELALL,                    // 8
      TXT_RESTRICTEDITEMS,                // 9
      TXT_CANCELEDITEMS,                // 30
      TXT_COUPONS,                        // 1
      TXT_AMOUNT,                        // 2        // NOT USED - TAR# 247554
      BTT_I_CORRECTED_THE_PROBLEM,       // 3        //
      TXT_BALANCEDUE,                    // 4        // NOT USED - TAR# 247554
      TXT_CHANGEDUE,                    // 5
      TXT_REFUNDED,                        // 6
      TXT_IN_ITEM,                        // 7        // NOT USED - TAR# 247554
      TXT_IN_STARTUP,                    // 8        // NOT USED - TAR# 247554
      TTL_STOREABORT,                    // 9
      TTL_STOREAUTH,                    // 40
      TTL_RESTRICTED,                    // 1
      TTL_CANCELEDITEMS,                // 2
      TTL_PAYMENT,                        // 3        // NOT USED - TAR# 247554
      TTL_CCPAYMENT,                    // 4        // NOT USED - TAR# 247554
      TTL_UTILITY,                        // 5        // NOT USED - TAR# 247554
      BTT_HELP,                            // 6
      BTT_START,                        // 7
      BTT_FTU,                            // 8        // NOT USED - TAR# 247554
      BTT_ITEMCANT,                        // 9        // NOT USED - TAR# 247554
      BTT_ITEMCANCEL,                    // 50        // NOT USED - TAR# 247554
      BTT_CPNCANT,                        // 1
      BTT_CPNCANCEL,                    // 2
      BTT_WONTSCAN,                        // 3        // NOT USED - TAR# 247554
      BTT_CANCEL,                        // 4
      BTT_CANCELALL,                    // 5
      BTT_CANCELPAY,                    // 6
      BTT_FINISH,                        // 7
      BTT_OK,                            // 8
      BTT_TRYAGAIN,                        // 9
      BTT_CALLHELP,                        // 60
      BTT_EXITHELP,                        // 1        // NOT USED - TAR# 247554
      BTT_YES,                            // 2
      BTT_NO,                            // 3
      BTT_CANCELSELECT,                    // 4
      BTT_READYTOPAY,                    // 5        // NOT USED - TAR# 247554
      BTT_USECOUPONS,                    // 6        // NOT USED - TAR# 247554
      BTT_BUYORCANCEL,                    // 7
      BTT_AUTHRESTS,                    // 8        // NOT USED - TAR# 247554
      BTT_AUTHVOIDED,                    // 9        // NOT USED - TAR# 247554
      BTT_AUTHCOUPONS,                    // 70        // NOT USED - TAR# 247554
      BTT_VISVERIFY,                    // 1        // NOT USED - TAR# 247554
      BTT_AUTHSLOGS,                    // 2        // NOT USED - TAR# 247554
      BTT_GOTOSHOP,                        // 3
      BTT_APPCANCELS,                    // 4
      BTT_DONE,                            // 5
      BTT_CASH,                            // 6        // NOT USED - TAR# 247554
      BTT_CREDITCARD,                    // 7
      BTT_DEBITCARD,                    // 8
      BTT_CHECKS,                        // 9        // NOT USED - TAR# 247554
      BTT_FOODSTAMPS,                    // 80        // NOT USED - TAR# 247554
      BTT_CONTINUE,                        // 1
      BTT_BACKUP,                        // 2        // NOT USED - TAR# 247554
      BTT_EXACTPAY,                        // 3        // NOT USED - TAR# 247554
      BTT_LOADOPTS,                        // 4
      BTT_LOGSTOSERVER,                    // 5        // NOT USED - TAR# 247554
      BTT_STARTTRACE,                    // 6        // NOT USED - TAR# 247554
      BTT_STOPTRACE,                    // 7        // NOT USED - TAR# 247554
      BTT_PRICESON,                        // 8        // NOT USED - TAR# 247554
      BTT_PRICESOFF,                    // 9        // NOT USED - TAR# 247554
      BTT_PHRASESON,                    // 90        // NOT USED - TAR# 247554
      BTT_PHRASESOFF,                    // 1        // NOT USED - TAR# 247554
      LTT_ATTRACT,                        // 2
      LTT_WELCOME,                        // 3
      LTT_ITEMENTRY,                    // 4        // NOT USED - TAR# 247554
      LTT_CPNENTRY,                        // 5
      LTT_CANTSCAN,                        // 6        // NOT USED - TAR# 247554
      LTT_SELECTITEM,                    // 7
      LTT_SELECTTARE,                    // 8        // NOT USED - TAR# 247554
      LTT_HOWMANYCANCEL,                // 9
      LTT_HOWMANYBUY,                    // 100
      LTT_NOPRICE,                        // 1
      LTT_RECALLED,                        // 2
      LTT_KEYBARCODE,                    // 3
      LTT_ENTERITEM,                    // 4        // NOT USED - TAR# 247554
      LTT_UNKNOWNITEM,                    // 5
      LTT_HELPFUL,                        // 6
      LTT_HELPONWAY,                    // 7
      LTT_CPNNOITEM,                    // 8
      LTT_CONTRANS,                        // 9
      LTT_PUTONSCALE,                    // 110
      LTT_RESETSCALE,                    // 1
      LTT_QTYCONFIRM,                    // 2
      LTT_CANCELITEMS,                    // 3
      LTT_CANCELNOMATCH,                // 4
      LTT_CANCELCONFIRM,                // 5
      LTT_CANCELALL,                    // 6
      LTT_SETALLASIDE,                    // 7
      LTT_WAITFORAPP,                    // 8
      LTT_STOREABORT,                    // 9
      LTT_ITEMRESHELF1,                    // 120
      LTT_ITEMRESHELF2,                    // 1
      LTT_READYTOPAY,                    // 2        // NOT USED - TAR# 247554
      LTT_SELAPPTYPE,                    // 3        // NOT USED - TAR# 247554
      LTT_PURCHASED,                    // 4        // NOT USED - TAR# 247554
      LTT_BIRTHDATE,                    // 5
      LTT_MMDDYY,                        // 6
      LTT_ALLAPPROVED,                    // 7        // NOT USED - TAR# 247554
      LTT_NOTAPPROVED,                    // 8        // NOT USED - TAR# 247554
      LTT_CANCELEDITEMS,                // 9
      MSG_CPNDEPOSIT,                    // 130
      LTT_CPNDEPOSIT,                    // 1
      LTT_CASHONLY,                        // 2        // NOT USED - TAR# 247554
      LTT_CARDONLY,                        // 3        // NOT USED - TAR# 247554
      MSG_CARDPROCESSING,                // 4
      LTT_CANTSCANCPN,                    // 5        // NOT USED - TAR# 247554
      LTT_CASHINSERT,                    // 6
      LTT_TAKECHANGE,                    // 7
      LTT_TAKECARD,                        // 8
      LTT_TAKERECEIPT,                    // 9
      LTT_THANKYOU,                        // 140
      LTT_SELECTCARD,                    // 1
      LTT_CONFIRMDEBITAMT,                // 2
      LTT_SIGNCHARGE,                    // 3
      LTT_SIGNCONFIRM,                    // 4
      LTT_WAITFORHELP,                    // 5        // NOT USED - TAR# 247554
      LTT_SELPAYMENT,                    // 6
      LTT_CARDORACCT,                    // 7        // NOT USED - TAR# 247554
      LTT_EXPIRYDATE,                    // 8        // NOT USED - TAR# 247554
      LTT_PINENTER,                        // 9
      LTT_DEBITAMT,                        // 150
      LTT_TAKECASH,                          // 1
      LTT_QTYTOOHIGH,                    // 2        // NOT USED - TAR# 247554
      LTT_UTILITY,                        // 3        // NOT USED - TAR# 247554
      LTT_OUTOFSERVICE,                    // 4
      LTT_SYSTEMMSG1,                    // 5
      LTT_SYSTEMMSG2,                    // 6
      LTT_SYSTEMMSG3,                    // 7
      MSG_RESTWARN,                        // 8
      MSG_NOPRICE,                        // 9
      MSG_RECALLED,                        // 160
      MSG_ENTERQTY,                        // 1
      MSG_KEYBARCODE,                    // 2
      MSG_ENTERITEM,                    // 3        // NOT USED - TAR# 247554
      MSG_UKITEMKEYED,                    // 4
      MSG_UKITEMSCANNED,                // 5
      MSG_HELPONWAY,                    // 6
      MSG_CPNNOITEM,                    // 7
      MSG_COUPWARN,                        // 8
      MSG_CONTRANS,                        // 9
      MSG_RESETSCALE,                    // 170
      MSG_QTYCONFIRM,                    // 1
      MSG_CANCELSELECT,                    // 2
      MSG_UNWANTED,                        // 3        // NOT USED - TAR# 247554
      MSG_CANTCANCEL,                    // 4
      MSG_CANCELCONFIRM,                // 5
      MSG_TRANSABORT,                    // 6
      MSG_SETALLASIDE,                    // 7
      MSG_WAITFORAPP,                    // 8
      MSG_CHECKBASKET,                    // 9        // NOT USED - TAR# 247554
      MSG_CPNREQAPP,                    // 180
      MSG_CPNVALUE,                        // 1
      MSG_CPNTOOHIGH,                    // 2
      MSG_QTYTOOHIGH,                    // 3        // NOT USED - TAR# 247554
      MSG_CONFIRMDEBITAMT,                // 4
      MSG_SIGNCONFIRM,                    // 5
      MSG_CANTREADCARD,                    // 6        // NOT USED - TAR# 247554
      MSG_NOTALLOWED,                    // 7        // NOT USED - TAR# 247554
      CO_LOADING,                        // 8
      CO_LOADED,                        // 9
      TB_LOADING,                        // 190
      TB_LOADED,                        // 1
      HC_LOADING,                        // 2        // NOT USED - TAR# 247554
      HC_LOADED,                        // 3        // NOT USED - TAR# 247554
      DM_LOADING,                        // 4
      DM_LOADED,                        // 5
      PS_LOADING,                        // 6
      PS_LOADED,                        // 7
      UE_LOADING,                        // 8
      UE_LOADED,                        // 9
      TESTING_TB,                        // 200
      TEST_SIGNON,                        // 1
      TEST_SIGNOFF,                        // 2
      TERM_DISPLAY,                        // 3        // NOT USED - TAR# 247554
      COPY_TO_SERVER,                    // 4
      COPY_COMPLETE,                    // 5
      OPTIONS_LOAD,                        // 6
      TRACING_STOPPED,                    // 7        // NOT USED - TAR# 247554
      TRACING_STARTED,                    // 8        // NOT USED - TAR# 247554
      BTT_STOP,                            // 9
      BTT_RESTART,                        // 210        // NOT USED - TAR# 247554
      LTT_CASHPAYMENT,                    // 1
      TTL_CASHPAYMENT,                    // 2        // NOT USED - TAR# 247554
      TXT_CASHAMT,                        // 3        // NOT USED - TAR# 247554
      BTT_DEBITCHECKING,                // 4
      BTT_DEBITSAVINGS,                    // 5
      MSG_PTR_COVER_OPEN,                // 6
      MSG_PTR_EMPTY,                    // 7        // NOT USED - TAR# 247554
      MSG_PTR_NEAREMPTY,                // 8        // NOT USED - TAR# 247554
      MSG_PTR_ERR_COVER_OPEN,            // 9        // NOT USED - TAR# 247554
      MSG_PTR_ERR_EMPTY,                // 220
      MSG_PTR_ERR_FAULT,                // 1
      MSG_PTR_ERR_REQUEST_SEQUENCE,        // 2
      MSG_MSR_CAPBIN_FULL,                // 3
      MSG_MSR_CAPBIN_REMOVED,            // 4
      MSG_MSR_CAPBIN_INSERTED,            // 5
      MSG_MSR_DEVICE_INOPERABLE,        // 6
      MSG_MSR_INVALID_CARD_INSERTED,    // 7
      MSG_MSR_SHUTTER_JAMMED_CLOSE,        // 8
      MSG_MSR_CARD_CAPTURED,            // 9        // NOT USED - TAR# 247554
      MSG_BILL_STATUS_REJECTED, // 230        // NOT USED - TAR# 247554   //SR672 - Enhanced device error display - Begin
      MSG_BILL_STATUS_RESET,    // 1        // NOT USED - TAR# 247554
      MSG_BILL_STATUS_JAM,        // 2
      MSG_BILL_STATUS_FAILURE,    // 3
      MSG_BILL_STATUS_FULL,        // 4
      MSG_BILL_STATUS_NEARFULL, // 5        // NOT USED - TAR# 247554
      MSG_BILL_STATUS_TAMPER_IN,// 6        // NOT USED - TAR# 247554
      MSG_BILL_STATUS_TAMPER_OUT,// 7
      MSG_BILL_STATUS_CHEAT,    // 8                                    ////SR672 - Enhanced device error display - End
      MSG_STATUS_UNEXPECTED,            // 9
      MSG_ERROR_GENERALFAILURE,            // 240
      MSG_ERROR_UNEXPECTED,                // 1
      MSG_ERROR_MSR_CARD_JAM,            // 2
      MSG_ERROR_MSR_INVALID_CARD,        // 3
      MSG_ERROR_MSR_READING,            // 4        // NOT USED - TAR# 247554
      MSG_ERROR_MSR_DEVICE_INOPERABLE,    // 5
      
        //Coin Dispenser Status Text
      MSG_CC_STATUS_EMPTY,                // 6        // NOT USED - TAR# 247554
      MSG_CC_STATUS_NEAREMPTY,            // 7        // NOT USED - TAR# 247554
      MSG_CC_STATUS_EMPTYOK,            // 8        // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGEFULL,            // 9        // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGENEARFULL,        // 250        // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGEFULLOK,         // 1        // NOT USED - TAR# 247554
      MSG_CC_STATUS_JAM,                // 2
      MSG_CC_STATUS_JAMOK,                // 3        // NOT USED - TAR# 247554
      MSG_CC_STATUS_INOPERABLE,            // 4
      MSG_CC_STATUS_TAMPER,                // 5        // NOT USED - TAR# 247554

      UE_STOPPING,                        // 6
      HC_STOPPING,                        // 7        // NOT USED - TAR# 247554
      TB_STOPPING,                        // 8
      DM_STOPPING,                        // 9
      ALL_STOPPED,                        // 260        // NOT USED - TAR# 247554
      BTT_MORE,                            // 1        // NOT USED - TAR# 247554
      BTT_MASTERKEY,                    // 2        // NOT USED - TAR# 247554
      MSG_CANCELITEMCPN,                // 3
      SA_LOADING,                        // 4        // NOT USED - TAR# 247554
      SA_LOADED,                        // 5        // NOT USED - TAR# 247554
      SA_STOPPING,                        // 6
      BTT_SALOGS,                        // 7
      LTT_CARDMISREAD,                    // 8
      BTT_ASSISTMODE,                    // 9
      BTT_APPCOUPS,                        // 270
      TTL_COUPONITEMS,                    // 1
      LTT_COUPONITEMS,                    // 2
      MSG_DONOTSCAN,                    // 3
      MSG_DONOTCASH,                    // 4
      MSG_DONOTINSERTCARD,                // 5
      MSG_CPNNOTALLOWED,                // 6        // NOT USED - TAR# 247554
      BTT_REBOOT,                        // 7        // NOT USED - TAR# 247554
      TTL_SECURITYLOG,                    // 8
      LTT_SECURITYLOG,                    // 9
      BTT_CLEARSECLOG,                    // 280
      BTT_STOREAPP,                        // 1        // NOT USED - TAR# 247554
      TXT_ENTER_ID,                        // 2
      TXT_ENTER_PASSWORD,                // 3
      MSG_ERROR_SCAN_DATALOSS,            // 4
      MSG_ERROR_SCAN_BADTAGSUFFIX,        // 5
      MSG_ERROR_SCAN_BADTAGPREFIX,        // 6
      MSG_ERROR_SCAN_BADTAG,            // 7
      MSG_ERROR_SCAN_SERIAL,            // 8
      MSG_CC_STATUS_TRANSPORT_JAM_CASH, // 9        // NOT USED - TAR# 247554
      MSG_CC_STATUS_TRANSPORT_JAM_COIN, // 290        // NOT USED - TAR# 247554
      MSG_CC_STATUS_HOPPER_JAM,            // 1
      TXT_CANTSCANCOUPON,                // 2
      TXT_REQUESTFORHELP,                // 3
      TXT_ENTER_OPERATOR_PROMPT,        // 4 
      HC_NOTREADY,                        // 5        // NOT USED - TAR# 247554
      HC_BADTRACK,                        // 6        // NOT USED - TAR# 247554
      HC_BADACCOUNT,                    // 7        // NOT USED - TAR# 247554
      HC_BADEXPIRY,                        // 8        // NOT USED - TAR# 247554
      HC_BADPIN,                        // 9        // NOT USED - TAR# 247554
      HC_DECLINED,                        // 300        // NOT USED - TAR# 247554
      LTT_CPNVALUE,                        // 1
      MSG_PINENTER,                        // 2
      BTT_CANCELITEMS,                    // 3
      LTT_SELECTCONTAINER,                // 4
      MSG_SELECTCONTAINER,                // 5
      BTT_NOCONTAINER,                    // 6
      BTT_OTHERCONTAINER,                // 7
      MSG_COIN,                            // 8
      MSG_BILL,                            // 9
      MSG_BINISLOW,                        // 310
      MSG_BINISEMPTY,                    // 1
      MSG_BINISJAMMED,                    // 2
      MSG_BINISAJAR,                    // 3        // NOT USED - TAR# 247554
      MSG_BINPROBLEM,                    // 4
      MSG_VOID_RATIO_ERROR,                // 5        // NOT USED - TAR# 247554
      MSG_NORMAL_COUPON_RATIO_ERROR,    // 6        // NOT USED - TAR# 247554
      MSG_HIGH_RISK_COUPON_RATIO_ERROR, //7        // NOT USED - TAR# 247554
      MSG_TOTAL_COUPON_RATIO_ERROR,        // 8        // NOT USED - TAR# 247554
      BTT_PAYWITHCARD,                    // 9
      BTT_GOBACK,                        // 320
      TXT_TRXVERIFICATION,                // 1
      BTT_PRODUCE,                        // 2        // NOT USED - TAR# 247554
      TXT_DEALQTY,                        // 3
      BTT_BIRTHDATENOTREQUIRED,            // 4
      MSG_DATEINVALID,                    // 5
      MSG_UNDER_RESTRICTED_AGE_LIMIT,     // 6
      MSG_DEVICESERROR,                    // 7
      MSG_CHANGEERROR,                    // 8
      TXT_WGTDEAL,                        // 9
      LTT_ASSISTMODE,                    // 330
      LTT_ASSISTMODESECURITYWAIT,        // 1
      LTT_ASSISTMODERETURN,                // 2
      MSG_ERROR_MSR_SHUTTER_JAM,        // 3
      MSG_ERROR_MSR_NO_CARD,            // 4
      MSG_MSR_POSSIBLE_CUST_TAMPERING,    // 5
      BTT_LOOKUPITEM,                    // 6
      MSG_SCANORKEYIN,                    // 7
      MSG_TYPEITEMDESC,                    // 8
      TXT_ITEMDESC,                        // 9        // NOT USED - TAR# 247554
      TXT_SELECTFROMLIST1,                // 340
      BTT_SELECTBYCATEGORY,                // 1        // NOT USED - TAR# 247554
      TXT_TYPEITEMSDESC,                // 2
      TXT_DESCRIPTION,                    // 3        // NOT USED - TAR# 247554
      TXT_SELECTFROMLIST2,                // 4        // NOT USED - TAR# 247554
      TXT_FINISHANDPAY,                    // 5        // NOT USED - TAR# 247554
      BTT_DEBITWITHCASHBACK,            // 6
      MSG_VOID_RESTRICTED_ALL,            // 7
      MSG_VOID_RESTRICTED_ALCOHOL,        // 8        // NOT USED - TAR# 247554
      MSG_VOID_RESTRICTED_TOBACCO,        // 9        // NOT USED - TAR# 247554
      BTT_LANESTATUS,                    // 350
      TTL_MEDIASTATUS,                    // 1
      BTT_MEDIAUPDATE,                    // 2
      BTT_MEDIAEXIT,                    // 3
      TTL_SYSTEMFUNCTIONS,                // 4
      LTT_SYSFUNCSELECT,                // 5
      BTT_ENCRYPTOR,                    // 6
      BTT_OUTOFSERVICE,                    // 7        // NOT USED - TAR# 247554
      BTT_RETURNASSIST,                    // 8        // NOT USED - TAR# 247554
      BTT_SYSTEMFUNCTIONS,                // 9
      BTT_CLOSELANE,                    // 360
      LTT_LANECLOSED,                    // 1
      TTL_ENTERPASSWORD,                // 2
      MSG_ENTERPASSWORD,                // 3
      MSG_SIGNCHARGE,                    // 4        // NOT USED - TAR# 247554
      MSG_PTR_ERR_HEAD_JAM,                // 5
      MSG_PTR_ERR_KNIFE_JAM,            // 6
      MSG_PTR_ERR_PAPER_JAM,            // 7        // NOT USED - TAR# 247554
      MSG_PTR_ERR_PAPER_NOT_LOADED_JAM, // 8
      MSG_PTR_ERR_COMMS_ERROR,            // 9
      MSG_PTR_ERR_PRINTER_OPEN,            // 370
      MSG_PTR_ERR_REPLACE_HEAD,            // 1
      MSG_PTR_ERR_SIDEWAYS_COMMS_ERROR, // 2
      MSG_PTR_ERR_EJECT_JAM,            // 3
      MSG_PTR_ERR_BLACKMARK_ERROR,        // 4
      MSG_PTR_ERR_TIME_OUT,                // 5
      TXT_CHECKING_HEALTH,                // 6        // NOT USED - TAR# 247554
      TXT_UNITS_STATUS,                    // 7
      TXT_RECEIPT_PRINTER,                // 8
      TXT_CASH_ACCEPTOR,                // 9
      TXT_CASH_CHANGER,                    // 380        // NOT USED - TAR# 247554
      TXT_NORMAL,                        // 1
      TXT_UNKNOWN,                        // 2
      TXT_LOW,                            // 3        // NOT USED - TAR# 247554
      TXT_EMPTY,                        // 4        // NOT USED - TAR# 247554
      TXT_HOPPER_JAM,                    // 5        // NOT USED - TAR# 247554
      TXT_CASSETTE_OUT,                    // 6
      TXT_TRANSPORT_JAM,                // 7
      LTT_CASHPROCESSING,                // 8
      LTT_CARDPROCESSING,                // 9
      TXT_RECALLEDITEMS,                // 390
      TXT_PRICENEEDEDITEMS,                // 1
      TXT_UNKNOWNITEMS,                    // 2
      TXT_JOURNAL_PRINTER,                // 3
      MSG_CASH_ACCEPTOR_CASSETTE_OUT,    // 4        // NOT USED - TAR# 247554
      TXT_RESTRICTEDNOTALLOWEDITEMS,    // 5
      MSG_SETASIDEAPPROVALS1,            // 6    
      MSG_SETASIDEAPPROVALS2,            // 7
      TXT_CASSETTE_NORMAL,              // 8
      MSG_UNDER_ALCOHOL_AGE_LIMIT,      // 9        // NOT USED - TAR# 247554
      MSG_UNDER_TOBACCO_AGE_LIMIT,      // 400        // NOT USED - TAR# 247554
      TXT_FINISHANDPAY_CARD_ONLY,       // 1
      TXT_NOT_PRESENT,                  // 2
      LTT_CPNNOTALLOWED,                // 3        // NOT USED - TAR# 247554
      LTT_CARDONLYSWIPE,                // 4        // NOT USED - TAR# 247554
      MSG_CANTREADCARDSWIPE,              // 5
      MSG_CANTREADCARDNOCASH,              // 6        // NOT USED - TAR# 247554
      MSG_CANTREADCARDSWIPENOCASH,        // 7
      TXT_FINISHANDPAY_CARDSWIPE_ONLY,  // 8        // NOT USED - TAR# 247554
      TXT_SYSTEMMESSAGE,                // 9
      MSG_WAITFORMESSAGE,               // 410
      LTT_TAKELOYALTYCARD,              // 1 =Take your Loyalty Card    
      MSG_ITEMCAUSEDTRXLIMIT,           // 2 =This item will cause your purchase to exceed...
      MSG_LC_AUTO_DISCOUNT,             // 3 =Take your loyalty card, discounts are automatic now!!    
      MSG_LC_MAIL_DISCOUNT,             // 4 =Take your loyalty card, discounts will be mailed to you!!    
      MSG_LC_INSERTED_BEFORE,           // 5 =Take your loyalty card, saved card information when it was inserted before 
      HT_LOADING,                       // 6 = Hard Totals loading message
      HT_LOADED,                        // 7 = Hard Totals loaded message
      HT_STOPPING,                      // 8 = Hard Totals stopping message
      BTT_ATM,                            // 9 = ATM        // NOT USED - TAR# 247554
      BTT_PURCHASE,                        //420= Purchase        // NOT USED - TAR# 247554
      MSG_ATTRACT,                        // 1 = Insert loyalty card or ....
      LTT_INSERTCARD,                    // 2 = Insert Card        // NOT USED - TAR# 247554
      MSG_INSERTCARD,                    // 3 = Please insert your card
      LTT_CASHAMOUNT,                    // 4 = Enter amount
      TXT_CASHAMOUNT,                    // 5 = Enter amount\n in multiples of ...        // NOT USED - TAR# 247554
      MSG_LANE_NUMBER,                    // 6 = Lane Format for Pager Messages
      MSG_MEDIA_LOW,                    // 7 = Media Low Tri-light Pager Message
      MSG_YELLOW,                        // 8 = Yellow Tri-light Pager Message
      MSG_RED,                            // 9 = Flashing Red Tri-light Pager Message
      MSG_OUTOFSERVICE,                 // 430= "Out of Service" Solid Red Pager Message
      MSG_LC_INVALID,                    // 1 = Loyalty card invalid.\nPlease verify.

      BTT_EBT,                            // 2        "EBT"                        EBT Support
      BTT_CASHBENEFIT,                    // 3        "Cash Benefit"    EBT Support        // NOT USED - TAR# 247554
      LTT_EBTTRANSFER,                    // 4    "Electronic benefits transfer"
      MSG_EBTTYPE,                        // 5        "Select the benefit type..."
      LTT_CONFIRMEBTCBAMT,                // 6        "Confirm cash benefit amount"
      LTT_FOODSTAMPLIMIT,                // 7    EBT Support EBT support
      MSG_FOODSTAMPLIMIT,                // 8
      LTT_CASHBENEFITLIMIT,                // 9        // NOT USED - TAR# 247554
      MSG_CASHBENEFITLIMIT,                // 440
      LTT_CONFIRMEBTFSAMT,                // 1        "Confirm food stamps"
      MSG_CONFIRMEBTFSAMT,                // 2
      MSG_CONFIRMEBTCBAMT,                // 3
      BTT_FOOD_STAMPS,                    // 4
      BTT_EBTCASH,                        // 5
      //Begin_Suspend
      BTT_SUSPEND,                        // 6
      //End_Suspend
      MSG_SCANNEDLOYALTY_NTHTIME,        // 7    "Your rewards are on the house. Scanned this card already."
      MSG_SCANNEDLOYALTY_FIRSTTIME,        // 8    "Scanned loyalty card. Your rewards are on the house."

      // Begin Coin Acceptor Modifications
      MSG_COINACCEPTOR_STATUS_REJECTED, // 9        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_RESET,    // 450        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_JAM,      // 1
      MSG_COINACCEPTOR_STATUS_FAILURE,  // 2
      MSG_COINACCEPTOR_STATUS_FULL,     // 3
      MSG_COINACCEPTOR_STATUS_NEARFULL, // 4        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_TAMPER_IN,// 5        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_TAMPER_OUT, // 6        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_CHEAT,    // 7        // NOT USED - TAR# 247554
      // End Coin Acceptor Modifications
      MSG_EBTNOFSTYPE,                  // 8
      MSG_BALANCEREMAINS,                // 9
      MSG_NOBALANCE,                    // 460

      // Begin Remote Approval Modifications
      RA_LOADING,                       //    1
      RA_LOADED,                        //    2
      RA_STOPPING,                      //    3
      RA_NORMAL_ITEM_FMT_ID,            //    4
      RA_VOIDED_ID,                     //    5            
      RA_SCANNED_UNKNOWN_ID,            //    6
      RA_KEYED_UNKNOWN_ID,              //    7        // NOT USED - TAR# 247554
      RA_RECALLED_ID,                   //    8
      RA_PRICE_UNKNOWN_ID,              //    9        // NOT USED - TAR# 247554
      RA_AGE_RESTRICTED_ID,             //  470
      RA_VISUAL_VALIDATE_ID,            //    1


      RA_CALL_FOR_HELP_ID,              //    2
      RA_MEDIA_LOW_ID,                  //    3
      RA_OUT_OF_SERVICE_ID,             //    4        // NOT USED - TAR# 247554
      RA_TRANS_SUSPEND_ID,              //    5
      RA_TRANS_DROPOFF_ID,              //    6
      RA_DROPOFF_ID,                    //    7
      LTT_DROPOFFCOUPONS,               //    8
      TXT_DROPOFFCOUPONS,               //    9
      RA_SUSPEND_ID,                    //  480   // instructional message 
      LTT_SUSPEND,                      //    1   // lead through
      TXT_SUSPEND,                      //    2   // approval        // NOT USED - TAR# 247554
      // End Remote Approval Modifications

      MSG_SIGN_SIGNATURE,               //    3
      LTT_REQSIG,                        //    4
      LTT_ACKSIG,                        //    5
      BTT_UP,                           //    6  
      BTT_DOWN,                         //    7
      MSG_PLACEITEMONSCALE,             //    8
      MSG_THANKYOUFORUSINGSCOT,         //    9
      MSG_ATTRACT_NO_LIMIT,                //  490
      MSG_TAKECARD,                     //    1
      MSG_TAKECHANGE,                   //    2  
      MSG_TAKERECEIPT,                  //    3
      MSG_DEBITAMT,                     //    4                     
      MSG_ITEM_QUANTITY_EXCEEDED,        //  495  -- this is for a per item limit exceeded
      LTT_ITEM_QUANTITY_EXCEEDED,       //  496  -- this is for a per item limit exceeded
      TXT_AMOUNTDUE,                    //    7    
      TXT_AMOUNTPAID,                   //    8 
      MSG_SELECTCONTAINERLIST,          //    9        // NOT USED - TAR# 247554
      BTT_PLASTICBAG,                   //  500        // NOT USED - TAR# 247554
      MSG_SIGNRECEIPT,                  //  501
      MSG_SETCANCELEDASIDE,             //  502
      LTT_INCORRECTPASSWORD,            //  503        // NOT USED - TAR# 247554
      MSG_INCORRECTPASSWORD,            //  504        // NOT USED - TAR# 247554
      LTT_INCORRECTID,                  //  505 
      MSG_INCORRECTID,                  //  506         // NOT USED - TAR# 247554
      TXT_VISUALITEMS,                    //  507
      LTT_VISUALITEM,                    //  508        // NOT USED - TAR# 247554
      MSG_VISUALITEM,                    //  509        // NOT USED - TAR# 247554
      BTT_SIGNAGAIN,                    //  510
      MSG_SIGSIGNCONFIRM,                //  511
      TTL_VISUALITEMS,                    //  512
      LTT_VISUALITEMS,                    //  513        // NOT USED - TAR# 247554
      BTT_APPVISUAL,                      // 514        // NOT USED - TAR# 247554
      TXT_FINISHANDPAY_CARDSWIPE_NOCASH,  // 515        // NOT USED - TAR# 247554
      BTT_OTHERPAYMENT,                   // 516
      TXT_OTHERPAYMENT,                   // 517
      MSG_CASHINSERT,                     // 518
      LTT_CPNTOOHIGH,                     // 519
      LTT_UNKNOWNITEMCODE,                // 520
      TXT_AMOUNTREFUNDED,                 // 521
      TTL_WRONG_PIN,                      // 522
      TTL_CARD_DECLINED,                  // 523
      MSG_REENTER_PIN,                    // 524
      MSG_INSERT_DIFF_CARD,               // 525
      MSG_SLIDE_DIFF_CARD,                // 526
      MSG_DECLINED_REINSERT,              // 527        // NOT USED - TAR# 247554
      MSG_DECLINED_RESLIDE,               // 528        // NOT USED - TAR# 247554
      MSG_MISREAD_MCRW,                   // 529
      MSG_MISREAD_SLIDE,                  // 530
      MSG_SIGCAP_DEVICE_FAILURE,          // 531    
      LTT_RESTRICTEDITEM,                 // 532
      MSG_TIMERESTRICTEDITEM,             // 533
      TXT_TIME_RESTRICTED_ITEM,           // 534
      RA_TIME_RESTRICTED_ITEM_ID,         // 535
      TXT_COIN_ACCEPTOR,                  // 536
      MSG_COINACCEPTOR_STATUS_RETURNED,   // 537        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_CLOSED,     // 538        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_INOPERABLE, // 539        // NOT USED - TAR# 247554
      MSG_COINACCEPTOR_STATUS_ESCROW_FULL,// 540        // NOT USED - TAR# 247554
      BTT_BARCODE,                        // 541
      MSG_ERR_NO_BARCODE,                 // 542
      MSG_ERR_BARCODE_ID_LENGTH,          // 543
      MSG_ERR_BARCODE_PSWD_LENGTH,        // 544
      LTT_RESTRICTEDNOTALLOW,             // 545
      TXT_RESTRICTEDNOTALLOW,             // 546   
      MSG_OPERATORPASSWORDINVALID,        // 547
      TXT_FINISHANDPAY_SWIPE_LOYAL,       // 548        // NOT USED - TAR# 247554
      TXT_FINISHPAY_LOYAL,                // 549        // NOT USED - TAR# 247554
      TXT_FINISHCARD_LOYAL,               // 550        // NOT USED - TAR# 247554
      TXT_FINISHSWIPE_LOYAL,              // 551        // NOT USED - TAR# 247554
      MSG_ZERO_LEADING_OP_PASSWORD,       // 552
      MSG_SETCPNASIDE,                      // 553    TAR# 102019
      MSG_DONOTSWIPECARD,                 // 554
      RA_OTHER_PAYMENT_ID,                  // 555
      TXT_SUSPEND_INCOMPLETED,            // 556
      TXT_QUANTITYRESTRICTEDITEMS,        // 557    TAR#126778 Require approval for quantity restricted items
      RA_QUANTITYRESTRICTED_ID,              // 558
      LTT_COUPONNOTACCEPTED,              // 559    TAR# 102019
      MSG_COUPONNOTACCEPTED,              // 560    TAR# 102019
      LTT_SETASIDECOUPON,                  //   1    TAR# 102019
      TXT_COUPONSONLYINASSIST,              //   2    102019
      MSG_HARDTOTALS_ERROR,               //   3    126490
      BTT_PRINT_SIGNATURE,                //   4        // NOT USED - TAR# 247554
      BTT_STORELOGIN,                     // 565    Changes for 2.0
      BTT_BACKSPACE,                      // 566          "        // NOT USED - TAR# 247554
      BTT_ENTER,                          // 567          " 
      BTT_CLEAR,                          // 568          "
      LTT_KEYINCODE,                      // 569          " 
      NOT_USED_YET,                       // 570         This is available !!!!!        // NOT USED - TAR# 247554
      MSG_ENTERQUANTITY,                  // 571
      MSG_KEYINCODE,                      // 572 
      MSG_ENTERID_PRESSENTER,             // 573
      MSG_ENTERPASSWORD_PRESSENTER,       // 574 
      RA_ENDCASHTENDER_BALANCEDUE,                   // 575
      RA_BEGINCARDTENDER,                 // 576
      RA_BEGINCASHTENDER,                 // 577
      RA_ENDCARDTENDER_BALANCEDUE,        // 578
      RA_LANE_SUSPENDED,                  // 579        // NOT USED - TAR# 247554
      RA_ATTEMPTED_VOID_OF_ITEM_WITH_COUPON,   // 580      
      RA_SUSPEND_DECLINED,                // 581
      RA_TENDER_COMPLETE,                 // 582
      RA_CARD_DECLINED,                   // 583
      RA_EBT_DECLINED,                    // 584
      RA_COUPONS_ONLY_IN_ASSISTMODE,      // 585        // NOT USED - TAR# 247554
      RA_COUPON_AMOUNT,                   // 586
      TXT_SIGNATURE_APPROVAL_NEEDED,      // 587        

      // Begin SCOT 2.0 new devices Modifications
      DF_LOADING,                       //    588
      DF_LOADED,                        //    589
      DF_STOPPING,                      //    590
      SE_LOADING,                       //    591
      SE_LOADED,                        //    592
      SC_LOADING,                       //    593
      SC_LOADED,                        //    594
      AD_LOADING,                       //    595
      AD_LOADED,                        //    596
      SF_LOADING,                       //    597
      SF_LOAD_FAIL,                     //    598
      SF_RESET_DEVICE,                  //    599

      //Begin Security Maintenance Mode additions
      BTT_SECURITYMAINTENANCE,            // 600
      LTT_SMMSELECT_MAINT_FUNCTION,        // 601        // NOT USED - TAR# 247554
      BTT_SMMEDITADD,                    // 602
      LTL_SMM_EDITADD,                    //     3            
      LTT_SMMEDITADD_INSTR,                //     4
      BTT_SMM_ADDWT,                    //     5
      BTT_SMM_DELWT,                    //     6
      BTT_SMM_EDIT_WTEXP,                //     7
      BTT_SMM_EDIT_WTSUBCK,                //     8
      BTT_SMM_EDIT_WTTOL,                //     9
      TXT_SMM_EDITADD_PROMPT,            // 610
      TXT_SMM_EDITADD_INSTR,            //    11
      LTL_SMM_ITEMWT_TITLE,                //    12
      TXT_SMM_ITEMWT_INSTR,                //    13
      LTL_SMM_WTEXP_TITLE,                //    14
      TXT_SMM_WTEXP_INSTR,                //    15
      LTL_SMM_WTSUBCK_TITLE,            //    16
      TXT_SMM_WTSUBCK_INSTR,            //    17
      LTL_SMM_WTTOL_TITLE,                //    18
      TXT_SMM_WTTOL_INSTR,                //    19
      BTT_SMM_WTTOL_KEYIN,                // 620
      BTT_SMM_DEFAULT,                    //    21
      LTL_SMM_KEYINWTTOL_TITLE,            //    22
      TXT_SMM_KEYINWTTOL_INSTR,            //    23
      LTL_SMM_INVALID_WTTOL,            //    24
      TXT_SMM_INVALID_WTTOL_INSTR,        //    25
      TXT_SMM_INVALID_WTTOL_PROMPT,        //    26
      //end SMM
        
      // messages for SigCap receipt Printing(SCP)
      MSG_SCP_TRANSACTIONNUM,            //  27
      MSG_SCP_LANENUM,                    //  28
      MSG_SCP_TOTAL_CHARGE,                //  29
      MSG_SCP_CREDIT_CARDNUM,            // 630
      MSG_SCP_OWNER_NAME,                //  31
      MSG_SCP_APPROVAL_CADE,            //  32        // NOT USED - TAR# 247554
      MSG_SCP_SIGNATURE,                //  33
      MSG_SCP_THANKS,                    //  34        // NOT USED - TAR# 247554
      BTT_KEYINBIRTHDATE,                //    35

      // Security Maintenance Mode info messages
      MSG_SMM_OFFLINE,                    //  36
      MSG_SMM_OFFLINE_CANCELLED,        //     37
      MSG_SMM_TOLCAPTION,                //    38
      MSG_SMM_TOLCOLUMNS,                //    39
      MSG_SMM_WLDBCAPTION,                // 640
      MSG_SMM_WLDBCOLUMNS,                //     41
      MSG_SMM_ITEMADDED,                //    42
      MSG_SMM_ITEMDELETED,                //    43
      MSG_SMM_SEPARATOR,                //    44
      MSG_SMM_DUPWEIGHT,                //    45
      MSG_SMM_Y,                        //    46
      MSG_SMM_N,                        //    47
      MSG_SMM_UPCLABEL,                    //    48
      MSG_SMM_PPWULABEL,                //    49
      MSG_SMM_REGULARLABEL,                // 650
      MSG_SMM_DEFAULTLBL,                //    51
      MSG_SMM_NOENTRIES,                //    52
      
      MSG_KEYCPNVALUE,                  //    53
      MSG_DATE_FORMAT_INVALID,            //    54
      LTT_EXCEPTIONITEM,                // 655
      MSG_SETASIDEEXCEPTION,            // 656
      MSG_REFUNDORDISCOUNT,             // 657
      MSG_CURRENCYISLOW,                // 658
      MSG_CURRENCYLOWSTATUS,            // 659
      MSG_CURRENCYALLNORMAL,            // 660
      MSG_ABBREV_DEF,                    //    61
      MSG_PERCENTSIGN,                    //    62
      MSG_ENCRYPTORSETUPFAIL,           //  63
      MSG_ATTRACT_DUAL_LANGUAGE,        //  64
      MSG_CANCELALLITEMS,               //  65
      MSG_SELECTLANGUAGE,               //  66        // NOT USED - TAR# 247554
      MSG_CARDSWIPED_SCANITEMS,         //  67
      LTT_PRINT_RECEIPT_SUCCESS,        //  68
      LTT_PRINT_RECEIPT_FAILED,         //  69
      MSG_ENTER_TRAINING_MODE,            // 670
      MSG_LEAVE_TRAINING_MODE,            //  71
      MSG_TRAINING_MODE_LINE,            //  72
      MSG_NEGATIVE_BALANCE,                //  73
      MSG_FALSE_TENDER_REASON,            //  74
      BTT_FILLCOINDISPENSER,            //  75
      MSG_FILLCOINDISPENSER,            //  76
      MSG_RUNREPORTS,                   //  77   used on Reports screen        // NOT USED - TAR# 247554
      MSG_SALESREPORTS,                 //  78   used on Reports screen        // NOT USED - TAR# 247554
      MSG_ASSISTANCEREPORTS,            //  79   used on Reports screen
      BTT_OTHERITEMS,                   // 680        // NOT USED - TAR# 247554
      LTT_PRODUCEFAVORITES,             // 681
      MSG_TERMINAL,                     //  82   used in Reports         // NOT USED - TAR# 247554
      MSG_SUMMARYREPORT,                //  83   used in Reports         // NOT USED - TAR# 247554
      BTT_TODAY,                        //  84   used on Reports screen
      BTT_HOURLY,                       //  85   used on Reports screen
      BTT_YESTERDAY,                    //  86   used on Reports screen
      BTT_PRINTREPORT,                  //  87   used on Reports screen
      MSG_TOTALAMOUNTCOLLECTED,         //  88   used in Reports        // NOT USED - TAR# 247554
      MSG_SALES,                        //  89   used in Reports
      MSG_CANCELLED,                    // 690   used in Reports         // NOT USED - TAR# 247554
      MSG_SUSPENDED,                    //  91   used in Reports        // NOT USED - TAR# 247554
      MSG_AVERAGETRANSACTIONTIME,       //  92   used in Reports        // NOT USED - TAR# 247554
      MSG_AVERAGESALESTOTAL,            //  93   used in Reports        // NOT USED - TAR# 247554
      MSG_ASSISTED,                     //  94   used in Reports        // NOT USED - TAR# 247554
      MSG_TOTALCASHDISPENSED,           //  95   used in Reports        // NOT USED - TAR# 247554
      MSG_HOURLYSUMMARY,                //  96   used in Reports        // NOT USED - TAR# 247554
      MSG_TOTALNUMBERTRANSACTIONS,      //  97   used in Reports        // NOT USED - TAR# 247554
      MSG_TIMEGENERATED,                //  98   used in Reports        // NOT USED - TAR# 247554
      LTT_UNSELLABLEITEM,               //  99
      MSG_UNSELLABLEITEM,               // 700
      MSG_INSERTCASHPAYWITHCARD,        //   1        // NOT USED - TAR# 247554
      MSG_SELECTOPTIONTOCONTINUE,       //   2        // NOT USED - TAR# 247554
      MSG_PINPADENTRY,                  //   3 
      LTT_PINPADENTRY,                  //   4
      MSG_INSUFFICIENTCASHBENEFIT,      //   5
      MSG_INSUFFICIENTFOODSTAMP,        //   6  
      MSG_PREVIOUS7DAYS,                //   7    used on Reports screen
      MSG_ASSISTSUMMARYREPORT,          //   8    used in Reports        // NOT USED - TAR# 247554
      MSG_AVERAGE_ITEMS,                //   9    used in Reports        // NOT USED - TAR# 247554
      MSG_RP_RESTRICTEDITEM,            // 710      used in Assistance Report                    // NOT USED - TAR# 247554
      MSG_RP_VOIDRATIO,                    //   1    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_COUPONRATIO,                //   2      used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_SECURITY,                     //   3    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_NOTFORSALE,                //   4      used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_UNKNOWNITEM,                //   5      used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_PRICEREQUIRED,                //   6    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_RESTRICTEDNOTALLOWED,      //   7    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_SETASIDECOUPON,            //   8      used in Assistance Report         // NOT USED - TAR# 247554
      MSG_RP_QUANTITYEXCEEDED,            //   9    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_CANTSCANCOUPON,            // 720    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_HELP,                        //   1    used in Assistance Report         // NOT USED - TAR# 247554
      MSG_RP_TIMERESTRICTED,            //   2    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_OTHERPAYMENT,              //   3    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_DROPOFFREQUIRED,           //   4    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_SUSPENDREQUESTED,          //   5    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_VISUALVERIFY,              //   6    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_NETSALESTOTAL,                //   7    used in Reports        // NOT USED - TAR# 247554
      MSG_TOTAL_STORELOGINS,            //   8    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_REASONLABEL,                  //   9    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_ASSISTHOURLYREPORT,           // 730    used in Reports        // NOT USED - TAR# 247554
      MSG_SUMMARY,                      //   1    used in Reports
      MSG_REPORTDATEFORMAT,             //   2    used in Reports        // NOT USED - TAR# 247554
      MSG_CARDDECLINED_PIP,             //   3
      MSG_ASSISTEDWITHPERCENT,          //   4    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_SETASIDECOINVOUCHER,       //   5    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_GIFTCERTIFICATE,           //   6    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_USERDEFINED1,              //   7    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_USERDEFINED2,              //   8    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_USERDEFINED3,              //   9    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_USERDEFINED4,              // 740    used in Assistance Report        // NOT USED - TAR# 247554
      MSG_RP_TENDERNONE,                //   1    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERCASH,                //   2    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERCREDIT,              //   3    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERDEBIT,               //   4    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERLOYALTY,             //   5    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERDEBITSAVING,         //   6    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDERDEBITCHECKING,       //   7    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TENDEREBT,                 //   8    used in Report Logging        // NOT USED - TAR# 247554

      MSG_RP_TENDERFOODSTAMPS,          //   9    used in Report Logging        // NOT USED - TAR# 247554
      MSG_RP_TODAYSUMMARY,              // 750    used on Reports screen
      MSG_RP_TODAYHOURLY,               //   1    used on Reports screen
      MSG_RP_YESTERDAYSUMMARY,          //   2    used on Reports screen
      MSG_RP_YESTERDAYHOURLY,           //   3    used on Reports screen
      MSG_RP_PREVIOUS7DAYSSUMMARY,      //   4    used on Reports screen
      MSG_RP_PROCESSING,                //   5    used while generate reports.
      LTL_THEFT_SENSOR_TITLE,            //     6    EAS    Deactivation Screen title        // NOT USED - TAR# 247554
      LTT_THEFT_SENSOR_MSG,                //     7    EAS Deactivation Screen instructions        // NOT USED - TAR# 247554
      BTT_LARGE_ITEM,                    //     8    EAS Deactivation Screen button        // NOT USED - TAR# 247554
      LTT_SLIDEONPAYMENTDEVICE,         //   9  used on Slide Card screen
      LTT_SLIDEONSIDEOFMONITOR,         // 760 used on Slide Card screen

      // new devices for electronic article surveillance (Sensormatic Option) LPM092100
      MS_LOADING,                       // 761    motion sensor for sensormatic        // NOT USED - TAR# 247554
      MS_LOADED,                        //     2        // NOT USED - TAR# 247554
      ED_LOADING,                       //     3    EAS Deactivator for sensormatic        // NOT USED - TAR# 247554
      ED_LOADED,                        //     4        // NOT USED - TAR# 247554

      MSG_RP_TENDERGIFTCERTIFICATE,     //   5        // NOT USED - TAR# 247554
      MSG_CONFIRMCREDITCASHBACK,        //   6
      RA_MEDIA_NORMAL_ID,               //   7    // TAR144557 & 152537 RAP not updating medio low status as fixed
      TTL_SUSPENDCONFIRMATION,            //   8
      LTT_SUSPENDCONFIRMORCANCEL,        //   9
      BTT_CONFIRMSUSPEND,                // 770
      BTT_CANCELSUSPEND,                //   1
      MSG_SUSPENDCONFIRMATION,            //   2
      MSG_TOTALSAVINGS,                    //   3
      MSG_TOTALREWARDS,                    //   4
      MSG_DEFAULTCUSTOMER_MESSAGE,        //   5

      //Cash Dispenser Status Text
      MSG_CC_STATUS_EMPTY_EX,            //   6          // NOT USED - TAR# 247554
      MSG_CC_STATUS_NEAREMPTY_EX,        //   7          // NOT USED - TAR# 247554
      MSG_CC_STATUS_EMPTYOK_EX,            //   8          // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGEFULL_EX,        //   9          // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGENEARFULL_EX,    // 780          // NOT USED - TAR# 247554
      MSG_CC_STATUS_PURGEFULLOK_EX,        //   1          // NOT USED - TAR# 247554
      MSG_CC_STATUS_JAM_EX,                //   2  
      MSG_CC_STATUS_JAMOK_EX,            //   3          // NOT USED - TAR# 247554
      MSG_CC_STATUS_INOPERABLE_EX,        //   4  
      MSG_CC_STATUS_TAMPER_EX,            //   5          // NOT USED - TAR# 247554
      RA_SUSPENDEDATLANE,                //   6
      RA_BIRTHDATEENTER,                //   7
      RA_VALIDBIRTHDAYREQUIRED,            //   8
      MSG_SLIDECARDCHECKBASKET,         //   9
      MSG_CARDDECLINEDSELECTPAYMENT,    // 790
      TTL_SELECTPAYMENTTYPE,            //   1    Title text for Finalization screen
      MSG_CC_STATUS_TRANSPORT_JAM_CASH_EX, //792         // NOT USED - TAR# 247554
      MSG_CC_STATUS_TRANSPORT_JAM_COIN_EX, //793        // NOT USED - TAR# 247554
      MSG_CC_STATUS_HOPPER_JAM_EX,        // 794
      MSG_VIDEOATATTRACT,                // 795
      MSG_COINCHANGEERROR,              // 796   
      MSG_BILLCHANGEERROR,              // 797
      BTT_LASTRECEIPT,                  // 798  //for Print Last Receipt module        // NOT USED - TAR# 247554
      TTL_CHECKYOURCART,                // 799
      MSG_CHECKYOURCART,                // 800
      TTL_PURCHASECRATE,                //  01
      LTT_PURCHASEFULLCRATE,            //  02
      BTT_SIGNATURES,                   //803        // NOT USED - TAR# 247554
      MSG_PRINTRECORDS,                 //804        // NOT USED - TAR# 247554
      BTT_VISUALITEMS,                    //805
      LTT_VERIFYITEMS,                    //806
      TTL_SECURITYTAG,                    //807
      MSG_SECURITYTAG,                    //808
      RA_POSMESSAGE,                    //809
      MSG_TRAININGMODE,                    //810
      MSG_SCANGIFTCARD,                 //811
      MSG_THANKYOUFIGTCARD,             //812
      MSG_TYPEJOBINFORMATION,            //813
      TXT_TYPEJOBINFORMATION,            //814        // NOT USED - TAR# 247554
      BTT_OPENREGISTER,              //815        // NOT USED - TAR# 247554
      BTT_CLOSEREGISTER,             //816        // NOT USED - TAR# 247554
      LTT_WAITFORASSISTANCE,         //817
      MSG_WAITFORASSISTANCE,         //818
      TXT_RA_VOIDED_ITEM_SUSP,         //819
      BTT_APPROVE_VOIDED_ITEM,         //820
      MSG_APPROVE_VOIDED_ITEM,         //821        // NOT USED - TAR# 247554
      LTT_APPROVE_VOIDED_ITEM,         //822
      TXT_VOIDEDITEMAPPROVAL,          //823
      TXT_SUSPENDCONFIRMATION,         //824
      MSG_BAGITEM,                        //825 T.I.D
      BTT_SKIP_BAGGING,                //826 T.I.D
      LTT_BAGGING_AREA,                    //827 T.I.D
      MSG_BAGITEMEAS,                    //828 T.I.D        // NOT USED - TAR# 247554
      LTT_UNPURCHASED_ITEM,                //829.I.D        // NOT USED - TAR# 247554
      BTT_ITEM_REMOVED,                //830 T.I.I.D
      MSG_REMOVEITEM,                    //831 T.I.D        // NOT USED - TAR# 247554
      LTT_RETURNITEMS,                    //832 T.I.D        // NOT USED - TAR# 247554
      MSG_LASTITEM,                        //833        // NOT USED - TAR# 247554
      MSG_FEESCUSTOMERAGREEMENT2,        //834=Fees && Customer Agreements:
      MSG_RECEIPT,                        //835=Receipt:
      MSG_FEESCUSTOMERAGREEMENT,         //836=Fees & Customer Agreements
      MSG_FEES,                          //837=Fees
      MSG_CUSTOMERAGREEMENT,             //838=Customer Agreement838
      MSG_CANCELACTION,                  //839=Cancel Action
      MSG_ITEMENTRY2,                     //840=Item Entry:
      MSG_ITEMENTRY,                      //841=Item Entry
      MSG_EMPTY1,                         //842=        // NOT USED - TAR# 247554
      MSG_EMPTY2,                         //843=        // NOT USED - TAR# 247554
      MSG_EMPTY3,                         //844        // NOT USED - TAR# 247554
      MSG_SCANADDITONALITEMS2,           //845=Scan Additional Items:
      MSG_SCANITEMSORPRESSFINISHEDSCANNING, //846=Scan items or press Finished Scanning
      MSG_FINISHSCANNING,                  //847=Finished Scanning
      MSG_QUANTITYNEEDED2,                //848=Quantity Needed:
      MSG_QUANTITYNEEDED,                 //849=Quantity Needed
      MSG_ONEITEM,                        //850=One Item
      MSG_TWOITEM,                        //851=Two Items
      MSG_THREEITEMS,                     //852=Three Items
      MSG_FOURITEMS,                      //853=Four Items
      MSG_FIVEITEMS,                      //854=Five Items
      MSG_ASSEMBLYFEE,                    //855=Assembly Fee:
      MSG_RECEIPT2,                        //856=Receipt:
      MSG_VERIFYASSEMBLYFEE,              //857=Verify Assembly Fee
      MSG_YES,                            //858=Yes
      MSG_NO,                             //859=No
      MSG_MEASUREMENTNEEDED2,              //860=Measurement Needed:
      MSG_MEASUREMENTNEEDED,               //861=Measurement Needed        // NOT USED - TAR# 247554
      MSG_COUPONVALUEENTRY2,               //862=Coupon Value Entry:
      MSG_COUPONVALUEENTRY,                //863=Coupon Value Entry
      MSG_BICAPPROVAL,                     //864=BIC Approval:
      MSG_VERIFYBICCARDHOLDER,             //865=Verify BIC Cardholder
      MSG_YES2,                             //866=Yes
      MSG_NO2,                              //867=No
      MSG_AUTHORIZATIONNUMBERENTRY,        //868=Authorization Number Entry:
      MSG_KEYINAUTHORIZATION,              //869=Key in the Authorization # and press 'Enter'
      MSG_ALERTS,                          //870=Alerts:
      MSG_CONFIRMSUSPEND,                  //871=Confirm Suspend
      MSG_CANCELSUSPEND,                   //872=Cancel Suspend
      MSG_DLIDNUMBERENTRY,                 //873=D.L. / I.D. Number Entry:
      MSG_KEYINDLID,                       //874=Key in the D.L. or I.D. # and press 'Enter'
      MSG_HEADCASHIERID,                   //875=Head Cashier ID for Cancel Purchase
      MSG_KEYINHEADCASHIER,                //876=Key in Head Cashier ID and press 'Enter'
      MSG_PRICEPRMT,                       //877=Price Prmt:
      MSG_ENTERPRICE,                      //878=Enter Price.
      MSG_APPROVEVOID,                     //879=Approve Void        // NOT USED - TAR# 247554
      MSG_SUSPENDTRANSACTION,              //880=Suspend Transaction        // NOT USED - TAR# 247554
      MSG_SUSPEND,                         //881=Suspend:
      MSG_CONFIRMSUSPEND2,                 //882=Confirm Suspend.
      MSG_CONFIRMSUSPNED,                  //883=Confirm Suspend
      MSG_CANCELSUSPEND2,                   //884=Cancel Suspend
      MSG_BAGGING_ALERT,                //885
      MSG_RETURNITEMS_TO_BAGGING,        //886
      BTT_ITEM_REMOVE,                    //887
      MSG_UNEXPECTED_WT,                //888
      RA_DESCRIPTIONINFO,                //889
      
      //new application security Msg
      SS_LOADING,                        // 890
      SS_LOADED,                        // 891
      SS_STOPPING,                        // 892
      DBM_LOADING,                        // 893
      DBM_LOADED,                        // 894
      DBM_STOPPING,                        // 895
      WTINFO_LOADING,                    // 896
      WTINFO_LOADED,                    // 897
      WTINFO_STOPPING,                    // 898
      TXT_RA_VOIDED_ITEM_APPROVAL,      // 899
      MSG_PLACEITEM_IN_BAGGING,            // 900
      TXT_SKIP_BAGGING_THRESHOLD,        // 901
      MSG_OVERALL_WEIGHT_DECREASE,        // 902
      TXT_ITEM_REMOVED_THRESHOLD,        // 903
      TXT_ITEM_WEIGHT_APPROVAL,            // 904

      TXT_MOREINFO_ON_RECEIPT,          // 905
      MSG_PLEASE_TAKE_YOUR_ITEMS,        // 906
      MSG_RP_VOIDEDITEMS,                // 907    used in Assistance Report        // NOT USED - TAR# 247554
      RA_SECURITY_EVENT,                // 908
      TOP_CAP_INSTR_RA_INCORRECT_WEIGHT,// 909
      BOTTOM_CAP_RA_INCORRECT_WEIGHT,    // 910        // NOT USED - TAR# 247554
      BUTTON_RA_CLEAR_SECURITY,            // 911
      BUTTON_RA_APPROVE_UPDATE,            // 912
      TOP_CAP_RA_UNEXPECTED_ITEM,        // 913
      SUMMARY_INSTR_RA_UNEXPECTED_ITEM,    // 914
      TOP_CAP_RA_SKIPBAGGING_THRES,        // 915
      SUMMARY_INSTR_RA_SKIPBAGGING_THRES,    // 916
      TOP_CAP_RA_ITEMREMOVED_THRES,        // 917
      TOP_CAP_RA_NEW_WEIGHT_ITEM,        // 918
      SUMMARY_INSTR_RA_NEW_WEIGHT_ITEM,    // 919
      BUTTON_RA_APPROVE_NO_UPDATE,        // 920
      TOP_CAP_RA_BAG_ITEM_TIMER,        // 921
      SUMMARY_INSTR_RA_BAG_ITEM_TIMER,    // 922 //NOT USED - TAR 253084 - This ID is used in SCOTMSGE.dat , but RAP only reads it and does dislay it anywhere
      BUTTON_RA_LIGHT_ITEM,                // 923        // NOT USED - TAR# 247554
      BUTTON_RA_ITEM_NOT_BAGGED,        // 924        // NOT USED - TAR# 247554
      SUMMARY_INSTR_RA_EAS_NOT_ACTIVATED,    // 925        // NOT USED - TAR# 247554
      TOP_CAP_RA_PICKING_UP_ITEMS_TIMER,    // 926
      SUMMARY_INSTR_RA_BAG_SCALE_FAILURE,    // 927
      BOTTOM_CAP_RA_BAG_SCALE_FAILURE,    // 928
      SUMMARY_INSTR_RA_WLDB_FAILURE,    // 929
      BOTTOM_CAP_RA_WLDB_FAILURE,        // 930
      RA_WEIGHTINFO,                    // 931
      RA_SECVIOLATIONINFO,                // 932
      RA_WTMISMATCH,                    // 933
      RA_WTINCREASE,                    // 934
      RA_WTDECREASE,                    // 935
      RA_SKIPBAGGINGTHRESHOLD,            // 936
      RA_REMOVEITMESTHRESHOLD,            // 937
      RA_BAGORBOXTHRESHOLD,                // 938        // NOT USED - TAR# 247554
      RA_TIMEREXPIRED,                    // 939
      RA_VOID_EVENT,                    // 940
      RA_VOIDEDITEM,                    // 941
      MSG_RP_SECURITYALERTS,            // 942    used in Assistance Report        // NOT USED - TAR# 247554
      BTT_BAGBOX,                        // 943
      SUMMARY_INSTR_RA_BAGBOX_THRES,    // 944
      RA_BAGBOXTHRESHOLD,                // 945
      TXT_BAGBOX_THRESHOLD,                // 946
      RA_NEWWEIGHTITEM,                    // 947
      MSG_REMOTE_ASSISTANCE,         // 948
      MSG_DPS_DISCONNECTED_ERR,      // 949        // NOT USED - TAR# 247554
      RA_WEIGHTINCREASEINFO,            // 950
      RA_WEIGHTDECREASEINFO,            // 951
      BTT_SMM_ADDZEROWT,                //952
      MSG_SS_COMM_FAILED,                //953
      MSG_SS_OPEN_FAILED,                //954
      MSG_SS_NOT_ENABLED,                //955
      MSG_SS_CLAIM_FAILED,              //956

      MSG_DBMC_FAILED,                    //957
      TXT_BAG_SCALE,                    //958
      TXT_WLDB,                            //959
      MSG_BAGSCALE_ERR_FAULT,            //960
      MSG_WLDB_ERR_FAULT,                //961
      SUMMARY_INSTR_RA_VOID_APPROVAL,    //962
      BUTTON_RA_VOID_APPROVAL,            //963
      SUMMARY_INSTR_RA_INCORRECT_WEIGHT,//964
      MSG_REMOTE_ASSISTANCE2,         // 965        // NOT USED - TAR# 247554
      MSG_DPS_DISCONNECTED_ERR2,      // 966        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED1,    //967        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED2,    //968        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED3,    //969        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED4,    //970        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED5,    //971        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED6,    //972        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED7,    //973        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED8,    //974        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED9,    //975        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED10,   //976        // NOT USED - TAR# 247554
      RA_UNEXPECTEDINCREASEINFO,        //977
      RA_DONTBAGTHRESHOLDINFO,            //978
      RA_ITEMREMOVEDTHRESHOLDINFO,        //979
      RA_PICKINGUPITEMTIMEREXPIREDINFO, //980
      RA_PRICEINFO,                        //981        // NOT USED - TAR# 247554
      BUTTON_RA_DEVICE_ERROR,            //982

      // Takeaway Belt messages
      TAB_LOADING,                      // 983
      TAB_LOADED,                       // 984
      MSG_BELT_PLACE_ITEM_ON_BELT,      // 985
      MSG_BELT_INCORRECT_ITEM,          // 986        // NOT USED - TAR# 247554
      MSG_BELT_ITEM_REMOVED,            // 987
      MSG_BELT_UNEXPECTED_ITEM,         // 988
      // End Takeaway Belt messages

      RA_STATE_SCANANDBAG,                // 989        // NOT USED - TAR# 247554
      SCOT_LANE_STATUS,                    // 990
      RA_LANEBTT_RESTRICTEDITEM,        // 991
      RA_THIRDLINE_RESTRICTEDITEM,        // 992
      RA_FIRSTLINE_LANECLOSED,          // 993 //awk.2.6.2 TAR192509
      RA_BUTTON_LANECLOSED,             // 994 //awk.2.6.2 TAR192509
      RA_LEADTHRU_LANECLOSED,           // 995 //awk.2.6.2 TAR192509
      RA_LANEBTT_UNKNOWNPRICE,            // 996
      RA_THIRDLINE_UNKNOWNPRICE,        // 997
      RA_LANEBTT_QUANTITYRESTRICTEDITEM,    // 998
      RA_THIRDLINE_QUANTITYRESTRICTEDITEM,    // 999
      RA_LANEBTT_RECALLEDITEM,            // 1000
      RA_THIRDLINE_RECALLEDITEM,        // 1001
      RA_LANEBTT_TIMERESTRICTEDITEM,    // 1002
      RA_THIRDLINE_TIMERESTRICTEDITEM,    // 1003

      RA_LANEBTT_VISUALVALIDATIONITEM,    // 1004
      RA_THIRDLINE_VISUALVALIDATIONITEM,    // 1005
      RA_LANEBTT_OTHERPAYMENT,            // 1006
      RA_THIRDLINE_CALLFORHELP,            // 1007
      RA_THIRDLINE_DEVICEERROR,            // 1008        // NOT USED - TAR# 247554
      RA_LANEBTT_STOREMODE,                // 1009
      RA_LANEBTT_PRICENEEDED,           // 1010        // NOT USED - TAR# 247554
      LTT_TRANSPORT_ITEM,                // 1011
      MSG_TRANSPORT_ITEM,                // 1012        // NOT USED - TAR# 247554
      LTT_BAG_AREA_BACKUP,                // 1013
      MSG_BAG_AREA_BACKUP,                // 1014
      RA_THIRDLINE_BAGAREABACKUP,        // 1015
      TOP_CAP_RA_BAG_AREA_BACKUP,        // 1016
      SUMMARY_INSTR_RA_BAG_AREA_BACKUP,    // 1017
      RA_AGERESTRICT_MINIMUM_AGE,       // 1018
      LTT_BELT_PLACEONBELT,             // 1019
      LTT_BELT_WELCOME,                 // 1020
      LTT_BELT_ATTENTION,               // 1021
      BTT_BELT_SKIPBELT,                // 1022
      RA_STATE_SAFETYCOVER,             // 1023        // NOT USED - TAR# 247554
      RA_LEADTHRU_SAFETYCOVEROPEN,      // 1024        // NOT USED - TAR# 247554
      RA_MSG_SAFETYCOVEROPEN,           // 1025
      RA_CANCELTRANSACTION,             // 1026
      RA_LTT_CANCELTRANSACTION,         // 1027
      TXT_COUPON_THRESHOLD_EXCEEDED,    // 1028
      LTT_INSERTCOUPON,                 // 1029
      MSG_INSERTCOUPON,                 // 1030
      MSG_SCANCOUPONS,                  // 1031
      RA_TC_UNDEACTIVATEDITEM,          // 1032
      RA_LTT_DEACTIVATEANDCLEAR,        // 1033
      TXT_UNDEACTIVATEDITEMONBELT,      // 1034        // NOT USED - TAR# 247554
      RA_THIRDLINE_UNDEACTIVATED,       // 1035
      LTT_UNDEACTIVATEDITEM,            // 1036
      RA_LANEBTT_VERIFYSIGNATURE,       // 1037 awk.3.27.2 TAR199004
      RA_FIRSTLINE_VERIFYSIGNATURE,     // 1038 awk.3.27.2 TAR199004
      RA_RECEIPT_MSG_EASITEM_REFERENCE, // 1039
      RA_LANEBTT_SIGCAPTURE,            // 1040 awk.4.23.2 TAR200003
      RA_THIRDLINE_SIGCAPTURE,          // 1041 awk.4.23.2 TAR200003
      PRN_ENDORSEMENT_CHIT_HEADER,      //1042=ENDORSEMENT CHIT:\nTHIS HAS PRINTED BECAUSE A DOCUMENT\nENDORSEMENT FAILED.  PLEASE ATTACH THIS\nCHIT TO THE APPROPRIATE DOCUMENT.\n----------------------------------------\n
      RA_ASSISTED_TENDER,               //1043=Use Assist Mode to accept %s from the customer.
      TXT_RAPASSISTEDTENDER,            //1044=Operator Assisted Tender
      LTT_ASSISTEDTENDER,               //1045=Pay with %s
      MSG_TAKEPAYMENTTOCASHIER,         //1046=Please take your %s\nto cashier at pay station
      MSG_ENDORSEMENTNEEDED,            //1047=Endorsement Needed. Please wait for operator assistance.
      MSG_DOCUMENTNUMBERENTRY,          //1048=Document Number Entry:        // NOT USED - TAR# 247554
      MSG_KEYINDOCUMENTNUM,             //1049=Key in the Document # and press 'Enter'        // NOT USED - TAR# 247554
      TXT_RAPDOCUMENTNUM,               //1050=Document Number        // NOT USED - TAR# 247554
      TXT_RAPPAPERSLIP,                 //1051=Paper Slip Approval
      RA_SIGNPAPERSLIP,                 //1052=Use AssitMode to print signature paper slip by pressing 9        // NOT USED - TAR# 247554
      TXT_WAITFORPAPERSLIP,             //1053=Wait for cashier to bring paper slip to sign
      LTT_ENTERCASHBACKVALUE,           //1054=Cash Back Amount        // NOT USED - TAR# 247554
      MSG_ENTERCASHBACKVALUE,           //1055=Key in Cash Back Amount in cents and press 'Enter'        // NOT USED - TAR# 247554
      BTT_OTHERAMOUNT,                  //1056=Other Amount        // NOT USED - TAR# 247554
      MSG_OVERCASHBACKLIMIT,            //1057=Over Cash Back limit, please reenter        // NOT USED - TAR# 247554
      TXT_JUSTASSISTEDTENDER,           //1058="Assisted Tender"
      MSG_RP_TENDER_REMOTE_CASH,        //1059=CASH @ RAP        // NOT USED - TAR# 247554
      TXT_PLEASEWAIT,                   //1060="PLEASE WAIT...                          "
      MSG_RP_TENDER_USERDEFINED11,    //1061=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED12,    //1062=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED13,    //1063=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED14,    //1064=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED15,    //1065=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED16,    //1066=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED17,    //1067=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED18,    //1068=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED19,    //1069=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_USERDEFINED20,    //1070=        // NOT USED - TAR# 247554
      MSG_RP_TENDER_CHECK,            //1071=        // NOT USED - TAR# 247554
      TXT_NEEDINFORMATION,              //1072=
      RA_SECVIOLATIONWITH_DNTYPEINFO,   //1073
      //    TAB Specific
      TXT_SKIP_BELT_THRESHOLD,            //1074 TAB 901 LANE
      RA_SKIPBELTTHRESHOLD,                //1075 TAB 936 LANE
      MSG_ATTENTION_ALERT,                //1076 TAB 885 LANE
      MSG_RETURNITEMS_TO_BELT,            //1077 TAB 886 LANE
      TXT_SMM_WTEXPBELT_INSTR,            //1078 TAB 615 LANE
      MSG_SMM_DUPBELTWEIGHT,            //1079 TAB 645 LANE
      TXT_SMM_ITEMWTBELT_INSTR,            //1080 TAB 613 LANE
      BTT_ASSISTMODETOGGLE,                //1081 "Toggle"
        BTT_ITEMREMOVED_ONITEMREMOVEVIOLATION,//1082 Btn Text for Item Removed on ItemRemove Violation screen CHU 072802 RFC208874
      MSG_RETURNITEMS_TO_BAGGING_ONITEMREMOVEVIOLATION,    //1083 CHU 072802 RFC208874
      RA_LANEBTT_COLLECCOUPONS,            //1084  //awk.8.6.2
      RA_FIRSTLINE_COLLECTCOUPONS,        //1085  //awk.8.6.2
      MSG_SMM_DELETEZERO,                //1086 TD //SMM MSG RFC 203307 
      MSG_CARDACCEPTED_SCANITEMS,        //1087 215064 HFE determined to use separate text for MCR LPM102302
      MSG_MSR_COMMUNICATIONERROR,        //1088 BZ TAR 219971        // NOT USED - TAR# 247554
      BTT_ENDREPORTPERIOD,                //1089 TD 3.1
      BTT_SELECTREPORT,                    //1090 TD 3.1
      LTT_SELRPTTYPE,                    //1091 TD 3.1        // NOT USED - TAR# 247554
      MSG_REPORTCNT,                    //1092 TD 3.1
      BTT_PRINTLAST,                    //1093 TD 3.1
      BTT_PRINTSIGN,                    //1094 TD 3.1
      LTT_REPORT,                        //1095 TD 3.1
      BTT_REPORTING,                    //1096 TD 3.1
      LTT_REPORTING,                    //1097 TD 3.1
      MSG_PTRSTAT_ERROR_STATE,          //1098 Error Handling
      MSG_UNKNOWN_DEVICE_ERROR,         //1099 Error Handling
      FMT_RECEIPTITEM1,                 //1100
      FMT_RECEIPTITEM2,                 //1101
      BTT_NEXT,                            //1102
      BTT_PREVIOUS,                        //1103
      MSG_SCALEIMAGE,                    //1104
      MSG_KGSCALEIMAGE,                    //1105
      FMT_RECEIPTITEM3,                 //1106
      FMT_RECEIPTITEM4,                 //1107
      FMT_RECEIPTITEM5,                 //1108
      FMT_RECEIPTITEM6,                 //1109
      FMT_RECEIPTITEM7,                 //1110
      FMT_RECEIPTITEM8,                 //1111
      FMT_RECEIPTITEM9,                 //1112
      FMT_RECEIPTITEM10,                //1113
      LTT_REPORTEOD,                    //1114
      MSG_REPORTINGOFFLINE,             //1115
      MSG_NCR_E5633_PURGE_OUT,          //1116  Error Handling
      MSG_NCR_E5633_CASS_OUT,           //1117  Error Handling
      MSG_NCR_E5633_FEED_FAILURE,       //1118  Error Handling
      MSG_NCR_E5633_TIMING_WHEEL_FAILED, //1119  Error Handling
      MSG_NCR_E5633_PICK_SENSOR,        //1120  Error Handling
      MSG_NCR_E5633_FEED_SENSOR,        //1121  Error Handling
      MSG_NCR_E5633_EXIT_SENSOR,        //1122  Error Handling
      MSG_NCR_E5633_BILL_JAMDIVERTER_ERROR,     //1123  Error Handling
      MSG_NCR_E5633_DIVERTER_ERROR,           //1124  Error Handling        // NOT USED - TAR# 247554
      MSG_NCR_ECOINDISP_ALARM_ERROR,    //1125  Error Handling
      MSG_NCR_ECOINDISP_OFFLINE_ERROR,  //1126  Error Handling
      BTT_RECONNECT,                    //1127
      LTT_REPORTNAME,                   //1128        // NOT USED - TAR# 247554    // USED - TAR# 248454
      MSG_DEVICEERRORNOTFIXED,          //1129
      LTT_BILLACCEPTORERROR,            //1130        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BILLACCEPTORCASSETTOUT,       //1131        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BILLACCEPTORCOMMUNICATIONERROR,            //1132        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BILLACCEPTORJAM,       //1133        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BILLACCEPTORFULL,       //1134        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BILLDISPENSERERROR,     //1135        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_PURGE_OUT,    //1136        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_CASS_OUT,     //1137        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_FEED_FAILURE, //1138        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_TIMING_WHEEL_FAILED,   //1139        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_PICK_SENSOR,           //1140        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_E5633_BILL_JAM,              //1141        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_COINDISPENSERERROR,              //1142        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_ECOINDISP_ALARM_ERROR,       //1143        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_NCR_ECOINDISP_OFFLINE_ERROR,     //1144        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_COINACCEPTORERROR,               //1145        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_OPOS_ECOIN_FAILURE,              //1146        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_BAGGINGAREASCALEERROR,           //1147        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_SS_COMM_FAILED,                  //1148        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_RECEIPTPRINTERROR,               //1149        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_DM_PTR_ERR_FAULT,                //1150        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_DM_PTR_COVER_OPEN,               //1151        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_DM_PTR_ERR_EMPTY,                //1152        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_PTRSTAT_ERROR_STATE,             //1153        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_SCANNERERROR,                    //1154        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_ERROR_SCAN_SERIAL,               //1155        // NOT USED - TAR# 247554    // USED - TAR# 248454
      MSG_NCR_ECOINDISP_OFFLINE_ERROR2,    //1156
      NCR_E5633_TIMING_WHEEL_FAILED2,      //1157
      LTT_SIGCAPERROR,                     //1158        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_ERROR_SIGCAP_NO_COMM2,           //1159        // NOT USED - TAR# 247554    // USED - TAR# 248454
      MSG_ERROR_SIGCAP_NO_COMM,            //1160
      LTT_PAGERERROR,                      //1161        // NOT USED - TAR# 247554    // USED - TAR# 248454
      LTT_PAGER_NO_COMM,            //1162        // NOT USED - TAR# 247554    // USED - TAR# 248454
      MSG_PAGER_NO_COMM,            //1163
          LTT_DM_590_PTR_ERR_FAULT,     //1164
          MSG_DM_590_PTR_ERR_FAULT,     //1165
          MSG_DM_590_PTR_COVER_OPEN,    //1166
          MSG_DM_590_PTR_ERR_PAPER_JAM, //1167
          LTT_DOOR_OPEN,                //1168
          MSG_DOOR_OPEN,                //1169
          MSG_DM_PTR_ERR_PAPER_JAM2,    //1170
          MSG_DATANEEDED_UNEXPECTEDINCREASE,        // 1171
          MSG_DATANEEDED_CONFIRMSUSPEND,            // 1172
          MSG_DATANEEDED_APPROVEVOIDEDITEM,         // 1173
          MSG_DATANEEDED_MISMATCHWEIGHT,            // 1174
          MSG_DATANEEDED_DONTBAGTHRESHOLDEXCEEDED,  // 1175
          MSG_DATANEEDED_NEWITEMENTRY,              // 1176
          MSG_DATANEEDED_BAGITEMTIMEREXPIRED,       // 1177
          MSG_DATANEEDED_DONEREMOVINGTHRESHOLDEXCEEDED,  // 1178
          MSG_DATANEEDED_WALKAWAYTIMEREXPIRED,      // 1179
          MSG_DATANEEDED_PRICENOTMATCH,             // 1180
          MSG_DATANEEDED_COUPONTHRESHOLDEXCEEDED,   // 1181
          MSG_DATANEEDED_COUPONDOLLARMAX,           // 1182
          MSG_DATANEEDED_PRICEPROMPT,               // 1183
          MSG_DATANEEDED_MOREDATA,                  // 1184
          MSG_DATANEEDED_BAGBOXTHRESHOLDEXCEEDED,   // 1185
          MSG_DATANEEDED_UNEXPECTEDDECREASE,        // 1186
          MSG_DATANEEDED_DEVICEERROR,               // 1187
          MSG_DATANEEDED_QUANTITY,                  // 1188
          MSG_DATANEEDED_ENTERITEM,                 // 1189
          MSG_DATANEEDED_ENTERCOUPON,               // 1190
          MSG_DATANEEDED_MEASUREMENT,               // 1191
          MSG_DATANEEDED_ASSEMBLYFEE,               // 1192
          MSG_DATANEEDED_VERIFYIDENTITY,            // 1193
          MSG_DATANEEDED_AUTHORIZATIONNUM,          // 1194
          MSG_DATANEEDED_CSASUSPEND,                // 1195
          MSG_DATANEEDED_DLIDNUMBER,                // 1196
          MSG_DATANEEDED_CANCELLTRANSACTION,        // 1197
          MSG_DATANEEDED_ENTERACCOUNTNUMBER,        // 1198
          MSG_DATANEEDED_ENTEREXPDATE,              // 1199
          MSG_DATANEEDED_PRICEPRMT,                 // 1200
          MSG_DATANEEDED_ASSISTEDTENDER,            // 1201
          MSG_DATANEEDED_CLOSELANE,                 // 1202
          MSG_DATANEEDED_ENDORSEDOCUMENT,           // 1203
          MSG_DATANEEDED_DOCUMENTNUM,               // 1204
          MSG_DATANEEDED_BAGAREABACKUP,             // 1205
          MSG_DATANEEDED_UNDEACTIVATEDITEM,         // 1206
          MSG_DATANEEDED_VERIFYSIGNATURE,           // 1207
          MSG_DATANEEDED_ASSISTEDITEM,              // 1208
          MSG_DATANEEDED_GIFTCARDLIMIT,             // 1209
          MSG_DATANEEDED_LASTVIEW,                  // 1210
          MSG_DATANEEDED_DEFAULT,                   // 1211
          MSG_RA_CAT_OTHER,                         // 1212
          MSG_RA_CAT_UNKNOWN,                       // 1213
          MSG_RA_CAT_RECEIPT,                       // 1214
          MSG_RA_CAT_DEVERR,                        // 1215
          MSG_RA_CAT_SECURITY,                      // 1216
          MSG_RA_CAT_VOID,                          // 1217
          MSG_RA_CAT_COLLECTION,                    // 1218
          MSG_RA_CAT_AGERESTRICT,                   // 1219
          MSG_RA_CAT_EFT,                           // 1220
          MSG_RA_CAT_AUDACTION,                     // 1221
          MSG_RA_CAT_OUTOFSERVICE,                  // 1222
          MSG_RA_CAT_MEDIASHORTAGE,                 // 1223
          MSG_RA_CAT_PRICENEEDED,                   // 1224
          MSG_RA_CAT_UNKNOWNITEM,                   // 1225
          MSG_RA_CAT_RECALLEDITEM,                  // 1226
          MSG_RA_CAT_TIMERESTRICT,                  // 1227
          MSG_RA_CAT_SUSPEND,                       // 1228
          MSG_RA_CAT_CALLFORHELP,                   // 1229
          MSG_RA_CAT_NEGATIVEDEPT,                  // 1230
          MSG_RA_CAT_VISUALVERIFY,                  // 1231
          MSG_RA_CAT_SIGCAP,                        // 1232
          MSG_RA_CAT_OTHERPAYMENT,                  // 1233
          MSG_RA_CAT_QTYRESTRICT,                   // 1234
          MSG_RA_CAT_EASITEMACTIVE,                 // 1235
          LTT_RP_PROCESSINGREPORT,                  // 1236
          MSG_TB_TENDER_COUPON,                     // 1237
          MSG_TB_TENDER_OTHERPAYMENT,               // 1238
          MSG_TB_TENDER_NONE ,                      // 1239
          MSG_TB_TENDER_CASH ,                      // 1240
          MSG_TB_TENDER_CREDIT,                     // 1241
          MSG_TB_TENDER_DEBIT,                      // 1242
          MSG_TB_TENDER_LOYALTY,                    // 1243
          MSG_TB_TENDER_DEBITSAVING,                // 1244
          MSG_TB_TENDER_DEBITCHECKING,              // 1245
          MSG_TB_TENDER_EBT,                        // 1246
          MSG_TB_TENDER_FOODSTAMPS,                 // 1247
          MSG_TB_TENDER_GIFTCARD,                   // 1248
          MSG_TB_TENDER_PIP_GENERIC,                // 1249 
          MSG_TB_TENDER_PIP_CREDIT,                 // 1250
          MSG_TB_TENDER_PIP_DEBIT,                  // 1251
          MSG_TB_TENDER_PIP_DEBIT_CASHBACK,         // 1252
          MSG_TB_TENDER_PIP_LOYALTY,                // 1253
          MSG_TB_TENDER_PIP_DEBITSAVING,            // 1254  
          MSG_TB_TENDER_PIP_DEBITCHECKING,          // 1255
          MSG_TB_TENDER_PIP_EBT,                    // 1256
          MSG_TB_TENDER_PIP_FOODSTAMPS,             // 1257
          MSG_TB_TENDER_PIP_GIFTCARD,               // 1258
          MSG_TB_TENDER_USERDEFINED1,               // 1259
          MSG_TB_TENDER_USERDEFINED2,               // 1260 
          MSG_TB_TENDER_USERDEFINED3,               // 1261
          MSG_TB_TENDER_USERDEFINED4,               // 1262 
          MSG_TB_TENDER_USERDEFINED5,               // 1263 
          MSG_TB_TENDER_USERDEFINED6,               // 1264
          MSG_TB_TENDER_USERDEFINED7,               // 1265
          MSG_TB_TENDER_USERDEFINED8,               // 1266
          MSG_TB_TENDER_USERDEFINED9,               // 1267
          MSG_TB_TENDER_USERDEFINED10,              // 1268
          MSG_TB_TENDER_USERDEFINED11,              // 1269
          MSG_TB_TENDER_USERDEFINED12,              // 1270 
          MSG_TB_TENDER_USERDEFINED13,              // 1271
          MSG_TB_TENDER_USERDEFINED14,              // 1272 
          MSG_TB_TENDER_USERDEFINED15,              // 1273 
          MSG_TB_TENDER_USERDEFINED16,              // 1274
          MSG_TB_TENDER_USERDEFINED17,              // 1275
          MSG_TB_TENDER_USERDEFINED18,              // 1276
          MSG_TB_TENDER_USERDEFINED19,              // 1277
          MSG_TB_TENDER_USERDEFINED20,              // 1278
          MSG_TB_TENDER_CHECK,                      // 1279
          MSG_TB_TENDER_CASH_RAP,                   // 1280
          MSG_TB_TENDER_ASSISTTENDER,               // 1281
          MSG_NEED_PRICE,                           // 1282
          MSG_AGE_RESTRICTED,                       // 1283        // NOT USED - TAR# 247554
          MSG_BIRTHDATE_ENTER,                      // 1284        // NOT USED - TAR# 247554
          MSG_FAVORITES_TAB,                        // 1285
          MSG_LANGUAGENAME,                         // 1286 - button text used in Dual Language
          MSG_CHECKITEMWEIGHT,                         // 1287 - RFC 232801
          MSG_LOOKUPITEM_TAB1,                      // 1288 - TAR 236758 needed for Dual Language
          MSG_LOOKUPITEM_TAB2,                      // 1288 - TAR 236758 
          MSG_LOOKUPITEM_TAB3,                      // 1288 - TAR 236758
          MSG_LOOKUPITEM_TAB4,                      // 1288 - TAR 236758
          MSG_LOOKUPITEM_TAB5,                      // 1288 - TAR 236758
          MSG_LOOKUPITEM_TAB6,                      // 1288 - TAR 236758
          MSG_LOOKUPITEM_TAB7,                      // 1288 - TAR 236758
          MSG_LOOKUPITEM_TAB8,                      // 1288 - TAR 236758
          MSG_REFILLCOINWITHLOWSENSOR,              // 1296
          MSG_DATANEEDED_TAKECHANGETIMEREXPIRED,    // 1297
          MSG_TRANSACTION_CONTROL,                  // 1298 - RFC 228102
          BTT_STARTTRANSACTION_AM,                  // 1299 - RFC 228102
          BTT_STARTTRANSACTION,                     // 1300 - RFC 228102
          BTT_EDITPICKLIST,                            // 1301
          BTT_CLOSE,                                // 1302 - Rel 3.2 Requirement 4.5.1 - Single Mode RAP 
          MSG_RA_LOYALTYCARDINFO,                    // 1303 
          MSG_RA_LOYALTYCARDINFO2,                    // 1304
          MSG_RA_CASHINSERTED,                        // 1305
          MSG_RA_CHANGEDISPENSED,                    // 1306
          MSG_RA_RECEIPTPRINTING,                    // 1307
          MSG_RA_PINENTERED,                        // 1308
          MSG_CASHFLOW_CASSOUT,                     // 1309
          MSG_CASHFLOW_COMERROR,                    // 1310
          MSG_CASHFLOW_BILLJAM,                     // 1311
          MSG_CASHFLOW_CASSFULL,                    // 1312
          MSG_TEC306_NOCOMM,                        // 1313
          MSG_TEC306_PAPEROUT,                      // 1314
          MSG_TEC306_COVEROPEN,                     // 1315
          MSG_TEC306_PAPERJAMPRE,                   // 1316        // NOT USED - TAR# 247554
          MSG_VOUCHER_PAYMENT,                      // 1317 - RFC 213717
          MSG_SCAN_VOUCHERS,                        // 1318 - RFC 213717
          MSG_TB_TENDER_VOUCHERS,                   // 1319 - RFC 213717
          RA_BEGINVOUCHERPAYMENT,                   // 1320 - RFC 213717
          RA_ENDVOUCHERPAYMENT_BALANCEDUE,          // 1321 - RFC 213717
          MSG_COUPONSENSOR_FAILURE,                 // 1322 - TAR 246799
          MSG_ITEMSREMAINAT,                        // 1323 - RFC 209702
          MSG_WEIGHTONSCALEAFTERTRANX,              // 1324 - RFC 209702
          MSG_PLACEEMBEDDEDPRICEITEMONSCALE,        // 1325 - TAR 249119
          MSG_DATANEEDED_INSERTCOUPONTIMEREXPIRED,  // 1326 - TAR 219178
          LTT_REPORT_TOUCH,                            // 1327 - RFC 249874
          RA_DOUBLEWEIGHT,                            // 1328 - RFC XXXXX
          MSG_CHECKITEMWEIGHTDOUBLE,                // 1329 - RFC XXXXX
          MSG_ITEMNOTBAGGED,                        // 1330 - RFC 251471
          MSG_SHIFT_KEY,                            // 1331 - TAR 245387 
             MSG_SPACE_KEY,                            // 1332 - TAR 245387 
          MSG_BACKSPACE_KEY,                        // 1333 - TAR 245387 
          MSG_ITEMNOTONBELT,                            // 1334 - TAR 253084
          MSG_RA_LANE_CLOSED,                       // 1335 - TAR 257312
          MSG_UTILITY,                              //1336
          MSG_EXIT_FASTLANE,                        //1337
          MSG_GET_DIAG_FILE,                        //1338
          MSG_REBOOT_SYSTEM,                        //1339
          MSG_SHUTDOWN_SYSTEM,                      //1340
          MSG_VOLUMN_CONTROL,                       //1341
          LTT_SELECT_UTILITY,                       //1342
          MSG_SEC_MGR_LOADING,                        // 1343 
          MSG_SEC_MGR_LOADED,                        // 1344
          MSG_SEC_MGR_ENDED,                        // 1345
          MSG_HOSTOFFLINE,                            // 1346 - Offline message on lane
          MSG_RA_HOSTOFFLINE,                        // 1347 - Offline message sent to RAP
          LTT_HOSTOFFLINE,                            // 1348 - Host Offline screen title
          MSG_NEWSEC_MISMATCH,                        // 1349 
          MSG_NEWSEC_UNEX_INCREASE,                    // 1350
          MSG_NEWSEC_UNEX_DECREASE,                    // 1351
          LTT_DEPOSITGIFTCARD,                        // 1352
          MSG_DEPOSTITGIFTCARD,                        // 1353
          MSG_COLLECTGIFTCARD,                        // 1354
          LTT_YOURBALANCE,                            // 1355
          RA_COLLECTGIFTCARD_EVENT,                    // 1356
          RA_COLLECTGIFTCARD,                        // 1357
          MSG_GIFTCARDAMOUNT,                        // 1358
          MSG_SLIP_PRINTER,                            // 1359
          MSG_CASH_DRAWER,                            // 1360
          MSG_CASH_CHANGER,                            // 1361
          MSG_KEYLOCK,                                // 1362
          MSG_LINE_DISPLAY,                            // 1363
          MSG_MICR,                                    // 1364
          MSG_MSR,                                    // 1365
          MSG_SCALE,                                // 1366
          MSG_SCANNER,                                // 1367
          MSG_SIGCAP,                                // 1368
          MSG_MISC,                                    // 1369
          MSG_ENCRYPTOR,                            // 1370
          MSG_CASHACCEPTOR,                            // 1371
          MSG_COINACCEPTOR,                            // 1372
          MSG_DATANEEDED_COLLECTGIFTCARD,            // 1373
          MSG_SCP_BLANKLINE,                        // 1374
          RA_PIM_START_ID,                  // 1375 - TAR 257368
          RA_PIM_CONTINUED_ID,              // 1376 - TAR 257368
          RA_PIM_QTY_TXT,                   // 1377 - TAR 257368
          RA_PIM_WT_TXT,                    // 1378 - TAR 257368
          RA_PIM_PRICE_TXT,                 // 1379 - TAR 257368
          RA_PIM_EXTPRICE_TXT,              // 1380 - TAR 257368
          LTT_SCANPROCESSING,               // 1381 - TAR 262131
          MSG_ABBREV_STD,                    // 1382 - req 6.14
          MSG_ABBREV_EXC,                    // 1383 - req 6.15
          BTT_LANGUAGE,                        // 1384
          LTT_SELECTLANGUAGE,                // 1385
          LTT_FATALERROR,                   // 1386
          MSG_FATALERROR,                   // 1387
          MSG_MULTIMEDIA_TRAINING_TOOL,     // 1388
          LTT_SIGNATURE_CAPTURE,            // 1389
          LTT_DEVICE_NOT_RESPONDING,        // 1390
          MSG_SIGNATURE_CAPTURE_NOT_RESPONDING, //1391
          MSG_TRILIGHT_ERROR,               // 1392 //+ dp185016 tar 277610, 277608  
          MSG_STOPANDRESTART,               // 1393 // tar 277610, 277608, 277605, 277604
          MSG_DIGITAL_INTERFACE_ERROR,      // 1394 // tar 277605, 277604 //-dp185016
          MSG_ENTER_ATTENDANT_MODE,                 // 1395
          MSG_LEAVE_ATTENDANT_MODE,                 // 1396
          MSG_ATTENDANT,                            // 1397
          MSG_SHOW_ATTENDANT_TRAINING,              // 1398
          MSG_SHOW_ATTENDANT,                       // 1399
          LTT_ASSISTANCEINPROGRESS,                 // 1400
          MSG_ASSISTANCEPLEASEWAIT,                 // 1401
      MSG_JUMPER_PLUG_OFF,                      //1402 //+dp185016 tar 277590
      MSG_STOP_AND_RESTART_FASTLANE,            //1403 //-dp185016
      MSG_POS_TB_OUTOFSYNC_STOP,            // 1404 //+dp185016 POS-TB outofsync rfc 282630
      MSG_POS_TB_OUTOFSYNC_NEEDASSISTMODE, // 1405
      BTT_SYNC_POS_TB,                      // 1406
      MSG_DATANEEDED_OUTOFSYNC_STOP,        // 1407 text to send to rap
      MSG_DATANEEDED_OUTOFSYNC_ASSIST,      // 1408 text to send to rap //-dp185016
          LTT_ASSISTMENUTITLE,                      // 1409
          LTT_CHANGEPRICE,                          // 1410
          LTT_CHANGEQUANTITY,                       // 1411
          MSG_ASSISTFUNCTIONS,                      // 1412
          MSG_POWER_FINISH_TRX,                     // 1413 - RFC 265463
          MSG_POWER_UNAVAILABLE,                    // 1414 - RFC 265463
          MSG_NCR_ECOINDISP_ALARM_ERROR_PART2,      // 1415 - unused (TAR 283924 first try)
          MSG_DATANEEDED_MULTIPACKITEM,             // 1416 - TAR 256154
          SUMMARY_INSTR_RA_MULTIPACKITEM,           // 1417 - TAR 256154
          MSG_MAXIMUM_WEIGHT_ALLOWANCE_EXCEEDED,    // 1418 - TAR 256154
          MSG_RA_MULTIPACKITEM,                     // 1419 - TAR 256154
          MSG_CANCELCONFIRM_AM,                     // 1420
          MSG_ASSISTMODE,                            // 1421
          LTT_REMOTESCANNER,                        // 1422
          MSG_REMOTESCANNER_CONNECT,                // 1423
          MSG_REMOTESCANNER_TOUCHCONNECT,           // 1424
          BTT_ACCEPT,                               // 1425
          MSG_QTYCONFIRM_AM,                        // 1426
          MSG_CHANGEQUANTITY,                       // 1427
          MSG_CHANGEPRICE,                          // 1428
          RA_LOWCOINS,                              // 1429
          BTT_VOIDITEM,                                // 1430
          BTT_VOIDALL,                                // 1431
          RA_LOWBILLS,                              // 1432
          MSG_ITEMSREMAINAT2,                        // 1433
          RA_THIRDLINE_RESTRICTEDNOTALLOWED,        // 1434
          MSG_COIN_BILL_DISPENSER_ERROR,            // 1435  TAR 296345
          MSG_SKIP_BAGGING_THRESHOLD_EXCEEDED,        // 1436  TAR 296349
            MSG_ITEM_REMOVED_THRESHOLD_EXCEEDED,        // 1436  TAR 296349
          MSG_NEW_ITEM_THRESHOLD_EXCEEDED,            // 1436  TAR 296349
            MSG_BAG_BOX_THRESHOLD_EXCEEDED,            // 1436  TAR 296349
         MSG_CASH_MANAGEMENT,                   // 1440
         MSG_COIN_MANAGEMENT,
         MSG_INSERT_OR_UNLOAD_CASH,
         MSG_INSERT_OR_UNLOAD_COIN,
         BTT_REMOVE,
         MSG_CHANGE_IN_COUNT,
         BTT_UNLOAD,
         MSG_AUTHENTICATE,
         MSG_COIN_STATUS,
         MSG_BILL_STATUS,
         MSG_LOCKBOX,                           // 1450
         MSG_CASHIER_ID,
         MSG_KEYIN_ID,
         MSG_CASHIER_PASSWORD,
         MSG_KEYIN_PASSWORD,
         MSG_INVALIDNUMBER,
         MSG_INCORRECTCASHIER,
         MSG_CURRENCYHIGH,
         MSG_FASTLANEREPORT,
         MSG_CASH_MANAGEMENTREPORT,
         MSG_BILLS,                             // 1460
         MSG_COINS,
         MSG_BINS,
         MSG_CHANGEINVALUE,
         MSG_TERMINALNUMBER,                
         MSG_CASHRECYCLER_DEVICE_ERROR,     
         LTT_CASHRECYCLER_FULL,             
         MSG_CASHRECYCLER_FULL,             
         LTT_CASHRECYCLER_EMPTY,            
         MSG_CASHRECYCLER_EMPTY,            
         LTT_BILLRECYCLER_DEVICE_ERROR,             //1470
         LTT_COINRECYCLER_DEVICE_ERROR,             
         LTT_CASHRECYCLER_NOT_READY,             
         MSG_CASHRECYCLER_NOT_READY,             
         LTT_CASHRECYCLER_HARDWARE_ERROR,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_11,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_12,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_13,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_14,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_15,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_16,        //1480
         MSG_BILLRECYCLER_HARDWARE_ERROR_17,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_18,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_21,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_22,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_24,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_25,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_26,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_27,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_28,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_31,        //1490
         MSG_BILLRECYCLER_HARDWARE_ERROR_32,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_41,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_42,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_43,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_44,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_45,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_46,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_47,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_48,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_49,        //1500
         MSG_BILLRECYCLER_HARDWARE_ERROR_51,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_52,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_53,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_57,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_58,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_59,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_61,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_62,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_71,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_72,        //1510
         MSG_BILLRECYCLER_HARDWARE_ERROR_73,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_75,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_76,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_81,   
         MSG_BILLRECYCLER_HARDWARE_ERROR_82,        
         MSG_BILLRECYCLER_HARDWARE_ERROR_83,   
         MSG_COINRECYCLER_HARDWARE_ERROR_11,   
         MSG_COINRECYCLER_HARDWARE_ERROR_12,   
         MSG_COINRECYCLER_HARDWARE_ERROR_14,   
         MSG_COINRECYCLER_HARDWARE_ERROR_15,        //1520
         MSG_COINRECYCLER_HARDWARE_ERROR_16,   
         MSG_COINRECYCLER_HARDWARE_ERROR_17,   
         MSG_COINRECYCLER_HARDWARE_ERROR_19,   
         MSG_COINRECYCLER_HARDWARE_ERROR_21,   
         MSG_COINRECYCLER_HARDWARE_ERROR_22,   
         MSG_COINRECYCLER_HARDWARE_ERROR_23,   
         MSG_COINRECYCLER_HARDWARE_ERROR_24,   
         MSG_COINRECYCLER_HARDWARE_ERROR_26,   
         MSG_COINRECYCLER_HARDWARE_ERROR_31,   
         MSG_COINRECYCLER_HARDWARE_ERROR_32,        //1530
         MSG_COINRECYCLER_HARDWARE_ERROR_41,   
         MSG_COINRECYCLER_HARDWARE_ERROR_43,   
         MSG_COINRECYCLER_HARDWARE_ERROR_44,   
         MSG_COINRECYCLER_HARDWARE_ERROR_50,   
         MSG_COINRECYCLER_HARDWARE_ERROR_51,   
         MSG_COINRECYCLER_HARDWARE_ERROR_52,   
         MSG_COINRECYCLER_HARDWARE_ERROR_53,   
         MSG_COINRECYCLER_HARDWARE_ERROR_54,   
         MSG_COINRECYCLER_HARDWARE_ERROR_55,   
         MSG_COINRECYCLER_HARDWARE_ERROR_56,        //1540
         MSG_COINRECYCLER_HARDWARE_ERROR_57,   
         MSG_COINRECYCLER_HARDWARE_ERROR_58,   
         MSG_COINRECYCLER_HARDWARE_ERROR_59,   
         MSG_COINRECYCLER_HARDWARE_ERROR_61,   
         MSG_COINRECYCLER_HARDWARE_ERROR_62,   
         MSG_COINRECYCLER_HARDWARE_ERROR_71,   
         MSG_COINRECYCLER_HARDWARE_ERROR_72,   
         MSG_COINRECYCLER_HARDWARE_ERROR_81,   
         MSG_COINRECYCLER_HARDWARE_ERROR_82,   
         MSG_COINRECYCLER_HARDWARE_ERROR_83,       //1550  
         MSG_CURRENCYISHIGH,                       
         MSG_BINISFULL,                            
         MSG_CURRENCYHIGHSTATUS,                   
         MSG_ACCEPTOR,                             
         MSG_DISPENSER,                            
         MSG_ACCEPTOR_PICKUP,                      
         MSG_DISPENSER_PICKUP,                     
         MSG_EXIT_AND_SAVE,                        
         MSG_EXIT_NO_SAVE,                         
         MSG_ACCEPTOR_TOTAL,                       //1560 
         MSG_DENOMINATION,                         
         MSG_COUNT,                                
         MSG_LOAD,                                 
         MSG_ENTER_LOAD_COUNT,                     
         MSG_ENTER_LOAD_VALUE,                     
         MSG_FULL_REPLACEMENT,                     
         MSG_DISPENSER_TOTAL,                      
         MSG_GRAND_TOTAL,                          
         MSG_VALUE,                                
         BTT_IGNORE_CHANGES,                       //1570
         MSG_CONFIRM_IGNORE_CHANGES_TITLE,         
         MSG_CONFIRM_IGNORE_CHANGES,              
         MSG_BILL_COIN_MANAGEMENT_LOAD_PICKUP,     
         MSG_REENTER_VALUE,                       
         MSG_COIN_DISPENSER_STATUS,           
         MSG_CASH_DISPENSER_STATUS,                
         MSG_TOTAL,                                
         MSG_TOTAL_VALUE,                          
         MSG_BILL_ACCEPTOR,                        
         MSG_BILL_DISPENSER,                       //1580
         MSG_COIN_DISPENSER,                    
         MSG_MEDIA_STATUS,                         //1582
         MSG_CURRENCYLOWPERCENTAGE,              //1583 tar 298689
         MSG_RECEIPTFORMAT_GENERICHEADERCOUNT,              
         MSG_RECEIPTFORMAT_GENERICHEADERVALUE,              
         MSG_RECEIPTFORMAT_GLORYHEADER,          //1586
         MSG_CONFIRM_IGNORE_CHANGES_BILL,        //1587
         MSG_COIN_DUMP_FAILURE,                  //1588
         MSG_COIN_OUT,                           //1589
         LTT_COINHOPPER_JAM,                     //1590
         MSG_COIN_DISCONNECT,                    //1591
         MSG_COINHOPPER_JAM,                     //1592
         MSG_REMOVE_COINS,                       //1593
         MSG_COINACCEPTOR_STATUS_FAILURE_3x,     //1594 - TAR 297103
         LTT_PERSONALIZATION_SAVE_PREF,          //1595
         MSG_PERSONALIZATION_SAVE_PREF,          //1596
         MSG_LOYALTY_CARD,                       //1597
         MSG_PERSONALIZATION_IN_EFFECT,          //1598 
         MSG_RAP_PERSONALATION_LOOKUP_FAILURE,   //1599

         MSG_CARDDISPENSER_STATUS,                 //1600
         MSG_CARDDISPENSER_BIN_LOW,                     //1601
         MSG_CARDDISPENSER_BIN_LOW_PERCENTAGE,         //1602
         MSG_CARDDISPENSER_BIN_NORMAL,                 //1603
         MSG_CARDDISPENSER_BIN_NORMAL_PERCENTAGE,     //1604
         MSG_CARDDISPENSER_BIN_EMPTY,                 //1605
         MSG_CARDDISPENSER_REJECT_NORMAL,             //1606
         MSG_CARDDISPENSER_REJECT_NORMAL_PERCENTAGE, //1607
         MSG_CARDDISPENSER_REJECT_EMPTY,             //1608
         MSG_CARDDISPENSER_REJECT_NEAR_FULL,         //1609
         MSG_CARDDISPENSER_REJECT_NEAR_FULL_PERCENTAGE,         //1610
         MSG_CARDDISPENSER_REJECT_FULL,                         //1611
         MSG_CARDMANAGEMENT,                         //1612
         MSG_CARDMANAGEMENT_REJECT_BIN,                 //1613
         MSG_CARDMANAGEMENT_DISPENSER_BIN,             //1614
         MSG_CARDMANAGEMENT_LEADTHRUTEXT,                 //1615
         MSG_ENTER_LOAD,                             //1616
         MSG_REENTER_LOAD,                             //1617
         MSG_CASSETTE_OUT_GENERIC,       //1618
         RA_LANEBTT_SUSPEND,                //1619    RFC 294962
         MSG_SAFEPAY_UNLOCK_NOTECOVER,        //1620    
         MSG_SAFEPAY_UNLOCK_CASSETTE,        //1621    
         MSG_SAFEPAY_CASSETTE,                //1622    
         MSG_SAFEPAY_NOTE_PICKUP,            //1623    
         MSG_SAFEPAY_NOTE_TRANSFER,            //1624    
         MSG_SAFEPAY_INSERT_UNLOAD_COIN,    //1625    
         MSG_SAFEPAY_INSERT_CASH,            //1626    
         MSG_SAFEPAY_BIT29_TITLE,            //1627    
         MSG_SAFEPAY_BIT29_TITLE2,            //1628    
         MSG_SAFEPAY_BIT29_TEXT,            //1629    
         MSG_SAFEPAY_BIT20_TITLE,            //1630    
         MSG_SAFEPAY_BIT20_TITLE2,            //1631    
         MSG_SAFEPAY_BIT20_TEXT,            //1632    
         MSG_SAFEPAY_BIT16_TITLE,            //1633    
         MSG_SAFEPAY_BIT16_TITLE2,            //1634    
         MSG_SAFEPAY_BIT16_TEXT,            //1635    
         MSG_SAFEPAY_BIT24_TITLE,            //1636    
         MSG_SAFEPAY_BIT24_TITLE2,            //1637    
         MSG_SAFEPAY_BIT24_TEXT,            //1638    
         MSG_SAFEPAY_BIT25_TITLE,            //1639    
         MSG_SAFEPAY_BIT25_TITLE2,            //1640    
         MSG_SAFEPAY_BIT25_TEXT,            //1641    
         MSG_SAFEPAY_BIT18_TITLE,            //1642        
         MSG_SAFEPAY_NOTE_COVER_T1,            //1643    
         MSG_SAFEPAY_NOTE_COVER_T2,            //1644    
         MSG_SAFEPAY_NOTE_COVER_MSG,        //1645
         MSG_TRANSUSPENDED,                    //1646  RFC 276161
         LTT_MONTH_DAY_YEAR,                //1647  TAR 330874
         LTT_MONTH_DAY_YEAR_NOCENTURY,        //1648  TAR 330874 
         LTT_DAY_MONTH_YEAR,                //1649  TAR 330874 
         LTT_DAY_MONTH_YEAR_NOCENTURY,        //1650  TAR 330874 
         LTT_YEAR_MONTH_DAY,                //1651    TAR 330874    
         LTT_YEAR_MONTH_DAY_NOCENTURY,        //1652  TAR 330874 
         MSG_EXCESS_LOAD_AMT,               //1653 TAR323136
         MSG_EXCESS_LOAD_COUNT,                //1654 TAR323136
         TTL_DEPT_SALE,                     //1655 RFQ 986
         LTT_DEPT_SALE,                     //1656 RFQ 986
         LTT_INVALID_DEPT,                  //1657 RFQ 986
     TXT_OVERDISPENSE,             //1658 RFC 332822
         BTT_PRINT_RECEIPT,                    //1659
         MSG_SELECT_PRINT_FUNCTION,            //1660
         MSG_UNABLE_PRINT_RECEIPT,            //1661
         BTT_PRINT_OTHERRECEIPT,            //1662
         MSG_BINISEMPTYORJAMMED,            //1663
         MSG_SAFEPAY_BIT18_TITLE2,            //1664        
         MSG_SAFEPAY_BIT18_TEXT,            //1665        
         MSG_SAFEPAY_BIT17_TITLE,            //1666        
         MSG_SAFEPAY_BIT17_TITLE2,            //1667        
         MSG_SAFEPAY_BIT17_TEXT,            //1668        
         MSG_SAFEPAY_BIT13_TITLE,            //1669        
         MSG_SAFEPAY_BIT13_TITLE2,            //1670        
         MSG_SAFEPAY_BIT13_TEXT,            //1671        
         MSG_SAFEPAY_BIT26_TITLE,            //1672        
         MSG_SAFEPAY_BIT26_TITLE2,            //1673        
         MSG_SAFEPAY_BIT26_TEXT,            //1674        
         MSG_SAFEPAY_BIT28_TITLE,            //1675        
         MSG_SAFEPAY_BIT28_TITLE2,            //1676        
         MSG_SAFEPAY_BIT28_TEXT,            //1677        
         MSG_SAFEPAY_BIT21_TITLE,            //1678        
         MSG_SAFEPAY_BIT21_TITLE2,            //1679        
         MSG_SAFEPAY_BIT21_TEXT,            //1680        
         MSG_SAFEPAY_BIT23_TITLE,            //1681        
         MSG_SAFEPAY_BIT23_TITLE2,            //1682        
         MSG_SAFEPAY_BIT23_TEXT,            //1683        
         MSG_SAFEPAY_BIT27_TITLE,            //1684        
         MSG_SAFEPAY_BIT27_TITLE2,            //1685        
         MSG_SAFEPAY_BIT27_TEXT,            //1686        
         MSG_CASHRECYCLER_EMPTY_2,            //1687        
         MSG_SAFEPAY_CASSETTE_ERROR,        //1688 
         MSG_SAFEPAY_CASSETTE_UNLATCH_TITLE,//1689        
         MSG_SAFEPAY_CALLSERVICE,            //1690        
         MSG_CASHMANAGEMENT,                //1691         
         MSG_SAFEPAY_SUPERVISOR_LOGIN,        //1692 
         BUTTON_RA_HOPPER_ERROR,            //1693
         MSG_HOPPER_SUBSTITUTION_CM,        //1694
         BUTTON_FIX_LATER,                    //1695
         BUTTON_CONDITITON_CORRECTED,       //1696
         BUTTON_CLEARED_JAM,                //1697
         TXT_TOUCH_CONDITITON_CORRECTED,    //1698
         MSG_HOPPER_SUBSTITUTION,           //1699
         MSG_BOTH_NOT_DISPENSE_HOPPER,      //1700
         MSG_SAFEPAY_FAIL_TO_DISPENSE,        //1701
         TXT_DUPLICATE_RECEIPT,                //1702 - RFC 334227
         MSG_LOAD_SIGN,                     //1703 - TAR 351153
         MSG_NCR_E5633_COMM_FAILURE,        //1704 - TAR 353071
         LTT_NCR_E5633_COMM_FAILURE,        //1705 - TAR 353071 
         MSG_SAFEPAY_DEVICE_DISABLED,        //1706 - TAR 356035
         MSG_COINACCEPTOR_JAM_T2,            //1707 - TAR 354368
         MSG_COINACCEPTOR_JAM_TXT,            //1708 - TAR 354368
             MSG_DONOTINSERTCASHCOIN,            //1709 - RFC 330995
          TTL_TAKEYOURRETURNCASH,            //1710 - RFC 330995
         RA_LNBTN_SUSPENSIONDECLINE_EVENT,    //1711 - TAR 352409 
         RA_LNWND_SUSPENSIONDECLINE,        //1712 - TAR 352409    
         RA_SUMINST_SUSPENSIONDECLINE,    //1713 - TAR 352409    
         MSG_DATANEEDED_SESPENSIONDECLINE,    //1714 - TAR 352409    
         MSG_RA_CAT_SUSPENSIONDECLINED,    //1715 - TAR 352409
         TXT_WEIGHTEXPECTED_ABBREVIATED,    //1716
         TXT_WEIGHTOBSERVED_ABBREVIATED,    //1717
         TXT_NUMBEROFITEMS_ABBREVIATED,        //1718
         TXT_WEIGHTADDED_ABBREVIATED,        //1719
         TXT_WEIGHTREMOVED_ABBREVIATED,        //1720
         MSG_TRAININGMODE_TRANS,            //1721 - Training Mode Transaction
         MSG_INTRAINING_STARTTRANS,        //1722 - Lane is in training mode, continue with the transaction?
          MSG_TM_TRANSACTION,                //1723 - Training Trx
         MSG_DATANEEDED_TRAININGMODE_TRANS, //1724 - TRAINING MODE TRANSACTION
         RA_SECONDLINE_SECURITYDISABLED,    //1725
         BTT_FINISH_LOOKUP,                    //1726 
         MSG_LOOKUP_TITLE1,                    //1727
         MSG_LOOKUP_TITLE2,                    //1728
         MSG_LOOKUP_NUMBER_ITEMS,            //1729
         BTT_SEARCH,                        //1730
         PL_IMAGES_LOADING,                    //1731
         PL_IMAGES_LOADED,                    //1732
         MSG_SELECT_GOBACK,                    //1733

         //SR085 - Messages for SR085 - Improve Bag/Box Behavior
         BTT_OWNBAG,                        //1734
         LTT_PUTBAGONSCALE,                 //1735
         MSG_PUTBAGONSCALE,                 //1736
         RA_PUTBAGONSCALE,                  //1737   
         LTT_BAGBOXNOTVALID,                //1738                       
         MSG_BAGBOXNOTVALID,                //1739
         RA_BAGBOXNOTVALID,                 //1740
         RA_INVALIDBAGINFO,                 //1741 
         MSG_DATANEEDED_BAGBOXNOTVALID,     //1742
          //+SR93 Biometrics
         MSG_FINGERPRINT_LOGIN,                //1743
         MSG_INVALID_FINGERPRINT,            //1744         
         MSG_FINGERPRINT_DEVICE_ERROR,        //1745
         MSG_FINGERPRINT_SERVER_ERROR,      //1746
         MSG_FINGERPRINT_ERROR_TITLE,        //1747
         LTT_INVALID_FINGERPRINT,            //1748
         LTT_FINGERPRINTPROCESSING,            //1749
         MSG_FINGERPRINTPROCESSING,            //1750
         LTT_WEIGHTSCALE_OVERCAPACITY,      //1751
         MSG_REMOVEDALLWT_FROMBAGSCALE,     //1752
         //-SR93 Biometrics
         MSG_ITEMNOTFOUND,                  //1753
         MSG_NOMATCHTRYAGAIN,               //1754
         MSG_SEARCHFROMLISTTITLE,           //1755
         MSG_SEARCHFROMLISTINSTRUCTION,     //1756
         MSG_KEYINACCENTLETTERSINSTRUCTION, //1757
         //+SR631.1
         MSG_COUPONSENSOR_BLOCKED_T1,        //1758 - RFC 631.1
         MSG_COUPONSENSOR_BLOCKED_T2,        //1759 - RFC 631.1
         MSG_COUPONSENSOR_BLOCKED_TEXT,        //1760 - RFC 631.1
         MSG_COUPONSENSOR_EXCEED_TEXT,        //1761 - RFC 631.1
         //-SR631.1
         MSG_CM_SCALEIMAGE,                    //1762
         MSG_CM_KGSCALEIMAGE,                //1763
         MSG_SM_SCALEIMAGE,                    //1764
         MSG_SM_KGSCALEIMAGE,                //1765
         MSG_SCANNER_SCALE_ERROR,            //1766
         MSG_SCANNER_SCALE_WTPARAM_FAILURE,    //1767
         MSG_SCANNER_SCALE_WTUNIT_UNSUPPORTED,    //1768
         BTT_KEYINCODE,                    //1769, copied from 569 for button key in code
        MSG_SEARCHFROMLISTINSTRUCTION2,        //1770 -multipick=y instruction
        //+SR678
         MSG_COUPONTOOHIGH,                    //1771
         MSG_MAXCOUPONLIMIT,                //1772 
         TXT_INSTR_MAXCOUPONLIMIT,            //1773
         MSG_DATANEEDED_MAXCOUPONLIMIT,        //1774
         TXT_APPROVE_MAXCOUPONLIMIT,        //1775 
         TXT_STOREAUTH_MAXCOUPONLIMIT,        //1776
         MSG_COUPONEXCEED_FINALIZATION,        //1777
         TXT_CLEARSECURITY_FINALIZATION,    //1778
         //-SR678
        MSG_CANCELITEMCPN_T2,               //1779, copy of 263 but took out "\n\n"
         //+ TAR 391995
        MSG_DATANEEDED_STARTTRANSACTION,    //1780
        MSG_DATANEEDED_THANKYOU_EXPIRED,    //1781
        MSG_DATANEEDED_STARTTRANSACTION_AM,    //1782
        MSG_DATANEEDED_SAFETYCOVER_OPEN,    //1783
        MSG_DATANEEDED_MULTIPACK_ITEM,        //1784
        //- TAR 391995
        LTT_INVALID_FINGERPRINT_LOGIN,      //1785  //TAR395474
        //+SR713
        LTT_BAGSALE,                        //1786
        MSG_BAGSALEPROMPT,                    //1787
        MSG_BAGPRICE,                        //1788                            
        //-SR713
        MSG_RA_CAT_COUPON,                  //1789    //TAR400905 
        //SR742+
        LTT_CREDIT_DEBIT_ONLY,                //1790    
        LTT_CASHONLYMODE,                    //1791    
        LTT_CARDONLYMODE,                    //1792 
        MSG_CREDITDEBITCARDMODE,            //1793
        MSG_CASHONLYMODE,                    //1794    
        TXT_RESTRICT_CARDS_ONLY,            //1795
        TXT_RESTRICT_CASH_ONLY,                //1796
        MSG_DEVICEERRORNOTFIXED_CASH_ONLY,  //1797 
        RAP_MSG_CASHONLYMODE,                //1798
        RAP_MSG_CARDONLYMODE,                //1799
        MSG_SELECTMODE_OPERATIONS,            //1800
        MSG_MODE_OPERATION,                    //1801
        BTT_ACCEPT_CARDONLY_MODE,            //1802
        BTT_ACCEPT_CASHONLY_MODE,            //1803
        BTT_EXIT_CARDONLY_MODE,                //1804
        BTT_EXIT_CASHONLY_MODE,                //1805    
        MSG_GIVE_CHANGE_MANUAL,                //1806 
        //SR742-
        MSG_INVALIDWEIGHT,                    //1807 //tar 407085  
        MSG_SELECTITEMONRECEIPT,            //1808 // tar414311
        LTT_BCR_DISPENSE_ERROR,                //1809 +Hopper Error Messages Improvement  G2
        LTT_BCR_COINS_STATUS_JAM,            //1810
        MSG_BCR_COINS_STATUS_JAM,            //1811
        LTT_BCR_COINS_JAM_BINS_FULL_OR_OUT,    //1812
        MSG_BCR_COINS_JAM_BINS_FULL_OR_OUT,    //1813
        LTT_BCR_HOPPERBIN_OUT_OR_UNSEATED,  //1814
        MSG_BCR_HOPPERBIN_OUT_OR_UNSEATED,    //1815
        LTT_BCR_COINSJAM_OR_DOOR_OPEN,        //1816
        MSG_BCR_COINSJAM_OR_DOOR_OPEN,        //1817
        MSG_BCR_ACCEPTOR_ERROR,                //1818
        LTT_BCR_ACCEPTOR_ERROR,                //1819 -Hopper Error Message Improvement
        MSG_RESETSCALE_5X,                    //1820
        BTT_ACCEPTWEIGHT,                   //1821 SR814.2
        MSG_ENTERWEIGHT,                    //1822 SR814.2
        MSG_DIGITAL_RECEIPT_GENERATED,      //1823 SR Personalization Digital Receipt
        MSG_DIGITAL_RECEIPT_NOT_GENERATED,  //1824 SR Personalization Digital Receipt
        LTT_SAVE_PREFERENCES,               //1825 SR815.5 Personalized Volume
        MSG_SAVE_PREFERENCES,               //1826 SR815.5 Personalized Volume
        MSG_PREFERRED_LANGUAGE,             //1827 SR815.5 Personalized Volume
        MSG_PREFERRED_VOLUME,               //1828 SR815.5 Personalized Volume
        BTT_APPROVE,                         //1829 Personalization Age Prompt
        MSG_SMM_WLDBCOLUMNS2,                //1830 TAR 423665
        MSG_ABBREV_INTERPOLATED,            //1831 TAR 423665
        MSG_ABBREV_EXCEPTIONDEPT,           //1832 TAR 423665
        MSG_ABBREV_EXCEPTIONTB,             //1833 TAR 423665
        MSG_ABBREV_MAXERROR,                //1834 TAR 423665
        MSG_INVALIDBARCODE,                    //1835 +SR879
        MSG_DATANEEDED_INVALIDBARCODE,        //1836 
        TXT_INVALIDBARCODE,                    //1837
        MSG_INVALIDCODE,                    //1838 
        MSG_WAITFORCASHIER,                    //1839 -SR879
        MSG_STORE_NUMBER,                   //1840 -SR834
        LTT_DATANEEDED_GO_TO_LANE,          //1841 -SR831
        BTT_FULLASSIST,                     //1842 SR819 
        BTT_OWNBAG_SAB,                     //1843 -SR880
        BTT_SEARCH_SAB,                     //1844 -SR841
        TXT_RECEIPT,                        //1845 -SR804
        LTT_SMM_SCAN_OR_KEYIN,                //1846 -SR821
        MSG_SMM_ITEMADDED_KEYIN,            //1847 -SR821
        MSG_SMM_ITEMDELETED_KEYIN,            //1848 -SR821
        MSG_ENTERWEIGHTASSIST,                //1849
        LTT_DEVICESINERROR,                 //1850 SSCOB-517
        BTT_DO_NOT_APPROVE,                 //1851 -SR803
        MSG_REMOVE_ITEM_FROM_BAGSCALE,      //1852 -SR803
        LTT_BIRTHDATE_TOUCH_ENTER,          //1853 -SR803
        BTT_OPTIONAL,                       //1854 -SR078
        MSG_SMM_O,                          //1855 -SR078
        MSG_SMM_DONOTBAG,                    //1856
        MSG_SMM_DONOTPUT_ONBELT,            //1857
        MSG_CASH_ABNORMAL_BALANCE,          //1858 Add Glory CM Support
        LTT_AM_PURCHASEFULLCRATE,            //1859 (see 802). Must be the same message minus the "\n"s
		LTT_PICKLISTASSISTED,				//1860
        MSG_DATANEEDED_PLA_VALIDATION,       //1861 SSCOADK-260
        MSG_WEIGHTITEM_VALIDATION,           //1862 SSCOADK-260
        LTT_WEIGHTITEM_VALIDATION,           //1863 SSCOADK-260
        BTT_GO_TO_LANE,                      //1864
        BTT_LOOKUP_WITHOUT_WEIGHT,           //1865
        BTT_MORE_POPULAR,                    //1866
        MSG_HANDINVIEW_VALIDATION,           //1867
        LTT_HANDINVIEW_VALIDATION,           //1868
        LTT_HANDINVIEW,                      //1869
        MSG_1870,                               //1870 SecureCam
        MSG_1871,                               //1871 SecureCam
        MSG_1872,                               //1872 SecureCam
        MSG_1873,                               //1873 SecureCam
        MSG_1874,                               //1874 SecureCam
        MSG_EXPECTED_WEIGHT,                    //1875 SecureCam
        MSG_OBSERVED_WEIGHT,                    //1876 SecureCam
        RA_ALERT_RECEIPT_MESSAGE,               //1877 SecureCam
        MSG_SC_MISMATCH_TOPCAPTION,             //1878 SecureCam
        MSG_SC_MISMATCH_INSTRUCTION,            //1879 SecureCam
        MSG_SC_UNEXPECTEDINCREASE_TOPCAPTION,   //1880 SecureCam
        MSG_SC_UNEXPECTEDINCREASE_INSTRUCTION,  //1881 SecureCam
        MSG_SC_UNSOLICITED_TOPCAPTION,          //1882 SecureCam
        MSG_SC_UNSOLICITED_INSTRUCTION,         //1883 SecureCam
        RA_SC_LINE1,                            //1884 SecureCam
        RA_SC_LINE2,                            //1885 SecureCam
        RA_SC_LINE3,                            //1886 SecureCam
        MSG_SC_TRUSTLEVEL_TOPCAPTION,           //1887 SecureCam
        MSG_SC_TRUSTLEVEL_INSTRUCTION,          //1888 SecureCam
        RA_SC_SECURITY_TRUST_LEVEL,             //1889 SecureCam
		LTT_SECURITYANALYSIS,					//1890 SecureCam
		MSG_SECURITYANALYSIS,					//1891 SecureCam
		RA_SECURITYANALYSIS,					//1892 SecureCam
		LTT_INVALIDBARCODESCAN,                //1893 SSCOADK-1210
		MSG_INVALIDBARCODESCAN,                //1894 SSCOADK-1210
        MSG_RA_CAT_GENERICDELAYED,             //1895 GenericDelayed
		TTL_LOADING_TRANSACTION,               //1896
        MSG_LOADING_TRANSACTION,               //1897
        TTL_CUSTOMER_AUDIT,                    //1898
        MSG_MOBILE_AUDIT_LANEBUTTON,           //1899
        TTL_ATTENDANT_AUDIT,                   //1900
        MSG_ATTENDANT_AUDIT,                   //1901
        MSG_RECALLMODE_BEGIN,                  //1902
        MSG_RECALLMODE_END,                    //1903
        MSG_NUMBER_ITEMS_RECALLED,             //1904
        MSG_ATTRACTTEXTMOBILE,                 //1905
        TTL_RECALL_COMPLETE,                   //1906
        MSG_RECALL_PLEASE_SCAN_MORE_ITEMS,     //1907
        MSG_GD_QUANTITY_RESTRICTED,            //1908

		// SSCOI-44529: Relocated this PS_MAXTEXT line here.
		// insert new messages before PS_MAXTEXT
        PS_MAXTEXT,                          // must be last enum

// RFQ 7477
		//******DO NOT ADD ANY NEW STRINGS AFTER PS_MAXTEXT
		//******THIS WAS DONE TO PRESERVE THE 2ND PRINTER STRINGS 
		//******AND TO AVOID FUTURE ADK MERGE PROBLEMS
		LTT_PAY_CHECK_ON_LANE = SCUD_BASE + 220,		//9220 - Begin RFC 356020 
		MSG_INSERT_CHECK_ON_LANE,						//9221
		LTT_MICR_ERR_TITLE1,							//9222
		LTT_MICR_ERR_TITLE2,							//9223
		MSG_MICR_NO_DATA,								//9224
		MSG_MICR_TIMEOUT,								//9225
		MSG_MICR_UNEXPECTED,							//9226
		MSG_MICR_NO_HARDWARE,							//9227
		MSG_MICR_JAM,									//9228
		MSG_MICR_LINE_ERROR,							//9229
		MSG_MICR_NO_CHECK,								//9230
		MSG_MICR_BAD_CHECK,								//9231
		RA_CHECK_ON_LANE,								//9232
		MSG_CHECK_DEVICE_NOT_AVAILABLE,					//9233
		MSG_MICR_CHECK_JAM,								//9234
		MSG_MICR_GENERAL_FAILURE,						//9235
		MSG_MICR_TIMEOUT_TEXT,							//9236
		MSG_MICR_NOT_CONNECTED,							//9237 - End RFC 356020
// RFQ 7477
		LTT_MEIJERCARD_VERIFICATION = SCUD_BASE+ 541,	//9541 - CRD 526693
        
};
    
const enum PSTEXTWAVE
{
        PS_NOSOUND,
        WELCOMEPSB,                        //  1
        THANKYOUFORUSING,                //  2
        FREQSHOPACCT,                    //  3
        AFTERSIGNING,                    //  4
        APPROVALNEEDED,                    //  5
        BTTNINVALID,                    //  6
        CANCELALLITEMS,                    //  7
        CANCELITEMS,                    //  8
        CARDNOTACCEPTED,                //  9
        CARDSWIPEDPLEASECONTINUE,        // 10 
        CONFIRMAMOUNT,                    // 11
        CONFIRMCANCEL,                    // 12
        CONFIRMQTY,                        // 13
        COUPONACCEPTED,                    // 14
        COUPONSENDOFPURCHASE,            // 15
        COUPONSNOTACCEPTED,                // 16
        DEPOSITCOUPONS,                    // 17
        DOYOUWISHTOCONTINUE,            // 18
        ENTERCOUPONVALUE,                // 19
        ENTERYOURPIN,                    // 20
        EXCEPTIONITEM,                    // 21
        INSERTCARD,                        // 22
        INSERTCASH,                        // 23 - NOT USED  TAR# 235222
        INSERTCASHCARD,                    // 24 - NOT USED  TAR# 235222
        INSERTCASHORSLIDECARD,            // 25 - NOT USED  TAR# 235222
        ITEMCANCELED,                    // 26
        ITEMPROCESSED,                    // 27
        ITEMVOIDED,                        // 28
        KEYINCODETOUCHENTER,            // 29
        KEYPADINVALID,                    // 30
        KEYPADVALID,                    // 31
        PICKLISTINVALID,                // 32
        PICKLISTVALID,                    // 33 - NOT USED  TAR# 235222
        PLACEITEMONSCALE,                // 34
        PURCHASEPRODUCE,                // 35
        QUANTITYNEEDED,                    // 36
        RECALLEDITEM,                    // 37
        RECEIPTPRINTBELOWPAYMENTDEVICE, // 38 - NOT USED  TAR# 235222
        RESTRICTEDITEM,                    // 39
        SCALENOTWORKING,                // 40
        SCANCOUPONS,                    // 41
        SECURITYLOW,                    // 42
        SECURITYMED,                    // 43
        SELECTAMT,                        // 44
        SELECTCARD,                        // 45
        SELECTCONTAINER,                // 46
        SELECTFROMLIST,                    // 47
        SELECTTYPE,                        // 48
        SETALLITEMSASIDE,                // 49
        SETCOUPONSASIDE,                // 50
        SELECTLANGUAGE,                    // 51
        SETREFUNDDISCOUNTASIDE,            // 52
        SIGNRECEIPTWAITAPPROVAL,        // 53
        SIGNSIGNATUREPAD,                // 54
        SLIDEYOURCARD,                    // 55
        TAKECARD,                        // 56
        TAKECHANGE,                        // 57
        TAKEINVALIDLOYALTYCARD,            // 58
        TAKELOYALTYCARD,                // 59
        TAKERECEIPTCOUPONDROP,            // 60
        TAKEYOURCARD,                    // 61
        TOUCHSTART,                        // 62
        TRANSACTIONLIMITEXCEEDED,        // 63
        TOUCHOK,                        // 64   RFC 294962
        TYPEITEMSDESCRIPTION,            // 65
        UNKNOWNITEM,                    // 66
        UNKNOWNPRICE,                    // 67
        UNMATCHEDCOUPON,                // 68
        INSERTCASHPAYWITHCARD,          // 69
        USEPINPAD,                      // 70
        SETITEMASIDECONTINUEWITHPURCHASES,    //71
        TENDERCANCELLED,                    //72
        SELECTPAYMENTOPTION,                //73 - NOT USED  TAR# 235222
        SLIDECARDONPAYMENTDEVICE,            //74
        SLIDECARDONSIDEOFMONITOR,            //75
        PASSITEMSBARCODEBELOWREDLINE,        //76 - NOT USED  TAR# 235222
        SELECTFROMFAVORITES,                //77
        SIGNSIGNATUREPAD_OKWITHPEN,            //78
        CALLFORHELP,                        //79
        COUPONSVALUETOOHIGH,                //80
        HELPONTHEWAY,                        //81
        CARDACKNOWLEDGEDSELECTCARDTYPE,     //82
        QUANTITYLIMITREACHED,                //83
        INSERTCASHSELECTPAYMENTTYPE,        //84
        INSERTCASHSELECTPAYMENTTYPEFREQSHOPPER, //85
        CANNOTFINDSCANNEDITEM,                //86
        SELECTPAYMENTTYPE,                    //87
        UNKNOWNITEMSETASIDETOUCHOK,         //88 
        LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD,  //89
        CHECKBASKETREMINDER,                //90
        PURCHASECRATE,                        //91
        SELECTPAYMENTTYPEFREQSHOPPER,        //92
        THANKYOUSECURITYTAGREMINDER,            //93
        SCANYOURCARD,                       //94
        THANKYOUGIFTCARD,                   //95 - NOT USED  TAR# 235222
        WAITFORASSISTANCE,            //96
        TYPEJOBINFO,                //97
        ENTERGIFTCARDVALUE,        //98 - NOT USED  TAR# 235222
        GIFTCARDVALUETOOHIGH,      //99 - NOT USED  TAR# 235222
        GIFTCARDAPPLIEDBALANCEREMAIN, //100 - NOT USED  TAR# 235222
        BAGITEM,                            //101
        BAGITEMSENSORMATIC,                    //102
        SEC_UNEXPECTEDITEM,                    //103
        SEC_INCORRECTITEM,                    //104 - NOT USED  TAR# 235222
        SEC_PICKINGUPITEMS,                    //105
        SEC_REMOVEDITEM,                    //106
        PICKINGUPITEMS,                        //107 - NOT USED  TAR# 235222
        ASSISTANCENEEDED,                    //108
        SEC_PLSTAKEYOURITEMS,                //109
        PLACEITEMONBELT,                    //110
        SEC_UNEXPECTEDITEMONBELT,           //111
        SEC_INCORRECTITEMONBELT,            //112 - NOT USED  TAR# 235222
        FREQUENTSHOPPERCARD,                //113 
        WELCOME_BELT,                       //114 - NOT USED  TAR# 235222
        LOYALTYCARDREMINDERAFTERFIRSTITEMSOLD_BELT,  //115
        BTTNVALID,                            //116
        SEC_BAGAREABACKUP,                    //117
        INSERTCOUPON,                       //118
        ENTERCASHBACKAMOUNT,                //119 - NOT USED  TAR# 235222
        INFORMATIONNEEDED,                  //120
        CHECKTOPAYSTATION,                    //121
        PAYMENTTOPAYSTATION,                //122
        SEC_ITEMREMOVEDONREMOVEVIOLATION,   //123 CHU 072802 RFC208874
        CARDACCEPTEDPLSCONTINUE,            //124    Generic for motorized card reader 
        TOUCHNEXTPRODUCE,                    //125    Touch Next Button For Produce 
        TAKECHANGEBELOWSCANNER,             //126 merged tar 232798 to 3.1.1 - fixed by Chen
        SKIPBAGFORLARGE,                    //127 RFC 210367 FFR: Add Skip bag Wave
        SKIPBELTFORLARGE,                    //128 RFC 210367 FFR: Add Skip Belt Wave
        SCANVOUCHERS,                       //129 RFC 213717
        PLEASERESCANITEM,                   //130 TAR238515
        DEPOSITYOURCARD,                    //131
        GIFTCARDAPPLLIEDBALANCEREMAINING,    //132
        PAYMENTBALANCE,                        //133
        SWIPECARDAGAIN,                        //134
        VOIDTRANS_SETALLITEMSASIDE,            //135    RFC 276161
        SUSPENDTRANS_SETALLITEMSASIDE,        //136    RFC 276161
        PUTBAGONSCALE,                      //137 SR085 - audio for new PutBagOnScale screen
        VERIFYBAGS,                         //138 SR085 - audio for new SecBagBoxNotValid screen
        VALIDBIOMETRIC,                        //139 SR93 - Biometrics
        SELECTFROMPOPULAR,                    //140 TAR 396117 - ji/40g339 - picklist screen is playing wrong audio.
        INSERTCASHPAYMENT,                    //141 TAR 412065
        ACCEPTWEIGHTBEEP,                     //142  SSCOADK-1148	
        ONSCREENSIGNSIGNATURE,                //143  SSCOADK-2116
        REMOVEITEMFROMPRODUCESCALE,           //144
        INSERTCASHORPAYANOTHERWAY,            //145 SSCOB-16841
        TAKERECEIPT,                           //146 - Audio for Take Receipt
        WAVE_MAXID                            // Add new wave id above this WAVE_MAXID
};
    
const enum WORKMSGS
{
      PS_BLANK = PS_MAXTEXT,          // " "
      PS_WORK1,                          // defined as needed
      PS_WORK2,                       // defined as needed
      PS_WORK3,                       // defined as needed
      PS_WORK4,                       // defined as needed
      PS_WORK5,                       // defined as needed
      PS_WORK6,                       // defined as needed
      BTT_TARE1,                      // not from SCOTMSGS
      BTT_TARE2,                      // not from SCOTMSGS
      WORKMSGS_LAST
};

/*  Used by setMascotImage to decide which frame to update the 
    mascot image in */
const enum FRAMES
{
        DEFAULT_FRAME = 1,
        KEYBOARD_FRAME,
        SCROLL_FRAME,
        ECHO_FRAME,
        CANCELITEM_FRAME,
        DEFAULT_FRAME_WITHOUTECHO,
        STORE_MODE_FRAME,
        ASSIST_MODE_FRAME,
        SCROLL_FRAME_WITHOUT_BUTTONS,
        PRODUCE_FRAME,   
        PRODUCE_FAVORITES_FRAME,
        ATM_FRAME,
        FINALIZATION_FRAME,
        ALPHANUMERIC_FRAME,
        ASSISTANCE_NEEDED_FRAME
};
     
const enum CONTEXT_TYPE {
  PRIMARYCONTEXT,
  NONEPRIMARYCONTEXT,
  BOTHCONTEXT,
};

#ifndef NO_CM_SPECIFIC_GARBAGE

#define SetCMLeadthruText   SetLeadthruText
#define SetCMButton1Text(x) SetButtonText(1, x)
#define SetCMButton2Text(x) SetButtonText(2, x)
#define SetCMButton3Text(x) SetButtonText(3, x)
#define SetCMButton4Text(x) SetButtonText(4, x)
#define SetCMBackground     SetBackground
#define CMButton            Button
#define AMButton            Button
#define SMButton            Button
#define CMButtonFlash       ButtonFlash
#define LeadThruText        SetLeadthruText
#define SetLTTinto2         SetLTTinto2
#define ShowCMFrame         ShowFrame
#endif   


#define STR_SEPARATOR       _T("@@")    // Used as a string separator: see TAR 273325 BMediano   

#define SIGCAP_WINDOWCLASS _T("SigCapWindowClass")

#endif // _PSPROCEDURES_CONSTANTS_H
