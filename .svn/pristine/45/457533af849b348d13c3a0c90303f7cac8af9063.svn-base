Rem *///////////////////////////////////////////////////////////////////
Rem *///////////////////////////////////////////////////////////////////
Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * OPOSALL.BAS
Rem *
Rem *   Includes all of the .BAS include files.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 96-03-18 OPOS Release 1.01                                    CRM
Rem * 96-04-22 OPOS Release 1.1                                     CRM
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////
Rem *///////////////////////////////////////////////////////////////////
Rem *///////////////////////////////////////////////////////////////////



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From Opos.h
Rem *
Rem *   General header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *   Add OPOS_FOREVER.
Rem *   Add BinaryConversion values.
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * OPOS "State" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_S_CLOSED& = 1
Public Const OPOS_S_IDLE& = 2
Public Const OPOS_S_BUSY& = 3
Public Const OPOS_S_ERROR& = 4


Rem *///////////////////////////////////////////////////////////////////
Rem * OPOS "ResultCode" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOSERR& = 100
Public Const OPOSERREXT& = 200

Public Const OPOS_SUCCESS& = 0
Public Const OPOS_E_CLOSED& = 1 + OPOSERR
Public Const OPOS_E_CLAIMED& = 2 + OPOSERR
Public Const OPOS_E_NOTCLAIMED& = 3 + OPOSERR
Public Const OPOS_E_NOSERVICE& = 4 + OPOSERR
Public Const OPOS_E_DISABLED& = 5 + OPOSERR
Public Const OPOS_E_ILLEGAL& = 6 + OPOSERR
Public Const OPOS_E_NOHARDWARE& = 7 + OPOSERR
Public Const OPOS_E_OFFLINE& = 8 + OPOSERR
Public Const OPOS_E_NOEXIST& = 9 + OPOSERR
Public Const OPOS_E_EXISTS& = 10 + OPOSERR
Public Const OPOS_E_FAILURE& = 11 + OPOSERR
Public Const OPOS_E_TIMEOUT& = 12 + OPOSERR
Public Const OPOS_E_BUSY& = 13 + OPOSERR
Public Const OPOS_E_EXTENDED& = 14 + OPOSERR


Rem *///////////////////////////////////////////////////////////////////
Rem * OPOS "BinaryConversion" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_BC_NONE& = 0
Public Const OPOS_BC_NIBBLE& = 1
Public Const OPOS_BC_DECIMAL& = 2


Rem *///////////////////////////////////////////////////////////////////
Rem * "CheckHealth" Method: "Level" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_CH_INTERNAL& = 1
Public Const OPOS_CH_EXTERNAL& = 2
Public Const OPOS_CH_INTERACTIVE& = 3


Rem *///////////////////////////////////////////////////////////////////
Rem * "ErrorEvent" Event: "ErrorLocus" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_EL_OUTPUT& = 1
Public Const OPOS_EL_INPUT& = 2
Public Const OPOS_EL_INPUT_DATA& = 3


Rem *///////////////////////////////////////////////////////////////////
Rem * "ErrorEvent" Event: "ErrorResponse" Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_ER_RETRY& = 11
Public Const OPOS_ER_CLEAR& = 12
Public Const OPOS_ER_CONTINUEINPUT& = 13


Rem *///////////////////////////////////////////////////////////////////
Rem * General Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_FOREVER& = -1



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposCash.h
Rem *
Rem *   Cash Drawer header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem * No definitions required for this version.



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposChan.h
Rem *
Rem *   Cash Changer header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "DeviceStatus" and "FullStatus" Property Constants
Rem * "StatusUpdateEvent" Event Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const CHAN_STATUS_OK& = 0                       ' DeviceStatus, FullStatus

Public Const CHAN_STATUS_EMPTY& = 11                   ' DeviceStatus, StatusUpdateEvent
Public Const CHAN_STATUS_NEAREMPTY& = 12               ' DeviceStatus, StatusUpdateEvent
Public Const CHAN_STATUS_EMPTYOK& = 13                 ' StatusUpdateEvent

