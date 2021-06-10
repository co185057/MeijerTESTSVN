/////////////////////////////////////////////////////////////////////////////
//
// ncrdevmgr.h
//
//   Device Manager header file for Applications.
//
//   Copyright (c) 1996 NCR.  All rights reserved.
//
// $Archive: /Fastlane/4.2/Development/Core/Source/Include/ncrdevmgr.h $
//
// $Revision: 9 $
//
// $Date: 3/31/08 2:23p $
//
//   Date                   Modification                          Author
// --------|---------------------------------------------------|-------------
// 96/11/15 Initial version.                                    C. Light
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NCRDEVMGR_H
#define _NCRDEVMGR_H

//NOTE FOR DM Developers ONLY:  Keep in mind that these Device Class values are used with the variables
//g_DeviceSupport and g_nControlIDs found in devmgrctl.cpp 
#define DMCLASS_START 0
enum{
	DMCLASS_RECEIPT     =       DMCLASS_START, 
	DMCLASS_JOURNAL				,
	DMCLASS_SLIP				,
	DMCLASS_PRINTER				, // Device Manager Use Only!
	DMCLASS_CASHDRAWER			,
	DMCLASS_CASHCHANGER			,
	DMCLASS_KEYLOCK				,
	DMCLASS_LINEDISPLAY			,
	DMCLASS_MICR				,
	DMCLASS_MSR					,
	DMCLASS_SCALE				,
	DMCLASS_SCANNER				,
	DMCLASS_SIGCAP				,
	DMCLASS_MISC				,
	DMCLASS_ENCRYPTOR			,
	DMCLASS_CASHACCEPTOR		,
	DMCLASS_COINACCEPTOR		,        //MHS-09/23/98
	DMCLASS_MAX_CLASSES		   	//IMPORTANT THIS MUST BE THE LAST Constant in list
};




#define DM_ALL_DEVICECLASSES       -1 
#define DM_ALL_DEVICEIDS           -1 

// InputType definitions
#define DM_DATA                    0 
#define DM_ERROR                   1 
#define DM_DIRECTIO                2

#if defined (_SCOT) || defined (_SCOTSSF_)
//Encryptor status returned via Input Data
#define DM_ENCRYPT_SECURE_MODE	 1000 
#define DM_ENCRYPT_COMPLETE		 1001 
#define DM_ENCRYPT_CANCELED		 1002 
#define DM_ENCRYPT_SHORT_PIN	 1003
#endif

// DM_ERROR ErrorCodes
#define DM_ERROR_GENERALFAILURE          -1 
#define DM_ERROR_SCALE_OVERWEIGHT        -2 
#define DM_ERROR_SCALE_UNDERZERO         -3 
#define DM_ERROR_SCALE_UNSTABLE          -4 
#define DM_ERROR_SCALE_WEIGHTUNCHANGED   -5 

#define DM_ERROR_UNEXPECTED              -10 

#if defined (_SCOT) || defined (_SCOTSSF_)
//motorized MSR  error events
#define DM_ERROR_MSR_CARD_JAM            -20 
#define DM_ERROR_MSR_INVALID_CARD        -21 
#define DM_ERROR_MSR_READING             -22 
#define DM_ERROR_MSR_DEVICE_INOPERABLE   -23 
#define DM_ERROR_MSR_TRACKS_UNSUPPORTED  -24
#define DM_ERROR_MSR_CARD_PRESENT		 -25
#define DM_ERROR_MSR_BLANK_TRACKS		 -27
#define DM_ERROR_MSR_SHUTTER_JAM		 -28
#define DM_ERROR_MSR_NO_CARD			 -29

//scanner

#define DM_ERROR_SCAN_DATALOSS           -30
#define DM_ERROR_SCAN_BADTAGSUFFIX       -31
#define DM_ERROR_SCAN_BADTAGPREFIX       -32
#define DM_ERROR_SCAN_BADTAG             -33
#define DM_ERROR_SCAN_SERIAL             -34


