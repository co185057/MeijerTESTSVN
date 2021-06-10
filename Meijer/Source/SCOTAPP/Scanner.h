
/********************************************************************/
/*  <scanner.h> header file                                         */
/*                                                                  */
/********************************************************************/

#ifndef _H_POS_SCANNER
#define _H_POS_SCANNER 1

/****************************************************************/
/* Values for the SUBTYPE field of ONLINE and OFFLINE messages. */
/****************************************************************/

/* Portable (Mobile) Scanners */
#define PosSCAN_SUBTYPE_4612            1  /* 4612 SurePoint Mobile*/

/* Hand-held Bar Code Readers */
#define PosSCAN_SUBTYPE_HHBCR_1         1           /* Model 1  */
#define PosSCAN_SUBTYPE_HHBCR_2         2           /* Model 2  */
#define PosSCAN_SUBTYPE_1520_A02        3           /* 1520-A02 */

/* Flatbed Scanners */
#define PosSCAN_SUBTYPE_4687            1     /* 4687 Models 1 & 2 */
#define PosSCAN_SUBTYPE_4686_1_2        2     /* 4686 Models 1 & 2 */
#define PosSCAN_SUBTYPE_4686_3_4        3     /* 4686 Models 3 & 4 */
#define PosSCAN_SUBTYPE_4696            4     /* 4696             */
#define PosSCAN_SUBTYPE_4697            5     /* 4697             */
#define PosSCAN_SUBTYPE_4698            6     /* 4698             */

/*****************************/
/* PosScanner resource names */
/*****************************/

#define PosNbarCodes1                   _T("barCodes1")
#define PosNbarCodes2                   _T("barCodes2")
#define PosNbarCodes3                   _T("barCodes3")
#define PosNbarCodes4                   _T("barCodes4")
#define PosNbeepFreq                    _T("beepFreq")
#define PosNbeepLength                  _T("beepLength")
#define PosNbeepState                   _T("beepState")
#define PosNbeepVolume                  _T("beepVolume")
#define PosNblinkLength                 _T("blinkLength")
#define PosNblock1Type                  _T("block1Type")
#define PosNblock2Type                  _T("block2Type")
#define PosNblock3Type                  _T("block3Type")
#define PosNblockReadMode               _T("blockReadMode")
#define PosNbVolSwitchState             _T("bVolSwitchState")
#define PosNcheckModulo                 _T("checkModulo")
#define PosNcode128ScansPerRead         _T("code128ScansPerRead")
#define PosNcode39ScansPerRead          _T("code39ScansPerRead")
#define PosNdReadTimeout                _T("dReadTimeout")
#define PosNdecodeAlgorithm             _T("decodeAlgorithm")
#define PosNdTouchMode                  _T("dTouchMode")
#define PosNiTFLength1                  _T("iTFLength1")
#define PosNiTFLength2                  _T("iTFLength2")
#define PosNiTFScansPerRead             _T("iTFScansPerRead")
#define PosNjANTwoLabelDecode           _T("jANTwoLabelDecode")
#define PosNlabelsQueued                _T("labelsQueued")
#define PosNlaserTimeout                _T("laserTimeout")
#define PosNmotorTimeout                _T("motorTimeout")
#define PosNlaserSwitchState            _T("laserSwitchState")
#define PosNqueueAllLabels              _T("queueAllLabels")
#define PosNscansPerRead                _T("scansPerRead")
#define PosNeAN13ScansPerRead           _T("eAN13ScansPerRead")
#define PosNeAN8ScansPerRead            _T("eAN8ScansPerRead")
#define PosNuPCAScansPerRead            _T("uPCAScansPerRead")
#define PosNuPCDScansPerRead            _T("uPCDScansPerRead")
#define PosNuPCEScansPerRead            _T("uPCEScansPerRead")
#define PosNstoreScansPerRead           _T("storeScansPerRead")
#define PosNtwoLabelFlagPair1           _T("twoLabelFlagPair1")
#define PosNtwoLabelFlagPair2           _T("twoLabelFlagPair2")
#define PosNtwoLabelFlagPair3           _T("twoLabelFlagPair3")
#define PosNtwoLabelFlagPair4           _T("twoLabelFlagPair4")
#define PosNuPCExpansion                _T("uPCExpansion")
#define PosNverifyPriceChk              _T("verifyPriceChk")