Public Const CHAN_STATUS_FULL& = 21                    ' FullStatus, StatusUpdateEvent
Public Const CHAN_STATUS_NEARFULL& = 22                ' FullStatus, StatusUpdateEvent
Public Const CHAN_STATUS_FULLOK& = 23                  ' StatusUpdateEvent

Public Const CHAN_STATUS_JAM& = 31                     ' DeviceStatus, StatusUpdateEvent
Public Const CHAN_STATUS_JAMOK& = 32                   ' StatusUpdateEvent

Public Const CHAN_STATUS_ASYNC& = 91                   ' StatusUpdateEvent


Rem *///////////////////////////////////////////////////////////////////
Rem * "ResultCodeExtended" Property Constants for Cash Changer
Rem *///////////////////////////////////////////////////////////////////


Public Const OPOS_ECHAN_OVERDISPENSE& = 1 + OPOSERREXT



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposCoin.h
Rem *
Rem *   Coin Dispenser header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "DispenserStatus" Property Constants
Rem * "StatusUpdateEvent" Event: "Data" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const COIN_STATUS_OK& = 1
Public Const COIN_STATUS_EMPTY& = 2
Public Const COIN_STATUS_NEAREMPTY& = 3
Public Const COIN_STATUS_JAM& = 4



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposDisp.h
Rem *
Rem *   Line Display header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem * 96-03-18 OPOS Release 1.01                                    CRM
Rem *   Add DISP_MT_INIT constant and MarqueeFormat constants.
Rem * 96-04-22 OPOS Release 1.1                                     CRM
Rem *   Add CapCharacterSet values for Kana and Kanji.
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "CapBlink" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_CB_NOBLINK& = 0
Public Const DISP_CB_BLINKALL& = 1
Public Const DISP_CB_BLINKEACH& = 2


Rem *///////////////////////////////////////////////////////////////////
Rem * "CapCharacterSet" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_CCS_NUMERIC& = 0
Public Const DISP_CCS_ALPHA& = 1
Public Const DISP_CCS_ASCII& = 998
Public Const DISP_CCS_KANA& = 10
Public Const DISP_CCS_KANJI& = 11


Rem *///////////////////////////////////////////////////////////////////
Rem * "CharacterSet" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_CS_ASCII& = 998
Public Const DISP_CS_WINDOWS& = 999


Rem *///////////////////////////////////////////////////////////////////
Rem * "MarqueeType" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_MT_NONE& = 0
Public Const DISP_MT_UP& = 1
Public Const DISP_MT_DOWN& = 2
Public Const DISP_MT_LEFT& = 3
Public Const DISP_MT_RIGHT& = 4
Public Const DISP_MT_INIT& = 5


Rem *///////////////////////////////////////////////////////////////////
Rem * "MarqueeFormat" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_MF_WALK& = 0
Public Const DISP_MF_PLACE& = 1


Rem *///////////////////////////////////////////////////////////////////
Rem * "DisplayText" Method: "Attribute" Property Constants
Rem * "DisplayTextAt" Method: "Attribute" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_DT_NORMAL& = 0
Public Const DISP_DT_BLINK& = 1


Rem *///////////////////////////////////////////////////////////////////
Rem * "ScrollText" Method: "Direction" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_ST_UP& = 1
Public Const DISP_ST_DOWN& = 2
Public Const DISP_ST_LEFT& = 3
Public Const DISP_ST_RIGHT& = 4


Rem *///////////////////////////////////////////////////////////////////
Rem * "SetDescriptor" Method: "Attribute" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const DISP_SD_OFF& = 0
Public Const DISP_SD_ON& = 1
Public Const DISP_SD_BLINK& = 2



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposKbd.h
Rem *
Rem *   POS Keyboard header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 96-04-22 OPOS Release 1.1                                     CRM
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *   Add "EventTypes" and "POSKeyEventType" values.
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "EventTypes" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const KBD_ET_DOWN& = 1
Public Const KBD_ET_DOWN_UP& = 2