// +TAR 352251
#define DM_PTR_ERR_RPM_NOT_READY         -35
#define DM_PTR_ERR_CDS_DOOR_OPEN         -36
#define DM_PTR_ERR_RPM_MECH_ERROR        -37
#define DM_PTR_ERR_RPM_PAPER_JAM         -38
// -TAR 352251


#endif

// StatusType definitions
#define DM_DRAWER_OPEN              1 
#define DM_DRAWER_CLOSED            2 

#define DM_PTR_COVER_OPEN           11 
#define DM_PTR_COVER_CLOSED         12 
#define DM_PTR_EMPTY                13 
#define DM_PTR_NEAREMPTY            14 
#define DM_PTR_PAPEROK              15 
#define DM_PTR_ERR_COVER_OPEN       16 
#define DM_PTR_ERR_EMPTY            17 
#define DM_PTR_ERR_FAULT            18 
#define DM_PTR_REQUEST_COMPLETED    19 
#define DM_PTR_ERR_REQUEST_SEQUENCE 20 
#define DM_PTR_ERR_HEAD_JAM		    21
#define DM_PTR_ERR_KNIFE_JAM	    22
#define DM_PTR_ERR_PAPER_JAM		23
#define DM_PTR_ERR_PAPER_NOT_LOADED_JAM	24
#define DM_PTR_ERR_COMMS_ERROR	    25
#define DM_PTR_ERR_PRINTER_OPEN	    26
#define DM_PTR_ERR_REPLACE_HEAD	    227		//changed from 27-> 227, 28 -> 228, 29 -> 229 to avoid conflict with new constants defined in OposPtr.h (PTR_SUE_SLP_EMPTY, PTR_SUE_SLP_NEAREMPTY, PTR_SUE_SLP_PAPEROK)
#define DM_PTR_ERR_SIDEWAYS_COMMS_ERROR	228
#define DM_PTR_ERR_EJECT_JAM	    229
#define DM_PTR_ERR_BLACKMARK_ERROR  30
#define DM_PTR_ERR_TIME_OUT			31

#define DM_KEYLOCK_L                41 
#define DM_KEYLOCK_N                42 
#define DM_KEYLOCK_S                43 
#define DM_KEYLOCK_X                44 

#if defined (_SCOT) || defined (_SCOTSSF_)
#define DM_MSR_CAPBIN_FULL              50 
#define DM_MSR_CAPBIN_REMOVED           51 
#define DM_MSR_CAPBIN_INSERTED          52 
#define DM_MSR_CARD_REMOVED             53 
#define DM_MSR_CARD_INSERTED_STAGED     54 
#define DM_MSR_CARD_DETECTED_ENTERING   55 
#define DM_MSR_DEVICE_INOPERABLE        56 
#define DM_MSR_INVALID_CARD_INSERTED    57 
#define DM_MSR_SHUTTER_JAMMED_CLOSE     58 
#define DM_MSR_CARD_CAPTURED            59 
#define DM_CASHACCEPTOR_STATUS_OK           60 
#define DM_CASHACCEPTOR_STATUS_INSERTED     61 
#define DM_CASHACCEPTOR_STATUS_REMOVED      62 
#define DM_CASHACCEPTOR_STATUS_REJECTED     63 
#define DM_CASHACCEPTOR_STATUS_RESET        64 
#define DM_CASHACCEPTOR_STATUS_JAM          65 
#define DM_CASHACCEPTOR_STATUS_FAILURE      66 
#define DM_CASHACCEPTOR_STATUS_FULL         67 
#define DM_CASHACCEPTOR_STATUS_NEARFULL     68 
#define DM_CASHACCEPTOR_STATUS_TAMPER_IN    69 
#define DM_CASHACCEPTOR_STATUS_TAMPER_OUT   70 
#define DM_CASHACCEPTOR_STATUS_ASYNC        71 
#define DM_CASHACCEPTOR_STATUS_CHEAT        72 
#define DM_CASHACCEPTOR_SUCCESS             73 
#define DM_MSR_CARD_DETECTED                74
#define DM_MSR_DATA_AVAILABLE               75
#define DM_MSR_CARD_NOT_CAPTURED            76
#define DM_MSR_EJECT_CAPTURE                77
#define DM_MSR_POSSIBLE_CUST_TAMPERING      78
#define DM_SIGCAP_STARTED                   79