/****************************/
/* Scanner resource classes */
/****************************/

#define PosCBarCodes1                   _T("BarCodes1")
#define PosCBarCodes2                   _T("BarCodes2")
#define PosCBarCodes3                   _T("BarCodes3")
#define PosCBarCodes4                   _T("BarCodes4")
#define PosCBeepFreq                    _T("BeepFreq")
#define PosCBeepLength                  _T("BeepLength")
#define PosCBeepState                   _T("BeepState")
#define PosCBeepVolume                  _T("BeepVolume")
#define PosCBlinkLength                 _T("BlinkLength")
#define PosCBlock1Type                  _T("Block1Type")
#define PosCBlock2Type                  _T("Block2Type")
#define PosCBlock3Type                  _T("Block3Type")
#define PosCBlockReadMode               _T("BlockReadMode")
#define PosCBVolSwitchState             _T("BVolSwitchState")
#define PosCCheckModulo                 _T("CheckModulo")
#define PosCCode128ScansPerRead         _T("Code128ScansPerRead")
#define PosCCode39ScansPerRead          _T("Code39ScansPerRead")
#define PosCDecodeAlgorithm             _T("DecodeAlgorithm")
#define PosCDReadTimeout                _T("DReadTimeout")
#define PosCDTouchMode                  _T("DTouchMode")
#define PosCITFLength1                  _T("ITFLength1")
#define PosCITFLength2                  _T("ITFLength2")
#define PosCITFScansPerRead             _T("ITFScansPerRead")
#define PosCJANTwoLabelDecode           _T("JANTwoLabelDecode")
#define PosCLabelsQueued                _T("LabelsQueued")
#define PosCLaserTimeout                _T("LaserTimeout")
#define PosCMotorTimeout                _T("MotorTimeout")
#define PosCLaserSwitchState            _T("LaserSwitchState")
#define PosCQueueAllLabels              _T("QueueAllLabels")
#define PosCScansPerRead                _T("ScansPerRead")
#define PosCEAN13ScansPerRead           _T("EAN13ScansPerRead")
#define PosCEAN8ScansPerRead            _T("EAN8ScansPerRead")
#define PosCUPCAScansPerRead            _T("UPCAScansPerRead")
#define PosCUPCDScansPerRead            _T("UPCDScansPerRead")
#define PosCUPCEScansPerRead            _T("UPCEScansPerRead")
#define PosCStoreScansPerRead           _T("StoreScansPerRead")
#define PosCTwoLabelFlagPair1           _T("TwoLabelFlagPair1")
#define PosCTwoLabelFlagPair2           _T("TwoLabelFlagPair2")
#define PosCTwoLabelFlagPair3           _T("TwoLabelFlagPair3")
#define PosCTwoLabelFlagPair4           _T("TwoLabelFlagPair4")
#define PosCUPCExpansion                _T("UPCExpansion")
#define PosCVerifyPriceChk              _T("VerifyPriceChk")

