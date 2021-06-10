/////////////////////////////////////////////////////////////////////
//
// NCRCCH.H
//
//   Cash Changer header file for OPOS Applications.
//   Shared use with Coin and Bill Disp SO
//
// Modification history
// ------------------------------------------------------------------
// 97-04-22 OPOS Release 1.2                                   Ashish
//
/////////////////////////////////////////////////////////////////////

#if !defined(NCRCCH_H)
#define      NCRCCH_H


#include "opos.h"

/////////////////////////////////////////////////////////////////////
// Property and Parameter constants to be added as required.
/////////////////////////////////////////////////////////////////////
//
// Example
//
// "DispenserStatus" Property Constants
// "StatusUpdateEvent" Event: "Data" Parameter Constants
/////////////////////////////////////////////////////////////////////
//
//	const LONG COIN_STATUS_OK       = 1;
//	const LONG COIN_STATUS_EMPTY    = 2;
//	const LONG COIN_STATUS_NEAREMPTY= 3;
//	const LONG COIN_STATUS_JAM      = 4;
//
/////////////////////////////////////////////////////////////////////
	const LONG CASH_BASE				=0;

/////////////////////////////////////////////////////////////////////
//	Status Codes
///////////////////////////////////////////////////////////////////////
	const LONG CASH_STATUS_OK					   	=CASH_BASE+1;
	const LONG CASH_STATUS_EMPTY				   	=CASH_BASE+2;
	const LONG CASH_STATUS_NEAREMPTY		   		=CASH_BASE+3;
	const LONG CASH_STATUS_JAM					   	=CASH_BASE+4;
	const LONG CASH_STATUS_INOPERABLE				=CASH_BASE+5;
	const LONG CASH_STATUS_FULL						=CASH_BASE+6;  // .rvl not being used by hawk
	const LONG CASH_STATUS_NEARFULL					=CASH_BASE+7;  // .rvl not being used by hawk
	const LONG CASH_STATUS_EMPTYOK					=CASH_BASE+8;
	const LONG CASH_STATUS_JAMOK				   	=CASH_BASE+9;
	const LONG CASH_STATUS_FULLOK				   	=CASH_BASE+10; // .rvl not being used by hawk
	const LONG CASH_STATUS_ASYNC				   	=CASH_BASE+11;
	const LONG CASH_STATUS_TAMPER				   	=CASH_BASE+12; // .rvl not being used by hawk
	const LONG CASH_STATUS_BILLS_TAKEN				=CASH_BASE+13;

	const LONG CASH_STATUS_BILLS_READY				=CASH_BASE+101;	// CMal091407

	// New CASH_STATUS StatusUpdateEvents for Cash Changer SO
	const LONG CASH_STATUS_CASSETTE_CHANGED			= CASH_BASE+14;
	const LONG CASH_STATUS_CASSETTE_MOVED			= CASH_BASE+15;
	const LONG CASH_STATUS_NOTES_CHANGED			= CASH_BASE+16;
	const LONG CASH_STATUS_CASSETTE_OK				= CASH_BASE+17;

////////////////////////////////////////////////////////////////////////////
//	DispenseOrder Codes
//////////////////////////////////////////////////////////////////////////////
	const LONG OPOS_CC_BDO_LOW_TO_HIGH				=CASH_BASE+14;
	const LONG OPOS_CC_BDO_HIGH_TO_LOW				=CASH_BASE+15;

///////////////////////////////////////////////////////////////////////////////
//	Device Specific Extended Error Codes
////////////////////////////////////////////////////////////////////////////////
	const LONG OPOS_ECASH_OVERDISPENSE				=OPOSERREXT+1; // changed value to match
                                                            // those in NCR5633CurrDispSoEx.h
	const LONG OPOS_ECASH_PARTIAL_DISPENSE_COINS	=CASH_BASE+17; // .rvl not being used by hawk
	const LONG OPOS_ECASH_PARTIAL_DISPENSE_BILLS	=CASH_BASE+18; // .rvl not being used by hawk
	const LONG OPOS_ECASH_PARTIAL_DISPENSE_UNKNOWN	=CASH_BASE+19; // .rvl not being used by hawk
	const LONG OPOS_ECASH_NOTES_PURGED				=CASH_BASE+20; // .rvl not being used by hawk

///////////////////////////////////////////////////////////////////////////////
//	Device Specific Extended Error Codes
////////////////////////////////////////////////////////////////////////////////

	const LONG NCR_ECASH_BILLMAX_40					=CASH_BASE+21;  // .rvl not being used by hawk
	const LONG NCR_ECASH_SUSPEND				   	=CASH_BASE+22;  // .rvl not being used by hawk
	const LONG NCR_ECASH_TI_NOTFOUND			   	=CASH_BASE+23;  // .rvl not being used by hawk
	const LONG NCR_ECASH_ULYFAIL				   	=CASH_BASE+24;  // .rvl not being used by hawk
	const LONG NCR_ECASH_COINMAX_25					=CASH_BASE+25;  // .rvl not being used by hawk
    const LONG NCR_ECASH_JAMMED_BIN_1               =CASH_BASE+26;  // bin 1 jammed
	const LONG NCR_ECASH_JAMMED_BIN_2               =CASH_BASE+27;  // bin 2 jammed
	const LONG NCR_ECASH_JAMMED_BIN_3               =CASH_BASE+28;  // bin 3 jammed
	const LONG NCR_ECASH_JAMMED_BIN_4               =CASH_BASE+29;  // bin 4 jammed
	const LONG NCR_ECASH_JAMMED_BIN_5               =CASH_BASE+30;  // bin 5 jammed
	const LONG NCR_ECASH_JAMMED_BIN_6               =CASH_BASE+31;  // bin 6 jammed
	const LONG NCR_ECASH_JAMMED_BIN_7               =CASH_BASE+32;  // bin 7 jammed
	const LONG NCR_ECASH_JAMMED_BIN_8               =CASH_BASE+33;  // bin 8 jammed
	const LONG NCR_ECASH_JAMMED_BINS                =CASH_BASE+34;  // Unable to determine jammed bin number


//////////////////////////////////////////////////////////////////////////////////////

#endif                  // !defined(NCRCCH_H)