Rem *///////////////////////////////////////////////////////////////////
Rem * "POSKeyEventType" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const KBD_KET_KEYDOWN& = 1
Public Const KBD_KET_KEYUP& = 2



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposLock.h
Rem *
Rem *   Keylock header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "KeyPosition" Property Constants
Rem * "WaitForKeylockChange" Method: "KeyPosition" Parameter
Rem * "StatusUpdateEvent" Event: "Data" Parameter
Rem *///////////////////////////////////////////////////////////////////

Public Const LOCK_KP_ANY& = 0                          ' WaitForKeylockChange Only
Public Const LOCK_KP_LOCK& = 1
Public Const LOCK_KP_NORM& = 2
Public Const LOCK_KP_SUPR& = 3



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposMicr.h
Rem *
Rem *   MICR header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "CheckType" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const MICR_CT_PERSONAL& = 1
Public Const MICR_CT_BUSINESS& = 2
Public Const MICR_CT_UNKNOWN& = 99


Rem *///////////////////////////////////////////////////////////////////
Rem * "CountryCode" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const MICR_CC_USA& = 1
Public Const MICR_CC_CANADA& = 2
Public Const MICR_CC_MEXICO& = 3
Public Const MICR_CC_UNKNOWN& = 99


Rem *///////////////////////////////////////////////////////////////////
Rem * "ResultCodeExtended" Property Constants for MICR
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_EMICR_NOCHECK& = 1 + OPOSERREXT       ' EndInsertion
Public Const OPOS_EMICR_CHECK& = 2 + OPOSERREXT         ' EndRemoval



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposMsr.h
Rem *
Rem *   Magnetic Stripe Reader header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *   Add ErrorReportingType values.
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "TracksToRead" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const MSR_TR_1& = 1
Public Const MSR_TR_2& = 2
Public Const MSR_TR_3& = 4

Public Const MSR_TR_1_2& = MSR_TR_1 Or MSR_TR_2
Public Const MSR_TR_1_3& = MSR_TR_1 Or MSR_TR_3
Public Const MSR_TR_2_3& = MSR_TR_2 Or MSR_TR_3

Public Const MSR_TR_1_2_3& = MSR_TR_1 Or MSR_TR_2 Or MSR_TR_3


Rem *///////////////////////////////////////////////////////////////////
Rem * "ErrorReportingType" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const MSR_ERT_CARD& = 0
Public Const MSR_ERT_TRACK& = 1


Rem *///////////////////////////////////////////////////////////////////
Rem * "ErrorEvent" Event: "ResultCodeExtended" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_EMSR_START& = 1 + OPOSERREXT
Public Const OPOS_EMSR_END& = 2 + OPOSERREXT
Public Const OPOS_EMSR_PARITY& = 3 + OPOSERREXT
Public Const OPOS_EMSR_LRC& = 4 + OPOSERREXT

Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposPpad.h
Rem *
Rem *   PIN Pad header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 99-10-11 OPOS Release 1.3                                     SMM
Rem *
Rem *
Rem *
Rem *///////////////////////////////////////////////////////////////////

Rem */////////////////////////////////////////////////////////////////////
Rem * "CapDisplay" Property Constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_DISP_UNRESTRICTED& = 1
Public Const PPAD_DISP_PINRESTRICTED& = 2
Public Const PPAD_DISP_RESTRICTED_LIST& = 3
Public Const PPAD_DISP_RESTRICTED_ORDER& = 4


