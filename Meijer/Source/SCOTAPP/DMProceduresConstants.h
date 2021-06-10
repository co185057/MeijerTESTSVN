
#ifndef _DMProceduresConstants
#define _DMProceduresConstants

#define DM_STATUS       103



//TAR 195179 & 191490 start
#define TEMP_SIGNATURE_DIRECTORY		_T("%DATA_DRIVE%\\scot\\data\\tempprint\\")
//TAR 195179 & 191490 end
//#define _K580 _T("K580")
#define _K590REL2 _T("K590REL2")
#define _K590REL3 _T("K590REL3")
#define _PRINTER_F306 _T("PRINTER_F306")
#define _CASHACCEPTOR_CASHFLOW _T("CashFlow66")
#define _BAGSCALE_FLINTEC _T("BAGSCALE_FLINTEC")
#define _BAGSCALE_TEDEAHUNTLEIGH _T("BAGSCALE_TEDEAHUNTLEIGH")
//-dp185016
#define _CASHDISPENSER_GLORY _T("GLORY")
// TAR 354761 - Make this constant match what is returned by GetDeviceName().
#define _CASHDISPENSER_SAFEPAY _T("SAFEPAY")

#define _HARDWARE_2x _T("HARDWARE_2x")
#define _HARDWARE_3x _T("HARDWARE_3x")
#define _HARDWARE_4x _T("HARDWARE_4x")
#define _HARDWARE_5x _T("HARDWARE_5x")

#define _COINACCEPTOR_2x _T("_COINACCEPTOR_2x");
#define _COINACCEPTOR_3x _T("CS435")
#define _COINACCEPTOR_4x _T("CF9520")
#define _COINRECYCLER_BCR _T("BCR")	//SR742
//vt185024 - SRxxx - Add support for BCR old cover in 5.0
#define _COINRECYCLER_BCR_OLDCOVER _T("BCR_OLDCOVER")

#define _COINDISPENSER_TELEQUIP _T("Telequip")
#define _CASHDISPENSER_BNR _T("BNR")
#define _CASHDISPENSER_563X _T("563X")
#define _CASHDISPENSER_5633 _T("5633")   // SSCOP-3348
#define _COINDISPENSER_TELEQUIPHOPPER _T("TelequipHopper")	

#define _PINPAD_2X _T("PINPAD_2X")      //tar 310198
#define _PINPAD_3X4X _T("PINPAD_3X4X")  //tar 310198
#define _PINPAD_5X _T("PINPAD_5X") 
#define ERROR_SIGCAP_NO_COMM 260
#define NCR_DIGITAL_INTERFACE_ERROR 261
#define NCR_TRILIGHT_ERROR 262
#define NCR_MOTION_SENSOR_BLOCKED 5     // SSCOADK-3539


#define IDC_SCOTDEVICEMANAGER1          1004
#define IDC_SCALE1                      1069

#define LEVEL2_HC_ON_CASHCHANGER        0x01

// TAR 353244 - Define number of supported cassettes.
#define DMCASSETTE_COUNT 4

#define DEVICEID0 0
#define DEVICEID1 1
#define WM_DISABLE_DEPOSIT      (WM_USER + 2)

// TAR 355933:  msec time lag before real disable of recycler.
#define RECYCLER_DISABLE_TIMEOUT 1750	

typedef  enum
{
	PRN_BITMAPTOK,
	PRN_BARCODETOK,
    PRN_SETPROPTOK,
	PRN_NOTOKEN
} PRNOBJECTTOKENS;



#define TOKEN_END		_T('\x0F')
#define BARCODETOK_START	_T('\x04')
#define BARCODETOK_END		TOKEN_END
#define BMPTOK_START		_T('\x05')
#define BMPTOK_END			TOKEN_END
#define SETPROPTOK_START    _T('\x06')
#define SETPROPTOK_END      TOKEN_END

#define DM_FS _T('~')         //Unicode and Japan requires us not to use E1, which
                              //is used to combine Kanji characters.

#endif //_DMProceduresConstants