/*********************************************************************/
/*                                                                  */
/* This block comment describes how the constants for the bar code  */
/* types and groups are built Each distinct bar code type is      */
/* assigned a bit position (or two) within a 32 bit word.  When a   */
/* bar code of that type is read, the appropriate field in the data  */
/* buffer is filled with a value that has that bit set.  When a     */
/* scanner is being configured to read a group of bar codes, the    */
/* bit(s) corresponding to the bar code(s) read in that mode are    */
/* set.  There is also an "odd mode" bit that is used when the mode  */
/* or bar code does not fit the normal set of rules               */
/*                                                                  */
/* In the figure, the 1's indicate a set bit while the periods      */
/* indicate a clear bit The bit positions were chosen like this   */
/* so it would be a little easier to remember the bit positions     */
/* while debugging.                                                 */
/*                                                                  */
/*********************************************************************/
/*                                                                  */
/*                          Bit 31                            Bit 0  */
/*        Label Type        |             Bit(s) set              |  */
/*  ---------------------   ---------------------------------------  */
/*  (Unknown)               .... .... .... .... .... .... .... ....  */
/*                                                                  */
/*  Codabar                 .... .... .... 1... .... .... .... ....  */
/*  Code 39                 .... .... .... .1.. .... .... .... ....  */
/*  Code 93                 .... .... .... ..1. .... .... .... ....  */
/*  Code 128                .... .... .... ...1 .... .... .... ....  */
/*                                                                  */
/*  UPC-A                   .... .... .... .... 1... .... .... ....  */
/*  UPC-E                   .... .... .... .... .1.. .... .... ....  */
/*  EAN-8                   .... .... .... .... ..1. .... .... ....  */
/*  EAN-13                  .... .... .... .... ...1 .... .... ....  */
/*                                                                  */
/*  UPC-D1                  .... .... .... .... .... 1... .... ....  */
/*  UPC-D2                  .... .... .... .... .... .1.. .... ....  */
/*  UPC-D3                  .... .... .... .... .... ..1. .... ....  */
/*  UPC-D4                  .... .... .... .... .... ...1 .... ....  */
/*  UPC-D5                  .... .... .... .... .... .... 1... ....  */
/*                                                                  */
/*  Interleaved 2 of 5      .... .... .... .... .... .... .... 1...  */
/*  Standard 2 of 5         .... .... .... .... .... .... .... .1..  */
/*                                                                  */
/*  Plus 2 (flag)           .... .... .... .... .... .... .... ..1.  */
/*  Plus 5 (flag)           .... .... .... .... .... .... .... ...1  */
/*                                                                  */
/*  "Odd" modes             1... .... .... .... .... .... .... ....  */
/*                                                                  */
/*  (Reserved)              .111 1111 1111 .... .... .... .111 ....  */
/*                                                                  */
/*  Labels with multiple bits set:                                  */
/*                                                                  */
/*  UPC-A+2                 .... .... .... .... 1... .... .... ..1.  */
/*  UPC-A+5                 .... .... .... .... 1... .... .... ...1  */
/*  UPC-E+2                 .... .... .... .... .1.. .... .... ..1.  */
/*  UPC-E+5                 .... .... .... .... .1.. .... .... ...1  */
/*  EAN-8+2                 .... .... .... .... ..1. .... .... ..1.  */
/*  EAN-8+5                 .... .... .... .... ..1. .... .... ...1  */
/*  EAN-13+2                .... .... .... .... ...1 .... .... ..1.  */
/*  EAN-13+5                .... .... .... .... ...1 .... .... ...1  */
/*                                                                  */
/*********************************************************************/

/***************************************************/
/* PosNbarCodes1 and PosNbarCodes2 resource values */
/***************************************************/

#define PosLGROUP_CODABAR               0x00080000L
#define PosLGROUP_CODE_39               0x00040000L
#define PosLGROUP_CODE_93               0x00020000L
#define PosLGROUP_CODE_128              0x00010000L
#define PosLGROUP_INT_2_OF_5            0x00000008L
#define PosLGROUP_NONE                  0x00000000L
#define PosLGROUP_PLUS_2_5_ONLY         0x8000F003L
#define PosLGROUP_STD_2_OF_5            0x00000004L
#define PosLGROUP_UPC_D1_TO_D5          0x00000F80L
#define PosLGROUP_UPC_EAN               0x0000F000L
#define PosLGROUP_UPC_EAN_CODABAR       0x0008F000L
#define PosLGROUP_UPC_EAN_CODE_39       0x0004F000L
#define PosLGROUP_UPC_EAN_CODE_93       0x0002F000L
#define PosLGROUP_UPC_EAN_CODE_128      0x0001F000L
#define PosLGROUP_UPC_EAN_D1_TO_D5      0x0000FF80L
#define PosLGROUP_UPC_EAN_D3            0x0000F200L
#define PosLGROUP_UPC_EAN_ITF           0x0000F004L
#define PosLGROUP_UPC_EAN_PLUS_2        0x0000F002L
#define PosLGROUP_UPC_EAN_PLUS_5        0x0000F001L
#define PosLGROUP_UPC_EAN_PLUS_2_5      0x0000F003L
#define PosLGROUP_UPC_EAN_2_5_CODABAR   0x0008F003L