Rem */////////////////////////////////////////////////////////////////////
Rem * "AvailablePromptsList" and "Prompt" Property constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_MSG_ENTERPIN& = 1
Public Const PPAD_MSG_PLEASEWAIT& = 2
Public Const PPAD_MSG_ENTERVALIDPIN& = 3
Public Const PPAD_MSG_RETRIESEXCEEDED& = 4
Public Const PPAD_MSG_APPROVED& = 5
Public Const PPAD_MSG_DECLINED& = 6
Public Const PPAD_MSG_CANCELED& = 7
Public Const PPAD_MSG_AMOUNTOK& = 8
Public Const PPAD_MSG_NOTREADY& = 9
Public Const PPAD_MSG_IDLE& = 10
Public Const PPAD_MSG_SLIDE_CARD& = 11
Public Const PPAD_MSG_INSERTCARD& = 12
Public Const PPAD_MSG_SELECTCARDTYPE& = 13


Rem */////////////////////////////////////////////////////////////////////
Rem * "CapLanguage" Property constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_LANG_NONE& = 1
Public Const PPAD_LANG_ONE& = 2
Public Const PPAD_LANG_PINRESTRICTED& = 3
Public Const PPAD_LANG_UNRESTRICTED& = 4

Rem */////////////////////////////////////////////////////////////////////
Rem * "TransactionType" Property Public constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_TRANS_DEBIT& = 1
Public Const PPAD_TRANS_CREDIT& = 2
Public Const PPAD_TRANS_INQ& = 3
Public Const PPAD_TRANS_RECONCILE& = 4
Public Const PPAD_TRANS_ADMIN& = 5


Rem */////////////////////////////////////////////////////////////////////
Rem * "EndEFTTransaction" Method Completion Code Public constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_EFT_NORMAL& = 1
Public Const PPAD_EFT_ABNORMAL& = 2


Rem */////////////////////////////////////////////////////////////////////
Rem * "DataEvent" Event Status Public constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_SUCCESS& = 1
Public Const PPAD_CANCEL& = 2
Public Const PPAD_TIMEOUT& = 3

Rem */////////////////////////////////////////////////////////////////////
Rem * "ErrorEvent" Event: "ResultCodeExtended" Parameter Constants
Rem */////////////////////////////////////////////////////////////////////

Public Const PPAD_BAD_KEY& = 1 + OPOSERREXT


Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposPtr.h
Rem *
Rem *   POS Printer header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem * 96-04-22 OPOS Release 1.1                                     CRM
Rem *   Add CapCharacterSet values.
Rem *   Add ErrorLevel values.
Rem *   Add TransactionPrint Control values.
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *   Remove PTR_RP_NORMAL_ASYNC.
Rem *   Add more barcode symbologies.
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * Printer Station Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_S_JOURNAL& = 1
Public Const PTR_S_RECEIPT& = 2
Public Const PTR_S_SLIP& = 4

Public Const PTR_S_JOURNAL_RECEIPT& = PTR_S_JOURNAL Or PTR_S_RECEIPT
Public Const PTR_S_JOURNAL_SLIP& = PTR_S_JOURNAL Or PTR_S_SLIP
Public Const PTR_S_RECEIPT_SLIP& = PTR_S_RECEIPT Or PTR_S_SLIP


Rem *///////////////////////////////////////////////////////////////////
Rem * "CapCharacterSet" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_CCS_ALPHA& = 1
Public Const PTR_CCS_ASCII& = 998
Public Const PTR_CCS_KANA& = 10
Public Const PTR_CCS_KANJI& = 11


Rem *///////////////////////////////////////////////////////////////////
Rem * "CharacterSet" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_CS_ASCII& = 998
Public Const PTR_CS_WINDOWS& = 999


Rem *///////////////////////////////////////////////////////////////////
Rem * "ErrorLevel" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_EL_NONE& = 1
Public Const PTR_EL_RECOVERABLE& = 2
Public Const PTR_EL_FATAL& = 3


