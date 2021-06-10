/*************************************************************************
 *
 *  ScotTelequip.h: Scot Telequip header file for the Scot Applications.
 * 
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 2000.  All rights reserved.
 *
 *************************************************************************/
#include "opos.h"
/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Scot Telequip
/////////////////////////////////////////////////////////////////////
const long NCR_ECOIN_BASE    = OPOSERREXT+50;


const long NCR_ECOINDISP_ALARM_ERROR    =NCR_ECOIN_BASE+1;
const long NCR_ECOINDISP_OFFLINE_ERROR  =NCR_ECOIN_BASE+2;

/////////////////////////////////////////////////////////////////////
// "DirectIO" Method Constants for Scot Telequip
/////////////////////////////////////////////////////////////////////

const long NCRDIO_CHAN_TELEQUIP_REPLENISHED   = 1401;
// Device must be Opened, Claimed and Enabled. The alarm must be off.
// Returns OPOS_E_NOHARDWARE if the device is offline
// Returns OPOS_E_FAILURE if the alarm is on. 
const long NCRDIO_CHAN_GET_FIRMWARE_VERSION   = 1402;
// Device must be Opened, Claimed and Enabled. 
// Returns version # in the long* argument (Ver 2.25 = 225) 
// Returns version string (from the device) in the BSTR* argument 
// Returns OPOS_E_NOHARDWARE if the device is offline
const long NCRDIO_SET_ENHANCED_LOW_COIN_CAPABILITY = 1403;
// Device must be Opened, Claimed and Enabled
// Returns: 1 if ELCC, 0 if NOT ELCC, -99 if not yet checked
// NOTE: ELCC is the Enhanced Low Coin Capability of the Telequip

const long NCRDIO_CHAN_COIN_HOPPER_EMPTY_BIN = 1404;
//Device must be Opened, Claimed, and Enabled. This
//DirectIO will be used by MTS to test empty Coin Hopper
//device. -- WA 
const long NCRDIO_CHAN_COIN_HOPPER_AUDIT_DUMP = 1405;

/////////////////////////////////////////////////////
//Get device serial number
const long NCRDIO_CHAN_SERIAL_NUMBER = 1406;

//Get device manufactured date
const long NCRDIO_CHAN_MANUFACTURED_DATE = 1407;

//Get SO Special Capabilities for Hopper Error Recovery
const long NCRDIO_CHAN_COIN_HOPPER_GET_SO_CAPS = 1408;

// TINA - 
const long NCRDIO_GET_COIN_PROPERTY = 0xFFFF0000;
const long NCRDIO_GET_BILL_PROPERTY = 0xFFFF0001;