// Status constants for CoinAcceptor
#define DM_COINACCEPTOR_STATUS_OK           80
#define DM_COINACCEPTOR_STATUS_ASYNC        81
#define DM_COINACCEPTOR_STATUS_RETURNED     82
#define DM_COINACCEPTOR_STATUS_REJECTED     83
#define DM_COINACCEPTOR_STATUS_RESET        84
#define DM_COINACCEPTOR_STATUS_JAM          85
#define DM_COINACCEPTOR_STATUS_FAILURE      86
#define DM_COINACCEPTOR_STATUS_FULL         87
#define DM_COINACCEPTOR_STATUS_NEARFULL     88
#define DM_COINACCEPTOR_STATUS_TAMPER_IN    89
#define DM_COINACCEPTOR_STATUS_TAMPER_OUT   90
#define DM_COINACCEPTOR_STATUS_ESCROW_FULL  91
#define DM_COINACCEPTOR_STATUS_CLOSED       92
#define DM_COINACCEPTOR_STATUS_CHEAT        93
#define DM_COINACCEPTOR_SUCCESS             94
#define DM_COINACCEPTOR_STATUS_INOPERABLE   95

#define DM_SIGCAP_ENDED                     96
#define DM_SIGCAP_FAILURE					97

#endif

#define DM_STATUS_UNEXPECTED            99 

// DM Exceptions
#define    DM_E_INVALIDDEVICECLASS     CUSTOM_CTL_SCODE(1000)
#define    DM_E_INVALIDDEVICEID        CUSTOM_CTL_SCODE(1001)
#define    DM_E_DEVICENOTCONFIGURED    CUSTOM_CTL_SCODE(1002)
#define    DM_E_DEVICENOTAVAILABLE     CUSTOM_CTL_SCODE(1003)
#define    DM_E_DEVICEFAILURE          CUSTOM_CTL_SCODE(1004)
#define    DM_E_ILLEGAL                CUSTOM_CTL_SCODE(1005)
#define    DM_E_TIMEOUT                CUSTOM_CTL_SCODE(1006)
#define    DM_E_INVALIDPARAMETER       CUSTOM_CTL_SCODE(1007)
#define    DM_E_NOTSUPPORTED           CUSTOM_CTL_SCODE(1008)
#define    DM_E_BITMAPTOOBIG           CUSTOM_CTL_SCODE(1009)

#if defined (_SCOT) || defined (_SCOTSSF_)
#define    DM_E_NO_CARD                CUSTOM_CTL_SCODE(1010)
#define    DM_E_CARD_JAMMED_CAPTURE    CUSTOM_CTL_SCODE(1011)
#define	   DM_E_BUSY				   CUSTOM_CTL_SCODE(1012)
#endif

// #defs for CashChanger
#if defined (_SCOT) || defined (_SCOTSSF_)
#define DM_E_CC_PURGED								CUSTOM_CTL_SCODE(1013)
#define DM_E_CC_PARTIAL_DISP_COINS		CUSTOM_CTL_SCODE(1014)
#define DM_E_CC_PARTIAL_DISP_BILLS		CUSTOM_CTL_SCODE(1015)
#define DM_E_CC_PARTIAL_DISP_UNKNOWN	CUSTOM_CTL_SCODE(1016)
#define DM_E_JAMMED										CUSTOM_CTL_SCODE(1017)
#define DM_E_CC_SHORTAGE							CUSTOM_CTL_SCODE(1018)
#define DM_E_CC_TRANSPORT_JAM_COIN		CUSTOM_CTL_SCODE(1019)
#define DM_E_CC_TRANSPORT_JAM_CASH		CUSTOM_CTL_SCODE(1020)
#define DM_E_CC_HOPPER_JAM						CUSTOM_CTL_SCODE(1021)
#define DM_E_CC_NEAR_EMPTY						CUSTOM_CTL_SCODE(1022)
//DL-FB 07/07/99 Added for TAR#102274 for Event Loggingwhen the number of bills to be Dispensed exceeds 40
#define DM_E_CC_BILLMAX_40						CUSTOM_CTL_SCODE(1023)