Rem *///////////////////////////////////////////////////////////////////
Rem * "MapMode" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_MM_DOTS& = 1
Public Const PTR_MM_TWIPS& = 2
Public Const PTR_MM_ENGLISH& = 3
Public Const PTR_MM_METRIC& = 4


Rem *///////////////////////////////////////////////////////////////////
Rem * "CutPaper" Method Constant
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_CP_FULLCUT& = 100


Rem *///////////////////////////////////////////////////////////////////
Rem * "PrintBarCode" Method Constants:
Rem *///////////////////////////////////////////////////////////////////

Rem *   "Alignment" Parameter
Rem *     Either the distance from the left-most print column to the start
Rem *     of the bar code, or one of the following:

Public Const PTR_BC_LEFT& = -1
Public Const PTR_BC_CENTER& = -2
Public Const PTR_BC_RIGHT& = -3

Rem *   "TextPosition" Parameter

Public Const PTR_BC_TEXT_NONE& = -11
Public Const PTR_BC_TEXT_ABOVE& = -12
Public Const PTR_BC_TEXT_BELOW& = -13

Rem *   "Symbology" Parameter:

Rem *     One dimensional symbologies
Public Const PTR_BCS_UPCA& = 101                       ' Digits
Public Const PTR_BCS_UPCE& = 102                       ' Digits
Public Const PTR_BCS_JAN8& = 103                       ' = EAN 8
Public Const PTR_BCS_EAN8& = 103                       ' = JAN 8 (added in 1.2)
Public Const PTR_BCS_JAN13& = 104                      ' = EAN 13
Public Const PTR_BCS_EAN13& = 104                      ' = JAN 13 (added in 1.2)
Public Const PTR_BCS_TF& = 105                         ' (Discrete 2 of 5) Digits
Public Const PTR_BCS_ITF& = 106                        ' (Interleaved 2 of 5) Digits
Public Const PTR_BCS_Codabar& = 107                    ' Digits, -, $, :, /, ., +;
                                                 '   4 start/stop characters
                                                 '   (a, b, c, d)
Public Const PTR_BCS_Code39& = 108                     ' Alpha, Digits, Space, -, .,
                                                 '   $, /, +, %; start/stop (*)
                                                 ' Also has Full ASCII feature
Public Const PTR_BCS_Code93& = 109                     ' Same characters as Code 39
Public Const PTR_BCS_Code128& = 110                    ' 128 data characters
Rem *        (The following were added in Release 1.2)
Public Const PTR_BCS_UPCA_S& = 111                     ' UPC-A with supplemental
                                                 '   barcode
Public Const PTR_BCS_UPCE_S& = 112                     ' UPC-E with supplemental
                                                 '   barcode
Public Const PTR_BCS_UPCD1& = 113                      ' UPC-D1
Public Const PTR_BCS_UPCD2& = 114                      ' UPC-D2
Public Const PTR_BCS_UPCD3& = 115                      ' UPC-D3
Public Const PTR_BCS_UPCD4& = 116                      ' UPC-D4
Public Const PTR_BCS_UPCD5& = 117                      ' UPC-D5
Public Const PTR_BCS_EAN8_S& = 118                     ' EAN 8 with supplemental
                                                 '   barcode
Public Const PTR_BCS_EAN13_S& = 119                    ' EAN 13 with supplemental
                                                 '   barcode
Public Const PTR_BCS_EAN128& = 120                     ' EAN 128
Public Const PTR_BCS_OCRA& = 121                       ' OCR "A"
Public Const PTR_BCS_OCRB& = 122                       ' OCR "B"


Rem *     Two dimensional symbologies
Public Const PTR_BCS_PDF417& = 201
Public Const PTR_BCS_MAXICODE& = 202

Rem *     Start of Printer-Specific bar code symbologies
Public Const PTR_BCS_OTHER& = 501


Rem *///////////////////////////////////////////////////////////////////
Rem * "PrintBitmap" Method Constants:
Rem *///////////////////////////////////////////////////////////////////