/****************************************************************/
/* PosNblock1Type, PosNblock2Type, and PosNblock3Type resource  */
/* values, as well as the constants used to indicate label type */
/****************************************************************/

#define PosLABEL_CODABAR                0x00080000L
#define PosLABEL_CODE_39                0x00040000L
#define PosLABEL_CODE_93                0x00020000L
#define PosLABEL_CODE_128               0x00010000L
#define PosLABEL_EAN_8                  0x00002000L
#define PosLABEL_EAN_8_PLUS_2           0x00002002L
#define PosLABEL_EAN_8_PLUS_5           0x00002001L
#define PosLABEL_EAN_13                 0x00001000L
#define PosLABEL_EAN_13_PLUS_2          0x00001002L
#define PosLABEL_EAN_13_PLUS_5          0x00001001L
#define PosLABEL_INT_2_OF_5             0x00000008L
#define PosLABEL_NO_CHECK               0x00000000L
#define PosLABEL_STD_2_OF_5             0x00000004L
#define PosLABEL_UNKNOWN                0x00000000L
#define PosLABEL_UPC_A                  0x00008000L
#define PosLABEL_UPC_A_PLUS_2           0x00008002L
#define PosLABEL_UPC_A_PLUS_5           0x00008001L
#define PosLABEL_UPC_D1                 0x00000800L
#define PosLABEL_UPC_D2                 0x00000400L
#define PosLABEL_UPC_D3                 0x00000200L
#define PosLABEL_UPC_D4                 0x00000100L
#define PosLABEL_UPC_D5                 0x00000080L
#define PosLABEL_UPC_E                  0x00004000L
#define PosLABEL_UPC_E_PLUS_2           0x00004002L
#define PosLABEL_UPC_E_PLUS_5           0x00004001L

#define PosLABEL_PLUS_2_FLAG            0x00000002L
#define PosLABEL_PLUS_5_FLAG            0x00000001L

/*****************************************************/
/* Resource values used by scanner devices for      */
/* PosNblinkLength, PosNbeepLength, PosNdReadTimeout */
/*****************************************************/

#define PosSHORT                        0x00
#define PosMEDLONG                      0x01
#define PosLONG                         0x02

/*****************************************************/
/* Resource values used by scanner devices for      */
/* PosNuPCExpansion                               */
/*****************************************************/

#define PosNO_EXPANSION                 0x00
#define PosUPCA_TO_EAN13                0x01
#define PosUPCE_TO_EAN13                0x02
#define PosUPCA_UPCE_TO_EAN13           0x03
#define PosUPCE_TO_UPCA                 0x04
#define PosUPCE_TO_UPCA_TO_EAN13        0x05

/*****************************************************/
/* Resource values used by scanner devices for      */
/* PosNverifyPriceChk                             */
/*****************************************************/

#define PosNO_VERIFY                    0x00
#define PosVERIFY_4DIGIT                0x01
#define PosVERIFY_5DIGIT                0x02

/***************************************************************/
/* Constants used in the Flags field of the scanner data block */
/***************************************************************/

#define POS_SCAN_LAST_BLOCK             0x0001
#define POS_SCAN_UNEXPECTED_DATA        0x0002
#define POS_SCAN_PARTIAL_LABEL          0x0004
#define POS_SCAN_LABEL_READ_ERROR       0x0008

/****************************************************************/
/* PosScannerDataHdr structure -- Scanner label data header     */
/****************************************************************/

typedef struct
{
  unsigned long  LblType;             /* Label Type   */
  unsigned short Flags;               /* Flags        */
  unsigned short LblLen;              /* Label Length */
} PosScannerDataHdr;

#endif  /* _H_POS_SCANNER */
