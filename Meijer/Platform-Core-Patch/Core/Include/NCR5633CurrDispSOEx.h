/*************************************************************************
 *
 *  NCR5633CurrDispSoEx.h:  NCR5633CurrDispSo header file for applications.
 *
 *   NCR5633CurrDispSo DirectIO/Extended result code header file
 *
 * 
 * $Date: 10/29/08 4:31p $
 *
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 2002.  All rights reserved.
 *
 *************************************************************************/

// Direct I/O commands

// Device must be Opened, Claimed and Enabled. 
// Returns version string from the device in the BSTR* argument
// long* argument not used
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_NOTCLAIMED if the device is not claimed
// Returns OPOS_E_ILLEGAL if the command is not supported

const long NCRDIO_5633SO_FIRMWARE   = 1201;

// Device must be Opened, Claimed and Enabled. 
// Returns model string for the device in the BSTR* argument ("5633" or "5635")
// Returns long value in the long* argument to represt true or false (0 or 1)
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_NOTCLAIMED if the device is not claimed
// Returns OPOS_E_ILLEGAL if the command is not supported

const long NCRDIO_5633SO_5635PRESENT   = 1202;

// Device must be Opened, Claimed and Enabled. 
// Returns cash list for the device in the BSTR* argument (;USD:100,USD:500,USD:1000)
// long* argument not used
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_NOTCLAIMED if the device is not claimed
// Returns OPOS_E_ILLEGAL if the command is not supported

const long NCRDIO_5633SO_CURRENCY_MAP     = 1203;


///////////////////////////////////////////////////////////////////////////////
//	Device Specific Extended Error Codes - added by .rvl
////////////////////////////////////////////////////////////////////////////////

const long NCR_E5633_PURGE_OUT				        	= OPOSERREXT+2;
const long NCR_E5633_CASS_OUT					        = OPOSERREXT+3;
const long NCR_E5633_PICK_SENSOR				      	= OPOSERREXT+4;
const long NCR_E5633_FEED_SENSOR			      		= OPOSERREXT+5;
const long NCR_E5633_EXIT_SENSOR			      		= OPOSERREXT+6;
const long NCR_E5633_BILL_JAM				         	= OPOSERREXT+7;

const long NCR_E5633_MISTRACKED_NOTE_AT_FEED		  	= OPOSERREXT+8;
const long NCR_E5633_MISTRACKED_NOTE_AT_DBL_DTCT        = OPOSERREXT+9;
const long NCR_E5633_MISTRACKED_NOTE_AT_EXIT		    = OPOSERREXT+10;
const long NCR_E5633_NOTE_TOO_LONG_AT_EXIT		        = OPOSERREXT+11;
const long NCR_E5633_BLOCKED_EXIT         		        = OPOSERREXT+12;      
const long NCR_E5633_TOO_MANY_NOTES		                = OPOSERREXT+13;
const long NCR_E5633_TIMING_WHEEL_FAILED		        = OPOSERREXT+14;
const long NCR_E5633_DIVERTER_ERROR		                = OPOSERREXT+15;
const long NCR_E5633_REJECT_RATE_EXCEEDED		        = OPOSERREXT+16;
const long NCR_E5633_SUSPECT_EXIT_ACCOUNTANCY		    = OPOSERREXT+17;
const long NCR_E5633_FEED_FAILURE		   	            = OPOSERREXT+18;
const long NCR_E5633_CASS_OUT_ONE		   	            = OPOSERREXT+19;
const long NCR_E5633_CASS_OUT_TWO		   	            = OPOSERREXT+20;
const long NCR_E5633_CASS_OUT_THREE		   	            = OPOSERREXT+21;
const long NCR_E5633_CASS_OUT_FOUR		   	            = OPOSERREXT+22;
const long NCR_E5633_FEED_FAILURE_ONE		   	        = OPOSERREXT+23;
const long NCR_E5633_FEED_FAILURE_TWO		   	        = OPOSERREXT+24;
const long NCR_E5633_FEED_FAILURE_THREE		   	        = OPOSERREXT+25;
const long NCR_E5633_FEED_FAILURE_FOUR		   	        = OPOSERREXT+26;
const long NCR_E5633_PICK_SENSOR_ONE		   	        = OPOSERREXT+27;
const long NCR_E5633_PICK_SENSOR_TWO		   	        = OPOSERREXT+28;
const long NCR_E5633_PICK_SENSOR_THREE		   	        = OPOSERREXT+29;
const long NCR_E5633_PICK_SENSOR_FOUR		   	        = OPOSERREXT+30;
const long NCR_E5633_COMMUNICATION_FAILURE		   	    = OPOSERREXT+31;
const long NCR_E5633_INTERNAL_FAILURE		   	        = OPOSERREXT+32;



