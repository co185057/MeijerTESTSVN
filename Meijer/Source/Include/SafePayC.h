/**
 * SafePayC.h
 *
 * SafePay Cash Recycler header file.
 * Contains constants / enumerations that are used by the FastLane Application.
 * Other constants are available in SafePayApi.h
 */

#ifndef _SAFEPAYC_H
#define _SAFEPAYC_H

// SafePay DirectIO commands 
enum	
{
	SP_DIO_NOTE_TRANSFER = 1,
	SP_DIO_LATCH_CASSETTE,
	SP_DIO_OPEN_COVER,
	SP_DIO_SAFEPAY_STATUS,
	SP_DIO_SAFEPAY_FULLSTATUS,
	SP_DIO_SAFEPAY_MESSAGE, 
	SP_DIO_SAFEPAY_EXTERRTEXT,
	SP_DIO_DISPENSE_LIST,
	SP_DIO_SETTRANTYPE,
	SP_DIO_GETINVENTORYLIST,
	SP_DIO_GETJAMREASON
};

// Error codes that require special handling.
const LONG SFPAY_ERROR_NOTE_DEPOSIT_JAM 		= 221;
const LONG SFPAY_ERROR_COIN_DOOR_OPEN			= 222; // TAR 354761
const LONG SFPAY_ERROR_HOPPER_LOCKED 		    = 224; // TAR 353757
const LONG SFPAY_ERROR_CASSETTE_JAM     		= 225;
const LONG SFPAY_ERROR_NOTE_VALIDATOR_IO_ERROR  = 227;
const LONG SFPAY_ERROR_FULL						= 261;
const LONG SFPAY_ERROR_NOTE_JAM					= 458;

#endif //  _SAFEPAYC_H
