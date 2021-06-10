

#ifndef __SankyoCPConst_h_included__
#define __SankyoCPConst_h_included__

#include "opos.h"

/// Track number

#define DEVICE_TRACK_ONE    1
#define DEVICE_TRACK_TWO    2
#define DEVICE_TRACK_THREE  3
#define DEVICE_WRITE_ABIITY 99
////////////////////////////////////////
/// Formats

#define TRACK_FORMAT_ABA   0x32
#define TRACK_FORMAT_IATA  0x33
#define TRACK_FORMAT_MINTS 0x34
#define TRACK_FORMAT_BINARY_210BPI  0xF1
#define TRACK_FORMAT_BINARY_105BPI  0xF5
#define TRACK_FORMAT_BINARY_SPECIAL_DENSITY 0xFA
#define TRACK_FORMAT_UNUSED 0x30

#define SANKYO_EXTENDED_RESULT_OFFSET 0x12C //Start at 300 


        //Card position status code

#define SANKYO_USER_MT_CRD_STAT_1 0x1 //No card detected within SCT and gate
#define SANKYO_USER_MT_CRD_STAT_2 0x2 //Card locates at card gate
#define SANKYO_USER_MT_CRD_STAT_3 0x4 //Card locates inside SCT (Transport) 

        //Hopper status
#define SANKYO_USER_MT_CRD_HP_STAT_1 0x41  //Card locates at the hopper lead line
#define SANKYO_USER_MT_CRD_HP_STAT_2 0x42  //Card locates at hopper and MCRW gate
#define SANKYO_USER_MT_CRD_HP_STAT_3 0x44  //Card locates at lead line to reject-stacker
#define SANKYO_USER_MT_CRD_HP_STAT_4 0x48  //Card locates at reject-stacker
#define SANKYO_USER_MT_CRD_HP_STAT_5 0x50  //Card locates at hopper
#define SANKYO_USER_MT_CRD_HP_STAT_6 0x60  //Few cards are in the hopper
#define SANKYO_USER_MT_CRD_HP_STAT_7 0x70  //Hopper empty (this is NCR defined value. 
                                           //It is not part of the Sankyo status

        //Error code
#define SANKYO_USER_MT_CRD_ERR_1        0x0  //Invalid command
#define SANKYO_USER_MT_CRD_ERR_2        0x1  //Invalid parameter
#define SANKYO_USER_MT_CRD_ERR_3        0x2  //Command execution is impossible
#define SANKYO_USER_MT_CRD_ERR_4        0x10 //Card jam
#define SANKYO_USER_MT_CRD_ERR_5        0x11 //Shutter failure
#define SANKYO_USER_MT_CRD_ERR_6        0x12 //Sesnor failure
#define SANKYO_USER_MT_CRD_ERR_7        0x13 //Irregular card length (LONG)
#define SANKYO_USER_MT_CRD_ERR_8        0x14 //Irregular card length (SHORT)
#define SANKYO_USER_MT_CRD_ERR_9        0x15 //EEPROM error
#define SANKYO_USER_MT_CRD_ERR_10       0x16 //Card was removed forcibly
#define SANKYO_USER_MT_CRD_ERR_11       0x17 //Jammed error at retrieve
#define SANKYO_USER_MT_CRD_ERR_12       0x18 //SW1 or SW2 error
#define SANKYO_USER_MT_CRD_ERR_13       0x20 //Parity error or clocking pulse error
#define SANKYO_USER_MT_CRD_ERR_14       0x21 //Read error
#define SANKYO_USER_MT_CRD_ERR_15       0x22 //Write error
#define SANKYO_USER_MT_CRD_ERR_16       0x23 //Read error
#define SANKYO_USER_MT_CRD_ERR_17       0x24 //Read error no encode and/or no magnetic stripe
#define SANKYO_USER_MT_CRD_ERR_18       0x25 //Write verify error (quality error)
#define SANKYO_USER_MT_CRD_ERR_19       0x26 //Read error - No postamble bits -keycard
#define SANKYO_USER_MT_CRD_ERR_20       0x27 //Read error - No preamable bits
#define SANKYO_USER_MT_CRD_ERR_21       0x28 //Read error - LRC error
#define SANKYO_USER_MT_CRD_ERR_22       0x29 //Write verify error
#define SANKYO_USER_MT_CRD_ERR_23       0x30 //Power down
#define SANKYO_USER_MT_CRD_ERR_24       0x40 //Card was pulled out during capture
#define SANKYO_USER_MT_CRD_ERR_25       0x45 //MCRW ejected the card forcibly
#define SANKYO_USER_MT_CRD_ERR_26       0x46 //The ejected card has not been withdrawn 
#define SANKYO_USER_MT_CRD_ERR_27       0xB0 //The guide door at hopper is open


///////////////////////////////////////////////////////////////////////////////
//	Device Specific Extended Error Codes
////////////////////////////////////////////////////////////////////////////////

const LONG HOPPER_EMPTY				=SANKYO_EXTENDED_RESULT_OFFSET+1;
const LONG HOPPER_OCCUPIED          =SANKYO_EXTENDED_RESULT_OFFSET+2;

/////////////////////////////////////////////////////////////////////
// "DirectIOEvent" "DirectIO" Parameter Constants
/////////////////////////////////////////////////////////////////////

const LONG SANKYO_DIOE_HOPPER_EMPTY            = 1;
const LONG SANKYO_DIOE_HOPPER_NEAR_EMPTY       = 2;
const LONG SANKYO_DIOE_HOPPER_NORMAL           = 3;
const LONG SANKYO_DIOE_HOPPER_GUIDE_DOOR_OPEN  = 4;
const LONG SANKYO_DIOE_HOPPER_SENSOR           = 5;
const LONG SNAKYO_DIOE_HOPPER_REJECT_STACKER   = 6;

#endif  //End of __SankyoCPConst_h_included__