// DM exceptions for Signature Capture
#define	   DM_E_NOFORM				   CUSTOM_CTL_SCODE(1200)
#define	   DM_E_NOSIGNATURE			   CUSTOM_CTL_SCODE(1201)
#define    DM_E_NOTINPROGRESS		   CUSTOM_CTL_SCODE(1202)
#define    DM_E_FILENOTOPEN			   CUSTOM_CTL_SCODE(1203)
#define    DM_E_INCOMPLETESIGNATURE	   CUSTOM_CTL_SCODE(1204)

#define DM_CC_STATUS_EMPTY						1
#define DM_CC_STATUS_NEAREMPTY				2
#define DM_CC_STATUS_EMPTYOK					3
#define DM_CC_STATUS_PURGEFULL				4
#define DM_CC_STATUS_PURGENEARFULL		5
#define DM_CC_STATUS_PURGEFULLOK			6
#define DM_CC_STATUS_JAM							7
#define DM_CC_STATUS_JAMOK						8
#define DM_CC_STATUS_INOPERABLE				9
#define DM_CC_STATUS_TAMPER						10
#define DM_CC_STATUS_BILLS_TAKEN			11
#define DM_CC_STATUS_TRANSPORT_JAM_CASH		12
#define DM_CC_STATUS_TRANSPORT_JAM_COIN		13
#define DM_CC_STATUS_HOPPER_JAM				14


//additional MSR 
#define    DM_E_CARD_JAMMED_EJECTED    CUSTOM_CTL_SCODE(1023)
#endif //_SCOT

// Constants for the ScaleWeightUnit property
#define DM_SCALE_WU_GRAM          0 
#define DM_SCALE_WU_KILOGRAM      1 
#define DM_SCALE_WU_OUNCE         2 
#define DM_SCALE_WU_POUND         3 

// Constants for the MSRTracksToRead property
#define DM_MSR_TRACK_1            1 
#define DM_MSR_TRACK_2            2 
#define DM_MSR_TRACK_3            4 
#define DM_MSR_TRACK_1_2          (DM_MSR_TRACK_1 | DM_MSR_TRACK_2 )
#define DM_MSR_TRACK_1_3          (DM_MSR_TRACK_1 | DM_MSR_TRACK_3 )
#define DM_MSR_TRACK_2_3          (DM_MSR_TRACK_2 | DM_MSR_TRACK_3 )
#define DM_MSR_TRACK_1_2_3        (DM_MSR_TRACK_1 | DM_MSR_TRACK_2 | DM_MSR_TRACK_3 )

//Constants for MSR Error Reporting
const int DM_MSR_ERT_CARD			= 0;
const int DM_MSR_ERT_TRACK			= 1;

// Constants for alignment
#define DM_ALIGN_LEFT             1 
#define DM_ALIGN_RIGHT            2 
#define DM_ALIGN_CENTER           3 

// Constants for WhenToCut parameter in PrinterCutPaper method
#define DM_WTC_NOW                1 
#define DM_WTC_AUTOADVANCE        2 

