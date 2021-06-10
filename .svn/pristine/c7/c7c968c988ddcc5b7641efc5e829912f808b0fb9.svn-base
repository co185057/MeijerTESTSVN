/*************************************************************************
 *
 *  NCRSankyoPCRW.h: Sankyo header file for the Scot Applications.
 * 
 *************************************************************************
 *
 * Copyright (c) NCR Corporation 2005.  All rights reserved.
 *
 *************************************************************************/
#include "opos.h"
/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Sankyo Card RWD device
/////////////////////////////////////////////////////////////////////
const long NCR_ECARD_BASE    = OPOSERREXT+100;


const long NCR_ECARDNDISP_ALARM_ERROR    =NCR_ECARD_BASE+1;
const long NCR_ECARDDISP_OFFLINE_ERROR   =NCR_ECARD_BASE+2;

/////////////////////////////////////////////////////////////////////
// "DirectIO" Method Constants for Sankyo device
/////////////////////////////////////////////////////////////////////

//Move card from hopper to the SCT 
const long NCRDIO_CHAN_DISPENSE_CARD          = 1801;

//Get device firmware
const long NCRDIO_CHAN_FIRMWARE_VERSION       = 1802;

//Move card from the SCT to reject stacker 
const long NCRDIO_CHAN_TREJECT_STACKER        = 1803;

//Move card all the way out of the gate. This feature will be used
//by Waterloo and engineering for burn-in test and to remove cards 
//from the hopper wihout having someone to pull them out.

const long NCRDIO_CHAN_REJECT_OUT             = 1804;

//To obtain sensor voltages level
const long NCRDIO_CHAN_SENSOR                 = 1805;

//To re-insert card from the gate back to device
const long NCRDIO_CHAN_RETRY                  = 1806;

//To obtain the card current position
const long NCRDIO_CHAN_CARD_POSITION         = 1807;

//Forced the SO to read data from tracks

const long NCRDIO_CHAN_FORCED_READ           = 1808;

//To set track format                       
const long NCRDIO_CHAN_SET_FORMAT            = 1908;

//To get track format 
const long NCRDIO_CHAN_GET_FORMAT            = 1909;

//To write formats to device             
const long NCRDIO_CHAN_WRITE_FORMATS         = 1910;
//To get hopper status
const long NCRDIO_CHAN_GET_HOPPER_STATUS     = 1911;

//To obtain the number of cards in the reject-stacker.
//The return value is a logical count
const long NCRDIO_CHAN_GET_REJECT_STACKER     = 1912;