Rem *   "Width" Parameter
Rem *     Either bitmap width or:

Public Const PTR_BM_ASIS& = -11                        ' One pixel per printer dot

Rem *   "Alignment" Parameter
Rem *     Either the distance from the left-most print column to the start
Rem *     of the bitmap, or one of the following:

Public Const PTR_BM_LEFT& = -1
Public Const PTR_BM_CENTER& = -2
Public Const PTR_BM_RIGHT& = -3


Rem *///////////////////////////////////////////////////////////////////
Rem * "RotatePrint" Method: "Rotation" Parameter Constants
Rem * "RotateSpecial" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_RP_NORMAL& = &H1
Public Const PTR_RP_RIGHT90& = &H101
Public Const PTR_RP_LEFT90& = &H102
Public Const PTR_RP_ROTATE180& = &H103


Rem *///////////////////////////////////////////////////////////////////
Rem * "SetLogo" Method: "Location" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_L_TOP& = 1
Public Const PTR_L_BOTTOM& = 2


Rem *///////////////////////////////////////////////////////////////////
Rem * "TransactionPrint" Method: "Control" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_TP_TRANSACTION& = 11
Public Const PTR_TP_NORMAL& = 12


Rem *///////////////////////////////////////////////////////////////////
Rem * "StatusUpdateEvent" Event: "Data" Parameter Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const PTR_SUE_COVER_OPEN& = 11
Public Const PTR_SUE_COVER_OK& = 12

Public Const PTR_SUE_JRN_EMPTY& = 21
Public Const PTR_SUE_JRN_NEAREMPTY& = 22
Public Const PTR_SUE_JRN_PAPEROK& = 23

Public Const PTR_SUE_REC_EMPTY& = 24
Public Const PTR_SUE_REC_NEAREMPTY& = 25
Public Const PTR_SUE_REC_PAPEROK& = 26

Public Const PTR_SUE_SLP_EMPTY& = 27
Public Const PTR_SUE_SLP_NEAREMPTY& = 28
Public Const PTR_SUE_SLP_PAPEROK& = 29

Public Const PTR_SUE_IDLE& = 1001


Rem *///////////////////////////////////////////////////////////////////
Rem * "ResultCodeExtended" Property Constants for Printer
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_EPTR_COVER_OPEN& = 1 + OPOSERREXT     ' (Several)
Public Const OPOS_EPTR_JRN_EMPTY& = 2 + OPOSERREXT      ' (Several)
Public Const OPOS_EPTR_REC_EMPTY& = 3 + OPOSERREXT      ' (Several)
Public Const OPOS_EPTR_SLP_EMPTY& = 4 + OPOSERREXT      ' (Several)
Public Const OPOS_EPTR_SLP_FORM& = 5 + OPOSERREXT       ' EndRemoval
Public Const OPOS_EPTR_TOOBIG& = 6 + OPOSERREXT         ' PrintBitmap
Public Const OPOS_EPTR_BADFORMAT& = 7 + OPOSERREXT      ' PrintBitmap



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposScal.h
Rem *
Rem *   Scale header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "WeightUnit" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Public Const SCAL_WU_GRAM& = 1
Public Const SCAL_WU_KILOGRAM& = 2
Public Const SCAL_WU_OUNCE& = 3
Public Const SCAL_WU_POUND& = 4


Rem *///////////////////////////////////////////////////////////////////
Rem * "ResultCodeExtended" Property Constants for Scale
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_ESCAL_OVERWEIGHT& = 1 + OPOSERREXT    ' ReadWeight



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposScan.h
Rem *
Rem *   Scanner header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *   Add "ScanDataType" values.
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "ScanDataType" Property Constants
Rem *///////////////////////////////////////////////////////////////////