// Constants for BarcodeType (symbology)
#define DM_BCS_UPCA               1 
#define DM_BCS_UPCE               2 
#define DM_BCS_JAN8               3 
#define DM_BCS_JAN13              4 
#define DM_BCS_ITF                5 
#define DM_BCS_CODABAR            6 
#define DM_BCS_CODE39             7 
#define DM_BCS_CODE128			  10	//BZ 06/12/02 TAR 204658: Add support for Code 128 barcode printed on receipts

// Constants for Encryptor Store Key
#if defined (_SCOT) || defined (_SCOTSSF_)
#define DM_ENCRYPTOR_CLEAR_ALL_KEYS   -1 
#define DM_ENCRYPTOR_XOR_KEY       1 
#define DM_ENCRYPTOR_FROM_PINPAD   2  
#define DM_ENCRYPTOR_NO_EXCHANGE_KEY   -1 

#define DM_ENCRYPTOR_PINBLOCK_FORMAT_ANSI     0
#define DM_ENCRYPTOR_PINBLOCK_FORMAT_DIEBOLD  1


// Constants for Misc Indicators
#define DM_MISC_MEDIA_INDICATOR1	 1 
#define DM_MISC_MEDIA_INDICATOR2	 2 
#define DM_MISC_MEDIA_INDICATOR3	 3 
#define DM_MISC_REMOTE_RELAY		 4 
#define DM_MISC_REMOTE_STATUS1	 5 
#define DM_MISC_REMOTE_STATUS2	 6 
#define DM_MISC_REMOTE_STATUS3	 7 
#define DM_MISC_MEDIA_INDICATOR4  8
#define DM_MISC_SELFCHECKOUT_MODE 9

#define DM_MISC_I_MIS_LED_1     11   // Printer
#define DM_MISC_I_MIS_LED_2     12   // Coin Acceptor
#define DM_MISC_I_MIS_LED_3     13   // Coin Dispenser
#define DM_MISC_I_MIS_LED_4     14   // Cash Dispenser
#define DM_MISC_I_MIS_LED_5     15   // MSR
#define DM_MISC_I_MIS_LED_6     16   // Money Order Printer
#define DM_MISC_I_MIS_LED_7     17   // Aux 1 - Unused
#define DM_MISC_I_MIS_LED_8     18   // Aux 2 - Unused

// Constants for Misc States
#define DM_MISC_STATE_UNKNOWN -1
#define DM_MISC_STATE_OFF		 0 	
#define DM_MISC_STATE_ON			 1 
#define DM_MISC_STATE_QUARTER_HZ  2 
#define DM_MISC_STATE_HALF_HZ	 3 
#define DM_MISC_STATE_ONE_HZ		 4 
#define DM_MISC_STATE_TWO_HZ		 5 
#define DM_MISC_STATE_FOUR_HZ	 6 

#endif

//Constants for SigCap States
#define DM_SIGCAP_NOTIFY_START				1
#define DM_SIGCAP_NO_NOTIFICATION			0

// Method related constants
#define DM_MAX_DEVINFO_LEN        100 



#if defined (_SCOT) || defined (_SCOTSSF_)	

// DeviceAvailability Status
#define DM_DA_NOT_CONFIGURED			_T('0')
#define DM_DA_LOADED					_T('1')
#define DM_DA_CONFIGURED_BUT_NOT_LOADED _T('2')

//CheckHealth
#define DM_CH_INTERNAL					1		
#define DM_CH_EXTERNAL					2		//When used with ATM devices, device errors should be cleared
#define DM_CH_INTERACTIVE				3	
#endif

#if defined (_SCOT) || defined (_SCOTSSF_)	

// Card Dispenser Status
#define DM_CD_HOPPER_EMPTY  			_T('1')
#define DM_CD_HOPPER_NEAR_EMPTY         _T('2')
#define DM_CD_HOPPER_DOOR_OPEN          _T('3')
#define DM_CD_CARD_REMOVED              _T('4')
#define DM_CD_CARD_AT_GATE              _T('5')
#define DM_CD_CARD_IN_TRANSPORT         _T('6')

#endif



#endif