Rem * One dimensional symbologies
Public Const SCAN_SDT_UPCA& = 101                      ' Digits
Public Const SCAN_SDT_UPCE& = 102                      ' Digits
Public Const SCAN_SDT_JAN8& = 103                      ' = EAN 8
Public Const SCAN_SDT_EAN8& = 103                      ' = JAN 8 (added in 1.2)
Public Const SCAN_SDT_JAN13& = 104                     ' = EAN 13
Public Const SCAN_SDT_EAN13& = 104                     ' = JAN 13 (added in 1.2)
Public Const SCAN_SDT_TF& = 105                        ' (Discrete 2 of 5) Digits
Public Const SCAN_SDT_ITF& = 106                       ' (Interleaved 2 of 5) Digits
Public Const SCAN_SDT_Codabar& = 107                   ' Digits, -, $, :, /, ., +;
                                                 '   4 start/stop characters
                                                 '   (a, b, c, d)
Public Const SCAN_SDT_Code39& = 108                    ' Alpha, Digits, Space, -, .,
                                                 '   $, /, +, %; start/stop (*)
                                                 ' Also has Full ASCII feature
Public Const SCAN_SDT_Code93& = 109                    ' Same characters as Code 39
Public Const SCAN_SDT_Code128& = 110                   ' 128 data characters

Public Const SCAN_SDT_UPCA_S& = 111                    ' UPC-A with supplemental
                                                 '   barcode
Public Const SCAN_SDT_UPCE_S& = 112                    ' UPC-E with supplemental
                                                 '   barcode
Public Const SCAN_SDT_UPCD1& = 113                     ' UPC-D1
Public Const SCAN_SDT_UPCD2& = 114                     ' UPC-D2
Public Const SCAN_SDT_UPCD3& = 115                     ' UPC-D3
Public Const SCAN_SDT_UPCD4& = 116                     ' UPC-D4
Public Const SCAN_SDT_UPCD5& = 117                     ' UPC-D5
Public Const SCAN_SDT_EAN8_S& = 118                    ' EAN 8 with supplemental
                                                 '   barcode
Public Const SCAN_SDT_EAN13_S& = 119                   ' EAN 13 with supplemental
                                                 '   barcode
Public Const SCAN_SDT_EAN128& = 120                    ' EAN 128
Public Const SCAN_SDT_OCRA& = 121                      ' OCR "A"
Public Const SCAN_SDT_OCRB& = 122                      ' OCR "B"

Rem * Two dimensional symbologies
Public Const SCAN_SDT_PDF417& = 201
Public Const SCAN_SDT_MAXICODE& = 202

Rem * Special cases
Public Const SCAN_SDT_OTHER& = 501                     ' Start of Scanner-Specific bar
                                                 '   code symbologies
Public Const SCAN_SDT_UNKNOWN& = 0                     ' Cannot determine the barcode
                                                 '   symbology.



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposSig.h
Rem *
Rem *   Signature Capture header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem * No definitions required for this version.



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposTone.h
Rem *
Rem *   Tone Indicator header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 97-06-04 OPOS Release 1.2                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem * No definitions required for this version.



Rem *///////////////////////////////////////////////////////////////////
Rem *
Rem * From OposTot.h
Rem *
Rem *   Hard Totals header file for OPOS Applications.
Rem *
Rem * Modification history
Rem * ------------------------------------------------------------------
Rem * 95-12-08 OPOS Release 1.0                                     CRM
Rem *
Rem *///////////////////////////////////////////////////////////////////


Rem *///////////////////////////////////////////////////////////////////
Rem * "ResultCodeExtended" Property Constants for Hard Totals
Rem *///////////////////////////////////////////////////////////////////

Public Const OPOS_ETOT_NOROOM& = 1 + OPOSERREXT         ' Create, Write
Public Const OPOS_ETOT_VALIDATION& = 2 + OPOSERREXT     ' Read, Write



Rem *End of OPOSALL.BAS*

